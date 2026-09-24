# Big Data — ISA 1 Complete Notes (Unit 1 & Unit 2)

**Legend:** 🔥 = High-yield / strong 4-mark candidate | ⭐ = Commonly asked as 1/2-marker

---

# UNIT 1: INTRODUCTION

## 1. What is Big Data?

- No single standard definition. Big Data = data whose **scale, diversity, and complexity** require new forms of processing (new architectures, techniques, algorithms, analytics) to extract value/insights and enhance decision-making.
- Example: Healthcare — Big Data predicts disease outbreaks, enables personalized medicine.

### Big Data & Analytics Pipeline
```
Input Data → Model → Analysis
```
- **Model**: human construct to understand real-world systems; represents inherent properties of data (schema/groupings); independent of the software using it.
- **Database**: grouping of tables; repository for querying for analytics/intelligence; can be distributed.
- **Table**: rows & columns; values can be number/date/hyperlink/image/text.
- **Flat file**: read start-to-end, no random middle access.
- **Name-value pair** vs **Hash key-value pair** (hash function computes key for indexing/searching, distributes across buckets).

### Processing & Analytics Types ⭐
| Type | Description |
|---|---|
| Batch processing | Series of transactions, no user interaction |
| Interactive transaction processing | Continuous exchange, e.g., e-banking UI; needs low latency (MPP, in-memory DBs, columnar DBs) |
| Real-time processing | Live/streaming data, low latency, decisions as data generated |
| Stream analytics | Continuous computing as events occur (log streams, Twitter) |
| Stream transaction processing | Specialized frameworks: Storm, S4, Spark Streaming, Flume |
| In-memory | Operations on RAM/cache — much faster than disk |

### 🔥 ETL and Analytics Maturity Levels (common 2-marker)
- **ETL** (Extract, Transform, Load): retrieve, integrate, transform, store data.
- **Descriptive analytics**: value from visualizations/reports (what happened).
- **Predictive analytics**: forecasts future outcomes.
- **Prescriptive analytics**: recommends decisions/options (e.g., maximize profit).
- **Cognitive analytics**: analyzes sentiment/emotion via learning & representation.
- **Service**: mechanism providing access to capabilities (via interface); **SOA** = architectural style of services, messages, operations, processes.

## 2. Classification of Data ⭐

| Type | Description | Examples |
|---|---|---|
| **Structured** | Predefined schema, tabular, rows/columns | RDBMS, spreadsheets |
| **Semi-structured** | Flexible schema, tags/markers create hierarchy | JSON, XML, CSV |
| **Unstructured** | No predefined format/schema; majority of all data | Text, email, social media, images, audio, video |

**Sources of data**: Individual footprint growing, but machine-generated data (IoT) growing faster — machine logs, RFID, sensor networks, vehicle GPS, retail transactions.

### Big Data Themes
- **Data Management**: infrastructure/processes to store, process, govern data.
- **Data Analytics**: extract value/knowledge once managed.
- **Google Search System**: web crawlers index billions of pages; ranks by relevance.
- **Amazon Recommendation System**: ML + collaborative filtering on purchase history, browsing, ratings.

## 3. 🔥 Traditional vs Big Data Approach (Machine Translation example — good 4-marker)

| Traditional (Rule-based) | Big Data (Statistical) |
|---|---|
| Linguistic expert builds model | No attempt to understand language |
| Needs grammar, morphology, word meaning | Uses parallel corpus (millions of sentence pairs) |
| Model = human-generated | Model = machine-generated (statistical) |
| Explicit word mapping rules | Infers mapping from correlation (e.g., "cat"→"billi") |

**Input Data → Model → Analysis**: Traditional = model is human-generated; Big Data = model is machine-generated.

### Domain Knowledge Debate
- Peter Norvig (Google): "The Unreasonable Effectiveness of Data" — correlation can be enough; J. Craig Venter's gene sequencing found 1000s of new species from data alone, no prior domain model.
- But: **"let the cat out of the bag"** → naive translation fails without domain/linguistic knowledge — shows pure data isn't always sufficient.

## 4. 🔥 Pitfalls in Big Data Analysis (frequently tested)

1. **Spurious correlation**: C→A and C→B does NOT imply A→B.
   - Example: Storks & human births positively correlated across Europe — hidden variable = nesting area, not causation.
2. **Selection bias / Gaps in data**: sample not representative of population.
   - Example: Hurricane Sandy tweets mostly from Manhattan → biased conclusion that Manhattan was worst-hit.
   - Convenience sampling contributes to this bias.
3. **Missing/negative data bias**: In medicine, negative results more likely to go unpublished → biased sample overestimates treatment benefit.

### Addressing the Issues
- Use **domain knowledge** to validate models.
- **Estimate errors** empirically.
- Nate Silver (*The Signal and the Noise*) correctly predicted US 2008/2012 elections using probabilistic thinking + human adjustment.
- **Weather forecasting** success: chaotic systems (Lorenz butterfly effect), human adjustment, probabilistic thinking, more sensitive to false negatives (predicting no rain when it rains) than false positives.
- **2008 Housing market collapse**: flawed independence assumption in risk models; "never predict out-of-sample situations based on in-sample data."

### 🔥 Big Data Error Estimation (Empirical Method)
- Purely empirical (observation/data-based, not theoretical).
- Data split into **training set** and **testing set**.
- Algorithm built on training set; error measured on unseen testing set.
- Standard way to compare analytics algorithms.
- Examples: Nate Silver's forecasting, Amazon recommendations (model from historical data, evaluated via interaction/purchase stats).

### Big Data Pipeline (Full)
```
Identifying data sources → Data Ingestion/Acquisition → Data Storage → Data Processing → Data Consumption
```
(Managed throughout by **Pipeline Management**)

## 5. History & Characteristics of Big Data

### History — 3 Phases ⭐
| Phase | Era | Driver |
|---|---|---|
| Phase 1 | 1970s | Structured Content — DB management, SQL |
| Phase 2 | Early 2000s | Web-based Unstructured Content — internet, search queries |
| Phase 3 | Current | Mobile & Sensor-based Content — IoT, mobile devices |

- Data grew from **4.4 zettabytes (2013)** → projected **44 zettabytes (2020)**.

## 6. 🔥🔥 The 5 V's of Big Data (VERY high-yield — likely 4-marker)

### Volume
- 44x increase 2009→2020 (0.8 ZB → 35 ZB).
- Old style: enterprises only generate data (manual entry). Big Data: everybody generates data (automated) — financial transactions, sensors/smart grid, comms & media.

### Variety
- Old style: fixed format/schema. Big Data: integrates Twitter, Maps, Facebook, text, numeric, images, audio, video, time series — static vs streaming.
- Need to link all types together to extract knowledge.

### Velocity
- Old style: input at night, output daily report. Big Data: real-time input, real-time response.

### Veracity
- Messiness/trustworthiness of data; affects quality/reliability.
- Old style: clean data (manual entry, fixed fields). Big Data: inconsistent data (hashtags, typos, abbreviations).
- Example: Aadhar/PAN/Voter ID mismatches in names/addresses — yet still usable ("postman still delivers").

### (Sometimes cited 5th V: Value — extracting worth from data)

## 7. Data Architecture Design

- **Logical layers in data processing architecture** (T1 Fig 1.2) — layered pipeline from source to consumption.
- **Big Data Storage plan**: RDBMS and NoSQL work together (T1 Fig 1.7).
- **Big Data Platforms**: Hadoop, Spark, NoSQL DBs, cloud platforms etc.

## 8. Case Study: Google Search (PageRank teaser)
- Initial implementation: 24M web pages, 322M links, **5 days** to compute PageRank.
- PageRank ∝ popularity of page — treats web as a graph; random user starts at random page, follows random links.
- Assumption: more popular a page (more incoming links) → better quality.

---

## 9. 🔥🔥 File Systems & HDFS (Very High Yield)

### Why HDFS?
- Example: 1.3 billion credit card transactions → 13 TB data. Needs efficient distributed storage/processing.

### File System Fundamentals
- Disks are **block-oriented** (e.g., 4KB blocks).
- File system = structured way to store/retrieve data; separates **metadata** (name, size, permissions, location) from **data** (actual content).
- Desirable file properties: long-term existence, sharable, structured (hierarchical).
- Operations: Create, Delete, Open, Close, Read, Write.

### Local vs Distributed File Systems
- **Local FS**: intermediate layer between apps and block-oriented disks; generic, not app-tuned.
- **Distributed FS (DFS)**: manages files/folders across multiple computers; appears as single filesystem; needed when data too large for one disk.

### 🔥 Why NOT existing file systems (NFS)?
- NFS: small block sizes (8–16KB) → large metadata overhead; designed for many small files.
- NFS scales **up** (grow volume) not **out** (add machines) — scaling out was retrofitted later (e.g., Clustered Shared Volumes in NTFS).
- HDFS is based on the **open-source version of Google File System (GFS)**.

### 🔥🔥 HDFS Design Principles ("HDFS is a filesystem designed for storing very large files with streaming data access patterns, running on clusters of commodity hardware.")

| Property | Description |
|---|---|
| Very Large | Files MB/GB/TB; PB-scale clusters operational |
| Read-mostly ("write-once, read-many") | Most efficient pattern; whole-dataset read time matters more than first-record latency |
| Commodity hardware | No need for expensive reliable hardware; runs on clusters of cheap machines |

### 🔥🔥 HDFS Architecture — NameNode, DataNode, Secondary NameNode (VERY LIKELY 4-marker)

**NameNode** (Master):
- Maintains/manages DataNodes.
- Stores block locations, permissions on local disk in two files:
  - **Fsimage**: complete namespace snapshot since NameNode creation.
  - **Edit log**: recent changes since last Fsimage.
- Receives **heartbeat** and **block reports** from DataNodes.

**DataNodes** (Slaves):
- Workhorses; commodity hardware storing file blocks.
- Serve client read/write requests.
- Perform block creation, replication, deletion (as instructed by NameNode).
- Send heartbeat (health) + block reports (list of blocks) to NameNode.

**Secondary NameNode**:
- Helper node — NOT a hot standby, just housekeeping backup.
- Downloads & merges Fsimage + edit logs periodically, refreshes edit log, sends updated image back to NameNode (and standby NameNode for faster failover).

### 🔥 HDFS Blocks
- Disk blocks: ~512 bytes (minimum read/write unit).
- **HDFS blocks: 128MB** (default, Hadoop v2+; v3 also 128MB) — much larger than disk blocks.
- Files broken into block-sized chunks stored as independent units.
- A file smaller than one block does NOT occupy full block's disk space.
- **Why large blocks?**
  - A file can be larger than any single disk.
  - Files distributed across disks; simplifies storage subsystem.
  - Blocks fit well with replication (fault tolerance/availability).
  - Minimizes seek-time overhead relative to transfer time.

### Why separate Metadata from Data? (Motivation for NameNode/DataNode split)
- Metadata: small, accessed less frequently, but needs fast/frequent lookup → kept on NameNode (in-memory).
- Data: much larger, needs parallel access → distributed across DataNodes.

### HDFS Basic Commands (conceptual understanding only — no exact syntax needed per exam note)
- `ls`, `mkdir`, `put` (local→HDFS), `get` (HDFS→local), `rm`, `dfsadmin -report` (cluster status)
- `fsck -files -blocks`: lists blocks per file, checks health (missing/under/over-replicated/corrupted blocks).

### 🔥🔥 HDFS Fault Tolerance (High-yield)

**1. Replication** (default factor, historically 3):
- Replica of data on different machines; if one machine fails, data available from replicas.

**2. Erasure Coding** (Hadoop 3+):
- Works like RAID — stripes file into small units across disks, computes **parity cells** via encoding algorithm.
- If a machine fails, block recoverable from parity cells (**decoding**).
- Reduces storage overhead to ~50% (vs 200% for 3x replication).

### 🔥 HDFS High Availability (HA) — post 2.x
- Objective: make NameNode resilient to failure, recover quickly.
- **Active–Standby NameNode configuration**.
- Edit logs stored on **shared storage**; block mappings stored in memory (rebuilt from DataNode block reports to both NameNodes).
- On failure: Standby takes over using shared edit logs + in-memory block mappings.
- **Failover Controller**: handles active↔standby transition; pluggable; uses **ZooKeeper** to ensure only one active NameNode.
  - ZooKeeper: NameNode maintains persistent session; if it crashes, session expires, other NameNode notified, takes exclusive lock (becomes leader).
  - Handles "ungraceful failure" (e.g., slow network mistaken for failure).

### Where HDFS Doesn't Score (⭐ common MCQ)
| Limitation | Why |
|---|---|
| Low-latency data access | HDFS optimized for high throughput, not tens-of-ms latency |
| Lots of small files | NameNode holds metadata in memory; each file/dir/block ≈150 bytes; millions of files = 100s of MB just metadata |
| Multiple writers / arbitrary modification | Single writer only; writes only appended at end of file |
| Not POSIX compliant | Can't mount as standard FS; needs separate client |

### HDFS on SSDs (conceptual, may appear as 1-marker)
- Bottleneck shifts from disk to network/CPU.
- Can use smaller blocks (no seek overhead).
- Favor Erasure Coding over 3x replication (cost efficiency).

---

## 10. Google File System (GFS)

- Developed by Ghemawat, Gobioff, Leung (2003 paper).
- Used internally for Google Search, Earth, Gmail.

### 🔥 GFS Architecture
- **Master node**: stores metadata (namespace, ACLs, file→chunk mapping).
- **Chunk servers**: store actual data chunks on local disk.
- **Chunk size: 64MB** (fixed, large) → reduces metadata, minimizes client-master interaction.
- **Replication**: default of 3 replicas per chunk.

### GFS Operations
- Read/Write: client queries master for chunk locations, then talks directly to chunkservers.
- **Atomic record appends**: concurrent writers can append to same file simultaneously.
- **Snapshots**: efficient copy for backup/replicas.

### GFS Fault Tolerance
- **Heartbeat messages**: master ↔ chunkserver liveness check.
- **Chunk replication**: ensures availability on failure.
- **Data integrity**: checksums per chunkserver; verified before returning data; on mismatch, master clones new valid replica.

### GFS vs HDFS Quick Comparison ⭐
| | GFS | HDFS |
|---|---|---|
| Chunk/Block size | 64MB | 128MB |
| Master | Single Master | NameNode (+Secondary+Standby) |
| Origin | Google (2003) | Inspired by GFS, Apache open-source |

### GFS Successor: Colossus (2-marker candidate)
- Google replaced GFS internally with **Colossus**.
- Eliminated single-master bottleneck (metadata distributed across **Bigtable** clusters).
- Reduced chunk size 64MB → **1MB**.
- Replaced replication with **Reed-Solomon erasure coding**.
- Works alongside **Borg** (resource orchestrator, ancestor of Kubernetes) and **Spanner** (globally consistent DB).

---

## 11. 🔥🔥 MapReduce Programming Model (Core topic — expect multiple questions across all mark levels)

### Motivation
- Need: scale out (cluster of cheap commodity machines), not scale up (expensive SMP machines) — 4x more cost-effective.
- Simple programming abstraction: **Map + Reduce**, separates "what" (application logic) from "how" (system details: locking, threading, fault tolerance).
- Origin: Google [OSDI'04]; Hadoop = open-source adoption (Yahoo, now Apache).

### Key Principles
- **Parallel processing**: across distributed servers.
- **Data locality**: move computation to data (not data to compute) — reduces network traffic.
- Framework handles: fault tolerance, I/O scheduling, parallelization — hidden from programmer.

### 🔥 MapReduce Programming Model — Formal Definition
```
Map function:     (Kin, Vin) → list(Kinter, Vinter)
Reduce function:  (Kinter, list(Vinter)) → list(Kout, Vout)
```
- **Map**: processes a key/value pair, generates intermediate key/value pairs; output sorted before going to reducer.
- **Reduce**: merges all intermediate values sharing the same key; runs only after ALL map tasks finish.
- **Partition function**: default = `hash(key) mod R` (R = #reducers); ensures same keys → same reducer.

### 🔥🔥 Word Count Example (classic, know cold)
- Mapper: for each line, split into words, emit `(word, 1)` for each.
- Reducer: sum all 1's for each key → `(word, total_count)`.

### 🔥🔥 Relational Algebra Operations via MapReduce (VERY high-yield for 4-markers — practice writing pseudocode)

**Selection** σ_C(R): 
- Map: read row t; if satisfies condition C, output (t, t). 
- Reduce: do nothing (identity).

**Projection** π_S(R):
- Map: read row t; compute subset of attributes t'; output (t', t').
- Reduce: eliminate duplicates — (t', [t',t',...]) → (t', t').

**Union** (R ∪ S):
- Mapper1: read row of R, output (t,t). Mapper2: read row of S, output (t,t).
- Reduce: eliminate duplicates — (t,[t,t]) → (t,t).
- Uses `MultipleInputs` (two different mappers reading two different files).

**Intersection** (R ∩ S):
- Same mappers as union.
- Reduce: output ONLY duplicates (i.e., key appearing from both R and S).

**Difference** (R − S):
- Mapper1 (R): output (t, "R"). Mapper2 (S): output (t, "S").
- Reduce: output t only if its value list contains ONLY "R" (not "S").

**Natural Join** (R ⋈ S on attribute B, R has A,B; S has B,C):
- Mapper1: read (a,b) of R → output (b, (R,a)).
- Mapper2: read (b,c) of S → output (b, (S,c)).
- Reduce: for each key b, combine (R,a) and (S,c) pairs → output (a,b,c).

**Grouping & Aggregation** (Group R(A,B,C) by A, aggregate f(B)):
- Map: for each row (a,b,c), output (a,b).
- Reduce: aggregate (a,[b1,b2,...]) → (a, f(b1,b2,...)).

### Sample Problems Format (practice style)
- Given: Employee(Name,Age), Dept(Name,Dept) tables → Natural Join walkthrough (Mapper1 out, Mapper2 out, Reducer in/out) — **expect similar pseudocode question**.
- Given: Dept table → Group by Dept, count employees — mapper emits (dept,1), reducer sums.

### 🔥 Job Submission / Execution Flow
- User submits: input data + MapReduce program + config.
- Input divided into **input splits**; Hadoop divides job into **map tasks** and **reduce tasks**.
- **One map task per split**; tasks run in parallel.
- Multiple reducers → multiple output files (merged view available via HDFS).

### 🔥 Split Size Considerations
- Small split size: more splits → more parallelism/load balancing, BUT overhead of managing splits/task creation dominates.
- **Optimal split size = HDFS block size (128MB)** — avoids inter-node data transfer (data locality).
- If split = block size: all data for a map task on same node (no network transfer). If split ≠ block: cross-node data transfer needed (perf hit).

### Map & Reduce Output Storage ⭐
- **Map output**: written to **local disk** (NOT HDFS) — temporary, discarded after reduce consumes it. If node fails before reducer consumes it, map task automatically rerun elsewhere.
- **Reduce output**: stored in **HDFS** for reliability — one replica local to reduce node, others off-rack (uses network bandwidth).
- Reduce has no data locality advantage (input comes from ALL mappers, sorted outputs transferred over network, merged, then passed to reduce function).

### 🔥 Combiners
- Mini-reducer that runs **on the mapper** (before shuffle) to reduce data transferred to reducer.
- Combines multiple map outputs locally before sending to reduce.
- Example: word count combiner pre-sums counts per mapper before shuffle.

### 🔥 Shuffle Process (know the flow)
```
Input Splitting → Mapping → Shuffling (sort+group by key across nodes) → Reducing → Output
```

### Distributed Execution Overview
```
User Program → forks → Master, Workers (Map workers, Reduce workers)
Master assigns map tasks and reduce tasks to workers
Map worker: reads split, writes intermediate to local disk
Reduce worker: reads remote intermediate data, sorts, applies reduce, writes output
```

### 🔥 Solved Review Problems (know the calculation method)
- 2GB file, 128MB split size, 4 disks (1/node):
  - #splits = 2048MB/128MB = **16**
  - splits/disk = 16/4 = **4**
  - #map tasks = 16 (1 per split)
  - map tasks/node = 16/4 = **4**
  - #reduce tasks = **user-specified**
- 230MB file, Hadoop v3 (128MB blocks): 2 blocks → **2 mappers**; reducers = configurable.
- Combiner runs **on the mapper** node.
- Record spanning block boundary: mapper reads till EOR (default `\n`) separator, handles split records across block boundary transparently.

---

## 12. 🔥🔥 YARN (Yet Another Resource Negotiator)

### Why YARN? (Hadoop 1.0 problems)
- **JobTracker** (Hadoop 1.0) did BOTH resource management AND job scheduling/monitoring — single point of failure, scalability limit (~4000 nodes).
- **TaskTracker**: one per task, managed by JobTracker.

### 🔥 Hadoop 1.0 Issues (good 2/4-marker)
| Issue | Description |
|---|---|
| Limits scalability | JobTracker on single machine does everything |
| Availability | JobTracker = single point of failure |
| Resource utilization | Fixed map/reduce slots — imbalance (map slots full, reduce slots idle) |
| Limited to MR only | Tightly coupled with MapReduce; can't run other app types |

### 🔥🔥 YARN Architecture — splits JobTracker responsibilities
| Component | Role |
|---|---|
| **Resource Manager** | Cluster-wide resource arbitration across ALL applications; launches application containers; monitors resource usage |
| **Node Manager** | Per-machine slave; monitors container resource usage on that node |
| **Application Master** | Per-application; negotiates resource containers from scheduler; tracks/monitors container progress |
| **Container** | Unit of resource allocation (memory, CPU, disk) |

### Data Locality in MapReduce (via YARN)
- Prefer running map task on node where input data resides.
- If unavailable: free slot on same rack as a data replica.
- Last resort: off-rack node (inter-rack network transfer, most expensive).

### 🔥🔥 YARN Schedulers (High-yield comparison)

**FIFO Scheduler** (early Hadoop): jobs run in submission order, one job can use whole cluster → others wait.

**Fair Scheduler**:
- Every user/job gets fair share of cluster capacity over time.
- Jobs placed in **pools** (default: one pool per user).
- Single job running → gets whole cluster. More jobs submitted → slots redistributed fairly.
- Supports **preemption**: kills tasks in over-capacity pools if another pool hasn't gotten fair share in time.
- Custom pools can have guaranteed minimum capacity.

**Capacity Scheduler**:
- Multiple **queues** (like Fair's pools), each with allocated capacity; can be hierarchical.
- Within a queue: **FIFO** (with priorities).
- Cannot use free/spare capacity from other queues even if idle — "like breaking cluster into smaller clusters."

### 🔥 Failure Handling in YARN (good 4-marker table)
| Failure Type | Cause | Detection | Recovery |
|---|---|---|---|
| **Task failure** | Runtime exception / hanging (no progress 10 min, timeout configurable) / killed (speculative duplicate) | JVM reports to AM / timeout | AM restarts task on different node |
| **Application Master failure** | HW/network failure | Periodic heartbeat to RM | RM restarts AM (default max-attempts = 2) |
| **Node Manager failure** | HW/crash/slow network | No heartbeat to RM for 10 min | Tasks of incomplete jobs rerun, possibly different node |
| **Resource Manager failure** | — | — | Active-Standby config; failover controller handles it; most serious (all tasks fail) |

### YARN Benefits — Case Study (Yahoo) ⭐
- Scales to 40,000+ servers, 100,000+ CPUs, 455 PB data, 850,000+ jobs/day.
- **Flexibility**: same cluster runs Hadoop, Storm, Spark simultaneously (resource sharing via YARN).

### Solved Exercises (know these types)
- 1000-node cluster, 2 pools max 50% each, no jobs running, new 600-node job submitted uses all 600 → scheduler could be **FIFO or Fair** (both allow full cluster use when no competing job).
- Task failure ≠ job failure (task just restarted).
- **Speculative duplicates**: tasks started because AM detects a slow-running task (backup task race).

---

## 13. 🔥🔥 Big Data Algorithms: Matrix-Vector Multiplication & PageRank

### Matrices & Vectors Basics
- Vector: ordered list of numbers; operations: addition (component-wise), scalar multiplication.
- Matrix: m×n array (m rows, n columns); square matrix = n×n; vector = 1×n (row) or n×1 (column) matrix.
- **Traditional storage**: n² space for n×n matrix (dense array) — infeasible for web-scale (n ~10¹⁰).

### 🔥 Representing WWW as Adjacency Matrix
- Directed graph: node = page, edge = link.
- Adjacency Matrix: rows=source, columns=destination (or vice versa per convention); 1 if link exists, else 0.
- Web graph is **sparse** (most entries 0) → dense storage wasteful.

### 🔥 Sparse Matrix Representation for HDFS
- Store only non-zero elements as `<row_number, column_number, value>` triples in CSV.
- Techniques: Dictionary of Keys (DoK), Coordinate List (COO).

### 🔥🔥 Matrix-Vector Multiplication via MapReduce (Very likely numeric problem in exam)
- Multiply m×n matrix M by n-vector v → m-element result x, where `x_i = Σ_j M_ij * v_j`.
- **Assumption**: vector v fits in memory (shared/broadcast to all mappers); M stored as CSV on HDFS, distributed.
- **Map**: for each M_ij, compute partial product M_ij * v_j; output key = **i** (row index, = index into result vector); value = M_ij*v_j.
- **Reduce**: sum all partial products for each key i → final x_i.
- If v does NOT fit in memory: partition M and v into **stripes**; same algorithm applied per stripe.

### 🔥🔥 PageRank (Extremely high-yield — expect a 4-marker)

**Motivation — Problems with early search/naive approaches:**
- **Term spam**: repeating keywords artificially inflates relevance ranking.
- Naive fix (count inbound links) also gameable via spam pages linking in.

**Two real solutions:**
1. **PageRank**: random surfer model — what fraction of random surfers end up at my page = importance.
2. Consider terms near links in pages pointing to mine (anchor text relevance).

**Transition Matrix M**:
- For n-node graph, n×n matrix.
- Entry M[dest][source] = 1/(out-degree of source) if link exists, else 0 — i.e., probability of moving from source to destination assuming equal probability among all out-links.
- Column for a source node sums to 1 (if it has out-links).

**Random Surfer Iteration**:
```
v0 = initial importance vector (uniform, e.g., 1/n each)
v(i+1) = M × v(i)
```
- Repeated iteration until convergence (v stops changing significantly).

**🔥 PageRank as Eigenvector Problem**:
- At convergence: `v(i+1) ≈ v(i)` → `M·v = v` → `M·v = 1·v`.
- This is the **eigenvector equation** `A·v = λ·v` with **eigenvalue λ = 1**.
- v = PageRank vector = the values ARE the page importance scores.

**Fixed-Point Iteration** (general concept):
- Solve `x = f(x)` by: initialize x0, compute `x(i+1) = f(x(i))`, repeat until `|x(i+1) - x(i)|` is small or max iterations reached.
- PageRank: `f(v) = M·v` (matrix multiplication as the fixed-point function).

**MapReduce Implementation**:
- Needs: multiple input files (adjacency list + current PageRank vector), iteration over multiple matrix-vector multiplication rounds.
- Each MR round produces a new estimate of v stored in HDFS; compared with previous iteration; used as input to next round.

---

## 14. 🔥🔥 Complexity of Big Data Algorithms — Communication Cost Model (High-yield, numeric problems likely)

### Why Study Complexity?
- Multiple algorithms can solve same problem — need to choose the best one.
- Study complexity of **workflow systems** (generalization of MapReduce).

### 🔥 Communication Cost Complexity Model
- **Communication cost = size of input** (not output).
  - Why? Algorithms tend to be linear in data size; network/disk speed << CPU speed → communication dominates total time.
  - Why only input, not output? Output feeds into next task (becomes input elsewhere); final output is generally small (aggregated) or must be human-readable.
- **Natural Join (R,S)**: Mapper input = r+s (read from disk); Reducer input = r+s (network reads). **Total complexity: O(r+s)**.

### 🔥🔥 3-Way Join Complexity (R, S, T) — classic exam numeric problem
**Case 1: Join R,S first, then join with T**
- Mapper1 input: r, Mapper2 input: s → Reducer1 input: r+s
- Let p = probability of match between r,s → intermediate result size = **prs**
- Mapper3 input: prs, Mapper4 input: t → Reducer2 input: t + prs
- **Total complexity: O(r + s + t + prs)**

**Case 2: Join S,T first, then join with R**
- Let q = probability of match between s,t → intermediate = **qst**
- **Total complexity: O(r + s + t + qst)**

- 🔥 **Key insight**: complexity depends on **join order**; if p≈q, choose join order producing smallest intermediate (rs, st, or rt — whichever pair is smallest).

### 🔥 Key Performance Parameters

**Wall Clock Time** vs **Communication Cost** — fundamental trade-off:
- Running everything on single CPU → low communication cost but high (slow) wall clock time.
- Dividing into more tasks → more communication, but lower wall clock time (more parallelism).

**Reducer size (q)**: max # of values that can share the same key (NOT number of reducers).
- Smaller q → more reducers possible → reduces wall clock time BUT increases communication cost.
- Max #reducers = T/q (T = total map outputs).

**Replication rate (r)**: 
```
r = (# key-value pairs output by Mapper) / (# input records to Mapper)
```
- = average communication cost from Map to Reduce tasks.

### 🔥🔥 Similarity Join Example (Image Comparison — classic worked numeric problem)

**Setup**: 1 million images, 1MB each = 1TB total DB. Need all pairs (x,y) where similarity s(x,y) > threshold t.

**Naive Algorithm**:
- Mapper: reads (i, Pi), generates ALL pairs ({i,j}, {Pi,Pj}).
- Reducer: reads pair, computes s(Pi,Pj).

**Naive Algorithm Analysis** (🔥 memorize this reasoning):
- Communication cost ≈ n² (n = #images) — EXTREMELY high (~10¹⁸ bytes = 1 exabyte; ~300 years over gigabit Ethernet!).
- **Replication rate is very high**: ~999,999 (each image copied once per every other image).
- **Reducer size is very LOW** (each pair has only 2 values, 2MB max input per reducer).
- **Parallelism is very high** (10¹⁸ independent reducer computations possible).
- ⇒ Algorithm is theoretically parallel but practically infeasible due to communication cost.

**Other Extreme — single node**: No communication cost, but very low parallelism (high wall clock time).

**🔥 Group-Based Algorithm (the practical middle-ground solution)**:
- Partition images into **g groups** (m = n/g images per group).
- Each group sent to g−1 other nodes/servers (compared with every other group once).
- **Total messages = g(g−1)**; **Total communication data ≈ n(g−1) ~ ng**.
- **Parallelism ≈ g(g−1)/2 = O(g²)** (= combinations of group pairs, nC2 style).
- **Trade-off**: increasing group size g increases communication cost but increases parallelism (reduces wall clock time)... 

  *(Correction per actual slide logic: larger group size increases communication (more reads) but reduces wall-clock time via more comparisons done in fewer group-pair rounds — trade-off between the two.)*
- Mapper: finds group u for image i; outputs (g−1) key-value pairs `({u,v}, (i,Pi))` for all v ≠ u.
- Reducer: one reducer per unique key {u,v}; stores images of both groups in memory; compares all pairs.
- Example: group size 1000 → ~2GB memory per reducer (2000 images), ~500,000 reducers; with 10,000-node cluster → 50 passes → **speedup of 10,000**.

---

## 15. Issues with Hadoop (⭐ good short-answer topic)

| Issue | Description |
|---|---|
| Small files problem | Hadoop designed for high-capacity; too many small files (<<128MB) burden NameNode memory |
| Slow processing speed | Data distributed/processed across cluster increases time, reduces speed vs in-memory systems |
| Batch-only, no real-time | Doesn't process streaming data natively |
| No delta/cyclic iteration | Not efficient for iterative algorithms (e.g., PageRank) — no native support for cyclic dataflow |

### 🔥 MapReduce Weaknesses & Limitations
- **Programming model**: hard to express everything as map+reduce; simple ops (e.g., word count sorted by frequency) need multiple MR jobs chained; lacks structure/data types.
- **No native iteration support**: each iteration reads/writes disk (overhead); must manually design algorithms to minimize iteration count.
- **Efficiency**: high communication cost (map→shuffle→combine→reduce); frequent disk writes; limited use of main memory.
- **Not for real-time streaming**: MR job needs to scan entire input.

### 🔥 Alternate Model — Motivation for Spark
- **Acyclic dataflow** limitation of Hadoop — inefficient for **working sets** (same data reused across iterations, e.g., PageRank's importance vector).
- Needed for: graph processing, machine learning (iterative), interactive analysis (Python/R) — reloading from persistent storage every iteration is too slow.

### 🔥 Apache Spark vs Apache Hadoop (⭐ comparison, likely 2/4-marker)
| | Hadoop | Spark |
|---|---|---|
| Processing | MapReduce | RDDs (Resilient Distributed Datasets) |
| Storage | HDFS (distributed file storage) | No native storage — mainly computation, runs on top of Hadoop/HDFS |
| Dependency | Provides full ecosystem (storage+resource mgmt+processing) | Doesn't need Hadoop to run, but often used with it |

---

## 16. Google BigQuery (Newer addition — likely 1-2 markers)

- **BigQuery** = autonomous data-to-AI platform; automates full data lifecycle (ingestion → AI-driven insights).

### Key Features
- AI-powered conversational/agentic experience (Gemini in BigQuery).
- Connect data to AI: SQL-based ML model creation/training; supports pre-trained, Vertex AI, or custom models.
- Built-in AI agents/workflow automation: natural language queries (data canvas), auto data prep, error detection.
- Open source/open formats: serverless Spark alongside SQL; managed **Apache Iceberg** tables via **BigLake**.
- Governance: powered by **Dataplex Universal Catalog** (metadata harvesting, profiling, data quality, lineage).
- **Architecture** decouples storage & compute for petabyte-scale analysis.

### 🔥 Underlying Google Infrastructure (good 1-marker matching question)
| Component | Role |
|---|---|
| **Borg** | Cluster management (compute) — ancestor to Kubernetes |
| **Colossus** | Distributed storage (file system) |
| **Jupiter** | Networking backbone (connectivity) |
| **Dremel** | Data analysis/query engine |

### Uses of BigQuery
1. Generative AI use cases (blend structured+unstructured data+Gemini via SQL).
2. Data warehouse migration (from Netezza, Oracle, Redshift, Teradata, Snowflake, Databricks).
3. Data integration/ELT: batch (Data Transfer Service), streaming (Pub/Sub subscriptions), CDC (Datastream).
4. Data-to-AI workflows: Colab Enterprise, BigQuery DataFrames, full ML flow.
5. Real-time analytics: built-in streaming ingestion, or Dataflow.
6. **Data clean rooms**: privacy-centric collaboration without copying/moving data.
7. Geospatial analytics: Earth Engine, Maps Platform integration.

---

# UNIT 2: BIG DATA INFRASTRUCTURES (COMPUTE/STORAGE)

## 17. Hadoop Ecosystem Overview

```
HDFS (storage) + MapReduce (computation) = foundation
```

| Tool | Purpose |
|---|---|
| Pig | Simplify analysis — scripting language, no complex code |
| Hive | Enable SQL — familiar queries on Hadoop data |
| Sqoop | Move data — transfer between RDBMS and Hadoop |
| Flume | Ingest logs — collect/move streaming event data |
| Oozie | Automate jobs — schedule/manage complex interdependent workflows |
| HBase | Real-time access — NoSQL DB for fast read/write |

### 🔥 Storage Type Use Cases (very common 2-marker/matching question)
| Storage | Data Type | Writes | Best For |
|---|---|---|---|
| **HDFS** | Unstructured | Append-only, no updates | Read entire file, analyze |
| **Hive** | Structured | — | SQL Analytics |
| **HBase/Cassandra** | Unstructured, columnar | Arbitrary writes | Real-time analytics, fast lookups |

**Motivational Exercise answer** (memorize the mapping logic):
1. Parsed/extracted transaction logs → **HIVE** (structured, ready for SQL analytics).
2. Unparsed raw transaction logs → **HDFS** (raw, unstructured).
3. Users/friends DB for periodic social network analysis → **HBase** (needs arbitrary/fast writes+reads).

---

## 18. 🔥🔥 Apache Hive

### What is Hive?
- Open-source **data warehouse infrastructure** on Hadoop; built on top of HDFS + MapReduce.
- Provides **SQL-like interface (HiveQL)** for structured data — no need for Java/MapReduce expertise.
- Originally by **Facebook** — for structured logs, multi-language access, traditional SQL workloads (filters/joins/aggregations).
- **Doesn't store data itself** — uses HDFS or HBase as actual store.
- Requires **definite schema** (structured data only).

### 🔥 Hive Limitations (frequently asked)
- Not good for record lookup (point queries).
- Not good for incremental addition of small batches.
- Not good for updates (Hive traditionally doesn't support UPDATE/DELETE well).
- Not good for unstructured/semi-structured data.
- Not real-time; not for OLTP; high query latency.

### 🔥🔥 Hive Components (High-yield — good 4-marker)
| Component | Role |
|---|---|
| WebUI / Hive CLI | GUI / shell to run queries |
| Thrift Server | Cross-language service provider for various programming languages |
| JDBC Driver | Connects Hive to Java applications |
| ODBC Driver | Connects apps supporting ODBC protocol |
| **Hive Driver** | Receives queries from all sources, sends to compiler |
| **Hive Compiler** | Parses query, semantic analysis, creates execution plan, talks to Metastore, converts HiveQL → **DAG of MapReduce jobs** |
| **Hive Metastore** | Central repo: table/partition structure, column types, SerDe info, HDFS file locations |
| **Hive Execution Engine** | Executes tasks in dependency order |
| **SerDe** | Serializer/Deserializer — reads disk data → Hive format and reverse (Thrift, JSON) |

### 🔥🔥 Hive Data Model (3 granular levels — very likely 2/4-marker)
| Level | Description |
|---|---|
| **Table** | Similar to RDBMS table; mapped to HDFS directory; supports filter/project/join/union |
| **Partition** | Organized by column/partition key for grouping; implemented as HDFS subdirectory |
| **Bucket** | Subdivision of partition; based on hash function of specified column, improves query efficiency |

### Hive Advantages
- No Java/programming expertise needed.
- SQL-like queries transformed to MapReduce/Spark jobs.
- Supports plain text, RCFile, HBase storage.
- Can operate on compressed data.
- Supports **UDFs** (user-defined functions).

---

## 19. 🔥🔥 HBase & Cassandra (Columnar Storage) — Very High Yield Unit

### Row vs Column Storage (🔥 fundamental concept)
- **Row storage**: entire row stored together (one line per row) — efficient for **adding/reading a single row** (OLTP/transactions).
- **Column storage**: each column separate file — efficient for **analytics** (e.g., relationship between two columns) since fewer I/Os needed for analytical queries scanning specific columns.
- History: DBs started row-oriented for transactions (bank balance read/update); Column DBs became popular with Big Data for analytics on large datasets & unstructured data.

### 🔥 HBase vs Cassandra Overview
- **HBase**: distributed column-oriented DB built ON TOP of HDFS; data organized as rows/columns of a table.
- **Cassandra**: distributed, **peer-to-peer** DB built by Facebook; inspired by **Amazon Dynamo DB**; same data model as HBase (inspired by Google **BigTable**).

### 🔥🔥 Data Model Terminology (HBase) — very likely 4-marker
| Term | Meaning |
|---|---|
| **Table** | Collection of rows sorted by RowID |
| **Row** | Collection of column families identified by RowKey (byte array, primary key, indexed for fast lookup) |
| **Column** | Key-value pair: `ColumnFamilyName:ColumnName` |
| **Column Family** | Collection of variable number of columns (part of schema; columns themselves are NOT part of schema — **dynamic columns**, since column names encoded in cells, different rows can have different columns in same family) |
| **Cell** | Combination of {row key, column, timestamp/version} as byte array — stores data |
| **Timestamp** | System timestamp or version number; allows single column to have different values at different times |

### 🔥🔥 HBase Architecture (Master-Slave — very high-yield)
| Component | Role |
|---|---|
| **MasterServer** | Assigns regions to RegionServers; detects failure via ZooKeeper; load balances regions; supports admin (schema changes, table/column family creation); analogous to HDFS NameNode |
| **RegionServer** | Slave; contains regions; communicates with client for data ops; serves reads/writes (using write-ahead log); decides region size/splitting; analogous to HDFS DataNode |
| **Region** | Split part of a table (horizontal range partition of rows); subset of table's rows |

- Table starts as single region → Master monitors load → dynamically splits into multiple regions.
- **META table**: keeps track of regions.

### 🔥 HBase Read/Write Path
- **BlockCache** & **MemStore**: reside in RAM.
- **HFiles** & **Write Ahead Log (WAL)**: persisted to HDFS.
- **MemStore**: write cache, sorted KeyValues in memory, specific per Region.
- **BlockCache**: read cache, LRU-evicted, shared across regions in a RegionServer.
- **WAL**: on disk, used for crash recovery.
- **HFile**: sorted KeyValues on disk.

### 🔥🔥 Cassandra Architecture — Peer-to-Peer (contrast heavily with HBase)
- **No master** — all nodes play same role; each independent yet interconnected.
- Any node can accept read/write requests regardless of data's actual location.
- Nodes organized as a **Ring**.
- On node failure: replicas (other nodes) serve read/write requests.
- **Coordinator node**: proxies between client and nodes holding data (client approaches any node).

### 🔥🔥 Cassandra Write Path Components (know this cold)
| Component | Role |
|---|---|
| **Commit log** | Crash-recovery mechanism; every write first goes here |
| **Mem-table** | Memory-resident structure; data written here after commit log |
| **SSTable** | Disk file; mem-table flushed here when threshold reached |
| **Bloom filter** | Quick, non-deterministic membership-test algorithm; special cache, checked on every query |

- **CQL** (Cassandra Query Language): access via `cqlsh` prompt or app language drivers; treats Keyspace (database) as container of tables.

### 🔥🔥 Cassandra vs HBase — Key Differences (VERY likely 4-marker)
| Aspect | HBase | Cassandra |
|---|---|---|
| Architecture | Master-Slave (HMaster/RegionServer) | Peer-to-Peer (Ring, no master) |
| Coordination | Master coordinates | Request coordination over partitioned dataset, no master |
| Membership/failure detection | Master (+ZooKeeper) | Nodes themselves (Ring membership) |
| Storage engine | Relies on **HDFS** | **Local persistence** — does NOT rely on HDFS |
| Query Language | HBase Shell (limited) | **CQL** — richer functions |
| Data organization | Regions across RegionServers | Partitions (partition key + columns) across nodes in Ring |
| Conflict resolution | — | **Last-Write-Wins** via timestamps (no coordination needed) ⭐ |

### Cassandra Key Features
- Elastic scalability (linear performance scaling as nodes added).
- Replication for failure prevention (like HBase).
- Handles structured, semi-structured, unstructured — dynamically adapts.
- Supports **ACID** properties.

---

## 20. 🔥🔥 Apache Sqoop (Legacy — RETIRED)

- **SQL-to-Hadoop**: bulk transfer between HDFS and RDBMS (MySQL, Oracle, PostgreSQL).
- **Bi-directional**: Import (RDBMS→HDFS/Hive/HBase), Export (HDFS→RDBMS).
- Uses **MapReduce/YARN** for parallel transfer.
- Supports incremental loads + full table imports; connector-based (JDBC) architecture.

### Sqoop Import/Export mechanics
- **Import**: inspects DB metadata → transfers data via map-only Hadoop job → stores as CSV in HDFS (newline = record separator).
- **Export**: inspects target table metadata → each map task transfers over multiple transactions for throughput/resource efficiency.

### 🔥 Sqoop Limitations → Why Retired
- Batch-only, no real-time/streaming.
- High overhead for small datasets (MapReduce startup cost).
- No built-in data validation.
- **No CDC (Change Data Capture) support**.
- **Status: Retired June 2021**, moved to Apache Attic.

---

## 21. 🔥 Apache Flume (Legacy — DORMANT)

- Distributed, reliable service to collect/aggregate/move large volumes of **streaming log data** to centralized store (typically HDFS).

### 🔥🔥 Flume Agent Architecture (know this — classic 2-marker)
| Component | Role |
|---|---|
| **Source** | Consumes events from external data generator |
| **Channel** | Temporary buffer storing events until consumed (bridges source-sink); e.g., JDBC channel, File system channel |
| **Sink** | Delivers events to final destination (HDFS/HBase) |
| **Agent** | Independent daemon (JVM) receiving Flume events, storing to centralized store |
| **Flume Event** | Basic unit of data transport — payload of byte array |

### Flume Limitations → Why Dormant
- Streaming-only, no batch processing.
- No data transformation capability.
- High latency, not suited for real-time analytics.
- **Status: Dormant since Oct 2024**, not recommended for new projects.

---

## 22. 🔥🔥 Apache SeaTunnel (Modern Replacement for Sqoop+Flume — HIGH YIELD)

### What is SeaTunnel?
- Easy-to-use, ultra-high-performance **distributed data integration platform**; syncs massive data between heterogeneous systems.
- Top-level ASF project (graduated June 2023); originally "Waterdrop" (2017), renamed Oct 2021; entered Incubator Dec 2021.
- **Unifies batch AND streaming** in one tool.
- **100+ connectors**; config via **1 config file** (HOCON/JSON/SQL — no Java/Scala needed).

### 🔥 Why SeaTunnel replaces Sqoop & Flume
| vs Sqoop | vs Flume |
|---|---|
| 100+ connectors (vs limited JDBC) | Handles batch AND streaming (Flume = streaming only) |
| Batch AND streaming in one tool | Built-in transform stage (Flume has none) |
| Built-in **CDC** for real-time sync | 100+ connectors vs Flume's narrow log sources |
| No MapReduce dependency — runs on **Zeta/Flink/Spark** | Actively maintained Apache Top-Level Project |

### 🔥🔥 The N×M Connector Problem (conceptual, good 2-marker)
- **Before**: point-to-point scripts — N sources × M sinks = N×M custom scripts to build/maintain (e.g., 3 sources×3 sinks = 9 scripts).
- **After (SeaTunnel)**: N+M connectors total (write once, reuse anywhere) — e.g., 3+3=6 connectors.

### 🔥🔥 Key Features of SeaTunnel
- **Batch & Streaming Support**: same engine/config format for both.
- **Large Connector Library**: 100+ sources/sinks.
- **Multiple Execution Engines**: Zeta (built-in, native), Apache Flink, Apache Spark.
- **CDC**: continuous capture of DB changes (insert/update/delete) without full-table scans.
- **Exactly-Once Fault Tolerance**: checkpointing + distributed snapshots.
- **Low-Code Configuration**: HOCON/JSON/SQL declarative pipelines.
- **Automated Schema Evolution**: auto-detects/propagates upstream schema changes.

### 🔥🔥 Pipeline Architecture: Source → Transform → Sink (very likely 4-marker with config file)
| Stage | Role |
|---|---|
| **Source (Read)** | Connects to origin (MySQL, Kafka, HDFS...), emits rows |
| **Transform (optional Reshape)** | Filters/maps/splits/SQL-transforms rows in-flight; can be skipped |
| **Sink (Write)** | Delivers final rows to target (warehouse/lake/queue) |

- Stages connect via `plugin_output`/`plugin_input` tags; if unset, output auto-flows to next stage ("pipe sections bolted end to end").

### 🔥 Config File Structure (HOCON — know the 4 blocks)
```hocon
env { job.mode = "BATCH" }       # global run settings: BATCH vs STREAMING
source { MySQL { result_table_name="orders" ... } }   # where to read; every source has result_table_name
transform { Filter { fields=[id,amount,city] } }        # optional reshaping
sink { Hive { table="orders_clean" } }                   # where final rows land
```

### 🔥 Execution Engines Comparison
| Engine | Type | Best For |
|---|---|---|
| **Zeta** | Native/built-in | Lightweight, pure sync jobs; recommended for most deployments; Local/Hybrid/Separated cluster modes |
| **Apache Flink** | Streaming-first | Mature streaming semantics, low-latency event processing, existing Flink users |
| **Apache Spark** | Batch-first | Heavy offline/batch ETL, existing Spark users |

### 🔥🔥 Connector V2 API & Architecture
- **Config File** (HOCON/JSON/SQL) — job fully described, no code to compile.
- **Connector V2 API** (translation layer) — decouples plugins from any single engine.
- **Execution Engine** — Zeta/Flink/Spark, chosen per job.
- **100+ connectors**: Databases (MySQL, PostgreSQL, Oracle, SQL Server, TiDB, MongoDB), Lakes/Warehouses (Hive, Iceberg, Hudi, Doris, ClickHouse, StarRocks, Snowflake), File Systems (HDFS, S3, OSS, Local FS, FTP), Message Queues (Kafka, Pulsar, RabbitMQ, RocketMQ, SQS).

### 🔥🔥 Change Data Capture (CDC) — Deep Dive (high-yield across Unit 2)
- Identifies/tracks row-level changes (insert/update/delete) in source DB, delivers to downstream near real-time.
- **How CDC works**:
  1. **Monitor DB Log** — reads transaction log (MySQL binlog, Postgres WAL).
  2. **Capture Changes** — extracts only INSERT/UPDATE/DELETE events, no full-table scans.
  3. **Deliver** — streams changes to target (lake/warehouse/queue) in real-time.
- CDC connectors: MySQL-CDC, PostgreSQL-CDC, Oracle-CDC, MongoDB-CDC.
- Benefits: avoids locking, minimizes source DB impact, supports exactly-once delivery.
- **Why CDC matters**: feeds Hudi (upserts/ACID on lakes), Druid (real-time Kafka ingestion for OLAP), Iceberg (row-level updates, time-travel).

---

## 23. 🔥🔥 Data Lake, Data Warehouse, Data Lakehouse (Foundational concepts — expect conceptual 4-marker)

### Evolution: Databases → Data Warehouse → Data Lake
- **Databases**: store/manage data for individual apps/operational systems.
- **Data Warehouse**: consolidates/structures data from multiple sources for BI; schema-on-write.
- **Data Lake**: stores massive diverse raw data at scale (on-prem HDFS or cloud); schema-on-read.

### 🔥🔥 Data Lake vs Data Warehouse (classic comparison table — VERY likely 4-marker)
| Aspect | Data Warehouse | Data Lake |
|---|---|---|
| Data | Primarily structured | Structured, semi-structured, unstructured |
| Structure defined | **Schema-on-write** (before storage) | **Schema-on-read** (when used) |
| Storage cost | Generally more expensive | Lower-cost, scalable |
| Data formats | Managed by warehouse system | CSV, JSON, Parquet, ORC, Avro etc. |
| Compute | Tied to warehouse system | Different engines usable |
| Management | More managed/optimized for analytics | More flexible, needs more config |

### Data Lake — Strengths & Gaps
- Strengths: massive scale storage in original form (low cost), flexible (reusable for different workloads), multiple compute engines.
- Gap: storage alone insufficient — still need to organize/manage collections, query efficiently, reliably update data over time.

### 🔥🔥 Data Lakehouse
- Combines: Data Lake (scalable storage, open formats, diverse data, multiple engines) + Data Warehouse (reliable management, analytical performance, easy update/query).
- **How**: keep lake's storage/file formats, ADD warehouse-like table capabilities via a modern **open table format**.
- Note: Lakehouse does NOT replace the data lake — adds capabilities to use lake data as reliable analytical tables.

### 🔥🔥 Open Table Formats — What Problem Do They Solve?
A data lake stores files; analytics needs tables. Open table format answers:
1. **File Mapping**: which files belong to a table?
2. **Schema Definition**: what is the table's schema?
3. **Data Mutation**: how to safely add/update/delete data?
4. **Version Control**: how to track different versions?
- "Open" = open specification, not tied to one storage/compute engine.
- Does NOT replace underlying storage — manages how files are treated as tables.
- Examples: **Apache Iceberg, Apache Hudi**.

---

## 24. 🔥🔥🔥 Apache Iceberg (VERY HIGH YIELD — expect multiple questions)

### Origin & Goals
- Created by **Netflix (2017)** to address Hive table format limitations for huge analytical tables.
- Open-sourced 2018, donated to ASF.
- **3 Design Goals**: 
  1. **Consistency** — reliable, atomic table changes.
  2. **Performance** — efficient query planning without excessive file listing.
  3. **Ease of use** — users don't need to know physical layout.
- Iceberg does NOT provide storage or compute — enables existing engines to work with lake-stored tables.

### 🔥🔥 Three Layers of an Iceberg Table (VERY high-yield — draw this structure)
```
1. CATALOG  → points to current metadata file ("Find the table")
2. METADATA → describes/tracks the table ("Describe and track the table")
     - Metadata file: schema, partitioning, snapshots
     - Snapshot: table state at a point in time
     - Manifest list: identifies manifest files for a snapshot
     - Manifest files: list data files + info about them
3. DATA     → actual data files (e.g., Parquet) ("Store the actual data")
```

### 🔥🔥 Iceberg's 6 Key Features (HIGH YIELD — expect a 4-marker naming/explaining these)

**1. Row-Level Operations (COW vs MOR)**:
| Copy-on-Write (COW) | Merge-on-Read (MOR) |
|---|---|
| Entire data file rewritten with updated data | Change written separately, merged with base file at read time |
| More data written, simpler/faster reads | Less data written, extra read-time work/latency |

**2. Hidden Partitioning**: partition spec fixed but hidden from user's logical view.
- With: user queries logical column directly (e.g., `WHERE event_timestamp >= ...`), Iceberg auto-maps to physical partitions.
- Without: must manually add physical partition columns (`event_year`, `event_month`...) — complex, breaks if partitioning changes.
- Advantage: DBAs evolve partitioning without breaking existing queries.

**3. Partition Evolution**: partitioning can change WITHOUT rewriting existing data.
- E.g., older data partitioned by month, newer by day — same logical query works across both.
- Without this: must rewrite/repartition existing data (expensive) OR sacrifice new scheme's benefits.

**4. Snapshots**: each committed state recorded as a snapshot.
- **Time travel**: `SELECT * FROM table AS OF snapshot_1`.
- **Rollback**: `ROLLBACK TABLE table TO snapshot_1` — restores prior state.
- No need to maintain separate full copies of the table.

**5. Schema Evolution**: schema can change without rebuilding all existing data (e.g., adding a `discount` column) — existing records remain as-is.

**6. Reliable Updates**: ACID transactions — commits are atomic; readers always see either fully-before or fully-after state, never partial updates.

---

## 25. 🔥🔥🔥 Apache Hudi (VERY HIGH YIELD)

### What Problem Does Hudi Solve?
- **Apache Parquet** = efficient columnar file format, but just a FILE FORMAT — doesn't define how to update a single record while maintaining consistency/history.
- **Hudi** = layer on top of Parquet/data lake files giving them **table semantics**: tracks record keys, tracks commits, manages updates/deletes, maintains history, provides incremental access.
- Does NOT replace the data lake — adds a layer.

### 🔥🔥 Record Key & preCombineField (very likely 4-marker with numeric example)
- **Record Key**: uniquely identifies a record (e.g., `recordKey = order_id`) — similar to primary key, but used specifically for write operations.
- **preCombineField**: determines which version of a record "wins" when multiple versions arrive in a single write.

### 🔥🔥 Hudi 1.0.2 — 3 Merge Modes (memorize with example)
| Mode | Rule |
|---|---|
| **COMMIT_TIME_ORDERING** | Record from the LATER Hudi commit wins |
| **EVENT_TIME_ORDERING** | Record with GREATER preCombineField value wins |
| **CUSTOM** | User-provided merger defines the rule |

**Worked example**: order_id=1001, incoming update_at=500, current update_at=2000:
- EVENT_TIME_ORDERING → selects 2000 record (greater value wins).
- COMMIT_TIME_ORDERING → selects the later-committed one (500, if it's the later commit) — commit recency wins, NOT value.

### 🔥 Hudi Write Operations
| Operation | Behavior |
|---|---|
| **Insert** | Adds new record whose key doesn't exist |
| **Upsert** | Insert if key absent; else merge with existing per merge rules |
| **Delete** | Removes record from current state, but preserved in commit history |

### 🔥🔥 Hudi Table Types: CoW vs MoR (VERY high-yield, compare directly with Iceberg's same concept)
| Copy-on-Write (CoW) | Merge-on-Read (MoR) |
|---|---|
| Updates modify existing base files directly | Updates written to delta log files first |
| New version of affected file written | Reads combine base files + log files for latest state |
| Simpler reads (data already in base files) | Writes cheaper; reads need merging |
| Trade-off: more data written | Trade-off: writes cheap, reads may need extra merge work |

- Both provide same table-level features; differ in HOW updates are stored/served.

### 🔥🔥 The Hudi Timeline
- Every change recorded as event on the **timeline** — provides table history, determines currently valid state.
- **Commit** = a completed write operation (commit ≠ changed row — one commit may not change final state for every record).
- Each commit has a unique commit time.
- Timeline stored in table's **`.hoodie`** metadata directory.

### Time Travel & Incremental Queries
- **Time travel**: query table as of a previous commit instant; doesn't undo/modify table, just changes which point in history is read.
- **Incremental queries**: retrieve only data affected by commits AFTER a specified instant (not full table reconstruction); uses `hoodie.datasource.query.type=incremental`.

### Delete & Rollback
- **Delete**: creates new commit; record absent from current state but visible via time travel to before deletion.
- **Rollback**: reverses a completed commit, restores table to pre-commit state (operates on specific commit).

### Compaction, Indexing, Metadata Table
- **Compaction**: (MoR) merges accumulated delta log files into new base files.
- **Indexing**: Hudi index maps record keys → file locations (needed to locate record for update/delete).
- **Metadata Table**: stores metadata (file listings, record/file metadata, column stats) so Hudi avoids repeatedly scanning underlying files.

### Hudi APIs
- Integrates directly with **Apache Spark** (datasource + Spark SQL extensions).
- Spark SQL: create/read/update/delete/query Hudi tables.
- Also provides REST-based table-management APIs — operate on Hudi table abstraction, not raw Parquet+metadata.

---

## 26. 🔥🔥🔥 Hudi vs Iceberg (EXTREMELY HIGH YIELD — appeared explicitly in slides as a dedicated comparison; near-certain 4-marker)

### 1. Mental Model of "Table"
| Iceberg | Hudi |
|---|---|
| Table = sequence of **snapshots**; every write = new snapshot | Table = sequence of **commits**; every row has identity |
| Thinks in terms of "which files make up table at this version" | Every table has a **primaryKey** — record identity NOT optional |
| No issue if no primary key set — many tables are pure append/overwrite, no record identity concept | Maintains **timeline** (`.hoodie` folder) logging every action (insert/upsert/delete/compaction/clustering/rollback) |
| — | Because it always knows "this row is an update to that row," can resolve version-winning via **preCombineField** |

### 2. How an Update Physically Happens
| | Iceberg | Hudi |
|---|---|---|
| **Copy-on-Write** | Rewrites file containing changed row entirely | Same idea — rewrite base Parquet file immediately |
| **Merge-on-Read** | Writes small **delete file** ("ignore this row"); query engine merges at read time (newer addition) | Changes go into **row-based delta log** appended next to base file; background compaction merges later — **Hudi's original, signature design** |

### 3. Concurrency & Conflict Resolution
| Iceberg | Hudi |
|---|---|
| **Optimistic concurrency at snapshot level** — two writers race to create next snapshot; first commit wins, loser retries | **Record-level conflict resolution** via preCombineField — if two versions of same key arrive, keeps one with greater field value (e.g., later `updated_at`) regardless of write order |

### 4. Time Travel & Incremental Reads
| Iceberg | Hudi |
|---|---|
| Query by **snapshot ID or timestamp** — natural since whole table is snapshot-versioned | Query `as.of.instant` a commit, OR genuinely incremental read (`hoodie.datasource.query.type=incremental`) returning only changed rows since a commit |

---

## 27. 🔥🔥🔥 Apache Druid (VERY HIGH YIELD)

### What is Druid?
- Open-source, **column-oriented database** purpose-built for fast, ad-hoc **OLAP** queries on event-driven data.
- Blends ideas from data warehouses + time-series DBs + search systems.
- Optimized for "how many/how much/trending how" questions over huge, constantly-arriving datasets, **sub-second** response.
- **NOT** general-purpose OLTP — no arbitrary multi-table joins, no row-level transactional updates.

### 🔥 Why Druid? (vs traditional approaches)
| Traditional | Druid |
|---|---|
| Data warehouses: powerful SQL, but seconds-to-minutes latency, batch-limited freshness | Sub-second aggregation across trillions of rows |
| OLTP DBs: fast row writes, not built for billions of row scans/aggregation | Native streaming ingestion — queryable within seconds of arrival |
| Search engines: fast filtering, weak numeric aggregation/rollups | Time-partitioned/pruned segments — scans only relevant data |
| | Optional **roll-up**: pre-aggregates raw events at ingest time |

### 🔥🔥 Data Organization Hierarchy (VERY high-yield)
```
Datasource (= logical "table")
  → Time Chunk (e.g., hourly, daily partition)
    → Segment (fundamental storage unit — immutable, columnar)
      → Compressed columns + indexes
```
- **Segment**: immutable columnar file; each column stored/compressed/indexed separately; **bitmap indexes** on dimension columns for fast filtering; once published moves to **deep storage** (S3/HDFS) and handed to Historicals.

### 🔥🔥 Druid Architecture — 3 Server Types, 5 Process Types (HIGH-YIELD, expect a 4-marker)
| Server Type | Processes | Responsibility |
|---|---|---|
| **Master** | Coordinator, Overlord | Manages data availability, assigns ingestion workloads |
| **Query** | Broker, Router | Handles/routes queries from clients |
| **Data** | Historical, MiddleManager | Stores queryable segments, executes ingestion tasks |

**Detail per process**:
| Process | Role |
|---|---|
| **Coordinator** | Manages data availability across cluster; assigns/balances/drops segments on Historicals |
| **Overlord** | Controls assignment of ingestion workloads, coordinates segment publishing |
| **Broker** | Receives external queries, prunes to relevant segments, forwards subqueries, merges results |
| **Router** | Optional unified API gateway in front of Brokers/Coordinators/Overlords; hosts web console |
| **Historical** | Downloads immutable segments from deep storage, serves queries against them |
| **MiddleManager** | Executes ingestion tasks, indexes streaming data real-time via per-task **Peon** processes |

- Key rule: **Query servers never touch storage directly** — Broker prunes & forwards to Data servers, which scan. Master coordinates but never touches query traffic/raw data.

### 🔥 Ingestion: Streaming vs Batch
| Streaming | Batch |
|---|---|
| Supervisor manages MiddleManager tasks reading continuously from Kafka | Native batch or Hadoop-based, loads bounded dataset in one job |
| Queryable within seconds; exactly-once semantics | Used for historical backfills/reprocessing |
| Segments auto-handed-off as time chunks close | Can reindex existing segments (change granularity/schema); no continuous supervisor |

### 🔥🔥 How a Query Executes (5-step flow — good 4-marker)
1. Query (SQL or native JSON) enters via **Router** or directly at **Broker**.
2. Broker prunes segment list — first by time, then other partitioning attributes.
3. Broker identifies which Historicals/MiddleManagers currently serve those segments, forwards re-written sub-query to each.
4. Each Data-server scans only its LOCAL segments, returns partial results.
5. Broker merges all partial results into final answer, returns to caller.

---

## 28. 🔥🔥🔥 Apache Airflow (VERY HIGH YIELD)

### What is a DAG? (foundational — expect 1-2 markers)
- **DAG (Directed Acyclic Graph)**: mathematical structure modeling workflows as tasks with explicit dependencies; no task can depend on itself (directly/transitively).
- **3 defining properties**:
  1. **Directed**: edges carry direction (A→B means B runs only after A completes).
  2. **Acyclic**: no cycles — guarantees finite execution order.
  3. **Graph**: nodes=tasks, edges=dependencies.
- Important: DAG defines WHAT and in WHAT ORDER — does NOT execute itself; **orchestrator (Airflow)** executes.

### 🔥 DAG Core Properties
| Property | Meaning |
|---|---|
| Topological Ordering | Tasks sortable so each appears before dependents |
| Parallel Execution | Independent tasks (no shared dependency) run simultaneously |
| Deterministic Execution | Same inputs → same task order always |
| Fault Isolation | Failed task retried in isolation, no full pipeline re-run |
| Idempotency | Safe re-triggering — reruns produce same result |

**Real-world DAG examples**: ETL (Extract→Transform→Validate→Load→Notify), ML training (Preprocess→Train→Evaluate→Register), CI/CD (Build→Unit Test→Integration Test→Deploy).

### What is Apache Airflow?
- Open-source platform to **programmatically author, schedule, monitor workflows** as DAGs written in **Python**.
- Originated at **Airbnb (2014)**; Apache Top-Level Project **2019**.
- Philosophy: **"Configuration as Code"** — workflows are Python scripts (versioned, testable, peer-reviewed); separates workflow logic from execution infra; NOT a streaming framework — it's an **orchestrator**.
- Motto: **"Author, Schedule, Monitor"**.

### 🔥🔥 Apache Oozie — Legacy Predecessor (know why Airflow replaced it — HIGH YIELD comparison)

**What is Oozie?**
- Server-based workflow scheduler for Hadoop jobs; coordinates MapReduce/Pig jobs via **XML-based** workflow definitions.
- Java Tomcat web application; receives requests, triggers workflow actions via Hadoop execution engine; unique callback HTTP URL notifies on completion (or polls).

**Oozie Job Types**:
| Type | Role |
|---|---|
| **Workflow** | Specified sequence of Hadoop jobs, with outcome-based decision points |
| **Coordinator** | Scheduled workflow job — time AND data dependent |
| **Bundle** | Batches a set of coordinators |

**Oozie Workflow Nodes** (🔥 memorize — likely diagram-based 4-marker):
| Node Type | Role |
|---|---|
| Start / End | Beginning/end of workflow |
| Error Node | Designates error occurrence + message |
| Action Node | Represents workflow task (MapReduce, Hive, Pig, Sqoop, Shell, Java) |
| **Fork** | Starts parallel tasks |
| **Join** | Merges parallel tasks (waits for all branches) |
| **Decision** | Switch-like conditional branching |

**Oozie Architecture Components**: MasterServer/Oozie Server (central coordination), Workflow Engine (executes DAG step by step), Coordinator Engine (time/data triggers), Metadata DB (Derby/MySQL — stores state/history/metadata).

### 🔥🔥 Why Airflow Replaced Oozie (VERY high-yield 4-marker — table format ideal)
| Oozie Limitations | Airflow Advantage |
|---|---|
| XML-based — verbose, hard to version-control, error-prone | **Python-native** — DAGs as code |
| No Python support — only Java/Shell actions | Dynamic DAGs — generated programmatically at runtime |
| Tightly coupled to Hadoop/YARN/HDFS | Cloud-agnostic: AWS, GCP, Azure, Kubernetes, on-prem |
| No dynamic task generation — static DAGs only | Rich operator library (1000+ providers) |
| Poor debugging — limited failure visibility | Modern web UI — live logs, retries, Gantt view |
| No support for modern stores (S3, BigQuery, Snowflake) | Pluggable executors: Local, Celery, Kubernetes, hybrid |
| Slow community innovation (maintenance mode since 2020) | Active community — top-10 Apache project |
| Basic UI | SLA monitoring, alerting, **XCom** for cross-task comm |

### 🔥🔥 Airflow Core Architecture (very high-yield — expect naming+role 4-marker)
| Component | Role |
|---|---|
| **Web Server** | FastAPI/Gunicorn HTTP server serving Airflow UI (trigger DAGs, inspect logs, manage connections/vars) |
| **Scheduler** | "Heart of Airflow" — continuously scans DAG files, evaluates dependencies, submits ready tasks to Executor |
| **Executor** | Determines HOW/WHERE tasks run (Local, Celery, Kubernetes) |
| **Metadata Database** | PostgreSQL/MySQL — stores DAG run state, task instances, connections, variables, XComs, SLA records |
| **Worker(s)** | Execute task code; with CeleryExecutor, pull tasks from message queue (Redis/RabbitMQ) |
| **DAG Processor** | Parses Python DAG files regularly, registers in metadata DB; separate process from Airflow 2.x+ |

### 🔥 DAG as Python Code — Structure
```python
with DAG(dag_id='etl_pipeline', start_date=..., schedule='@daily', catchup=False) as dag:
    t1 = PythonOperator(task_id='extract', python_callable=extract)
    t2 = PythonOperator(task_id='transform', python_callable=transform)
    t3 = PythonOperator(task_id='load', python_callable=load)
    t1 >> t2 >> t3   # sets sequential dependency
```
- **`with DAG()`**: sets dag_id, start_date, schedule; all operators in block registered as tasks.
- **Operators**: PythonOperator, BashOperator, SQLOperator etc — each = one node/task.
- **`>>`**: sets dependency (sequential); independent tasks set separately run in parallel.
- **`schedule=`**: accepts cron strings (`@daily`), timedelta, Dataset objects (event-driven), or None (manual).

### 🔥 Operators — Building Blocks
- Operator = template for a single task unit; 40+ built-in + 1000+ via provider packages.
- Categories: Core (PythonOperator, BashOperator, BranchPythonOperator, EmptyOperator), Database (PostgresOperator, MySqlOperator, BigQueryOperator), Cloud (AWS/GCP/Azure specific), ML/Spark (SparkSubmitOperator, DatabricksRunNowOperator).
- Providers pip-installable (e.g., `apache-airflow-providers-google`).

### 🔥🔥 Airflow Key Features (good rapid-fire list for 1-markers)
| Feature | Description |
|---|---|
| Dynamic Pipelines | Tasks generated via loops/conditions/external data at parse time |
| Extensible | Custom operators/hooks/sensors/executors via plugin system |
| Rich Web UI | Graph, Grid, Gantt, Tree views; live logs, retry controls |
| Flexible Scheduling | Cron, timedelta, Dataset-trigger, manual; backfill support |
| **XCom** | Cross-task communication — tasks share small data values (push/pull), stored in metadata DB |
| **Sensors** | Wait-operators — block until external condition met (file arrival, API response, row count threshold) |
| SLA & Alerting | Per-task SLAs; email/Slack alerts on miss/failure |
| Connections & Variables | Centralized credential store + runtime config injection |

### 🔥🔥 Executors & Scaling Strategies (very high-yield table)
| Executor | Use Case | Key Trait |
|---|---|---|
| **Sequential** | Development only | One task at a time, SQLite backend, not for production |
| **Local** | Small teams/single node | Subprocess parallelism, needs PostgreSQL |
| **Celery** | Distributed scale-out | Worker pool via Redis, horizontally scalable, production-grade |
| **Kubernetes** | Cloud-native/serverless | One K8s pod per task, auto-scales to zero, best isolation |
| **CeleryKubernetes** | Hybrid | Celery for fast tasks, K8s for heavy tasks |

### 🔥 Scheduling Strategies & Backfill
| Strategy | Syntax/Behavior |
|---|---|
| Cron Expression | `schedule='0 6 * * *'`; aliases `@daily`, `@hourly` etc |
| Timedelta | `schedule=timedelta(hours=6)` — relative to previous run |
| **Dataset Trigger** (2.4+) | `schedule=Dataset('s3://...')` — runs when upstream DAG updates dataset; true data-driven, no polling |
| Manual/None | `schedule=None` — UI/CLI/API triggered only |

- **`catchup=True`** (default): schedules ALL missing historical runs if DAG has past start_date.
- **`catchup=False`**: only runs from current time onward — prevents flooding scheduler.
- **CLI Backfill**: `airflow dags backfill --start-date ... --end-date ... dag_id` — manually trigger historical range.

### Airflow Web UI Components ⭐
DAGs List, Graph View, Grid View, Gantt View, Task Logs, XComs Browser, Connections, Variables.

### Real-World Case Studies ⭐ (quick recall)
- **Airbnb**: core ETL, 1000+ DAGs, 100K+ task instances/day.
- **Spotify**: recommendation model training, KubernetesExecutor, 500+ DAGs.
- **LinkedIn**: feed ranking, data quality gates, SLA reduced latency incidents ~40%.
- **NASA JPL**: scientific data pipelines, sensor polling before processing.

---

## 29. 🔥 Data Lakehouse Ecosystem — How Everything Connects (great 4-marker synthesis question)

```
SeaTunnel (ingestion/CDC) 
   → Data Lake storage (HDFS/S3, Parquet files)
      → Open Table Format (Iceberg/Hudi) — adds ACID, schema evolution, time travel
         → Query engines (Spark, Flink, Druid, Hive)
   → Orchestrated end-to-end by Airflow (or legacy Oozie)
```
- SeaTunnel replaces Sqoop (batch RDBMS transfer) + Flume (streaming log ingestion) with a unified batch+streaming+CDC tool.
- Iceberg/Hudi turn raw lake files into ACID-compliant, evolvable "tables" — the "Lakehouse" capability.
- Druid specializes in real-time OLAP on top of ingested (often CDC-fed) event data.
- Airflow orchestrates the entire pipeline (ingestion→transform→load→analytics) as Python DAGs, replacing XML-based Oozie.

---

# 🎯 Quick-Reference: Highest-Yield Topics for 4-Markers

1. **HDFS Architecture** (NameNode/DataNode/Secondary NameNode) + Fault Tolerance (Replication vs Erasure Coding) + HA
2. **MapReduce pseudocode** for relational ops (Select/Project/Union/Intersection/Difference/Join/Group-by) — practice writing these
3. **PageRank** — transition matrix, eigenvector equation, fixed-point iteration
4. **3-way Join Complexity** & **Similarity Join / Group-based algorithm** numeric reasoning
5. **YARN Architecture** (RM/NM/AM/Container) + Schedulers (FIFO/Fair/Capacity) + Failure handling table
6. **Hive Components & Data Model** (Table/Partition/Bucket)
7. **HBase vs Cassandra** architecture (Master-Slave vs Peer-to-Peer) + write path components
8. **SeaTunnel** — Source/Transform/Sink pipeline, N×M problem, why it replaces Sqoop/Flume
9. **Iceberg's 3 layers + 6 features** (COW/MOR, hidden partitioning, partition evolution, snapshots, schema evolution, reliable updates)
10. **Hudi vs Iceberg** — the 4-point comparison (table model, update mechanics, concurrency, time travel) — appeared verbatim in slides
11. **Hudi**: preCombineField merge modes, CoW vs MoR, timeline/commits
12. **Druid architecture**: 3 server types/5 processes + query execution flow + segment hierarchy
13. **Oozie vs Airflow**: why Airflow replaced Oozie (table), DAG node types (Fork/Join/Decision)
14. **Airflow architecture**: Scheduler/Executor/Webserver/Metadata DB + Executor types (Local/Celery/K8s)
15. **Data Lake vs Data Warehouse vs Lakehouse** comparison
