# Big Data – Units 3 & 4: Complete Exam Notes, Practice Sets and Mock Exam

*Exam pattern covered: 40 marks = 4 descriptive × 4 marks (2 per unit) + 4 MCQs × 2 marks (2 per unit) + 16 MCQs × 1 mark (8 per unit).*

## How to use these notes

- Read each unit's **Detailed Notes** first, then the **4-mark model answers**, then the **comparison tables / MCQ traps**, then attempt the **MCQs**, and finally use the **Revision Sheet** the night before.
- The **mock exam** is at the very end; the **answer key and model answers are placed after it** so you can attempt it first.
- Marker legend: **⚠ MCQ TRAP** = commonly confused fact; **★** = high-probability exam point; **\[Supplement\]** = added from standard knowledge because the slides were thin (still within syllabus).

## Syllabus ↔ slide coverage map (important)

| Syllabus topic | Where it is in your slides | Notes |
| --- | --- | --- |
| Unit 3: Spark, Scala/PySpark programming model | Spark Introduction, PySpark Hands-on, Scala deck | Covered in 3.1–3.4 |
| Unit 3: Transformations & Actions, RDD, DataFrames, Wide/Narrow, Architecture | Spark Architecture, RDD, RDD Operations decks | 3.5–3.6, 3.9 |
| Unit 3: Spark SQL, Relational operators with Spark | Spark SQL deck (no separate 'relational operators' deck) | 3.7–3.8; relational-algebra mapping added as \[Supplement\] |
| Unit 3: Streaming algorithms – sampling, Bloom filter, Flajolet-Martin | Found in your **Unit 4 PDF** (Streaming Algorithms 1 & 2) | Placed under **Unit 3** because the syllabus says so (3.10) |
| Unit 4: Streaming analytics use cases | 'Streaming analytics use cases' deck | 4.1 |
| Unit 4: Kafka – use cases, architecture | Found in your **Unit 3 PDF** (Kafka deck) | Placed under **Unit 4** (4.2) |
| Unit 4: Streaming Spark, Flink, MLlib, GraphX, success stories | Unit 4 PDF | 4.3–4.7 |

---

# UNIT 3 — IN-MEMORY COMPUTATION

## 3.1 Apache Spark: Introduction and Motivation

### 3.1.1 What is Apache Spark?

- **Definition:** Apache Spark is an **open-source, unified engine for large-scale data processing**, built for **speed, ease of use and sophisticated analytics**; a faster, more general alternative to MapReduce.
- Started in **2009** as a research project at **UC Berkeley's RAD Lab** (AMPLab).
- Used at scale by Netflix, Yahoo, eBay (multiple petabytes, clusters of 8,000+ nodes – from slides).
- 'Unified' = one engine for **batch, streaming, interactive/SQL, machine learning (MLlib) and graph (GraphX)**.
- Spark is **only a compute engine**: it has **no storage system and no cluster manager of its own** (it plugs into HDFS/S3/Cassandra and YARN/Mesos/Kubernetes/Standalone).

### 3.1.2 Why Spark? – Weaknesses of MapReduce (★ 4-mark favourite)

| Area | MapReduce problem |
| --- | --- |
| Programming model | Hard to express every computation as map + reduce; simple tasks (e.g. word count sorted by frequency) need **several chained MR jobs**; little control over data types/structure |
| Iteration | **No native support for iteration**; every iteration **reads from and writes back to HDFS** → disk I/O dominates runtime |
| Efficiency | High communication cost (map → shuffle → combine → reduce); **frequent writes of intermediate output to disk**; main memory barely used |
| Real-time | **Not workable for real-time stream processing**; every job **re-scans the entire input** |

### 3.1.3 What Spark does differently

1. **Acyclic data flow (DAG):** a computation is a **Directed Acyclic Graph** of transformations; transformations are combined into a **single plan**, reducing intermediate data movement and disk I/O.
2. **Reuse the working set:** frequently reused data is **kept in memory** and reused across computations/iterations (e.g. PageRank repeatedly updates and reuses the rank vector).
3. **Result:** less disk I/O, less data movement, faster iterative computation. Well suited to **graph processing, machine learning, interactive analysis (Python/R)**.

### 3.1.4 In-memory computing

- Data lives in **RAM, not on slow disk** and is processed **in parallel** across the cluster. Two pillars: **RAM storage + parallel distributed processing**. Made practical because **RAM is cheap**.
- **Iterative processing:** read from persistent store once → each iteration reads from / writes to memory.
- **Interactive processing:** **cache once, run many different queries** on the cached data. Good fit for real-time risk management and fraud detection.

**Challenges of in-memory processing (slide):**

1. How to **distribute data among the DRAM** of the cluster?
2. What if **memory is not sufficient**?
3. How to handle **failures, since memory is volatile**? → answered by **RDDs (partitioning + lineage + spill/persist levels)**.

### 3.1.5 Hadoop (MapReduce) vs Spark ★

| Feature | Hadoop MapReduce | Apache Spark |
| --- | --- | --- |
| Processing model | Batch only, disk-based (map→shuffle→reduce) | Batch + streaming + interactive + iterative, **in-memory** |
| Data abstraction | Key-value records in files | **RDD / DataFrame** |
| Iteration | Each iteration = new job via HDFS | Data cached in memory across iterations |
| Speed | Slower (disk I/O) | Faster (often \~10× or more for iterative; slides: caching gives >10× faster actions) |
| Storage | Has **HDFS** | **No own storage** – uses HDFS/S3/etc. |
| Fault tolerance | Replication in HDFS, re-run tasks | **RDD lineage** recomputation |
| Real-time | No | Near-real-time via Spark Streaming |
| Ease of use | Verbose Java code | Concise APIs in Scala, Python, Java, R |
| Dependency | — | **Does not need Hadoop** but can run on it (reads HDFS) |

**Issues with Hadoop listed in slides:** (i) small-files problem (small file ≪ 128 MB HDFS block; too many files overload the NameNode); (ii) slow processing; (iii) batch only, no real-time; (iv) no delta/cyclic iteration.

### 3.1.6 Word count – the hello-world of Spark

```python
text_file = sc.textFile('input.txt')                    # or hdfs://.../input.txt
words      = text_file.flatMap(lambda line: line.split())   # lines -> words
word_pairs = words.map(lambda w: (w, 1))                    # (word, 1)
counts     = word_pairs.reduceByKey(lambda a, b: a + b)     # sum per key
counts.collect()                                            # ACTION: triggers execution
```

```scala
val sc = new SparkContext(new SparkConf().setAppName('Spark Count'))
val tokenized  = sc.textFile(args(0)).flatMap(_.split(' '))
val wordCounts = tokenized.map((_, 1)).reduceByKey(_ + _)
```

- Each step creates an **RDD** (kept in memory and reusable). Only `collect()` (action) actually runs the job. `countByKey` is an alternative.

### 3.1.7 Log-mining example (slides)

```python
lines    = spark.textFile('hdfs://...')            # base RDD
errors   = lines.filter(startsWithERROR())        # transformed RDD
messages = errors.map(split('\t'), 2)             # transformed RDD
cachedMsgs = messages.cache()                     # keep in memory (reused)
cachedMsgs.filter(containsfoo()).count()          # action 1 (reads HDFS, fills cache)
cachedMsgs.filter(containsbar()).count()          # action 2 (served from cache -> fast)
```

- Driver ships tasks to workers; each worker holds blocks of the file and a **cache** of its partition (Cache 1/2/3 ↔ Block 1/2/3).

---

## 3.2 Scala (the native language of Spark)

**What is Scala?** **SCA**lable **LA**nguage by **Martin Odersky (EPFL, Switzerland)**. A modern **multi-paradigm** language that blends **object-oriented and functional** programming in a concise, type-safe way; **compiles to JVM bytecode** and interoperates with Java libraries.

| Property | Meaning / slide point |
| --- | --- |
| **Object-oriented** | Every value is an object; types/behaviour described by **classes and traits**; **mixin composition** replaces multiple inheritance |
| **Functional** | Every function is a value; **anonymous functions, higher-order functions, nested functions, currying**; **case classes + pattern matching**; singleton `object` |
| **Statically typed** | Generic classes, variance annotations, type bounds, implicit parameters, polymorphic methods |
| **Type inference** | Compiler infers types (`val collegeName = 'PES University'`) → less boilerplate; **parameter types must still be declared**; return type usually inferred (declare explicitly if you want e.g. Float) |
| **Immutable by default style** | `val` = immutable (no reassignment); `var` = mutable |
| **Concurrency** | Full access to Java API (threads, fine-grained) **plus Actors** (message passing, `!`) for coarse-grained concurrency |
| **Lazy evaluation** | Built-in `lazy` keyword defers a computation until needed |
| **Interoperable** | Calls Java methods, extends Java classes; `Array[String]` instead of `String[]`, `args(i)` instead of `args[i]` |

**Why functional programming suits Big Data (slide logic):**

- Biggest concurrency problem = **shared mutable state**. If data is **immutable**, any thread can read it with **no locks/synchronisation**.
- Functional operations **create new structures** instead of modifying old ones (original list unchanged); order of operations not significant; functions are objects (can be passed/returned); operate on collections.
- Big-data = **independent parallel operations (like `map`) + consolidation (like `reduce`/aggregation)** → naturally present in functional languages. Concurrency, parallelism and **referential transparency** are key.

**Java vs Scala (compact)**

| Aspect | Java | Scala |
| --- | --- | --- |
| Verbosity | Boilerplate getters/setters | `class Person(var firstName: String, var lastName: String, var age: Int)` – one line |
| Interface | `interface` (no concrete methods/fields in old Java) | **`trait`** can hold **concrete methods and fields**; `class C extends Super with T` |
| Variables | `final String x` | `val x: String` (immutable), `var` (mutable) |
| Choice | `switch` (statement), `?:` | `match` expression with patterns; `if` is an expression |
| Operators | No operator overloading | Operators are just methods; `a + b` = `a.+(b)` |
| Primitives | Primitives ≠ objects | **All values are objects** (compiler optimises to primitives) |
| Static | `static` members | Singleton **`object`** |
| Compatibility | Backward compatible | Limited backward compatibility; syntax more complex |

**Partition example (functional style):** `val (minors, adults) = people partition (_.age < 18)` replaces \~10 lines of Java.

**Spark vs Scala (⚠ MCQ TRAP):** Spark = **distributed cluster-computing framework**; Scala = **programming language** (Spark is written in Scala). Spark ≠ Scala.

**Spark with Python vs Scala:** Scala = native, static typing, runs on JVM directly (usually faster, new features first); Python = easier, rich libraries (pandas, NumPy), most data scientists use it, but needs a bridge to the JVM (see 3.3) and serialisation overhead.

---

## 3.3 PySpark

- **PySpark = Python API for Spark** released by the Spark community; lets you work with **RDDs/DataFrames in Python**.
- **Features (slide):** (1) real-time computation/low latency due to in-memory; (2) **polyglot** (Scala, Java, Python, R); (3) **caching and disk persistence**; (4) **fast processing**; (5) works well with RDDs (Python is dynamically typed).
- Supports **interactive** use (`pyspark` shell) and **batch** jobs (scripts via `spark-submit`).

**Setup (slide):** download & untar Spark (e.g. `spark-3.3.4-bin-hadoop3.tgz`) into `/opt/spark`; set environment variables in `~/.bashrc`:

```bash
export SPARK_HOME=/opt/spark
export PATH=$SPARK_HOME/bin:$SPARK_HOME/sbin:$PATH
export PYSPARK_PYTHON=/usr/bin/python3.10
export PYSPARK_DRIVER_PYTHON=/usr/bin/python3.10
source ~/.bashrc      # then run: pyspark
```

### PySpark architecture (★)

- **SparkContext is maintained per JVM** (one per JVM). Python cannot create JVM objects directly, so **Py4J** acts as a **bridge**: it lets the Python interpreter access **Java objects living in the JVM** and call their methods as if they were Python methods.
- Python driver program → **Py4J** → **Java SparkContext** (drives the application) → cluster executors. Data between Python and JVM must be **serialised (cPickle/pickle serializer)**.

```
 Python driver --Py4J--> JVM Driver (Java SparkContext) --> Cluster Manager --> Executors
   (your code)                                                         (Python workers, pickled data)
```

**SparkContext:** main entry point; represents connection to the cluster; used to create **RDDs and broadcast variables**; **at least master and app name** must be set. Interacts with cluster manager (e.g. YARN). Created automatically in `spark-shell`/`pyspark` (as `sc`), or programmatically via `SparkContext.getOrCreate()`; **one SparkContext per JVM**.

**SparkSession (Spark 2+):** unified entry point for DataFrames/SQL (`spark = SparkSession.builder.appName('x').getOrCreate()`); `spark.sparkContext` gives the context.

---

## 3.4 Spark / PySpark Programming Model

### 3.4.1 Lifecycle of a Spark program (★ 4-mark)

1. **Create input RDDs** from external data (`textFile`) or by **parallelizing** a driver collection (`parallelize`).
2. **Lazily transform** them into new RDDs (`filter`, `map`, …).
3. **`cache()`** intermediate RDDs that will be reused.
4. **Launch actions** (`count`, `collect`) to start parallel computation, which Spark **optimises and executes**.

- **Until an action is seen, only a DAG is built by the driver – no actual work is done.**

### 3.4.2 Creating RDDs – two ways

```python
rdd1 = sc.textFile('hdfs://.../input.txt')                       # 1. load external dataset
rdd2 = sc.parallelize(['Hello world', 'Hello Spark', 'Hello PySpark'])  # 2. distribute a collection
```

```python
from pyspark.sql import SparkSession
spark = SparkSession.builder.appName('Create RDD').getOrCreate()
myData = spark.sparkContext.parallelize([(1,2),(3,4),(5,6)])
myData.collect()                       # [(1, 2), (3, 4), (5, 6)]
```

### 3.4.3 DataFrames in PySpark (why needed, how created)

- **Problem:** RDDs are **opaque** to Spark – it cannot parse the values or understand structure (e.g. rows of `USN, Name, Marks`), so no SQL-like queries and no optimisation.
- **DataFrame** (introduced **2015**, inspired by **R data frames and Pandas**): a **distributed collection of data organised into named columns** – an abstraction **over RDD with a schema** and a **built-in optimiser (Catalyst)**. Memory aid: **DataFrame = RDD + Schema + DSL**.

```python
# from JSON
df = spark.read.json('file:///home/user/students.json'); df.show()
# from an existing RDD (name the columns)
df = rdd.toDF(['USN','name'])
# from a Python list
Employee = spark.createDataFrame([('1','Joe','70000','1'),('2','Henry','80000','2')],
                                 ['Id','Name','Salary','DepartmentId'])
df.printSchema()                    # tree format: root |-- usn: long (nullable = true) ...
df.select('name').show()            # projection
df.select('name', df.marks + 1).show()   # computed column
```

**Exercise (slides): total tax paid per PAN holder** (`<pan, date, tax_paid>`):

```python
# RDD way: split lines manually
rdd.map(lambda l: l.split(',')).map(lambda f: (f[0], float(f[2]))).reduceByKey(lambda a,b: a+b)
# DataFrame way: by column NAME, no manual splitting
df = rdd.toDF(['pan','date','tax_paid'])
df.groupBy('pan').sum('tax_paid')
```

---

## 3.5 RDD – Resilient Distributed Dataset (★★ most important topic)

### 3.5.1 Definition and meaning

- **RDD = Resilient Distributed Dataset:** an **immutable, partitioned, locality-aware, distributed collection** of records that can be operated on in parallel, with **lineage** for fault recovery.
  - **Resilient** → lost partitions are **recomputed using lineage**.
  - **Distributed** → data is **partitioned across machines**.
  - **Dataset** → collection of data to process.
- An RDD either **points to a source** (e.g. HDFS) or **applies transformations to parent RDD(s)**. Computation is **lazily evaluated**; chained RDDs form a **lineage DAG**.
- **Immutable:** operations create **new RDDs** rather than modifying existing ones. *Why necessary?* Enables safe sharing across nodes without locks, deterministic **recomputation from lineage**, and easy caching.

### 3.5.2 Why the RDD abstraction? (slide)

Supports operations **beyond map and reduce**; supports **in-memory computation**; allows **arbitrary composition** of operators; **simplifies scheduling**; captures **dependencies generically**.

### 3.5.3 The five properties of an RDD (★ exact list)

| # | Property | Interface method | Hadoop analogue |
| --- | --- | --- | --- |
| 1 | **Set of partitions** ('splits') | `partitions()` | Input splits |
| 2 | **List of dependencies** on parent RDDs | `dependencies()` | *New in Spark* |
| 3 | **Function to compute a partition** given parents | `iterator(p, parentIters)` | map()/reduce() code |
| 4 | **Preferred locations** (data locality; optional) | `preferredLocations(p)` | Block locations |
| 5 | **Partitioner** (hash/range; optional, for shuffles) | `partitioner()` | Partitioner |

**Worked examples from the slides**

| RDD type | Partitions | Dependencies | Compute | Preferred locations | Partitioner |
| --- | --- | --- | --- | --- | --- |
| **HadoopRDD** | one per HDFS block | none | read the block | HDFS block location | none |
| **FilteredRDD** | same as parent | **1-to-1 (narrow)** with parent | compute parent, then filter | ask parent (none) | none |
| **JoinedRDD** | one per reduce task | **shuffle on each parent (wide)** | read & join shuffled data | none | **HashPartitioner(numTasks)** |

### 3.5.4 Fault tolerance via lineage

```
messages = textFile(...).filter(startsWithERROR).map(split('\t')(2))
HDFS file --filter--> Filtered RDD --map--> Mapped RDD
```

If a node holding a Mapped-RDD partition fails, Spark re-reads **only the needed HDFS block**, re-applies filter→map and rebuilds **only the lost partition** – no replication of data required. Lineage is stored as a **DAG**: operations performed, RDDs produced, and dependencies between them.

### 3.5.5 Operation types

- **Transformations** → create a **new RDD** (lazy). **Actions** → **trigger computation and return a value** to the driver / write output. (Detailed in 3.6.)

### 3.5.6 Related systems compared (slide 'Task assignment')

DryadLINQ/FlumeJava (distributed collection API but **cannot reuse datasets across queries efficiently**); relational DBs (lineage, logical logging, materialised views); GraphLab/Piccolo/BigTable/RAMCloud (**fine-grained writes**, like distributed shared memory); iterative MapReduce – Twister/HaLoop (implicit sharing for fixed pattern); caching systems – Nectar (no explicit control over what is cached). **RDDs = coarse-grained transformations + lineage + explicit control of persistence.**

---

## 3.6 RDD Operations: Transformations and Actions (★★)

### 3.6.1 Core distinction

|  | **Transformation** | **Action** |
| --- | --- | --- |
| Returns | A **new RDD** | A **value** to the driver (or writes to storage) |
| Evaluation | **Lazy** – only recorded in lineage/DAG | **Eager** – **triggers execution** of the whole DAG |
| Examples | `map, flatMap, filter, groupBy, reduceByKey, join, union, distinct, sample` | `collect, count, take, reduce, fold, aggregate, saveAsTextFile` |
| Fault tolerance | Lineage built from them | Result is returned/saved |

⚠ **MCQ TRAP:** `reduceByKey` is a **transformation** (returns RDD) but `reduce` is an **action**; `reduceByKey` is a **wide** transformation; `countByKey` and `countByValue` are **actions** (return a local map to the driver); `cache()/persist()` are **not actions** (lazy markers).

### 3.6.2 Transformations with worked outputs (from slides)

| Transformation | Meaning | Example → output |
| --- | --- | --- |
| `map(func)` | Apply func to **every element**; **exactly one output per input**; output type may differ from input type | `x=[b,a,c]; x.map(lambda z:(z,1))` → `[(b,1),(a,1),(c,1)]` |
| `flatMap(func)` | Apply func, then **flatten**; **zero or many outputs per input** (e.g. split lines into words) | `[1,2,3].flatMap(lambda x:(x,x*100,42))` → `[1,100,42,2,200,42,3,300,42]` |
| `filter(func)` | Keep elements satisfying a **predicate**; **narrow** (no shuffle) | `[1,2,3].filter(lambda x:x%2==1)` → `[1,3]` |
| `groupBy(func)` | Key = output of func; value = all items with that key | `['John','Fred','Anna','James'].groupBy(lambda w:w[0])` → `J:[John,James], F:[Fred], A:[Anna]` |
| `groupByKey()` | On a **pair RDD**, collect all values per key | `[(B,5),(B,4),(A,3),(A,2),(A,1)]` → `B:[5,4], A:[3,2,1]` |
| `reduceByKey(f)` | Merge values per key using an associative function **with map-side combine** | `[(one,1),(two,1),(two,1),(three,1)×3]` → `(one,1),(two,2),(three,3)` |
| `mapPartitions(f)` | Apply f **once per partition** (gets an iterator) – cheaper setup per partition | `[1,2,3]` in 2 partitions `[[1],[2,3]]`, f yields sum then 42 → `[[1,42],[5,42]]` |
| `mapPartitionsWithIndex(f)` | Same, plus **partition index** | → `[[(0,1)],[(1,5)]]` |
| `sample(withReplacement, fraction, seed)` | Random subset | `x.sample(False,0.4,42)` → \~40% of elements |
| `union(other)` | All items of both RDDs; **duplicates NOT removed** | `[1,2,3]` ∪ `[3,4]` → `[1,2,3,3,4]` |
| `join(other)` | Pair RDDs: all pairs with the **same key** | `[(a,1),(b,2)]` ⋈ `[(a,3),(a,4),(b,5)]` → `(a,(1,3)),(a,(1,4)),(b,(2,5))` |
| `distinct()` | Remove duplicates (needs shuffle) | `[1,2,3,3,4]` → `[1,2,3,4]` |
| `keyBy(f)` | Make a pair RDD; key computed from value | `['John','Fred','Anna','James'].keyBy(lambda w:w[0])` → `(J,John),(F,Fred),(A,Anna),(J,James)` |

`glom()` shows elements **grouped by partition** (used in slide demos to see partitioning).

**map vs flatMap (⚠ TRAP):** `map` returns **one element per input** (a list stays a list *inside* the RDD); `flatMap` **flattens** so the RDD contains the individual items.

```python
lines = sc.parallelize(['a b', 'c d e'])
lines.map(lambda l: l.split()).collect()      # [['a','b'], ['c','d','e']]   (2 elements)
lines.flatMap(lambda l: l.split()).collect()  # ['a','b','c','d','e']        (5 elements)
```

### 3.6.3 reduceByKey vs groupByKey (★ asked often)

```scala
val words = Array('one','two','two','three','three','three')
val pairs = sc.parallelize(words).map(w => (w, 1))
val r = pairs.reduceByKey(_ + _).collect()                    // combine locally, then shuffle
val g = pairs.groupByKey().map(t => (t._1, t._2.sum)).collect() // shuffle ALL pairs, then sum
```

| Feature | `reduceByKey` | `groupByKey` |
| --- | --- | --- |
| Map-side (local) combine | **Yes** – partial sums per partition before shuffle | **No** |
| Data shuffled | **Much less** (one partial value per key per partition) | **All** (key, value) pairs |
| Memory risk | Low | A key with huge values may **not fit in memory** / OOM |
| Function needed | **Associative & commutative** reduce function | None (returns an `Iterable`) |
| Use when | Aggregation (sum, count, max) | You truly need the **full list** of values |
| Dependency | Wide | Wide |

### 3.6.4 Actions with worked outputs

| Action | Meaning | Example |
| --- | --- | --- |
| `collect()` | Return **all elements** to the driver as a list; **entire RDD must fit in driver memory** (use for small results/unit tests) | `[1,2,3].collect()` → `[1,2,3]` |
| `count()` | Number of elements | `{1,2,2,3,4,5,5,6}.count()` → **8** |
| `countByValue()` | How many times each element occurs | → `{1:1,2:2,3:1,4:1,5:2,6:1}` |
| `countByKey()` | Pair RDD: count per key, returns map to driver | `[(J,James),(F,Fred),(A,Anna),(J,John)]` → `{J:2, F:1, A:1}` |
| `take(n)` | First n elements (tries to touch few partitions, so order is not guaranteed; a 'biased' sample) | slide example `take(4)` on `{1,2,2,3,4,5,5,6}` |
| `top(n)` | Largest n elements by default ordering |  |
| `reduce(f)` | Combine elements **pairwise** with a **commutative & associative** function; result **same type** as elements; **throws exception on empty RDD** | `[1,2,3,4].reduce(a+b)` → **10** |
| `fold(zero)(f)` | Like reduce but with a **zero value (identity element)** used for each partition; **works on empty RDD** | `rdd.fold(0)(_+_)` (0 = identity for +, 1 for ×) |
| `aggregate(zero)(seqOp, combOp)` | Combine elements **within** partitions with seqOp, then **across** partitions with combOp; **result type can differ** from element type | `[1,2,3,4].aggregate(([],0), seqOp, combOp)` → `([1,2,3,4], 10)` |
| `sum(), mean(), max(), stdev()` | Numeric statistics | `[2,4,1]` → sum 7, mean 2.33, max 4 |
| `getNumPartitions()` | Number of partitions | `sc.parallelize([1,2,3],2)` → 2 |
| `saveAsTextFile(path)` | Write RDD as text files (one part-file per partition) |  |

**reduce vs fold vs aggregate (⚠):** reduce → same type, no zero, fails on empty; fold → same type, **zero value**, safe on empty; aggregate → **different result type**, needs **two functions** (seqOp, combOp).

### 3.6.5 Pair RDDs (key-value RDDs)

- Spark gives **pair RDDs** (RDD of `(K, V)`) with **special operations**: `reduceByKey, groupByKey, join, keyBy, mapValues, countByKey` etc. Needed whenever you need **state/aggregation per key**.
- `countByKey(K,V) → HashMap(K, Int)` is a pair-RDD action.

### 3.6.6 Persistence (caching)

- `cache()` / `persist()` keeps a dataset **in memory across operations**; each node stores the partitions it computes and reuses them in later actions → future actions often **>10× faster**.
- **Fault tolerant cache:** a lost cached partition is **recomputed from lineage**.
- **\[Supplement\] Storage levels:** `MEMORY_ONLY` (default for `cache()` on RDDs), `MEMORY_AND_DISK` (spill to disk), `DISK_ONLY`, `MEMORY_ONLY_SER` (serialised, compact), and `_2` variants (replicated on 2 nodes). `unpersist()` frees the cache. Cache is **lazy** – it is filled when the first action runs.

### 3.6.7 Shared variables (★)

|  | **Broadcast variable** | **Accumulator** |
| --- | --- | --- |
| Purpose | Efficiently **distribute a large read-only value** to all nodes (e.g. lookup table) | **Aggregate information** across tasks (counters, sums) |
| Direction | Driver → workers | Workers → driver |
| Mutability | **Read-only** on workers | Workers can only **add**; only the **driver reads** the value |
| Operation constraint | — | Must be **associative and commutative** (add) |
| Analogue | Distributed cache | **MapReduce counters** |
| Note | Cached once per machine in **serialised form**, deserialised before each task; explicit use helps when **multiple stages** need the same data | Numeric types supported by default; custom accumulator types allowed |

```python
lookup = sc.broadcast({'IN': 'India', 'US': 'USA'})
bad = sc.accumulator(0)
def f(code):
    if code not in lookup.value: bad.add(1)
    return lookup.value.get(code, 'Unknown')
rdd.map(f).count(); print(bad.value)
```

---

## 3.7 Spark SQL and DataFrames (★★)

### 3.7.1 Motivation: structured data processing

- Many big-data jobs process **structured / semi-structured** data. In plain Spark RDD all functions/objects are **black boxes** – Spark does **not know the data's structure**, so the programmer must hand-code **parsing (raw record → fields), conversion (cast to types) and processing**. No optimisation is possible.
- **Earlier systems:** **Pig/Pig Latin** (SQL-like → MapReduce), **Hive/HiveQL** (SQL-like queries on Hadoop), **Shark** (Hive on Spark: translates HiveQL to RDD programs – first attempt at SQL on Spark; limited because it could only query external Hive data and was awkward to call from Spark programs).
- **Spark SQL (SIGMOD 2015, Armbrust et al.):** redesigned for Spark's query model – **supports all popular relational operators**, can be **intermixed with RDD code**, and adds the **DataFrame API** as an enhancement over RDDs. Tagline: *Relational data processing in Spark.*

### 3.7.2 Two ways to use it

```python
df = spark.read.json('students.json')
# 1) DataFrame (DSL) API – method calls
df.where(df.marks > 20).select('name', 'marks').show()
# 2) SQL strings on a registered view
df.createOrReplaceTempView('students')
spark.sql('SELECT name, marks FROM students WHERE marks > 20').show()
```

- Both produce the **same optimised plan** – **'DataFrames and SQL share the same optimization/execution pipeline.'**

### 3.7.3 Data model (slide)

- **Nested data model.** **Primitive SQL types:** boolean, integer, double, decimal, string, date, timestamp. **Complex types:** **struct, array, map, union**. **User-defined types (UDTs)**. **First-class support for complex types.**

### 3.7.4 DataFrame operations

- Same operations as SQL (**select, where, join, groupBy**) but written as **method calls**; an expression like `employees('gender') === 'female'` is **not evaluated immediately** – it **builds a column expression**; operators build an **abstract syntax tree (AST)** that is **optimised by Catalyst**.
- **Built-in ops:** filter (selection), select (projection), join, group-by (aggregation), load/store in various formats, cache, conversion between RDD ↔ DataFrame.

**Advantages of DataFrames over plain relational query languages (3 points ★):**

1. **Holistic optimisation** – code written in Python/Scala/SQL is optimised **together as one plan**, not separate opaque steps.
2. **Real control structures** – `if`/`for` loops around queries (impossible inside a SQL string).
3. **Early error detection** – the **logical plan is analysed immediately**, so schema mismatches (wrong column) surface at write-time, not after a job runs.

**Querying native datasets:** Spark infers schema from your own classes – **Java/Scala via reflection** (`case class User(name: String, age: Int)` → schema `{name: String, age: Int}`), **Python by sampling rows** (no static types). Native objects are used **without conversion**; you can add `.where/.groupBy` onto RDDs of your objects and **join them with external structured data (CSV, Parquet, JDBC)**. Enables **columnar storage with hot columns cached in memory**.

**User-defined functions (UDFs):** built-in function set is limited, so UDFs add custom logic; **defined and registered inline** in your code (unlike **Pig**, where UDFs must be a separate Java package loaded into the script); work on a single value or aggregate a table; once registered, callable from **both DataFrame code and SQL strings**.

```python
from pyspark.sql.functions import udf
from pyspark.sql.types import StringType
spark.udf.register('grade', lambda m: 'A' if m >= 40 else 'B')
spark.sql('SELECT name, grade(marks) FROM students').show()
```

### 3.7.5 Catalyst – Spark SQL's extensible query optimiser (★★)

- Represents a query as a **tree** and applies **rules (transformations)** that rewrite the tree into a faster equivalent. e.g. tree `Add(Attribute(x), Add(Literal(1), Literal(2)))` for `x + (1 + 2)` is simplified to `x + 3` (**constant folding**).
- **Rules** are **pattern-matching (partial) functions** that rewrite matching subtrees and leave everything else untouched → adding a new operator never requires editing old rules. Rules are applied in **batches until a fixed point** is reached; `transform` can contain arbitrary Scala code.

```
SQL / DataFrame --> [Unresolved Logical Plan]
  1 ANALYSIS (uses Catalog) --> [Logical Plan]
  2 LOGICAL OPTIMISATION --> [Optimised Logical Plan]
  3 PHYSICAL PLANNING (Cost Model picks best) --> [Physical Plans] --> [Selected Physical Plan]
  4 CODE GENERATION --> Java bytecode / RDDs
```

| Phase | What happens | Detail from slides |
| --- | --- | --- |
| **1. Analysis** | Resolve **unresolved attributes/relations** (e.g. `SELECT col FROM sales`) | Look up relations by name in the **Catalog**; map attributes to operator inputs; assign **unique IDs** to references to the same value; **propagate and coerce types** (e.g. `1 + col`) |
| **2. Logical optimisation** | **Rule-based** rewrites | **Constant folding, predicate pushdown, projection (column) pruning, null propagation, boolean expression simplification**; \~**800 lines of code** |
| **3. Physical planning** | One logical plan → **multiple physical plans**; a **cost model** selects the best | **Pipelines projections and filters into a single map** operation; rule-based for the rest |
| **4. Code generation** | Compile the tree to **Java bytecode** to evaluate expressions fast | Uses **Scala quasiquotes** `q'...'`; \~**700 lines** of code; avoids per-row interpretation/boxing |

**Predicate pushdown / column pruning example (slide):** `events JOIN users ... WHERE city = 'Melbourne' SELECT timestamp`: naive plan joins everything then filters; optimised plan **pushes the filter below the join** and **scans only needed columns** from the Parquet/Hive tables → far less data read and joined.

**Example Catalyst rule:** find a **Filter above a Project**; check the filter can be evaluated **without** the project's output; if yes **swap the operators** (filter first).

**Extension points:** **Data sources** must implement `createRelation(params)` returning a **`BaseRelation`** (CSV, Avro, Parquet, JDBC). **UDTs** map user types to Catalyst built-in types.

**Schema inference for semi-structured JSON:** infers schema **in one pass or from a sample**; represents it as a **tree of STRUCT types** (atoms, arrays, other STRUCTs); picks the **most specific type** that fits **all observed values** of a field (also for array elements); merges per-record schemas in **one reduce operation**; same trick used for Python.

### 3.7.6 RDD vs DataFrame (★ comparison)

| Feature | RDD | DataFrame |
| --- | --- | --- |
| Structure | Unstructured/opaque objects | **Named columns + schema** |
| Optimisation | None (user code is a black box) | **Catalyst optimiser + code generation** |
| Introduced | Spark 1.0 (original) | **2015** (Spark 1.3) |
| API style | Functional (`map`, `reduceByKey`) | Relational (`select`, `groupBy`, SQL) |
| Type safety | Compile-time in Scala/Java | Runtime schema checks (analysis at plan time) |
| Serialisation | Java/Kryo of whole objects | Compact columnar/binary (Tungsten) \[Supplement\] |
| Best for | Low-level control, unstructured data | Structured/semi-structured data, SQL, performance |

---

## 3.8 Relational Operators with Spark

Spark SQL supports **all the popular relational operators**; you can use them via **SQL**, **DataFrame methods**, or mix with RDD operations.

| Relational algebra | SQL | DataFrame (PySpark) | RDD equivalent |
| --- | --- | --- | --- |
| **Selection σ** (rows) | `WHERE` | `df.filter(cond)` / `df.where(cond)` | `rdd.filter(f)` |
| **Projection π** (columns) | `SELECT a,b` | `df.select('a','b')` | `rdd.map(f)` |
| **Join ⋈** | `JOIN ... ON` | `df1.join(df2, on='k', how='inner')` | `rdd1.join(rdd2)` (pair RDDs) |
| **Aggregation γ** | `GROUP BY` + `SUM/COUNT/AVG` | `df.groupBy('k').agg(sum('v'))` | `reduceByKey` / `aggregateByKey` |
| **Union ∪** | `UNION ALL` | `df1.union(df2)` | `rdd1.union(rdd2)` |
| **Distinct** | `SELECT DISTINCT` | `df.distinct()` | `rdd.distinct()` |
| **Intersection ∩, Difference −** | `INTERSECT`, `EXCEPT` | `df1.intersect(df2)`, `df1.subtract(df2)` | `intersection`, `subtract` |
| **Cartesian ×** | `CROSS JOIN` | `df1.crossJoin(df2)` | `rdd1.cartesian(rdd2)` |
| **Sort** | `ORDER BY` | `df.orderBy('a')` | `sortBy` / `sortByKey` |
| Load/Store, Cache | `CREATE TABLE`, `CACHE TABLE` | `spark.read/df.write`, `df.cache()` | `textFile`, `saveAsTextFile`, `cache` |

**Worked example (employee/department):**

```python
emp = spark.createDataFrame([(1,'Joe',70000,1),(2,'Henry',80000,2),(3,'Sam',60000,2),(4,'Max',90000,1)],
                            ['Id','Name','Salary','DeptId'])
dept = spark.createDataFrame([(1,'IT'),(2,'Sales')], ['DeptId','DeptName'])

emp.filter(emp.Salary > 65000).select('Name')               # selection + projection -> Joe, Henry, Max
emp.join(dept, 'DeptId').groupBy('DeptName').avg('Salary')  # join + aggregation: IT 80000.0, Sales 70000.0

emp.createOrReplaceTempView('emp'); dept.createOrReplaceTempView('dept')
spark.sql('''SELECT d.DeptName, MAX(e.Salary) AS top
             FROM emp e JOIN dept d ON e.DeptId = d.DeptId
             GROUP BY d.DeptName ORDER BY top DESC''').show()   # IT 90000, Sales 80000
```

**Join types \[Supplement\]:** `inner` (default), `left_outer`, `right_outer`, `full_outer`, `left_semi`, `left_anti`, `cross`.

**Execution notes:** `filter`/`select` are **narrow** (pipelined into one map); `join`, `groupBy`, `distinct`, `orderBy` need a **shuffle (wide)**. Catalyst performs **predicate pushdown** (filter before join) and **column pruning** automatically, which hand-written RDD code cannot get. Small tables can be **broadcast-joined** \[Supplement\] (`broadcast(df)` hint) to avoid shuffling the big table.

---

## 3.9 Spark Architecture, Lazy Execution, DAG, Dependencies and Scheduling (★★)

### 3.9.1 Spark ecosystem (stack)

```
+--------------------------------------------------------------+
|  Spark SQL / DataFrames | Spark Streaming | MLlib | GraphX    |  <- Libraries & DSLs
+--------------------------------------------------------------+
|  Spark Core:  Compute Engine  +  Core APIs                    |
|     (memory mgmt, task scheduling, fault recovery,            |
|      talks to cluster manager & storage)                      |
|   Core APIs = Structured (DataFrame, Dataset)                 |
|             + Unstructured/low-level (RDD, Accumulators,      |
|               Broadcast variables)                            |
+--------------------------------------------------------------+
|  Cluster manager: Standalone | YARN | Mesos | Kubernetes      |
|  Storage: HDFS | Amazon S3 | Google Cloud Storage | Cassandra   |
+--------------------------------------------------------------+
```

- Spark is a **distributed processing engine**; it **does not ship its own cluster manager or distributed storage** – any compatible ones can be used.
- **Spark Core** = compute engine (**memory management, task scheduling, fault recovery, interacting with cluster manager and storage**) + core APIs (available in **Scala, Python, Java, R**).
- **Libraries:** **Spark SQL** (relational tables, schema, SQL; sources: Hive, Parquet, JSON) • **Spark Streaming** (DStream = stream of datasets for intervals/time windows; high-throughput, fault-tolerant; joins only **within a window** or with a static dataset, since joins on unbounded streams are impossible) • **MLlib** (basic statistics, classification – Naive Bayes, decision trees, logistic regression; clustering – k-means, GMM; all scale out) • **GraphX** (graph abstraction: directed multigraph, properties on vertices and edges, stored as RDDs; operators like `subgraph`, `mapVertices`).

### 3.9.2 Four styles of processing in Spark

| Style | Use |
| --- | --- |
| **Batch** | Large datasets, big map-reduce-type jobs |
| **Streaming** | Incoming information in **near real time** |
| **Iterative** | ML algorithms like **gradient descent** – data accessed repeatedly until **convergence** |
| **Interactive** | Data exploration; large chunks in memory + very quick response |

### 3.9.3 Architecture – driver, executors, cluster manager (★)

```
            +---------------- Driver Program ----------------+
            |  SparkContext / SparkSession  (the 'master')   |
            |  builds DAG, makes stages/tasks, schedules     |
            +----------------------+-------------------------+
                                   | asks for resources
                         +---------v----------+
                         |  Cluster Manager   |  (Standalone / YARN / Mesos / K8s)
                         +----+----------+----+
                              |          |
                     +--------v--+  +----v-------+
                     | Worker    |  | Worker     |
                     | Executor  |  | Executor   |  <- run TASKS, hold CACHE,
                     | [tasks]   |  | [tasks]    |     report status to driver
                     +-----------+  +------------+
```

| Component | Responsibilities |
| --- | --- |
| **Driver process** (master) | **Maintains information about the application; responds to the user's program/input; analyses, distributes and schedules work** across executors; converts program into a **DAG** |
| **Executors** (workers) | **Execute the code assigned by the driver; report the state of computation back to the driver**; store cached data |
| **Cluster manager** | **Controls physical machines and allocates resources** to Spark applications. Three core options in slides: **Spark Standalone, YARN, Mesos** (Kubernetes also supported) |

**Working steps:** (1) user code submitted → driver creates SparkContext; (2) driver builds **DAG of RDDs** as transformations are declared; (3) on an **action**, DAG is split into **stages** at shuffle boundaries and each stage into **tasks (one per partition)**; (4) scheduler asks cluster manager for executors and **ships tasks to where data is (locality)**; (5) executors run tasks, return results/status; (6) driver assembles the result.

**Job / Stage / Task \[Supplement\]:** each **action → one job**; each job → **stages** (separated by shuffles); each stage → **tasks** (one per partition, the smallest unit of work).

### 3.9.4 Lazy execution / lazy evaluation (★ 4-mark)

- **Hadoop:** the master starts executing as soon as the job is submitted. **Spark:** the driver executes **nothing until it meets an action**; transformations are **only noted for lineage**.

```python
lines = spark.textFile('hdfs://...')   # does NOT read the file; just notes an RDD is to be created
lines.map(f)                           # does NOT transform; adds a node to the DAG
lines.map(f).count()                   # ACTION: triggers execution - all transformations done
                                       # block-by-block on the same server, followed by count
```

**Advantages:** (a) **manageability** – program in small operations; grouping reduces passes over data; (b) **saves computation and increases speed** – only needed values computed; fewer driver↔cluster trips; (c) **reduces time and space complexity**; allows working with **infinite data structures**; (d) **optimisation** – whole plan seen before running, pipelining of operations, no unnecessary intermediate materialisation.

### 3.9.5 DAG representation

- The driver converts a program into a **DAG**: **each RDD is a node; each transformation/action is an edge**. The DAG is used for **scheduling** and **fault recovery (lineage)**.

**PageRank in Spark (slide example):**

```python
lines = sc.textFile('urls.txt')
links = lines.map(lambda u: u.split()).groupByKey().cache()   # (url, [neighbours]) reused -> cache
ranks = links.map(lambda u_n: (u_n[0], 1.0))                   # initial rank 1.0
for i in range(MAXITER):
    contribs = links.join(ranks).flatMap(lambda x: computeContribs(x))   # each page sends rank/num_neighbours
    ranks = contribs.reduceByKey(add).mapValues(lambda r: r*0.85 + 0.15) # damping 0.85
```

- `links` is **static and reused every iteration → cached**; only `ranks` changes. **Question in slide:** links and ranks are spread over many nodes – how does the **join** work? → matching keys must be on the **same node**, so Spark **shuffles by key** (hash/range partitioning) — or avoids it via **co-partitioning**.

### 3.9.6 Narrow vs wide dependencies (★★ high MCQ probability)

|  | **Narrow dependency** | **Wide (shuffle) dependency** |
| --- | --- | --- |
| Definition | Each partition of the parent RDD is used by **at most one** child partition | **Multiple** child partitions may depend on a parent partition |
| Data movement | **No shuffle**; operations are **pipelined** within one stage | **Shuffle** (data moves node-to-node across network) |
| Recovery | Re-compute only the **one** lost parent partition | May need **many** parent partitions → costly |
| Stage boundary | **No** – same stage | **Yes** – **a new stage begins** |
| Slide examples | `map, flatMap, mapPartitions, filter, sample, union` | `intersection, distinct, reduceByKey, groupByKey, join, cartesian, repartition, coalesce` |

⚠ **MCQ TRAPS:** (1) **Shuffle = movement of data from one node to another.** (2) The slide lists `coalesce` under wide; in real Spark `coalesce` (without shuffle) is narrow and `repartition` is wide – if asked from the slide, follow the slide; if asked conceptually, remember repartition always shuffles. (3) A `join` of two RDDs that are **co-partitioned** (same partitioner) is **narrow**. (4) `union` is narrow.

**Co-partitioning:** make sure **both inputs of a join are partitioned with the same function** (e.g. hash both `links` and `ranks` on URL) → the repeated join in PageRank needs **no shuffle** each iteration. Spark supports **two partitioners: hash and range**.

### 3.9.7 Scheduling (stages and locality)

- The DAG is **broken into stages at shuffle boundaries; computation inside a stage is local (pipelined)**.
- Example in slide: Stage 1 = `groupBy` (A→B), Stage 2 = `map`/`union` branch (C, D, E, F), Stage 3 = `join` result – join needs a shuffle, so it starts a new stage.
- **Task assignment uses data locality with delay scheduling:** if a partition is **cached in memory on a node**, send the task there; otherwise if the RDD gives **preferred locations** (e.g. HDFS block hosts) send it to those nodes; else any free node (after waiting a short time for a local slot).

---

## 3.10 Streaming Algorithms – Sampling, Set Membership (Bloom Filters), Counting Unique Elements (Flajolet–Martin)

### 3.10.1 Why streaming algorithms?

- A **stream** is a never-ending sequence of events (sensor readings, web clicks, queries, transactions). It **cannot be stored in full**, so we must **summarise** it (e.g. *how many unique elements so far?* – in SQL `COUNT(DISTINCT key)` needs all data stored).
- **Windowing** approach: keep the last **n elements** or last **t seconds**; allows relational operations (like Spark's model).
- **Stream data model:** multiple streams (different rates, unsynchronised) → **archival store** (offline) + **working store** (memory/disk: summaries, parts of streams); **queries** are **standing** (continuously running, e.g. max temperature ever) or **ad hoc** (arbitrary, need stored window, e.g. unique users in last 30 days).
- **Issues:** **velocity** (rate; mission-critical systems need a decision per element) and **volume/#streams** (many streams each needing a little memory can exhaust memory).
- **Design rules:** process **in memory, not off disk**; prefer **approximate answers** over exact; **hashing** gives repeatable randomness (same key → same hash). Examples of approximate methods: **sampling, Bloom filter, Flajolet–Martin**.

### 3.10.2 Sampling a stream

**Goal:** keep a small subset that is **representative**, so analysing the sample is like analysing the whole stream. **Motivating example:** search-engine stream of tuples `(user, query, time)`; we can store **1/10** of it; question: **'What fraction of a typical user's queries were repeated over the past month?'**

**Obvious (naive) algorithm:** for each tuple generate a random integer 0..9; keep if 0, else discard → each tuple kept with p = 1/10 independently.

**Flaw (★):** suppose a user issued query *s* **twice**. Both copies are kept with probability (1/10)×(1/10) = **1/100** (not 1/10), so we detect the repetition only 1% of the time. Exactly one copy is kept with probability 2×(1/10)(9/10) = **18/100**, so a repeated query looks like a **singleton**.

- Let user have *s* queries once and *d* queries exactly twice. **True** fraction repeated = d/(s+d). **Sample estimate** = (d/100) ÷ (s/10 + 18d/100 + d/100) = **d/(10s + 19d)** → **biased low**.

**Refined algorithm – sample by USER, not by tuple:**

- Select **1/10th of the users** and keep **ALL** their queries. When a query arrives: look up the user; if in sample add the query; if the user is new, draw a random number 0..9 and add the user if 0.
- **Drawback:** must **store every user seen so far** with an in/out decision (a lookup table).
- **Optimisation (hash-based):** no table needed – **hash(username) → 0..9; select if hash == 0.** Same user always hashes the same, so all of a user's queries are in or out together.

**Generalisation (★ formula):**

1. Identify the **key components** of the tuple (here only `user`; `query` and `time` are not hashed).
2. Hash the key into buckets **0..b−1**.
3. To get a sample fraction **a/b**, **select the tuple if hash(key) < a**.

- Exercise answer (slides): transactions by (user, country), 1/20 sample → hash **(userid, country)** into **0..19**, keep if hash = 0 (gives 1/20 of all user–country pairs; if you want 1/20 of *users* hash only userid).

**Varying the sample size (memory limit):** as the stream grows, the number of keys in the sample grows. If memory is full, **lower the threshold from a to a−1** and **delete stored tuples whose key hashes to a−1**. The new sample is a **subset of the old** and still a **uniform (a−1)/b sample of keys**.

### 3.10.3 Set membership – Bloom filters (★★)

**Problem (filtering):** stream of emails; we have **1 billion known non-spam addresses** (20 bytes each ≈ 18.63 GiB) but only **1 GB RAM**; disk lookup is too slow. We want to decide **instantly** whether an address is in the set.

**Basic idea (1 hash function):**

- 1 GB = **8 × 10⁹ bits** → bit array of n = 8 billion bits, all initialised 0.
- **Initialisation:** hash each non-spam address to 0..n−1 and **set that bit to 1** (about 1 billion bits become 1 → \~1/8 of the array).
- **Use:** hash the incoming address. **Bit = 0** → definitely **not** in the set → **spam (filter out)**. **Bit = 1** → **maybe** in the set → let through (may be spam that collided).
- → **No false negatives, but false positives possible.**

**General Bloom filter:**

- **Components:** an array of **n bits**; **k independent hash functions h₁…hₖ**; a set **S of m keys**.
- **Insert key a:** compute h₁(a)…hₖ(a), set all **k bits to 1**.
- **Lookup key b:** compute the k hashes; if **all k bits = 1** → *probably in S*; if **any bit = 0** → *definitely not in S*.
- **False-positive probability = (1 − e^(−km/n))^k** (derivation: a given bit is 0 after inserting m keys with k hashes with probability (1−1/n)^(km) ≈ e^(−km/n); all k checked bits being 1 has probability (1 − e^(−km/n))^k).
- **Numbers for the email example (m = 10⁹, n = 8×10⁹):** k=1 → 1−e^(−1/8) ≈ **0.1175**; k=2 → (1−e^(−1/4))² ≈ **0.0489**; k=3 ≈ **0.0306**; k=4 ≈ **0.0240**; k=5 ≈ **0.0217**; k=6 ≈ **0.0216**. **\[Supplement\] Optimal k = (n/m)·ln 2 ≈ 5.5** here. More hashes help only up to this point, after which the array fills with 1s.

```
INSERT 'kvs@pes.edu' (k=3):   bit array
 h1 -> 2   h2 -> 9   h3 -> 14      idx: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
                                       0 0 1 0 0 0 0 0 0 1  0  0  0  0  1
LOOKUP c: h(c) hits idx 4 (=0)  -> DEFINITELY NOT in set (slide exercise: c is spam)
LOOKUP b: all k bits = 1        -> POSSIBLY in set
```

**Extensions (slides):**

1. **Use secondary storage:** about 7/8 of spam hashes to a 0 bit (7B zeros out of 8B bits) and is filtered from memory; for the other **1/8** do a **disk lookup** to confirm non-spam.
2. **Cascaded Bloom filters:** put **two filters in series with different hash functions**; items passing the first (bit = 1) are checked by the second → **rejects much more spam**.

**Properties / limitations \[Supplement\]:** space-efficient, constant-time insert and lookup, **cannot delete** a key (clearing a bit may remove others – *counting Bloom filters* fix this), cannot list members, **false-positive rate rises as more keys are added**; **never** a false negative. **Applications:** spam/URL filtering, cache look-ups, database/Cassandra/HBase read avoidance, web crawlers (seen URLs), safe-browsing.

### 3.10.4 Counting distinct elements – Flajolet–Martin (FM) algorithm (★★)

**Motivation:** how many **distinct users** visit a site (Amazon has userid; Google has IP addresses – \~4 billion), or **distinct users per web page** (users × pages combinations). Exact counting needs storing every distinct element and comparing each new one with all → too much memory/time.

**Key definitions:**

- **Hash function** must produce **more distinct values than the number of elements** (IP addresses → hash range > 4 billion; URLs → 64 bits).
- **Tail length** of a hash value = **number of trailing 0s** (at the end of the binary string). Example: `11110100` has tail length **2**.
- **R = the maximum tail length** seen over all stream elements. **Estimate of distinct elements = 2^R.**

**Algorithm:**

1. Choose a hash function h mapping elements to bit strings of sufficient length.
2. For each stream element a: compute h(a), count its **trailing zeros** r(a).
3. Keep only **R = max r(a)** (just a few bits of state – no storing of elements!).
4. Output **2^R**.

- Duplicates hash to the **same** value so they never change R → it counts **distinct** elements automatically.

**Why it works (★ derivation):**

- A hash bit is 0 with prob ½. **P(h(a) ends in at least r zeros) = 2⁻ʳ** (last 1 bit zero = ½, last 2 bits = ¼, …). **P(tail length is exactly r) = 2⁻⁽ʳ⁺¹⁾.**
- With **m distinct elements**, **P(no element has tail length ≥ r) = (1 − 2⁻ʳ)^m ≈ e^(−m·2⁻ʳ)**, so **P(at least one element has tail length ≥ r) = 1 − e^(−m/2ʳ)**.
- **Three cases:**

| Case | m/2ʳ | P(at least one with tail ≥ r) | Meaning |
| --- | --- | --- | --- |
| **m ≫ 2ʳ** | very large | 1 − e^(−large) ≈ **1** | likely to find such tail lengths |
| **m ≈ 2ʳ** | moderate | some **fraction** | reasonable chance |
| **m ≪ 2ʳ** | very small | 1 − e^(≈0) ≈ **0** | **unlikely** to find r this large |

→ The largest r observed (R) cannot be much larger than log₂ m, and is not much smaller → **m ≈ 2^R**.

**Worked example (slide exercise).** Stream of user ids `10 10 7 10 6 14 14 12 6 5 7` (distinct = {5, 6, 7, 10, 12, 14}, so **true m = 6**). Hash = *cube the id, write as 12 bits, take the middle 6 bits* (bits 4–9):

| id | id³ | 12-bit | middle 6 bits | tail length |
| --- | --- | --- | --- | --- |
| 5 | 125 | 0000 0111 1101 | 001111 | 0 |
| 6 | 216 | 0000 1101 1000 | 011011 | 0 |
| 7 | 343 | 0001 0101 0111 | 101010 | 1 |
| 10 | 1000 | 0011 1110 1000 | 111010 | 1 |
| 12 | 1728 | 0110 1100 0000 | 011000 | **3** |
| 14 | 2744 | 1010 1011 1000 | 010111 | 0 |

**R = 3 → estimate = 2³ = 8** (true value 6). Repeated ids (10, 6, 7, 14) give identical hashes, so they do not change R.

**Practical improvements (★):**

- **Single hash problems:** the estimate is **always a power of 2**, and has **high variance** (one lucky element with a long tail doubles the estimate).
- **Use k hash functions**, get estimates 2^Rᵢ: **average** is pulled up by outliers (large 2^R); **median** is always a power of 2.
- **Combined approach:** split the k hashes into **groups**, take the **average within each group**, then take the **median of the group averages** (e.g. 6 hashes → 3 groups of 2: g₁=(h₁,h₂), …; average each group, then median of the 3 averages). Gives estimates that are not powers of 2 and are outlier-resistant.
- **Space:** only **O(log m)** bits per hash function \[Supplement\].

**FM vs Bloom vs Sampling (summary):**

|  | **Sampling** | **Bloom filter** | **Flajolet–Martin** |
| --- | --- | --- | --- |
| Question answered | Statistics on a representative subset | **Is x in set S?** (membership/filtering) | **How many distinct elements?** |
| Core trick | Hash the **key**; keep if hash < a (fraction a/b) | **k hashes set/check k bits** in an n-bit array | **Max trailing zeros R; estimate 2^R** |
| Error type | Sampling error | **False positives only** (no false negatives) | Estimate within factor; power of 2 |
| State | Sampled tuples | Bit array | A few bits (R) per hash function |

---

## UNIT 3 — EXAM KIT

### U3-A. Potential 4-mark questions with model answers

**Q1. Explain the limitations of MapReduce and how Spark overcomes them. (4)** *Model answer:* MapReduce (i) writes intermediate output and re-reads input from **HDFS on every job/iteration**, so disk I/O dominates; (ii) has **no native support for iterative or interactive workloads** – each iteration is a separate job; (iii) forces everything into **map + reduce**, so simple tasks need chained jobs; (iv) **cannot process real-time streams** and re-scans the whole input each job. **Spark** models a computation as a **DAG** of transformations (a single plan), keeps the **working set in memory** (`cache()`), reuses it across iterations/queries (>10× faster), recovers from failures using **RDD lineage** instead of replicating data, and offers **richer operators, SQL, streaming, MLlib and GraphX** in one engine.

**Q2. What is an RDD? List its properties and explain how it provides fault tolerance. (4)** *Model answer:* An **RDD (Resilient Distributed Dataset)** is an **immutable, partitioned, distributed collection** that can be processed in parallel. Its five properties: **(1) set of partitions, (2) list of dependencies on parent RDDs, (3) a function to compute a partition from its parents, (4) preferred locations (data locality), (5) a partitioner (hash/range)**. **Fault tolerance:** Spark records the **lineage** (the DAG of transformations that built each RDD). If a node fails, only the **lost partitions are recomputed** by replaying the transformations from the source (e.g. re-read one HDFS block, then filter→map) – no data replication needed. Immutability makes this recomputation deterministic.

**Q3. Differentiate transformations and actions with examples and explain lazy evaluation. (4)** *Model answer:* **Transformations** (`map, filter, flatMap, reduceByKey, join`) create a **new RDD** from an existing one and are **lazy** – Spark only records them in the lineage/DAG. **Actions** (`count, collect, reduce, take, saveAsTextFile`) **return a value to the driver or write output** and **trigger execution**. **Lazy evaluation:** nothing runs until an action is invoked; e.g. `lines = textFile(...)`, `errors = lines.filter(...)` do no work, `errors.count()` then executes everything block-by-block on each worker. Benefits: fewer passes over data, **pipelining**, whole-plan **optimisation**, no unnecessary intermediate results, saves driver–cluster trips.

**Q4. Explain narrow and wide dependencies with examples. Why do they matter? (4)** *Model answer:* **Narrow:** each parent partition is used by **at most one** child partition (`map, filter, flatMap, union, sample`) – no shuffle, **pipelined in one stage**, cheap recovery (recompute one parent partition). **Wide:** a parent partition feeds **many** child partitions (`reduceByKey, groupByKey, distinct, join, repartition`) – requires a **shuffle** (data movement between nodes), creates a **stage boundary**, costly recovery. The scheduler cuts the DAG into **stages at shuffle boundaries**, so minimising wide dependencies (e.g. **co-partitioning** two RDDs before repeated joins as in PageRank) reduces network traffic and speeds up jobs.

**Q5. Describe the Spark architecture and the roles of its components. (4)** *Model answer:* A Spark application = **one driver + many executors**, with resources from a **cluster manager** (Standalone, YARN, Mesos, Kubernetes). The **driver** runs `SparkContext`, maintains application info, builds the **DAG**, splits it into stages/tasks and **schedules** them using data locality. **Executors** are processes on worker nodes that **run tasks, cache data and report status** to the driver. The **cluster manager** controls physical machines and **allocates resources**. Spark Core (memory management, scheduling, fault recovery) sits on this; **Spark SQL, Streaming, MLlib, GraphX** are libraries on top; storage is external (HDFS, S3, Cassandra).

**Q6. Explain the Catalyst optimiser phases in Spark SQL. (4)** *Model answer:* Catalyst represents a query as a **tree** and rewrites it using **rules** in four phases: **(1) Analysis** – resolves unresolved attributes/relations using the **catalog**, assigns IDs, propagates/coerces types; **(2) Logical optimisation** – rule-based rewrites such as **constant folding, predicate pushdown, projection pruning, null propagation, boolean simplification**; **(3) Physical planning** – generates several physical plans and picks one with a **cost model**, pipelining filters/projections into one map; **(4) Code generation** – compiles the tree into **Java bytecode** using **Scala quasiquotes**. DataFrames and SQL share this same pipeline.

**Q7. Why were DataFrames introduced? Compare with RDDs. (4)** *Model answer:* RDDs are **opaque** – Spark cannot see inside the records, so users hand-code parsing/typing and nothing is optimised. A **DataFrame (2015)** is a **distributed collection with named columns (RDD + schema + DSL)**, enabling **SQL-style operations, Catalyst optimisation and code generation**, schema inference from JSON/Hive/Parquet, and early error detection. Advantages over plain SQL: **holistic optimisation across languages, real control structures (if/for), early analysis errors**. RDDs still give **low-level control** for unstructured data.

**Q8. Distinguish `reduceByKey` and `groupByKey`. (4)** *Model answer:* Both are wide pair-RDD transformations. `reduceByKey(f)` performs a **map-side combine** – values are merged **within each partition first**, so only partial results are shuffled; needs an **associative, commutative** function. `groupByKey()` **shuffles every (key, value) pair** and returns an iterable per key; it moves more data and can **overflow memory** for hot keys. Word count with `reduceByKey(_+_)` is therefore preferred over `groupByKey().map(sum)`; use `groupByKey` only if you truly need all values.

**Q9. Explain Bloom filter operation and its false-positive rate. (4)** *Model answer:* A Bloom filter is an **array of n bits** with **k hash functions**, used for **set membership** of a set S of m keys. **Insert:** hash the key with all k functions and **set the k bits to 1**. **Query:** hash the key; **if any bit is 0 → definitely not in S; if all are 1 → probably in S.** So **no false negatives, but false positives** occur with probability **(1 − e^(−km/n))^k**. Example: 10⁹ good e-mail ids, 8×10⁹ bits, k=1 → \~11.75% false positives; k=2 → \~4.9%. Used for spam filtering; extensions: disk look-up for the 1/8 'maybe' cases and **cascaded filters**.

**Q10. Explain the Flajolet–Martin algorithm for counting distinct elements. (4)** *Model answer:* Hash each stream element to a bit string (range larger than the number of elements). Define **tail length = number of trailing zeros**. Keep **R = maximum tail length** seen. Estimate **distinct count ≈ 2^R**. *Reason:* P(a hash ends in ≥ r zeros) = 2⁻ʳ, so with m distinct items the chance that some item has tail ≥ r is 1 − e^(−m/2ʳ): \~1 if m ≫ 2ʳ, \~0 if m ≪ 2ʳ; hence the largest r seen satisfies m ≈ 2ʳ. Duplicates have equal hashes so only **distinct** elements matter. Practical fix: use **many hash functions, average within groups, then take the median of the averages** to avoid power-of-2 and outlier problems. Memory: only R per hash function.

**Q11. Explain why naive stream sampling fails and how the refined algorithm works. (4)** *Model answer:* Naive: keep each tuple with prob 1/10. For a query issued twice, both copies are kept with probability **1/100** (one copy 18/100), so repeats are **under-detected** and the estimate d/(10s+19d) is biased. **Refined:** sample by **key (user)**, not tuple: hash the user to 0..9 and keep **all** tuples of users with hash 0 → a consistent **1/10 of users** with all their queries. Generalising: choose key attributes, hash to 0..b−1, keep tuple if **hash < a** → fraction **a/b**. If memory is full, lower a to a−1 and delete tuples whose key hashes to a−1.

**Q12. Explain the PySpark architecture and the lifecycle of a Spark program. (4)** *Model answer:* Spark's SparkContext lives in the **JVM** (one per JVM). **PySpark** uses **Py4J** to let the Python driver call the **Java SparkContext**; data is exchanged between Python and JVM using the **pickle serialiser**, and executors run Python worker processes. **Lifecycle:** create input RDDs (`textFile`/`parallelize`) → **lazily transform** them (`filter`, `map`) → `cache()` reused RDDs → call **actions** (`count`, `collect`) which trigger optimised parallel execution. Until an action is seen the driver only builds the DAG.

**Q13. Explain broadcast variables and accumulators. (4)** *Model answer:* Both are Spark **shared variables**. **Broadcast variables** cache a **read-only** value (e.g. a large lookup table) **once per machine** instead of shipping it with every task, using efficient broadcast algorithms; useful when multiple stages need the same data. **Accumulators** are variables workers can only **add** to using an **associative and commutative** operation (counters, sums, like MapReduce counters); only the **driver** can read the final value. Broadcast = driver→workers data distribution; accumulator = workers→driver aggregation.

### U3-B. Important definitions, facts, keywords, formulae, algorithms

| Term | One-line definition / fact |
| --- | --- |
| **Apache Spark** | Open-source unified in-memory engine for large-scale data processing; began 2009 at UC Berkeley |
| **RDD** | Immutable, partitioned, distributed collection with lineage; lazily evaluated |
| **Lineage** | DAG recording how an RDD was derived; used to recompute lost partitions |
| **Transformation / Action** | Lazy, returns new RDD / triggers execution, returns value |
| **DAG** | Directed Acyclic Graph: RDDs = nodes, operations = edges |
| **Shuffle** | Movement of data between nodes; caused by wide dependencies; creates a stage boundary |
| **Stage / Task / Job** | Job per action; stage = pipelined narrow ops between shuffles; task = work on one partition |
| **Driver / Executor** | Coordinator + scheduler / worker JVM that runs tasks and caches data |
| **SparkContext** | Entry point to cluster; one per JVM; `SparkSession` unified entry for SQL |
| **Py4J** | Bridge between Python interpreter and Java objects in the JVM |
| **DataFrame** | Distributed collection with named columns = RDD + schema + DSL; 2015 |
| **Catalyst** | Tree-based rule-driven query optimiser of Spark SQL (analysis, logical opt, physical planning, code gen) |
| **UDF** | User-defined function registered inline, callable from SQL and DataFrame code |
| **Broadcast / Accumulator** | Read-only shared value / add-only shared counter |
| **Bloom filter** | n-bit array + k hashes for membership; false positives, no false negatives |
| **Tail length / R / 2^R** | Trailing zeros of hash / max tail length / FM estimate of distinct count |

**Formulae**

- **Bloom false-positive rate:** `(1 − e^(−km/n))^k` (n bits, m keys, k hashes); optimal `k ≈ (n/m) ln 2`.
- **FM:** `P(tail ≥ r) = 2⁻ʳ`; `P(no element has tail ≥ r) = (1 − 2⁻ʳ)^m ≈ e^(−m/2ʳ)`; **estimate = 2^R**.
- **Sampling:** fraction **a/b** ⇒ hash key to 0..b−1, keep if `hash < a`; naive duplicate detection probability = (1/10)² = 1/100.
- **Hash-partitioning:** `partition = hash(key) mod numPartitions`.

### U3-C. Frequently confused concepts – comparison tables

| Confusion | Distinguish |
| --- | --- |
| **map vs flatMap** | 1 output per input vs 0..n outputs flattened |
| **map vs mapPartitions** | function per element vs per partition (iterator) |
| **reduce vs reduceByKey** | **action** on whole RDD returning a value vs **transformation** per key returning RDD |
| **reduce vs fold vs aggregate** | no zero, fails on empty / zero (identity), same type / zero + seqOp + combOp, different result type |
| **groupBy vs groupByKey** | key computed by a function vs key already in the pair |
| **count vs countByValue vs countByKey** | number of elements / map element→count / map key→count (pair RDD). All actions |
| **cache vs persist** | cache = persist with default MEMORY\_ONLY; persist lets you choose a storage level |
| **collect vs take** | all elements (must fit in driver) vs first n |
| **union vs distinct** | union keeps duplicates; distinct removes (needs shuffle) |
| **Spark vs Hadoop vs Scala** | engine in memory vs storage+MapReduce disk vs language |
| **SparkContext vs SparkSession** | core entry point (RDDs) vs unified entry (SQL/DataFrame) |
| **DataFrame vs RDD** | schema + Catalyst vs opaque objects |
| **Narrow vs wide** | no shuffle vs shuffle |
| **Bloom vs FM** | membership (is it present?) vs cardinality (how many distinct?) |
| **val vs var (Scala)** | immutable vs mutable |
| **Shark vs Spark SQL** | Hive on Spark (translated HiveQL) vs redesigned relational engine with DataFrames/Catalyst |

### U3-D. Practice MCQs – 2 marks each (10)

**1.** Four distinct elements of a stream have hash tail lengths 3, 0, 5 and 2. The Flajolet–Martin estimate of distinct elements is: (a) 5 (b) 10 (c) 32 (d) 11 **Ans: (c)** R = max = 5 → 2⁵ = 32. (Not the sum, not R itself.)

**2.** A Bloom filter has n = 8×10⁹ bits, stores m = 10⁹ keys and uses k = 2 hash functions. The false-positive probability is approximately: (a) 11.75% (b) 4.9% (c) 2.4% (d) 0.5% **Ans: (b)** (1 − e^(−2×10⁹/8×10⁹))² = (1 − e^(−0.25))² = 0.2212² ≈ 0.049. (a) is k = 1, (c) is k = 4.

**3.** A search stream is sampled by keeping each tuple independently with probability 1/10. For a query issued exactly twice, the probability that **both** copies are sampled and the probability that **exactly one** is sampled are: (a) 1/10 and 9/10 (b) 1/100 and 18/100 (c) 1/100 and 9/100 (d) 1/10 and 1/100 **Ans: (b)** both = (1/10)²; exactly one = 2·(1/10)(9/10) = 18/100 → the repeat usually looks like a singleton.

**4.** What does this PySpark code return? `sc.parallelize([1,2,3,4,5,6]).filter(lambda v: v%2==0).map(lambda v: v*v).reduce(lambda a,b: a+b)` (a) 20 (b) 56 (c) 91 (d) 12 **Ans: (b)** evens 2,4,6 → squares 4,16,36 → sum 56. `reduce` is the only action → one job, filter+map pipelined in one stage.

**5.** `x = sc.parallelize(['a b c', 'd e'])`. Then `len(x.map(lambda l: l.split()).collect()) + len(x.flatMap(lambda l: l.split()).collect())` equals: (a) 5 (b) 7 (c) 10 (d) 2 **Ans: (b)** map → 2 elements (two lists); flatMap → 5 words; 2 + 5 = 7.

**6.** A job does `textFile → flatMap → map → reduceByKey → mapValues → filter → collect`. The number of stages is: (a) 1 (b) 2 (c) 3 (d) 4 **Ans: (b)** only `reduceByKey` is a wide dependency → one shuffle → two stages.

**7.** `r = sc.textFile(p).map(f); r.cache(); r.count(); r.first(); r2 = r.filter(g)` – how many Spark jobs are launched? (a) 1 (b) 2 (c) 3 (d) 4 **Ans: (b)** `count` and `first` are actions; `map`, `cache`, `filter` are lazy.

**8.** `sc.parallelize([1,2,3,4]).aggregate((0,0), lambda acc,v:(acc[0]+v, acc[1]+1), lambda a,b:(a[0]+b[0], a[1]+b[1]))` returns: (a) (10, 4) (b) (10, 10) (c) (4, 10) (d) 10 **Ans: (a)** (sum, count) = (10, 4) → mean 2.5. Result type differs from element type → that is why `aggregate` is used.

**9.** Catalyst rewrites the expression `x + (1 + 2)` as `x + 3`. This happens in which phase and is called: (a) Analysis – type coercion (b) Logical optimisation – constant folding (c) Physical planning – cost-based selection (d) Code generation – quasiquotes **Ans: (b)** rule-based tree rewrite in logical optimisation.

**10.** From a stream of `(user, item, time)` you need a 3/20 sample of **users** with all their activity. Best method: (a) keep a tuple if hash(user,item,time) < 3 over 0..19 (b) keep a tuple if hash(user) < 3 over 0..19 (c) keep a tuple if hash(user) = 3 over 0..19 (d) keep each tuple with random probability 3/20 **Ans: (b)** key = user only; threshold a=3, b=20. (c) gives 1/20; (a) and (d) break the per-user consistency.

### U3-E. Practice MCQs – 1 mark each (26)

1. Spark originated at: (a) MIT (b) UC Berkeley (c) Google (d) Yahoo — **(b)**; 2009 RAD Lab/AMPLab.
2. RDD stands for: (a) Random Data Distribution (b) Resilient Distributed Dataset (c) Reliable Disk Data (d) Replicated Distributed Database — **(b)**.
3. Which is an **action**? (a) map (b) filter (c) reduce (d) flatMap — **(c)**; others are transformations.
4. Which is NOT a component of the RDD interface? (a) partitions (b) dependencies (c) compute function (d) primary-key index — **(d)**; the five are partitions, dependencies, compute, preferred locations, partitioner.
5. Lost RDD partitions are recovered using: (a) HDFS replicas of the RDD (b) lineage (c) checkpoint only (d) Zookeeper — **(b)**.
6. A narrow transformation is: (a) groupByKey (b) reduceByKey (c) filter (d) distinct — **(c)**.
7. A wide transformation is: (a) map (b) filter (c) union (d) groupByKey — **(d)**.
8. Number of SparkContexts allowed per JVM: (a) one (b) two (c) unlimited (d) one per thread — **(a)**.
9. Py4J is used to: (a) compress data (b) let Python access Java objects in the JVM (c) schedule tasks (d) store RDDs — **(b)**.
10. In lazy evaluation, transformations are executed: (a) immediately (b) when an action is called (c) at compile time (d) at cluster start — **(b)**.
11. DataFrames were introduced in: (a) 2009 (b) 2012 (c) 2015 (d) 2020 — **(c)**.
12. A DataFrame is best described as: (a) RDD + schema (b) HDFS file (c) key-value store (d) graph — **(a)** (RDD + schema + DSL).
13. Catalyst is: (a) a cluster manager (b) Spark SQL's query optimiser (c) a storage format (d) a streaming engine — **(b)**.
14. The Catalyst phase that uses a cost model: (a) analysis (b) logical optimisation (c) physical planning (d) code generation — **(c)**.
15. Catalyst code generation relies on: (a) Scala quasiquotes (b) JNI (c) Py4J (d) Hadoop streaming — **(a)**.
16. Broadcast variables are: (a) writable by workers (b) read-only, cached on each machine (c) stored only on the driver (d) disk-only — **(b)**.
17. Accumulators can be: (a) read by workers (b) only added to by workers, read by the driver (c) deleted by tasks (d) used only for strings — **(b)**.
18. `cache()` on an RDD uses storage level: (a) DISK\_ONLY (b) MEMORY\_ONLY (c) OFF\_HEAP (d) MEMORY\_AND\_DISK\_2 — **(b)**.
19. Which action works on an empty RDD using a zero value? (a) reduce (b) fold (c) first (d) max — **(b)**; reduce throws an exception.
20. A Bloom filter can produce: (a) false negatives only (b) false positives only (c) both (d) neither — **(b)**.
21. Bloom false-positive formula is: (a) (1 − e^(−km/n))^k (b) e^(−km/n) (c) 2^(−r) (d) (1 − 2^(−r))^m — **(a)**.
22. Tail length of the hash `10110000` is: (a) 1 (b) 2 (c) 4 (d) 8 — **(c)** four trailing zeros.
23. FM estimates the number of distinct elements as: (a) R (b) 2R (c) 2^R (d) R² — **(c)**.
24. P(a hash value ends in at least r zeros) is: (a) 1/r (b) 2⁻ʳ (c) r/2 (d) 2^r — **(b)**.
25. To sample a/b of a stream by key you keep a tuple if: (a) hash(key) < a (hash range 0..b−1) (b) hash(tuple) = 0 (c) random() < b (d) key mod a = 0 — **(a)**.
26. Which is NOT a Spark cluster manager? (a) Standalone (b) YARN (c) Mesos (d) HDFS — **(d)** HDFS is storage.
27. Scala was designed by: (a) James Gosling (b) Martin Odersky (c) Guido van Rossum (d) Matei Zaharia — **(b)**, at EPFL.
28. In Scala, `val` declares: (a) a mutable variable (b) an immutable value (c) a method (d) a class — **(b)**; `var` is mutable.
29. `union` of `[1,2,3]` and `[3,4]` gives: (a) `[1,2,3,4]` (b) `[1,2,3,3,4]` (c) `[3]` (d) `[1,2,4]` — **(b)** duplicates are not removed.
30. In a repeated join such as PageRank, shuffles can be avoided by: (a) broadcasting ranks (b) co-partitioning both RDDs with the same partitioner (c) using collect (d) repartition(1) — **(b)**.

### U3-F. UNIT 3 REVISION SHEET (read this last)

- **Spark** = in-memory unified engine (2009, Berkeley). Fixes MapReduce: disk I/O, no iteration, no real-time. Stack: **Core + SQL + Streaming + MLlib + GraphX**. No own storage / cluster manager.
- **Programming model:** create RDD (`textFile`, `parallelize`) → lazy transformations → `cache()` → action. **SparkContext** one per JVM; PySpark = **Py4J** + pickle.
- **RDD 5 properties:** partitions, dependencies, compute, preferred locations, partitioner. Immutable + lineage = fault tolerance.
- **Transformations (lazy):** map, flatMap, filter, groupBy, groupByKey, reduceByKey, mapPartitions, sample, union, join, distinct, keyBy. **Actions:** collect, count, take, top, reduce, fold, aggregate, countByKey, countByValue, saveAsTextFile, sum/mean/max/stdev.
- **reduceByKey** (combine before shuffle) ≫ **groupByKey**.
- **Narrow:** map, flatMap, filter, union, sample, mapPartitions. **Wide:** groupByKey, reduceByKey, distinct, join, intersection, cartesian, repartition. Stage boundary at each shuffle. **Co-partition** to avoid join shuffle.
- **Architecture:** Driver (DAG, schedule) → Cluster manager (Standalone/YARN/Mesos/K8s) → Executors (tasks, cache). Delay scheduling for locality.
- **Shared variables:** broadcast (read-only), accumulator (add-only).
- **DataFrame** (2015) = RDD + schema + DSL; **Catalyst:** analysis → logical opt → physical planning (cost model) → code gen (quasiquotes). Rules: constant folding, predicate pushdown, projection pruning.
- **Relational ops:** filter=σ, select=π, join=⋈, groupBy+agg=γ, union, distinct, orderBy; SQL via temp views; UDFs inline.
- **Streaming algorithms:** Sampling → hash key, keep if hash < a of b (naive tuple sampling detects a repeat with prob 1/100). **Bloom:** n bits, k hashes, FPR (1−e^(−km/n))^k, no false negatives; cascade/secondary storage. **FM:** tail length = trailing zeros, R = max, estimate 2^R; combine with averages in groups + median. Example: ids {5,6,7,10,12,14} → R=3 → 8.
- **Scala:** OO + functional, static typing, type inference, val/var, traits, JVM.

---

# UNIT 4 — STREAMING ANALYSIS AND ADVANCED ANALYTICS ON BIG DATA

## 4.1 Streaming Analytics – Concepts and Use Cases

### 4.1.1 What is streaming analytics?

- **Streaming analytics** = gaining **real-time insights from continuous data streams**, enabling **faster decisions and actions**. Data is processed **as it arrives** (event by event or in small batches) instead of being stored first and analysed later.
- **Examples of streaming data (slides):** ocean temperature/GPS sensors (1 sensor per 150 sq. miles → \~1,000,000 sensors × 10 readings/sec ≈ **3.5 TB/day**); London's **6 million video cameras**; Google **billions of queries/day**; Yahoo **billions of clicks/day**.
- **Applications needing near-real-time results (slides):** social-network trends, website statistics, **intrusion detection**, transport systems (**Uber**) – need large clusters and **latencies of a few seconds**.

| Batch analytics | Streaming analytics |
| --- | --- |
| Bounded, stored data; high latency (minutes–hours) | **Unbounded**, continuous data; **low latency (ms–seconds)** |
| Run periodically over complete data | **Standing queries** run continuously |
| Examples: daily/monthly reports (Hadoop) | Examples: fraud alert, live dashboards |

**Stream queries recap:** **standing query** (continuously running, e.g. *alert when temperature > threshold*, *alert if visit from blacklisted country*) vs **ad hoc query** (arbitrary, needs stored window, e.g. *average of last n readings (n unknown in advance)*, *countries visiting in last year*).

### 4.1.2 Streaming analytics use cases (from the use-case deck) ★

| Use case | How streaming is used | Key points |
| --- | --- | --- |
| **Log analysis** | Distributed real-time processing of log streams; query continuous streams, create **new streams** for further use | Detect **anomalies/incidents before they reach the user**; router/network logs show **router attack detection**, application internet activity; alerts raised as events occur. Tools: **Splunk, Elasticsearch** (with Upsolver). Most common IT-operations use case |
| **Fraud detection** | Analyse **streaming transaction data** to spot anomalies and **stop fraudulent transactions before completion** | **Machine-learning, e.g. binary classification (fraud / not fraud)** on patterns in real time; notify via **email, SMS, social media**; applies across industries |
| **Cyber security** | Detect anomalies in network/traffic streams to **isolate threats in real time** | Identify a **DDoS attack** – suspicious volume of traffic from a **single IP address** or from one profile; stream processing can **filter data before storage** and feed a larger analytics platform |
| **Sensor / IoT data** | Real-time processing of device data | **Aircraft**: fault insights as they occur → fewer maintenance delays, better **flight safety**; **oil & gas**: monitor temperature, pressure to ensure **integrity of production and refining**; accurate **inventory and supply-chain management** |
| **Online advertising** | Track **user behaviour, clicks and interests** in real time | Promote **ads/sponsored content** the user is likely to be interested in (social networks) |
| **Database migration** | **Streaming architecture** to move **on-prem DBs → cloud data lake** | **Schema-on-read, built-in transforms, automatic partition & compaction, native CDC (change data capture)** for near-real-time latest view; minimises processing complexity |
| **Others (intro slide)** | Real-time **customer-behaviour analysis** (personalised marketing/support), **supply-chain optimisation**, **predictive maintenance** in manufacturing | — |

**Exam tip:** for a '4-mark use-case' question pick **any 3–4 use cases** and give *what is streamed → what is detected → benefit* (e.g. credit-card transactions → binary classifier → block fraud before completion).

---

## 4.2 Apache Kafka – Use Cases and Architecture (★★)

### 4.2.1 Need for data pipelines

- **Event:** something that happens at a specific time, one at a time (e.g. *'Student 23489 entered building'* when an ID is scanned). Stream processing needs events delivered to compute servers.
- Data sources are **varied, stream at different rates, are light-weight (cannot compute)**; analysis happens on servers → data must be **transferred from sources to compute servers**.
- **Point-to-point** connections are simple but break down: multiple sources (cameras), multiple backends (e.g. stock-trading analysts analysing the same data differently) → **N clients × M backends = N×M connections to build and maintain**; a failing connection or new service touches everything.
- **Backends:** Hadoop, security, real-time, analytics systems.

```
WITHOUT Kafka (N x M)                 WITH Kafka (N + M)
 S1 --\ /--> B1                        S1 --\            /--> B1
 S2 ----X----> B2                      S2 ----> KAFKA ---+--> B2
 S3 --/ \--> B3                        S3 --/            \--> B3
```

**Why Kafka?** An **intermediary** that connects different sources and multiple backends and **decouples** the pipeline: **producers and consumers do not need to know about each other.**

### 4.2.2 Publish–subscribe model (three essential elements)

1. **Publisher (producer):** decides **what data to send**, packages it into a **message** and **publishes** it to the communication infrastructure; the simplest part; **knows nothing about subscribers**.
2. **Subscriber (consumer):** tells the infrastructure the **type/category of messages** it wants (**not who** to receive them from); messages are **delivered by the infrastructure**; **knows nothing about publishers**.
3. **Communication infrastructure (broker cluster):** the **most complex part**; decides **which messages go to which subscribers**.

### 4.2.3 Routing models (★ MCQ trap)

|  | **Topic-based** | **Content-based** | **Pattern-based (Kafka's variant)** |
| --- | --- | --- | --- |
| How | Publishers label messages with a **topic**; subscribers subscribe to topics and receive **all** messages on them | Subscribers give **matching criteria** on message content | Subscribe using a **wildcard/regex on topic names** |
| Example | All fire sensors in B-block | Ads featuring a given celebrity | All topics matching `ipl*` |
| In Kafka? | **Yes (primary)** | **No – not supported** | **Yes (simpler version)** |

**Pros/cons of this decoupled communication:** (+) **no hard-wired connections**; **flexible** – easy to add/remove publishers or subscribers. (−) **design and maintenance of topics**; **performance overhead** of the infrastructure. **Exercise (bookstore portal):** activities login/list/get details/buy/status/return/logout with backends Security, Order processing, Book info → **topic-based**, each message type = a topic.

### 4.2.4 Topics, messages, partitions (★★)

- **Topic:** a **named logical category** (e.g. `shopping-cart-done`, `user-signups`); technically a **log data structure – an ordered, append-only sequence of records stored on disk**; **multi-tenant** (many producers write, many consumers read).
- **Kafka message (record) fields:** **Key** (used for **partitioning**), **Value** (actual **payload**), **Offset** (position in the partition), **Size**, **Checksum** (detect corruption), **Magic byte** (message-format version), **Compression codec** (**Snappy, GZip, LZ4**), **Timestamp** (set by the **producer at send time or by the broker at arrival**, depending on config).
- **Partition = physical storage unit:** each topic is split into **partitions** – independent, **immutable, append-only logs** distributed across **broker nodes**; this **bypasses single-server disk and I/O limits** (scalability) – e.g. a topic split into 3 partitions: P0 next write at offset 12, P1 at 9, P2 at 12. **Each message in a partition gets a unique, increasing offset.**
- **Partition assignment of a message:** **key present → `hash(key) % N`** (N = #partitions) so the **same key always goes to the same partition** (ordering per key); **no key → round-robin / sticky** distribution to balance load.
- **Ordering guarantee:** only **within a partition**, not across partitions \[Supplement\].
- **Consumers track the offset they have read;** comparing their offset with the partition's latest offset tells them if new messages are waiting.

### 4.2.5 Fault tolerance – replication

- **Reliability by replication** (like HDFS): each **partition is copied across multiple brokers.**
- **Leader** handles **all reads and writes** of the partition; **followers replicate the leader**; if the leader fails a **new leader is elected**.
- **ISR = in-sync replicas.** **Durability levels (`acks`):** **`acks=1`** – leader only acknowledges (no follower ack; fast, small loss risk); **`acks=-1/all`** – leader waits for the **ISR quorum** to ack (**recommended**, safest). \[Supplement\] `acks=0` = fire-and-forget.
- **Recommended:** **replicas = 3, quorum = 2.**

### 4.2.6 Consumers and consumer groups (★)

- **Consumer group:** typically **multiple instances of one application**; **each partition's messages are delivered to ONE consumer within the group** (**load balancing**); **different groups each receive all messages** (publish-subscribe across groups, queue-like inside a group).
- **Exercise 4 (4 partitions P0–P3):** Group A with 2 consumers → **C1: P0, P3; C2: P1, P2** (each gets 2). Group B with 4 consumers → each gets exactly 1 partition (e.g. C3: P0, C4: P3, …). **If consumers > partitions, the extra consumers stay idle** (max useful parallelism = #partitions).
- **Exercise 5 (1 topic, 3 servers, 3 partitions, 3 replicas, group of 3):**

```
        Server 0         Server 1         Server 2
        P0(L) P1  P2     P0  P1(L) P2     P0  P1  P2(L)     (L = leader, others = follower replicas)
          |                  |                 |
         C0                 C1                C2             <- consumer group, one partition each
```

(every server holds a replica of every partition; leaders are spread across servers; each consumer reads its partition from the leader).

### 4.2.7 Kafka servers: broker and controller roles; ZooKeeper vs KRaft

- A **Kafka server** = a process/machine running Kafka; it can have the **Broker role** (**stores data**, serves produce/consume requests, hosts primary and replica partition logs) and/or the **Controller role** (**manages cluster metadata, elects partition leaders**, acts as administrative authority).

|  | **ZooKeeper mode (older)** | **KRaft mode (newer)** |
| --- | --- | --- |
| Who picks the controller | External coordinator **ZooKeeper** | Kafka servers themselves via **consensus (Raft-based election)** |
| Requirement | **ZooKeeper mandatory** | **No ZooKeeper** |
| Versions | Older Kafka | **Newer Kafka** |

### 4.2.8 Kafka use cases

- **Slide list ('Who uses Kafka'):** **LinkedIn** – activity data and operational metrics (Kafka was created there); **Twitter** – part of its **Storm** stream-processing infrastructure; **Square** – **bus to move all system events** (logs, custom events, metrics) to data centres, outputs to **Splunk, Graphite, Esper-like alerting**; also **Spotify, Uber, Tumblr, Goldman Sachs, PayPal, Box, Cisco, CloudFlare, DataDog, LucidWorks, MailChimp, Netflix**.
- **\[Supplement\] Typical use patterns:** (1) **messaging / decoupling** microservices; (2) **website activity tracking** (clicks, searches); (3) **log and metrics aggregation**; (4) **stream-processing source/sink** for Spark/Flink/Storm; (5) **event sourcing / commit log**; (6) **real-time data pipelines** into Hadoop/warehouses.

### 4.2.9 Kafka vs traditional point-to-point messaging \[Supplement\]

|  | Point-to-point connections | Kafka |
| --- | --- | --- |
| Connections | N×M | N + M via broker |
| Coupling | Tight | Loose (publish–subscribe) |
| Storage/replay | None | **Persistent log**, consumers can re-read from any offset |
| Scaling | Hard | **Partitions** + consumer groups |
| Fault tolerance | Per connection | **Replication + leader election** |

---

## 4.3 Streaming Spark (Spark Streaming, DStreams) (★★)

### 4.3.1 Requirements for a streaming framework (slides, built up step-by-step)

Scalable to large clusters • **second-scale latencies** • **simple programming model** • **integrated with batch & interactive processing** • **efficient fault tolerance in stateful computations**.

**Can we use Hadoop? (Exercise 2):** stream of stock records `num_stock`; `Find_max` updates global variable `Max_num_stock`: `if num_stock > Max_num_stock: Max_num_stock = num_stock`. Problems: Hadoop processes a **whole file in the map, not one record at a time**; `Max_num_stock` is **global state** that depends on the **entire stream**; assuming it runs on a single node is **unreasonable** (transactions can exceed one node). **Case study Conviva:** real-time monitoring of online-video metadata (HBO, ESPN, ABC…): 1000s of metrics, millions of sessions, dozens of nodes; had to build a **custom stream system + a Hadoop backend** → **twice the code, twice the bugs**.

**Traditional stateful stream systems:** **record-at-a-time, event-driven**; each node holds **mutable state**; **state is lost if a node dies** → fault tolerance is hard. **Storm:** replays records not processed → **at-least-once**, may update state **twice**. **Trident** (on Storm): transactions → **exactly-once** but **slow per-state transaction updates**.

### 4.3.2 What is Spark Streaming? Discretised streams

- Framework for **large-scale stream processing**: scales to **100s of nodes**, **second-scale latency**, integrates with Spark batch/interactive, **simple batch-like API**, ingests from **Kafka, Flume, ZeroMQ, Twitter, HDFS, TCP sockets**.
- **Idea – Discretised Stream Processing (micro-batching):** **chop the live stream into batches of X seconds**; Spark treats **each batch as an RDD** and processes it with normal RDD operations; results are returned **in batches**. **Batch sizes as low as ½ second, end-to-end latency ≈ 1 second.** Run the streaming computation as **a series of very small, deterministic batch jobs.**

```
 live stream --> [Spark Streaming: splits into batches of X sec] --> RDD@t, RDD@t+1, RDD@t+2 ...
                                  --> [Spark engine] --> processed results (in batches)
```

- **Exercise 4 insight:** Hadoop could be made to do 1-second updates by **batching input into an HDFS file every second** and running MapReduce – the same idea Spark Streaming implements efficiently in memory.

### 4.3.3 DStream (★)

- **DStream (Discretised Stream) = a sequence of RDDs**, one per batch interval; any transformation on a DStream is applied **to each RDD**, producing a new DStream.
- **Hashtag example (Twitter):**

```scala
val tweets   = ssc.twitterStream(<user>, <pwd>)         // DStream of tweets: batch@t, t+1, t+2 (each an RDD in memory)
val hashTags = tweets.flatMap(status => getTags(status))  // transformation: new DStream
hashTags.saveAsHadoopFiles('hdfs://...')                 // output operation: every batch saved to HDFS
```

- **Spark vs Spark Streaming program:** the **same code** works on a log file (`sc.hadoopFile`) and on a stream (`ssc.twitterStream`/`kafkaStream`) → **'one stack to rule them all'**: explore interactively in `spark-shell`, reuse in batch programs, reuse in streaming.
- **PySpark equivalent \[Supplement\]:**

```python
from pyspark import SparkContext
from pyspark.streaming import StreamingContext
sc = SparkContext('local[2]', 'WC'); ssc = StreamingContext(sc, 1)      # 1-second batches
lines  = ssc.socketTextStream('localhost', 9999)
counts = lines.flatMap(lambda l: l.split()).map(lambda w: (w, 1)).reduceByKey(lambda a,b: a+b)
counts.pprint(); ssc.start(); ssc.awaitTermination()
```

### 4.3.4 DStreams, receivers and job execution

- Every DStream is associated with a **Receiver** that **reads data from a source and stores it in Spark memory** for processing. A receiver runs on an **executor as a long-running task**; the **driver launches tasks to process the received blocks every batch interval.**
- **Sources:** **Basic** – file systems, sockets; **Advanced** – **Kafka, Flume** (and Twitter). Output operations: `saveAsHadoopFiles`, `foreach`, `print`.
- **Transformations:** standard RDD ops (`map, countByValue, reduce, join`) + **stateful** ops (`window, countByValueAndWindow`).

### 4.3.5 Stateless transformations

- Applied to **each batch independently; no information carried between batches** (`map, flatMap, filter, repartition, reduceByKey, groupByKey`). Example: **total shares sold per company in each batch/minute** – `reduceByKey` on the `<company, shares>` DStream; every batch is processed independently, **no state across time**.

### 4.3.6 Stateful transformations (★★)

- Needed when **state must persist across batches** (e.g. **maximum stock sold across the whole day** per company). **Data must be in pair RDDs (key, value)** so there is **state per key**. Two options: **window operators** (state over **short periods**) and **session-based state** (**longer** periods).

**Window operations:** `hashTags.window(Minutes(10), Seconds(1)).countByValue()` – **window length = 10 min** (how much history), **sliding interval = 1 s** (how often computed). Naively recomputes the count over **all data in the window every slide**.

**Smart (incremental) window:** `hashTags.countByValueAndWindow(Minutes(10), Seconds(1))` → **add counts of the new batch entering the window, subtract counts of the batch leaving** → much more efficient. General form: `reduceByKeyAndWindow(_ + _, _ - _, Minutes(1), ...)` — requires an **'inverse reduce' function** (subtract for counting).

**Session-based state:** `tweets.updateStateByKey(updateMood)` – keeps **arbitrary per-key state** (e.g. per-user mood). **RDD must be (key, value)**, key = user. When user *Dinkar* (current mood *Happy*) tweets *'Eating icecream'*, Spark passes (Happy, tweet) to `updateMood`, which returns **VeryHappy**, and stores it as Dinkar's new state.

|  | **Stateless** | **Stateful (window / updateStateByKey)** |
| --- | --- | --- |
| Memory of past batches | None | Yes (per key / per window) |
| Needs checkpointing | No | **Yes** |
| Failure recovery | Just recompute the batch | Need state from previous batches |
| Examples | `map, filter, reduceByKey` per batch | `window, countByValueAndWindow, updateStateByKey` |

### 4.3.7 Fault tolerance and checkpointing (★)

- **All intermediate data are RDDs**, so lost partitions can be **recomputed via lineage**. **Input batches are replicated in the memory of multiple worker nodes**; data lost on worker failure is recomputed from the replicated input on other workers.
- **Stateless:** history not required – simply recompute. **Stateful:** state from previous batches needed → **how much state to retain?**
- **Checkpointing:** for long-running stateful streams lineage would grow forever; a **checkpoint saves an RDD (e.g. hashTags and tagCounts at time t+2) to reliable storage and forgets the earlier lineage.** (Hands-on: create a `checkpoint_BIGDATA` directory with write permission.)

### 4.3.8 Performance claims (slides)

- **6 GB/s (60 M records/s) on 100 nodes at sub-second latency** (100 streams, 100 EC2 instances × 4 cores).
- **Throughput per node:** **Spark Streaming 670k records/s**, **Storm 115k**, **Apache S4 7.5k**.
- **Recovers from faults/stragglers within 1 second.**
- **Applications:** **Conviva** (1–2 s latency, millions of sessions, scales linearly), **Mobile Millennium** (traffic transit-time estimation using online ML on GPS data; Markov-chain Monte Carlo, CPU-intensive, scales linearly).

### 4.3.9 Limitations

- Works in **batches → near-real-time, not true real-time (record-at-a-time)**; may be **unacceptable for strict low-latency scenarios** (→ motivation for **Flink**). \[Supplement\] DStream API is now legacy; **Structured Streaming** is the newer DataFrame-based API.

### 4.3.10 Hands-on (Streaming Spark) – summary

- **Problem:** from a tweets data set (fields ID, language, Date, source, len, likes, RTs, Hashtags, Usernames, …) compute **(1) total number of tweets**, **(2) number sent with 'Twitter for Android'.**
- **Steps:** (i) `mkdir checkpoint_BIGDATA; chmod 777 checkpoint_BIGDATA`; (ii) `python3 app.py` streams data over a **socket**; (iii) `spark-submit spark.py` receives and processes it; (iv) stdout/stderr get mixed – redirect: `spark-submit spark.py > output.txt`. Setup: Hadoop 3.2, Java 1.8, Spark 3.0; `export SPARK_HOME=/usr/local/spark`.

### 4.3.11 Spark Streaming vs Storm vs Hadoop MapReduce

|  | Hadoop MapReduce | Storm | Spark Streaming |
| --- | --- | --- | --- |
| Model | Batch (files) | **Record-at-a-time** | **Micro-batch (DStream of RDDs)** |
| Latency | Minutes+ | Milliseconds | \~**Seconds** (½–1 s) |
| Fault tolerance | Re-run tasks | **At-least-once** (Trident: exactly-once but slower) | **RDD lineage + replicated input + checkpoints** (exactly-once for state) |
| Throughput/node | — | 115k rec/s | **670k rec/s** |
| Integration | — | Separate from batch | **Same API as batch/interactive Spark** |

---

## 4.4 Apache Flink (★★)

### 4.4.1 What is Flink and why use it?

- **Apache Flink** = framework for processing big data in **real-time (streaming) and batch modes**; designed for **low-latency, real-time data streaming**; handles **both streaming and batch**. *Simple use case:* a stock-market app giving **live price updates every second**.
- **Core philosophy:** **everything is a stream.** Any data (credit-card transactions, sensor measurements, machine logs, user interactions) is produced as a **stream of events**; **batch = a bounded stream.**

**Flink vs other big-data tools (slide):** **Hadoop → batch processing**; **Spark → batch and micro-batch**; **Flink → low-latency real-time streaming** (native, record-at-a-time). **Takeaway: use Flink when real-time processing is needed.**

| Feature | Hadoop MapReduce | Spark (Streaming) | **Flink** |
| --- | --- | --- | --- |
| Primary model | Batch, disk | Batch + **micro-batch** streaming | **True streaming** (batch as bounded stream) |
| Latency | High | Seconds | **Very low (ms)** |
| Time semantics | — | Mostly processing time (DStream) | **Event time + processing time** |
| State | None | Per-batch/window state (updateStateByKey) | **First-class managed state, exactly-once checkpoints** |
| APIs | Map/Reduce | RDD, DataFrame, DStream | **DataStream API (streaming), DataSet API (batch)** |

### 4.4.2 Key concepts

- **Streaming:** continuous flow (live tweets, sensor data). **Batch processing:** static data (daily sales).
- **APIs:** **DataStream API** for streaming data; **DataSet API** for batch data. \[Supplement\] In newer Flink releases the DataSet API is deprecated/removed and the DataStream (and Table/SQL) API handles both bounded and unbounded data.

### 4.4.3 Architecture (simplified) (★)

```
   Client --submits job (REST)--> JOB MANAGER  ('planner' / 'project manager')
                                   |  breaks job into tasks, schedules them,
                                   |  coordinates checkpoints & recovery
                  +----------------+----------------+
                  v                v                v
            TASK MANAGER 1   TASK MANAGER 2   TASK MANAGER 3   ('workers' / 'team members')
            [task slots]     [task slots]     [task slots]      execute tasks, hold local state
```

- **Job Manager:** the **'planner' that organises tasks** – breaks down the job. **Task Managers:** the **'workers' that execute tasks** – process the data. *Analogy:* Job Manager = project manager; Task Managers = team members.

### 4.4.4 Four important aspects of the architecture (slide list)

1. **Process unbounded and bounded data**
   - **Unbounded streams:** have a **start but no defined end**; never terminate; must be **processed continuously** as events arrive; cannot wait for all data; often need events **in order of occurrence** to reason about completeness.
   - **Bounded streams:** have a **defined start and end**; can **ingest all data first**; **ordered ingestion not required** (can sort); processing bounded streams = **batch processing**.
2. **Deploy applications anywhere:** integrates with **Hadoop YARN, Kubernetes**, or runs **stand-alone**; automatically determines needed resources from the configured **parallelism** and requests them from the resource manager; on failure it **requests new resources to replace the failed container**; job submission/control via **REST calls**.
3. **Run applications at any scale:** parallelised into possibly **thousands of tasks**; maintains very large state; **asynchronous and incremental checkpointing** → minimal latency impact while guaranteeing **exactly-once state consistency**. Reported: **trillions of events/day, multiple TB of state, thousands of cores**.
4. **Leverage in-memory performance:** tasks access **local state, kept in memory** or in **access-efficient on-disk structures** if too large → very low latency; **exactly-once** consistency after failure by **periodic asynchronous checkpoints of local state to durable storage**.

### 4.4.5 Stateful vs stateless processing

- **Stateless:** each event processed independently (e.g. **filtering tweets**). **Stateful:** remembers past events (e.g. **tracking user sessions**, running counts) → 'smarter decisions by remembering previous data'. State must be **checkpointed** for fault tolerance.

### 4.4.6 Event time vs processing time (★ MCQ trap)

|  | **Event time** | **Processing time** |
| --- | --- | --- |
| Meaning | **When the event actually happened** (timestamp in the data) | **When Flink received/processed it** (system clock) |
| Example | Time a chat message was **sent** | Time it was **received** |
| Behaviour | Correct results even with **late / out-of-order** events | Fast, simple, but results depend on network delays |
| Use | Lets Flink work 'as if events happened in real time' | Low-latency monitoring |

- &#91;Supplement\] Flink tracks event-time progress using **watermarks** ('no events earlier than t will arrive'), which decide when a window can fire and how **late data** is handled.

### 4.4.7 Windowing in Flink

- **Windowing = grouping stream data by time** to compute aggregates over unbounded streams.

| Window | Description | Example |
| --- | --- | --- |
| **Tumbling** | **Fixed-size, non-overlapping**; each event in exactly one window | Every **5 minutes** |
| **Sliding** | Fixed size but **overlapping** (slide < size); an event can be in several windows | 5-minute windows computed **every 1 minute** |
| **Session** \[Supplement\] | Dynamic windows closed after a gap of inactivity | User browsing sessions |

```
time: |--0--5--10--15--20-->
Tumbling(5):  [0-5) [5-10) [10-15) [15-20)
Sliding(10,5):[0-10) [5-15) [10-20)   (overlap)
```

### 4.4.8 Word count in Flink (pseudo-code)

Goal: count words in a **continuous stream** – **1) read words from the stream, 2) group by word, 3) count occurrences**.

```java
DataStream<String> text = env.socketTextStream('localhost', 9999);
text.flatMap(line -> split into (word, 1))     // tokenise
    .keyBy(pair -> pair.word)                   // group by word
    .window(TumblingProcessingTimeWindows.of(Time.seconds(5)))
    .sum(1)                                     // count occurrences
    .print();
env.execute('Streaming WordCount');
```

*Why word count?* the classic example to demonstrate real-time processing.

### 4.4.9 Use cases, challenges, best practices, case study

- **Use cases:** **IoT** – real-time sensor monitoring; **Finance** – fraud detection; **E-commerce** – product recommendations. *Why real-time?* immediate reaction to data changes.
- **Challenges:** handling **high volumes**; ensuring **low latency**; managing **stateful operations**. **Best practices:** **start with small datasets**; **test for scalability**; **keep code modular**.
- **Case study – real-time sentiment analysis:** **stream tweets → analyse sentiment → display live sentiment trends**; impact: companies **gauge public reaction instantly**.

### 4.4.10 Flink vs Spark Streaming – when to choose which

| Choose **Flink** when | Choose **Spark Streaming** when |
| --- | --- |
| Millisecond latency, per-event processing, complex event-time logic, large state | Second-level latency is fine; you want **one stack for batch + ML + SQL + graph**; existing Spark skills |

---

## 4.5 Spark MLlib – Machine Learning on Spark (★★)

### 4.5.1 Motivating problem: text classification

**Goal:** given a text document, **predict its topic** (e.g. science vs non-science). **Logistic regression** finds the **best line (hyperplane) separating two sets of points** (each document = a point of features). \[Supplement\] model: P(y=1|x) = 1 / (1 + e^−(w·x + b)), trained by gradient descent (iterative → ideal for Spark's in-memory model).

### 4.5.2 ML workflow and pain points

```
TRAINING:   Raw data (label,text) -> Feature extraction -> Train model -> Evaluate (error)
TESTING /   New data (text)       -> Feature extraction -> Model.predict -> Predictions
PRODUCTION
```

**Pain points with raw RDDs:** (1) create/handle **many RDDs and data types** (labels, features, predictions); (2) the **whole pipeline written as one non-modular script**; (3) **parameter tuning** – training many models for different data splits and parameter sets.

**Solutions:** **DataFrames** solve the RDD-creation/handling pain; **ML Pipelines** (Transformers, Estimators, Evaluators) solve the scripting pain; **Parameters API + Tuning** solve the parameter-tuning pain.

### 4.5.3 DataFrame recap

**Distributed collection of data organised into named columns** (like a relational table; inspired by R and pandas); announced **Feb 2015**; **scales from KBs to PBs**; supports many formats (Hive, existing RDDs); **Catalyst optimisation + code generation**; APIs in Python, Java (Scala, R). **DataFrame = RDD + Schema + DSL.**

### 4.5.4 ML Pipelines (introduced in Spark 1.2/1.3) – key concepts (★)

Pipelines let developers **only (a) program the feature extraction and (b) specify the model**, and **automate** writing the script, training, evaluating by testing, or deploying to production.

| Concept | Role (slide wording) | Technical note |
| --- | --- | --- |
| **Transformer** | **Extracts features from a DataFrame and stores them in a new DataFrame** (e.g. Tokenizer, HashingTF) | Implements `transform(df)`; a trained **Model is also a Transformer** |
| **Estimator** | The **ML algorithm** (e.g. LogisticRegression); MLlib has standard algorithms; users can add their own | Implements `fit(df)` → returns a **Model** (e.g. LogisticRegressionModel) |
| **Evaluator** | **Computes predictions and estimates metrics such as error; helps tune parameters**; depends on the estimator (evaluator for LR ≠ for decision trees – slide) | In practice evaluators (e.g. `BinaryClassificationEvaluator`) compute a **metric** (AUC, accuracy, RMSE) from a predictions DataFrame |
| **Pipeline** | Chain of stages (Transformers + Estimator) | `Pipeline.fit()` → **PipelineModel** (Transformer) |
| **Parameters (ParamMap)** | Uniform API to set hyper-parameters | Grids used for tuning |
| **Tuning** | Given **Estimator + Parameter Grid + Evaluator → find best parameters** | `CrossValidator`, `TrainValidationSplit` |

**Flow in the slides:** TRAINING: `(label, text)` → **Transformer** → `(label, text, features)` → **Estimator** → **Model** → **Evaluator** (predictions) → pipeline trained. TESTING/PRODUCTION: `(text)` → same **Transformer** → `(text, features)` → **Model** → `(text, features, prediction)`. The **same transformer must be used in train and predict.**

### 4.5.5 Working example 1 – text classification pipeline with tuning

```python
from pyspark.ml import Pipeline
from pyspark.ml.feature import Tokenizer, HashingTF
from pyspark.ml.classification import LogisticRegression
from pyspark.ml.evaluation import BinaryClassificationEvaluator
from pyspark.ml.tuning import ParamGridBuilder, CrossValidator

train = spark.createDataFrame([(0, 'spark is fast', 1.0), (1, 'ipl match today', 0.0),
                               (2, 'big data with spark', 1.0), (3, 'football score', 0.0)],
                              ['id', 'text', 'label'])
tok  = Tokenizer(inputCol='text', outputCol='words')                     # Transformer 1
tf   = HashingTF(inputCol='words', outputCol='features', numFeatures=1000) # Transformer 2
lr   = LogisticRegression(maxIter=10)                                    # Estimator
pipe = Pipeline(stages=[tok, tf, lr])

grid = (ParamGridBuilder().addGrid(tf.numFeatures, [100, 1000])
                          .addGrid(lr.regParam, [0.1, 0.01]).build())    # parameter grid
cv   = CrossValidator(estimator=pipe, estimatorParamMaps=grid,
                      evaluator=BinaryClassificationEvaluator(), numFolds=2)
model = cv.fit(train)                                                    # trains many models, picks best
test  = spark.createDataFrame([(4, 'spark streaming')], ['id', 'text'])
model.transform(test).select('text', 'prediction').show()                # same transformers reused
```

### 4.5.6 Working example 2 – k-means clustering

```python
from pyspark.ml.feature import VectorAssembler
from pyspark.ml.clustering import KMeans
df = spark.createDataFrame([(1.0,1.0),(1.2,0.8),(8.0,8.2),(7.9,8.1)], ['x','y'])
data  = VectorAssembler(inputCols=['x','y'], outputCol='features').transform(df)
model = KMeans(k=2, seed=1).fit(data)
model.clusterCenters()            # two centres, near (1.1,0.9) and (7.95,8.15)
model.transform(data).select('x','y','prediction').show()
```

### 4.5.7 Exercise 4 (slides) – recognising sounds

Dataset lines `<bell.wav>, bell`; method: extract frequencies from the wav file, **Gaussian model**: average frequency per sound; for a new sound compute its average frequency and **output the closest match**.

- **Input DataFrame:** `(file, label)` e.g. `(bell.wav, bell)`; **Feature DataFrame:** `(file, label, frequencies)`; **Transformer** (same for train and predict): wav → average frequency; **Model:** `train(FeatureDataFrame)` learns the average frequency per label; `predict(PredictDataFrame)` returns the **closest-matching sound**.

### 4.5.8 MLlib algorithm families (slide list)

| Family | Algorithms |
| --- | --- |
| **Classification** | Logistic regression, Decision tree, Random forest, Gradient-boosted trees, Multilayer perceptron, SVM, Naïve Bayes |
| **Clustering** | **K-means, LDA, GMM** |
| **Collaborative filtering** | **ALS** (Alternating Least Squares) – recommendation |
| **Frequent pattern mining** | FP-growth \[Supplement\] |
| Also (earlier slide) | Basic statistics; all methods **scale out** across the cluster |

### 4.5.9 Deep learning with big data (slides)

- **Challenges:** deep learning (e.g. **TensorFlow**) is **iterative, heavy matrix–vector (linear algebra)**, evolved on a **single machine (scale-up)** then got its **own heterogeneous cluster (CPUs, GPUs, TPUs)**; but the **data sits on HDFS and big-data platforms use Spark** → two separate clusters; can we use the **same cluster**?
- **TensorFlowOnSpark (Yahoo):** supports **model parallelism and data parallelism**; **< 10 lines of code change**; the **algorithm and parameter server run on Spark executors**; **reads data directly from HDFS**; Spark RDD data is fed to the executor which passes it to TensorFlow; **RDMA** gives faster network transfers.
- **Other solutions:** **SystemML (IBM)** – **declarative ML language translated to MapReduce/Spark**; **Intel BigDL** – distributed deep learning on Spark, **modelled on Torch**.

### 4.5.10 spark.mllib vs spark.ml \[Supplement\]

`spark.mllib` = original **RDD-based** API (maintenance mode); `spark.ml` = **DataFrame-based Pipeline API** (the one described above, recommended).

---

## 4.6 Spark GraphX (★★)

### 4.6.1 Why graphs need special treatment

- Graphs are everywhere: **web graph** (vertices = pages, edges = hyperlinks → which pages matter most?), **social network** (users, follows → who is influential?), **payments** (accounts/merchants, transactions → suspicious cycles?), **road network** (junctions, roads with distances → shortest route?).
- Common thread: the answer for a vertex **depends on its neighbours (and their neighbours)** → **iterative, relationship-driven** problems that row-at-a-time processing handles poorly.
- **PageRank with MapReduce hurts:** each iteration is a **full job** (whole graph read from HDFS, shuffled, written back even though links never change); the **convergence loop (compare v(i) and v(i+1)) is hand-written**.

|  | **Data-parallel** | **Graph-parallel** |
| --- | --- | --- |
| Unit of work | A record / partition of records | A **vertex using its neighbours' values** |
| Systems | MapReduce, Spark RDDs/SQL | **Pregel, GraphLab, Spark GraphX** |
| Good at | ETL, joins, aggregation | **PageRank, connected components, shortest paths** |
| Iteration | Each pass = new job over all data | **Supersteps over a graph kept in memory** |

### 4.6.2 What is GraphX?

- **Spark's library for graphs and graph-parallel computation.** A graph is stored as **two RDDs: vertices and edges**; the same data can be viewed as **graph or collections without copying**; operations are built from normal Spark ops (join, map, group-by) → **inherits Spark fault tolerance**; API in **Scala and Java** (Python → **GraphFrames**).
- Sits on **Spark Core** alongside SQL, Streaming, MLlib. Paper: *GraphX: Graph Processing in a Distributed Dataflow Framework*, OSDI 2014.

### 4.6.3 Property graph model (★)

- A **directed multigraph with user-defined properties on every vertex and edge**; each vertex has a unique **VertexId (64-bit Long)**; **two vertices may have more than one edge** (e.g. friend and co-worker); **immutable and distributed** (changing a graph creates a new graph).
- Slide example: users Asha(21), Bharath(22), Chitra(20), Dev(23), Esha(21), Farhan(22) with `follows` edges: 1→2, 2→3, 3→1, 4→1, 5→6. Type: `Graph[(String, Int), String]`.
- **Three views:** `graph.vertices` (id, attr), `graph.edges` (srcId, dstId, attr), `graph.triplets` (**an edge joined with both end vertices**: srcAttr, attr, dstAttr). Triplet = SQL join: `SELECT src.id, dst.id, src.attr, e.attr, dst.attr FROM edges e JOIN vertices src, vertices dst ON e.srcId=src.id AND e.dstId=dst.id`.

**Demo 1 – build a graph:**

```scala
import org.apache.spark.graphx._
val users: RDD[(VertexId, (String, Int))] = sc.parallelize(Seq(
  (1L,('Asha',21)),(2L,('Bharath',22)),(3L,('Chitra',20)),(4L,('Dev',23)),(5L,('Esha',21)),(6L,('Farhan',22))))
val follows: RDD[Edge[String]] = sc.parallelize(Seq(
  Edge(1L,2L,'follows'),Edge(2L,3L,'follows'),Edge(3L,1L,'follows'),Edge(4L,1L,'follows'),Edge(5L,6L,'follows')))
val g = Graph(users, follows)
g.triplets.map(t => t.srcAttr._1 + ' ' + t.attr + ' ' + t.dstAttr._1).collect()
// Asha follows Bharath, Bharath follows Chitra, Chitra follows Asha, Dev follows Asha, Esha follows Farhan
```

Other builders: `Graph.fromEdges(edges, default)`, `GraphLoader.edgeListFile(sc, path)` (file of 'src dst' lines).

### 4.6.4 Graph operators

| Group | Operators | Purpose |
| --- | --- | --- |
| **Information** | `numVertices, numEdges, inDegrees, outDegrees` | Basic statistics |
| **Structural** | `subgraph, reverse` | Choose which vertices/edges to keep |
| **Neighbourhood** | `aggregateMessages` | Collect information from neighbours |
| **Iterative** | `pregel` | Repeat until nothing changes |
| **Algorithms** | `staticPageRank, connectedComponents, …` | Ready-made analytics |
| Property / join (optional) | `mapVertices, mapEdges, mapTriplets`; `joinVertices, outerJoinVertices` | Change attributes but keep structure |

**Demo 2 – degrees, subgraph:** `g.inDegrees.collect()` → `(1,2) (2,1) (3,1) (6,1)` (Asha has 2 followers; Dev and Esha have none so don't appear). `g.subgraph(vpred = (id, attr) => attr._2 >= 21)` keeps users aged ≥21 → **5 vertices, 3 edges**: Chitra (20) removed **and both edges touching her are removed** (an edge survives only if both end vertices satisfy the predicate).

**Demo 3 – `aggregateMessages` = map-reduce along edges:**

```scala
val followers = g.aggregateMessages[Int](ctx => ctx.sendToDst(1), (a, b) => a + b)
followers.collect()      // (1,2) (2,1) (3,1) (6,1)
```

`sendMsg` (like map): sees src/edge/dst attributes, **sends a message to src or dst**; `mergeMsg` (like reduce): **combines messages for the same vertex**; result = **one value per vertex that received a message**. \[Optional\] average age of followers: message = (count, age); merge adds pairs; divide at the end → Asha: (20+23)/2 = **21.5**. Keep messages **small, fixed-size**.

### 4.6.5 Pregel – 'think like a vertex' (★)

- **Google Pregel (SIGMOD 2010):** runs a graph algorithm as a series of **supersteps**; in each, every vertex that **received messages updates its value and sends messages to neighbours**; a **barrier** separates supersteps; **stops when no messages remain**.
- **GraphX signature:** `graph.pregel(initialMsg, maxIterations, activeDirection)(vprog, sendMsg, mergeMsg)`

| Function | Type | Role |
| --- | --- | --- |
| `vprog` | `(VertexId, VD, A) => VD` | **Update a vertex** from its current value and the incoming message |
| `sendMsg` | `EdgeTriplet => Iterator[(VertexId, A)]` | Look at **one edge** and decide which neighbour gets what message |
| `mergeMsg` | `(A, A) => A` | **Combine two messages** to the same vertex |

**Difference from Google's Pregel:** in GraphX messages are **computed per edge (both end vertices visible)** and can **only go to neighbours**. Loop ends when no messages remain or `maxIterations` reached.

**Demo 4 – single-source shortest path (source = vertex 1):** edges 1→2 (4), 1→3 (1), 3→2 (2), 2→4 (5), 3→4 (8).

```scala
val init = Graph.fromEdges(roads, 0.0).mapVertices((id, _) => if (id == 1L) 0.0 else Double.PositiveInfinity)
def vprog(id: VertexId, dist: Double, newDist: Double) = math.min(dist, newDist)
def sendMsg(t: EdgeTriplet[Double, Double]) =
  if (t.srcAttr + t.attr < t.dstAttr) Iterator((t.dstId, t.srcAttr + t.attr)) else Iterator.empty
def mergeMsg(a: Double, b: Double) = math.min(a, b)
val sssp = init.pregel(Double.PositiveInfinity)(vprog, sendMsg, mergeMsg)   // (1,0.0)(2,3.0)(3,1.0)(4,8.0)
```

| Superstep | Messages delivered | d(1) | d(2) | d(3) | d(4) |
| --- | --- | --- | --- | --- | --- |
| Start | none | 0 | ∞ | ∞ | ∞ |
| 1 | 2 gets 4, 3 gets 1 | 0 | 4 | 1 | ∞ |
| 2 | 2 gets 3 (via 3), 4 gets 9 | 0 | 3 | 1 | 9 |
| 3 | 4 gets 3+5 = 8 | 0 | 3 | 1 | **8** |
| 4 | none improves → **stop** | 0 | 3 | 1 | 8 |

Best route to 4 = 1→3→2→4 = 1+2+5 = **8** (not 1→3→4 = 9). Vertex 2 changes twice (first 4, then 3).

### 4.6.6 PageRank in GraphX vs MapReduce (★)

- **Random surfer model:** each page shares its importance equally among out-links. 4-page graph (1→2,1→3, 2→1,2→3,2→4, 3→1,3→4, 4→1,4→2). Transition matrix **M** (column = from, row = to): rows P1 = \[0, 1/3, 1/2, 1/2\]; P2 = \[1/2, 0, 0, 1/2\]; P3 = \[1/2, 1/3, 0, 0\]; P4 = \[0, 1/3, 1/2, 0\].
- Start **v₀ = \[1/4,1/4,1/4,1/4\]**; **v₁ = M·v₀ = \[1/3, 1/4, 5/24, 5/24\]**; repeat **v(i+1) = M·v(i)** until it stops changing (**v = M·v** = PageRank).

```scala
val web = GraphLoader.edgeListFile(sc, 'web_graph.txt').cache()
web.staticPageRank(2, 0.0).vertices.collect()    // runs n iterations of v(i+1) = M.v(i)
```

| Page | 1 iteration | 2 iterations | 50 iterations |
| --- | --- | --- | --- |
| P1 | 0.3333 (1/3) | 0.2917 (7/24) | 0.3051 |
| P2 | 0.2500 (1/4) | 0.2708 (13/48) | 0.2542 |
| P3 | 0.2083 (5/24) | 0.2500 (1/4) | 0.2373 |
| P4 | 0.2083 (5/24) | 0.1875 (3/16) | 0.2034 |
| (GraphX starts every page at 1 instead of 1/4, so its raw values are 4× larger.) |  |  |  |

|  | **Hadoop MapReduce** | **Spark GraphX** |
| --- | --- | --- |
| One iteration | A **full MapReduce job** | One **in-memory step** |
| Graph structure | **Re-read from HDFS every iteration** | **Loaded and cached once**; only values change |
| Stopping rule | **Hand-written** loop comparing v(i), v(i+1) | **Built-in** `staticPageRank(n, tol)` |
| Code size | Mapper + reducer + driver | A few lines |
| Fault tolerance | Files in HDFS between jobs | **RDD lineage** (+ optional checkpoints) |
| Maths identical; what changes is **where data lives between iterations and who writes the loop.** |  |  |

### 4.6.7 Built-in algorithms (`org.apache.spark.graphx.lib`)

**PageRank** (importance) • **Connected Components** (which vertices are linked at all) • **Strongly Connected Components** (reachable both ways) • **Triangle Count** (how tightly knit a neighbourhood) • **Label Propagation** (fast community detection) • **Shortest Paths** (hop distance to landmarks) • **SVD++** (recommendations). **Demo 6:** `g.connectedComponents().vertices.collect()` → `(1,1)(2,1)(3,1)(4,1)(5,5)(6,5)`: group 1 = Asha, Bharath, Chitra, Dev; group 5 = Esha, Farhan. Each vertex labelled with the **smallest id in its group**; **edge direction is ignored** (Dev follows Asha, so Dev joins her group).

### 4.6.8 GraphX vs GraphFrames

|  | **GraphX** | **GraphFrames** |
| --- | --- | --- |
| Built on | **RDDs** | **DataFrames** |
| Languages | Scala, Java | **Python**, Scala, Java |
| Vertex ids | **64-bit Long only** | **Any type** (an `id` column) |
| Extra queries | Operators, Pregel | **Motif (pattern) search, SQL** |
| Packaging | Ships with Spark | Separate package |
| Slides: in late 2024 Spark developers voted to **deprecate GraphX**; GraphFrames may replace it. Learn GraphX for the **concepts**; use GraphFrames in PySpark. Motif query: `g.find('(a)-[]->(b); (b)-[]->(c); (c)-[]->(a)')` finds follow cycles Asha→Bharath→Chitra→Asha (appears 3 times, once per starting point). |  |  |

### 4.6.9 Optional extras (slides 'Additional Information')

- **Vertex-cut partitioning:** real graphs have a few huge vertices (celebrities); **edge-cut** puts each vertex on one machine so a huge vertex's edges cross the network constantly; **vertex-cut** assigns **edges** to machines and **copies a vertex to several machines** (only its value must be synced); choose with `graph.partitionBy` (`RandomVertexCut, EdgePartition1D, EdgePartition2D`).
- **Storage:** **vertex table + routing table + edge table**; vertex values are shipped only to the edge partitions that need them, and **not re-sent if unchanged**.
- **Join operators:** `joinVertices` (only matching vertices; property type must stay same) vs `outerJoinVertices` (every vertex; function gets `Option`, type may change).
- **Tips:** `graph.cache()` before reuse; use `pregel` rather than hand-written loops; **checkpoint** long runs; **keep messages small**; vertex ids must be **Long**.

---

## 4.7 Practical Examples of Big Data Success Stories (★)

| Company | Big-data use | Key points from slides |
| --- | --- | --- |
| **Google** | Search, **PageRank**, **BigQuery**, self-driving car | Processes **3.5 billion requests/day** against **20 billion web pages**; **PageRank** used link information (importance of sites) beyond keyword matching; moving towards **semantic search** (connections between words); **BigQuery (2010)** – commercial cloud service to store and analyse big datasets (**pay for storage and compute time**); **MapReduce** influence; **self-driving car** using sensors, cameras, Maps, Streetview with real-time analysis |
| **Kaggle** | **Crowdsourcing, predictive modelling, gamification** | Platform organising the world's data scientists (\~**150,000**) for companies like Amazon, Facebook, Microsoft, Wikipedia; charges up to **$300/hour** for consultancy; runs **competitions** (gamification); Hal Varian: 'organise the brainpower of the world's most talented data scientists' |
| **Walmart** | **Data mining / market-basket** product recommendations | World's largest retailer (2M+ employees, \~20,000 stores in 28 countries); patterns from **products bought together**; increased **conversion rate**; goal: optimise in-store and e-commerce shopping experience, logistics efficiency; **Hadoop and NoSQL** give real-time access to centralised data from many sources |
| **Netflix** | **Recommendation engine**; data-driven content creation | Data points: titles watched, **how often playback stopped**, ratings; predicts what customers will enjoy; decisions on **new content** driven by data; stack: **Hadoop, Hive, Pig + traditional BI** |
| **Uber** | **Surge (dynamic) pricing**, usage analytics | Uses user data to see which features are used; **supply and demand** drive price; e.g. New Year's Eve fare for one mile can rise from 200 to 1000 (slide); **machine-learning** to find where demand is strong; short-term affects demand, long-term affects customer retention |
| **eBay** | **Streaming data analysis**, metadata/data governance | Needs systems that **analyse and act on data as it arrives**; tools **Apache Spark, Storm, Kafka**; analysts search **metadata tags** with proper **security and permissions (data governance)**; contributes back to **open source** |

**Common success pattern:** *collect large data → analyse with distributed tools (Hadoop/Spark/Kafka) → build recommendation/pricing/prediction models → improve customer experience and revenue.*

---

## UNIT 4 — EXAM KIT

### U4-A. Potential 4-mark questions with model answers

**Q1. Explain any four streaming-analytics use cases. (4)** *Model answer:* (1) **Log analysis** – real-time processing of network/application/router logs detects anomalies and attacks before users are affected (Splunk, Elasticsearch). (2) **Fraud detection** – streaming transactions are scored by ML (e.g. binary classification) so fraud is **blocked before completion** and alerts sent by email/SMS. (3) **Cyber security** – stream analysis of traffic spots a **DDoS** attack (abnormal traffic from one IP/profile) and isolates threats in real time. (4) **Sensor/IoT data** – aircraft fault detection, oil-and-gas pressure/temperature monitoring, supply-chain inventory. Others: **online advertising** (real-time click/interest targeting) and **database migration** with CDC to cloud lakes.

**Q2. Why is Kafka needed? Explain the publish–subscribe model and routing in Kafka. (4)** *Model answer:* Point-to-point links between N sources and M backends give **N×M connections** that are hard to maintain. **Kafka** is an intermediary that **decouples producers from consumers**. **Publisher** creates messages and publishes them to a topic without knowing the subscribers; **subscriber** registers interest in a category of messages and the **communication infrastructure (brokers)** delivers them. **Routing:** Kafka uses **topic-based** routing (subscribe to topics) plus a **pattern-based** variant (wildcard such as `ipl*`); **content-based routing is not supported**. Pros: no hard-wired connections, flexible; cons: topic design effort and infrastructure overhead.

**Q3. Explain Kafka topics, partitions, offsets and message structure. (4)** *Model answer:* A **topic** is a named, **append-only, ordered log on disk**. It is split into **partitions** spread across brokers to overcome single-server disk/I/O limits. Every message in a partition gets a unique increasing **offset**; consumers remember the offset read. **Partition choice:** with a key → `hash(key) % N` (same key → same partition); without key → **round-robin/sticky**. A **message** has **key, value (payload), offset, size, checksum, magic byte, compression codec (Snappy/GZip/LZ4) and timestamp** (producer or broker time).

**Q4. How does Kafka achieve fault tolerance and load balancing? (4)** *Model answer:* **Replication:** each partition is copied on several brokers (recommended **3 replicas, quorum 2**). The **leader** handles all reads/writes; **followers** replicate it; on failure a **new leader is elected** by the **controller**. Durability via **acks**: `acks=1` (leader only) or `acks=all/-1` (wait for **ISR** quorum). **Load balancing:** consumers form a **consumer group**; each partition is delivered to **exactly one** consumer in the group, so 4 partitions with 2 consumers → 2 partitions each; extra consumers beyond #partitions stay idle. Different groups each receive the full stream.

**Q5. Explain Spark Streaming architecture with DStreams. (4)** *Model answer:* Spark Streaming uses **discretised stream (micro-batch) processing**: the live stream is **chopped into batches of X seconds** (as low as ½ s, latency ≈ 1 s); **each batch is an RDD** and is processed by Spark's engine with normal RDD operations; results emerge in batches. A **DStream is a sequence of RDDs**; a **Receiver** (running as a long-lived task on an executor) reads from sources (**Kafka, Flume, sockets, HDFS, Twitter**) and stores data in memory; the driver launches tasks every interval. Output operations (`saveAsHadoopFiles`, `foreach`) push results to storage. Same code can run on batch data and streams.

**Q6. Compare stateless and stateful transformations in Spark Streaming with examples. (4)** *Model answer:* **Stateless** ops process each batch **independently** (`map, filter, reduceByKey`) – e.g. shares sold per company in each batch; recovery = recompute. **Stateful** ops **retain information across batches** and need **pair RDDs (key-state)**: **window operations** (`window(Minutes(10), Seconds(1)).countByValue()`; the incremental `countByValueAndWindow` **adds the new batch and subtracts the expired one**) and **session state** via `updateStateByKey` (e.g. per-user mood updated from each tweet). Stateful jobs need **checkpointing** (store RDD, truncate lineage) and replicated input for fault tolerance.

**Q7. Explain fault tolerance in Spark Streaming. (4)** *Model answer:* All intermediate data are **RDDs**, so lost partitions are recomputed through **lineage**. **Input batches are replicated in memory on multiple workers**, so a worker failure does not lose input. **Stateless** computations simply recompute; **stateful** computations need earlier state, so Spark **checkpoints** RDDs (e.g. hashTags/tagCounts at t+2) to reliable storage and **forgets older lineage**. Recovery from faults/stragglers takes about **1 second**; per-node throughput is **670k records/s**, versus Storm 115k and S4 7.5k.

**Q8. Describe Apache Flink's architecture and key features. (4)** *Model answer:* Flink is a **stream-first** framework for **real-time and batch** processing. **Job Manager** (planner) breaks a job into tasks and coordinates them; **Task Managers** (workers) execute the tasks. Key aspects: processes **bounded and unbounded** streams, **deploys anywhere** (YARN, Kubernetes, standalone; REST), **runs at any scale** (thousands of tasks, TB of state, **asynchronous incremental checkpointing → exactly-once**), and **in-memory local state** for low latency. APIs: **DataStream** (streaming) and **DataSet** (batch). Supports **event time vs processing time** and **tumbling/sliding windows**.

**Q9. Differentiate event time and processing time, and tumbling and sliding windows in Flink. (4)** *Model answer:* **Event time** = when the event **occurred** (timestamp in data); **processing time** = when Flink **receives/processes** it. Event time gives correct results despite delay/out-of-order arrival. **Tumbling window:** **fixed-size, non-overlapping** (every 5 min; each event in one window). **Sliding window:** **overlapping** windows (e.g. 10-min window every 5 min; an event may be in several windows). Example: an event at minute 7 lies in 1 tumbling-5 window (\[5,10)) but in 2 sliding(10,5) windows.

**Q10. Explain the Spark ML pipeline with Transformer, Estimator and Evaluator, using text classification. (4)** *Model answer:* Goal: predict a document's topic by **logistic regression**. **Pipeline:** `Tokenizer` and `HashingTF` are **Transformers** (DataFrame → new DataFrame with features); `LogisticRegression` is an **Estimator** whose `fit()` returns a **Model**; an **Evaluator** computes metrics (error/AUC) to compare models. `Pipeline.fit(training)` gives a **PipelineModel**; `transform(test)` adds predictions; the **same transformers** are used for training and prediction. **Tuning:** give Estimator + **ParamGrid** + Evaluator (CrossValidator) to find the best parameters. Pipelines remove the pain points of many RDDs, monolithic scripts and manual tuning.

**Q11. Explain the GraphX property graph and the Pregel operator with the shortest-path example. (4)** *Model answer:* GraphX stores a **directed multigraph** with properties on vertices and edges as **two RDDs** (vertices with 64-bit `VertexId`, edges); views: `vertices`, `edges`, **`triplets` (edge + both end vertices)**. **Pregel** runs in **supersteps**: each vertex that received messages runs `vprog`, `sendMsg` (per edge) emits messages to neighbours, `mergeMsg` combines them; a barrier separates supersteps; stops when **no messages remain**. SSSP: init source = 0, others ∞; vprog = min(old, new); sendMsg sends src+weight if smaller than dst; final distances for the slide graph: **(1:0, 2:3, 3:1, 4:8)**.

**Q12. Compare PageRank on MapReduce and on GraphX. (4)** *Model answer:* Same maths **v(i+1) = M·v(i)**. **MapReduce:** one **full job per iteration**, graph **re-read from HDFS and shuffled every iteration**, **hand-written convergence check**, more code, fault tolerance via files in HDFS. **GraphX:** graph **loaded and cached once in memory**, only rank values move, an **in-memory step per iteration**, built-in `staticPageRank(n, tol)` in a few lines, fault tolerance through **RDD lineage** with optional checkpoints. Hence far less disk I/O and simpler code.

**Q13. Write short notes on any three big-data success stories. (4)** *Model answer:* **Google** – PageRank, MapReduce, BigQuery, self-driving car; 3.5 B requests/day over 20 B pages. **Netflix** – recommendation engine using viewing data (titles watched, playback stops, ratings) on Hadoop/Hive/Pig; data-driven content creation. **Uber** – surge pricing from real-time supply/demand with ML. Also **Walmart** (market-basket recommendations, Hadoop/NoSQL), **Kaggle** (crowdsourced predictive modelling), **eBay** (streaming analytics with Spark, Storm, Kafka).

### U4-B. Important definitions, facts, keywords, formulae

| Term | Definition / fact |
| --- | --- |
| **Streaming analytics** | Real-time insights from continuous data streams |
| **Standing query / ad hoc query** | Continuously running query / arbitrary query needing stored window |
| **Kafka** | Distributed publish–subscribe log-based messaging platform; decouples producers and consumers |
| **Topic / Partition / Offset** | Named log / physical ordered append-only slice on a broker / position of a message in a partition |
| **Broker / Controller** | Stores partitions and serves clients / elects partition leaders (ZooKeeper older, **KRaft** newer) |
| **Leader / Follower / ISR** | Serves reads+writes / replicates leader / in-sync replica set |
| **acks** | `1` leader only; `-1/all` ISR quorum; recommended replicas 3, quorum 2 |
| **Consumer group** | Instances of one app; each partition → one member |
| **DStream** | Sequence of RDDs, one per batch interval |
| **Receiver** | Reads source data and stores into Spark memory (long-running task) |
| **window(length, slide)** | Sliding-window state over last `length` recomputed every `slide` |
| **updateStateByKey** | Per-key arbitrary state across batches |
| **Checkpointing** | Persist RDD to reliable storage and drop lineage |
| **Flink JobManager / TaskManager** | Planner/scheduler / worker executing tasks |
| **Bounded / Unbounded stream** | Finite (batch) / infinite (streaming) |
| **Event time / Processing time** | Time event occurred / time system processes it |
| **Tumbling / Sliding window** | Fixed, non-overlapping / overlapping |
| **Transformer / Estimator / Evaluator / Pipeline** | `transform()` DataFrame→DataFrame / `fit()` → Model / metric calculator / chain of stages |
| **Property graph / Triplet** | Directed multigraph with attributes / edge + src + dst vertex attributes |
| **Pregel / Superstep** | Vertex-centric BSP model / one round of message-compute |
| **aggregateMessages** | map-reduce along edges (`sendMsg`, `mergeMsg`) |

**Formulae / numbers to memorise:** partition = `hash(key) % N`; **replicas 3 / quorum 2**; Spark Streaming **latency \~1 s, batch ≥ ½ s**; **670k vs 115k vs 7.5k** records/s/node; **6 GB/s on 100 nodes**; PageRank `v(i+1) = M·v(i)`, v₁ = \[1/3, 1/4, 5/24, 5/24\]; SSSP result (0, 3, 1, 8); connected components labels (1,1,1,1,5,5); Google **3.5 B requests/day, 20 B pages**.

### U4-C. Frequently confused concepts – comparison tables

| Confusion | Distinguish |
| --- | --- |
| **Kafka vs Spark Streaming vs Flink** | Messaging/pipeline broker (stores & transports events) vs micro-batch processing engine vs true streaming engine. Kafka is typically the **source/sink** for the other two |
| **Topic vs partition** | Logical named category vs physical log unit; ordering guaranteed only within a partition |
| **Consumer vs consumer group** | One reader vs cooperating readers that split partitions; multiple groups each see all data |
| **Leader vs follower vs controller** | Partition-level read/write replica, passive replica, cluster-level elector |
| **ZooKeeper vs KRaft** | External coordinator (old) vs built-in Raft consensus (new) |
| **Topic-based vs content-based vs pattern-based** | Kafka: yes / **no** / yes (wildcard on topic name) |
| **acks=1 vs acks=all** | Leader only (fast, some loss risk) vs ISR quorum (durable) |
| **Stateless vs stateful** | No memory vs state across batches (window / updateStateByKey) |
| **window() vs countByValueAndWindow** | Recompute on full window vs incremental add-new/subtract-old |
| **Micro-batch vs record-at-a-time** | Spark Streaming (≈ s) vs Storm/Flink (ms) |
| **At-least-once vs exactly-once** | Storm may duplicate state updates; Trident / Flink / Spark (state) exactly-once |
| **Event time vs processing time** | Occurrence vs arrival/processing |
| **Tumbling vs sliding** | Non-overlap vs overlap |
| **DataStream vs DataSet** | Flink streaming API vs batch API |
| **Transformer vs Estimator vs Model** | `transform()` / `fit()` / result of `fit()` (itself a Transformer) |
| **spark.mllib vs spark.ml** | RDD API vs DataFrame pipeline API |
| **GraphX vs GraphFrames** | RDD-based, Long ids, Scala/Java vs DataFrame-based, any ids, Python + motif search |
| **aggregateMessages vs Pregel** | One round of neighbour aggregation vs repeated rounds until no messages |
| **subgraph edges** | Removed if **either** end vertex fails the predicate |
| **inDegrees** | Vertices with 0 in-edges are **absent** from the result |

### U4-D. Practice MCQs – 2 marks each (11)

**1.** A topic has 4 partitions and a consumer group has 5 consumers. How many consumers are idle? (a) 0 (b) 1 (c) 2 (d) 4 **Ans: (b)** one partition per consumer at most → 4 busy, 1 idle.

**2.** A producer sends a keyed message; hash(key) = 17 and the topic has 3 partitions. The partition is: (a) 0 (b) 1 (c) 2 (d) 3 **Ans: (c)** 17 mod 3 = 2 (partitions are numbered 0–2; there is no partition 3).

**3.** A topic has 3 partitions, replication factor 3, on a 3-broker cluster. Total partition replicas stored? (a) 3 (b) 6 (c) 9 (d) 27 **Ans: (c)** 3 partitions × 3 replicas = 9 (each broker holds one replica of every partition).

**4.** `hashTags.window(Minutes(10), Seconds(1)).countByValue()` – what does it compute? (a) counts of the last 1 s every 10 min (b) counts over the last 10 min, recomputed every 1 s (c) counts per batch only (d) cumulative counts forever **Ans: (b)** window length 10 min; sliding interval 1 s.

**5.** With `countByValueAndWindow`, tag 'cat' has window count 50; the new batch has 8 occurrences and the batch leaving the window had 5. The new count is: (a) 45 (b) 53 (c) 58 (d) 63 **Ans: (b)** 50 + 8 − 5 = 53 (add new, subtract old).

**6.** `updateStateByKey` with an update function that sums values. The state for key 'A' is 7 and the next batch contains values \[2, 3\] for 'A'. The new state is: (a) 5 (b) 7 (c) 10 (d) 12 **Ans: (d)** 7 + 2 + 3 = 12.

**7.** In Flink, an event with timestamp at minute 7 falls into how many windows of a **sliding window of size 10 min, slide 5 min** (windows start at 0)? (a) 1 (b) 2 (c) 3 (d) 4 **Ans: (b)** windows \[0,10) and \[5,15). A tumbling 5-min window would give 1.

**8.** On the slide graph (Asha 21, Bharath 22, Chitra 20, Dev 23, Esha 21, Farhan 22; edges Asha→Bharath, Bharath→Chitra, Chitra→Asha, Dev→Asha, Esha→Farhan) `g.subgraph(vpred = (id, attr) => attr._2 >= 22)` leaves how many edges? (a) 0 (b) 1 (c) 2 (d) 3 **Ans: (a)** vertices kept: Bharath, Dev, Farhan; every edge has at least one endpoint aged < 22, so all are dropped. (With `>= 21` it keeps 5 vertices and 3 edges.)

**9.** For the 4-page PageRank example, v₁ = \[1/3, 1/4, 5/24, 5/24\]. The value of P3 in v₂ = M·v₁ (row P3 = \[1/2, 1/3, 0, 0\]) is: (a) 5/24 (b) 1/4 (c) 3/16 (d) 7/24 **Ans: (b)** 1/2·(1/3) + 1/3·(1/4) = 1/6 + 1/12 = 1/4.

**10.** In the shortest-path graph (1→2: 4, 1→3: 1, 2→4: 5, 3→4: 8) the weight of edge 3→2 is changed from 2 to 4. The final distance to vertex 4 is: (a) 8 (b) 9 (c) 10 (d) 13 **Ans: (b)** d(2) = min(4, 1+4) = 4 → via 2: 4+5 = 9; via 3: 1+8 = 9 → 9.

**11.** `Pipeline(stages=[Tokenizer, HashingTF, LogisticRegression])` – before `fit()` the stages are: (a) 3 Transformers (b) 2 Transformers + 1 Estimator (c) 1 Transformer + 2 Estimators (d) 3 Estimators **Ans: (b)** after `fit()` the resulting `PipelineModel` holds 3 Transformers (LogisticRegression becomes a LogisticRegressionModel).

### U4-E. Practice MCQs – 1 mark each (30)

1. Detecting a DDoS attack by analysing traffic from one IP is a use case of: (a) log analysis (b) cyber security (c) advertising (d) migration — **(b)**.
2. A **standing query** is: (a) arbitrary one-off (b) continuously running (c) needs full archive (d) SQL only — **(b)**; e.g. alert when temperature > threshold.
3. 'Average of last n readings (n arbitrary)' is a(n): (a) standing query (b) ad hoc query (c) window join (d) none — **(b)**.
4. Kafka primarily **decouples**: (a) disk and RAM (b) producers and consumers (c) HDFS and YARN (d) map and reduce — **(b)**.
5. Kafka does **not** support: (a) topic-based routing (b) pattern-based subscription (c) content-based routing (d) consumer groups — **(c)**.
6. A Kafka topic is stored as: (a) random-access table (b) ordered append-only log (c) B-tree (d) in-memory cache only — **(b)**.
7. A keyed message goes to partition: (a) random (b) hash(key) % N (c) always 0 (d) round-robin — **(b)**; no key → round-robin/sticky.
8. The **offset** is: (a) the broker id (b) a message's position within a partition (c) a topic's size (d) a timestamp — **(b)**.
9. In Kafka the partition **leader** handles: (a) only writes (b) only reads (c) all reads and writes (d) only metadata — **(c)**.
10. Recommended replicas / quorum: (a) 1/1 (b) 2/1 (c) 3/2 (d) 5/5 — **(c)**.
11. `acks=all (-1)` means the leader waits for: (a) nobody (b) ISR quorum acknowledgement (c) the consumer (d) the controller — **(b)**.
12. In a consumer group each partition is read by: (a) all members (b) exactly one member (c) two members (d) the producer — **(b)**.
13. Newer Kafka elects its controller through: (a) ZooKeeper (b) KRaft (c) YARN (d) HDFS NameNode — **(b)**.
14. A DStream is: (a) a single RDD (b) a sequence of RDDs (c) a DataFrame (d) a graph — **(b)**.
15. Spark Streaming processing model is: (a) record-at-a-time (b) micro-batch (c) offline only (d) MapReduce — **(b)**.
16. Which is a **stateful** operation? (a) map (b) filter (c) updateStateByKey (d) flatMap — **(c)**.
17. Checkpointing in Spark Streaming: (a) stores an RDD and forgets earlier lineage (b) deletes input (c) replicates the driver (d) compresses logs — **(a)**.
18. Storm's default processing guarantee: (a) exactly-once (b) at-least-once (c) at-most-once (d) none — **(b)**; Trident adds exactly-once.
19. Spark Streaming throughput per node (slides): (a) 7.5k (b) 115k (c) 670k (d) 6M records/s — **(c)**.
20. Flink's components: (a) Driver & Executor (b) JobManager & TaskManager (c) NameNode & DataNode (d) Master & Slave only — **(b)**.
21. An **unbounded** stream: (a) has an end (b) has a start but no defined end (c) is always sorted (d) is batch — **(b)**.
22. **Event time** is: (a) time Flink processes the event (b) time the event occurred (c) job start time (d) checkpoint time — **(b)**.
23. A **tumbling** window is: (a) overlapping (b) fixed-size, non-overlapping (c) dynamic by inactivity (d) global — **(b)**.
24. Flink's batch API (older) is: (a) DataStream (b) DataSet (c) DStream (d) RDD — **(b)**.
25. `Estimator.fit()` returns: (a) a DataFrame (b) a Model (c) an RDD (d) a Pipeline stage list — **(b)**.
26. Which is a clustering algorithm in MLlib? (a) ALS (b) K-means (c) Naive Bayes (d) SVM — **(b)**.
27. ALS in MLlib is used for: (a) clustering (b) collaborative filtering/recommendation (c) text tokenising (d) graphs — **(b)**.
28. TensorFlowOnSpark was developed by: (a) Google (b) Yahoo (c) IBM (d) Intel — **(b)**; IBM = SystemML, Intel = BigDL.
29. A GraphX **triplet** is: (a) three vertices (b) an edge joined with both end vertices (c) a triangle count (d) a 3-hop path — **(b)**.
30. Pregel computation stops when: (a) no messages remain (b) one superstep finishes (c) memory is full (d) driver restarts — **(a)** (or `maxIterations` reached).
31. GraphX vertex ids are: (a) Strings (b) 64-bit Long (c) UUIDs (d) any type — **(b)**; GraphFrames allow any type.
32. For graph processing in **PySpark** you use: (a) GraphX (b) GraphFrames (c) MLlib (d) Flink — **(b)**.
33. Google launched **BigQuery** in: (a) 2004 (b) 2006 (c) 2010 (d) 2015 — **(c)**.
34. **Surge pricing** is associated with: (a) Netflix (b) Uber (c) Walmart (d) Kaggle — **(b)**.
35. **Kaggle**'s business model: (a) crowdsourced data-science competitions (b) cloud storage (c) retail (d) video streaming — **(a)**.

### U4-F. UNIT 4 REVISION SHEET (read this last)

- **Streaming use cases:** log analysis, fraud detection, cyber security (DDoS), sensors/IoT (aircraft, oil & gas), online advertising, DB migration (CDC), predictive maintenance. Standing vs ad hoc queries.
- **Kafka:** broker between sources and backends (N×M → N+M). Pub-sub: producer, consumer, brokers. **Topic-based + pattern-based; no content-based.** Topic = append-only log; **partition** = scale unit; **offset** per partition; key → `hash(key) % N` else round-robin/sticky. **Replication** (leader reads/writes, followers, ISR, acks 1 / all, replicas 3, quorum 2). **Consumer group:** partition → one consumer. **Controller** via ZooKeeper (old) / KRaft (new). Users: LinkedIn, Twitter (Storm), Square, Netflix, Uber…
- **Spark Streaming:** micro-batch (½–1 s), DStream = RDDs, receivers (Kafka, Flume, socket), stateless (map, reduceByKey) vs stateful (window, countByValueAndWindow incremental, updateStateByKey), checkpointing + replicated input + lineage; 670k vs 115k vs 7.5k rec/s/node; limitation: near-real-time only.
- **Flink:** true streaming; JobManager + TaskManagers; bounded/unbounded; YARN/K8s/standalone; exactly-once with async incremental checkpoints; in-memory state; **event vs processing time**; **tumbling vs sliding** windows; DataStream/DataSet; use cases IoT, fraud, recommendations.
- **MLlib:** DataFrame-based **Pipelines**: Transformer (`transform`), Estimator (`fit` → Model), Evaluator (metric), Params + CrossValidator tuning. Algorithms: LR, DT, RF, GBT, MLP, SVM, NB; K-means, LDA, GMM; ALS; FP mining. Deep learning: TensorFlowOnSpark (Yahoo), SystemML (IBM), BigDL (Intel).
- **GraphX:** property graph = vertex RDD + edge RDD, directed multigraph, Long ids; triplets; operators (inDegrees, subgraph, aggregateMessages, pregel); Pregel supersteps; PageRank v(i+1)=M·v(i) in-memory vs MapReduce HDFS each iteration; ConnectedComponents; GraphFrames for Python.
- **Success stories:** Google (PageRank, BigQuery 2010, 3.5 B/day), Kaggle (crowdsourcing), Walmart (Hadoop/NoSQL, recommendations), Netflix (recommendations), Uber (surge pricing), eBay (Spark/Storm/Kafka streaming).

---

# SYLLABUS COVERAGE CROSS-CHECK

| Syllabus item | Covered in | Status |
| --- | --- | --- |
| **U3** Spark and Scala / PySpark programming model | 3.1, 3.2 (Scala), 3.3 (PySpark, Py4J), 3.4 (lifecycle, SparkContext, creating RDDs/DataFrames) | Covered |
| **U3** Transformations and Actions | 3.6 (tables with outputs, reduceByKey vs groupByKey, pair RDDs, persistence, shared variables) | Covered |
| **U3** Spark SQL | 3.7 (motivation, DataFrame API, data model, UDFs, Catalyst 4 phases, extensions, schema inference) | Covered |
| **U3** Relational operators with Spark | 3.8 (relational algebra ↔ SQL ↔ DataFrame ↔ RDD, worked example) + 3.7.4 | Covered (slides thin → supplemented) |
| **U3** Spark architecture – RDD, DataFrames, wide & narrow dependencies | 3.5 (RDD), 3.7.6 (DataFrame), 3.9 (architecture, lazy evaluation, DAG, dependencies, scheduling) | Covered |
| **U3** Streaming algorithms – sampling | 3.10.2 | Covered |
| **U3** Set membership – Bloom filters | 3.10.3 | Covered |
| **U3** Counting unique elements – Flajolet–Martin | 3.10.4 | Covered |
| **U4** Streaming analytics use cases | 4.1 | Covered |
| **U4** Kafka – use cases, architecture | 4.2 | Covered |
| **U4** Streaming Spark | 4.3 | Covered |
| **U4** Apache Flink | 4.4 | Covered |
| **U4** Spark MLlib – working examples | 4.5 (pipeline, two code examples, sound-recognition exercise, deep learning) | Covered |
| **U4** Spark GraphX | 4.6 (property graph, operators, Pregel, PageRank, algorithms, GraphFrames) | Covered |
| **U4** Practical examples of Big Data success stories | 4.7 (Google, Kaggle, Walmart, Netflix, Uber, eBay) | Covered |

**Per-unit deliverables checklist:** detailed notes ✔ • 4-mark model answers (13 per unit) ✔ • definitions/facts/formulae ✔ • confusion tables ✔ • 2-mark MCQs (10 in Unit 3, 11 in Unit 4) ✔ • 1-mark MCQs (30 in Unit 3, 35 in Unit 4) ✔ • revision sheets ✔ • mock exam ✔.

**Slide-vs-reality notes to remember:** (a) slide lists `coalesce` as wide, real Spark treats it as narrow unless shuffle is forced; (b) slides describe an Evaluator loosely – in Spark code it computes a metric, while `Model.transform` produces predictions; (c) Spark Streaming DStream API is the legacy API (Structured Streaming is newer); (d) the DataSet API of Flink is being retired in newer releases; (e) the slides state GraphX was voted for deprecation in late 2024. Answer in the exam according to the slides' wording, and mention the nuance only if the question asks for it.

---

# 40-MARK MOCK EXAMINATION (Units 3 & 4)

**Time: 90 minutes • Maximum marks: 40.** Attempt it first without looking at the answer key at the end of this document.

## Section A – Descriptive (4 × 4 = 16 marks)

**Q1 (Unit 3).** Explain how Spark executes the following word-count program. Your answer must mention lazy evaluation, the DAG, narrow/wide dependencies, stages and the role of the driver and executors. **(4)**

```python
lines  = sc.textFile('hdfs://.../input.txt')
words  = lines.flatMap(lambda l: l.split())
pairs  = words.map(lambda w: (w, 1))
counts = pairs.reduceByKey(lambda a, b: a + b)
counts.collect()
```

**Q2 (Unit 3).** (a) State the Flajolet–Martin algorithm and why 2^R estimates the number of distinct elements. **(2)** (b) For the stream 3, 1, 4, 1, 5, 9, 2, 6 with hash h(x) = (3x + 1) mod 32 (5-bit binary), find the tail length of each element, R, and the estimate of distinct elements; compare with the true count. **(2)**

**Q3 (Unit 4).** A Kafka topic `orders` has 3 partitions, replication factor 2, on 3 brokers, and is read by a consumer group of 2 consumers. Explain (i) how keyed messages are assigned to partitions, (ii) how the consumers share the partitions, and (iii) what happens when the leader of partition 1 fails. **(4)**

**Q4 (Unit 4).** Describe the Pregel operator of GraphX and trace single-source shortest path from vertex A on the graph A→B (2), A→C (5), B→C (1), C→D (3), showing the distances after every superstep. **(4)**

## Section B – MCQs, 2 marks each (4 × 2 = 8 marks)

**Q5 (Unit 3).** A Bloom filter has n = 1000 bits, stores m = 250 keys and uses k = 1 hash function. The false-positive probability is about: (a) 25% (b) 22% (c) 7.8% (d) 2.5%

**Q6 (Unit 3).** `rdd = sc.parallelize([('a',1),('b',2),('a',3),('b',4),('a',5)])` `rdd.map(lambda kv: (kv[0], kv[1]*2)).reduceByKey(lambda x,y: x+y).filter(lambda kv: kv[1] > 12).collect()` returns: (a) `[('a',18)]` (b) `[('a',18),('b',12)]` (c) `[('b',12)]` (d) `[]`

**Q7 (Unit 4).** A topic has 4 partitions. Keyed messages arrive whose key hashes are 6, 9, 10, 14 and 4 (partition = hash % 4). How many of these five messages go to partition 2? (a) 1 (b) 2 (c) 3 (d) 4

**Q8 (Unit 4).** Flink uses **tumbling windows of 10 minutes** (windows \[0,10), \[10,20), \[20,30)). Events arrive with event times at minutes 3, 9, 10, 17 and 21. How many windows contain exactly two events? (a) 0 (b) 1 (c) 2 (d) 3

## Section C – MCQs, 1 mark each (16 × 1 = 16 marks)

**Unit 3**

**Q9.** Which is NOT a transformation? (a) sample (b) union (c) aggregate (d) distinct

**Q10.** Predicate pushdown is performed in which Catalyst phase? (a) Analysis (b) Logical optimisation (c) Physical planning (d) Code generation

**Q11.** In Spark, resources are allocated to applications by the: (a) executor (b) cluster manager (c) RDD (d) accumulator

**Q12.** RDDs are immutable, which means: (a) they cannot be cached (b) transformations create new RDDs (c) they are stored only on disk (d) they cannot be partitioned

**Q13.** Which dependency requires a shuffle? (a) narrow (b) wide (c) one-to-one (d) pipelined

**Q14.** A Bloom filter reports 'definitely not in the set' when: (a) all k bits are 1 (b) at least one of the k bits is 0 (c) the array is full (d) k = 1

**Q15.** The tail length of the binary hash 110 is: (a) 0 (b) 1 (c) 2 (d) 3

**Q16.** A broadcast variable is best suited to: (a) counting errors across tasks (b) sharing a large read-only lookup table (c) storing results on the driver (d) writing logs

**Unit 4**

**Q17.** Messages within a Kafka partition are ordered by: (a) timestamp only (b) offset (c) key (d) consumer id

**Q18.** A consumer group has 3 consumers reading a topic with 6 partitions. Each consumer is assigned: (a) 1 partition (b) 2 partitions (c) 3 partitions (d) all 6

**Q19.** In `window(Minutes(10), Seconds(1))` the 'Seconds(1)' is the: (a) window length (b) batch size of Kafka (c) sliding interval (d) checkpoint interval

**Q20.** Spark Streaming recovers lost data using: (a) lineage + replicated input + checkpoints (b) only disk logs (c) ZooKeeper (d) NameNode

**Q21.** In Flink the component that breaks a job into tasks and coordinates them is the: (a) TaskManager (b) JobManager (c) DataNode (d) Receiver

**Q22.** In Spark ML, calling `fit()` on an Estimator returns a: (a) DataFrame (b) Model (c) RDD (d) Graph

**Q23.** In GraphX, the operator that keeps only the vertices/edges satisfying predicates is: (a) aggregateMessages (b) subgraph (c) pregel (d) triplets

**Q24.** Which company is associated with a data-driven recommendation engine built on Hadoop, Hive and Pig? (a) Uber (b) Kaggle (c) Netflix (d) eBay

*(End of paper. Scroll down only after attempting.)*

---

# MOCK EXAM – ANSWER KEY AND MODEL ANSWERS

## Answer key – Sections B and C

| Q | Ans | Reason |
| --- | --- | --- |
| 5 | **(b)** | 1 − e^(−250/1000) = 1 − e^(−0.25) ≈ 0.2212 ≈ 22% |
| 6 | **(a)** | doubled values: a = 2+6+10 = 18, b = 4+8 = 12; filter > 12 keeps only ('a',18) |
| 7 | **(c)** | 6%4 = 2, 9%4 = 1, 10%4 = 2, 14%4 = 2, 4%4 = 0 → three messages in partition 2 |
| 8 | **(c)** | \[0,10): minutes 3, 9 → 2; \[10,20): 10, 17 → 2; \[20,30): 21 → 1; two windows have exactly 2 |
| 9 | **(c)** | `aggregate` is an action |
| 10 | **(b)** | rule-based logical optimisation (predicate pushdown, constant folding, column pruning) |
| 11 | **(b)** | cluster manager allocates resources (Standalone/YARN/Mesos/K8s) |
| 12 | **(b)** | immutable → new RDD per transformation; lineage enables recomputation |
| 13 | **(b)** | wide dependency = shuffle = stage boundary |
| 14 | **(b)** | any 0 bit proves the key was never inserted |
| 15 | **(b)** | 110 has one trailing zero |
| 16 | **(b)** | broadcast = read-only value cached per machine; accumulator is for counters |
| 17 | **(b)** | each message has a unique, increasing offset within its partition |
| 18 | **(b)** | 6 partitions / 3 consumers = 2 each |
| 19 | **(c)** | slide interval = how often the window is evaluated; window length is 10 min |
| 20 | **(a)** | RDD lineage, replicated input batches in memory, checkpointing for state |
| 21 | **(b)** | JobManager = planner; TaskManagers execute |
| 22 | **(b)** | `fit()` → Model (itself a Transformer) |
| 23 | **(b)** | `subgraph` (structural operator) |
| 24 | **(c)** | Netflix – Hadoop, Hive, Pig + BI for recommendations |

**Scoring guide:** Section B: 4 × 2 = 8 (Q5–Q8); Section C: 16 × 1 = 16 (Q9–Q24).

## Model answers – Section A (with marking scheme)

**Q1 (4 marks)**

1. **Lazy evaluation (1):** `textFile`, `flatMap`, `map`, `reduceByKey` are **transformations**; each only adds an RDD (node) to the **lineage DAG**; nothing is read or computed. `collect()` is an **action** and triggers execution of the whole DAG as a job.
2. **DAG and dependencies (1):** DAG = lines → words → pairs → counts. `flatMap` and `map` have **narrow dependencies** (each parent partition feeds one child partition) and are **pipelined** together, record by record, with no intermediate storage. `reduceByKey` has a **wide dependency**: all pairs with the same word must meet, so a **shuffle** occurs (with map-side combine to reduce the data moved).
3. **Stages (1):** the DAG is cut at the shuffle → **Stage 1** = textFile + flatMap + map (+ local combine); **Stage 2** = reduce side of `reduceByKey`. One task per partition per stage.
4. **Driver/executors (1):** the **driver** (SparkContext) builds the DAG, creates stages/tasks and **schedules tasks using data locality** (HDFS block locations) via the **cluster manager**; **executors** run the tasks, write/read shuffle data and report status; `collect()` returns the final (word, count) list to the driver. Lost partitions would be recomputed through lineage.

**Q2 (4 marks)** (a) **(2)** Choose a hash function with a range larger than the number of distinct elements. For each element compute h(a) and its **tail length** (trailing zeros); maintain **R = maximum tail length**; output **2^R**. Reason: P(h(a) ends in at least r zeros) = 2⁻ʳ, so with m distinct elements P(some element has tail ≥ r) = 1 − (1 − 2⁻ʳ)^m ≈ 1 − e^(−m/2ʳ): near 1 when m ≫ 2ʳ and near 0 when m ≪ 2ʳ, so the largest r seen satisfies **m ≈ 2^R**. Duplicates hash identically so only distinct elements count; only R is stored. (Improve by averaging within groups of hashes and taking the median.) (b) **(2)**

| x | 3x+1 | mod 32 | binary (5 bits) | tail length |
| --- | --- | --- | --- | --- |
| 3 | 10 | 10 | 01010 | 1 |
| 1 | 4 | 4 | 00100 | 2 |
| 4 | 13 | 13 | 01101 | 0 |
| 1 (dup) | 4 | 4 | 00100 | 2 (no change) |
| 5 | 16 | 16 | 10000 | **4** |
| 9 | 28 | 28 | 11100 | 2 |
| 2 | 7 | 7 | 00111 | 0 |
| 6 | 19 | 19 | 10011 | 0 |

**R = 4 → estimate = 2⁴ = 16**; true distinct elements = {3,1,4,5,9,2,6} = **7**. The estimate is a power of two and over-shoots; averaging several hash functions (median of group averages) would improve it. *(1 mark for the table/tail lengths, 1 mark for R, estimate and comparison.)*

**Q3 (4 marks)** (i) **(1)** A keyed message goes to partition **`hash(key) % 3`**, so all messages with the same key land in the same partition (order preserved per key); messages without key are spread round-robin/sticky. (ii) **(1.5)** The two consumers form one **consumer group**; each partition is delivered to **only one** consumer of the group: e.g. C1 reads P0 and P2, C2 reads P1 (rebalanced if a consumer joins/leaves). Consumers track their **offset** in each partition. Another group would receive all messages independently. (iii) **(1.5)** With replication factor 2, each partition has a **leader** (all reads/writes) and one **follower** on a different broker. If P1's leader broker fails, the **controller** (KRaft quorum, or ZooKeeper in older Kafka) **elects the in-sync follower as the new leader**; producers and consumers switch to it. With `acks=all`, committed messages were already on the ISR, so no acknowledged data is lost (with `acks=1` the latest unreplicated messages might be lost).

**Q4 (4 marks)** **Pregel (2):** A vertex-centric model: computation proceeds in **supersteps** separated by barriers. In each superstep every vertex that received messages runs **`vprog`** to update its value; **`sendMsg`** is evaluated per edge triplet to send messages to neighbours; **`mergeMsg`** combines multiple messages to a vertex. It stops when **no messages remain** (or maxIterations). In GraphX messages are computed per edge (both end vertices visible) and go only to neighbours: `graph.pregel(initialMsg, maxIter, activeDir)(vprog, sendMsg, mergeMsg)`. For SSSP: initial distance 0 for A and ∞ elsewhere; `vprog = min(old, new)`; `sendMsg` sends src+weight if smaller than dst; `mergeMsg = min`. **Trace (2):**

| Superstep | Messages | d(A) | d(B) | d(C) | d(D) |
| --- | --- | --- | --- | --- | --- |
| Start | none | 0 | ∞ | ∞ | ∞ |
| 1 | B gets 2, C gets 5 | 0 | 2 | 5 | ∞ |
| 2 | C gets 2+1 = 3 (via B); D gets 5+3 = 8 (via old C) | 0 | 2 | 3 | 8 |
| 3 | D gets 3+3 = 6 | 0 | 2 | 3 | **6** |
| 4 | no improvement → stop | 0 | 2 | 3 | 6 |

Shortest paths from A: B = 2, C = 3 (A→B→C), D = 6 (A→B→C→D).

---

**Good luck! Final 10-minute checklist before the exam:** (1) RDD 5 properties and lineage; (2) transformations vs actions and narrow vs wide lists; (3) Catalyst 4 phases; (4) Bloom FPR formula and no-false-negative property; (5) FM = 2^R with tail length = trailing zeros; (6) Kafka topic/partition/offset/leader/ISR/consumer group/KRaft; (7) DStream = RDD sequence, window vs updateStateByKey, checkpoint; (8) Flink JobManager/TaskManager, event vs processing time, tumbling vs sliding; (9) MLlib Transformer/Estimator/Evaluator; (10) GraphX triplet, aggregateMessages, Pregel, PageRank v = M·v.
