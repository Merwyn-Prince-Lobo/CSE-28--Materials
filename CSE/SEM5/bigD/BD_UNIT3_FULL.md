# BIG DATA — UNIT 3 FULL NOTES (Spark • PySpark • RDD • Spark SQL • Kafka • Scala)

## Contents
0. Exam Cheatsheet
1. Spark Intro + PySpark + DataFrames
2. Spark Architecture, Lazy Execution, DAG, Scheduling
3. RDDs + Transformations, Actions, Persistence, Shared Variables
4. Spark SQL + Catalyst
5. Kafka
6. Scala + Functional Programming

---

# 00 — BD Unit 3 EXAM CHEATSHEET (Spark • PySpark • RDD • Spark SQL • Kafka • Scala)
> Read this last-night-before. Details are in files 01–06.

## 🔥 One-liners
- **Spark** = unified in-memory distributed engine, from UC Berkeley RAD Lab (2009); faster/more general than MapReduce
- **RDD** = Resilient (lineage recovery) Distributed (partitions) Dataset, **immutable**, **lazy**
- **Lineage** = DAG of ops/dependencies → recompute lost partitions
- **Transformation** = lazy, makes new RDD | **Action** = triggers execution, returns result
- **Lazy execution** = nothing runs until an action; DAG built first
- **Narrow dep** = 1 parent partition → ≤1 child partition, no shuffle | **Wide dep** = needs shuffle
- **Stage boundary** = shuffle
- **Driver** plans/schedules, **Executors** run tasks, **Cluster manager** allocates resources (Standalone/YARN/Mesos)
- **SparkContext** = entry point/connection to cluster; **1 per JVM**
- **Py4J** = Python ↔ JVM bridge; data serialized with **cPickle**
- **DataFrame** = RDD + schema (named columns) + optimizer (2015)
- **Catalyst** = Spark SQL optimizer (tree rewrite rules)
- **Kafka** = pub-sub log system that **decouples** producers & consumers (N×M → N+M)
- **Partition** = unit of scaling/parallelism in Kafka; **offset** = position in partition
- **Scala** = OO + functional, static types + inference, runs on JVM

## 🥊 Comparison tables (high-probability questions)

### Hadoop MapReduce vs Spark
| | MapReduce | Spark |
|---|---|---|
| Data flow | disk (HDFS) after every step | **in-memory**, DAG |
| Iteration | no native support, disk each time | **cache/reuse working set** |
| Model | only map + reduce | many ops (map, filter, join, …) |
| Real-time | no (batch only) | streaming supported |
| Storage | HDFS (own) | **none** (uses HDFS/S3/…) |
| Fault tolerance | replication in HDFS | **lineage** recompute |

### map vs flatMap
`map` → 1 output per input. `flatMap` → 0..n outputs per input, flattened (word split).

### reduceByKey vs groupByKey
reduceByKey **combines locally before shuffle** → less data moved → **preferred**. groupByKey shuffles **all** pairs.

### reduce vs fold
fold has a **zero (identity) value**, works on **empty** RDD; reduce **throws** on empty.

### cache vs persist levels
`cache()` = **MEMORY_ONLY**. Others: MEMORY_AND_DISK, *_SER (serialized, less memory/more CPU), DISK_ONLY, *_2 (replicated 2 nodes).

### coalesce vs repartition
coalesce = **reduce** partitions (after filtering); repartition = **increase/decrease**, **always full shuffle**.

### Broadcast vs Accumulator
Broadcast = **read-only** large value cached per machine. Accumulator = **write-only add** (associative + commutative) counters/sums.

### RDD vs DataFrame
RDD = opaque, no schema, no optimizer. DataFrame = named columns/schema + Catalyst optimizer.

### Topic-based vs content-based routing
Topic: subscribe by label (Kafka ✅). Content: subscribe by criteria (Kafka ❌; only **wildcard pattern** on topic names).

### ZooKeeper vs KRaft
ZooKeeper = external coordinator (old). KRaft = built-in consensus among Kafka servers (new, no ZK).

### Java vs Scala (quick)
Scala: fewer lines, type inference, all values objects, operator overloading, traits (can have concrete methods/fields), `val`/`var`, match expressions, lazy keyword. Java: more verbose, interfaces only abstract, no operator overloading, better backward compat.

### Python vs Scala with Spark
Python slower, easy, rich DS libs. Scala faster, harder syntax, weak DS/visualization libs.

## 🧠 Likely long-answer questions
1. **Limitations of MapReduce and how Spark solves them** → file 01 §2–3
2. **Explain RDD, lineage, fault tolerance** → 01 §5, 03 §1–3
3. **The 5 components of an RDD** → 03 §2 (partitions, dependencies, compute fn, preferred locations, partitioner)
4. **Spark architecture (driver/executor/cluster mgr) + ecosystem** → 02 §1, §3
5. **Lazy evaluation + advantages** → 02 §4
6. **DAG, narrow/wide dependencies, stages, scheduling, PageRank** → 02 §5–7
7. **Transformations vs actions with examples** → 03 §4–5
8. **Persistence / storage levels** → 03 §7
9. **Broadcast & accumulators** → 03 §9
10. **DataFrames: need and creation** → 01 §9
11. **Catalyst: 4 phases + example** → 04 §7
12. **Kafka architecture: producers, consumers, topics, partitions, replication, consumer groups** → 05
13. **Controller & broker roles; ZooKeeper vs KRaft** → 05 §9
14. **Why Scala for Spark; functional programming & big data** → 06

## ✍️ Code to memorize
```python
# Word count
sc.textFile("in.txt").flatMap(lambda l: l.split()).map(lambda w:(w,1)).reduceByKey(lambda a,b:a+b).collect()

# Log mining
lines = sc.textFile("hdfs://..."); errors = lines.filter(lambda l: l.startswith("ERROR"))
msgs = errors.map(lambda l: l.split("\t")[2]); msgs.cache(); msgs.count()

# DataFrame: total tax per PAN
df = rdd.toDF("pan","date","tax"); df.groupBy("pan").sum("tax")

# PageRank loop
ranks = contribs.reduceByKey(add).mapValues(lambda r: r*0.85 + 0.15)
```

## ⚠️ Slide quirks / typos (don't get confused)
- Slide 158: `take(4)` example shows `{2,2,3,4}` but **take order isn't guaranteed**
- Slide 269: `def squareOf(x: Int) : x * x` is a typo, should be `= x * x`
- Slide 91 lists **repartition/coalesce as wide**
- Slide 129 titled "Task Assignment" is really **Related Work**

---

# 01 — Spark Intro + PySpark + DataFrames (Slides 1–58)

## 1. What is Apache Spark?
- Open-source **unified engine** for large-scale data processing
- Started **2009**, UC Berkeley **RAD Lab**
- Built for **speed, ease of use, sophisticated analytics**
- Faster, more general alternative to **MapReduce**
- Used by Netflix, Yahoo, eBay: multiple **petabytes** on clusters of **8000+ nodes**

## 2. Why Spark? MapReduce weaknesses

**Programming model**
- Hard to express every computation as map + reduce
- Simple tasks need **multiple chained MR jobs** (e.g. word count sorted by frequency)
- Little control over data types/structure
- **No native iteration** → every iteration reads/writes **HDFS (disk)** → disk I/O dominates

**Efficiency pain points**
- High communication cost: map → shuffle → combine → reduce
- Frequent writes of **intermediate output to disk**
- Main memory barely used
- Not workable for **real-time stream** processing
- Every job **re-scans entire input**

## 3. What Spark does differently
| Idea | Meaning |
|---|---|
| **Acyclic data flow** | Computation = **DAG** of transformations, combined into one plan → less intermediate data movement + disk I/O |
| **Reuse working set** | Keep frequently used data **in memory**, reuse across computations/iterations (e.g. **PageRank** reuses the rank vector) |

Benefits: less disk I/O, less data movement, faster iterative computation.
Good for: **graph processing, ML, interactive analysis (Python/R)**.

## 4. In-memory computation
- **Iterative processing**: data in RAM, each iteration reads from memory and writes back to memory (vs MR: persistent store every time)
- **Interactive processing**: cache data once, run many different queries on it
- Good fit for **real-time risk management & fraud detection**
- Two pillars: **RAM storage + parallel distributed processing**; cheap RAM makes it practical

**Challenges of in-memory**
1. How to distribute data among DRAM of the cluster?
2. What if memory is not sufficient?
3. How to handle failures since memory is **volatile**?
→ Answer to all: **RDD** (partitions + lineage + spill/persistence levels)

## 5. RDD (intro)
- **R**esilient = recovers lost data using **lineage**
- **D**istributed = partitioned across machines
- **D**ataset = collection being processed
- **Immutable**: ops create new RDDs, never modify existing
- Can be kept **in memory** for reuse
- **Lineage** = DAG tracking: operations on RDD, RDDs produced, dependencies
  - e.g. `Input RDD → filter() → map() → reduceByKey() → Output RDD`
  - Lost partition → **recompute** from lineage (no extra copy stored)
- Ops are two kinds: **Transformations** (new RDD) and **Actions** (trigger computation, return result)

### Log mining example (very common exam Q)
```python
lines      = spark.textFile("hdfs://...")      # base RDD (in-memory struct, collection of strings)
errors     = lines.filter(startsWithERROR())   # transformed RDD: keep only ERROR lines
messages   = errors.map(split("\t"), 2)        # apply func to each element → new RDD
cachedMsgs = messages.cache()                  # keep in memory, will be reused
cachedMsgs.filter(containsfoo()).count()       # action: counts objects
cachedMsgs.filter(containsbar()).count()
```
- **Driver** ships tasks to **Workers**; each worker holds a **Block** of the file and its own **Cache**
- 1st action reads from HDFS + fills cache; later actions hit cache (fast)

## 6. Word count
**Scala**
```scala
val sc = new SparkContext(new SparkConf().setAppName("Spark Count"))
val tokenized  = sc.textFile(args(0)).flatMap(_.split(" "))
val wordCounts = tokenized.map((_, 1)).reduceByKey(_ + _)   // can also use countByKey
```
**PySpark**
```python
text_file   = sc.textFile("input.txt")                      # or hdfs://localhost:9000/user/username/input.txt
words       = text_file.flatMap(lambda line: line.split())
word_pairs  = words.map(lambda word: (word, 1))
word_counts = word_pairs.reduceByKey(lambda a, b: a + b)
word_counts.collect()
```
Each step creates an RDD (data value that can be kept in memory and reused).

## 7. Spark use cases (from slide image)
- **Twitter sentiment analysis** (trending topics → campaigns; sentiment → crisis mgmt, service adjusting, target marketing)
- **NYSE: real-time analysis of stock market data**
- **Banking: credit card fraud detection**
- **Genomic sequencing**

---

## 8. PySpark
- **Python API** released by Apache Spark community to support Python with Spark
- Easily work with RDDs in Python; Python has rich libs; most data scientists use Python

**Features (5)**
1. **Real-time computations** – in-memory → low latency
2. **Polyglot** – Scala, Java, Python, R
3. **Caching & disk persistence**
4. **Fast processing**
5. **Works well with RDDs** – Python is dynamically typed

**Modes**: `pyspark` shell = **interactive**; PySpark scripts = **batch**.

### Setup (commands)
```bash
sudo tar -xzf spark-3.3.4-bin-hadoop3.tgz -C /opt/spark --strip-components=1
echo 'export SPARK_HOME=/opt/spark' >> ~/.bashrc
echo 'export PATH="$SPARK_HOME/bin:$SPARK_HOME/sbin:$PATH"' >> ~/.bashrc
echo 'export PYSPARK_PYTHON=/usr/bin/python3.10' >> ~/.bashrc
echo 'export PYSPARK_DRIVER_PYTHON=/usr/bin/python3.10' >> ~/.bashrc
source ~/.bashrc
pyspark        # starts the interactive shell
```
(PySpark is bundled with Spark but needs env vars: Spark path + Py4j path.)

### SparkContext
- Main **entry point** for Spark functionality = connection to Spark cluster
- Starts/cancels jobs, talks to **cluster manager (YARN etc.)**, is the "master" of your Spark app
- Used to create **RDDs and broadcast variables**
- At least **master** and **app name** must be set (params or conf)
- Created **programmatically**: `SparkContext.getOrCreate()` (Spark v2+), or **interactively** when you start `spark-shell`
- **One SparkContext per JVM**

### PySpark architecture
- Q: SparkContext is per JVM → who handles it for Python?
- **Py4J** = bridge between Python and Java. Python interpreter can access Java objects in JVM and call methods as if they were Python methods
- Python driver → **Py4J** → **Java SparkContext** (drives the application)
- Data must be **serialized using cPickle** serializer between Python and JVM

### Lifecycle of a Spark program
1. Create input RDDs (external data or `parallelize` a collection in driver)
2. **Lazily transform** (filter, map…) to define new RDDs
3. `cache()` intermediate RDDs that will be reused
4. Launch **actions** (`count()`, `collect()`) → kicks off parallel computation, optimized + executed
5. Until an action is seen, **only DAG is built by master – no real work**

### Two ways to create an RDD
1. Load an **external dataset**
2. **Distribute a collection** of objects (`parallelize`)

```python
rdd = sc.parallelize(["Hello world", "Hello Spark", "Hello PySpark"])

from pyspark.sql import SparkSession
spark = (SparkSession.builder
         .appName("Python Spark create RDD example")
         .config("spark.some.config.option", "some-value")
         .getOrCreate())
myData = spark.sparkContext.parallelize([(1,2),(3,4),(5,6),(7,8),(9,10)])
myData.collect()

Employee = spark.createDataFrame(
    [('1','Joe','70000','1'),('2','Henry','80000','2')],
    ['Id','Name','Sallary','DepartmentId'])
```

---

## 9. DataFrames
**Need**: RDDs are **opaque** to Spark (can't parse values/structure) → want SQL-like queries.

**What is it**
- Introduced **2015**; inspired by **R data frames & Pandas**
- **Distributed collection of data into named columns**
- Abstraction **over RDD**: lets you define a **schema** on an RDD
- Has a built-in **optimizer** for queries

**Create**
```python
df = spark.read.json("file:///home/.../students.json")   # from JSON
df.show()
df = rdd.toDF("USN", "name")                              # from existing RDD, naming columns
```
(also from Hive tables / other data sources)

**Use**
```python
df.printSchema()                       # tree format: usn long, name string, marks long
df.select("name").show()
df.select("name", df.marks + 1).show()
```

**Exercise: total tax paid per PAN holder** (CSV: pan number, date, tax_paid)
- **RDD way**: split each line, map to `(pan, tax)`, `reduceByKey(add)`
- **DataFrame way** (uses column **names**, no manual splitting):
```python
df = rdd.toDF("pan number", "date", "taxpaid")
df.select("pan number", "taxpaid").groupBy("pan number").sum()
```

---

# 02 — Spark Ecosystem, Architecture, Lazy Execution, DAG, Scheduling (Slides 59–94, 250–251)

## 1. Spark ecosystem (layers)
```
Libraries / DSL :  Spark SQL | Spark Streaming | MLlib | GraphX
Spark Core      :  Compute engine  +  Core APIs (Structured / Unstructured)
Storage + Cluster Manager (pluggable, NOT built in)
```
**Storage & cluster manager**
- Spark = distributed **processing** engine; **no built-in** cluster resource manager or distributed storage
- Cluster manager: **YARN, Mesos, Kubernetes** (also Spark standalone)
- Storage: **HDFS, Amazon S3, Google Cloud Storage, Cassandra FS** etc.

**Spark Core** = 2 parts
1. **Compute engine**: memory management, task scheduling, fault recovery, **talks to cluster manager + storage**; executes/manages Spark jobs
2. **Core APIs**
   - **Structured API** → DataFrames, Datasets (optimized for structured data)
   - **Unstructured (low-level) API** → RDDs, Accumulators, Broadcast variables
   - Available in Scala, Python, Java, R

**Libraries & DSL**
| Library | What it does |
|---|---|
| **Spark SQL** | structured data; view datasets as relational tables; define schema; run SQL; sources: Hive, Parquet, JSON |
| **Spark Streaming** | streaming data (e.g. web server logs); high-throughput + fault-tolerant; **DStream** = stream of datasets from time intervals (1 sec, 1 min…); joins on unbounded streams impossible → join **within a time window** or **stream with static dataset** |
| **MLlib** | scalable ML: basic statistics, classification (Naive Bayes, decision trees, LR), clustering (k-means, Gaussian mixture); all scale out |
| **GraphX** | graph processing; **directed multigraph**, properties on each vertex+edge, RDDs for edges and vertices; operators like `subgraph`, `mapVertices` |

## 2. Four styles of data processing
1. **Batch** – large datasets, big map-reduce style jobs
2. **Streaming** – near real-time incoming data
3. **Iterative** – ML (e.g. **gradient descent**), repeated data access until convergence
4. **Interactive** – data exploration, data in memory, very quick response

## 3. Architecture: Driver, Executors, Cluster Manager
| Component | Job |
|---|---|
| **Driver process** ("Master") | maintains app info; responds to user program/input; **analyzes, distributes, schedules** work |
| **Executors** | **execute code** assigned by driver; **report state** back to driver |
| **Cluster manager** | controls physical machines, **allocates resources**; one of **Standalone, YARN, Mesos** |

Spark app = **1 driver + N executors**.

## 4. Lazy execution
- Hadoop: master starts executing job on submit. **Spark: driver executes NOTHING until an action.**
- Transformations are just **noted for lineage**.
- Flow: `Disk → T1 → T2 → A1` → on action, all transformations run **block by block on the same server**, then the action.
- Example: `lines = spark.textFile(...)` doesn't read file, just notes RDD to be created.

**Advantages (4)**
| | |
|---|---|
| **Manageability** | organize program into small ops; groups ops → fewer passes over data |
| **Saves computation, increases speed** | only necessary values computed; saves driver↔cluster trips |
| **Reduces complexity** | time + space; can work with **infinite data structure**; action triggered only when data needed |
| **Optimization** | fewer queries; Spark optimizes the whole plan |

## 5. DAG representation
- Driver converts program → **DAG**
- **Each RDD = node**, **transformations/actions = edges**

### PageRank in Spark (know this)
```python
lines = textfile("urls.txt")
links = lines.map(lambda urls: urls.split()).groupByKey().cache()
ranks = links.map(lambda url_neighbors: (url_neighbors[0], 1.0))
for iteration in range(MAXITER):
    contribs = links.join(ranks).flatMap(lambda x: computeContribs(x))
    ranks = contribs.reduceByKey(add).mapValues(lambda rank: rank*0.85 + 0.15)
```
- `computeContribs`: each URL sends `rank / num_neighbors` to each neighbor
- **0.85 / 0.15** = damping factor
- `links` is `cache()`d because reused every iteration
- Q: `links` and `ranks` are spread across nodes — how does **join** work? → **partitioning**
  - Spark supports **hash** and **range** partitioning
  - **Copartition** `links` & `ranks` (e.g. hash both on URL) → **avoids shuffle** on each join

## 6. Narrow vs Wide dependencies
| | **Narrow** | **Wide** |
|---|---|---|
| Definition | each parent partition used by **at most one** child partition | multiple child partitions depend on a parent partition |
| Shuffle? | **No**; can **pipeline** | **May need shuffle** |
| Ops (slide 91) | map, flatMap, mapPartitions, filter, sample, union | intersection, distinct, reduceByKey, groupByKey, join, cartesian, repartition, coalesce |
- **Shuffle** = movement of data from one node to another
- Follow the slide's classification in exams. (FYI: in real Spark `coalesce` without shuffle is actually narrow, but the slide lists it as wide.)

## 7. Scheduling
- **Break DAG into stages at shuffle boundaries**; **within a stage computation is local** (pipelined)
- Example DAG: Stage 1 (groupBy), Stage 2 (map, union), Stage 3 (join)
- Scheduler assigns tasks using **data locality** via **delay scheduling**:
  - if partition is **in memory on a node** → send task to that node
  - else if RDD has **preferred locations** (e.g. HDFS file block) → send to those nodes

---

## 8. Hadoop issues (slide 250) — why Spark
- **Small files problem**: HDFS block default **128 MB**; too many small files overload **NameNode**
- **Slow processing**: MR distributes + processes over cluster (disk, shuffle)
- **Batch only**, no real-time/stream processing
- **No delta (iterative) iteration**: no cyclic data flow

## 9. Spark vs Hadoop (slide 251)
- Hadoop uses **MapReduce**; Spark uses **RDDs**
- Hadoop has **HDFS** (scalable distributed storage); **Spark has no storage**, mainly computation (often on top of Hadoop)
- Spark **doesn't need Hadoop** to run, but can create distributed datasets from **HDFS** files

---

# 03 — RDDs in Detail + Transformations, Actions, Persistence, Shared Variables (Slides 95–172)

## 1. What is an RDD (details)
- **Partitioned, locality-aware, distributed collection**
- **Immutable** (slide asks "why necessary?" → safe sharing across nodes, no sync issues, lineage-based recovery stays valid)
- Either **points to source (HDFS)** or **applies transformations to parent RDDs**
- Computation is **lazy**: lineage DAG of chained RDDs

**Why the RDD abstraction?**
- Support ops beyond map/reduce, in-memory computation, arbitrary composition of operators, simpler scheduling
- Key problem: how to capture **dependencies generically**

## 2. The 5 things every RDD has (**exam favourite**)
1. **Set of partitions** ("splits")
2. **List of dependencies** on parent RDDs *(new vs Hadoop)*
3. **Function to compute a partition** given parents
4. **Optional preferred locations** (data locality)
5. **Optional partitioner** (hash/range; for shuffle)

**RDD interface**
| Operation | Meaning |
|---|---|
| `partitions()` | list of Partition objects |
| `preferredLocations(p)` | nodes where partition p is faster to access |
| `dependencies()` | list of dependencies |
| `iterator(p, parentIters)` | compute elements of partition p given parent iterators |
| `partitioner()` | metadata: hash/range partitioned? |

**Examples**
| | HadoopRDD | FilteredRDD | JoinedRDD |
|---|---|---|---|
| Partitions | one per HDFS block | same as parent | one per **reduce task** |
| Dependencies | none | **1-1** with parent | **shuffle** on each parent |
| Compute | read corresponding block | compute parent and filter | read + join shuffled data |
| Preferred locations | HDFS block location | ask parent (none) | none |
| Partitioner | none | none | **HashPartitioner(numTasks)** |

(Exercise on slide 103 = JoinedRDD → answer is the last column.)

## 3. Fault tolerance
```scala
messages = textFile(...).filter(startsWithERROR()).map(split("\t")(2))
```
HDFS file → (read into memory) **Filtered RDD** → **Mapped RDD**.
If a node dies, Spark replays **only the lost partition** from lineage (re-read block → filter → map). No replication of the data needed.

---

## 4. Transformations (create NEW RDD, lazy)
| # | Transformation | Meaning |
|---|---|---|
| 1 | `map(func)` | apply func to each element → exactly **1 output per input**; input/output types can differ |
| 2 | `filter(func)` | keep elements where func is true (**narrow**, no shuffle) |
| 3 | `flatMap(func)` | each input → **0 or more** outputs (func returns a Seq); e.g. split line into words |
| 4 | `mapPartitions(func)` | runs **per partition**, func: Iterator<T> → Iterator<U> |
| 5 | `mapPartitionsWithIndex(func)` | same + partition **index** (Int, Iterator<T>) |
| 6 | `sample(withReplacement, fraction, seed)` | random sample |
| 7 | `union(other)` | all items of both (**duplicates not removed**) |
| 8 | `intersection(other)` | common elements |
| 9 | `distinct([numTasks])` | remove duplicates |
| 10 | `groupByKey([numTasks])` | (K,V) → (K, Iterable<V>); for aggregation prefer reduceByKey/aggregateByKey |
| 11 | `reduceByKey(func,[numTasks])` | (K,V) → (K,V) values aggregated with func (V,V)=>V |
| 12 | `aggregateByKey(zero)(seqOp, combOp)` | result type **U can differ** from V |
| 13 | `sortByKey([ascending],[numTasks])` | sort by key |
| — | `join(other)` | all pairs with same key |
| — | `keyBy(f)` | make pair RDD; key = f(value) |
| 17 | `pipe(cmd)` | pipe each partition through shell cmd |
| 18 | `coalesce(n)` | **decrease** partitions (use after heavy filtering) |
| 19 | `repartition(n)` | more/fewer partitions, **always full shuffle** |
| 20 | `repartitionAndSortWithinPartitions(partitioner)` | repartition + sort within each partition (more efficient than repartition then sort) |
| — | `groupBy(f)` | key = f(item), value = all items with that key |

### Code examples (outputs)
```python
x = sc.parallelize(["b","a","c"])
x.map(lambda z: (z,1)).collect()            # [('b',1),('a',1),('c',1)]

x = sc.parallelize([1,2,3])
x.flatMap(lambda x: (x, x*100, 42)).collect()   # [1,100,42,2,200,42,3,300,42]
x.filter(lambda x: x%2 == 1).collect()          # [1,3]

x = sc.parallelize(['John','Fred','Anna','James'])
x.groupBy(lambda w: w[0])       # [('J',['John','James']),('F',['Fred']),('A',['Anna'])]
x.keyBy(lambda w: w[0])         # [('J','John'),('F','Fred'),('A','Anna'),('J','James')]

x = sc.parallelize([('B',5),('B',4),('A',3),('A',2),('A',1)])
x.groupByKey()                  # [('A',[3,2,1]),('B',[5,4])]

x = sc.parallelize([1,2,3], 2)                 # 2 = number of partitions
x.glom().collect()                             # [[1],[2,3]]   (glom shows elements per partition)

x = sc.parallelize([("a",1),("b",2)]); y = sc.parallelize([("a",3),("a",4),("b",5)])
x.join(y).collect()             # [('a',(1,3)),('a',(1,4)),('b',(2,5))]

sc.parallelize([1,2,3,3,4]).distinct().collect()   # [1,2,3,4]
sc.parallelize([1,2,3,4,5]).sample(False, 0.4, 42) # sample w/o replacement, 40%, seed 42
```
**map vs flatMap**: `map` → one element out per input; `flatMap` → a list/many elements out, then **flattened**.

### reduceByKey vs groupByKey (**very likely exam Q**)
```scala
val words = Array("one","two","two","three","three","three")
val wordPairsRDD = sc.parallelize(words).map(w => (w, 1))
wordPairsRDD.reduceByKey(_ + _).collect()                       // better
wordPairsRDD.groupByKey().map(t => (t._1, t._2.sum)).collect()  // worse
```
| | **reduceByKey** | **groupByKey** |
|---|---|---|
| Combining | **Map-side combine first** within each partition (local `(a,1),(a,1)`→`(a,2)`), then shuffle partial sums | **No local combine**; shuffles **every (k,v)** pair, then sums after |
| Data shuffled | **Much less** | **Much more** (all raw pairs) |
| Memory | low | may blow up (all values of a key on one node) |
| Use | aggregations (sum/count) | when you really need all values |

Slide diagram: 3 partitions with `(a,1)`s and `(b,1)`s → reduceByKey locally combines to `(a,1),(b,1) | (a,2),(b,2) | (a,3),(b,2)` then shuffle → final `(a,6)`, `(b,5)`. groupByKey shuffles all raw pairs → final `(a,6)`, `(b,5)` computed only after shuffle.

---

## 5. Actions (return value to driver / write output, **trigger execution**)
| Action | Meaning |
|---|---|
| `reduce(func)` | aggregate with func (2 args → 1), must be **commutative + associative**; **throws exception on empty RDD** |
| `fold(zero)(func)` | like reduce + **zero value** (must be **identity**: 0 for +, 1 for ×); works on **empty** collection |
| `aggregate(zero, seqOp, combOp)` | seqOp combines within partition, combOp merges partition results; result type can differ |
| `collect()` | all elements to driver as array; **must fit in driver memory**; good for unit testing |
| `count()` | number of elements |
| `first()` | first element (= `take(1)`) |
| `take(n)` | first n elements; accesses few partitions → **biased, order not guaranteed** |
| `takeSample(withRepl, num, [seed])` | random sample array |
| `takeOrdered(n, [ordering])` | first n by natural order/comparator |
| `top(n)` | top elements by default ordering |
| `countByValue()` | count of each distinct element; e.g. {1,2,2,3,4,5,5,6} → {(1,1),(2,2),(3,1),(4,1),(5,2),(6,1)} |
| `countByKey()` | only on (K,V); returns **hashmap (K, Int)** |
| `max() / sum() / mean() / stdev()` | numeric stats |
| `getNumPartitions()` | number of partitions |
| `saveAsTextFile(path)` | write as text (local/HDFS); calls toString |
| `saveAsSequenceFile(path)` | Hadoop SequenceFile (Java/Scala) |
| `saveAsObjectFile(path)` | Java serialization; load with `SparkContext.objectFile()` |
| `foreach(func)` | run func on each element, for **side effects** (accumulators, external storage) |

```python
x = sc.parallelize([1,2,3,4])
x.reduce(lambda a,b: a+b)                       # 10
seqOp  = lambda data, item: (data[0]+[item], data[1]+item)
combOp = lambda d1, d2: (d1[0]+d2[0], d1[1]+d2[1])
x.aggregate(([], 0), seqOp, combOp)             # ([1,2,3,4], 10)
sc.parallelize([2,4,1]).max()   # 4 ; .sum() = 7 ; .mean() = 2.33 ; .stdev()
sc.parallelize([('J','James'),('F','Fred'),('A','Anna'),('J','John')]).countByKey()
# {'J':2,'F':1,'A':1}
```
**reduce vs fold**: same signature idea, fold takes a **zero value** applied at the start of **each partition** and is **defined for empty** collections; reduce throws on empty.

---

## 6. Pair RDDs (key–value)
- Spark has **separate pair RDDs** + separate operations for them
- **Pair transformations**: `reduceByKey(func)`, `groupByKey()`, `combineByKey(createCombiner, mergeValue, mergeCombiners, partitioner)` (different result type), `mapValues(func)` (**key unchanged**), `flatMapValues(func)` (tokenization), `keys()`, `values()`, `sortByKey()`, `join`
- Example on `{(1,2),(3,4),(3,6)}`: `reduceByKey(x+y)` → `{(1,2),(3,10)}`; `groupByKey()` → `{(1,[2]),(3,[4,6])}`; `mapValues(x+1)` → `{(1,3),(3,5),(3,7)}`; `keys()` → `{1,3,3}`; `values()` → `{2,4,6}`
- **Pair action**: `countByKey()` → HashMap of (k, Int)

---

## 7. Persistence (cache)
- Spark can **persist/cache** dataset **in memory across operations**
- Each node stores the slices it computes, reuses them in other actions → future actions often **>10x faster**
- Cache is **fault-tolerant**: lost partition **recomputed** from lineage
- `cache()` = persist with default **MEMORY_ONLY**

| Storage level | Description |
|---|---|
| **MEMORY_ONLY** (default) | deserialized Java objects in JVM; if doesn't fit, some partitions **not cached, recomputed on the fly** |
| **MEMORY_AND_DISK** | if doesn't fit, **spill to disk** and read from there |
| **MEMORY_ONLY_SER** | **serialized** (byte array per partition); more space-efficient, more CPU |
| **MEMORY_AND_DISK_SER** | like above but spill to disk instead of recompute |
| **DISK_ONLY** | only on disk |
| **MEMORY_ONLY_2, MEMORY_AND_DISK_2 …** | same but **replicate each partition on 2 nodes** |

Cached word count:
```scala
val f = sc.textFile("README.md")
val w = f.flatMap(l => l.split(" ")).map(word => (word, 1)).cache()
w.reduceByKey(_ + _).collect.foreach(println)
```
```python
from operator import add
f = sc.textFile("README.md")
w = f.flatMap(lambda x: x.split(' ')).map(lambda x: (x, 1)).cache()
w.reduceByKey(add).collect()
```

## 8. Related work (slide 129)
| System | Limitation vs RDD |
|---|---|
| DryadLINQ, FlumeJava | similar distributed-collection API but **can't reuse datasets efficiently across queries** |
| Relational DBs | lineage/provenance, logical logging, materialized views |
| GraphLab, Piccolo, BigTable, RAMCloud | **fine-grained writes**, like distributed shared memory |
| Iterative MapReduce (Twister, HaLoop) | implicit data sharing for a **fixed computation pattern** |
| Caching systems (Nectar) | store data in files, **no explicit control** over what's cached |

## 9. Shared variables (2 types)
| | **Broadcast variables** | **Accumulators** |
|---|---|---|
| Purpose | efficiently distribute **large read-only values** | **aggregate** information (counters/sums) |
| How | **read-only** variable **cached on each machine** instead of shipping a copy with every task; efficient broadcast algorithms reduce communication cost | tasks can only **add** via **associative + commutative** op (like MapReduce **counters**) |
| Notes | Spark already auto-broadcasts common data within a stage (cached **serialized**, deserialized before each task). Explicit broadcast useful when **tasks across multiple stages** need same data or **deserialized caching** matters | numeric types by default; **custom accumulator types** supported |

---

# 04 — Spark SQL, DataFrames API, Catalyst Optimizer (Slides 173–200)

Ref paper: Armbrust et al., **"Spark SQL: Relational Data Processing in Spark"**, SIGMOD 2015.

## 1. Why Spark SQL? (structured data processing)
- In RDD API, all **functions/objects are black boxes** → Spark doesn't know the data's internal structure
- Structure has to be coded **inside your functions**: **Parsing** (raw → fields), **Conversion** (cast types), **Processing** (logic)

**Earlier attempts**
| Tool | Note |
|---|---|
| **Pig / Pig Latin** | builds on Hadoop; converts SQL-like programs → MapReduce |
| **Hive / HiveQL** | SQL-like queries (on Hadoop) |
| **Shark** (Hive on Spark) | translates HiveQL → RDD programs; **initial attempt** at SQL on Spark |

**Spark SQL** = **redesigned** for Spark's query model
- Supports all popular **relational operators**
- Can be **intermixed with RDD operations**
- Uses the **DataFrame API** as an enhancement over RDD API

## 2. Data model
- **Nested data model**
- Primitive types: boolean, integer, double, decimal, string, date, timestamp
- Complex types: **structs, arrays, maps, unions**
- **User-defined types**
- **First-class support** for complex types

## 3. Built-in operations
Filter (selection), Select (projection), Join, GroupBy (aggregation), Load/Store in various formats, Cache, RDD ⇄ DataFrame conversion.

## 4. DataFrame operations
- Same ops as SQL (select, where, join, groupBy) written as **method calls**, not SQL text
- e.g. `employees("gender") === "female"` **doesn't evaluate immediately** → builds a **column expression**
- Operators build an **AST (abstract syntax tree)**, which is optimized by **Catalyst**
- Alternative: register DataFrame as **temp table**, query with real SQL strings

### Advantages over plain relational query languages
1. **Holistic optimization** – functions across Python/Scala/SQL optimized together as one plan
2. **Real control structures** – can use `if`/`for` around queries (impossible inside plain SQL string)
3. **Early error detection** – logical plan validated immediately → schema mismatches show **at write-time**, not after a job fails

## 5. Querying native datasets
- Schema inferred from your own classes
  - **Java/Scala**: via **reflection**
  - **Python**: by **sampling rows** (no static types)
  - `case class User(name: String, age: Int)` → schema `{name: String, age: Int}` for free
- Native objects used **directly**, no conversion to separate row format
- Benefits: add relational ops (`.where`, `.groupBy`) onto RDDs of your objects; **join native RDDs with external structured data** (CSV, Parquet, JDBC)
- Enables **columnar storage**, with **hot columns cached in memory**

## 6. UDFs (User-Defined Functions)
- Spark SQL has a **limited built-in function set** → UDFs plug in custom logic
- Defined and **registered inline** in your code
- Contrast: **Pig UDFs** must be a **Java package** loaded separately
- Can work on a **single value or aggregate** across a table
- Once registered, callable from **both DataFrame code and SQL strings**

## 7. Catalyst (query optimizer)
- Represents queries as **trees**; applies **transformations (rules)** to rewrite into faster equivalent plans
- Tree example: `x + (1 + 2)` → `Add(Attribute(x), Add(Literal(1), Literal(2)))` → **constant folding** → `x + 3`

### Catalyst rules
- **Pattern-matching functions** that rewrite subtrees into specific structures
- A rule is a **partial function**: fires only on matched patterns, leaves rest untouched → **adding a new operator never requires editing existing rules**
- Multiple patterns in the same `transform` call
- May take **multiple batches to reach a fixed point**
- `transform` can contain **arbitrary Scala code**

### 4 phases of plan optimization & execution (**exam Q**)
```
SQL query / DataFrame
   → [1 Analysis]            Unresolved Logical Plan → Logical Plan      (uses Catalog)
   → [2 Logical Optimization] Logical Plan → Optimized Logical Plan
   → [3 Physical Planning]    → Physical Plans → (Cost Model) → Selected Physical Plan
   → [4 Code Generation]      → RDDs
```
**Data Frames and SQL share the same optimization/execution pipeline.**

| Phase | Details |
|---|---|
| **1. Analysis** | attribute is **unresolved** if type unknown or not matched to an input table. Resolve by: look up relations by name from **Catalog**; map named attributes to children of operator; **UID** for refs to same value; **propagate & coerce types** (e.g. `1 + col`). Example: `SELECT col FROM sales` |
| **2. Logical optimization** | standard **rule-based** optimization: **constant folding, predicate pushdown, projection (column) pruning, null propagation, boolean expression simplification**. Only ~**800 LOC** |
| **3. Physical planning** | generate physical plans, pick one using **cost model**; e.g. **pipeline projections and filters into a single map** |
| **4. Code generation** | uses Scala **quasiquotes** `q"..."`; Catalyst turns SQL tree into **AST for Scala code** that evaluates the expression; only ~**700 LOC** |

**Predicate pushdown + column pruning example**
```python
events = add_demographics(sqlCtx.load("/data/events", "parquet"))
training_data = events.where(events.city == "Melbourne").select(events.timestamp).collect()
```
Logical plan: `scan(events) , scan(users) → join → filter(city) → select`.
Optimized (slide 193): **predicate pushdown** (filter moved down toward the scan) + **column pruning** (scan only needed columns) → less data read and joined.

**Example Catalyst transformation (slide 194)**: find **filters on top of projections** → check the filter can be evaluated **without the result of the project** → if so, **switch the operators** (push filter down).

## 8. Extensions
- **Data Sources**: implement **`createRelation`** (takes key-value params, returns a **`BaseRelation`**). E.g. CSV, Avro, Parquet, JDBC
- **User-Defined Types (UDTs)**: map user types to structures of Catalyst built-in types

## 9. Schema inference for semi-structured data (JSON)
- Auto-infers schema from records (one pass or sample)
- Represents as a **tree of STRUCT types** (contain atoms, arrays, other STRUCTs)
- Finds **most appropriate type** per field based on **all data observed** in that column; same for array element types
- **Merges schemas** of single records in **one reduce operation**
- Same trick for Python typing

---

# 05 — Apache Kafka: Architecture & Usage (Slides 201–247)

Ref: *Kafka: The Definitive Guide* (Confluent).

## 1. Why data pipelines?
- **Stream processing** = processing **events**. Event = something that happens at a specific time, one at a time (e.g. university gate scans student ID: "Student 23489 entered building")
- Events processed on the server once they arrive
- **Sources**: varied, stream at varied rates, **lightweight, can't compute**. **Analysis on servers** → data must be moved source → server
- Simple approach: **point-to-point connection**
- Problem 1: **multiple sources** (e.g. many cameras → central system)
- Problem 2: **multiple backends**, each processing same data differently (e.g. stock trading: different analysts)
- Result: **maze of connections** → **N clients × M backends = N×M connections** to build + maintain; one failing connection or new service touches everything
- Example backends: **Hadoop, Security, Real-time, Analytics**

## 2. Why Kafka?
- An **intermediary** connecting different sources and multiple backends
- **Decouples** the pipeline: producers and consumers **don't need to know about each other**

## 3. Publish–Subscribe model: 3 components
1. **Publisher** (producer)
2. **Subscriber** (consumer)
3. **Communication infrastructure** (Kafka brokers)

| | Producer / Publisher | Consumer / Subscriber |
|---|---|---|
| Role | decides **what data** to send, packages it into a **message**, publishes to infrastructure | tells infrastructure the **type of messages** it wants; does **not** say *who* from |
| Knows other side? | **No** | **No** |
| Complexity | simplest part | – |

The **communication infrastructure** is the **most complex part**: decides which messages go to which subscriber.

## 4. Routing models
| | **Topic-based** | **Content-based** |
|---|---|---|
| How | publishers send messages with **topic labels**; subscribers subscribe to topics and get **all** messages on it | subscribers define **matching criteria**, get messages that match |
| Example | all fire sensors in B block | ads featuring Virat Kohli |
| Kafka? | **Yes** | **NOT supported** — Kafka supports a simpler **pattern-based** version: **wildcard** on topic names (e.g. `ipl*`) |

**Pros of pub-sub**: no hard-wired connections; flexible (easy to add/remove publishers/subscribers)
**Cons**: design + maintenance of **topics**; **performance overhead** from the infrastructure

**Exercise 1**: pipelines for e.g. camera feeds, stock trades; backends = Hadoop, security, real-time analytics.
**Exercise 2 (bookstore: login, list books, details, buy, order status, return, logout; backends: Security, Order processing, Book info)** → **Topic-based**; each message type can be a topic.

## 5. Topics and messages
- **Topic** = **log data structure**: **ordered, append-only sequence of records stored on disk**. Names like `/shopping-cart-done`, `/user-signups`

**Kafka message fields**
| Field | Purpose |
|---|---|
| **Key** | used for **partitioning** |
| **Value** | actual **payload** |
| **Offset** | **position** of message in the partition |
| **Size** | message length |
| **Checksum** | detects **corruption** |
| **Magic byte** | **format version** |
| **Compression codec** | **Snappy, GZip, LZ4** |
| **Timestamp** | set by **producer** at send time or by **broker** at arrival time (config) |

## 6. Scaling: Partitions
- **Topic** = logical category: unique name, groups related messages, **multi-tenant** (many producers write / consumers read concurrently); divided physically into partitions
- **Partition** = **physical storage unit**: **append-only immutable log** on disk, **distributed across broker nodes** → overcomes **single-server disk/IO bottleneck**
- Each message in a partition gets a **unique offset**
- Example: topic split into 3 partitions — P0 next write at offset 12, P1 at 9, P2 at 12

**How messages are assigned to partitions**
- **Key provided** → `hash(key) % N` → **same key always same partition**
- **No key** → **round-robin / sticky** distribution to balance load

**Consumers track the latest offset they've read**; comparing with the partition's newest offset tells them if new messages are waiting.

## 7. Fault tolerance: Replication
- Exercise 3: how is reliability guaranteed? (hint: HDFS) → **Replication**: each partition copied across multiple brokers
- **Leader** replica: performs **all reads and writes**
- **Followers** replicate the leader
- If leader fails → **new leader chosen** (from in-sync replicas)
- **Durability levels (`ack`)**; sync write confirmed after **ISR (in-sync replicas) quorum**
  - `ack = 1` → **leader only** (no follower ack)
  - `ack = -1 / all` → leader waits for **all ISR** ack
- **Recommended: replicas = 3, quorum = 2**

## 8. Delivery to consumers: Consumer groups
- **Consumer group** = typically **multiple instances of one application**
- A **partition delivers its messages to ONE member of the group** → **load balancing**
- Different groups each get **all** messages (independent)

**Exercise 4** (topic with 4 partitions P0–P3):
- Group A (2 consumers): C1 → P0, P3; C2 → P1, P2 (each gets equal partitions)
- Group B (4 consumers): C3 → P0, C4 → P3, … (one partition each)

**Exercise 5** (1 topic, 3 servers, 3 partitions, 3 replicas/partition, group of 3 consumers):
- Every server holds a **replica of every partition** (P0, P1, P2); each partition has **one leader**, with leaders **spread across the servers** for balance (draw leader replicas marked on the slide diagram)
- Consumers **C0, C1, C2** each assigned **one partition** (reads go to the **leader**)

## 9. Controllers & server roles
- A **Kafka server** = process/machine running Kafka; can have **one or both roles**
| **Controller role** | **Broker role** |
|---|---|
| manages **cluster metadata + quorum state** | **stores data**: persists/manages topic partitions |
| **elects partition leaders** | serves **produce (write) + consume (read)** requests |
| administrative authority | hosts **primary + replica partition logs** |

**Who picks the controller?**
| **ZooKeeper** (old) | **KRaft** (new) |
|---|---|
| **external coordinator** maintains which server is the controller | servers decide **among themselves** via **consensus / leader election** protocol |
| ZooKeeper **mandatory** | **No ZooKeeper** needed |
| older Kafka versions | newer Kafka versions |

## 10. Who uses Kafka
**LinkedIn** (activity data + operational metrics), **Twitter** (part of **Storm** stream processing), **Square** (bus for all system events across data centers → Splunk, Graphite, Esper-like alerting), Spotify, Uber, Tumblr, Goldman Sachs, PayPal, Box, Cisco, CloudFlare, Datadog, LucidWorks, MailChimp, Netflix.

---

# 06 — Scala & Functional Programming (Slides 248–288)

## 1. What is Scala?
- **SCA**lable **LA**nguage, by **Martin Odersky** at **EPFL (Switzerland)**
- Modern **multi-paradigm** language: concise, elegant, **type-safe**; integrates **OO + functional**
- Runs on **JVM** (compiles to **bytecode**), interoperates with Java libs
- Good for web services, **data-intensive apps**

**Object-oriented**: **every value is an object**; types/behaviour described by **classes and traits**; **mixin-based composition** replaces multiple inheritance.
**Functional**: **every function is a value**; anonymous functions, **higher-order functions**, **nested functions**, **currying**; **case classes + pattern matching** (algebraic types); **singleton objects** group non-class functions.
**Statically typed**: generic classes, variance annotations, upper/lower type bounds, inner classes, abstract type members, compound types, self-types, implicit params/conversions, polymorphic methods. **Type inference**: compiler infers types so you needn't annotate.

## 2. Spark vs Scala (don't mix up!)
| Spark | Scala |
|---|---|
| open-source **distributed general-purpose cluster-computing framework** | **general-purpose programming language**: functional + strong static types |
| speeds up Hadoop computation | web apps, streaming data, distributed apps, parallel processing |

## 3. Python with Spark vs Scala with Spark
| Criteria | Python | Scala |
|---|---|---|
| **Performance** | comparatively **slower** | **faster** (Spark is written in Scala, integrates well) |
| **Learning curve** | easy syntax, high-level, productive | **arcane syntax**, hard to learn, but rewarding |
| **Data science libs** | rich libs/visualization; easy to port R parts | **lacks** proper DS libraries/visualization tools |

## 4. Java vs Scala
- Scala very similar to Java: compiled to **bytecode**, executes on **JVM**
- **Fewer lines** than Java (type inference, everything an object, function passing)
- Built-in **lazy evaluation** via `lazy` keyword
- Scala supports **operator overloading**, Java **doesn't**
- Scala treats methods/functions as **variables (values)**
- Scala **syntax more complicated** than Java
- Java **backward compatible**; Scala only **limited backward compatibility**

### Syntax differences
| | Java | Scala |
|---|---|---|
| Class | `public class Person { public final String name; public final int age; Person(String name,int age){...} }` | `class Person(val name: String, val age: Int) {}` |
| Method | `int fun(int x) { return result; }` (no parameterless methods) | `def fun(x: Int): Int = { result }` ; `def fun = result` |
| Variables | `int x = e;` / `final String x = e;` | `var x: Int = e` (mutable) / `val x: String = e` (immutable) |
| Call | `obj.meth(arg)` | `obj.meth(arg)` **or** `obj meth arg` (infix) |
| Choice | `cond ? a : b` (expr), `if` is a **statement**, `switch` is **statement only** | `if (c) a else b` is an **expression**; `expr match { case p1 => e1 ... }` |
| Interface | `interface T` – **no concrete methods, no fields** | `trait T` – **concrete methods + fields allowed** |
| Inheritance | `class C extends Super implements T` | `class C extends Super with T` (**mixin composition**) |
| main | `static` members | `object Example1 { def main(args: Array[String]) }`; arrays `args(i)` not `args[i]`; `Array[String]` not `String[]` |

**Partition by age**
```scala
val people: Array[Person]
val (minors, adults) = people partition (_.age < 18)   // function value + infix call + pattern match
```
(Java needs ArrayLists + loop + toArray.)

## 5. Why Scala (4 selling points)
1. **Minimal verbosity** (Java Person with getters/setters ≈ 15 lines vs `class Person(var firstName: String, var lastName: String, var age: Int)`)
2. **Referential transparency / type inferencing**
3. **Concurrency**
4. **Functional programming**

## 6. Type inference
- Java: must declare every type; errors caught by compiler (good) but verbose
- Python/Ruby: no declarations, easier to write, harder to debug
- **Scala: statically typed + infers types** → less typing, errors caught at compile time. Downside: more kinds of error messages
```scala
val collegeName = "PES University"     // inferred String
def squareOf(x: Int) = x * x           // return type inferred Int  (slide has a typo with ':')
```
- **Parameters must be explicitly typed** (strict type safety); **return type** may be inferred
- If you need `Float` return, **declare it explicitly** — otherwise inferred return would be **Int**
- **Generic functions**: operations limited to the generic type; mismatched types → returns False (slide example)

## 7. Consistency
- Java: values are objects **unless primitive**
- **Scala: all values are objects**; compiler turns them into primitives (so no efficiency loss; helper objects like `RichInt`)
- Java has operators + methods (different syntax); **Scala: operators are just methods**, either syntax works

## 8. Concurrency
- **Fine-grained**: frequent interaction between threads (very hard to get right)
- **Coarse-grained**: infrequent interaction between independent sequential processes (much easier)
- Java 5/6: good fine-grained support (**threads**); Scala has **full access to Java API** + **Actors** for **coarse-grained** concurrency (message sending with `!`)
- Best-known functional languages: ML, OCaml, Haskell. Scala is an **"impure" functional language** (functional style possible but not forced)

## 9. Functional programming
- Big concurrency problem = **shared mutable state** (threads reading/changing the same variables)
- If all data **immutable** → any thread can read anytime, **no locks/sync**
- Functional languages use only immutable data and still work
- Functional ops **create new structures**, don't modify old ones (**original list left unchanged**)
- Program implicitly captures **data flow**; **order of operations not significant**
- Functions are **objects**, can be **passed as arguments**, **returned**, and **operate on collections**
- `val` = **immutable** (reassignment not allowed); `var` = **mutable** (reassignment allowed)
- Type safety with generic params

**Quicksort: Java-style vs functional**
- Java-style: focus on **HOW** (explicit iteration, decide when/how to `swap()`)
- Functional: focus on **WHAT**: pick pivot → sort values smaller → sort values larger → **concatenate**
- Self-learning Q: does the given program sort ascending or descending? (xs = 3,1,2,0,7,6,4,5) → reverse the comparison to flip order; parallelize by sorting the two partitions independently.

## 10. Functional programming and Big Data
- Big data uses **parallel disk, memory and CPU** in clusters
- Operations = **independent parallel operations** (like `map()`) + **consolidation** (like aggregation/`reduce`)
- Efficiency needs **concurrency, parallelism, referential transparency** → all **naturally present** in functional environments
- That's why Spark's API (`map`, `filter`, `reduceByKey`, lambdas, immutable RDDs) is functional
