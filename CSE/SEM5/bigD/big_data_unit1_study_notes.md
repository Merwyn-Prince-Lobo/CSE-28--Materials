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
