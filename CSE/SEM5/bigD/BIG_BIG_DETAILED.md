# Big Data – Unit 1 Complete Study Notes

> Structured for revision and understanding.  
> Every major topic follows: **Definition → Logic → Example → Analogy → Key points → Comparisons**.

---

## 1. What is Big Data?

### Definition
Big Data is data whose **scale, diversity, and complexity** require new processing methods, architectures, techniques, algorithms, and analytics to extract useful knowledge and support decision-making.

There is no single universally accepted definition.

### Clean understanding
Normal data can often be handled by one machine and traditional databases. Big Data becomes difficult because the data is too large, too fast, too diverse, or too unreliable for conventional systems.

### Logic
Traditional systems struggle when:
- Data volume becomes extremely large.
- Data arrives continuously and rapidly.
- Data comes in different formats.
- Data must be processed across many machines.
- Hardware failures become common.

### Example
Healthcare systems collect:
- Patient records
- Medical images
- Sensor readings
- Genomic data
- Doctor reports

Combining and analyzing this data can help predict disease outbreaks and personalize treatment.

### Analogy
A small notebook can manage one person's expenses. A notebook cannot manage the transactions of an entire country. At that scale, we need distributed storage, parallel processing, and automated analysis.

---

## 2. Big Data and Analytics Pipeline

```text
Input Data → Model → Analysis
```

A complete Big Data pipeline is:

```text
Data Sources
    ↓
Data Ingestion / Acquisition
    ↓
Data Storage
    ↓
Data Processing
    ↓
Data Analysis
    ↓
Data Consumption
```

### Stages

| Stage | Meaning |
|---|---|
| Data sources | Identify where data originates |
| Ingestion | Collect/import data into the system |
| Storage | Store data reliably and distributively |
| Processing | Clean, transform, aggregate, or compute on data |
| Analysis | Extract patterns and knowledge |
| Consumption | Present results through reports, dashboards, APIs, or applications |

---

## 3. Model

### Definition
A model is a human-created representation that helps us understand real-world systems or phenomena.

### Important points
- Represents inherent properties of data.
- Can be a map, schema, mathematical model, or data relationship.
- May represent records, tables, and associations.
- Does not depend on the software used to store the data.

### Example
A college database model may contain:

```text
Student(Student_ID, Name, Branch)
Course(Course_ID, Course_Name)
Enrollment(Student_ID, Course_ID)
```

### Analogy
A map is not the actual city. It is a simplified representation that helps us understand the city.

---

## 4. Traditional Approach vs Big Data Approach

### Traditional approach
- Understand the system first.
- Build a human-designed model.
- Use rules and domain experts.
- Apply the model to data.

### Big Data approach
- Collect huge amounts of examples.
- Discover patterns statistically.
- Build a machine-generated model.
- Use data to infer relationships.

### Example: Machine Translation

#### Traditional translation
A linguistic expert defines:
- Grammar rules
- Word meanings
- Sentence structure
- Word-to-word mappings
- Morphology

#### Big Data translation
- Collect millions of parallel English-Hindi sentences.
- Observe how words and phrases correspond.
- Build a statistical or learned model.
- Infer translations from patterns.

Example:

```text
English: cat
Hindi: billi
```

If this mapping repeatedly appears in the corpus, the system learns the relationship.

### Comparison

| Traditional Approach | Big Data Approach |
|---|---|
| Human-generated model | Machine-generated model |
| Rule-based | Data-driven |
| Requires domain experts | Requires large datasets |
| Hard to manually encode exceptions | Learns patterns from examples |
| Can fail on unexpected language | Can generalize from similar examples |

---

## 5. Domain Knowledge and the Unreasonable Effectiveness of Data

### Core idea
Large amounts of data can produce surprisingly effective models even without explicitly understanding every underlying rule.

However, domain knowledge is **not obsolete**.

### Example: “Let the cat out of the bag”
Literal translation:

```text
Release the cat from the bag
```

Actual meaning:

```text
Reveal a secret
```

A purely word-by-word model may fail without contextual/domain knowledge.

### Conclusion
Data is powerful, but domain knowledge helps:
- Validate results.
- Detect nonsensical conclusions.
- Interpret correlations.
- Handle exceptions.
- Understand context.

---

## 6. Correlation vs Causation

### Correlation
Two variables change together.

### Causation
One variable directly produces a change in another.

### Important rule

```text
Correlation does not imply causation.
```

### Example: Storks and birth rates
A study may show:
- More storks in a region.
- Higher human birth rates in that region.

This does **not** mean storks deliver babies.

A hidden variable may be responsible, such as:
- Larger rural areas
- More nesting space
- Larger families
- Different population structure

### General structure

```text
C → A
C → B
```

A common hidden variable `C` can cause both `A` and `B`.

---

## 7. Data Analysis Pitfalls

### 7.1 Selection Bias

#### Definition
Selection bias occurs when the sample used for analysis is not representative of the complete population.

#### Example
Tweets during Hurricane Sandy were used to study emergency impact. If most tweets came from Manhattan, the dataset may incorrectly suggest Manhattan was the most affected region.

#### Analogy
Asking only students sitting in the front row whether the entire class understands the lecture.

---

### 7.2 Missing Data Bias

Negative or unsuccessful results may be less likely to be published.

#### Example
If successful medical trials are published but failed trials remain hidden, the treatment may appear more effective than it actually is.

#### Result
The sample becomes biased and overestimates treatment benefits.

---

### 7.3 Out-of-Sample Prediction

A model should not confidently predict situations that are very different from the data on which it was trained.

### Rule

```text
Do not assume that patterns learned from known situations always work for unseen situations.
```

---

## 8. Error Estimation

### Definition
Error estimation evaluates how well a model performs on unseen data.

### Method
Split data into:

```text
Training Set → Build the model
Testing Set  → Measure performance
```

### Example
For 100,000 records:

```text
80,000 → Training
20,000 → Testing
```

### Logic
If the model is evaluated on the same data used to train it, the performance may look falsely high.

### Important terms
- Training error: Error on training data.
- Testing error: Error on unseen data.
- Generalization: Ability to perform well on new data.

---

## 9. Weather Forecasting as an Example

Weather is:
- Dynamic
- Nonlinear
- Chaotic
- Sensitive to initial conditions

Small changes in initial measurements can produce different predictions.

### Why forecasting improves
- Historical data
- Numerical models
- Human adjustments
- Probability estimation
- Continuous comparison with actual weather

### Probabilistic thinking
Instead of saying:

> It will definitely rain.

A better statement may be:

> There is a 70% probability of rain.

---

## 10. Data Types

### 10.1 Structured Data

#### Definition
Data organized according to a fixed schema, usually in rows and columns.

#### Examples
- Relational databases
- Spreadsheets
- SQL tables

```text
Student_ID | Name | Age
101        | Ravi | 20
102        | Anu  | 21
```

### 10.2 Semi-structured Data

#### Definition
Data without a rigid tabular structure but containing tags, keys, or markers that provide organization.

#### Examples
- JSON
- XML
- CSV
- Emails with headers

### 10.3 Unstructured Data

#### Definition
Data without a predefined schema or fixed organization.

#### Examples
- Images
- Videos
- Audio
- Social media posts
- Text documents
- Emails
- PDFs

### Comparison

| Type | Schema | Examples |
|---|---|---|
| Structured | Fixed | SQL table, spreadsheet |
| Semi-structured | Flexible | JSON, XML, CSV |
| Unstructured | No predefined schema | Video, audio, image, free text |

---

## 11. Typical Sources of Big Data

- Machine logs
- RFID readers
- Sensor networks
- GPS traces
- Retail transactions
- Social media
- Mobile devices
- IoT devices
- Web clicks
- Banking transactions
- Medical devices
- Smart grids

### Key idea
Machines increasingly generate more data than humans manually generate.

---

## 12. Processing Types

### 12.1 Batch Processing

#### Definition
Processing a collection of data together without continuous user interaction.

#### Example
Generating a daily bank transaction report.

#### Analogy
Washing a full basket of clothes together at night.

---

### 12.2 Interactive Processing

#### Definition
Continuous interaction between a user and a system.

#### Example
Online banking UI.

### Requirement
Low latency.

Technologies that help:
- MPP
- In-memory databases
- Columnar databases

---

### 12.3 Real-Time Processing

#### Definition
Processing live data with low latency as it is generated.

#### Example
Fraud detection during a card transaction.

---

### 12.4 Stream Analytics

#### Definition
Continuous computation over an incoming sequence of events.

#### Examples
- Twitter streams
- Server logs
- IoT sensor streams
- Stock market events

### Framework examples
- Storm
- S4
- Spark Streaming
- Flume

---

### 12.5 In-Memory Processing

#### Definition
Processing data in RAM or cache instead of repeatedly reading from disk.

### Why faster?
RAM access is much faster than disk access.

---

## 13. ETL

### Full form

```text
Extract → Transform → Load
```

### Definition
ETL is the process of retrieving data, integrating/cleaning it, transforming it into a useful form, and storing it in a target system.

### Example
1. Extract sales data from multiple stores.
2. Transform dates, currencies, and missing values.
3. Load into a data warehouse.

---

## 14. Types of Analytics

### 14.1 Descriptive Analytics

Answers:

> What happened?

Examples:
- Monthly sales report
- Dashboard
- Number of users last month

---

### 14.2 Predictive Analytics

Answers:

> What is likely to happen?

Examples:
- Predict disease outbreak
- Forecast sales
- Predict customer churn

---

### 14.3 Prescriptive Analytics

Answers:

> What should we do?

Examples:
- Best price to maximize profit
- Best delivery route
- Best allocation of resources

---

### 14.4 Cognitive Analytics

Uses learning, understanding, sentiment, and emotion analysis.

Examples:
- Sentiment analysis
- Emotion detection
- Language understanding

### Comparison

| Analytics | Main Question |
|---|---|
| Descriptive | What happened? |
| Predictive | What may happen? |
| Prescriptive | What should be done? |
| Cognitive | What does the data mean or express? |

---

## 15. Service-Oriented Architecture (SOA)

### Definition
SOA is an architectural style based on independent services that communicate through messages and interfaces.

### Components
- Services
- Messages
- Operations
- Processes
- Interfaces
- Service descriptions
- Constraints and policies

### Examples
- Web services
- Cloud services
- BigQuery services

### Analogy
A restaurant has separate services:
- Ordering
- Payment
- Cooking
- Delivery

Each service performs one responsibility and communicates with others.

---

# Characteristics of Big Data

## 16. The 4 Vs

The commonly discussed characteristics in the supplied material are:

1. Volume
2. Variety
3. Velocity
4. Veracity

---

## 17. Volume

### Definition
The enormous quantity of generated, stored, and processed data.

### Why volume is increasing
- Digital transactions
- IoT
- Sensors
- Mobile phones
- Social media
- Automated logging
- Online services

### Old data vs Big Data

| Traditional Data | Big Data |
|---|---|
| Generated mainly by enterprises | Generated by everyone and machines |
| Often manually entered | Mostly automatically generated |
| Smaller scale | Massive scale |
| Centralized storage | Distributed storage |

---

## 18. Variety

### Definition
The existence of many different formats, structures, and sources of data.

### Examples
- Text
- Numbers
- Images
- Audio
- Video
- Time series
- Social media
- GPS
- Multi-dimensional arrays
- Streaming data

### Why difficult?
All formats must often be connected to extract useful knowledge.

---

## 19. Velocity

### Definition
The speed at which data is generated, transmitted, and processed.

### Traditional system
```text
Night-time input → Daily report
```

### Big Data system
```text
Continuous input → Immediate response
```

### Examples
- Stock market feeds
- Sensor data
- Online transactions
- Social media streams

---

## 20. Veracity

### Definition
Veracity refers to the trustworthiness, quality, accuracy, and messiness of data.

### Sources of poor veracity
- Typos
- Hashtags
- Abbreviations
- Duplicate records
- Missing values
- Conflicting names
- Different addresses
- Inconsistent formats

### Example
The same person may have slightly different names or addresses in multiple identity records.

### Key point
Big Data can contain valuable information even when it is noisy.

---

# File Systems and Distributed Storage

## 21. File System

### Definition
A file system controls how data is stored, organized, named, accessed, and retrieved from storage devices.

### Common operations
- Create
- Delete
- Open
- Close
- Read
- Write

### File properties
- Persistence
- Sharing
- Structure
- Permissions
- Metadata

---

## 22. Metadata vs Data

### Data
Actual content of a file.

### Metadata
Information about the file.

Examples:
- Filename
- File size
- Permissions
- Location
- Block mapping

### Comparison

| Data | Metadata |
|---|---|
| Actual content | Information about content |
| Large | Relatively small |
| Frequently transferred/read | Accessed less frequently |
| Stored across data nodes | Often maintained by a master |

---

## 23. Local File System

A local file system stores data on one machine and manages disk blocks through the operating system.

Examples:
- NTFS
- FAT32
- ext4

### Limitation
A single machine has limited:
- Storage
- CPU
- Memory
- Network bandwidth
- Fault tolerance

---

## 24. Distributed File System

### Definition
A distributed file system stores files across multiple computers but presents them as one logical file system.

### Advantages
- Storage larger than one disk
- Parallel access
- Scalability
- Fault tolerance
- Distributed processing

### Analogy
Instead of keeping an entire library in one room, books are distributed across many rooms while users still search it as one library.

---

## 25. Scaling Up vs Scaling Out

### Scaling up
Increase capacity of one machine.

Example:
- More RAM
- Faster CPU
- Larger disk

### Scaling out
Add more machines to the cluster.

Example:
- Add 10 servers to store and process data.

### Big Data preference
Big Data systems generally prefer **scale out** using commodity hardware.

---

## 26. NFS Limitations

NFS was designed mainly for traditional network file access.

Limitations for Big Data:
- Small block sizes
- High metadata overhead
- Primarily scale-up oriented
- Not designed originally for massive distributed processing
- Limited support for large-scale parallel data processing

---

# Google File System (GFS)

## 27. GFS Definition

Google File System is a distributed file system designed for:
- Very large files
- Commodity hardware
- High throughput
- Frequent hardware failures
- Large sequential reads and writes

GFS inspired HDFS.

---

## 28. GFS Design Assumptions

- Files are very large.
- Hardware failures are normal.
- Most workloads are read-heavy.
- Files are commonly write-once and read-many-times.
- Streaming access is more important than low latency.
- Commodity hardware is preferred.
- Sequential operations dominate random access.

---

## 29. GFS Architecture

```text
             Client
                |
             Master
          /     |     \
    Chunkserver Chunkserver Chunkserver
```

### Components

#### Master
Maintains metadata:
- Namespace
- File-to-chunk mapping
- Chunk locations
- Access information

It does not normally serve the actual file data.

#### Chunkservers
- Store actual chunks.
- Serve read/write requests.
- Send heartbeat messages.
- Report chunk information.
- Perform replication and deletion.

#### Client
- Requests chunk locations from the master.
- Communicates directly with chunkservers for data transfer.

---

## 30. GFS Chunk Size

GFS traditionally uses a large chunk size, commonly **64 MB**.

### Why large chunks?
- Less metadata
- Fewer master interactions
- Better sequential throughput
- Reduced seeking and coordination

---

## 31. GFS Read Operation

1. Client asks Master for chunk locations.
2. Master returns chunk handle and locations.
3. Client contacts the nearest/appropriate chunkserver.
4. Data is read directly from chunkserver.

### Important
The master is not involved in transferring every byte of data.

---

## 32. GFS Fault Tolerance

### Replication
Chunks are replicated on multiple chunkservers.

### Heartbeats
Master periodically checks chunkserver health.

### Checksums
Chunkservers detect corrupted data using checksums.

### Re-replication
If a chunkserver fails, the master creates another valid replica.

### Snapshots
Efficient copies can be created for backup or replication.

---

## 33. GFS Scalability

To reduce master bottleneck:
- Clients cache metadata.
- Master handles metadata, not bulk data.
- Data transfer occurs directly between client and chunkserver.
- More chunkservers can be added.

---

## 34. GFS vs HDFS

| GFS | HDFS |
|---|---|
| Google proprietary/internal design | Apache open-source system |
| Chunkservers | DataNodes |
| Master | NameNode |
| Chunk | Block |
| Traditionally 64 MB chunks | Commonly 128 MB blocks |
| Inspired HDFS | Inspired by GFS |
| Designed for Google's workloads | Designed for Hadoop ecosystem |

---

# Hadoop Distributed File System (HDFS)

## 35. HDFS Definition

HDFS is a distributed file system designed for storing very large files with streaming access patterns on clusters of commodity hardware.

### Main goal
High throughput rather than very low latency.

---

## 36. Why HDFS?

A single machine cannot efficiently handle:
- Terabytes or petabytes of data
- Massive concurrent reads
- Hardware failures
- Large-scale analytics

HDFS distributes data across multiple machines.

---

## 37. HDFS Architecture

```text
                    Client
                      |
                   NameNode
              /       |       \
          DataNode  DataNode  DataNode
```

### NameNode
Master daemon responsible for:
- Namespace
- File-to-block mapping
- Permissions
- DataNode monitoring
- Block placement
- Replication decisions

### DataNode
Worker daemon responsible for:
- Storing blocks
- Serving read/write requests
- Replication
- Deletion
- Sending heartbeats
- Sending block reports

### Secondary NameNode
- Helper/checkpoint node
- Merges FsImage and Edit Logs
- Not a live backup
- Not a hot standby

---

## 38. FsImage and Edit Log

### FsImage
Stores the complete file system namespace at a checkpoint.

### Edit Log
Stores recent changes after the last FsImage checkpoint.

### Checkpoint process
Secondary NameNode:
1. Gets FsImage.
2. Gets Edit Log.
3. Merges them.
4. Produces updated checkpoint.
5. Sends it back to NameNode.

---

## 39. HDFS Blocks

HDFS divides files into large logical blocks.

Common block size:

```text
128 MB
```

A file smaller than one block does not occupy the full block size physically.

### Formula

```text
Number of blocks = ceil(File size / Block size)
```

### Example
File size = 500 MB  
Block size = 128 MB

```text
500 / 128 = 3.90625
ceil = 4 blocks
```

Block distribution:

```text
128 + 128 + 128 + 116 MB
```

---

## 40. Why Large Blocks?

- Reduces metadata overhead.
- Reduces disk seek impact.
- Improves sequential throughput.
- Makes distributed processing efficient.
- Reduces number of map tasks in some cases.

---

## 41. HDFS Replication

Default replication factor is commonly:

```text
3
```

If a block exists as 3 copies, losing one DataNode does not necessarily lose the data.

### Physical storage example
Logical file = 500 MB  
Replication factor = 3

Approximate physical storage:

```text
500 × 3 = 1500 MB
```

---

## 42. HDFS Read Flow

1. Client asks NameNode for block locations.
2. NameNode returns DataNode locations.
3. Client reads directly from the nearest DataNode.
4. Client reads blocks in sequence.
5. NameNode is not involved in bulk data transfer.

---

## 43. HDFS Write Flow

1. Client asks NameNode to create a file.
2. NameNode selects DataNodes.
3. Client sends data to DataNode 1.
4. DataNode 1 forwards to DataNode 2.
5. DataNode 2 forwards to DataNode 3.
6. Acknowledgements travel backward.
7. NameNode records block metadata.

```text
Client → DN1 → DN2 → DN3
Client ← DN1 ← DN2 ← DN3
```

---

## 44. Heartbeat and Block Report

### Heartbeat
DataNode periodically informs NameNode that it is alive.

### Block Report
DataNode sends the list of blocks it stores.

### If heartbeat stops
NameNode considers the DataNode failed and starts recovery/re-replication.

---

## 45. Rack Awareness

HDFS distributes replicas across different racks when possible.

### Why?
If an entire rack fails, replicas on other racks remain available.

### Goal
Balance:
- Fault tolerance
- Network bandwidth
- Storage locality

---

## 46. HDFS Fault Tolerance

### Replication
Multiple copies of blocks.

### Checksums
Detect corrupted blocks.

### Re-replication
Creates new copies when replicas are lost.

### Erasure Coding
Stores data and parity information with less storage overhead than 3x replication.

---

## 47. Erasure Coding

### Logic
A file is divided into data cells and parity cells.

If some cells are lost, the missing data can be reconstructed using the remaining cells.

### Similar to
RAID systems.

### Advantage
Lower storage overhead.

### Disadvantage
More computation is required during recovery.

---

## 48. HDFS High Availability

### Active–Standby architecture

```text
Active NameNode
       |
Shared Edit Logs
       |
Standby NameNode
```

### During failure
1. Active NameNode fails.
2. Standby uses shared edits and metadata.
3. Standby becomes active.
4. Clients continue after failover.

### ZooKeeper
Helps with:
- Failure detection
- Leader election
- Ensuring only one active NameNode

---

## 49. Where HDFS Does Not Perform Well

- Low-latency applications
- Large numbers of tiny files
- Multiple simultaneous writers
- Random modifications
- Arbitrary offset updates
- POSIX-style file operations
- Millisecond-level access requirements

### Why small files are problematic?
NameNode stores metadata in memory. Many small files create huge metadata overhead.

---

## 50. HDFS Important Commands

```bash
hdfs dfs -ls /
hdfs dfs -mkdir /mydir
hdfs dfs -put localfile /mydir/
hdfs dfs -get /mydir/remotefile .
hdfs dfs -rm /mydir/remotefile
hdfs dfsadmin -report
hdfs fsck / -files -blocks
```

---

# MapReduce

## 51. Why MapReduce?

HDFS stores data, but storage alone is not enough.

We need to process the data across many machines.

MapReduce provides:
- Parallel processing
- Data locality
- Fault tolerance
- Automatic task scheduling
- Distributed execution
- Simple programming abstraction

---

## 52. MapReduce Definition

MapReduce is a distributed programming model and execution framework for processing very large datasets using two major functions:

```text
Map → Reduce
```

It was inspired by Google's distributed computing model and implemented in Hadoop.

---

## 53. Core MapReduce Pipeline

```text
Input
  ↓
Input Splits
  ↓
Mapper
  ↓
Shuffle and Sort
  ↓
Reducer
  ↓
Output
```

Expanded:

```text
HDFS Input
   ↓
InputFormat
   ↓
RecordReader
   ↓
Mapper
   ↓
Combiner (optional)
   ↓
Partitioner
   ↓
Shuffle
   ↓
Sort
   ↓
Reducer
   ↓
HDFS Output
```

---

## 54. Map Function

### Mathematical form

```text
(Kin, Vin) → list(Kintermediate, Vintermediate)
```

### Meaning
Mapper receives one input key-value pair and emits zero or more intermediate key-value pairs.

---

## 55. Reduce Function

### Mathematical form

```text
(Kintermediate, list(Vintermediate))
    → list(Kout, Vout)
```

### Meaning
Reducer receives one key and all values belonging to that key.

---

## 56. Word Count Example

### Input

```text
Big Data Big
Data Hadoop
```

### Mapper output

```text
Big 1
Data 1
Big 1
Data 1
Hadoop 1
```

### Shuffle and sort

```text
Big → [1, 1]
Data → [1, 1]
Hadoop → [1]
```

### Reducer output

```text
Big 2
Data 2
Hadoop 1
```

---

## 57. MapReduce Logic

### Map
Break a large problem into smaller independent tasks.

### Shuffle
Group all identical keys together.

### Reduce
Combine values belonging to each key.

### Analogy
- Mapper: Each student counts words in one page.
- Shuffle: Put all counts for the same word into one pile.
- Reducer: Add each pile.

---

## 58. Input Split vs HDFS Block

### HDFS Block
Physical/logical storage unit in HDFS.

### Input Split
Logical unit of input processed by one mapper.

Often:

```text
One block ≈ One input split ≈ One mapper
```

But they are not conceptually identical.

### Important
Split size affects parallelism.

---

## 59. Number of Mappers

Usually:

```text
Number of mappers ≈ Number of input splits
```

### Example
File size = 230 MB  
Block/split size = 128 MB

```text
ceil(230 / 128) = 2 splits
```

Therefore:

```text
2 map tasks
```

---

## 60. Number of Reducers

Reducer count is configurable by the user.

It is not automatically equal to the number of blocks.

---

## 61. Partitioner

### Definition
The partitioner decides which reducer receives each intermediate key-value pair.

Default idea:

```text
hash(key) % number_of_reducers
```

### Critical rule

```text
Same key must always go to the same reducer.
```

Otherwise, counts for the same word would be split incorrectly.

---

## 62. Shuffle and Sort

### Shuffle
Transfers mapper output to reducers over the network.

### Sort
Sorts intermediate keys and groups equal keys.

### Why expensive?
- Network transfer
- Disk spills
- Sorting
- Merging
- Serialization/deserialization

---

## 63. Combiner

### Definition
A combiner is an optional mini-reducer executed near the mapper.

### Purpose
Reduce the amount of data sent across the network.

### Example

Without combiner:

```text
Apple 1
Apple 1
Apple 1
```

With combiner:

```text
Apple 3
```

### Important facts
- Optional.
- May run zero, one, or multiple times.
- Must be safe for repeated execution.
- Usually used for associative and commutative operations.
- Runs on mapper side.

---

## 64. Map Output vs Reduce Output

| Map Output | Reduce Output |
|---|---|
| Temporary | Final result |
| Stored on local disk | Stored in HDFS |
| Not replicated normally | Replicated for reliability |
| May be regenerated | Persistent |
| Used by reducers | Final user output |

---

## 65. Data Locality

### Traditional approach

```text
Move data to computation
```

### MapReduce approach

```text
Move computation to data
```

### Why?
Moving a small program is cheaper than moving terabytes of data over the network.

Levels:
1. Node locality
2. Rack locality
3. Off-rack locality

---

## 66. MapReduce Fault Tolerance

If a mapper or reducer fails:
- Framework detects failure.
- Task is restarted elsewhere.
- Lost intermediate output is regenerated.
- Failed machines do not necessarily fail the whole job.

### Speculative execution
If a task is unusually slow, Hadoop may run another copy of the same task and use the faster result.

---

## 67. Map-Only Jobs

Some tasks do not require aggregation.

### Example
Search for lines containing a pattern.

Mapper:

```text
If line matches pattern:
    emit line
```

Reducer:
- Not required.

---

## 68. Common MapReduce Problems

### Average rating
Mapper:

```text
Movie → Rating
```

Reducer:

```text
Movie → (sum of ratings / number of ratings)
```

### Unique word count
Mapper emits:

```text
word → 1
```

Reducer only emits the word once per grouped key.

---

## 69. Split Calculation Example

Given:
- File size = 2 GB = 2048 MB
- Split size = 128 MB
- 4 nodes

```text
Number of splits = 2048 / 128 = 16
Number of mappers = 16
Approximate splits per node = 16 / 4 = 4
Number of reducers = user-configured
```

---

# Hadoop Job Management and YARN

## 70. Why YARN?

MapReduce needs:
- Resource allocation
- Task scheduling
- Monitoring
- Failure handling
- Cluster management

---

## 71. Hadoop 1 Architecture

### JobTracker
Responsible for:
- Resource management
- Scheduling
- Monitoring
- Failure handling

### TaskTracker
Runs tasks on worker machines.

### Problems
- JobTracker becomes a bottleneck.
- Single point of failure.
- Fixed map/reduce slots.
- Poor resource utilization.
- Only MapReduce applications supported.
- Limited scalability.

---

## 72. YARN

### Full form
Yet Another Resource Negotiator.

### Definition
YARN is Hadoop's general-purpose cluster resource management and job scheduling layer.

It separates:
- Resource management
- Application management

---

## 73. YARN Components

### 73.1 ResourceManager (RM)
Cluster-wide master.

Responsibilities:
- Allocates resources.
- Schedules applications.
- Arbitrates resources among applications.

---

### 73.2 NodeManager (NM)
Runs on each worker node.

Responsibilities:
- Monitors resource usage.
- Manages containers.
- Launches and monitors tasks.

---

### 73.3 ApplicationMaster (AM)
One per application.

Responsibilities:
- Manages application lifecycle.
- Requests containers.
- Tracks task progress.
- Handles application-level failures.

---

### 73.4 Container
A bundle of allocated resources.

May include:
- CPU
- Memory
- Environment
- Launch command

---

## 74. YARN Job Flow

```text
Client submits application
          ↓
ResourceManager
          ↓
ApplicationMaster starts
          ↓
AM requests containers
          ↓
NodeManagers launch containers
          ↓
Tasks execute
          ↓
AM monitors progress
          ↓
Application completes
```

---

## 75. Hadoop 1 vs YARN

| Hadoop 1 | Hadoop with YARN |
|---|---|
| JobTracker handles everything | Responsibilities separated |
| TaskTracker executes tasks | NodeManager manages containers |
| Mainly MapReduce | Multiple frameworks |
| Single central bottleneck | Better scalability |
| Fixed slots | Dynamic resource allocation |
| Poor utilization | Better utilization |
| Less flexible | General-purpose platform |

---

# Scheduling

## 76. Fair Scheduler

### Definition
Fair scheduling attempts to give running applications a fair share of cluster resources over time.

### Features
- Pools for users/applications
- Fair allocation
- Dynamic sharing
- Prevents one job from monopolizing resources
- May support preemption

### Analogy
If several people share a gaming PC, each person gets a fair amount of time rather than one person using it forever.

---

## 77. Capacity Scheduler

### Definition
Capacity Scheduler divides cluster resources into queues with guaranteed capacity.

### Features
- Queue-based
- Hierarchical queues
- Guaranteed minimum capacity
- FIFO within queues, subject to priorities
- Unused capacity may be temporarily borrowed
- Useful for organizations/departments

### Analogy
A college allocates guaranteed lab capacity to CSE, ECE, and ME departments. If one department is idle, another may temporarily use spare capacity.

---

## 78. Fair Scheduler vs Capacity Scheduler

| Fair Scheduler | Capacity Scheduler |
|---|---|
| Focuses on fairness over time | Focuses on guaranteed queue capacity |
| Pools/users | Queues/departments |
| Dynamic fair sharing | Capacity guarantees |
| Prevents starvation | Supports organizational allocation |
| Suitable for shared clusters | Suitable for multi-tenant organizations |

---

# Quick Revision Tables

## 79. Important Comparisons

### Batch vs Stream

| Batch | Stream |
|---|---|
| Processes stored data | Processes continuously arriving data |
| Higher latency | Low latency |
| Periodic execution | Continuous execution |
| Daily reports | Fraud detection/live monitoring |

### Scale Up vs Scale Out

| Scale Up | Scale Out |
|---|---|
| Bigger machine | More machines |
| Expensive at high scale | Uses commodity hardware |
| Limited maximum capacity | Highly scalable |
| Vertical scaling | Horizontal scaling |

### NameNode vs DataNode

| NameNode | DataNode |
|---|---|
| Master | Worker |
| Stores metadata | Stores actual blocks |
| Tracks block locations | Serves block data |
| Monitors DataNodes | Sends heartbeat/block reports |

### GFS Master vs HDFS NameNode

Both maintain metadata and coordinate storage nodes.

### GFS Chunkserver vs HDFS DataNode

Both store actual distributed data chunks/blocks.

---

# Exam-Oriented One-Line Definitions

- **Big Data:** Data whose scale, diversity, and complexity require new processing architectures and techniques.
- **Model:** A representation used to understand a real-world system.
- **Structured data:** Data with a fixed predefined schema.
- **Semi-structured data:** Data with flexible organization using tags or keys.
- **Unstructured data:** Data without a predefined schema.
- **ETL:** Extract, Transform, Load.
- **Batch processing:** Processing data in groups without continuous interaction.
- **Stream analytics:** Continuous processing of incoming events.
- **Veracity:** Trustworthiness and quality of data.
- **Distributed file system:** A file system that stores data across multiple machines as one logical system.
- **GFS:** Google's distributed file system designed for huge files and commodity hardware.
- **HDFS:** Hadoop's distributed file system optimized for high-throughput access.
- **NameNode:** HDFS master that manages metadata.
- **DataNode:** HDFS worker that stores blocks.
- **FsImage:** Checkpointed file system namespace.
- **Edit Log:** Recent namespace changes.
- **MapReduce:** Distributed programming model using Map and Reduce functions.
- **Mapper:** Converts input key-value pairs into intermediate pairs.
- **Reducer:** Aggregates values belonging to the same key.
- **Partitioner:** Sends each intermediate key to a reducer.
- **Combiner:** Optional mini-reducer that reduces mapper output locally.
- **YARN:** Hadoop's resource management and scheduling layer.
- **ResourceManager:** Cluster-wide resource allocator.
- **NodeManager:** Per-node container manager.
- **ApplicationMaster:** Manages one application's lifecycle.
- **Container:** Allocated CPU/memory resource bundle.

---

# Memory Tricks

## Big Data 4 Vs

```text
Volume = How much?
Variety = What types?
Velocity = How fast?
Veracity = How trustworthy?
```

## HDFS

```text
NameNode = Names/Metadata
DataNode = Data
Secondary NameNode = Checkpoint helper
```

## MapReduce

```text
Map = Produce
Shuffle = Group
Reduce = Combine
```

## YARN

```text
RM = Resources
NM = Node machine
AM = Application manager
Container = Allocated resources
```

## Analytics

```text
Descriptive = Past
Predictive = Future
Prescriptive = Action
Cognitive = Understanding
```

---

# Final Conceptual Flow

```text
Big Data
   ↓
Need distributed storage
   ↓
GFS concepts
   ↓
HDFS stores huge files across DataNodes
   ↓
Need distributed processing
   ↓
MapReduce processes data in parallel
   ↓
Need resource management
   ↓
YARN allocates cluster resources
   ↓
Schedulers divide resources fairly or by capacity
```

# Big Data – Unit 2 Complete Study Notes

> Structured for revision and understanding. Built from the Unit 2 slide deck, in the same format as the Unit 1 notes.  
> Every major topic follows: **Definition → Logic → Example → Analogy → Key points → Comparisons**.  
> Exam section (MCQs + 2-markers + 4-markers + numericals) is at the very end.

---

## 0. Unit 2 Roadmap

Unit 1 gave us **HDFS (storage)** + **MapReduce (compute)** + **YARN (resources)**.  
Unit 2 answers: *"How do we actually build algorithms, store different kinds of data, ingest it, schedule it and manage it around Hadoop?"*

### Where each part of the slide deck lives in these notes

| Slide-deck topic | Section here |
|---|---|
| Relational operations with MapReduce (select, project, union, intersection, difference, join, group-by) | 2 |
| Algorithm complexity, communication cost, 3-way join, similarity join | 3 |
| Storage choices: HDFS vs Hive vs HBase/Cassandra, columnar storage | 4 |
| Hive | 5 |
| HBase | 6 |
| Cassandra | 7 |
| Pig / Pig Latin | 8 |
| Data ingestion: Sqoop, Flume, Apache SeaTunnel, CDC | 9 |
| Workflow orchestration: DAG, Oozie, Airflow | 10 |
| Ambari (cluster management) | 11 |
| Data Warehouse → Data Lake → Lakehouse, Apache Iceberg, Apache Hudi | 12 |
| Apache Druid | 13 |
| Quick revision, one-liners, memory tricks | 14 |
| Exam practice: MCQs, 2-markers, 4-markers, numericals | 15 |

---

## 1. The Hadoop Ecosystem – Big Picture

### Definition
The Hadoop ecosystem is the set of tools built around HDFS and MapReduce, each solving one specific problem so that we do not have to hand-write low-level MapReduce for everything.

### The toolkit (from the slides)

| Tool | Purpose | One-line role |
|---|---|---|
| **Pig** | Simplify analysis | Scripting language (Pig Latin) that compiles to MapReduce |
| **Hive** | Enable SQL | Run SQL-like queries (HiveQL) on data in Hadoop |
| **Sqoop** | Move data | Transfer between relational DBs and Hadoop (now legacy) |
| **Flume** | Ingest logs | Collect streaming log/event data into Hadoop (now legacy) |
| **Oozie** | Automate jobs | Schedule and manage dependent workflows (legacy) |
| **HBase** | Real-time access | NoSQL columnar DB for fast read/write on massive data |

### Modern additions covered in this unit

| Tool | Purpose |
|---|---|
| **Apache SeaTunnel** | Unified data integration; replaces Sqoop and Flume |
| **Apache Airflow** | Python-based workflow orchestration; replaces Oozie |
| **Apache Ambari** | Install, manage, monitor Hadoop clusters |
| **Cassandra** | Peer-to-peer columnar NoSQL DB |
| **Apache Iceberg / Hudi** | Open table formats that turn a data lake into a lakehouse |
| **Apache Druid** | Real-time OLAP analytics database |

### Big-picture flow

```text
Data sources (RDBMS, logs, events, DB changes)
        ↓
Ingestion      : Sqoop | Flume | SeaTunnel | CDC
        ↓
Storage        : HDFS | Hive tables | HBase / Cassandra | Data lake → Lakehouse (Iceberg/Hudi)
        ↓
Processing     : MapReduce | Pig | Hive | (Spark/Flink engines)
        ↓
Orchestration  : Oozie | Airflow
Cluster mgmt   : YARN (resources) + Ambari (install / monitor)
        ↓
Serving        : Hive | Druid | dashboards
```

### Analogy
A city has roads (HDFS), factories (MapReduce), delivery trucks (Sqoop/Flume/SeaTunnel), a traffic controller (Oozie/Airflow), a control room (Ambari), and translators so that ordinary people can use the factory (Pig/Hive). No single tool does everything.

---

# Part A – Algorithms on MapReduce

## 2. Relational Operations with MapReduce

### Why this topic exists
Business data usually lives in tables and we query it with SQL / relational algebra. In Big Data, the "table" may be a CSV file in HDFS. We must express each relational operator as **map + reduce**.

### 2.1 Relational algebra – quick recap

A **relation** is a table.
- A **row** is a **tuple** (one data item).
- A **column** is an **attribute**.
- A relation is written R(A1, A2, ..., An).

| Operator | Symbol | Meaning |
|---|---|---|
| Selection | σC(R) | Pick rows of R that satisfy condition C |
| Projection | πS(R) | Pick a subset S of columns of R |
| Union | R ∪ S | Rows in R or S |
| Intersection | R ∩ S | Rows in both R and S |
| Difference | R − S | Rows in R but not in S |
| Natural join | R ⋈ S | Combine rows of R and S that match on common attributes |
| Grouping | γ | Partition R by attribute(s) G |
| Aggregation | SUM, COUNT, AVG, MAX, MIN | Compute a value per group |

Other fundamental operators in the slides: Cartesian product, Rename.

Relational algebra is a **procedural query language**: it takes one or two relations and produces a new relation.

### 2.2 The running example (slide problem)

Data stored as a CSV in HDFS:

```text
1, Virat Kohli,      Captain, RCB
2, Gautham Gambhir,  Captain, KKR
3, Anil Kumble,      Coach,   MI
4, Virender Sehwag,  Coach,   KXIP
```

Queries:
- (a) All details of Coaches → `SELECT * FROM T WHERE Role='Coach'` → **selection**
- (b) Only names of Coaches → `SELECT Name FROM T WHERE Role='Coach'` → **selection + projection**
- (c) Total number of Coaches → `SELECT COUNT(*) FROM T WHERE Role='Coach'` → **selection + aggregation**

---

### 2.3 Selection σC(R)

#### Algorithm
- **Map:** read each row t. If t satisfies condition C, output **(t, t)**.
- **Reduce:** do nothing (identity). *(A map-only job would also do.)*

#### Trace for query (a)

```text
Map output:
<"3, Anil Kumble, Coach, MI",       "3, Anil Kumble, Coach, MI">
<"4, Virender Sehwag, Coach, KXIP", "4, Virender Sehwag, Coach, KXIP">

Reduce output:
3, Anil Kumble, Coach, MI
4, Virender Sehwag, Coach, KXIP
```

#### Analogy
A security guard at a gate checking every person's ID card and letting only those with the correct badge through. No one needs to talk to another person, so each guard (mapper) works independently.

---

### 2.4 Projection πS(R)

#### Algorithm
- **Map:** read each row t, compute t' = the subset of attributes in S, output **(t', t')**.
- **Reduce:** **eliminate duplicates**: (t', [t', t', t']) → (t', t').

#### Why a reducer is needed
Projection can create duplicate rows (two employees in the same department, projecting only Department). Relational projection is a set operation, so duplicates must be removed. The shuffle brings identical t' together at one reducer, which emits it once.

#### Trace for query (b)

```text
Map output:    <"Anil Kumble","Anil Kumble">   <"Virender Sehwag","Virender Sehwag">
Reduce output: Anil Kumble
               Virender Sehwag
```

#### Analogy
Photocopying only the "Name" column from every page of a register, then removing repeated names from the pile.

---

### 2.5 The "two input files" problem (needed for Union, Intersection, Difference, Join)

#### Problem
Set operations and joins need to read **two different files/tables** and produce one output. A mapper normally reads one input.

#### Why not just combine the files first?
- Files can be combined in many ways (append one after the other, or merge-sort).
- The files may not have the same structure.

#### The Hadoop solution
Use **MapReduce itself** to merge them:
- Two mappers, **each reads one file**.
- Each mapper writes key-value pairs.
- The reducer uses the **keys** to merge.
- The output can feed later MapReduce passes.

#### Hadoop API

```java
MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, Mapper1.class);
MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, Mapper2.class);
```

The same problem appears in **matrix–vector multiplication** (need both matrix M and vector v).

---

### 2.6 Union R ∪ S

R and S must have the **same schema**. Output every record that is in R or S (once).

#### Algorithm
- **Mapper 1:** read each row t of R, output **(t, t)**.
- **Mapper 2:** read each row t of S, output **(t, t)**.
- **Reducer:** eliminate duplicates: (t, [t, t]) → (t, t).

#### Worked example (from the slide)

```text
File 1 (R): A, B, C, D
File 2 (S): A, E, F, C
```

```text
Mapper 1 output : (A,A) (B,B) (C,C) (D,D)
Mapper 2 output : (A,A) (E,E) (F,F) (C,C)

Reducer input   : A→[A,A]   B→[B]   C→[C,C]   D→[D]   E→[E]   F→[F]
Reducer output  : A  B  C  D  E  F
```

`A` and `C` arrive twice (once from each mapper). The reducer emits each key once.

#### Analogy
Merging the attendance sheets of two classes into one list of "everyone who attended at least one", writing each name only once.

---

### 2.7 Intersection R ∩ S

#### Algorithm
- **Mapper 1:** each row of R → **(t, t)**.
- **Mapper 2:** each row of S → **(t, t)**.
- **Reducer:** output t **only if it appears from both** mappers, i.e. the value list has two entries: (t, [t, t]) → (t, t).

#### Same example

```text
Reducer input : A→[A,A]  B→[B]  C→[C,C]  D→[D]  E→[E]  F→[F]
Output R ∩ S  : A  C
```

#### Note
This simple version assumes there are no duplicate rows *within* a single relation (relations are sets). Otherwise a duplicate inside R alone would look like "appears in both".

#### Analogy
People whose names appear on **both** the cricket team list and the football team list.

---

### 2.8 Difference R − S

"All rows in R that are not in S."

#### Algorithm
Here the reducer must know **which table** a row came from, so mappers add a tag.
- **Mapper 1:** each row of R → **(t, "R")**.
- **Mapper 2:** each row of S → **(t, "S")**.
- **Reducer:** output t **only if its value list is exactly [R]**, i.e. it never appeared in S.

#### Same example (R = A,B,C,D ; S = A,E,F,C)

```text
Reducer input : A→[R,S]  B→[R]  C→[R,S]  D→[R]  E→[S]  F→[S]
Output R − S  : B  D
```

#### Why the tag is needed
With (t,t) alone we cannot tell whether a duplicate came from R or S, and R − S is **not symmetric** (R − S ≠ S − R).

#### Analogy
Students enrolled in the college (R) minus those who already paid the fee (S) = students who still owe fees.

---

### 2.9 Natural Join R(A,B) ⋈ S(B,C)

Combine rows of R and S that have the **same value of the common attribute B**. A and C are the other attributes.

#### Algorithm
- **Mapper 1:** for each (a, b) in R, output **(b, (R, a))**.
- **Mapper 2:** for each (b, c) in S, output **(b, (S, c))**.
- **Reducer:** for a key b, take every pair ((R, a), (S, c)) and output **(a, b, c)**.

The join attribute becomes the **key**, so all rows sharing that value meet at one reducer. The tag (R or S) tells the reducer where each value came from.

#### Worked example (slide class exercise)

```text
Employee E(Name, Age)         Dept D(Name, Dept)
Gabbar  35                    Gabbar  Bandit
Viru    37                    Viru    Hero
Jai     33                    Jai     Hero
Baldev  44                    Baldev  Police
Basanti 31                    Basanti Heroine
```

```text
Mapper 1 output:  Gabbar,(E,35)  Viru,(E,37)  Jai,(E,33)  Baldev,(E,44)  Basanti,(E,31)
Mapper 2 output:  Gabbar,(D,Bandit)  Viru,(D,Hero)  Jai,(D,Hero)  Baldev,(D,Police)  Basanti,(D,Heroine)

Reducer input:    Gabbar  → (E,35),(D,Bandit)
                  Viru    → (E,37),(D,Hero)
                  Jai     → (E,33),(D,Hero)
                  Baldev  → (E,44),(D,Police)
                  Basanti → (E,31),(D,Heroine)

Reducer output:   Gabbar,35,Bandit
                  Viru,37,Hero
                  Jai,33,Hero
                  Baldev,44,Police
                  Basanti,31,Heroine
```

#### Analogy
A wedding planner has a **guest list** (name, table) and a **meal-preference list** (name, meal). Two clerks each read one list and write the guest's name on the top of a slip. All slips with the same name are put in one tray (shuffle). A third person (reducer) staples the two slips in each tray into a single record.

#### Key points
- Join key = shuffle key.
- The reducer performs the actual "pairing".
- Cost of one join ≈ **O(r + s)** (see Section 3).

---

### 2.10 Grouping and Aggregation

For relation R(A, B, C): **group by A**, aggregate with function f(B).

Example: `Friends(User, Friend, Date)`; group by User and COUNT(Friend) → a table of (User, number of friends).

#### Algorithm
- **Map:** for each line (a, b, c), output **(a, b)**.
- **Reduce:** take (a, [b1, b2, b3, ...]) and output **(a, f(b1, b2, b3, ...))**.

The **grouping attribute is the key**; the **aggregate function runs in the reducer**.

#### Worked example (slide problem): number of employees per department

```text
Dept D(Name, Dept):  Gabbar Bandit | Viru Hero | Jai Hero | Baldev Police | Basanti Heroine
```

```text
Mapper output   : (Bandit,1) (Hero,1) (Hero,1) (Police,1) (Heroine,1)
Reducer input   : Bandit→[1]  Hero→[1,1]  Police→[1]  Heroine→[1]
Reducer output  : Bandit 1 | Hero 2 | Police 1 | Heroine 1
```

*(This is exactly word count with department names as the words.)*

#### Analogy
Vote counting: booth workers (mappers) write "Party X: 1" for each ballot, all slips of the same party are moved to one table (shuffle), and a counter (reducer) sums them.

### 2.11 Summary table – relational operators in MapReduce

| Operator | Mapper output | Reducer action |
|---|---|---|
| Selection σC(R) | (t, t) if t satisfies C | Nothing (identity) |
| Projection πS(R) | (t', t') with t' = chosen attributes | Remove duplicates |
| Union R ∪ S | (t, t) from both files | Remove duplicates, emit t once |
| Intersection R ∩ S | (t, t) from both files | Emit t only if it came from both |
| Difference R − S | (t, "R") / (t, "S") | Emit t only if list = [R] |
| Natural join R ⋈ S | (b, (R, a)) / (b, (S, c)) | Pair R-values with S-values → (a, b, c) |
| Group + aggregate | (a, b) | (a, f(list of b)) |

### 2.12 Exam traps for this section
- Selection has **no real reduce work**; projection's reducer is only for **duplicate elimination**.
- Difference needs **tags**; union and intersection do not.
- Intersection outputs keys that appear **twice** (from both mappers); union outputs **all** keys once.
- For a join, the **key is the common attribute**, not the whole tuple.
- Multiple input files are handled by **`MultipleInputs`** with one mapper class per file.

---

## 3. Algorithm Complexity for MapReduce

### 3.1 Why study complexity?

There are usually **many MapReduce algorithms** for the same problem (matrix multiplication, multi-way joins). We need a way to choose. We actually study **workflow systems**, which generalize MapReduce (many big-data systems are workflow systems).

### 3.2 Single-node view (class exercise)

| Problem | What complexity depends on |
|---|---|
| Matrix multiplication | Total number of **computations** |
| Database query | **Disk reads** |

In a distributed system this changes: the bottleneck is **moving data**.

### 3.3 Communication Cost Complexity Model

#### Definition
**Communication cost = the size of the input to a task** (summed over all tasks of the algorithm).

#### Why communication cost?
- Most algorithms are **linear in the data**, so computation is cheap.
- **Network speed ≪ CPU speed** and **disk speed ≪ CPU speed**.
- So a large share of time is spent **communicating**, not computing.

#### Why count only *input* size?
- The **output of one task is the input of another**, so it is already counted there.
- The **final output** is usually small (after aggregation); otherwise it is not human-readable.

#### Analogy
Shifting house: the cost is dominated by carrying boxes between the old and new home (communication), not by unpacking each box (computation).

### 3.4 Natural join R ⋈ S

Let r = size of R, s = size of S.

```text
Mapper input   = r + s      (reading from disk)
Reducer input  = r + s      (network transfer)
Total          = O(r + s)
```

### 3.5 3-way join R ⋈ S ⋈ T

Needs **two MapReduce phases**. Let r, s, t be the sizes.

#### Case 1: join R,S first, then T
- p = probability that a row of R matches a row of S.
- Intermediate result size = **p·r·s**.

```text
Phase 1 : mappers read r and s  → reducer input r + s
Phase 2 : mappers read p·r·s (intermediate) and t → reducer input t + p·r·s
Total   : O(r + s + t + p·r·s)
```

#### Case 2: join S,T first, then R
- q = probability of match between S and T; intermediate size = **q·s·t**.

```text
Total   : O(r + s + t + q·s·t)
```

#### Key insight
**Cost depends on the join order.** If p ≈ q, first perform the join whose intermediate result is smallest (the smallest of r·s, s·t, r·t).

#### Small numeric example
r = 1000, s = 2000, t = 500, p = q = 0.01
- Join R,S first: 1000 + 2000 + 500 + 0.01·1000·2000 = 3500 + 20,000 = **23,500**
- Join S,T first: 3500 + 0.01·2000·500 = 3500 + 10,000 = **13,500**  → better (smaller intermediate).

### 3.6 Key performance parameters

#### Wall-clock time
- **Definition:** time taken for the **entire job to finish**.
- You can make communication cost near zero by running everything on **one CPU**, but then the job runs very slowly.
- Splitting the work raises communication but reduces wall-clock time.
- So we must **trade off communication cost against wall-clock time**.

#### Reducer size q
- **Definition:** the **maximum number of values that can be associated with one key** (i.e. the max input a single reducer call handles).
- **It is NOT the number of reducers.**
- If the total number of map outputs is T, then **max number of reducers = T / q**.
- **Small q → more reducers → more parallelism → lower wall-clock time, but higher communication.**

#### Replication rate r
- **Definition:** (number of key-value pairs output by mappers) / (number of input records to mappers).
- It is the **average communication cost per input** from map tasks to reduce tasks.

```text
Small reducer size q  ⇒  many reducers  ⇒  high parallelism (low wall-clock)
                       ⇒  but each input is copied to many reducers ⇒ high replication rate ⇒ high communication
```

### 3.6c Comparison

| Parameter | Meaning | Effect |
|---|---|---|
| Communication cost | Total input size to all tasks | Lower is better |
| Wall-clock time | Total elapsed time of job | Lower is better |
| Reducer size q | Max values per key | Small q → more reducers |
| Replication rate | Map output pairs per input record | High rate → high communication |
| Parallelism | Number of tasks running at once | More parallelism → lower wall-clock |

---

### 3.7 Similarity Join (worked case study)

#### Problem
- Database of **n = 1,000,000 images**, each **1 MB** → total **1 TB**.
- A function s(x, y) (symmetric: s(x,y) = s(y,x)) tells how similar two images are.
- **Output all pairs x, y with s(x, y) > threshold t.**

We must compare **every pair** of images.

#### 3.7.1 Naive algorithm

```text
Mapper : reads (i, Pi) and generates ({i,j}, {Pi, Pj}) for every other image j
Reducer: reads ({i,j}, {Pi, Pj}) and computes s(Pi, Pj)
```

Every pair gets its **own reducer**.

| Quantity | Value |
|---|---|
| Replication rate | **999,999** (each image is sent once for each of the other images) |
| Key-value pairs | 1,000,000 × 999,999 ≈ **10¹²** |
| Data communicated | 10¹² pairs × 1 MB ≈ **10¹⁸ bytes = 1 exabyte** |
| Time over gigabit Ethernet | ≈ 10¹⁰ seconds ≈ **300 years** |
| Reducer size q | **Very small**: only 2 values (2 MB) per reducer |
| Parallelism | **Very high**: about n(n−1)/2 ≈ 5×10¹¹ (order of 10¹²) reducers possible |
| Communication cost | ~ n² → **impractical** |

**Verdict:** best parallelism, but **impossible communication cost**. The algorithm "doesn't work".

#### 3.7.2 The other extreme: do everything on one node
- Mapper and reducer on the same node.
- **No communication cost**, but **very low parallelism** (very high wall-clock time).

```text
Naive (one pair per reducer)          One node
  high communication  (bad)             low communication  (good)
  high parallelism    (good)            low parallelism    (bad)
                  ⇒  we want something in between
```

#### 3.7.3 Group-based algorithm (the middle path)

**Idea:** divide images into **g groups**. A reducer receives **two groups**, keeps them in memory, and compares **all pairs** inside those two groups.

```text
n   = total images
g   = number of groups
m   = n / g   images per group
```

- Each group must be compared with the other g−1 groups → **each group is sent to g−1 reducers**.
- Total messages = g(g−1).
- **Total data (communication) = m·g(g−1) = n(g−1) ≈ n·g** images.
- **Number of reducers (parallelism) = g(g−1)/2 = C(g,2) = O(g²)**.
- **Memory per reducer** = 2 groups = 2m images.

**Mapper:** input (i, Pi); find group u of image i; output **g−1 pairs ({u,v}, (i, Pi))** for every v ≠ u.  
**Reducer:** one reducer per unique key {u,v}; store the images of groups u and v; compare all pairs of images; *(the slide's trick: if v = u+1, also compare all pairs inside group u so that within-group pairs get covered)*.

Example with 100 groups G0…G99: G0 is sent to reducers of 99 pairs (G0–G1, …, G0–G99); the same for every other group.

#### 3.7.4 Numbers (n = 10⁶ images)

| Groups g | Images per group m | Communication ≈ n·g (images) | Parallelism ≈ g(g−1)/2 | Memory per reducer |
|---|---|---|---|---|
| **100** (slide 137) | 10,000 | 10⁶ × 100 = **10⁸** | ≈ 4,950 ≈ **10⁴** | ~20 GB |
| **1000** | 1000 | 10⁶ × 1000 = **10⁹** | ≈ 499,500 ≈ **5×10⁵** (slide rounds to 10⁶) | ~2 GB (2000 images) |

- Group of 1000: ~**500,000 reducers**; on a **10,000-node cluster** → **about 50 passes**, **speedup ≈ 10,000**.

> **Note on the slide wording:** "groups of 100" on slide 137 uses **100 as the number of groups (g = 100)**. If a question says "group *size* = 100", then g = n/m = 10⁶/100 = 10⁴ groups, each group is sent to 9,999 reducers, communication ≈ n·g = 10¹⁰ images and parallelism ≈ g²/2 ≈ 5×10⁷. **State your assumption in the exam.**

#### 3.7.5 Trade-off (the exam point)
As the **number of groups g increases** (smaller groups):
- Communication cost **increases** (n·g).
- Parallelism **increases** and wall-clock time **decreases** (O(g²)).
- Reducer memory **decreases** (2n/g).

As **g decreases** (larger groups) the reverse happens.

> The slide also notes: from a communication-cost view it does not matter whether the mapper sends the images to the reducer or the reducer reads them from disk (both are the same), but reading from disk is probably faster in practice.

#### Analogy
A stadium of one million people where **everyone must shake hands with everyone**.
- Naive: every pair meets in its own private room, and each person's photo is photocopied for every partner (huge copying).
- One node: one person does all the handshakes alone (no copying, but forever).
- Group-based: split people into blocks; **each pair of blocks meets in one room** and all handshakes inside that room happen there.

### 3.8 Summary
- Many MapReduce algorithms can solve one problem.
- Choose using **communication cost** (data volume into each phase) and **wall-clock time** (depends on parallelism).
- Usually there is a **trade-off**, tuned by parameters like **group size**.

### 3.9 Exam traps for this section
- **Reducer size ≠ number of reducers.**
- Communication cost counts **input** of tasks, not output.
- Join order changes the cost of a 3-way join.
- Naive similarity join: **high** parallelism, **very high** communication. Single node: **zero** communication, **very low** parallelism.
- Group-based: communication ~ **n·g**, parallelism ~ **g²/2**.

---

# Part B – Storage Choices and NoSQL

## 4. Choosing the Right Storage: HDFS vs Hive vs HBase/Cassandra

### Logic
We already know HDFS (Unit 1). It is excellent for **batch scans of big files**, but it is not the best fit for every access pattern. Different tools sit on top of (or beside) HDFS to cover the gaps.

### 4.1 Limitations of HDFS
HDFS is good for batch processing (scans over big files) but **not good for**:
- Record lookup (finding one record fast)
- Incremental addition of small batches
- Updates

### 4.2 Limitations of Hive
- **Doesn't store data**: uses HDFS (or HBase) as the actual store.
- Provides an **SQL interface** for querying.
- Data must be **structured** (definite schema).
- Not good for record lookup, small incremental additions, updates, or unstructured / semi-structured data.

### 4.3 Which store for which data?

| Store | Best for | Access pattern |
|---|---|---|
| **HDFS** | Unstructured data | Writes: no updates, only appends. Read the **entire file** and analyse |
| **Hive** | Structured data | Analytics via **SQL** |
| **HBase / Cassandra** (columnar) | Unstructured / semi-structured data | **Arbitrary writes**, random reads, analytics |

### 4.4 Motivational exercise (from the slides)

| Data | Store | Why |
|---|---|---|
| Parsed transaction logs of user activity (relevant fields extracted) | **Hive** | Structured; analysed with SQL |
| Unparsed (raw) transaction logs | **HDFS** | Raw, unstructured; just append and scan |
| Database of users and friends at a social site, periodically analysed | **HBase** | Needs record-level access/updates on a large sparse dataset |

### 4.5 Columnar storage

#### Row storage vs column storage
- **Row storage:** the database is stored as a single file, **one row per line**.
- **Column storage:** **each column is a separate file**, one value per line.

For any operation we must do I/O to load data from disk, so the storage layout decides how much I/O we do.

| Task | Better layout | Why |
|---|---|---|
| Analyse relationship between **age and earnings** (touches 2 columns, all rows) | **Column storage** | Reads only those two column files |
| **Add a new row** or **read one row** | **Row storage** | One row is contiguous; a single I/O |

#### History
- Early databases were built for **transactions** (read bank balance, update balance) → **row storage** is more efficient.
- **Column DBs** became popular with **Big Data**: more efficient for **analytics**, especially when the DB is large, and used to handle **unstructured data**.

#### Analogy
Row storage = a stack of **index cards, one per person** (great to pull one person's card). Column storage = **one register per attribute** (one register of ages, one of salaries): to compute average age you open just the age register instead of flipping through every card.

### 4.6 Unstructured data example (slide)

A clinic stores visits with a date and a list of symptoms.
- **Structured (relational)** design needs several tables: `Visit(customer_id, visit_id, date)` and `Symptoms(customer_id, visit_id, symptom_id, symptom)` with joins.
- **Unstructured / column-family** design stores it in one place:

```text
Date: {2-Oct-2017}
Symptoms: {Chest pain, Headache}
```

Simpler and more efficient because the number of symptoms varies per visit.

---

## 5. Apache Hive

### 5.1 Definition
Hive is an **open-source data warehouse infrastructure** for processing **structured data in Hadoop**. It gives an **SQL-like language (HiveQL)** to query, analyse, summarise and manage data in HDFS, built on top of MapReduce and Hadoop.

Originally developed by the **Facebook** infrastructure team to support:
- Structured logs with rich data types (structs, lists, maps).
- A user base that wanted to access data in the language of their choice.
- Many traditional SQL workloads (filters, joins, aggregations).
- Other non-SQL workloads.

### 5.2 Core idea

```text
HiveQL query  →  Hive compiler  →  DAG of MapReduce (or Spark) jobs  →  run on Hadoop cluster  →  result
```

- Hive operates on data **already in HDFS**.
- **Metastore** stores the schema (metadata) of the data, so HDFS files can be treated as tables.
- Hive **does not store data itself**.

### 5.3 Analogy
Hive is a **translator at a meeting**: you speak SQL, the cluster only understands MapReduce, and Hive translates in real time. The **Metastore** is the **library catalogue** (which book is on which shelf, what each book contains) while **HDFS** is the **library shelves** holding the actual books.

### 5.4 Features

#### Advantages
- No need for Java or MapReduce expertise.
- SQL-like queries (HQL) are **implicitly transformed to MapReduce or Spark jobs**.
- Can analyse very large datasets in HDFS.
- Supports multiple storage types: plain text, **RCFile** (Record Columnar File), HBase.
- Can operate on **compressed** data.
- Supports **UDFs** (user-defined functions).

#### Limitations (as per slides)
- **Not for real-time** data.
- **Not a full database:** no update, alter or deletion of individual records.
- **Not designed for OLTP.**
- **Not for unstructured data.**
- **High query latency** (because jobs are compiled to MapReduce).

### 5.5 Hive components

| Component | Role |
|---|---|
| **Hive Client** | Lets applications in Java, Python, C++ etc. talk to Hive (via Thrift, JDBC or ODBC drivers) |
| **WebUI / Hive CLI** | Web GUI / command-line shell to run Hive queries |
| **Thrift Server (HiveServer)** | Cross-language service provider; accepts requests from clients and passes them to the Driver |
| **JDBC Driver** | Connects Java applications to Hive (class `org.apache.hadoop.hive.jdbc.HiveDriver`) |
| **ODBC Driver** | Lets ODBC-compatible applications connect to Hive |
| **Hive Driver** | Receives queries from WebUI, CLI, Thrift, JDBC/ODBC and hands them to the compiler |
| **Hive Compiler** | Parses the query, does semantic analysis of query blocks/expressions, builds the execution plan using metastore information, converts HiveQL into a **DAG of MapReduce jobs** |
| **Optimizer** | Optimizes the plan before execution |
| **Hive Execution Engine** | Executes the tasks **in order of their dependencies** |
| **Hive MetaStore** | Central repository of **structure information**: tables, partitions, column names and types, serializers/deserializers and the HDFS files where data lives |
| **SerDe** | **Serialization/Deserialization**: reads data from disk and converts it to Hive format; reverse on writes (e.g. Thrift, JSON) |

**Hive Services:** all client interactions go through Hive Services; the Driver processes requests, the compiler converts them into an actionable plan, the plan is optimized and passed to the execution engine using SerDe.

### 5.6 Query flow

```text
Client (CLI / WebUI / JDBC / ODBC / Thrift)
        ↓
     Driver
        ↓
    Compiler  ⇄  Metastore   (schema, partitions)
        ↓
    Optimizer
        ↓
 Execution Engine  →  MapReduce / Spark jobs  →  HDFS (data)
```

### 5.7 Hive data model

Hive data is stored as **HDFS files** and organised at three levels:

| Level | Meaning | Physical form |
|---|---|---|
| **Table** | Like a table in a relational DB; supports filter, project, join, union | Mapped to an **HDFS directory** |
| **Partition** | Table divided by a **partition key (column)** to group similar data; helps scalability and pruning | **HDFS sub-directory** |
| **Bucket** | Subdivision of a partition, based on the **hash of a chosen column**, to speed up queries | **Files** |

```text
Table   (directory)
  └── Partition   (sub-directory)   e.g. ds=2009-03-20
         └── Bucket   (files)       hash(column) mod #buckets
```

#### Analogy
Table = a **cupboard**, partition = a **shelf** labelled by date, bucket = a **box** on that shelf chosen by hashing (e.g. roll number mod 4).

### 5.8 Hive compilation example (from the VLDB paper on the slide)

```sql
FROM (SELECT a.status, b.school, b.gender
      FROM status_updates a JOIN profiles b
        ON (a.userid = b.userid AND a.ds = '2009-03-20')
     ) subq1
INSERT OVERWRITE TABLE gender_summary PARTITION(ds='2009-03-20')
  SELECT subq1.gender, COUNT(1) GROUP BY subq1.gender
INSERT OVERWRITE TABLE school_summary PARTITION(ds='2009-03-20')
  SELECT subq1.school, COUNT(1) GROUP BY subq1.school
```

What it shows:
- The **sub-query joins** `status_updates` and `profiles` on `userid` (restricted to one date partition).
- The result feeds **two different summaries** (`gender_summary`, `school_summary`), each a **group-by + COUNT**.
- Hive compiles this into a **DAG of MapReduce jobs** (a join job followed by group-by/aggregation jobs), so the user never writes Map/Reduce code.

### 5.9 Installation (slide)
Hive needs three software packages: **Java Development Kit**, **Hadoop**, and a **compatible Hive version** (Hive 1.2 onwards supports Java 1.7 or newer).

### 5.10 Exam traps
- Hive is **not a database**; it is a **SQL layer over Hadoop**. It stores data in HDFS/HBase.
- **Metastore** stores metadata, **not** the data.
- Hive = **high latency, batch**, not OLTP.
- Partition = **sub-directory**; bucket = **hash-based files** inside a partition.

---

## 6. Apache HBase

### 6.1 Definition
HBase is a **distributed, column-oriented database built on top of HDFS**, modelled on Google **BigTable**. Data is **logically** organised as rows and columns of a table, but designed for **semi-structured data** whose fields may vary in size and presence.

### 6.2 Why HBase?
HDFS is bad at **record lookup, small incremental writes, updates**. HBase gives **random, real-time read/write access** on huge datasets while storing its files in HDFS.

### 6.3 Data model – terminology

| Term | Meaning |
|---|---|
| **Table** | Collection of rows **sorted on RowID (row key)** |
| **Row** | Identified by a **row key** (byte array); acts as the **primary key** and is **indexed** for fast lookup |
| **Column family** | Group of related columns; **part of the table schema** (declared upfront); a table has a fixed set of column families |
| **Column** | Written **ColumnFamily:ColumnName** (e.g. `Info:height`); **columns are NOT part of the schema**, so **dynamic columns** are allowed |
| **Cell** | Smallest unit; identified by **{row key, column, timestamp/version}** and holds the value as a byte array |
| **Timestamp / version** | System timestamp (or any unique version number) attached to each cell; allows **multiple versions** of the same cell |

```text
Cell = ( RowKey , Column Family:Column , Timestamp )  →  Value
R, (K,V),  TS1
R, (K,V1), TS2      ← same cell, newer version
R, (K,V2), TS3
```

### 6.4 Example (slide)

| Row key | Info:height | Info:age | School:House | School:Sports |
|---|---|---|---|---|
| HarryPotter | 4.5ft | 11@2011 | Gryffindor | Quidditch |
| Voldemort | 7ft | 50 | Slytherin | *(empty)* |

Stored as column families:

```text
HarryPotter → Info:{height:"4.5ft", age:"11@2011"}
              School:{House:"Gryffindor", Sports:"Quidditch"}
Voldemort   → Info:{height:"7ft", age:"50"}
              School:{House:"Slytherin", Role:"Prefect@1980, DarkLord@1995"}
```

Points to notice:
- The **School** family has **different columns in different rows** (Sports for one, Role for another) → **dynamic columns**, no empty cells are stored.
- The same column (`Role`) holds **different values at different timestamps** (Prefect@1980, DarkLord@1995).
- Different types of data go into **different column families** (Info vs School).

#### Analogy
A giant **phone directory** whose entries are sorted by name (row key). Each entry has **sections** (column families) such as "Personal Info" and "School", and each person can have their own extra fields inside a section. Old values are not erased; the newest one is shown, older ones stay with their dates.

### 6.5 Architecture (master–slave)

HBase has three major components:
1. **MasterServer (HMaster)**
2. **RegionServers**
3. **Regions**
Plus **ZooKeeper** for coordination and failure detection.

| Component | Role | HDFS analogue |
|---|---|---|
| **Region** | A **horizontal range partition of a table's rows** (subset of rows). Created **automatically**: a table starts with one region and the master monitors load and **splits into multiple regions** dynamically | Like a block |
| **RegionServer** (slaves, many) | Holds regions; **communicates with the client** and manages data operations; **serves reads and writes** (using a log) for its regions; decides region size based on a threshold | **DataNode** |
| **MasterServer** | **Assigns regions** to region servers, **detects failure using ZooKeeper**, **monitors** RegionServers and **load balances** regions; supports **admin functions** (schema changes, create tables/column families) | **NameNode** |
| **META table** | Keeps track of which regions live on which servers | – |

```text
            Client
              |   (ZooKeeper + META table locate the region)
          HMaster  ── assigns/balances regions, admin
        /     |     \
 RegionServer RegionServer RegionServer
  [Region][Region] [Region]  ...   (data in HFiles on HDFS)
```

### 6.6 Read / write path (inside a RegionServer)

| Component | Where | Purpose |
|---|---|---|
| **BlockCache** | RAM | **Read cache**, LRU-evicted, **shared across regions** of a RegionServer |
| **MemStore** | RAM | **Write cache**: sorted map of KeyValues, **specific to every region** |
| **Write-Ahead Log (WAL)** | Persisted on **HDFS** | Used for **recovery** after a crash |
| **HFile** | **HDFS** (disk) | **Sorted KeyValues** on disk; the final persisted data |

```text
WRITE :  Client → WAL (durable log on HDFS) + MemStore (RAM, sorted) → flushed later to HFile on HDFS
READ  :  Client → BlockCache / MemStore / HFiles → merged result
```

- BlockCache and MemStore live in **RAM**.
- HFiles and the WAL are **persisted to HDFS**.

#### Analogy
You take notes in class: you first jot the point in a **diary (WAL)** so you never lose it, then put it on your **desk notepad (MemStore)** in order. When the notepad fills up you copy it neatly into the **permanent notebook (HFile)**. For reading, you keep your most-used pages **bookmarked (BlockCache)**.

### 6.7 HBase shell commands (slides)

```text
create 'test', 'data'                         # table 'test' with column family 'data'
put 'test', 'row1', 'data:1', 'value1'        # table, row key, column (family:qualifier), value
put 'test', 'row2', 'data:2', 'value2'
get 'test', 'row1'                            # fetch a specific row
scan 'test'                                   # fetch all rows
```

`get` output shows `COLUMN  data:1`, `CELL  timestamp=..., value=value1`.

### 6.8 Comparison – HDFS vs Hive vs HBase

| Feature | HDFS | Hive | HBase |
|---|---|---|---|
| Nature | File system | SQL layer / warehouse | NoSQL columnar DB |
| Data type | Unstructured files | Structured | Semi/unstructured |
| Random read/write | No | No | **Yes** |
| Updates | Append only | Not supported (per slides) | **Yes** |
| Query interface | Commands / MR | HiveQL | Shell / API (get, put, scan) |
| Latency | Batch | High | Low (real-time) |

### 6.9 Exam traps
- **Column families are fixed in schema; columns are not.**
- A **cell** = row key + column + timestamp.
- **Regions** are **row-range partitions** and split automatically.
- **HMaster ≈ NameNode**, **RegionServer ≈ DataNode**; failures detected via **ZooKeeper**.
- **MemStore = RAM write cache, WAL = durable log for recovery, HFile = on HDFS.**

---

## 7. Apache Cassandra

### 7.1 Definition
Cassandra is an **open-source, distributed, columnar NoSQL database**, built at **Facebook**, that handles extremely large datasets across many nodes. It draws its **data model from Google BigTable** (same model as HBase) and its **architecture from Amazon Dynamo**.

### 7.2 Key features
- **Peer-to-peer architecture** (no master), unlike HBase's master–slave.
- **Elastic scalability:** add nodes to the cluster with **linear performance scaling**.
- **Replication** across multiple nodes prevents data loss on failure (similar to HBase).
- Handles **structured, semi-structured and unstructured** data and can **change data structures dynamically**.
- The slides list support for **ACID** properties.
- Uses its **own local persistence/storage engine** (**does not rely on HDFS**).

> Side note: in real systems Cassandra is better described as having **tunable consistency** rather than full ACID transactions. For this exam, follow the slide, which lists ACID.

### 7.3 Architecture – peer-to-peer ring

- Data is distributed among **all nodes** in the cluster, organised as a **ring**.
- **All nodes play the same role.** Each node is independent and interconnected with others.
- **Any node can accept read/write requests**, no matter where the data actually lives. The node contacted acts as the **coordinator** (proxy between client and the nodes holding the data).
- If a node goes down, **other nodes act as replicas** for its data and requests are still served.
- **Ring membership and failure detection** are handled **by the nodes themselves** (no master).

```text
            ┌── Node A ──┐
        Node F          Node B      ← ring: all nodes equal
            │   RING    │            any node = coordinator
        Node E          Node C
            └── Node D ──┘
```

#### Analogy
A company with **no boss**: every employee is equal, every one can take a customer's order (coordinator) and forward it to whoever holds the file, and if one employee is absent, colleagues who keep copies step in. HBase is the opposite: it has a **head librarian (HMaster)** who assigns work.

### 7.4 Write path and storage structures

| Term | Meaning |
|---|---|
| **Commit log** | **Crash-recovery** mechanism. **Every write is first written to the commit log.** |
| **Mem-table** | **Memory-resident** structure; after the commit log, data goes here (sometimes multiple mem-tables per column family) |
| **SSTable** | **Disk file** to which the mem-table is **flushed when it reaches a threshold** |
| **Bloom filter** | A quick, **probabilistic** test for whether an element is a member of a set (a special kind of cache). Accessed **after every query** to avoid needless disk reads |

```text
Write → Commit log (disk, for recovery)
      → Mem-table (RAM)
      → flush at threshold → SSTable (disk)
Read  → check Bloom filter → look only at SSTables that may contain the key
```

A Bloom filter can say "**definitely not here**" or "**maybe here**" (false positives possible, false negatives not).

#### Analogy
Commit log = **diary**, mem-table = **whiteboard**, SSTable = **archived file**, Bloom filter = a **bouncer** who instantly says "this person is definitely not inside" so you don't waste time searching the whole building.

### 7.5 Query language
- **CQL (Cassandra Query Language)** treats the database (**Keyspace**) as a **container of tables**.
- Use the **`cqlsh`** prompt or application-language drivers.
- CQL is **richer in functions** than the HBase query language.

### 7.6 Data organisation
Cassandra organises data into **partitions**. Each partition has a **partition key** and consists of multiple columns. Partitions are stored on a node, and **each node is responsible for a fraction of the partitions**.

### 7.7 Cassandra vs HBase

| Feature | HBase | Cassandra |
|---|---|---|
| Architecture | **Master–slave** (HMaster + RegionServers) | **Peer-to-peer ring**, no master |
| Origin / inspiration | Google BigTable | BigTable data model + Amazon Dynamo architecture; built at Facebook |
| Storage engine | **Built on HDFS** | **Local persistence engine**, no HDFS |
| Request coordination | Via master/regions | Over partitioned dataset, **any node coordinates** |
| Failure detection | **Master via ZooKeeper** | **Nodes themselves** |
| Data organisation | Tables → regions (row-key ranges) | Partitions with partition key on ring |
| Query language | HBase shell (get/put/scan) | **CQL** (richer) |
| Data model | Column families, cells with timestamps | **Same data model** |

### 7.8 Exam traps
- **No master** in Cassandra; **every node equal**.
- Write order: **commit log → mem-table → SSTable**.
- Cassandra does **not** use HDFS.
- Bloom filter = probabilistic membership test, **not** a storage file.

---

## 8. Apache Pig and Pig Latin

### 8.1 Why Pig? (limitations of MapReduce)
MapReduce is powerful but **low-level**:
- **Manual coding:** operations like JOIN, FILTER must be coded by hand.
- **Complex and rigid:** code is hard to read, maintain and debug.
- **Lacks flexibility:** the rigid two-step (map → reduce) model is not ideal for complex pipelines.
- **Low productivity:** even a simple job needs significant effort and expertise.
- Data analysis needs programs translated into **a series of Map and Reduce stages**, requiring retraining.

We want an abstraction where the user gives input in a **high-level scripting language** and it is **converted to MapReduce** automatically. Pig (with Pig Latin) is that; Hive is the SQL-like alternative.

### 8.2 What is Pig?
- Apache Pig is a **high-level language/platform** for writing complex MapReduce transformations using a **simple scripting language**.
- Introduced by **Yahoo** as an open-source high-level **data flow** system.
- Provides **abstraction over MapReduce**.
- Supports **all kinds of data** (structured and unstructured), stored in HDFS (hence the name "pig", which eats anything).

### 8.3 Components

| Component | Role |
|---|---|
| **Pig Latin** | The SQL-like, **data-flow language**; operators like join, group, filter, limit; supports **automatic optimization** and functions written in other languages such as Java |
| **Pig execution / PigServer** | Runtime environment that runs Pig Latin programs |
| **Grunt** | The **Pig shell** (interactive) |

**Pig Latin scripts are internally converted into MapReduce jobs.**

Pig Latin defines transformations such as **aggregate, join, sort**. Pig is often used for **ETL pipelines, quick research on raw data, and iterative data processing**.

### 8.4 Running modes

| Mode | Where it runs | Command |
|---|---|---|
| **MapReduce mode** (default) | Needs a Hadoop cluster and HDFS; input/output on **HDFS** | `pig` |
| **Local mode** | Single machine, local host and **local file system** | `pig -x local` |

### 8.5 Pig vs MapReduce

| Apache Pig | Hadoop MapReduce |
|---|---|
| High-level **data-flow** tool | **Low-level** data processing paradigm |
| **Scripting language** | **Compiled programming language** (Java/Python) |
| Abstraction at higher level | Abstraction at lower level |
| **Fewer lines of code** | More lines of code |
| Less development effort | More development effort |
| Built-in joins, filters, ordering, sorting | Such operations are tedious to code |
| Provides **nested data types: tuples, bags, maps** | No nested data types |
| Code efficiency is lower than hand-written MR | Hand-tuned MR can be more efficient |

### 8.6 Worked example – top 10 most popular IPL matches at each venue

Two datasets:
- `Visits(user, matchId, time)`
- `MatchInfo(matchId, venue, winner)`

#### Data flow (logic)

```text
Load Visits → Group by matchId → Foreach matchId generate count      (popularity per match)
Load MatchInfo →  Join on matchId
        → Group by venue → Foreach venue generate top10 matchIds
```

#### As MapReduce (Pig compiles to 3 jobs)

```text
Job 1 : Map1 / Reduce1  → Group by matchId, generate count
Job 2 : Map2 / Reduce2  → Join with MatchInfo on matchId
Job 3 : Map3 / Reduce3  → Group by venue, generate top10(matchIds)
```

#### In Pig Latin (as on the slide)

```pig
visits          = load '/ipldata/visits' as (user, matchid, time);
gMatches        = group visits by matchId;
matchPopularity = foreach gMatches generate matchId, count(visits);
matchInfo       = load '/ipldata/matchInfo' as (matchId, venue, winner);
venueCounts     = join gMatches by matchId, matchInfo by matchId;
gVenues         = group venueCounts by venue;
topMatches      = foreach gVenues generate top(matchPopularity, 10);
store topMatches into '/data/topMatches';
```

Statement-by-statement: `load` reads data, `group` collects rows by key, `foreach ... generate` computes per-group values (here a count), `join` combines two datasets on a key, `store` writes the result.

*(The slide joins `gMatches` but then uses `matchPopularity`; logically the popularity counts are what get joined. The course says you need not memorise Pig syntax, only the basics and how it maps to MapReduce tasks.)*

The same task in raw MapReduce would need **three separately written Map/Reduce programs** chained by hand; in Pig it is **about 8 lines**.

#### Analogy
MapReduce = **building the kitchen and cooking each step by hand**. Pig Latin = **writing the recipe in plain steps**; the kitchen (Pig runtime) turns the recipe into actual cooking (MapReduce jobs).

### 8.7 Exam traps
- Pig is created at **Yahoo**; Hive at **Facebook**.
- Pig Latin is **data-flow (procedural)** style; Hive is **SQL-like (declarative)**.
- Modes: **MapReduce (default)** and **Local** (`-x local`).
- Nested types: **tuple, bag, map**.

---

# Part C – Data Ingestion

## 9. Data Ingestion: Sqoop, Flume, SeaTunnel and CDC

### 9.1 Why ingestion tools?
Data has to get **into** Hadoop from many places (databases, logs, message queues). Plain `hdfs dfs -put` needs data already packaged as **files** and copies **one file at a time**, which is slow and unreliable for **continuously generated** data (e.g. web-server logs).

### 9.2 Apache Sqoop (SQL-to-Hadoop) – legacy

#### Definition
A tool for **bulk import/export** of data between **structured data stores** (relational DBs, enterprise data warehouses, NoSQL) and **HDFS/Hive/HBase**. It is a **data-migration tool with a connector architecture**, supporting plugins for new external systems.

#### Context
Many traditional applications work with structured DBs and generate big data. To analyse it in Hadoop, the data must be **provisioned into HDFS** and results sent **back** to the SQL database. That is what Sqoop does.

#### Positioning
- **Bulk data transfer** tool for voluminous data.
- **Import/Export** to/from SQL (MySQL, Oracle, PostgreSQL, DB2) to **HDFS, Hive, HBase**.
- **Defines the schema** for import.
- Integrates with **Oozie as an action**.
- Supports **plugins** for new data sources.

#### Import (RDBMS → HDFS)

```bash
sqoop import --connect jdbc:mysql://localhost/acmedb \
      --table ORDERS --username test --password ****
```

- `--connect`: connection to the DB; `--table`: table to import (**each row is split into a map**).
- **Step 1:** inspect the database to gather **metadata** about the data being imported.
- **Step 2:** transfer the data using a **map-only Hadoop job** and store it in an **HDFS directory**, as a **CSV file with newline as record separator**.

#### Export (HDFS → RDBMS)

```bash
sqoop export --connect jdbc:mysql://localhost/acmedb \
      --table ORDERS --username test --password **** \
      --export-dir /user/arvind/ORDERS
```

- `--export-dir`: HDFS directory to export from.
- **Step 1:** inspect the target table's metadata. **Step 2:** each **map task** transfers the data using **many transactions** for optimal throughput and minimal resource use.

#### Limitations and status
- **Batch-only** (no real-time/streaming).
- **High overhead for small datasets** (MapReduce start-up).
- **No built-in data validation.**
- **No CDC** support.
- **Retired in June 2021** and moved to the **Apache Attic**.

#### Analogy
A **moving truck** between the old warehouse (RDBMS) and the new giant warehouse (HDFS); you book the truck, it carries big batches, and it can also bring goods back.

---

### 9.3 Apache Flume – legacy

#### Definition
A **distributed, reliable, fault-tolerant service** for **collecting, aggregating and moving large amounts of streaming data** (logs and events, e.g. web-server logs) from many sources to a **centralized store** (HDFS or HBase), with failover and recovery.

#### Architecture: Agent = Source + Channel + Sink

```text
Data generator → [ Source → Channel → Sink ] → HDFS / HBase
                        (one Flume Agent = independent JVM daemon)
```

| Component | Role |
|---|---|
| **Agent** | Independent daemon (JVM process) that receives events from data generators and stores them in a central store |
| **Source** | Part of the agent; receives data from generators and passes it to one or more channels as **Flume events** |
| **Flume event** | **Basic unit of data transport**; a **byte-array payload** (with optional headers) |
| **Channel** | **Transient store / buffer** between source and sink; holds events until the sink consumes them (e.g. **JDBC channel, file-system channel**) |
| **Sink** | Consumes events from the channel and **delivers them to the destination** (HDFS, HBase) |

#### Limitations and status
- **Streaming-only**, no batch.
- **No data transformation** capability.
- **High latency**, not suited for real-time analytics.
- **Dormant since October 2024**, not recommended for new projects.

#### Analogy
Rainwater from many roofs (log sources) flows through **pipes** into a **buffer tank (channel)** and is then pumped (sink) into a big **reservoir (HDFS)**. If the pump is slow, the tank absorbs the flow.

---

### 9.4 Apache SeaTunnel – the modern replacement

#### Definition
An **easy-to-use, ultra-high-performance, distributed data integration platform** that synchronizes massive volumes of data between **heterogeneous systems**, reliably and efficiently. It **unifies batch and streaming** data movement in one tool.

#### History / facts

| Fact | Detail |
|---|---|
| Original name | **Waterdrop** (2017) |
| Renamed | **SeaTunnel** (October 2021) |
| Entered Apache Incubator | December 2021 |
| Top-Level Project (graduated) | **June 2023** |
| Connectors | **100+** |
| Configuration | **One config file** (HOCON / JSON / SQL), **no Java/Scala needed** |

#### The N × M connector problem

```text
BEFORE : every source needs a custom script to every sink
         3 sources × 3 sinks = 9 point-to-point scripts to build and maintain
AFTER  : sources and sinks each plug into SeaTunnel once
         3 + 3 = 6 connectors — write once, reuse anywhere
General: N × M  →  N + M
```

Analogy: **direct flights between every pair of cities** (N×M routes) vs a **hub airport** (N+M routes).

#### Pipeline architecture: Source → Transform → Sink

| Stage | Role |
|---|---|
| **Source** | Connects to the origin system (MySQL, Kafka, HDFS…) and **emits rows** |
| **Transform** | **Optional** reshaping in flight: filters, maps, splits, SQL transforms |
| **Sink** | Writes the final rows to the target (warehouse, lake, queue) |

Stages are linked with **`plugin_output`** (`result_table_name`) and **`plugin_input`** (`source_table_name`); if not set, the previous module's output is fed forward.

#### Architecture deep dive

```text
Config file (HOCON/JSON/SQL)
        ↓
Connector V2 API  (translation layer that decouples plugins from any one engine)
        ↓
Execution engine (choose per job): Zeta | Apache Flink | Apache Spark
        ↓
Source connectors  ⇄  Sink connectors   (100+)
```

| Engine | Character |
|---|---|
| **Zeta** | **Native**, built-in, lightweight, **default and recommended** for most deployments and pure sync jobs |
| **Apache Flink** | **Streaming-first**; reuse an existing Flink cluster; low-latency event processing |
| **Apache Spark** | **Batch-first**; reuse an existing Spark cluster; heavy ETL |

#### The config file (HOCON) – four blocks

```hocon
env       { job.mode = "BATCH" }                       # global settings (BATCH or STREAMING)
source    { MySQL  { result_table_name = "orders"  table-name = "orders" } }
transform { Filter { source_table_name = "orders"  fields = [id, amount, city] } }   # optional
sink      { Hive   { table = "orders_clean" } }
```

#### Key features
1. **Batch and streaming** with the same engine and config format.
2. **Large connector library** (100+): databases, files, lakes, warehouses, message queues, cloud/SaaS.
3. **Multiple execution engines** (Zeta, Flink, Spark).
4. **Change Data Capture (CDC)**.
5. **Exactly-once fault tolerance** (checkpointing, distributed snapshots: no loss, no duplication).
6. **Low-code configuration.**
7. **Automated schema evolution** (upstream schema changes propagate downstream).

#### Why SeaTunnel replaces Sqoop and Flume

| Sqoop / Flume limitation | SeaTunnel answer |
|---|---|
| Sqoop: batch-only | Batch **and** streaming in one tool |
| Flume: streaming-only | Batch **and** streaming |
| Sqoop: MapReduce start-up overhead | **No MapReduce dependency**; runs on Zeta/Flink/Spark |
| Sqoop: no CDC | **Built-in CDC** |
| Sqoop: no data validation | Exactly-once checkpointing |
| Flume: no transformation | Built-in **transform** stage |
| Narrow connector sets | **100+ connectors** |
| Legacy (Sqoop retired 2021, Flume dormant 2024) | Actively maintained Apache **Top-Level Project** |

---

### 9.5 Change Data Capture (CDC)

#### Definition
CDC is a data-integration pattern that **identifies and tracks row-level changes (INSERT, UPDATE, DELETE)** in a source database and **delivers them to downstream systems in near real time**.

#### How it works

```text
1. Monitor DB log   → read the DB's own transaction log (MySQL binlog, PostgreSQL WAL, MongoDB oplog)
2. Capture changes  → extract only INSERT / UPDATE / DELETE events (no full-table scans)
3. Deliver          → stream them to the target (lake, warehouse, queue) in real time
```

**CDC connectors in SeaTunnel:** MySQL-CDC, PostgreSQL-CDC, Oracle-CDC, MongoDB-CDC.

#### Why it matters
- **No repeated full-table queries**, so minimal load on the source DB.
- **Avoids locking.**
- Supports **exactly-once** delivery.
- The mechanism behind **real-time data-lake synchronization**.
- **Foundation for modern tools:**
  - **Apache Hudi:** handles **upserts/deletes** on data lakes with ACID; CDC keeps the lakehouse continuously in sync.
  - **Apache Druid:** ingests CDC streams (from Kafka) for **sub-second OLAP** on freshly changed data.
  - **Apache Iceberg:** open table format with row-level updates; CDC enables **incremental processing and time-travel**.

#### Analogy
Instead of downloading the **entire bank statement** every day, you get an **SMS for each transaction** as it happens. The transaction log is the bank's own record; CDC just reads new lines.

### 9.6 Sqoop vs Flume vs SeaTunnel

| Feature | Sqoop | Flume | SeaTunnel |
|---|---|---|---|
| Purpose | RDBMS ↔ Hadoop bulk transfer | Streaming log/event ingestion into Hadoop | Unified data integration |
| Mode | Batch only | Streaming only | **Batch + streaming** |
| Engine | MapReduce (map-only job) | Agent (Source-Channel-Sink) | Zeta / Flink / Spark |
| Transform | No | No | **Yes** |
| CDC | No | No | **Yes** |
| Connectors | JDBC-based | Narrow (logs) | **100+** |
| Status | **Retired June 2021** | **Dormant Oct 2024** | Active TLP (June 2023) |

### 9.7 Exam traps
- Sqoop import = **map-only job**, output as CSV in HDFS.
- Flume unit = **event**; channel = **transient buffer**; agent = **source + channel + sink**.
- SeaTunnel default engine = **Zeta**; **Transform is optional**.
- CDC reads the **transaction log**, not the tables.
- Sqoop **retired (2021)**, Flume **dormant (2024)**.

---

# Part D – Workflows and Cluster Management

## 10. Workflow Orchestration: DAG, Oozie, Airflow

### 10.1 Why do we need workflow managers?

#### Motivating scenario (slide): building a recommendation engine
Goal: real-time, data-driven recommendations like Amazon's "Customers who bought this also bought...". Recommendations are **pre-computed**, because computing online is too slow for the volume of data.

Steps that must happen **in order**:

```text
1. Copy logs from production system   ┐
2. Dump the production database       ┘ (ingest clickstream + purchase data)
3. Apply Big Data algorithms (clean, process, run recommendation algorithm)
4. Pre-compute recommendations
   → Sanity check
5. Store recommendations in the production database (publish to website)
```

Challenges: **How do we run this multi-step process automatically? What if one step fails? How do we guarantee correct order?**

#### Workflow
A **workflow** is a sequence of interdependent jobs, common in data centers and Big Data pipelines. A workflow manager provides:
- **Orchestrate jobs:** define and manage the sequence of interdependent jobs.
- **Automate execution:** schedule at specific times or recurring intervals.
- **Simplify management:** run a complex multi-step process with a single command.
- **Ensure reliability:** define actions when a job fails.

### 10.2 DAG – Directed Acyclic Graph

#### Definition
A DAG is a mathematical structure used to model workflows as a **set of tasks with explicit dependency relationships**, where **no task can depend on itself, directly or indirectly**.

#### Three defining properties

| Property | Meaning |
|---|---|
| **Directed** | Edges have a direction: A → B means **B depends on A** and runs only after A succeeds |
| **Acyclic** | **No cycles**: a task cannot depend on itself, even transitively; this guarantees a **finite execution order** |
| **Graph** | **Nodes = tasks, edges = dependencies**; the whole structure is the workflow topology |

**A DAG defines WHAT tasks run and in WHAT ORDER; it does not execute them.** The orchestrator (Oozie/Airflow) executes.

#### Properties of DAG-based workflows

| Property | Meaning |
|---|---|
| **Topological ordering** | Tasks can always be sorted so each appears before its dependents |
| **Parallel execution** | Independent tasks (no shared dependency) run **simultaneously** |
| **Deterministic execution** | Same inputs → same task order |
| **Fault isolation** | A failed task can be **retried in isolation** without re-running the whole pipeline |
| **Idempotency** | A workflow can be safely re-triggered; re-running gives the same result |

#### Real-world DAGs
- **ETL:** Extract → Transform → Validate → Load → Notify
- **ML training:** Pre-process → Train → Evaluate → Register model
- **CI/CD:** Build → Unit test → Integration test → Deploy
- **Reports:** Query DB → Aggregate → Format → Email → Archive

#### Analogy
Cooking a meal: you cannot **frost** the cake before **baking** it (directed dependency), and you cannot have "bake requires frosting and frosting requires baking" (a cycle, which would make the meal impossible). Chopping vegetables and boiling rice can go on **in parallel** because they don't depend on each other.

---

### 10.3 Apache Oozie – legacy Hadoop workflow scheduler

#### Definition
Oozie is a **workflow director / server-based workflow scheduler for Hadoop jobs**. It manages and coordinates multiple interdependent jobs as a **single workflow**, using **XML-based workflow definitions**, on a YARN cluster.

- Dependencies between jobs are specified by the user as a **DAG**; Oozie schedules them in the right order and at a specified frequency.
- Workflows can be specified in terms of **sequencing, parallelization and aggregation** of jobs.
- **Tightly integrated with the Hadoop stack:** supports **MapReduce, Hive, Pig, Sqoop** jobs, plus system jobs like **Java and Shell**.
- It is an **open-source Java web application running on Tomcat**. It receives requests from a client, triggers workflow actions using the Hadoop execution engine, and gives each task a **unique callback HTTP URL** which the task notifies on completion. Oozie can also **poll** a task for completion.
- Leverages existing Hadoop machinery for **load balancing and fail-over**.

#### Oozie architecture

| Component | Role |
|---|---|
| **Oozie Server** | Central service coordinating execution of all jobs |
| **Workflow Engine** | Executes the workflow DAG **step by step** |
| **Coordinator Engine** | Triggers workflows on **time or data availability** |
| **Metadata DB** (Derby/MySQL) | Stores workflow state, history, metadata |

Flow: **user submits workflow (XML)** → Oozie (Tomcat app) → runs supported applications (MapReduce, Pig, Hive, Sqoop, Spark, Java, Shell…).

#### Workflow = Action nodes + Control-flow nodes

| Node type | Meaning |
|---|---|
| **Action node** | A **workflow task**: run MapReduce/Pig/Hive job, import via Sqoop, run shell/Java, move files in HDFS. When it finishes, the **next task** in the workflow runs |
| **Start node** | Marks the start of the workflow |
| **End node** | Marks successful end |
| **Error / Kill node** | Marks an error and prints the error message |
| **Fork** | Starts **parallel** tasks |
| **Join** | **Waits** for all parallel branches to complete, then continues |
| **Decision** | Like a **switch** statement: chooses the next branch based on a runtime condition |

At the end of a workflow (and optionally on entry/exit of an action node) Oozie updates the client via an **HTTP callback**.

#### Types of Oozie jobs

| Type | Meaning |
|---|---|
| **Workflow** | A specified sequence of Hadoop jobs with **outcome-based decision points and control dependency** |
| **Coordinator** | A **scheduled workflow job**; **time- and data-dependent** (recurring) |
| **Bundle** | **Batch of a set of coordinated jobs** (groups multiple coordinators) |

#### Example workflows from the slides

**Example 1**

```text
Start → Spark job → Fork → ┬ Pig job ┐
                           ├ MR job  ├→ Join → End
                           └ Hive job┘
```
The Join waits until **all three parallel jobs** finish.

**Example 2 (complex pipeline)**

```text
Start → Map-Reduce → Fork → ┬ MR Streaming ┐
                            └ Pig          ┴→ Join → Decision → MR Pipes → Java → File-System → End
```
- **MR Streaming:** MapReduce using scripts in Python/Perl.
- **MR Pipes:** MapReduce via the Pipes API (C++ programs).
- **Java:** custom logic or validation. **File-System:** HDFS operations like move/delete.

#### Running an Oozie workflow (slide commands)

```bash
# 1. Workflow definition + resources (workflow.xml, jars, Pig scripts) are placed in HDFS
hadoop fs -put hadoop-examples/target/<name of workflow dir> <name of workflow>
# 2. Tell the client which Oozie server to use
export OOZIE_URL="http://localhost:11000/oozie"
# 3. Run it (job.properties holds parameters)
oozie job -config ch05/src/main/resources/max-temp-workflow.properties -run
# 4. See results
hadoop fs -cat <location of result>
```

`job.properties` example: `nameNode=hdfs://localhost:8020`, `jobTracker=localhost:8021`, `oozie.wf.application.path=${nameNode}/user/${user.name}/<workflow>`.

#### Oozie vs YARN (exam favourite)

| YARN | Oozie |
|---|---|
| **Resource manager**: allots resources (containers) and runs jobs | **Workflow manager**: connects and controls the **sequence** of jobs and monitors the flow |
| Serves Oozie **and** other applications | Submits its jobs **to** YARN |

**YARN manages resources for individual jobs; Oozie sequences and controls those jobs.**

#### Limitations of Oozie (why Airflow replaced it)
- **XML** workflows: verbose, hard to version-control, error-prone.
- **No Python** support (only Java actions or shell).
- **Tightly coupled to Hadoop/YARN/HDFS**, not cloud-agnostic.
- **No dynamic task generation** (static DAGs only).
- **Poor debugging** and limited failure visibility; **basic UI**.
- **No support for modern stores** (S3, BigQuery, Snowflake).
- **Slow community innovation:** in **maintenance mode since 2020**.

#### Analogy
Oozie is an **old-school orchestra conductor** who works only with **paper sheet music in XML** and only in the Hadoop concert hall.

---

### 10.4 Apache Airflow – modern orchestrator

#### Definition
Apache Airflow is an **open-source platform to programmatically author, schedule and monitor workflows**. Workflows are **DAGs written in Python**.

| Fact | Detail |
|---|---|
| Origin | **Airbnb, 2014** |
| Apache Top-Level Project | **2019** |
| Language | **Python 3.x** (DAGs are pure Python) |
| Licence | Apache 2.0 |
| Philosophy | **Configuration as Code** (workflows are Python scripts: versioned, testable, peer-reviewed) |
| Motto | **"Author, Schedule, Monitor"** |
| Not | A streaming framework; it is an **orchestrator** |

#### Why Airflow? (traditional problems vs solution)

| Traditional (cron / shell scripts) | Airflow |
|---|---|
| No retry on failure | **Configurable retry** per task with exponential back-off |
| No dependency management | **Explicit dependencies** in the DAG |
| No pipeline visibility | **Real-time UI** with logs, Gantt chart, task history |
| Single-machine bottleneck | **Celery / Kubernetes executors** for distributed scale |
| Hard-coded cron | Cron, **event-driven (Dataset)**, manual, or API triggers |
| Config in XML/shell | **DAGs as Python code** |

#### Core architecture

| Component | Role |
|---|---|
| **Web Server** | Serves the **Airflow UI** (port 8080): trigger DAGs, view logs, manage connections/variables |
| **Scheduler** | **Heart of Airflow**: continuously scans DAG files, evaluates dependencies, **submits ready tasks to the Executor** |
| **Executor** | Decides **HOW and WHERE** tasks run (local, Celery, Kubernetes) |
| **Workers** | Processes that **execute task code**; with Celery they pull tasks from a message queue (Redis/RabbitMQ) |
| **Metadata database** | PostgreSQL/MySQL storing DAG-run state, task instances, connections, variables, XComs, SLA records |
| **DAG Processor** | **Parses Python DAG files** regularly and registers them in the metadata DB (separate process from Airflow 2.x) |

```text
DAG files (Python) → DAG Processor → Metadata DB
                                         ↓
                    Scheduler → Executor → Workers (run tasks) 
                         ↑                    ↓
                  Web Server (UI) ←── Metadata DB (state, logs, XCom)
```

#### A DAG in Python

```python
from airflow import DAG
from airflow.operators.python import PythonOperator
from datetime import datetime

def extract(): ...
def transform(): ...
def load(): ...

with DAG(dag_id='etl_pipeline',
         start_date=datetime(2025, 1, 1),
         schedule='@daily',
         catchup=False) as dag:
    t1 = PythonOperator(task_id='extract',   python_callable=extract)
    t2 = PythonOperator(task_id='transform', python_callable=transform)
    t3 = PythonOperator(task_id='load',      python_callable=load)
    t1 >> t2 >> t3          # sequential dependency
```

- `with DAG(...)` sets dag_id, start_date, schedule; **operators inside become tasks**.
- **`>>`** sets dependency (`t1 >> t2 >> t3` = sequential). Independent tasks run **in parallel**.
- `schedule` accepts cron strings (`'@daily'`, `'0 6 * * *'`), `timedelta`, **Dataset** objects (event-driven) or `None` (manual only).

#### Operators, Sensors, XCom

| Concept | Meaning |
|---|---|
| **Operator** | A **template for one task unit**. 40+ built-in, 1000+ via provider packages (e.g. PythonOperator, BashOperator, PostgresOperator, SparkSubmitOperator, cloud operators for AWS/GCP/Azure). Providers are **pip-installable** |
| **Sensor** | Special **wait-operator** that **blocks until an external condition is met** (file arrives, API response, SQL row-count, S3 object exists) |
| **XCom** | **Cross-communication:** tasks share **small** data values via push/pull, stored in the metadata DB |
| **Connections / Variables** | Central credential store (encrypted with Fernet) and runtime key-value config |
| **SLA / alerting** | Define SLAs per task; email/Slack alerts on misses or failures |

#### Executors

| Executor | Use | Notes |
|---|---|---|
| **Sequential** | **Development only** | One task at a time, no parallelism, SQLite backend, not for production |
| **Local** | Small teams / single node | Subprocess parallelism |
| **Celery** | Distributed scale-out | Worker pool via Redis/RabbitMQ; horizontally scalable; production-grade |
| **Kubernetes** | Cloud-native | **One K8s pod per task**, auto-scales to zero, best isolation |
| **CeleryKubernetes** | Hybrid | Celery for fast tasks, K8s for heavy tasks |

#### Scheduling, catchup, backfill

| Strategy | Example | Meaning |
|---|---|---|
| Cron | `schedule='0 6 * * *'` | Every day at 06:00 UTC; aliases `@daily @hourly @weekly @monthly` |
| Timedelta | `schedule=timedelta(hours=6)` | Every 6 hours relative to previous run |
| Dataset trigger (Airflow 2.4+) | `schedule=Dataset('s3://bucket/file.csv')` | Run when an upstream DAG updates a dataset (data-driven, no polling) |
| Manual | `schedule=None` | Trigger via UI, CLI or REST API only |

- **`catchup=True` (default):** if the DAG has a **past start_date**, Airflow schedules **all missing historical runs** automatically (good for reprocessing).
- **`catchup=False`:** run **only from now onwards**; avoids flooding the scheduler with hundreds of backfill runs on first deployment.
- **CLI backfill:** `airflow dags backfill --start-date 2025-01-01 --end-date 2025-01-31 my_dag` runs historical ranges manually.

#### Web UI views
**DAGs list**, **Graph view** (topology; click a task for logs/state/retry), **Grid view** (matrix of runs × tasks; green = success, red = failed, yellow = running), **Gantt view** (timeline of durations; spot bottlenecks), **Task logs** (stdout/stderr, live), **XComs browser**, **Connections**, **Variables**.

#### Case studies (slide)
- **Airbnb:** core ETL orchestration; 1000+ DAGs; replaced cron/internal tooling.
- **Spotify:** recommendation model training; each task isolated in a K8s pod.
- **LinkedIn:** feed ranking/analytics; data-quality gates block downstream tasks on failure.
- **NASA JPL:** scientific data pipelines; sensors wait for data arrival; retries and alerting.

#### Analogy
Airflow is a **modern conductor with digital sheet music (Python)**: the score can be **generated by code** (dynamic DAGs), the conductor sends **retry cues** if a player misses, a **screen (UI)** shows who is playing, and the orchestra can grow from one hall (local) to many halls (Celery/Kubernetes).

### 10.5 Oozie vs Airflow

| Feature | Oozie | Airflow |
|---|---|---|
| Workflow definition | **XML** | **Python code** (DAGs as code) |
| Dynamic DAGs | No (static) | **Yes** (generated at runtime) |
| Environment | Tied to **Hadoop/YARN/HDFS** | **Cloud-agnostic** (AWS, GCP, Azure, K8s, on-prem) |
| Operators / integrations | Hadoop jobs, Java, Shell | **1000+ providers** (S3, BigQuery, Snowflake...) |
| UI | Basic | Rich: graph, grid, Gantt, live logs, retries |
| Executors | Hadoop engine | Local, Celery, Kubernetes, hybrid |
| Monitoring | Limited | SLA, alerting, XCom |
| Status | **Maintenance mode since 2020**, legacy | Active Apache TLP (2019) |

### 10.6 Exam traps
- **DAG has no cycles**; it defines order but **does not execute** tasks.
- Oozie **fork/join** = parallel; **decision** = switch; workflow is **XML** stored in **HDFS**.
- **YARN = resources, Oozie/Airflow = sequencing.**
- Airflow **Scheduler** submits tasks to the **Executor**; **Workers** run them.
- `catchup=True` is the **default**.
- **SequentialExecutor = dev only.**

---

## 11. Apache Ambari

### 11.1 Definition
Ambari is an **open-source, web-based management framework for Hadoop clusters** for **provisioning, managing and monitoring** them. It was **introduced in late 2012** and became an **Apache top-level project in 2013**; the first tool of its kind.

### 11.2 Why Ambari?
- Simplifies **installation, configuration and management**.
- **Easy, efficient, repeatable creation** of clusters.
- **Manages and monitors** clusters.

### 11.3 What it does

| Function | Details |
|---|---|
| **Provisioning** | **Wizard-driven** cluster install across cloud, virtual and physical environments; uses **REST APIs** to automate installation; configuration through a web front end |
| **Managing** | **Single point** for start/stop and re-configuration of services; advanced configuration and host controls |
| **Monitoring** | **Single dashboard** for cluster health/status; **pre-configured and customizable metrics, notifications and alerts** |
| **Integration** | **REST API** to connect to other tools |

**Three layers of supported Hadoop components:**
1. **Core Hadoop:** HDFS, MapReduce
2. **Essential Hadoop:** Pig, Hive, HCatalog, HBase, ZooKeeper
3. **Hadoop support:** Oozie, Sqoop, Ganglia, Nagios

### 11.4 Features
Platform independent, pluggable components, version management and upgrades, extensibility, failure recovery, security.

### 11.5 Architecture

| Component | Role |
|---|---|
| **Ambari Web** | Client-side UI; calls the **Ambari REST API** to view cluster info and perform operations |
| **Ambari Server** | **Master process**; talks to **agents** on each node; has a **database** for cluster metadata |
| **Ambari Agent** | Installed on **each node**; periodically sends **heartbeat/health status, metrics and service status**; its actions are **driven by the master** |

```text
Ambari Web (browser) → REST API → Ambari Server (+ DB) ⇄ Ambari Agents (one per node)
```

| Extra concept | Meaning |
|---|---|
| **Stack** | Coordinated, tested **set of Hadoop ecosystem components** to install (e.g. **HDP**, Hortonworks Data Platform) |
| **Blueprint** | **Cluster definition file**: **two JSON files**, one generic template and one with specific properties (stack name/version, security…) that launch deployment |
| **Views** | The **user-interface** extensions |

**Defining a cluster in Ambari:**

| Term | Meaning |
|---|---|
| Stack | Set of services and where to get packages (e.g. HDP) |
| Service | Components that make up the service (e.g. HDFS) |
| Component | Building blocks of the service (NameNode, DataNode) |
| Category | **Master, slave, client** |

### 11.6 Analogy
Ambari is the **control room and installer for a building**: the **blueprint** is the architectural plan, the **agents** are the sensors in each room reporting to the central **server**, and the **dashboard** shows lights, temperature and alarms; you start/stop equipment from one place.

### 11.7 Exam traps
- Ambari = **install + manage + monitor**; it does **not** run workflows (that is Oozie/Airflow) or allocate resources (YARN).
- **Blueprint = two JSON files**; **Agent sends heartbeat**; **Server has the DB**.

---

# Part E – Lakehouse and Real-Time Analytics

## 12. From Data Warehouse to Data Lake to Data Lakehouse

### 12.1 Evolution

```text
Databases  →  Data Warehouse  →  Data Lake  →  Data Lakehouse
```

| Stage | Purpose |
|---|---|
| **Databases** | Store and manage data for **individual applications** and operational systems |
| **Data Warehouse** | **Consolidate and structure** data from multiple sources for large-scale **business intelligence**; works best with **structured, prepared data**; can become expensive/less flexible as volume and variety grow |
| **Data Lake** | Store **massive amounts of diverse data in raw, original form** on highly scalable storage (on-prem HDFS or cloud); structured, semi-structured, unstructured; used for analytics / ML / heavy processing |

As organizations grow, data becomes distributed across many systems, motivating consolidation.

### 12.2 What the data lake solves and what it doesn't

**Solves storage:**
- Massive scale, original form, without structuring everything upfront (**low cost**).
- The same data supports different workloads (**flexible**); different compute engines can be used.

**But storing data is not enough.** We also need to:
- **Organize and manage** large collections of data,
- **Efficiently query and process** it,
- **Reliably update and maintain** it over time.

A lake gives flexible, scalable storage but **not all the capabilities of a well-managed analytical system**.

### 12.3 Data Lake vs Data Warehouse

| Comparison | Data Warehouse | Data Lake |
|---|---|---|
| Data | Primarily **structured** | Structured, semi-structured, unstructured |
| When is structure defined? | **Schema-on-write**: structure defined **before** data is stored | **Schema-on-read**: store first, interpret **when used** |
| Storage cost | Generally more expensive | **Lower-cost**, scalable |
| Data formats | Physical format managed by the warehouse | Many formats: **CSV, JSON, Parquet, ORC, Avro** |
| Compute | Typically **tied to the warehouse system** | **Different engines** for different workloads |
| Management / performance | More managed, optimised for analytical queries | More flexible, but **needs more configuration and optimisation** |

#### Analogy
A **warehouse** is a supermarket: everything is sorted, labelled and priced **before** it goes on the shelf (schema-on-write). A **lake** is a huge storage yard where you **dump everything as it arrives** and decide how to read it later (schema-on-read). A **lakehouse** is that yard **with a librarian and catalogue** added on top.

### 12.4 Data Lakehouse

#### Definition
A Data Lakehouse **combines the strengths of a data lake and a data warehouse**:

| From the Data Lake | From the Data Warehouse |
|---|---|
| Scalable, lower-cost storage | Reliable data management |
| Open formats | Good analytical performance |
| Diverse data | Easy updates and querying |
| Multiple compute engines | |

#### How?
**Keep the storage and file formats of the data lake and add warehouse-like table capabilities through a modern open table format.**

> **The lakehouse does not replace the data lake; it adds the capabilities needed to use lake data as reliable analytical tables.**

### 12.5 Open Table Formats

A data lake stores **files**, but analytics needs data to behave like reliable **tables**. A table layer must answer:

| Question | Capability |
|---|---|
| Which files belong to the table? | **File mapping** |
| What is the table's schema? | **Schema definition** |
| How do we safely add, update, delete data? | **Data mutation** |
| How do we track different versions? | **Version control** |

**Open Table Format** = a **table-management layer on top of data-lake storage** (e.g. **Apache Iceberg, Apache Hudi**).
- **"Open"** = an **open specification**, not tied to one storage system or compute engine.
- It **does not replace the underlying storage**; it manages **how files are treated as tables**.

### 12.6 Apache Parquet (background for Iceberg/Hudi)
Logically a table is rows and columns; **physically** a data lake stores **files**. **Parquet** is a **column-oriented file format designed for efficient analytical workloads**; it answers "**how data is stored efficiently**". But Parquet is **a file format, not a database**: it does not define **how to update a single record while maintaining consistency or history**. That is what Hudi/Iceberg add.

---

## 12A. Apache Iceberg

### Definition
Iceberg is an **open table format for huge analytic datasets**.

- **Created by Netflix in 2017** to overcome limits of the **Hive table format** on very large analytical tables; **open-sourced in 2018** and donated to the Apache Software Foundation.
- **Three goals:** **Consistency** (reliable atomic table changes), **Performance** (efficient query planning without excessive file listing), **Ease of use** (users need not know the physical layout).
- **Iceberg provides neither storage nor a compute engine**: it enables **existing engines** to work with tables stored in the data lake.

### Structure of an Iceberg table – three layers

```text
1. CATALOG   → finds the table; points to the table's CURRENT metadata file
        ↓
2. METADATA  → metadata file  (schema, partitioning, snapshots)
               └ snapshot     (state of the table at a point in time)
                  └ manifest list  (identifies the manifest files of a snapshot)
                     └ manifest files (list the data files + info about them)
        ↓
3. DATA      → data files (e.g. Parquet) holding the actual rows
```

#### Analogy
A **book**: the **catalog** is the library card pointing to the book; the **metadata file** is the table of contents; a **snapshot** is a specific edition; **manifest lists/files** are the index pointing to pages; the **data files** are the pages.

### Six features

#### 1. Row-level operations (UPDATE and DELETE)

| Copy-on-Write (COW) | Merge-on-Read (MOR) |
|---|---|
| When a row changes, the **entire data file containing it is rewritten** with updated data: `[A B C D] → [A B C' D]` | The change is **written separately to a new file** and **merged with the original at read time**: `[A B C D] + [C']` → read/merge |
| **More data written; simpler, faster reads** | **Less data written; extra work/latency during reads** |

COW and MOR are **complementary**: trade **write latency vs read performance** based on the workload.

**Analogy:** a typo on a printed page. **COW** = reprint the whole page. **MOR** = stick a **correction slip** next to it; readers combine page + slip when reading.

#### 2. Hidden partitioning
Iceberg stores the **partition spec in metadata**, hiding the physical layout from the user.

```sql
-- With hidden partitioning: query the logical column
SELECT * FROM events
WHERE event_timestamp >= '2026-08-10' AND event_timestamp < '2026-08-11'

-- Without: user must add physical partition columns
SELECT * FROM events
WHERE event_year = 2026 AND event_month = 8 AND event_day = 10 AND event_timestamp >= ...
```

**Benefit:** users don't need physical columns like `day(event_timestamp)`; DBAs can **evolve partitioning without breaking queries**. **Drawback of the alternative:** queries are complex, tightly coupled, and break if partitioning changes.

#### 3. Partition evolution
Partitioning can **change without rewriting existing data**:

```text
Older data → PARTITION BY month(date)
Newer data → PARTITION BY day(date)
```
Queries keep using the same logical column; Iceberg applies the right partition info to each part. Without it, changing partitioning means **rewriting/repartitioning all existing data (very expensive)** or keeping the old scheme and losing performance.

#### 4. Snapshots (time travel and rollback)
Each committed state is a **snapshot**.

```text
Snapshot 1 (1000 orders) → +50 new orders → Snapshot 2 (1050 orders)
Later: the 50 orders were wrong.
Time travel : SELECT * FROM table AS OF snapshot_1     (read old state)
Rollback    : ROLLBACK TABLE table TO snapshot_1        (restore current state)
```
No separate copy of the table is needed. **Analogy:** Google Docs **version history**: view an old version or restore it.

#### 5. Schema evolution
The schema can change without rebuilding data.

```text
orders(order_id, customer_id, amount)  → add column →  orders(order_id, customer_id, amount, discount)
```
Existing records stay as they are; only the schema/metadata evolves.

#### 6. Reliable updates (atomic commits / ACID)
Changes are **committed atomically**, so readers see a **consistent** state.

```text
Before update (State A) → update in progress → After commit (State B)
Readers see either State A or State B, never a half-updated table.
```
Concurrent writers do not corrupt the table. **Analogy:** an ATM transfer either fully completes or doesn't happen.

### Iceberg exam traps
- Iceberg is a **table format**, **not** a storage system or compute engine.
- Iceberg layers: **Catalog → Metadata → Data**.
- **COW = more write, faster read; MOR = less write, slower read.**
- Snapshot **time travel reads** an old state; **rollback restores** it.

---

## 12B. Apache Hudi

### Why Hudi?
Parquet stores data efficiently, but it is a **file format, not a database**. Hudi **manages the data-lake files as a table** by adding table semantics.

| Hudi tracks / manages | Hudi enables |
|---|---|
| **Record keys** | **Upsert** and **delete** records |
| **Commits** | **Atomic commits** |
| **Updates and deletes** | **Time travel** |
| **Table history** | **Rollback** |
| **Incremental access** | Incremental queries |

> Hudi does not replace the data lake; it adds a layer that gives the files **table semantics**.

### Key concepts

#### Record key
To update or delete one record Hudi must **uniquely identify** it. The **record key** (e.g. `recordKey = order_id`) is like a **primary key**, used specifically to identify records during writes. **In Hudi, record identity is not optional.**

#### preCombineField and merge modes ("which record wins?")
When multiple versions of the same key arrive, Hudi needs a **merge rule**. Hudi 1.0.2 offers **three merge modes**:

| Mode | Winner |
|---|---|
| **COMMIT_TIME_ORDERING** | The record from the **later Hudi commit** wins |
| **EVENT_TIME_ORDERING** | The record with the **greater preCombineField value** (e.g. `update_at`) wins |
| **CUSTOM** | A user-provided merger defines the rule |

**Example:** current record for `order_id = 1001` has `update_at = 2000`; an incoming one has `update_at = 500`.
- **EVENT_TIME_ORDERING** → keeps the **2000** record (greater preCombineField).
- **COMMIT_TIME_ORDERING** → keeps the **500** record (it is the later commit).

#### Write operations
1. **Insert:** add a record whose key does not exist (e.g. 1004, Dave, PLACED).
2. **Upsert:** insert if key does not exist, **otherwise merge** with the existing record per the merge rules (e.g. 1001 → ALICE, COMPLETED, 2000).
3. **Delete:** remove a record from the **current** table state while **preserving the operation in the commit history**.

#### Table types

| Copy-on-Write (CoW) | Merge-on-Read (MoR) |
|---|---|
| Updates **modify the existing base files**: a **new version of the affected file** is written | Updates are first written to **delta log files** instead of rewriting base files |
| Reads are simpler (latest data already in base files) | During a read, Hudi **combines base files + log files** |
| **Tradeoff:** updates can write **more data** | **Tradeoff:** writes cheaper, reads need **extra merging** |

Both provide the **same table-level features**; they differ in how updates are stored and served.

#### Timeline and commits
- Every change is an **event on the Hudi timeline** (stored in the **`.hoodie`** metadata directory).
- A **commit = a completed write operation** (each has a **unique commit time**). **A commit ≠ a changed row**; not every incoming record necessarily changed the final state.
- The timeline gives the **sequence of changes** and which table state is currently valid.

#### Time travel and incremental queries
- **Time travel:** query the table **as it was at an earlier commit instant**. It **does not undo or modify the table**; it only changes which point of history you read.
- **Incremental query** (`hoodie.datasource.query.type=incremental`): returns only records from **commits after a given instant** instead of reconstructing the whole table.

#### Delete and rollback
- **Delete** creates a **new commit**; the record is gone from the current state but **earlier versions still contain it**, so time travel can show it.
- **Rollback** **reverses a completed commit** and restores the table to the state before that commit (operates on a specific commit).

#### Compaction, indexing, metadata table
- **Compaction (MoR):** merges accumulated **delta log files into new base files**.
- **Index:** maps **record keys → file locations**, so updates/deletes can find the file containing a record.
- **Metadata table:** stores file listings, record/file metadata and **column statistics**, so Hudi doesn't repeatedly scan data files.

#### APIs
Integrates with **Apache Spark** (datasource and Spark SQL extensions: create/read/update/delete/query), plus table-management APIs (including REST) that work on the **table abstraction** rather than raw Parquet files.

#### Analogy
Hudi is a **school register with roll numbers**. The **record key** = roll number; if two updates for the same student arrive, the **preCombineField** (latest timestamp) decides which entry wins; the **timeline** is a **git log** of every change; **MoR compaction** is periodically merging **sticky-note corrections** into the main register; an **incremental query** is "show me only what changed since yesterday's commit".

---

## 12C. Iceberg vs Hudi

| Aspect | **Apache Iceberg** | **Apache Hudi** |
|---|---|---|
| **Mental model of a table** | A **sequence of snapshots**; thinks in terms of **which files make up the table at this version**; **no record identity required** (many tables are pure append/overwrite) | A **sequence of commits** where **every row has an identity** (primary key/record key is not optional); maintains a **timeline (.hoodie)** of every action |
| **Update (COW)** | Rewrite the file containing the changed row | Same idea: rewrite the base Parquet file |
| **Update (MOR)** | **Newer addition**: small **delete file** says "ignore this row in that file"; engine merges at read time | **Original signature design**: changes go to a **row-based delta log** next to the base file; **background compaction** later merges into a new base file |
| **Concurrency / conflicts** | **Optimistic concurrency at snapshot level**: two writers race for the next snapshot; first commit wins, loser retries | Resolves at the **record level** using **preCombineField** (keeps the greater value, e.g. later `updated_at`, regardless of write order) |
| **Time travel** | Query by **snapshot ID or timestamp**; natural because the table is snapshot-versioned | Query **as-of a commit instant**; also **true incremental reads** returning only rows changed since a commit |
| **Origin** | **Netflix**, 2017 (open-sourced 2018) | Timeline/commit-oriented design |

**One-liner:** *Iceberg thinks in snapshots of files; Hudi thinks in commits of records.*

---

## 13. Apache Druid

### 13.1 Definition
Druid is an **open-source, column-oriented database purpose-built for fast, ad-hoc analytical (OLAP) queries on event-driven data**. It blends ideas from **data warehouses, time-series databases and search systems**, and answers "**how many / how much / trending how**" questions over huge, constantly arriving datasets in **sub-second** time.

**Not** a general-purpose OLTP database: **no arbitrary multi-table joins** and **no row-level transactional updates**.

### 13.2 Why Druid? (vs traditional approaches)

| Traditional | Limitation | Druid's answer |
|---|---|---|
| **Data warehouses** | Powerful SQL, but seconds-to-minutes latency; freshness limited by batch cycles | **Sub-second aggregation**, even across trillions of rows; **native streaming ingestion** (queryable within seconds) |
| **OLTP databases** | Fast row-level writes, but not built to scan/aggregate billions of rows | **Time-partitioned segments pruned per query**: only relevant data is scanned |
| **Search engines** | Fast filtering, weak at numeric aggregation and rollups | Optional **roll-up** pre-aggregates raw events at ingest time |

### 13.3 Data organisation

```text
Datasource  (≈ table)
   └── Time chunk   (e.g. 1 hour or 1 day)
          └── Segment  (immutable, columnar file)
                 └── compressed columns + indexes
```

- A **segment** is the **fundamental unit of storage**: **immutable**, **columnar**; each column is **stored, compressed and indexed separately**; **bitmap indexes on dimension columns** make filtering very fast.
- Column types in a segment: **timestamp**, **dimensions** (e.g. `country`, dictionary + bitmap), **metrics** (e.g. `bytes_sent`, compressed).
- Once published, segments move to **deep storage (S3, HDFS…)** and are handed off to **Historicals**.

### 13.4 Architecture – three server types

| Server type | Processes | Responsibility |
|---|---|---|
| **Master** | **Coordinator, Overlord** | Manage data availability; assign ingestion workloads |
| **Query** | **Broker, Router** | Handle and route queries from external clients |
| **Data** | **Historical, MiddleManager** | Store queryable segments and execute ingestion tasks |

| Process | Role |
|---|---|
| **Coordinator** | Manages **data availability**: **assigns, balances and drops segments** on Historicals |
| **Overlord** | Controls **assignment of ingestion workloads** and coordinates **segment publishing** |
| **Broker** | Receives external queries, **prunes to relevant segments**, forwards sub-queries, **merges results** |
| **Router** | **Optional API gateway** in front of Brokers, Coordinators, Overlords; hosts the **web console** |
| **Historical** | **Downloads immutable segments from deep storage** and serves queries against them |
| **MiddleManager** | Runs **ingestion tasks**, indexes **streaming data in real time** through per-task **Peon** processes |

Rules: **query servers never touch storage directly**; **data servers do the scanning**; **master servers coordinate but never touch query traffic or raw data**.

### 13.5 Ingestion

| Streaming | Batch |
|---|---|
| A **supervisor** manages MiddleManager tasks that read continuously from Kafka or a streaming service | **Native batch** or **Hadoop-based** ingestion loads a bounded dataset in one job |
| Data queryable **within seconds**; **exactly-once** semantics | Used for **historical backfills and reprocessing**; can **reindex** segments to change granularity/schema |
| Segments **handed off automatically** as time chunks close | **No continuous supervisor**; the job runs once and completes |

### 13.6 How a query executes

```text
1. Query (SQL or native JSON) enters via the Router or directly at the Broker.
2. Broker PRUNES the segment list: first by TIME, then by other partitioning attributes.
3. Broker finds which Historicals and MiddleManagers serve those segments and forwards a re-written sub-query to each.
4. Each data-server process scans only its LOCAL segments and returns partial results.
5. Broker MERGES all partial results into the final answer and returns it.
```

### 13.7 Analogy
Druid is a **live sports scoreboard/analytics desk**. Every event (a goal, a click) is written into **time-stamped, sealed folders (segments)** grouped by hour. The **Broker** is the receptionist who knows which archive room has which hour; **Historicals** are archive rooms; **MiddleManagers** are the desk taking in **live** events; the **Coordinator** decides which room stores which folder; the **Overlord** manages the intake jobs. Because each folder is **column-organised and indexed**, questions like "how many clicks from India in the last hour?" are answered almost instantly.

### 13.8 Exam traps
- Druid = **OLAP on event/time-series data**, **not OLTP**; **no arbitrary joins/row updates**.
- **Segment = immutable columnar file**; hierarchy **datasource → time chunk → segment**.
- **Broker merges**, **Historical serves**, **MiddleManager ingests**, **Coordinator assigns segments**, **Overlord assigns ingestion tasks**.
- Query path: **time pruning first**.

---