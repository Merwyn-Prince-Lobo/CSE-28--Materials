# DBMS Units 3 & 4: Exam-Oriented Study Notes

_Built from your lecture decks (DBMS_unit3.pdf and DBMS_unit4.pdf, PES University, Dr. Nagasundari S) in the exact syllabus order. Material I added to complete a syllabus topic that the slides explain thinly is tagged_ **_[Supplement]_** _. Everything else comes from your slides. Where a slide contained an error that the deck itself later corrected, the corrected version is used._

## 0. Exam Blueprint and How to Use These Notes

|Section|Marks<br>each|Count|Total|From each<br>unit|
|---|---|---|---|---|
|Descriptive questions|4|4|16|2|
|MCQ (2-mark, usually numeric or<br>application)|2|4|8|2|
|MCQ (1-mark, usually recall or concept)|1|16|16|8|
|**Total**|||**40**||

**Legend used throughout: Key** = must memorise. **Trap** = a common MCQ trap. **Shortcut** = a method that saves exam time.

**Suggested study order:** (1) read the unit notes, (2) memorise the Definitions and Revision Sheet, (3) do the 4-mark questions by writing them out, (4) do the MCQs, (5) attempt the Mock Exam at the end without looking at the answer key.

#### **Contents**

- **UNIT 3: Design Concepts and Implementation** : 3.1 Query Processing, 3.2 Query Optimization, 3.3 Functional Dependencies, 3.4 Inference Rules and Closure, 3.5 Equivalence, Minimal Cover and Keys, 3.6 Informal Design Guidelines and Anomalies, 3.7 Normal Forms (1NF, 2NF, 3NF), 3.8 General Definitions, BCNF and Decomposition, 3.9 Higher Normal Forms (4NF, 5NF), 3.10 Worked Numericals, 3.11 Comparison Tables, 3.12 Likely 4-mark Questions, 3.13 Definitions and Formulae, 3.14 Practice MCQs, 3.15 Revision Sheet

- **UNIT 4: Next-Gen Data Management** : transactions, serializability, concurrency control, locking, NoSQL, key-value (Redis), graph (Neo4j), vector databases, vector search (relational and NoSQL), Next.js with SQL

**Mock Examination (40 marks)** and **Answer Key with Model Answers**

**Syllabus Coverage Cross-Check**

# UNIT 3: Design Concepts and Implementation

**Syllabus:** Introduction to strategies of Query processing and Query optimization; Functional Dependencies; Inference Rules; Normal Forms Based on Primary Keys (1NF, 2NF, 3NF); General Definitions of Second and Third Normal Forms; Boyce-Codd Normal Form; Higher Normal Forms.

## 3.1 Introduction to Query Processing

### 3.1.1 What is query processing?

**Query processing** is the set of activities that turns a high-level query (SQL) into low-level operations that the physical storage layer can execute, picks an efficient way of executing it, and finally runs it to produce the answer. It includes (a) **translation** of the query into an internal form, (b) **query optimization** , and (c) **execution** .

SQL is _declarative_ : it says **what** is wanted, not **how** to get it. The DBMS must decide the "how". That decision is the whole point of this topic.

### 3.1.2 Pipeline of query processing

```
 SQL text
    |
    v

 [Scanner]  ---> identifies tokens (keywords, attribute names, relation
names)
    |
    v
 [Parser]   ---> checks syntax against the grammar of SQL
    |
    v

 [Validator]---> checks that relation/attribute names exist in the schema and
make sense
    |
    v
 Internal representation: QUERY TREE (or QUERY GRAPH, a DAG)
    |
    v
 [Query Optimizer] ---> chooses a good execution strategy / execution plan
    |
    v
 [Code Generator]  ---> generates code to execute that plan
    |
    v
 [Runtime Database Processor] ---> runs the code, produces the query RESULT
```

|Module|Task|
|---|---|
|Scanner|Identifies the**tokens**in the query text: SQL<br>keywords, attribute names, relation names|
|Parser|Checks**syntax**according to the rules of the query<br>language|
|Validation|Checks all attribute and relation names are**valid**<br>**and semantically meaningful**in the database<br>schema|
|Query optimizer|Produces a**good execution plan**(execution<br>strategy)|
|Code generator|**Generates code**to execute the plan|
|Runtime database processor|**Executes**the code and produces the result|

**Key:** The internal representation is usually a **query tree** (a tree data structure). It can also be a **query graph** , which is generally a **directed acyclic graph (DAG)** . A query has many possible execution strategies; **choosing a suitable one is called query optimization.**

**Trap:** Scanner = tokens; Parser = syntax; Validator = schema/semantic check. Do not swap these.

### 3.1.3 Translating SQL into relational algebra: query blocks

An SQL query is first translated into an **extended relational algebra expression** , represented as a **query tree** , and then optimized. SQL queries are decomposed into **query blocks** , the basic units that can be translated into algebraic operators and optimized.

- A **query block** contains a single **SELECT-FROM-WHERE** expression, and may also contain **GROUP BY** and **HAVING** clauses.

- A **nested query** has an **inner block** and an **outer block** . The optimizer chooses an execution plan for **each query block** .

#### **Example (textbook example used in the slides):**

```
SELECT Lname, Fname
FROM   EMPLOYEE
WHERE  Salary > ( SELECT MAX(Salary)          -- inner block
                  FROM   EMPLOYEE
                  WHERE  Dno = 5 );
```

- Inner block: **ℱ MAX(Salary) ( σ Dno=5 (EMPLOYEE) )** , which returns a single value, called _c_ in the slides.

- Outer block: **π Lname, Fname ( σ Salary > c (EMPLOYEE) )** .

### 3.1.4 Additional operators: semi-join and anti-join

Most modern SQL engines (MySQL, PostgreSQL and others) internally convert `EXISTS` / `NOT EXISTS` / `IN` subqueries into **semi-joins** and **anti-joins** . This avoids redundant joins and large intermediate results.

||**Semi-join**(written<br>`T1.X S=`<br>`T2.Y`)|**Anti-join**(written<br>`T1.x A=`<br>`T2.y`)|
|---|---|---|
|Used to unnest|`EXISTS`,<br>`IN`,<br>`ANY`|`NOT EXISTS`,<br>`NOT IN`,<br>`ALL`|
|T1 row is**returned**<br>when|it finds a match in T2|it finds**no**match in T2|
|T1 row is**rejected**<br>when|no match is found|it finds a match with any T2 value|
|Search behaviour|Stops at the**first**match|Rejects as soon as one match is<br>found|

||**Semi-join**(written<br>`T1.X S=`|**Anti-join**(written<br>`T1.x A=`|
|---|---|---|
||`T2.Y`)|`T2.y`)|
|Duplicates|Left row appears**once**|Left row appears at most once|

**Key:** Inner join returns _every_ matching pair (a row of T1 matching 3 rows of T2 gives 3 result rows). A semi-join returns the T1 row **once** (1 result row). This duplicate-free behaviour is exactly why optimizers map `EXISTS` to a semi-join.

**Trap:** The semi-join result contains columns of the **left table only** .

### 3.1.5 Algorithms for the SELECT operation

SELECT is basically a **search operation** that locates records in a disk file satisfying a condition. If the algorithm scans the file it is a **file scan** ; if it uses an index it is an **index scan** .

Slide examples: **OP1:** `SELECT * FROM EMPLOYEE WHERE Ssn = '123456789'` and **OP2:** `SELECT * FROM DEPARTMENT WHERE Dnumber > 5` .

|Code|Method|When it applies|Records<br>retrieved|Note|
|---|---|---|---|---|
|**S1**|**Linear search**<br>(brute force)|Always applicable; no<br>assumption about<br>ordering or index|Any<br>number|Read every block into a<br>buffer, test each<br>record in memory; cost<br>grows linearly (O(n))|
|**S2**|**Binary search**|**Equality**on a**key**<br>attribute and the file is<br>**ordered**on that<br>attribute|At most 1|More efficient than<br>linear (OP1 if Ssn is the<br>ordering attribute)|
|**S3a**|**Primary index**|**Equality**on a key<br>attribute that has a<br>primary index|At most 1|e.g. Ssn = '123456789'|
|**S3b**|**Hash key**|**Equality**on a key<br>attribute with hashing|At most 1|Same example|

|Code|Method|When it applies|Records<br>retrieved|Note|
|---|---|---|---|---|
|**S4**|**Primary index**<br>**for multiple**<br>**records**|Comparison<br>`>`,<br>`>=`,<br>`<`,<br>`<=`on a key with a<br>primary index|Many|Find the record<br>satisfying the equality,<br>then read all<br>**subsequent**records in<br>the ordered file; for<br>`<`<br>or<br>`<=`, read all<br>**preceding**records<br>(OP2: Dnumber > 5)|

The slides also note there are further methods for **conjunctive** (AND) and **disjunctive** (OR) selection conditions.

**Shortcut for scenario questions:** equality on key + primary index → **S3a** ; range on ordered indexed key → **S4** ; no index, unordered file → **S1** .

### 3.1.6 Implementing the JOIN operation

JOIN is **one of the most time-consuming operations** in query processing. Most joins in practice are **EQUIJOIN** or **NATURAL JOIN** . A join on two files is a **two-way join** ; a join on more than two files is a **multiway join** .

**Running example from the slides:** join condition `Employee.DeptID = Department.DeptID` .

|Code|Algorithm|How it works|Needs|Approx. cost<br>idea|
|---|---|---|---|---|
|**J1**|**Nested-loop**<br>**join**(nested-<br>block join)|For each record_t_of<br>the**outer**relation R,<br>scan**every**record_s_<br>of inner relation S<br>and test<br>`t[A] =`<br>`s[B]`|Nothing (default<br>brute force)|About |R| × |S|<br>record accesses|
|**J2**|**Index-based**<br>**nested-loop join**|For each outer tuple,<br>use an**index on the**<br>**join attribute of the**<br>**inner relation**to find<br>matches directly|Index on inner join<br>attribute|About |R| ×<br>log|S||

|Code|Algorithm|How it works|Needs|Approx. cost<br>idea|
|---|---|---|---|---|
|**J3**|**Sort-merge join**|Sort both relations<br>on the join<br>attributes (if not<br>already sorted),<br>then scan both**once**<br>in step, merging<br>matching values<br>(like the merge step<br>of merge sort)|Both files sorted on<br>join attributes|Sorting cost +<br>one pass over<br>each file|
|**J4**|**Partition-hash**<br>**join**|Hash both files with<br>the**same hash**<br>**function**on the join<br>attribute into<br>partitions, then join<br>matching partitions|Enough memory for<br>smaller file's<br>partitions (simple<br>version)|Roughly one<br>pass over each<br>file|

#### **Partition-hash join, two phases (important):**

1. **Partitioning phase:** make one pass through the file with **fewer records** (say R) and hash each record into buckets of an in-memory hash table using h(A).

2. **Probing phase:** make one pass through the other file S; hash each record with the **same** h(B) to **probe** the matching bucket; combine it with all matching records of R in that bucket.

The simple version **assumes the smaller file fits entirely in memory** after partitioning. _Slide example:_ hash both tables on CID using `CID % 3` → Partition 1 holds {O1 (C1), C1}, Partition 2 holds {O2 (C2), C2}, Partition 3 holds {O3 (C3), C3}; each partition pair is then joined in memory.

**Comparison (J1 to J4):**

|Feature|Nested-loop|Index-based NL|Sort-merge|Partition-hash|
|---|---|---|---|---|
|Special access<br>path needed|No|**Index on inner**|**Sorted**inputs|Hash function<br>(and memory)|

|Feature|Nested-loop|Index-based NL|Sort-merge|Partition-hash|
|---|---|---|---|---|
|Handles non-<br>equality joins|Yes|Mostly equality|Equality<br>(ordering<br>based)|**Equality only**|
|Inner relation<br>scanned|Fully, for each<br>outer tuple|Never fully<br>(index lookups)|Once|Once (probe)|
|Best when|Tables tiny, no<br>indexes|Index exists on<br>inner join<br>attribute|Inputs already<br>sorted|Large unsorted<br>inputs, equality<br>join|

**Trap:** In J2 the index must be on the **inner** relation's join attribute, not the outer. In J4 the **smaller** file is the one partitioned first (build side) and the **other** file probes.

**[Supplement] Why hashing and sort-merge need an equality join:** both rely on equal join values landing in the same bucket (hash) or in adjacent positions (sort). A condition like `R.A < S.B` cannot be handled this way.

### 3.1.7 External sorting

**External sorting** is commonly needed in query processing when relations are too large for memory. It is used to order tuples for **ORDER BY** , **aggregation** and **duplicate elimination** , and as the first step of **sort-merge join** . **[Supplement]** Method: _sort phase_ (sort memorysized runs and write them to disk) followed by _merge phase_ (merge the sorted runs).

### 3.1.8 Summary of Query Processing

SQL is translated to relational algebra (query blocks) → nested queries mapped to **semijoin/anti-join** → selection implemented with S1 to S4 (and conjunctive/disjunctive variants) → joins implemented with J1 to J4 → external sorting for ORDER BY / aggregation / duplicate elimination.

## 3.2 Introduction to Query Optimization

### 3.2.1 What and why

**Query optimization** is the process of improving the efficiency of SQL queries by choosing a good execution plan among many equivalent ones. **Goal:** minimise execution time and maximise performance. **Why it matters:** large tables make poorly written queries slow; efficient queries save **CPU, memory and I/O** , improve user experience and scalability.

### 3.2.2 Common causes of slow queries

|Cause|Effect|
|---|---|
|**Missing or unused indexes**|Full table scan instead of an index lookup|
|**SELECT ***|Fetches unnecessary columns, raises data<br>transfer time|
|**Complex joins and nested subqueries**|Multiple table scans, more computation and<br>memory|
|**Functions/expressions in the WHERE**<br>**clause**|Prevent index usage, force full table scan|
|**Large unfiltered data sets**|Processing a huge number of rows|

### 3.2.3 Key optimization techniques

1. **Use indexes wisely** : speeds up searches and joins.

2. **Retrieve only needed columns** : reduces transfer and processing.

3. **Optimize joins** : join only the required tables, on indexed attributes.

4. **Filter early using WHERE** : limit rows before later operations.

5. **Use LIMIT for large results** : fetch only the needed records.

### 3.2.4 [Supplement] Heuristic (rule-based) optimization on the query tree

The optimizer rewrites the query tree using rules that nearly always reduce the size of intermediate results:

1. **Push selections (σ) down** the tree: filter rows as early as possible.

2. **Push projections (π) down** : discard unneeded columns early.

3. Combine a **Cartesian product followed by a selection** into a **join** .

4. **Reorder joins** so the most restrictive (smallest-result) join runs first.

5. A **cost-based** optimizer then compares estimated costs (disk blocks read, CPU) of candidate plans and picks the cheapest.

### 3.2.5 Using EXPLAIN (MySQL)

`EXPLAIN` placed before a query shows **how MySQL will execute it** : the chosen plan, the **order in which tables are accessed** , and **which indexes are used** . It helps find bottlenecks.

```
EXPLAIN SELECT column1, column2 FROM table_name WHERE condition;
```

#### **Important output columns:**

|Column|Meaning|Values to recognise|
|---|---|---|
|`type`|Access method|`ALL`=**full table scan**(worst);<br>`range`= index range<br>scan;<br>`ref`= index lookup on non-unique index;<br>`const`/<br>`eq_ref`= best|
|`key`|Index actually used|`NULL`=**no index used**|
|`rows`|Estimated rows examined|Compare with rows returned|
|`Extra`|Additional info|`Using temporary`= temporary table needed;<br>`Using index condition`= filter pushed into index|

**Slide Example 2 (orders table):** `SELECT customer_id, SUM(total_amount) FROM orders GROUP BY customer_id;`

- Before indexing: `type = ALL` , `key = NULL` , `Extra = Using temporary` → full scan plus a temporary table.

- Fix: `CREATE INDEX idx_customer ON orders(customer_id);` → `key =`

- `idx_customer` , `Extra = NULL` . The temporary table disappears because the grouping column is already read in order.

- Totals returned are 250.00 and 320.00 (customer 1: 100+150; customer 2: 200+120 per the data shown).

**Trap (slide correction):** The original slide wrongly claimed an index was used before `CREATE INDEX` . Correct reading: **`type = ALL` and** **`key = NULL` mean no index** .

**Example 1 (HeatWave):** In the `airportdb` example, `EXPLAIN` is used to check whether a query can be **offloaded to MySQL HeatWave** (the secondary engine). Running with `use_secondary_engine` disabled compares HeatWave time against normal DB-system time.

### 3.2.6 Indexing: with and without

The best way to speed up SELECT is to create **indexes on the columns tested in the query** . Index entries act like **pointers** to table rows.

||Without index|With index|
|---|---|---|
|Method|**Full table scan**: every record is|**B-tree**index jumps directly to qualifying|
||checked|records|
|Cost|**O(n)**(linear)|**O(log n)**|
|`EXPLAIN`|`type = ALL`,<br>`key = NULL`, rows ≈|`type = range`or<br>`ref`,<br>`key = idx_...`,|
||whole table|fewer rows|

#### **Slide measurements:**

- `SELECT * FROM Employee WHERE salary > 50000;` with 6 rows: 3 rows qualify.

- Before index: `rows = 6` , `type = ALL` . After `CREATE INDEX idx_salary ON Employee(salary)` : `type = range` , `rows = 3` , `Extra = Using index condition` . Million-row `customers` table, `WHERE customer_id = 12345` : **0.68 s without index, 0.00 s with index** ( `type` becomes `ref` , `rows = 1` ).

**Non-sargable predicate (slide question answered):** `WHERE salary + 100 > 5000` does **not** use `idx_salary` because the indexed column is wrapped in an **expression** , so MySQL cannot map it onto ordered index entries; it falls back to `type = ALL` . **Rewrite** as `WHERE salary > 4900` to keep the column bare and restore index use.

**Key:** Keep the indexed column **bare** on one side of the comparison. Functions or arithmetic on the column defeat the index.

## 3.3 Functional Dependencies (FDs)

### 3.3.1 Why we need them: design quality

So far, grouping attributes into relations relied on the designer's common sense. We need **formal measures of "goodness"** of a relation schema. Quality can be discussed at two levels:

- **Logical (conceptual) level:** how users interpret the relation and its attributes. **Implementation (physical storage) level:** how tuples are stored and updated.

FD theory mainly supports **top-down design (design by analysis)** : analyse and decompose sets of attributes that appear together in forms, files and reports. The **implicit goals** are **information preservation** and **minimum redundancy** . The **universal relation schema** R = {A1, A2, ..., An} is only a **theoretical device** for developing the theory; the database is not actually stored as one table.

### 3.3.2 Definition

A **functional dependency X → Y** between two sets of attributes X and Y (subsets of R) is a **constraint** on the possible tuples of any relation state r(R): for any two tuples t1 and t2 with **t1[X] = t2[X]** , we must have **t1[Y] = t2[Y]** .

In words: **X functionally determines Y** if and only if whenever two tuples agree on X they necessarily agree on Y. X is the **determinant** (left side); Y is the **dependent** (right side).

#### **Key facts:**

- If **X is a candidate key** of R then **X → R** (X determines every attribute); more generally X → Y for any subset Y.

- **X → Y does not imply Y → X.**

- An FD is a property of the **semantics (meaning)** of the attributes. It is derived from real-world constraints and must hold in **every** legal relation state.

- States that satisfy all FDs are called **legal relation states (legal extensions)** . FDs are used to specify the "goodness" of designs and to **define normal forms** .

**Examples (COMPANY database):** `SSN → ENAME` ; `PNUMBER → {PNAME, PLOCATION}` ; `{SSN, PNUMBER} → HOURS` .

**Diagrammatic notation:** each FD is drawn as a **horizontal line** ; the left-side attributes are connected to it by vertical lines; arrows point from the line to the right-side attributes.

### 3.3.3 Instance can disprove an FD but never prove it

A single pair of tuples agreeing on X but differing on Y **refutes** an FD permanently. But the current data can only show that an FD **"may hold"** , because an FD is a constraint on **all legal states** , which come from meaning, not from the currently stored rows.

_Slide Example (EMP):_ `Department → Salary` does **not** hold because department HR appears with salaries 50000 and 55000.

_Slide Example (TEACH):_ Only `Text → Course` may exist. `Teacher → Course` , `Teacher → Text` , `Course → Text` are **ruled out** (for example Smith teaches two different courses). Also `Course → Teacher` is refuted by (Smith, Data Structures) and (Brown, Data Structures).

_Slide Example R(A, B, C, D) with 4 tuples:_ FDs that **may hold** : B → C; C → B; {A,B} → C; {A,B} → D; {C,D} → B. FDs that **do not hold** : A → B; B → A; D → C.

**SQL test for X → Y (slide):** `SELECT X FROM R GROUP BY X HAVING COUNT(DISTINCT Y) > 1;` An **empty result means the FD holds in the current data** ; any returned row is a counterexample.

### 3.3.4 Prime and non-prime attributes

**Prime attribute:** belongs to **some candidate key** of R.

**Non-prime attribute:** does **not** belong to any candidate key.

In WORKS_ON(Ssn, Pnumber, Hours), **Ssn and Pnumber are prime** ; Hours is non-prime.

### 3.3.5 Types of functional dependency

|Type|Definition|Example|
|---|---|---|
|**Full FD**|X → Y where**removing any attribute**<br>from X makes the FD fail; for every A<br>in X, (X − {A}) does**not**determine Y|{Ssn, Pnumber} → Hours (neither<br>Ssn → Hours nor Pnumber → Hours<br>holds). ABC → D where no proper<br>subset determines D|
|**Partial FD**|X → Y where**some attribute can be**<br>**removed**from X and the FD still<br>holds; a**non-prime**attribute<br>depends on**part of a candidate key**|{Ssn, Pnumber} → Ename, because<br>Ssn → Ename alone holds|

|Type|Definition|Example|
|---|---|---|
|**Transitive**<br>**FD**|X → Y and Y → Z give X → Z; a**non-**<br>**prime**attribute depends on the key<br>**indirectly through another non-**<br>**prime attribute**|Ssn → Dnumber and Dnumber →<br>Dmgr_ssn give Ssn → Dmgr_ssn. In<br>StudentID → Dept → DeptHead,<br>StudentID → DeptHead is transitive|
|**Trivial FD**|Y⊆X (holds by the**reflexive rule**,<br>always true)|ABC → BC; {A} → {A}|
|**Non-trivial**|Y⊄X (Y has at least one attribute|AB → C|
|**FD**|not in X); carries new information||

**Trap:** {A,B,C} → {B,C}, {A,B} → {A} and {A} → {A} are all **trivial** . {A,B} → {C} is **non-trivial** . Also **SSN → ENAME is non-transitive** , because there is no X with SSN → X and X → ENAME.

**Trap:** Partial dependency needs a **composite** key. If the primary key is a single attribute, partial dependency is impossible, so 2NF is automatically satisfied.

## 3.4 Inference Rules for FDs and Closure

### 3.4.1 Implied FDs

An FD X → Y is **inferred from (implied by)** a set F of FDs if X → Y holds in **every** legal relation state that satisfies F.

_Example:_ each department has one manager (Dept_no → Mgr_ssn) and a manager has one phone (Mgr_ssn → Mgr_phone) ⇒ **Dept_no → Mgr_phone** is implied and need not be stated.

### 3.4.2 Armstrong's inference rules

|Rule|Name|Statement|Example|
|---|---|---|---|
|**IR1**|**Reflexive**|If Y⊆X then X → Y|{A,B} → {A}|
|**IR2**|**Augmentation**|If X → Y then XZ → YZ|Ssn → Ename gives {Ssn, Bdate} →<br>{Ename, Bdate}|

|Rule|Name|Statement|Example|
|---|---|---|---|
|**IR3**|**Transitive**|If X → Y and Y → Z then|Ssn → Dnumber and Dnumber → Dname|
|||X → Z|give Ssn → Dname|

**Key:** IR1, IR2, IR3 together are **sound** (every FD derived actually holds in every relation satisfying F) and **complete** (every FD that logically follows from F can be derived). They are called **Armstrong's axioms** .

**Additional (derived) rules** , all deducible from IR1 to IR3:

|Rule|Statement|
|---|---|
|**Decomposition (projective)**|If X → YZ then X → Y and X → Z|
|**Union (additive)**|If X → Y and X → Z then X → YZ|
|**Pseudo-transitivity**|If X → Y and WY → Z then WX → Z|

**Trap (cautionary note in the slides):**

- X → A and X → B imply X → AB (union) is **valid** .

- X → A and **Y → B** do not give a split; what is valid is that they imply **XY → AB** (augmentation + union).

- **XY → A does NOT necessarily imply X → A or Y → A.** A composite determinant cannot be split in general.

**Shortcut for "derive" questions:** write each step with its justification (Given, Augmentation, Transitivity, Union, Decomposition). Marks go to the justified steps.

### 3.4.3 Closure concepts

Closure helps to **identify keys** , **test normalization** and **check whether X → Y is implied by F** .

|Closure|Symbol|Meaning|
|---|---|---|
|**Attribute closure**|X⁺|Set of**all attributes**functionally determined by X<br>under F. If X⁺ = R, X is a**superkey**|
|**FD-set closure**|F⁺|Set of**all FDs logically implied by F**(using<br>Armstrong's axioms)|

_Example:_ F = {A → B, B → C}: A⁺ = {A, B, C}; F⁺ contains A → B, B → C, A → C, AB → C, and many more.

**Size of F⁺:** a set of n attributes has 2ⁿ subsets, so there are at most **2ⁿ × 2ⁿ = 2^(2n)** possible FDs. (Hence computing F⁺ directly is expensive; we use X⁺ instead.)

### 3.4.4 Algorithm: closure X⁺ of attribute set X under F

```
Input : set F of FDs on R, and X ⊆ R
Output: X+
X+ := X
repeat
    oldX+ := X+
    for each FD  Y -> Z  in F do
        if  X+ ⊇ Y  then  X+ := X+ ∪ Z
until (X+ = oldX+)
```

**Test whether X → Y is implied by F:** compute X⁺; the FD holds **iff Y ⊆ X⁺** .

**Worked example A (slide, 7 attributes):** R(A, B, C, D, E, F, G), F = {A → BC, BC → DE, D → F, CF → G}.

- D⁺: {D} → {D, F} (D → F). Nothing else applies ⇒ **D⁺ = {D, F}** .

- {B, C}⁺: {B,C} → {B,C,D,E} (BC → DE) → {B,C,D,E,F} (D → F) → {B,C,D,E,F,G} (CF → G) ⇒ **{B,C}⁺ = {B,C,D,E,F,G}** (no A).

**Worked example B (CLASS relation from the slides):** CLASS(Classid, Course#, Instr_name, Credit_hrs, Text, Publisher, Classroom, Capacity) with

FD1: Classid → Course#, Instr_name, Credit_hrs, Text, Publisher, Classroom, Capacity FD2: Course# → Credit_hrs

- FD3: {Course#, Instr_name} → Text, Classroom FD4: Text → Publisher

FD5: Classroom → Capacity

Closures: **{Classid}⁺ = CLASS** (so Classid is a key); **{Course#}⁺ = {Course#, Credit_hrs}** (it does not include Instr_name because different instructors can teach the same course, and not Text because different instructors may use different texts); **{Course#,**

**Instr_name}⁺ = {Course#, Instr_name, Credit_hrs, Text, Publisher, Classroom, Capacity}** , which does **not** include Classid, so it is **not** a candidate key (the same course by the same instructor can be different classes).

**Worked example C (slide):** R = (A, B, C, G, H, I), F = {A → B, A → C, CG → H, CG → I, B → H}. Members of F⁺: **A → H** (transitivity A → B, B → H); **AG → I** (augment A → C with G to get AG → CG, then transitivity with CG → I); **CG → HI** (augment CG → I with CG, augment CG → H with I, then transitivity).

#### **More closure problems from the slides:**

|Relation and F|Asked|Answer|
|---|---|---|
|R(A..G); F = {A→B, BC→DE,<br>AEG→G}|A⁺, (AC)⁺,<br>(ADE)⁺|A⁺={A,B}; (AC)⁺={A,B,C,D,E};<br>(ADE)⁺={A,B,D,E}|
|R(A..E); F = {A→BC, CD→E, B→D,<br>E→A}|B⁺|{B, D}|
|R(A..H); F = {A→BC, CD→E, D→AEH,<br>E→C, ABH→BD, DH→BC}|(BCD)⁺|{A,B,C,D,E,H}|
|R(P..V); F = {P→Q, QR→ST, PTU→V}|(QR)⁺, (PR)⁺|(QR)⁺={Q,R,S,T}; (PR)⁺={P,Q,R,S,T} (U<br>never appears on a right side, so<br>PTU→V never fires)|
|R(P..T); F = {P→QR, RS→T, Q→S,<br>T→P}|T⁺|{P,Q,R,S,T} = all of R, so**T is a**<br>**candidate key**|

### 3.4.5 Derivation problems from the slides (fully worked)

|#<br>Given F|Derive|Steps|
|---|---|---|
|1<br>{A→B, BC→D}|AC → D|Augment A→B with C: AC→BC;|
|||transitivity with BC→D: AC→D|

|#|Given F|Derive|Steps|
|---|---|---|---|
|2|{A→BC, B→D, C→E}|A → DE|Decompose: A→B, A→C;<br>A→B,B→D gives A→D; A→C,C→E<br>gives A→E; union: A→DE|
|3|{AB→C, C→D, D→E}|AB → E|AB→C,C→D gives AB→D;<br>AB→D,D→E gives AB→E|
|4|{A→B, A→C, BC→D}|A → D|Union: A→BC; transitivity with<br>BC→D: A→D|
|5|{A→B, C→D, BD→E}|AC → E|Augment: AC→BC and AC→AD;<br>decompose: AC→B, AC→D;<br>union: AC→BD; transitivity with<br>BD→E: AC→E|
|6|{A→BC, B→E, CD→EF}|AD → F|A→C (decomp); AD→CD<br>(augment D); CD→EF; AD→EF<br>(transitivity); AD→F (decomp)|
|7|R(Street, Zip, City); {City<br>Street→Zip, Zip→City}|Street Zip →<br>Street Zip City|Zip→City; augment Street:<br>Street Zip→Street City;<br>augment City Street→Zip with<br>City Street; transitivity gives<br>Street Zip → City Street Zip|
|8|{AB→E, BE→I, E→G, GI→H}|AB → GH|AB→G (AB→E,E→G); AB→BE<br>(augment B); AB→I (with BE→I);<br>AB→GI (union); AB→H (with<br>GI→H); AB→GH (union)|

**Shortcut:** instead of a derivation, you can often prove "F implies X → Y" by computing X⁺ and checking that Y ⊆ X⁺. Use derivation only when the question says "using Armstrong's axioms".

## 3.5 Equivalence of FD Sets, Minimal Cover, and Finding Keys

### 3.5.1 Cover and equivalence

- F **covers** E (E is covered by F) if **every FD in E is in F⁺** , that is, every FD of E can be inferred from F.

E and F are **equivalent** if **E⁺ = F⁺** , which holds when **E covers F and F covers E** . **Test whether F covers E:** for each FD X → Y in E, compute **X⁺ using F** and check that **Y ⊆ X⁺** .

**Three possible cases for sets F and G:** (1) F covers G; (2) G covers F; (3) both cover each other, so **F = G (equivalent)** .

**Trap:** equivalence is about **closures** , not about looking alike or having the same number of FDs. An FD can be missing from G syntactically and still be derivable from it.

**Worked example 1 (slide):** F = {A→C, AC→D, E→AD, E→H}, G = {A→CD, E→AH}.

- F covers G? Under F: A⁺ = {A,C,D} ⊇ {C,D} ✓; E⁺ = {E,A,D,C,H} ⊇ {A,H} ✓.

- G covers F? Under G: A⁺={A,C,D} contains C ✓; (AC)⁺={A,C,D} contains D ✓; E⁺= {E,A,H,C,D} contains A,D,H ✓.

- **Both hold ⇒ F = G. Answer: option (D) all of the above.**

**Worked example 2 (slide):** F = {A→B, B→C, AB→D}, G = {A→B, B→C, A→C, A→D}. (AB)⁺ under G = {A,B,C,D} so AB→D holds in G; A⁺ under F = {A,B,C,D} so A→C and A→D hold in F. **Equivalent.**

#### **Practice (slide):**

- FD1 = {x→y, xy→z, w→xz, w→vy}, FD2 = {x→yz, w→xv}: **equivalent** . (w⁺ under FD2 = {w,x,v,y,z} ⊇ {x,z}.)

- F = {A→B, B→C, A→C}, G = {A→B, B→C, A→D}: **G covers F, F does not cover G** (A⁺ under F = {A,B,C} lacks D), so **not equivalent** .

### 3.5.2 Extraneous attributes and minimal sets

An attribute in an FD is **extraneous** if it can be removed **without changing the closure** of the FD set. Formally, for X → A in F, attribute Y in X is extraneous if F implies (F − {X→A}) ∪ {(X − Y) → A}.

A set of FDs is **minimal** if:

1. Every FD has a **single attribute on its right-hand side** (canonical form).

2. No FD X → A can be replaced with Y → A, where Y is a **proper subset** of X, and still give an equivalent set ( **no extraneous left-side attribute** ).

3. **No FD can be removed** and still leave an equivalent set ( **no redundant FD** ).

**Minimal cover** of E: a minimal set of FDs (canonical form, no redundancy) that is **equivalent to E** . A set can have **more than one** minimal cover.

### 3.5.3 Algorithm for minimal cover

**Textbook order:** (1) split RHS; (2) remove extraneous LHS attributes; (3) remove redundant FDs. **Alternative order shown in the slides (Method 2):** (1) split RHS; (2) remove redundant FDs; (3) remove extraneous attributes. Either order is accepted; state the order you use.

```
1. F := E
2. Decompose: replace X -> {A1,...,An} by X->A1, ..., X->An
3. For each FD X -> A and each B in X:
      if (F - {X->A}) U {(X-{B}) -> A} is equivalent to F
         replace X->A by (X-{B}) -> A          // remove extraneous attribute
B

4. For each remaining FD X -> A:
      if (F - {X->A}) is equivalent to F
         remove X->A                            // remove redundant FD
```

**How to test redundancy of X → A:** remove it, compute X⁺ with the remaining FDs; if **A ∈ X⁺** the FD is redundant. **How to test an extraneous attribute Y in X:** compute (X − Y)⁺ using F; if it contains A, Y is extraneous.

**Example 1 (slide, textbook order):** E = {B→A, D→A, AB→D}.

- Step 2: AB→D: from B→A, B→AB; with AB→D transitivity gives **B→D** , so A is extraneous. E′ = {B→A, D→A, B→D}.

- Step 3: B→D and D→A give B→A, so **B→A is redundant** .

- **Minimal cover = {B→D, D→A}.**

**Example 2 (slide):** G = {A→BCDE, CD→E}. Split: A→B, A→C, A→D, A→E, CD→E. Neither C nor D is extraneous in CD→E. A→CD and CD→E give A→E, so **A→E is redundant** . Minimal cover = {A→B, A→C, A→D, CD→E}, written compactly as {A→BCD, CD→E}.

**Example 3 (slide, long, Method 2):** {A→C, AB→C, C→DI, CD→I, EC→AB, EI→C}.

- Split: A→C, AB→C, C→D, C→I, CD→I, EC→A, EC→B, EI→C.

- Redundant: **AB→C** (A→C already gives it) and one of **C→I / CD→I** (the slides drop C→I, then reduce CD→I to C→I by removing the extraneous D).

- Extraneous: in **CD→I** , D is extraneous because C⁺ = {C,D,I}; it becomes C→I.

- **Minimal cover = {A→C, C→D, C→I, EC→A, EC→B, EI→C}.**

**Practice answers from the slides:**

|Given|Minimal cover|
|---|---|
|{v→w, vw→x, y→vxz}|{v→wx, y→vz}|
|{A→B, C→B, D→ABC, AC→D}|{A→B, C→B, D→AC, AC→D}|
|R(W,X,Y,Z): {X→W, WZ→XY, Y→WXZ}|{X→W, WZ→Y, Y→X, Y→Z} (WZ→X and Y→W are<br>redundant)|

**Trap:** After removing redundant FDs, **recompute with the reduced set** each time. Order of removal can give a different, equally valid minimal cover.

### 3.5.4 Finding keys

**Superkey** = set of attributes that determines all of R (S⁺ = R). **Key (candidate key)** = a **minimal** superkey: removing any attribute destroys the property. R itself is always a default superkey.

#### **Algorithm to find one key (slides):**

```
K := R
for each attribute A in K:
    compute (K - {A})+ with respect to F
    if (K - {A})+ contains all attributes of R:
        K := K - {A}
```

This finds **only one** key; the key found depends on the order in which attributes are removed.

#### **Method to find all candidate keys (slides):**

1. **Essential attributes** = attributes that **do not appear on the RHS** of any FD. They **must be in every candidate key** .

2. The remaining attributes are **non-essential** .

3. **Case 1:** if the essential attributes' closure is all of R, then the essential attributes form the **only** candidate key.

4. **Case 2:** otherwise add non-essential attributes in combinations (smallest first) until the closure is R; **multiple** candidate keys are possible.

**Counting superkeys (Case 1):** if there are _e_ essential attributes and _n_ total attributes, and the essential attributes alone form the only candidate key, then **superkeys = 2^(n − e)** (each non-essential attribute may be present or absent).

**Worked example (slide):** R(A,B,C,D,E,F), F = {C→F, E→A, EC→D, A→B}. Essential attributes = **C, E** (never on a RHS). {C,E}⁺ = {C,E,F,A,D,B} = R. So **CE is the only candidate key** ; n = 6, e = 2 so superkeys = 2⁴ = **16** .

#### **Slide practice answers:**

|Problem|Essential|Candidate keys|Superkeys|
|---|---|---|---|
|R(A,B,C,D,E); AB→C, C→D, B→E|A, B|**AB**(only one)|2³ =**8**|
|R(E,F,G,H,I,J,K,L,M,N); EF→G, F→IJ,<br>EH→KL, K→M, L→N|E, F, H|**EFH**(only one)|2⁷ =**128**|
|R(P,Q,R,S,T); FD = {PQ→R, S→T}|P, Q, S|**PQS**(PQ⁺={P,Q,R}, QS⁺=<br>{Q,S,T}, PS⁺={P,S,T} all fall<br>short)|2² = 4|

**Shortcut:** If an attribute appears on **no** RHS it is in every key. If an attribute appears only on RHSs and never on any LHS, it is in **no** key.

## 3.6 Informal Design Guidelines for Relation Schemas

**Relational database design** = grouping attributes to form "good" relation schemas. It is judged at two levels: **logical (conceptual)** and **storage (implementation)** .

### 3.6.1 Two design methodologies

||**Bottom-up (design by synthesis)**|**Top-down (design by analysis)**|
|---|---|---|
|Starting<br>point|Basic**relationships among**<br>**individual attributes**|Groupings of attributes that occur<br>**naturally together**(invoice, form,<br>report)|
|Process|Build relation schemas from those<br>binary relationships|Analyse relations individually and<br>collectively and**decompose**until<br>desirable properties hold|
|Practicality|**Not popular**: collecting binary<br>relationships among all attribute<br>pairs is next to impossible|**The usual approach**; normalization<br>theory supports it|

### 3.6.2 Four informal measures of quality

Design concerns **base relations** . The four measures (not always independent of each other):

1. Make the **semantics of the attributes clear** .

2. **Reduce redundant information** in tuples.

3. **Reduce NULL values** in tuples.

4. **Disallow spurious tuples** .

### Guideline 1: clear semantics

Each tuple should represent **one entity or relationship instance** . Semantics = interpretation of attribute values in a tuple; the **ease of explaining a relation's meaning** is an informal measure of its design.

- Do **not** mix attributes of different entities (EMPLOYEE, DEPARTMENT, PROJECT) in one relation.

- Refer to other entities **only through foreign keys** .

- Keep **entity and relationship attributes apart** as far as possible.

**Violation examples: EMP_DEPT** mixes employee and department attributes (Dname, Dmgr_ssn). **EMP_PROJ** mixes employee, project and WORKS_ON relationship attributes (Ename, Pname, Plocation). Both have clear meanings but still violate Guideline 1. The simple COMPANY schema (separate EMPLOYEE, DEPARTMENT, PROJECT, WORKS_ON...) is the better design.

### Guideline 2: redundancy and anomalies

Redundant storage **wastes space** and causes **anomalies** . Design so that no insertion, deletion or update anomaly occurs; if one must exist (for performance), **document it** so applications can cater for it. If EMP_DEPT is used as a **view** , anomalies can be handled with **triggers, functions or procedures** .

Consider **EMP_PROJ(Ssn, Pnumber, Hours, Ename, Pname, Plocation)** with primary key {Ssn, Pnumber}:

|Anomaly|What goes wrong|Slide example|
|---|---|---|
|**Update**<br>**anomaly**|A fact stored in many rows must be<br>changed in**all**of them; changing<br>only one makes data**inconsistent**|Renaming project 1 from "ProductX"<br>to "ProductA" must be done in<br>**every**row (100+ employees).<br>Updating one row leaves project 10<br>with**two names**|
|**Insertion**<br>**anomaly**|Cannot record a fact without an<br>**unrelated fact**(because of a key<br>attribute)|Cannot insert a**project**until an<br>employee is assigned (Ssn is part of<br>the key, so MySQL gives**ERROR**<br>**1364**). Conversely cannot insert an<br>**employee**not assigned to a project|
|**Deletion**<br>**anomaly**|Deleting one fact**removes another**<br>fact|Deleting a project deletes the<br>employees on it; deleting the**only**<br>**employee**of a project deletes the<br>project. In EMP_DEPT, deleting Borg<br>(the only employee of department<br>1) erases**Headquarters**from the<br>database with no error|

**Trap:** The deletion anomaly raises **no DBMS error** ; the delete is legal. That is what makes it dangerous.

### Guideline 3: NULL values

Design so tuples have **as few NULLs as possible** . If NULLs are unavoidable they should apply only in **exceptional cases** . Attributes that are often NULL can go in a **separate relation with the primary key** .

- **Reasons for NULL:** attribute **not applicable** , **value unknown** (may exist), or **known to exist but unavailable** .

- _Example:_ if only **15%** of employees have individual offices, do not put Office_number in EMPLOYEE; create **EMP_OFFICES(Essn, Office_number)** containing only those employees.

### Guideline 4: avoid spurious tuples (at any cost)

Design relations so they can be **joined by equality on (primary key, foreign key) pairs** in a way that **guarantees no spurious tuples** . Avoid relations whose common attributes are **not** (foreign key, primary key) combinations.

_Slide example:_ decomposing EMP_PROJ into **EMP_LOCS(Ename, Plocation)** and **EMP_PROJ1(Ssn, Pnumber, Hours, Pname, Plocation)** and natural-joining on **Plocation** (not a key) turns **16 genuine rows into 57** , i.e. **41 spurious tuples** (employees paired with projects they never worked on).

**Key:** A **spurious tuple** is an extra, false tuple produced by joining relations that were decomposed badly. It means a **lossy (additive) decomposition** .

## 3.7 Normalization and Normal Forms Based on Primary Keys (1NF, 2NF, 3NF)

### 3.7.1 Basic definitions

- **Normalization:** process of analysing a relation schema, using its **FDs and keys** , to achieve **minimum redundancy** and **minimum insertion, deletion and update anomalies** ; done by **decomposing** bad relations into smaller ones.

- **Normal form:** the **highest normal form condition a relation meets** ; it indicates the **degree of normalization** .

|Normal form|Based on|
|---|---|
|1NF|Atomic values (basic requirement of the relational<br>model)|
|2NF, 3NF, BCNF|**Keys and FDs**|
|4NF|Keys and**multivalued dependencies (MVDs)**|
|5NF (PJNF)|Keys and**join dependencies (JDs)**|

A good design also needs two **additional properties of decomposition** : **lossless (nonadditive) join** and **dependency preservation** (section 3.8).

**Practical use:** designers **need not normalise to the highest form** . Usually **up to 3NF or BCNF** ; **4NF is rarely used and 5NF almost never** , because MVDs/JDs are hard to detect.

### 3.7.2 Keys and attribute definitions

- **Superkey** S of R: a subset of attributes such that **no two tuples** in any legal state have t1[S] = t2[S].

- **Key K:** a superkey that is **minimal** (removing any attribute stops it being a superkey).

- **Candidate keys:** all keys of a relation. One is chosen as **primary key** ; the others are **secondary (alternate) keys** . If no key is known, the **whole relation** is a default superkey.

- **Prime attribute:** member of **some candidate key** . **Non-prime:** not a member of any candidate key.

### 3.7.3 First Normal Form (1NF)

**Definition:** the domain of every attribute contains only **atomic (indivisible)** values; each field holds a **single value** from its domain.

**1NF disallows: composite attributes** , **multivalued attributes** , and **nested relations** (relations within relations), i.e. no **repeating groups** or sets of values in a single field.

#### **Fixing a 1NF violation:**

1. Put the multivalued attribute into a **separate relation** along with the **primary key** of the original ( **preferred** ).

2. **Expand the key** so there is a separate tuple for each value (introduces **redundancy** ; later leads to MVDs).

3. If the maximum number of values is small and known, add **one column per value** (introduces NULLs; usually poor).

_Slide example (DEPARTMENT):_ Dlocations is multivalued; the 1NF versions are DEPT_LOCATIONS(Dnumber, Dlocation) plus DEPARTMENT, or expanding the key to (Dnumber, Dlocation). _Nested relations (EMP_PROJ with nested PROJS):_ decompose into **EMP_PROJ1(Ssn, Ename)** and **EMP_PROJ2(Ssn, Pnumber, Hours)** by **propagating the primary key** .

### 3.7.4 Second Normal Form (2NF, with respect to primary key)

Uses FDs and the **primary key** .

**Definition:** a relation R is in **2NF** if it is in **1NF** and **every non-prime attribute A is fully functionally dependent on the primary key** .

- Test: check FDs whose **left side is part of the primary key** . If any **non-prime attribute depends on part of the key** , R fails 2NF.

- If the primary key is a **single attribute** , no test is needed ( **2NF holds automatically** ).

- Fix: **second normalization** : for each partial dependency, create a new relation with the **partial key and the attributes it determines** , and **keep the original relation with the remaining attributes plus the full key** .

**Slide example EMP_PROJ(Ssn, Pnumber, Hours, Ename, Pname, Plocation)** with FDs: FD1 {Ssn, Pnumber} → Hours; FD2 Ssn → Ename; FD3 Pnumber → {Pname, Plocation}. FD2 and FD3 are **partial dependencies** . **2NF decomposition:**

- **EP1(Ssn, Pnumber, Hours)**

- **EP2(Ssn, Ename)**

**EP3(Pnumber, Pname, Plocation)**

Rejoining EP1, EP2, EP3 gives exactly the **16 original rows** (lossless) and renaming a project now touches **one row** (update anomaly removed).

### 3.7.5 Third Normal Form (3NF, with respect to primary key)

**Transitive FD:** X → Z derivable from X → Y and Y → Z. _Examples:_ **SSN → DMGRSSN is transitive** (SSN → DNUMBER and DNUMBER → DMGRSSN); **SSN → ENAME is nontransitive** .

**Definition:** R is in **3NF** if it is in **2NF** and **no non-prime attribute is transitively dependent on the primary key** .

**Exception (important):** in X → Y, Y → Z with X the primary key, the transitive dependency is a **problem only if Y is not a candidate key** . If Y **is** a candidate key there is **no problem** . _Example:_ EMP(SSN, Emp#, Salary) with SSN → Emp# → Salary: Emp# is a candidate key, so **no 3NF violation** .

**Slide example EMP_DEPT(Ename, Ssn, Bdate, Address, Dnumber, Dname, Dmgr_ssn)** with Ssn → Dnumber → {Dname, Dmgr_ssn}: transitive. **3NF decomposition:**

**ED1(Ename, Ssn, Bdate, Address, Dnumber)**

- **ED2(Dnumber, Dname, Dmgr_ssn)**

### 3.7.6 Normal forms defined informally

|Normal form|Informal rule|
|---|---|
|**1NF**|**All attributes are atomic**|
|**2NF**|All attributes depend on**the whole key**|
|**3NF**|All attributes depend on**nothing but the key**|

_(Memory line: "the key, the whole key, and nothing but the key.")_

**Trap:** Each normal form is **strictly stronger** than the previous: BCNF ⊂ 3NF ⊂ 2NF ⊂ 1NF.

### 3.7.7 The LOTS relation (textbook example used in the slides)

**LOTS(Property_id#, County_name, Lot#, Area, Price, Tax_rate)** with FDs:

- **FD1:** Property_id# → {County_name, Lot#, Area, Price, Tax_rate}

- **FD2:** {County_name, Lot#} → {Property_id#, Area, Price, Tax_rate}

- **FD3:** County_name → Tax_rate

- **FD4:** Area → Price

Candidate keys: **Property_id#** and **{County_name, Lot#}** . Primary key: Property_id# (chosen).

|Stage|Result|
|---|---|
|**2NF (with respect to the primary key)**: no<br>partial dependency on Property_id#|**LOTS1(Property_id#, County_name, Lot#,**<br>**Area, Price)**and**LOTS2(County_name,**|
|(single-attribute key) but see general<br>definition below. For 2NF (every key): FD3<br>violates because County_name is part of<br>key {County_name, Lot#}|**Tax_rate)**|
|**3NF**: LOTS1 has Area → Price, Area is not a<br>key and Price is non-prime|**LOTS1A(Property_id#, County_name, Lot#,**<br>**Area)**and**LOTS1B(Area, Price)**|
|Final|**LOTS1A, LOTS1B, LOTS2**are in 3NF|

## 3.8 General Definitions of 2NF and 3NF, BCNF, and Decomposition Properties

### 3.8.1 Why general definitions?

The earlier definitions consider **only the primary key** . The **general definitions** handle relations with **multiple candidate keys** . Any attribute in **any** candidate key is prime; all others are non-prime.

### 3.8.2 General definition of 2NF

R is in **2NF** if every **non-prime attribute A is fully functionally dependent on every key of R** .

_Example:_ in LOTS, **County_name → Tax_rate** violates 2NF (County_name is part of candidate key {County_name, Lot#}). Second normalization gives **LOTS1(Property_id#, County_name, Lot#, Area, Price)** and **LOTS2(County_name, Tax_rate)** .

### 3.8.3 General definition of 3NF

R is in **3NF** if **whenever a non-trivial FD X → A holds in R** , **either**

- **(a) X is a superkey of R** , **or**

- **(b) A is a prime attribute of R** .

_Example:_ LOTS1 violates 3NF because **Area → Price** holds, **Area is not a superkey** , and **Price is not prime** .

**Interpretation of condition (a):** it catches two kinds of violations: (i) a **prime attribute (part of a key) determines a non-prime attribute** (this is a **2NF violation, partial dependency** ), and (ii) a **non-prime attribute determines a non-prime attribute** (a **3NF violation, transitive dependency** ).

**Alternative definition of 3NF:** every non-prime attribute is **fully functionally dependent on every key** and **non-transitively dependent on every key** . (So a 3NF relation also meets 2NF.)

**Condition (b)** is the loophole: it lets through dependencies that **BCNF** catches.

### 3.8.4 Boyce-Codd Normal Form (BCNF)

**Definition:** R is in **BCNF** if **whenever an FD X → A holds in R, X is a superkey of R** .

||**3NF**|**BCNF**|
|---|---|---|
|Condition for every non-|X is a superkey**OR**A|**X must be a superkey**(no escape|
|trivial FD X → A|is prime|clause)|
|Strength|Weaker|**Stronger**|
|Relationship|Every BCNF relation<br>is in 3NF|**Some 3NF relations are not in**<br>**BCNF**|
|Violation occurs only when||X is**not**a superkey**and**A is**prime**<br>(otherwise 3NF would also fail)|

In practice **most 3NF relations are also in BCNF** . A relation is 3NF but not BCNF **only** if there exists X → A with X not a superkey and A **prime** (Fig. 14.13(b): C → B, C not superkey, B prime).

**Trap:** "R is in BCNF" ⇒ "R is in 3NF" is true; the **converse is false** .

**Hierarchy:** every 2NF is in 1NF; every 3NF in 2NF; every BCNF in 3NF. **Goal: BCNF (or at least 3NF) for every relation.**

### 3.8.5 Example: TEACH is in 3NF but not BCNF

**TEACH(Student, Course, Instructor)** with

- **fd1:** {Student, Course} → Instructor

- **fd2:** Instructor → Course (each instructor teaches only one course)

Candidate key: **{Student, Course}** (also {Student, Instructor}). So Course is **prime** .

- fd1: determinant is a key ✓.

- fd2: **Instructor is not a superkey** (an instructor appears in several rows) but **Course is prime** , so **3NF is satisfied by condition (b)** , but **BCNF fails** .

### 3.8.6 Two properties of good decomposition

|Property|Meaning|Importance|
|---|---|---|
|**Non-additive**<br>**(lossless) join**|Joining the decomposed relations<br>gives**exactly the original**(no<br>**spurious tuples**)|**Mandatory, "must be**<br>**achieved at any cost"**|

|Property|Meaning|Importance|
|---|---|---|
|**Dependency**<br>**preservation**|Every FD of F is represented in<br>**some individual decomposed**<br>**relation**(so it can be enforced<br>without joins)|**Desirable**, sometimes<br>sacrificed|

**Why lossless matters more:** a lost dependency can still be **checked by the application or across a join** (inconvenient but data stays correct). A lossy join **invents tuples that were never true** : wrong data, unrecoverable.

### 3.8.7 Test for lossless binary decomposition (Property NJB)

A decomposition **D = {R1, R2}** of R has the **lossless (non-additive) join property** with respect to FD set F **if and only if** either

- **(R1 ∩ R2) → (R1 − R2)** is in F⁺, **or**

- **(R1 ∩ R2) → (R2 − R1)** is in F⁺.

**In words:** the **common attributes must be a key of at least one** of the two relations. **Apply NJB to the three decompositions of TEACH:**

|Decomposition|R1∩R2|Is the FD in F⁺?|Lossless?|
|---|---|---|---|
|**D1**= {Student, Instructor}|Student|Student → Instructor?|**No**(lossy: rejoin gives|
|and {Student, Course}||No. Student → Course?<br>No|19 rows vs 9, so 10<br>spurious)|
|**D2**= {Course, Instructor}<br>and {Course, Student}|Course|Course → Instructor?<br>No. Course → Student?<br>No|**No**|
|**D3**= {Instructor, Course}|Instructor|**Instructor → Course**is|**Yes**(rejoin gives|
|and {Instructor, Student}||fd2,**Yes**|exactly 9 rows)|

All three decompositions **lose fd1** ({Student, Course} → Instructor); we **accept losing dependency preservation** but **never** the non-additive join. **D3 is the good decomposition.**

### 3.8.8 General procedure to achieve BCNF

Let R be a relation not in BCNF and X → A the FD that violates BCNF. Decompose R into

1. **R − A** (everything except A), and

2. **X ∪ A** .

If either piece is still not in BCNF, **repeat** .

_TEACH:_ violating FD Instructor → Course gives (TEACH − Course) = **(Instructor, Student)** and (Instructor ∪ Course) = **(Instructor, Course)** , which is exactly **D3** .

**Why (R − A) and not R:** if A stays beside its determinant, the same bad FD travels with it and the redundancy survives. E.g. R(W,X,Y,Z), violating FD Y → W: pieces are (X,Y,Z) and (Y,W). Keeping (W,X,Y) would still contain Y → W with Y not a superkey.

## 3.9 Higher Normal Forms: MVD, 4NF, JD, 5NF

### 3.9.1 Multivalued dependency (MVD) and Fourth Normal Form

**Definition (MVD X →→ Y):** on R, with Z = R − (X ∪ Y): if two tuples t1, t2 exist with **t1[X] = t2[X]** , then two more tuples **t3, t4** must also exist with

- t3[X] = t4[X] = t1[X] = t2[X],

- **t3[Y] = t1[Y]** and **t4[Y] = t2[Y]** ,

- **t3[Z] = t2[Z]** and **t4[Z] = t1[Z]** .

In words: for a given X, the set of Y values is **independent of the set of Z values** , so the relation must contain **all combinations (cross product)** of them.

- An MVD X →→ Y is **trivial** if **(a) Y ⊆ X** or **(b) X ∪ Y = R** . A trivial MVD specifies no significant constraint. Otherwise it is **non-trivial** .

- MVDs arise from **1NF** , which forbids sets of values in an attribute: the "expand the key" way of fixing multivalued attributes creates them.

- Slide examples of MVDs: SID →→ CID; SID →→ CNAME; SNAME →→ CNAME.

**4NF definition:** R is in **4NF** with respect to a set F of FDs and MVDs if, for every **non-trivial MVD X →→ Y in F⁺** , **X is a superkey of R** .

**Example (Fig. 14.15a, EMP(Ename, Pname, Dname))** with MVDs **Ename →→ Pname** and **Ename →→ Dname** (projects and dependents are independent of each other). Ename is not a key, so EMP violates 4NF. **Decompose:**

#### **EMP_PROJECTS(Ename, Pname)**

#### **EMP_DEPENDENTS(Ename, Dname)**

_Counting:_ if Smith has projects {X, Y} and dependents {John, Anna}, EMP must store **2 × 2 = 4** rows to hold 2 + 2 = 4 independent facts. After decomposition: 2 + 2 rows, rejoining gives back all 4 losslessly.

### 3.9.2 Join dependency (JD) and Fifth Normal Form (PJNF)

**Definition (JD): JD(R1, R2, ..., Rn)** on R says every legal state r of R has a **non-additive join decomposition** into R1, ..., Rn, that is, **�(πR1(r), ..., πRn(r)) = r** .

An **MVD is the special case of a JD with n = 2** : X →→ Y is JD(XY, XZ).

A JD is **trivial** if one of the Ri equals R.

**5NF (Project-Join Normal Form, PJNF):** R is in **5NF** with respect to FDs, MVDs and JDs if, for every **non-trivial JD(R1, ..., Rn) in F⁺** , **every Ri is a superkey of R** .

**Example (Fig. 14.15c, SUPPLY(Sname, Part_name, Proj_name))** has **no MVD** (so it is in 4NF) but satisfies **JD(R1, R2, R3)** , with R1(Sname, Part_name), R2(Sname, Proj_name), R3(Part_name, Proj_name). No Ri is a superkey, so it is **not in 5NF** . **5NF decomposition:** R1, R2, R3.

_Slide measurement:_ SUPPLY has **7 tuples** . Joining **only R1 and R2** gives **9 rows** (2 spurious), so **no binary decomposition is lossless** (hence 4NF holds). The **three-way join gives exactly 7** , proving the JD holds.

**Why 5NF is rarely used:** a JD is not visible from any pair of attributes; it only shows up when **three or more** projections are rejoined. Discovering JDs in databases with hundreds of relations is next to impossible. BCNF only needs FDs, which designers already collect.

## 3.10 Worked Numerical Problems (Normalization)

#### **Standard procedure for any "is R in 2NF/3NF/BCNF? If not, decompose" question:**

1. Find **candidate key(s)** using attribute closure (essential attributes first).

2. List **prime** and **non-prime** attributes.

3. Check every FD against the definition.

4. **Decompose** , and add a relation containing the key if the key would otherwise be lost.

### 3.10.1 2NF problems

#### **P1. R(P,Q,R,S,T,U,V,W,X,Y); FD = {PQ→R, PS→VW, QS→TU, P→X, W→Y}.**

- (PQS)⁺ = all attributes ⇒ **candidate key PQS** . Prime: P, Q, S. Non-prime: R, T, U, V, W, X, Y.

- PQ→R, PS→VW, QS→TU, P→X: each is a **partial dependency** (part of key determines non-prime) ⇒ **not in 2NF** . W→Y is non-prime → non-prime, which is **not a 2NF issue** (it is a 3NF/transitive issue).

- **Decomposition (slide):** R1(P,Q,R), R2(P,S,V,W), R3(Q,S,T,U), R4(P,X), R5(W,Y), and **R6(P,Q,S)** to keep the key.

- _Note:_ strictly, 2NF only needs the partial dependencies removed; splitting out R5(W,Y) already goes towards 3NF.

**P2. R(P,Q,R,S,T); FD = {PQ→R, S→T}.** (PQS)⁺ = PQSRT ⇒ key PQS; prime P,Q,S; non-prime R,T. PQ→R (partial) and S→T (partial) ⇒ **not in 2NF** . **Decomposition:** R1(P,Q,R), R2(S,T), R3(P,Q,S).

**P3 (practice answer). R(A,B,C,D,E); FD = {A→B, B→E, C→D}.** Essential: A, C; (AC)⁺ = ABCDE ⇒ key **AC** ; prime A,C; non-prime B,D,E. A→B, C→D and derived A→E are partial ⇒ **not in 2NF** . **2NF decomposition:** R1(A,B,E), R2(C,D), R3(A,C). (R1 is still **not in 3NF** : A→B→E is transitive.)

### 3.10.2 3NF problems

**P4. R(X,Y,Z); FD = {X→Y, Y→Z}.** X⁺ = XYZ ⇒ key X. Both FDs pass 2NF. Y→Z: Y is not a superkey and Z is not prime ⇒ **not in 3NF** . **Decomposition:** R1(Y,Z), R2(X,Y).

**P5. R(X,Y,Z,W,P); FD = {X→Y, Y→P, Z→W}.** (XZ)⁺ = XZYPW ⇒ key **XZ** ; prime X,Z; non-prime Y,W,P. X→Y and Z→W are partial ⇒ not in 2NF. Decompose for 2NF: R1(X,Y,P), R2(Z,W), R3(X,Z). R1 still has Y→P (transitive) ⇒ split it. **Final 3NF: R1(X,Y), R2(Z,W), R3(X,Z), R4(Y,P)** .

**P6. R(P,Q,R,S,T,U,V,W,X,Y); FD = {PQ→R, P→ST, Q→U, U→VW, S→XY}.** PQ⁺ = all ⇒ key **PQ** ; prime P,Q. FDs P→ST, Q→U, U→VW, S→XY each have a non-superkey determinant and nonprime dependent ⇒ **not in 3NF** . _[Supplement] Complete answer:_ 2NF: (P,Q,R), (P,S,T,X,Y), (Q,U,V,W); 3NF: **(P,Q,R), (P,S,T), (S,X,Y), (Q,U), (U,V,W)** .

### 3.10.3 BCNF problems

**P7. R(A,B,C,D,E); FD = {A→BC, C→DE}.** Key **A** . A→BC: A is a key ✓. **C→DE: C is not a superkey ⇒ violates BCNF.** Decompose using the violating FD: **R1(A,B,C)** and **R2(C,D,E)** . Join on C is lossless because C is a key of R2. Both in BCNF.

**P8. R(W,X,Y,Z); F = {WX→Y, X→Z, Y→W}.** Candidate keys **WX** and **XY** . WX→Y fine. **X→Z** (X not a superkey) and **Y→W** (Y not a superkey) violate BCNF. **Decomposition: R1(X,Y), R2(X,Z), R3(Y,W)** . It is **lossless** (rejoin returns exactly the original rows) but **WX→Y is not preserved** (no single relation contains W, X, Y together).

**Trap (slide correction):** R1 must be **(X,Y)** , not (W,X,Y). Keeping W beside X and Y leaves Y→W inside R1 with Y not a superkey, so (W,X,Y) would still violate BCNF.

**P9. Book relation, candidate key {author, book title}; FDs: author → nationality; book title → genre, number of pages.** Both determinants are only parts of the key ⇒ violate BCNF. **Decomposition:** R1(author, nationality), R2(book title, genre, number of pages), R3(author, book title).

## 3.11 Comparison Tables and Frequently Confused Concepts

**Table 1: Normal forms at a glance**

|NF|Condition|Removes|Typical violation|Typical fix|
|---|---|---|---|---|
|**1NF**|All values atomic|Multivalued,<br>composite, nested<br>attributes|Phone numbers list<br>in one column|Separate relation<br>with key, or<br>expand rows|
|**2NF**|1NF + no**partial**<br>dependency of<br>non-prime on a key|Partial<br>dependencies|{Ssn,Pnumber} →<br>Ename (Ssn →<br>Ename)|Split off partial<br>key +<br>dependents|
|**3NF**|2NF + for each FD<br>X→A: X superkey<br>**or**A prime|Transitive<br>dependencies|Ssn → Dnumber →<br>Dname|Split off Y → Z|
|**BCNF**|For each FD X→A:<br>**X is a superkey**|All non-key<br>determinants|Instructor →<br>Course in TEACH|R − A and X∪A|

|NF|Condition|Removes|Typical violation|Typical fix|
|---|---|---|---|---|
|**4NF**|For each non-<br>trivial MVD X→→Y:<br>X superkey|MVDs|Ename →→ Pname|Split into two<br>relations|
|**5NF**|Each non-trivial JD<br>component is a<br>superkey|Join<br>dependencies|SUPPLY|Split into 3<br>projections|

#### **Table 2: Types of FD**

||Full|Partial|Transitive|Trivial|
|---|---|---|---|---|
|Key idea|Whole|Part of|Via a middle non-|RHS⊆|
||determinant|determinant|prime attribute|LHS|
||needed|enough|||
|Linked|Needed for 2NF|Violates 2NF|Violates 3NF|Always|
|normal form||||allowed|

#### **Table 3: 3NF vs BCNF**

||3NF|BCNF|
|---|---|---|
|Rule|X superkey**or**A<br>prime|X superkey|
|Lossless decomposition always possible|**Yes**|Yes|
|Dependency preserving decomposition|**Yes**|**No**(may lose FDs)|
|always possible|||
|Redundancy|May still exist|Eliminated (for<br>FDs)|

#### **Table 4: Lossless vs dependency preserving**

||Lossless (non-additive) join|Dependency preservation|
|---|---|---|
|Question|Can we rebuild R exactly by|Can every FD be checked inside one|
||joining?|relation?|

||Lossless (non-additive) join|Dependency preservation|
|---|---|---|
|Violation|**Spurious tuples**|FD must be checked by a join|
|causes|||
|Mandatory?|**Yes**|Desirable only|
|Test|NJB for binary decompositions|Check each FD fits inside some Ri|

#### **Table 5: FD vs MVD vs JD**

||FD|MVD|JD|
|---|---|---|---|
|Symbol|X → Y|X →→ Y|JD(R1,...,Rn)|
|Meaning|X determines<br>**one**Y|X determines a**set**of Y<br>independent of other attributes|R equals join of its<br>projections|
|Normal|2NF, 3NF, BCNF|**4NF**|**5NF**|
|form||||
|Special<br>case||JD with n = 2||

#### **Table 6: Anomalies**

|Anomaly|Trigger|Consequence|
|---|---|---|
|Update|Change duplicated data in one row only|Inconsistent copies|
|Insertion|Need unrelated data to insert a fact|Cannot store the fact|
|Deletion|Delete a row holding two facts|Unintended loss of the second fact|

#### **Table 7: Candidate vs super vs primary vs prime**

|Term|Meaning|
|---|---|
|Superkey|Any set whose closure is R|
|Candidate key|Minimal superkey|
|Primary key|The chosen candidate key|

|Term|Meaning|
|---|---|
|Secondary/alternate key|Other candidate keys|
|Prime attribute|Member of**any**candidate key|

#### **Common confusions (read before the exam):**

- **Trap:** "Superkey count" is 2^(n−e) **only when the essential attributes alone are the single candidate key** .

- **Trap:** X⁺ is a set of **attributes** ; F⁺ is a set of **FDs** .

- **Trap:** F⁺ can have up to **2^(2n)** FDs.

- **Trap:** A relation with a **single-attribute** candidate key is automatically in **2NF** .

- **Trap:** A relation with **only two attributes** is always in **BCNF** (and 3NF, 2NF).

- **Trap:** Reflexive/trivial FDs never cause normal-form violations.

## 3.12 Likely 4-Mark Questions with Model Answers

**Q1. Explain the steps in processing a high-level SQL query. (4)** _Answer:_ A query is first **scanned** (tokens such as SQL keywords, attribute and relation names identified), **parsed** (syntax checked against the grammar) and **validated** (all relation and attribute names checked against the schema). An internal representation, the **query tree** (or a DAG query graph), is built. The **query optimizer** considers many equivalent execution strategies and produces a good **execution plan** . The **code generator** generates code for the plan and the **runtime database processor** executes it to produce the result. SQL is first mapped to relational algebra by splitting it into **query blocks** (SELECT-FROM-WHERE with optional GROUP BY/HAVING); nested subqueries such as EXISTS/NOT EXISTS are converted to **semi-joins/anti-joins** .

**Q2. Describe any four algorithms for the SELECT operation. (4)** _Answer:_ **S1 Linear search:** every block is read and every record tested; always applicable. **S2 Binary search:** equality on a key attribute on which the file is ordered; faster than linear. **S3a Primary index:** equality on a key attribute with a primary index; retrieves at most one record. **S3b Hash key:** equality on a key with hashing; at most one record. **S4 Primary index for ranges:** for >, >=, <, <= locate the equality record through the index, then read all subsequent (or preceding) records of the ordered file. Index-based methods are **index scans** ; others are **file scans** .

**Q3. Explain nested-loop, index-based nested-loop, sort-merge and partition-hash join. (4)** _Answer:_ **Nested-loop:** for each outer record t, scan all inner records and test t[A] = s[B]; no access path needed; cost about |R|×|S|. **Index-based nested-loop:** uses an index on the inner join attribute so each outer tuple does one index lookup; cost about |R| ×log|S|. **Sort-merge:** both relations sorted on the join attributes are scanned once in step and matching values merged. **Partition-hash:** both files are partitioned with the same hash function; the smaller file is hashed first (partitioning phase) and the other file probes the buckets (probing phase). Hash and sort-merge need equality conditions.

**Q4. What is query optimization? Describe techniques and the use of EXPLAIN and indexes. (4)** _Answer:_ Query optimization improves query efficiency by choosing a good execution strategy, to minimise time and CPU/memory/I/O. Techniques: use indexes wisely; select only needed columns instead of `SELECT *` ; join only required tables on indexed columns; filter early with WHERE; use LIMIT. Avoid functions on indexed columns in WHERE (non-sargable). `EXPLAIN` shows the access type, key used, rows examined and Extra. Without an index `type = ALL` , `key = NULL` (full scan, O(n)); with a B-tree index the type becomes `range` / `ref` and cost is O(log n) (e.g. 0.68 s to 0.00 s on a million rows).

**Q5. Define functional dependency. Explain its types with examples. (4)** _Answer:_ An FD X → Y is a constraint: if two tuples agree on X they must agree on Y. **Full:** removal of any attribute from X breaks it, {Ssn,Pnumber} → Hours. **Partial:** a part of X is enough, {Ssn,Pnumber} → Ename since Ssn → Ename. **Transitive:** X→Y, Y→Z give X→Z through a non-prime Y, Ssn → Dnumber → Dmgr_ssn. **Trivial:** Y ⊆ X, ABC → BC. **Non-trivial:** Y ⊄ X. FDs come from the meaning of the data and hold in every legal state.

**Q6. State Armstrong's axioms and the derived rules. Derive AC → D from {A→B, BC→D}. (4)** _Answer:_ Reflexive (Y ⊆ X ⇒ X→Y); Augmentation (X→Y ⇒ XZ→YZ); Transitivity (X→Y, Y→Z ⇒ X→Z). They are **sound and complete** . Derived: **decomposition** , **union** , **pseudotransitivity** . Derivation: (1) A→B given; (2) AC→BC augmentation of (1) with C; (3) BC→D given; (4) AC→D transitivity of (2),(3).

**Q7. Explain the algorithm for the closure of an attribute set. Find {B,C}⁺ for F = {A→BC, BC→DE, D→F, CF→G}. (4)** _Answer:_ Start X⁺ = X; repeatedly, for each FD Y→Z with Y ⊆ X⁺ add Z; stop when X⁺ does not change. {B,C}⁺: {B,C} → add D,E (BC→DE) → add F (D→F) → add G (CF→G) = **{B,C,D,E,F,G}** . Uses: key finding, normalization test, implication test (X→Y holds iff Y ⊆ X⁺).

**Q8. What is a minimal cover? Find it for {A→BCDE, CD→E}. (4)** _Answer:_ A minimal cover is an equivalent set with (1) single-attribute RHS, (2) no extraneous LHS attribute, (3) no redundant FD. Split: A→B, A→C, A→D, A→E, CD→E. In CD→E neither C nor D alone gives E,

so no extraneous attribute. A→CD and CD→E give A→E, so A→E is redundant. **Minimal cover = {A→B, A→C, A→D, CD→E}** i.e. {A→BCD, CD→E}.

**Q9. Explain insertion, deletion and update anomalies with EMP_PROJ. (4)** _Answer:_ EMP_PROJ(Ssn, Pnumber, Hours, Ename, Pname, Plocation) stores project details with every employee row. **Update:** renaming project 1 must be done in every row; missing one gives inconsistency. **Insertion:** a new project cannot be inserted until an employee is assigned (Ssn is part of the key). **Deletion:** deleting the only employee of a project deletes the project details. Cause: mixing entities and redundancy; cure: decompose into EP1(Ssn,Pnumber,Hours), EP2(Ssn,Ename), EP3(Pnumber,Pname,Plocation).

**Q10. Define 1NF, 2NF and 3NF with examples of decomposition. (4)** _Answer:_ **1NF:** all attributes atomic (no multivalued/composite/nested). **2NF:** 1NF and every non-prime attribute fully dependent on the key; EMP_PROJ is split to remove Ssn → Ename and Pnumber → Pname. **3NF:** 2NF and no non-prime attribute transitively dependent on the key; EMP_DEPT is split into ED1(Ename,Ssn,Bdate,Address,Dnumber) and ED2(Dnumber,Dname,Dmgr_ssn). General 3NF: for each X→A, X is a superkey or A is prime.

**Q11. Differentiate 3NF and BCNF with the TEACH example. (4)** _Answer:_ 3NF allows X→A if X is a superkey **or** A is prime; BCNF demands X be a superkey. TEACH(Student, Course, Instructor) with {Student,Course}→Instructor and Instructor→Course: Course is prime so 3NF holds, but Instructor is not a superkey so BCNF fails. Decompose into (Instructor, Student) and (Instructor, Course): lossless because Instructor → Course, but {Student,Course}→Instructor is no longer preserved.

**Q12. Explain lossless join and dependency preservation. State the test for binary decomposition. (4)** _Answer:_ Lossless: joining the pieces gives exactly R with no spurious tuples (mandatory). Dependency preserving: every FD appears in some piece (desirable, sometimes sacrificed in BCNF). **NJB test:** D = {R1,R2} is lossless iff (R1∩R2) → (R1−R2) or (R1∩R2) → (R2−R1) is in F⁺, i.e. the common attributes form a key of one of the pieces.

**Q13. Explain MVD and 4NF, JD and 5NF. (4)** _Answer:_ MVD X→→Y: for tuples agreeing on X, the Y values and Z values (Z = R−XY) occur in all combinations; 4NF requires X to be a superkey for each non-trivial MVD, e.g. EMP(Ename,Pname,Dname) split into EMP_PROJECTS and EMP_DEPENDENTS. JD(R1..Rn): R equals the join of its projections; MVD is a JD with n=2; 5NF requires each Ri to be a superkey, e.g. SUPPLY split into three binary relations. 5NF is rarely used because JDs are hard to detect.

**Q14. Show that two FD sets are equivalent. (4)** _Answer:_ F and G are equivalent if each covers the other. To check F covers G, compute X⁺ under F for every X→Y in G and verify Y ⊆ X⁺; repeat reversed. Example F = {A→C, AC→D, E→AD, E→H}, G = {A→CD, E→AH}: under F, A⁺={A,C,D}, E⁺={E,A,D,C,H}; under G the same closures arise; so F = G.

**Q15. How are candidate keys found? Example. (4)** _Answer:_ Find essential attributes (never on an RHS); they are in every key. If their closure is R they form the only key; else add non-essential attributes by combination. Example R(A..F), F = {C→F, E→A, EC→D, A→B}: essential C,E; {C,E}⁺ = R ⇒ CE the only key; superkeys = 2⁴ = 16.

## 3.13 Important Definitions, Facts, Formulae and Algorithms (Unit 3)

#### **Definitions**

- **Query optimization:** choosing a suitable execution strategy among many for a query. **Query block:** a single SELECT-FROM-WHERE (with optional GROUP BY, HAVING); basic unit of translation and optimization.

- **Semi-join (S=):** returns a left row once when it has at least one match; used for EXISTS/IN/ANY. **Anti-join (A=):** returns a left row only if it has **no** match; used for NOT EXISTS/NOT IN/ALL.

- **Functional dependency X → Y:** t1[X] = t2[X] implies t1[Y] = t2[Y] in every legal state. **Prime attribute:** member of some candidate key. **Non-prime:** not in any candidate key. **Full FD / Partial FD / Transitive FD / Trivial FD.**

- **Superkey:** X⁺ = R. **Candidate key:** minimal superkey.

- **X⁺:** attribute closure. **F⁺:** closure of FD set. F **covers** E if E ⊆ F⁺. **Equivalent:** E⁺ = F⁺.

- **Minimal cover:** single-attribute RHS, no extraneous LHS attribute, no redundant FD, equivalent to original.

- **Normalization:** decomposing relations using FDs and keys to minimise redundancy and anomalies.

- **1NF:** atomic values. **2NF:** 1NF and no partial dependency of non-prime on a key. **3NF:** each X→A has X superkey or A prime. **BCNF:** each X→A has X superkey. **4NF:** each non-trivial MVD has X superkey. **5NF:** each non-trivial JD has every Ri a superkey.

- **Lossless (non-additive) join** and **dependency preservation.**

- **Spurious tuple:** a false tuple generated by a lossy join.

#### **Formulae and counts**

|Item|Value|
|---|---|
|Max number of FDs over n attributes|2^(2n)|
|Subsets of n attributes|2^n|
|Superkeys when essential attributes (e of n)<br>form the only key|2^(n−e)|
|NJB lossless test|(R1∩R2) → (R1−R2) or (R1∩R2) → (R2−R1)<br>in F⁺|
|BCNF decomposition step|R → (R − A) and (X∪A) for violating FD<br>X→A|
|Union rule|X→Y, X→Z⇒X→YZ|
|Pseudo-transitivity|X→Y, WY→Z⇒WX→Z|
|Nested-loop join cost|about |R| × |S||
|Index-based nested-loop|about |R| × log|S||

**Algorithms to know by heart:** attribute closure (3.4.4); minimal cover (3.5.3); finding a key and all candidate keys (3.5.4); BCNF decomposition (3.8.8); NJB test (3.8.7); precedence of normal-form checking (find key → prime/non-prime → check each FD → decompose).

## 3.14 Practice MCQs: Unit 3

### 3.14.1 Two-mark MCQs (application and numeric)

**1.** R(A,B,C,D,E,F,G), F = {A→BC, BC→DE, D→F, CF→G}. What is {B,C}⁺? (a) {B,C,D,E} (b) {B,C,D,E,F} (c) {B,C,D,E,F,G} (d) {A,B,C,D,E,F,G} **Answer: (c).** BC→DE adds D,E; D→F adds F; CF→G adds G. A is never reached (no FD has A on a RHS).

**2.** R(A,B,C,D,E), F = {AB→C, C→D, B→E}. How many candidate keys and superkeys? (a) 1 and 8 (b) 2 and 8 (c) 1 and 16 (d) 3 and 4 **Answer: (a).** Essential attributes A, B; (AB)⁺ = ABCDE so AB is the only key; superkeys = 2^(5−2) = 8.

**3.** R(A,B,C,D,E,F), F = {C→F, E→A, EC→D, A→B}. Total superkeys? (a) 4 (b) 8 (c) 16 (d) 32 **Answer: (c).** Essential C, E; (CE)⁺ = R; superkeys = 2^(6−2) = 16.

**4.** The minimal cover of {A→BC, B→C, A→B, AB→C} is (a) {A→B, B→C} (b) {A→BC, B→C} (c) {A→C, B→C} (d) {A→B, A→C, B→C} **Answer: (a).** Split to A→B, A→C, B→C, AB→C. AB→C is redundant (B→C), A→C is redundant (A→B, B→C).

**5.** TEACH(Student, Course, Instructor), Instructor→Course, {Student,Course}→Instructor. Which decomposition is lossless? (a) {Student,Instructor},{Student,Course} (b) {Course,Instructor},{Course,Student} (c) {Instructor,Course},{Instructor,Student} (d) none **Answer: (c).** Common attribute Instructor and Instructor→Course holds (NJB). In (a) common = Student, in (b) common = Course: neither is a key of a piece.

**6.** R(A,B,C,D), F = {A→B, B→C, C→D, D→A}. Number of candidate keys and highest normal form? (a) 1, 3NF (b) 4, BCNF (c) 4, 3NF only (d) 2, BCNF **Answer: (b).** Each single attribute determines all others (cycle), so 4 keys; every determinant is a key, so BCNF.

**7.** R(A,B,C,D,E), F = {AB→C, C→D, D→E}. Highest normal form? (a) 1NF (b) 2NF (c) 3NF (d) BCNF **Answer: (b).** Key AB. No partial dependency (AB→C full; C,D non-prime). But C→D and D→E are non-prime → non-prime (transitive), so not 3NF.

**8.** R(A,B,C,D), F = {AB→C, C→D, D→A}. Highest normal form? (a) 2NF (b) 3NF but not BCNF (c) BCNF (d) 1NF **Answer: (b).** Keys are AB, BC and BD (so every attribute is prime). Every RHS is prime, so 3NF holds; but C→D has C not a superkey, so BCNF fails.

**9.** Two rows of T1 each match 3 rows of T2 on the join condition. How many result rows do a semi-join and an inner join give? (a) 2 and 6 (b) 6 and 6 (c) 2 and 2 (d) 3 and 6 **Answer: (a).** Semi-join returns each left row once (2); inner join returns every matching pair (2 × 3 = 6). **10.** For a relation with n = 3 attributes, the maximum number of possible FDs is (a) 9 (b) 16 (c) 64 (d) 512 **Answer: (c).** 2^(2n) = 2^6 = 64.

**11.** F = {A→B, B→C, A→C}, G = {A→B, B→C, A→D}. Which is true? (a) F = G (b) G covers F but F does not cover G (c) F covers G but G does not (d) neither covers the other **Answer: (b).** Under G: A⁺ = {A,B,C,D}, B⁺ = {B,C} so every FD of F follows. Under F: A⁺ = {A,B,C} lacks D, so A→D does not follow.

**12.** R(P,Q,R,S,T), FD = {PQ→R, S→T}. After 2NF decomposition (including a key relation), the relations are (a) (P,Q,R), (S,T) (b) (P,Q,R), (S,T), (P,Q,S) (c) (P,Q,R,S,T) (d) (P,Q,R), (S,T), (P,Q,S,T) **Answer: (b).** Key PQS; PQ→R and S→T are partial; add the key relation (P,Q,S).

**13.** EMP_PROJ has 16 rows. Joining EMP_LOCS and EMP_PROJ1 on Plocation returns 57 rows. How many spurious tuples? (a) 16 (b) 41 (c) 57 (d) 73 **Answer: (b).** 57 − 16 = 41. Cause: Plocation is not a key of either piece.

### 3.14.2 One-mark MCQs (concept and recall)

**1.** Which module identifies SQL keywords and attribute names as tokens? (a) Parser (b) Scanner (c) Optimizer (d) Code generator **Answer: (b).** Scanner = tokens; Parser = syntax.

**2.** A query graph is generally a (a) tree (b) cyclic graph (c) DAG (d) linked list **Answer: (c).** The query tree is a tree; the query graph is a directed acyclic graph.

**3.** A semi-join is used to unnest (a) NOT EXISTS (b) EXISTS (c) UNION (d) GROUP BY **Answer: (b).** NOT EXISTS uses an anti-join.

**4.** Binary search for SELECT needs (a) any attribute (b) equality on a key on which the file is ordered (c) a hash index (d) non-key range **Answer: (b).**

**5.** The index-based nested-loop join needs an index on (a) the outer join attribute (b) the inner relation's join attribute (c) the primary key only (d) none **Answer: (b).**

**6.** Which join method needs both inputs sorted on the join attributes? (a) Nested-loop (b) Sort-merge (c) Hash (d) Index **Answer: (b).**

**7.** In the partition-hash join the second phase is called (a) building (b) probing (c) merging (d) sorting **Answer: (b).** First phase is partitioning.

**8.** In EXPLAIN output, `type = ALL` indicates (a) index-only access (b) full table scan (c) constant lookup (d) range scan **Answer: (b).**

**9.** Which predicate is non-sargable (prevents index use)? (a) salary > 4900 (b) salary + 100 > 5000 (c) salary = 5000 (d) salary BETWEEN 1 AND 9 **Answer: (b).** The indexed column is inside an expression.

**10.** X→Y holds when tuples agreeing on X (a) also agree on Y (b) differ on Y (c) are identical (d) are unique **Answer: (a).**

**11.** Which is NOT one of Armstrong's three basic axioms? (a) Reflexive (b) Augmentation (c) Transitive (d) Union **Answer: (d).** Union is derived.

**12.** X⁺ is (a) set of FDs implied (b) set of attributes determined by X (c) set of keys (d) set of tuples **Answer: (b).** F⁺ is the set of FDs.

**13.** {Ssn,Pnumber} → Ename (with Ssn → Ename) is a (a) full FD (b) partial FD (c) transitive FD (d) trivial FD **Answer: (b).**

**14.** Which violates 1NF? (a) composite key (b) multivalued attribute (c) foreign key (d) NULL **Answer: (b).** Composite keys are allowed; composite/multivalued **attributes** are not.

**15.** 2NF removes (a) transitive dependencies (b) partial dependencies (c) multivalued dependencies (d) join dependencies **Answer: (b).**

**16.** 3NF removes (a) partial (b) transitive dependencies of non-prime attributes on the key (c) MVDs (d) trivial FDs **Answer: (b).**

**17.** BCNF requires the determinant of every non-trivial FD to be a (a) prime attribute (b) candidate attribute (c) superkey (d) foreign key **Answer: (c).**

**18.** Which statement is true? (a) every 3NF is BCNF (b) every BCNF is 3NF (c) BCNF and 3NF are equal (d) 2NF implies 3NF **Answer: (b).**

**19.** Which decomposition property is mandatory? (a) dependency preservation (b) lossless join (c) both optional (d) neither **Answer: (b).**

**20.** An MVD is a JD with n equal to (a) 1 (b) 2 (c) 3 (d) 4 **Answer: (b).**

**21.** Normal form defined using multivalued dependencies: (a) 3NF (b) BCNF (c) 4NF (d) 5NF **Answer: (c).** 5NF uses JDs.

**22.** 5NF is also called (a) PJNF (b) DKNF (c) BCNF (d) EKNF **Answer: (a).** Project-Join Normal Form.

**23.** Essential attributes are those that (a) appear on every LHS (b) appear on no RHS (c) are prime (d) are foreign keys **Answer: (b).** They are in every candidate key.

**24.** Deleting the only employee of a project also deletes the project. This is the (a) update (b) insertion (c) deletion anomaly (d) join anomaly **Answer: (c).**

**25.** Guideline 3 says relations should have (a) many NULLs (b) as few NULLs as possible (c) no keys (d) no foreign keys **Answer: (b).**

**26.** The trivial FD ABC → BC follows from the (a) union rule (b) reflexive rule (c) transitive rule (d) augmentation rule **Answer: (b).**

**27.** A minimal cover has (a) multi-attribute RHS (b) single-attribute RHS (c) only keys (d) only trivial FDs **Answer: (b).**

**28.** Which normal form is rarely used in practice because JDs are hard to detect? (a) 1NF (b) 3NF (c) BCNF (d) 5NF **Answer: (d).**

## 3.15 Compact Revision Sheet: Unit 3

**Query processing:** Scanner (tokens) → Parser (syntax) → Validator (schema) → Query tree/DAG → Optimizer (plan) → Code generator → Runtime processor. Query block = SELECT-FROM-WHERE (+GROUP BY/HAVING). EXISTS → semi-join; NOT EXISTS → antijoin.

**SELECT:** S1 linear, S2 binary (ordered key equality), S3a primary index (equality, key), S3b hash (equality, key), S4 primary index range.

**JOIN:** J1 nested-loop (|R|×|S|), J2 index nested-loop (index on inner), J3 sort-merge (both sorted), J4 partition-hash (partition smaller, probe with other).

**Optimization:** index wisely; no SELECT *; filter early; LIMIT; keep indexed column bare; `type=ALL` , `key=NULL` = full scan; index: O(n) to O(log n).

**FD:** X→Y. Full / partial / transitive / trivial. Instance can refute, never prove. Prime = in some candidate key.

**Armstrong:** Reflexive, Augmentation, Transitive (sound + complete). Derived: Decomposition, Union, Pseudo-transitivity. XY→A does not imply X→A.

**Closure:** loop until stable; X→Y iff Y ⊆ X⁺; superkey iff X⁺ = R; max FDs 2^(2n). **Equivalence:** F covers G if every X→Y of G has Y ⊆ X⁺ (under F); equivalent if both ways. **Minimal cover:** split RHS → remove extraneous LHS attributes → remove redundant FDs. **Keys:** essential (not on any RHS) in every key; closure = R; superkeys 2^(n−e) if single key. **Anomalies:** update (inconsistency), insertion (cannot add), deletion (lose other fact). Guidelines: clear semantics, less redundancy, few NULLs, no spurious tuples.

**Normal forms:** 1NF atomic; 2NF whole key; 3NF nothing but the key (X superkey **or** A prime); BCNF X superkey; 4NF MVD; 5NF JD.

**Decomposition:** lossless (mandatory, NJB) vs dependency preserving (desirable). BCNF algorithm: (R−A) and (X∪A). TEACH: 3NF not BCNF; D3 lossless; fd1 lost; D1 gives 19 rows vs 9.

**Numbers to remember:** 0.68 s → 0.00 s (million rows); 16 → 57 rows (41 spurious); TEACH 9 → 19 (10 spurious); SUPPLY 7 rows, 9 with R1�R2; superkeys 16, 8, 128.

# UNIT 4: Next-Gen Data Management

**Syllabus:** Database transactions; Concurrency control; Locking; Introduction to NoSQL databases; Key-value database; Graph databases (Neo4j); Introduction to Vector databases; Vector search with Relational and NoSQL databases; Web application development using Next.js and SQL.

## 4.1 Database Transactions

### 4.1.1 Single-user vs multi-user systems

A DBMS is **single-user** if at most one user can use it at a time, and **multi-user** if many users can access the database **concurrently** . Concurrency is what makes transactions and concurrency control necessary.

### 4.1.2 What is a transaction?

A **transaction** is a **unit of program execution** that **reads** one or more data items and **possibly updates (writes)** one or more data items. It is the **smallest unit of work that is either fully completed or fully rolled back** (no partial completion). A transaction may be written in SQL or in a host language (Java, C++) with embedded database access through **JDBC/ODBC** .

_Example:_ transfer of $50 from account A to account B. To the customer it is **one operation** ; inside the DBMS it is **several operations** , and it is essential that **either all occur or none occur** . Collections of operations forming a single logical unit of work are called transactions.

- A transaction is delimited by **begin transaction** and **end transaction** ; all operations between them belong to it.

- **Read-only transaction:** does not update the database. **Read-write transaction:** updates as well as reads.

- **Two main issues:** (1) **failures** of various kinds (hardware failures, system crashes) during a transaction; (2) **concurrent execution** of multiple transactions.

### 4.1.3 A simple transaction model: read(X) and write(X)

|Operation|Meaning|Steps inside the DBMS|
|---|---|---|
|**read(X)**|Transfers item X from the database to<br>a variable X in the transaction's**main-**<br>**memory buffer**|1) find the disk block holding X; 2)<br>copy that block into a buffer if not<br>already there; 3) copy X from the<br>buffer to the program variable|
|**write(X)**|Transfers the variable X from the<br>buffer to the database item X|1) find the disk block; 2) copy block<br>into buffer if needed; 3) copy X from<br>the program variable into the buffer;<br>4) store the updated block back on<br>disk**immediately or later**|

- A write need **not** reach the disk immediately; it may be held elsewhere and executed later (the notes assume immediate update for simplicity).

- **Read set** of a transaction = all items read; **write set** = all items written.

- **Data buffers:** the DBMS keeps buffers in main memory, each holding one disk block. When all are full a **buffer replacement policy** (commonly **LRU, Least Recently Used** ) selects a buffer to replace; a modified buffer is **written back to disk before reuse** .

**Why allow concurrent execution?** It is easier to run transactions serially, but concurrency gives **improved throughput and resource utilisation** and **reduced waiting time** .

### 4.1.4 Transaction execution states

```
   BEGIN
     |
     v
  [ ACTIVE ] ---- READ / WRITE operations ----
     |   \
  END|    \ failure / abort
     v     \
 [ PARTIALLY COMMITTED ] --- failure ---> [ FAILED ] --- rollback ---> [
TERMINATED (aborted) ]
     |
  checks pass (commit point)
     v
 [ COMMITTED ] ------------------------------------------------------> [
TERMINATED ]
```

|State|Meaning|
|---|---|
|**Active**|Entered immediately on start; READ and WRITE<br>are executed here|
|**Partially committed**|Transaction has**ended**; concurrency-control and<br>recovery protocols may do**additional checks**(e.g.<br>ensure a crash will not stop changes from being<br>recorded)|
|**Committed**|Checks succeeded:**commit point**reached; all<br>changes**must be recorded permanently**, even if a<br>system failure occurs|
|**Failed**|A check failed, or the transaction was**aborted**<br>while active|
|**Terminated**|Transaction leaves the system|

**After an abort there are two options:**

- **Restart** the transaction **only if** it was aborted because of a hardware/software error **not caused by its own logic** . A restarted transaction is a **new transaction** .

- **Kill** it, usually because of an **internal logical error** (fix needs rewriting the program), **bad input** , or **desired data not found** .

**Transaction operations (DBMS level):** BEGIN_TRANSACTION (start), READ/WRITE, END_TRANSACTION (read/write operations over), **COMMIT_TRANSACTION** (successful end; changes safely made permanent), **ROLLBACK/ABORT** (unsuccessful end; all effects must be **undone** ).

### 4.1.5 Transaction support in SQL

- **No explicit BEGIN TRANSACTION is needed** in standard SQL (a transaction starts implicitly). In MySQL you use `START TRANSACTION` / `BEGIN` , or run with `autocommit=0` .

- **Every transaction must have an explicit end statement: COMMIT or ROLLBACK.**

- **Access mode:** `READ WRITE` ( **default** ; allows SELECT, UPDATE, INSERT, DELETE, CREATE) or `READ ONLY` (data retrieval only; an UPDATE gives **ERROR 1792** in MySQL). (MySQL permits writes even at READ UNCOMMITTED; the SQL-92 rule that READ UNCOMMITTED implies READ ONLY is not applied.)

```
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE, READ WRITE;
START TRANSACTION;
  UPDATE account SET bal = bal - 50 WHERE id = 'A';
  UPDATE account SET bal = bal + 50 WHERE id = 'B';
COMMIT;          -- or ROLLBACK;
```

### 4.1.6 ACID properties

|Property|Meaning|Responsible<br>component|Fund-transfer<br>illustration|
|---|---|---|---|
|**A**tomicity|**All or nothing**: either all<br>operations are reflected<br>in the database or none|**Recovery system**(uses<br>the**log**, which records<br>old values on disk)|Failure after write(A)<br>and before write(B)<br>loses money; updates<br>of a partial<br>transaction must not<br>stay|
|**C**onsistency|Executing a transaction<br>**in isolation preserves**<br>**consistency**(explicit<br>constraints such as<br>primary/foreign keys,<br>and implicit ones). The<br>database may be<br>**temporarily**<br>**inconsistent during**the<br>transaction but must be<br>consistent**at**<br>**completion**|Programmer / integrity<br>checking|**A + B is unchanged**<br>by the transfer|
|**I**solation|Concurrent transactions<br>are**unaware of each**<br>**other**;**intermediate**<br>**results are hidden**|**Concurrency-control**<br>**system**|If T2 reads between<br>steps 3 and 6 it sees<br>A + B too small|
|**D**urability|After a transaction<br>completes successfully<br>(user notified), changes<br>**persist even after**<br>**failures**|**Recovery system**|Committed transfer<br>survives a crash|

**Fund-transfer transaction Ti:** 1. read(A) 2. A := A − 50 3. write(A) 4. read(B) 5. B := B + 50 6. write(B).

**Key:** Isolation can be achieved **trivially by serial execution** , but concurrency has large performance benefits, so the goal is a state **equivalent to some serial order** .

**Trap:** Atomicity and **durability** are both handled by the **recovery system** ; **isolation** by the **concurrency-control system** . In the slides' demonstration: at READ UNCOMMITTED the mid-transfer sum was **2950** , at READ COMMITTED it was **3000** .

**Slide Q&A (answers):** (1) persistence after crash = **Durability** . (2) failure after A updated but before B = system enters an **inconsistent state** . (3) appears as if no other transaction runs = **Isolation** . (4) transaction = a collection of operations that **either all occur or none occur** , ensuring data integrity.

## 4.2 Schedules and Serializability

### 4.2.1 Schedules

A **schedule** (history) S of n transactions is an **ordering of their operations** , interleaving allowed, such that **the order of operations of each Ti is preserved** . It is a **total order** over the operations. A schedule must contain **all** instructions of its transactions. A successful transaction ends with **commit** ; a failed one with **abort** .

### 4.2.2 Conflicting operations

Two operations conflict if **all three** hold: (1) they belong to **different transactions** ; (2) they access the **same data item** ; (3) **at least one is a write** .

|Pair|Conflict?|
|---|---|
|read(Q), read(Q)|**No**|
|read(Q), write(Q)|Yes (**read-write conflict**)|
|write(Q), read(Q)|Yes|
|write(Q), write(Q)|Yes (**write-write conflict**)|
|Different items (e.g. R1(A), W2(B))|No|
|Same transaction (e.g. r1(X), w1(X))|No|

_Slide examples:_ In Sa: r1(X); r2(X); w1(X); r1(Y); w2(X); w1(Y), the pairs r1(X)&w2(X), r2(X)&w1(X), w1(X)&w2(X) conflict; r1(X)&r2(X) do not (both reads); w2(X)&w1(Y) do not (different items).

A conflict **forces a logical order** ; non-conflicting adjacent operations of different transactions can be **swapped** without changing the result.

### 4.2.3 Banking example: schedules 1 to 4

T1 transfers $50 from A to B. T2 transfers **10 percent of the balance** of A to B. Initial A = $1000, B = $2000.

|Schedule|Execution|Final<br>A|Final<br>B|A + B|
|---|---|---|---|---|
|**1 (serial)**T1 then<br>T2|T1: 950, 2050; T2 moves 95|**855**|**2145**|3000✓|
|**2 (serial)**T2 then<br>T1|T2 moves 100: 900, 2100; T1<br>moves 50|**850**|**2150**|3000✓|
|**3 (concurrent)**|Non-serial but equivalent to<br>Schedule 1|855|2145|3000✓|
|**4 (bad**<br>**concurrent)**|Interleaving loses an update|**950**|**2100**|**3050** ✗(gained<br>$50)|

**Key:** Not every concurrent schedule is correct. Schedule 4 is **inconsistent** , so the concurrency-control component must ensure only **serializable** schedules run.

### 4.2.4 Serial vs serializable

||**Serial schedule**|**Serializable schedule**|
|---|---|---|
|Interleaving|**None**: one transaction completes<br>before the next starts|Operations interleaved (non-serial)|
|Concurrency|**None**: poor resource utilisation and<br>throughput|**Yes**: better CPU utilisation and<br>throughput|
|Correctness|Always consistent|Consistent because**equivalent to**<br>**some serial schedule**|

A non-serial schedule of n transactions is **serializable** if it is **equivalent to some serial schedule** of the same n transactions. Serial schedules limit concurrency, which is unacceptable in practice, so we **allow schedules that are equivalent to a serial one** .

### 4.2.5 Schedule equivalence

- **Result equivalent:** same final database state. May be **accidental** , so cannot define equivalence alone.

- **Conflict equivalent:** the **relative order of every pair of conflicting operations is the same** in both schedules.

- **View equivalent:** same final state and **same read/write behaviour** for all transactions (each transaction reads the same values).

**Conflict serializable:** a schedule that is **conflict equivalent to some serial schedule** (it can be converted into a serial schedule by swapping non-conflicting adjacent operations).

### 4.2.6 Testing conflict serializability: precedence graph (Algorithm 20.1)

1. Create a **node** Ti for each transaction.

2. For each **write_item(X) by Ti followed by read_item(X) by Tj** : edge **Ti → Tj** (writeread).

3. For each **read_item(X) by Ti followed by write_item(X) by Tj** : edge **Ti → Tj** (readwrite).

4. For each **write_item(X) by Ti followed by write_item(X) by Tj** : edge **Ti → Tj** (writewrite).

5. The schedule is **conflict serializable if and only if the precedence graph has no cycle.**

In short: list conflicting pairs → draw edge from the **earlier** to the **later** transaction → look for a cycle. If acyclic, a **topological order** of the graph gives an **equivalent serial schedule** . **Trap:** edge direction is **earlier → later** . A **cycle means NOT conflict serializable.** No cycle does not mean the schedule is serial.

### 4.2.7 Practice problems from the slides

|#|Schedule|Conflicting pairs and|Result|
|---|---|---|---|
|||edges||
|1|R1(A), R2(A), R1(B),|R2(A)–W1(A):**T2→T1**;|**Cycle T1↔T2: NOT conflict**|
||R2(B), R3(B), W1(A),|R1(B)–W2(B):**T1→T2**;|**serializable**|
||W2(B)|R3(B)–W2(B):**T3→T2**||

|#|Schedule|Conflicting pairs and<br>edges|Result|
|---|---|---|---|
|2|(4 transactions on X, Y)|**T2→T3, T2→T1, T3→T1,**<br>**T3→T4, T1→T4, T2→T4**|**No cycle: serializable**,<br>equivalent serial order**T2**<br>**→ T3 → T1 → T4**|
|3|(4 transactions on A, B)|**T4→T2, T3→T2, T1→T3,**<br>**T1→T2**|**No cycle: serializable**<br>(order e.g. T1, T4, T3, T2)|
|4|(2 transactions on A, B)|R1(A)–W2(A): T1→T2;<br>R2(A)–W1(A) and W2(A)–<br>W1(A): T2→T1; R2(B)–<br>W1(B): T2→T1; R1(B)–<br>W2(B) and W1(B)–W2(B):<br>T1→T2|**Cycle: NOT conflict**<br>**serializable**|

Another slide example: the non-serializable schedule of T3 and T4 cannot be swapped into either <T3,T4> or <T4,T3>.

**Slide Q&A (answers):** conflict = **same item and at least one is a write** ; serializable = **a non-serial schedule equivalent to some serial schedule** ; cycle in precedence graph = **not conflict serializable** .

## 4.3 Concurrency Control

### 4.3.1 What and why

**Concurrency control** is the procedure of **managing simultaneous transactions** , ensuring their **atomicity, isolation, consistency and serializability** . It restricts concurrent access so that there is **no erroneous update** . Control is achieved through **concurrency-control schemes** ; the most frequently used are **two-phase locking** and **snapshot isolation** .

**Why needed:** (1) **Prevent data conflicts** (incorrect or inconsistent updates when several transactions touch the same items); (2) **ensure data integrity** , avoiding **race conditions** where the outcome depends on timing.

**Methods of concurrency control:**

|Method|Idea|
|---|---|
|**Lock-based protocols**(e.g. two-phase|Shared locks for read, exclusive locks for|
|locking)|write; block conflicting access|
|**Timestamp-based protocols**|Order transactions by timestamps|
|**Optimistic concurrency control**|Execute without locks, validate before commit|

### 4.3.2 Problems caused by uncontrolled concurrency

|Problem|What happens|Slide demo|
|---|---|---|
|**Lost update**|Two transactions interleave<br>so one update is**overwritten**|Both read X = 90; T2 writes 92,<br>T1 writes 87 (correct final value<br>was 89);**T2's update is lost**|
|**Temporary update**<br>**(dirty read)**|A transaction reads an<br>**uncommitted**update of<br>another that later**fails and**<br>**rolls back**; the read value was<br>invalid|T2 (READ UNCOMMITTED)<br>reads 87 before T1 aborts and X<br>goes back to 90|
|**Incorrect summary**|An**aggregate**(e.g. SUM) is<br>computed while other<br>transactions are updating the<br>data, so it sees**partial**<br>**updates**|Mid-transfer SUM = 227 instead<br>of 230|
|**Unrepeatable (non-**<br>**repeatable) read**|A transaction reads the**same**<br>**item twice**and another<br>transaction changes it in<br>between, so the two reads<br>differ|READ COMMITTED re-read<br>changes 90 → 100; REPEATABLE<br>READ keeps 100|
|**Phantom**|A transaction re-runs a query<br>with a WHERE condition and<br>finds**new rows**inserted by<br>another transaction|Row count 2 → 3 under READ<br>COMMITTED|

**Trap:** Non-repeatable read = an **existing row's value changes** ; phantom = **new rows appear** (or disappear) in the result set.

### 4.3.3 Locking

A **lock** is a mechanism to control concurrent access to a data item. Lock requests go to the **concurrency-control manager** ; a transaction can proceed **only after the lock is granted** .

|Mode|Name|Allows|Requested with|
|---|---|---|---|
|**S**|**Shared**|Transaction can**read**but**not write**the item|lock-S(Q)|
|**X**|**Exclusive**|Transaction can**read and write**the item|lock-X(Q)|

A transaction may access an item **only if it holds a lock on it** . If the item is already locked in an **incompatible** mode, the requester **waits** until all incompatible locks are released.

**Lock-compatibility matrix** (can the requested mode be granted while another transaction holds the mode shown?):

|Requested\Held|**S**|**X**|
|---|---|---|
|**S**|**Yes**|No|
|**X**|No|No|

**Key:** Only **S with S** is compatible. Several shared locks may be held **simultaneously** ; an exclusive lock is held by **only one** transaction. _Slide demo:_ S + S granted at once; X(A) had to wait ~4.83 s until the S lock was released.

**Locking the whole database:** a policy of locking the entire database until commit allows only **one transaction at a time** and gives **poor concurrency** .

**Locking too early is not enough; unlocking too early is dangerous.** _Slide example:_ A = 100, B = 200. Serially T2 displays 300 (A + B). If T1 **unlocks B too early** , T2 sees an inconsistent state and displays **250** . (InnoDB, which holds the X lock until COMMIT, always showed 300.)

### 4.3.4 Locking protocol, legal schedules

A **locking protocol** is a set of rules followed by all transactions when requesting and releasing locks. A schedule is **legal under a protocol** if it can be generated by transactions obeying the protocol. A protocol **ensures serializability** if **all legal schedules under it are serializable** .

### 4.3.5 Deadlock

If transactions delay unlocking, a **deadlock** can occur: _Slide example:_ T3 holds lock-X(B) and then needs lock-X(A); T4 holds lock-S(A) and requests lock-S(B). **T4 waits for T3's lock on B while T3 waits for T4's lock on A. Neither can progress.** One transaction must **roll back** .

- **MySQL/InnoDB behaviour:** detects the deadlock automatically, returns **ERROR 1213** and **rolls back one** transaction (the victim) to break the cycle.

- **Avoiding deadlocks: update rows in a consistent order** in all transactions. **[Supplement]** Deadlock **detection** is done with a **wait-for graph** ; a cycle means deadlock.

### 4.3.6 Starvation

A transaction can wait **indefinitely** if others keep getting compatible locks. _Slide scenario:_ T2 holds S on an item; T1 requests X and waits; T3 requests S (compatible with T2) and is granted; T4 requests S before T3 releases... T1 is blocked indefinitely: **starvation** .

**Cure:** grant a lock request only if **(1) there is no other transaction holding a conflicting lock** and **(2) no other transaction is waiting for a lock on the item and made its request earlier** (a **first-come-first-served (FIFO)** lock manager). The slide demo confirmed T3's later S request also waits behind T1's X request.

### 4.3.7 Two-Phase Locking (2PL)

|Phase|Rule|
|---|---|
|**Phase 1: Growing**|A transaction**may obtain locks but may not**<br>**release any**|
|**Phase 2: Shrinking**|A transaction**may release locks but may not**<br>**obtain any new**lock|

A transaction starts in the growing phase; the **first release** moves it to the shrinking phase.

- The **lock point** is the point where the transaction acquires its **final lock** (end of growing phase). **Transactions can be serialized in the order of their lock points.** 2PL **ensures conflict-serializable schedules** .

- **2PL does NOT ensure freedom from deadlock.**

Basic 2PL does not by itself give **recoverability** or freedom from **cascading roll-back** , so extensions are used.

#### **Variants:**

||**Basic 2PL**|**Strict 2PL**|**Rigorous 2PL**|
|---|---|---|---|
|Locks<br>released|Any time in<br>shrinking phase|**All exclusive locks held**<br>**until commit/abort**|**All locks (shared and**<br>**exclusive) held until**<br>**commit/abort**|
|Guarantees|Conflict<br>serializability|Serializability +<br>**recoverability and no**<br>**cascading roll-backs**|Serializability;<br>transactions serialized**in**<br>**commit order**|
|Concurrency|Highest|Lower|Lowest (shared locks also<br>held to the end)|
|Deadlock|Possible|Still possible (not<br>reduced)|Still possible|
|Starvation|Possible|Possible|Possible (avoid with FIFO<br>lock manager)|

**Key:** Most real databases implement **rigorous 2PL** but simply call it two-phase locking. MySQL InnoDB uses **row-level locking** , suitable for multi-user, highly concurrent **OLTP** applications. _Slide demo:_ different rows update without waiting; the same row waits until T1 commits.

**[Supplement] Why strict matters:** if T1 wrote X and released the lock before committing, T2 could read the uncommitted value; if T1 then aborts, T2 must also roll back (a **cascading roll-back** ). Holding X locks until commit prevents this.

### 4.3.8 Lock conversions

Two-phase locking **with lock conversions** :

- **Growing phase:** may acquire lock-S, may acquire lock-X, and may **convert S to X (upgrade)** .

**Shrinking phase:** may release lock-S and lock-X, and may **convert X to S (downgrade)** .

This protocol still **ensures serializability** . _Slide demo:_ T1 upgraded lock-S(A) to lock-X(A); T2's lock-S(A) waited until T1 committed.

### 4.3.9 Implementation: lock manager and lock table

- The **lock manager** is a process that receives **lock and unlock messages** from transactions and replies with **lock-grant messages** (or a message asking the transaction to **roll back** in case of deadlock). The requester waits until answered.

- It maintains an in-memory **lock table** : for each data item a **linked list of requests** , each noting the **transaction, requested mode, and whether granted** . Dark boxes = **granted** , light boxes = **waiting** .

- A **new request** is added to the **end of the queue** and granted if **compatible with all earlier requests** .

- **Unlock** deletes the request and later requests are checked to see if they can now be granted.

- If a transaction **aborts** , **all its waiting or granted requests are deleted** .

- The manager may keep a list of locks held by each transaction for efficiency.

- This algorithm **guarantees freedom from starvation** , since a request is never granted while an **earlier** request waits. _Slide figure:_ five items I4, I7, I23, I44, I912; T23 holds locks on I912 and I7 and waits for I4.

- MySQL: `performance_schema.data_locks` shows granted X locks and waiting S requests.

**Slide Q&A (answers):** purpose of concurrency control = ensure **atomicity, isolation and serializability** ; the level that allows only committed reads but permits non-repeatable and phantom reads = **READ COMMITTED** .

## 4.4 Isolation Levels

**Isolation levels** control how transactions are isolated from each other, balancing **consistency** and **concurrency** . They are specified with `SET TRANSACTION ISOLATION LEVEL <level>` .

|Level|Meaning|
|---|---|
|**READ UNCOMMITTED**|**Lowest**isolation; may read**uncommitted**data|
|**READ COMMITTED**|Reads**only committed**data|
|**REPEATABLE READ**|Data read once**will not change**within the<br>transaction|

|Level|Meaning|
|---|---|
|**SERIALIZABLE**|**Highest**; complete isolation from other<br>transactions|

**Defaults:** the **SQL-standard default is SERIALIZABLE** ; **MySQL/InnoDB defaults to REPEATABLE READ** ; some systems use READ COMMITTED.

#### **Violations that can occur below SERIALIZABLE:**

- **Dirty read:** T1 reads an update of T2 that has **not yet committed** ; if T2 aborts, T1 has read a value that never existed.

- **Non-repeatable read:** T1 reads a value, T2 updates it, T1 reads again and sees a **different value** .

- **Phantom:** T1 reads rows satisfying a WHERE condition; T2 **inserts** a row that also satisfies it; the new row is a **phantom** . If the equivalent serial order is T1 then T2, the phantom should not be seen; if T2 then T1 it should. If the system cannot ensure the right behaviour it does not handle the phantom problem.

#### **Violation table (SQL standard):**

|Isolation level|Dirty read|Non-repeatable<br>read|Phantom|
|---|---|---|---|
|READ<br>UNCOMMITTED|**Possible**|**Possible**|**Possible**|
|READ COMMITTED|Not<br>possible|**Possible**|**Possible**|
|REPEATABLE READ|Not<br>possible|Not possible|**Possible**(per the<br>standard)|
|SERIALIZABLE|Not<br>possible|Not possible|Not possible|

**Trap:** The slides' MySQL demo shows an **InnoDB snapshot at REPEATABLE READ did not show the phantom (3 → 3)** , whereas READ COMMITTED did (2 → 3). In **exam tables follow the SQL standard** : phantom is still _possible_ at REPEATABLE READ. At SERIALIZABLE, the other session's SELECT **waits** for the first session's COMMIT.

**Slide demos:** READ UNCOMMITTED showed a dirty read (A+B = 2950 mid-transfer); READ COMMITTED showed only committed values (3000).

#### **Example (SERIALIZABLE hire-and-raise, MySQL form):**

```
DELIMITER //
CREATE PROCEDURE hire_and_raise(IN p_ssn CHAR(9), IN p_dno INT)
BEGIN
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN ROLLBACK; SELECT 'UNDO: transaction rolled back' AS status; END;
  SET TRANSACTION ISOLATION LEVEL SERIALIZABLE, READ WRITE;
  START TRANSACTION;
  INSERT INTO EMPLOYEE (Fname, Lname, Ssn, Dno, Salary)
         VALUES ('Robert', 'Smith', p_ssn, p_dno, 35000);
  UPDATE EMPLOYEE SET Salary = Salary * 1.1 WHERE Dno = p_dno;
  COMMIT;
  SELECT 'transaction committed' AS status;
END //
DELIMITER ;
```

This sets SERIALIZABLE (no dirty reads, non-repeatable reads or phantoms), performs an INSERT and an UPDATE, and **rolls back on any error** through the exit handler (the embedded-SQL `WHENEVER SQLERROR GOTO UNDO` ).

#### **Concurrency-control summary table:**

|Problem|Prevented by|
|---|---|
|Lost update, incorrect summary|Locking (S/X) / serializable behaviour|
|Dirty read|READ COMMITTED or higher|
|Non-repeatable read|REPEATABLE READ or higher|
|Phantom|SERIALIZABLE (standard)|

## 4.5 Introduction to NoSQL Databases

### 4.5.1 What is NoSQL?

**NoSQL ("not only SQL")** databases are **non-tabular** databases that store data differently from relational tables. Types by data model: **document, key-value, wide-column (columnbased), graph** .

**Why they emerged:** storage became cheap, so applications stored **far more data** in **all shapes** (structured, semi-structured, polymorphic) and **defining a schema in advance became nearly impossible** . NoSQL lets developers store **huge amounts of unstructured data with flexibility** . Most NoSQL systems are **distributed** databases/storage systems focusing on **semi-structured data, high performance, availability, replication and scalability** , rather than immediate consistency, powerful query languages and structured storage.

**Motivating example:** a free email service (Gmail, Yahoo Mail) with millions of users each holding thousands of messages. A relational system may be unsuitable because (1) SQL systems offer **too many services** (powerful query language, concurrency control) the application may not need, and (2) the relational model may be **too restrictive** . A NoSQL system with a **dynamic schema** is better.

#### **Industry systems (slides):**

|System|Origin / type|
|---|---|
|**BigTable**|Google;**column-based / wide-column**; used in<br>Gmail, Maps, web indexing|
|**HBase**|Apache open-source system with concepts similar<br>to BigTable|
|**DynamoDB**|Amazon;**key-value (key-object)**store on AWS|
|**Cassandra**|Facebook, now Apache; uses ideas of**both key-**<br>**value and column-based**|
|**MongoDB, CouchDB**|**Document stores**|
|**Neo4j, GraphBase**|**Graph-based**|
|**Redis**|Open-source,**in-memory**data-structure store<br>(database, cache, message broker)|

### 4.5.2 Characteristics related to distributed systems

1. **Scalability: horizontal scalability** (adding nodes) while the system is operating, so data must be redistributed among new nodes **without interrupting** operation.

2. **Availability, replication and eventual consistency:** data is **replicated on two or more nodes** so that if a node fails the data is still available; replication also improves **read performance** .

3. **Replication models:**

   - **Master-slave:** one master copy; **all writes go to the master** and propagate to slaves, usually with **eventual consistency** .

   - **Master-master:** reads and writes at **any replica** , but different copies may not always show the same values.

4. **Sharding (horizontal partitioning):** file records are **distributed across nodes** to spread the access load. **Sharding + replication** together improve **load balancing and availability** .

5. **High-performance data access:** find individual records among millions using **hashing** (location = h(K)) or **range partitioning** (location i holds keys in range Kimin ≤ K ≤ Kimax).

- 4.5.3 Characteristics related to data models and query languages

1. **No schema required:** flexible **semi-structured, self-describing** data (e.g. two Redis hashes with different fields).

2. **Less powerful query languages:** queries often **fetch single objects by key** ; systems expose a **programming API** of functions rather than SQL.

3. **Versioning:** some systems store **multiple versions of a data item with timestamps** .

### 4.5.4 Categories of NoSQL systems

|Category|Model|Examples|
|---|---|---|
|**Document-based**|Documents in formats such as**JSON**|**MongoDB, CouchDB**|
|**Key-value stores**|Simple model: fast access to a**value by**<br>**its key**; value can be a record, object,<br>document or complex structure|**DynamoDB, Redis**|

|Category|Model|Examples|
|---|---|---|
|**Column-based / wide-**<br>**column**|Table partitioned**by column into**<br>**column families**(vertical partitioning);<br>each family stored in its own files|**Cassandra, HBase**<br>(BigTable)|
|**Graph-based**|Data as**graph**; related nodes found by<br>**traversing edges**using**path**<br>**expressions**|**Neo4j**|

**Trap (slide Q&A):** among CouchDB, MongoDB, DynamoDB, Neo4j the **key-value** one is **DynamoDB** ; CouchDB and MongoDB are document stores; Neo4j is graph. Neo4j is **not** column-based.

### 4.5.5 CAP theorem

Three desirable properties of a **distributed system with replicated data** :

|Letter|Property|Meaning|
|---|---|---|
|**C**|**Consistency**|All nodes show the**same copy**of a replicated data<br>item|
|**A**|**Availability**|Every read/write request is either**processed**<br>**successfully**or gets a message that it cannot be<br>completed|
|**P**|**Partition tolerance**|The system**continues to operate**if a network fault<br>splits nodes into partitions that can talk only within<br>themselves|

**CAP theorem:** it is **not possible to guarantee all three at the same time** in a distributed system with data replication; the designer must **choose two of the three** .

- Traditional SQL applications emphasise **consistency (ACID)** .

- NoSQL data stores usually accept **weaker consistency** and guarantee **availability and partition tolerance** , adopting **eventual consistency** (replicas eventually become identical) instead of full serializability.

**Trap:** "A" in CAP = **Availability** (not Atomicity). "C" in CAP is about **replica agreement** , different from "C" in ACID (integrity-constraint consistency).

#### **ACID vs NoSQL tendencies:**

||Relational (SQL)|NoSQL|
|---|---|---|
|Schema|Fixed, defined in advance|Dynamic / schema-less|
|Scaling|Mostly vertical|**Horizontal**(sharding)|
|Consistency|Strong (ACID)|Often**eventual**|
|Query language|Powerful SQL, joins|Simple API, limited joins|
|Data|Structured|Semi/unstructured|

**Slide Q&A (answers):** key-value DB = **DynamoDB** ; A in CAP = **Availability** ; statements (i) dynamic schema and (iii) sharding distributes load are **correct** ; (ii) vertical scalability is **wrong** (NoSQL uses horizontal), (iv) Neo4j column-based is **wrong** .

## 4.6 Key-Value Databases (Redis)

### 4.6.1 Key-value store concept

The **simplest** NoSQL type. Data is stored as **key-value pairs** : the **key** is a unique identifier (like an SRN) used to locate an item; the **value** is the data itself (any format). Works like a **giant dictionary / hash map persisted on disk** . Usually **schema-less** ; offers **high performance, scalability and flexibility** .

**Use cases: caching** , **message queues using pub-sub** , **user preferences** , **session storage** . **Examples:** Redis, Amazon DynamoDB, CouchDB, Aerospike.

### 4.6.2 Redis: features

**Open-source, advanced key-value store** for high-performance, scalable web applications.

- **In-memory:** holds the database entirely in **RAM** ; disk only for **persistence** . **Versatile data structures:** strings, hashes, lists, sets, sorted sets and more. **Lua scripting** support (run custom code inside the server).

- **Replication and clustering:** scales horizontally with **Redis Cluster** ; replicates to any number of slaves ( **master-slave** architecture).

**Use cases (with slide examples):**

|Use case|Example|
|---|---|
|**Caching**frequently requested data to reduce<br>database load|Zomato popular restaurants|
|**Message broker**(asynchronous**pub/sub**)|Roblox notifications|
|**Streaming data processing / real-time analytics**|Xignite|
|**Session storage**(login status, shopping cart)|Amazon cart|
|**Game leaderboards**(sorted sets)|HackerRank leaderboard|

**Limitations: RAM is expensive** for large datasets; **manual memory management** (developers must configure **eviction policies** ); **potential data loss** on a crash since data is in RAM; **not suitable for complex queries** (joins, aggregations).

### 4.6.3 Redis basics: keys

- Each key is a **binary-safe string** ; each key must be **unique** .

- A **colon (:)** shows hierarchy (like namespaces): e.g. `PES:Department` is **one single key** . By default **values are strings** .

- _Example (user table):_ `HSET user:101 name "Alice" age 30` where the key `user:101` follows the pattern `user:<id>` .

#### **Key lifecycle commands:**

|Command|Description|Example|
|---|---|---|
|`EXPIRE key`|Sets a**timeout**; key is deleted after|`EXPIRE user:session:123`|
|`seconds`|it|`3600`|
|`TTL key`|Returns**remaining time to live**in<br>seconds|`TTL user:session:123`|
|`PERSIST key`|**Removes the timeout**, making the<br>key persistent|`PERSIST user:session:123`|
|`DEL key [key`|Deletes one or more keys|`DEL user:session:123`|
|`...]`||`temp:data:abc`|

### 4.6.4 Five Redis data structures

|Structure|Description|Use case|
|---|---|---|
|**String**|Binary sequence of bytes,**max 512 MB**|CRUD, caching,**atomic**<br>**counters**|
|**List**|Sequence of strings built on**linked lists**|Task lists, stacks/queues,<br>logging|
|**Hash**|Map of**field-value pairs**(like a<br>dictionary or JSON object)|Structured data, objects (user<br>profile)|
|**Set**|**Unordered collection of unique**strings|Unique items, tagging|
|**Sorted set**<br>**(ZSET)**|Set where every member has a**score**;<br>ordered by score (ties by lexicographic<br>order); like a priority queue|**Leaderboards, ranking**|

### 4.6.5 Redis command reference

#### **Strings**

|Command|Purpose|Example|
|---|---|---|
|`SET key value [EX`<br>`seconds]`|Set value, optionally with<br>expiry|`SET user:1:name "Alice" EX 3600`|
|`GET key`|Get value|`GET user:1:name`|
|`MSET k1 v1 k2 v2`|Set many pairs<br>**atomically**|`MSET user:1:name "Alice"`<br>`user:2:name "Bob"`|
|`MGET k1 k2`|Get many values|`MGET user:1:name user:2:name`|
|`INCR`/<br>`DECR key`|Add/subtract**1**to an<br>integer|`INCR page:views`|
|`INCRBY`/<br>`DECRBY key`<br>`n`|Add/subtract**n**|`DECRBY items:in_stock 5`|
|`APPEND key value`|Append to existing value|`APPEND log:file "Logged in"`|

#### **Lists**

|Command|Purpose|
|---|---|
|`LPUSH key e...`|Add to the**head (left)**|
|`RPUSH key e...`|Add to the**tail (right)**|
|`LPOP key`/<br>`RPOP key`|Remove and return**first / last**element|
|`LRANGE key start stop`|Get a range (**both ends inclusive**;<br>`0 -1`= whole<br>list)|
|`LLEN key`|Length|
|`BLPOP key [key ...] timeout`|**Blocking**LPOP; waits for an element|
|`LTRIM key start stop`|Trim list to the range|

#### **Hashes**

|Comm|and||Purpose|
|---|---|---|---|
|`HSET`|`key`|`field value [field value ...]`|Set fields|
|`HGET`|`key`|`field`|Get one field|
|`HGET`|`ALL`|`key`|All fields and values|
|`HDEL`|`key`|`field ...`|Delete fields|
|`HINC`|`RBY`|`key field n`|Increment integer field|
|`HLEN`|`key`||Number of fields|
|`HEXI`|`STS`|`key field`|**1 if present else 0**|

#### **Sets**

|Command|Purpose|
|---|---|
|`SADD key member ...`|Add members (duplicates ignored)|
|`SREM key member ...`|Remove members|
|`SMEMBERS key`|All members|
|`SCARD key`|Number of members|

|Command|Purpose|
|---|---|
|`SISMEMBER key member`|**1 if member else 0**|
|`SINTER`/<br>`SUNION`/<br>`SDIFF`|Intersection / union / difference|

#### **Sorted sets**

|Command|Purpose|
|---|---|
|`ZADD key score member ...`|Add/update (**score first, then member**)|
|`ZINCRBY key inc member`|Increase a score|
|`ZRANGE key start stop [WITHSCORES]`|Members**ascending**by score|
|`ZREVRANGE key start stop [WITHSCORES]`|Members**descending**|
|`ZRANK key member`|Rank**from lowest**(0-indexed)|
|`ZREVRANK key member`|Rank**from highest**(0-indexed)|
|`ZREM key member ...`|Remove|
|`ZCARD key`|Number of members|

**Trap:** `LPUSH` / `LPOP` act on the **left/head** ; `RPUSH` / `RPOP` on the **right/tail** . `ZADD` takes **score before member** . `ZRANK` is **ascending** , so rank 0 = lowest score; use `ZREVRANK` for the leaderboard position.

### 4.6.6 Redis hands-on case studies (with commands)

#### **Strings: user profile and session.**

|`MSET user:1:name "Vedant" user:2:name "Bob"     # create two names at once`|
|---|
|`MGET user:1:name user:2:name                      # fetch both`|
|`KEYS *                                            # list all keys`|
|`SET user:3:name "Alice" EX 60                     # temporary user, 60 s`|
|`expiry`|
|`APPEND user:2:name "Ellison"                      # BobEllison`|
|`SET user:1:height "120"`|
|`GET user:1:height`|
|`INCRBY user:1:height 10                           # becomes 130`|

#### **Lists: to-do list.**

|`RPUSH todolist "Do laundry"        # add normal task at end`|
|---|
|`LRANGE todolist 0 -1               # view all`|
|`LPUSH todolist "Call Dad"          # urgent task at top`|
|`LLEN todolist                      # how many tasks`|
|`LPOP todolist                      # remove top task`|
|`RPOP todolist                      # remove bottom task`|
|`RPUSH todolist "Buy groceries"`|
|`LTRIM todolist 0 0                 # keep only the first element`|

#### **Hashes: student management.**

```
HSET user:1 name "Vedant" age "21"
HGET user:1 name
HGETALL user:1
HSET user:2 name "Bob" age "23"
HINCRBY user:2 age 2               # age 25
HLEN user:2                        # number of fields (2)
HDEL user:2 age                    # remove age
HEXISTS user:2 age                 # 0 after delete
```

#### **Sets: RSVP.**

```
SADD rsvp "User A" "User B" "User C"
SMEMBERS rsvp
SADD rsvp "User A" "User D"        # User A not duplicated
SISMEMBER rsvp "User D"            # 1
SCARD rsvp                         # headcount
SREM rsvp "User D"
```

#### **Sorted sets: leaderboard.**

```
ZADD leaderboard 100 "player1" 200 "player2" 300 "player3"
ZCARD leaderboard
ZREVRANGE leaderboard 0 -1                 # high to low
ZREVRANGE leaderboard 0 -1 WITHSCORES
ZINCRBY leaderboard 500 "player1"          # now 600
ZRANK leaderboard "player1"                # position from lowest
ZREVRANK leaderboard "player1"             # position from highest (0 = top)
ZREM leaderboard "player1"                 # remove cheater
```

#### **Redis vs relational (compare):**

||Redis|RDBMS (MySQL/PostgreSQL)|
|---|---|---|
|Storage|**In RAM**(disk for persistence)|Disk based|
|Model|Key to value / data structures|Tables, rows, columns|
|Query|Commands by key,**no joins**|**SQL**with joins, aggregation|
|Speed|Very fast reads/writes|Slower, durable|
|Typical role|**Cache, sessions, queues, leaderboards**|**System of record**|

## 4.7 Graph Databases (Neo4j)

### 4.7.1 What is a graph database and Neo4j?

A **graph database** (graph-oriented NoSQL system) represents data as a **graph** : a collection of **nodes (vertices)** and **edges** .

- **Nodes** represent **entities** (Person, Product).

- **Edges (relationships)** represent links between nodes (FRIEND_OF, PURCHASED). **Neo4j** is an **open-source graph database implemented in Java** , queried with the **Cypher query language (CQL)** .

#### **Why Neo4j is used:**

- Relational databases struggle with **complex relationships** : too many **JOINs** .

- Neo4j is optimised for **connected data** and relationship-heavy queries; it follows **direct links** (fast traversals) instead of rebuilding joins.

- **Intuitive modelling** of real-world networks; **flexible schema** (easy to add nodes, edges, properties).

- **Cypher** gives simple, readable **pattern matching** .

- **Built-in graph algorithms:** shortest path, PageRank, community detection.

#### **Applications:**

|Area|Example|
|---|---|
|Social networks|Facebook, LinkedIn: friend suggestions, feeds|

|Area|Example|
|---|---|
|E-commerce/streaming|Amazon, Netflix:**recommendations**|
|Banking/finance|Mastercard:**fraud detection**by pattern analysis|
|Knowledge graphs|Google, NASA|
|IT/network management|Track dependencies across systems and<br>applications|

**Key advantage over relational (slide Q&A): efficient handling of highly connected data** (not faster disk I/O, not SQL support). Nodes typically represent **entities** such as people, places or things. The storage model is based on **nodes and relationships** .

### 4.7.2 Neo4j data model (building blocks)

|Concept|Meaning|Maps to (ER / relational<br>idea)|
|---|---|---|
|**Node**|Fundamental unit of a graph; holds<br>**properties**as key-value pairs|Entity|
|**Label**|Common name attached to a set of nodes<br>(or relationships); a node can have**zero or**<br>**more**labels|Entity type (and<br>subclasses)|
|**Relationship**|**Directed**connection with a**start node**,**end**<br>**node**and a**relationship type**; can have<br>properties|Relationship instance|
|**Relationship**<br>**type**|Type name (WorksFor, Manager, LocatedIn,<br>WorksOn)|Relationship type|
|**Property**|**Key-value pair**; key is a**string**; value is a<br>primitive or**array of one primitive type**|Attribute|
|**Path**|One or more nodes with connecting<br>relationships, typically a query or traversal<br>result|Path expression|

Nodes with the same label form a **collection** used for querying.

- A node can have **multiple labels** separated by colons: `PERSON:EMPLOYEE:MANAGER` . This resembles an entity belonging to a type PERSON plus subclasses EMPLOYEE and MANAGER.

- **Relationships are directed** (arrow →), but can be **traversed in either direction** . A relationship from Emp to Dept is an **outgoing** relationship for Emp and an **incoming** relationship for Dept. In the COMPANY example only **WorksOn** carries a property ( **Hours** ).

- **Properties** can be specified with a **map pattern** , e.g. `{Lname: 'Smith', Fname: 'John', Minit: 'B'}` .

- A **path** specifies a **traversal of part of the graph** , typically used as a **pattern** in a query; it is similar to path expressions in OQL and XPath/XQuery.

- **Optional schema:** graphs can be created **without a schema** ; from Neo4j 2.0 **indexes and constraints** (e.g. a **uniqueness (key-like) constraint** on a property of a label) can be defined.

- **Node identifiers:** Neo4j creates an **internal unique system-defined id** for each node (auto-generated, starts from zero in the slides). Users can create **indexes** on properties of nodes with a label (e.g. Empid for EMPLOYEE, Dno for DEPARTMENT, Pno for PROJECT) for efficient lookup.

#### **Neo4j vs ER model (slide table):**

|Neo4j|ER model|
|---|---|
|Relationship is**directed**|Relationship is**not directed**|
|A node**may have no label**|Every entity**must belong to an entity**<br>**type**|
|Graph model is the basis of an**actual high-**<br>**performance distributed database system**|ER/EER is**mainly used for database**<br>**design**|

**Data browser:** run CQL at the **`$` prompt** and click **Execute** . Results can be seen in **UI View** (graph diagram) or **Grid View** . **Grid View** exports to **CSV or JSON** ; **UI View** exports **JSON only** .

### 4.7.3 Cypher clause families

|Family|Clauses|
|---|---|
|**Read**|**MATCH**,**OPTIONAL MATCH**,**WHERE**,**LOAD CSV**<br>(START was removed in Neo4j 4.0)|
|**Write**|**CREATE**,**MERGE**,**SET**,**DELETE**,**REMOVE**,<br>**FOREACH**, CSV import (CREATE UNIQUE was<br>removed; use MERGE/constraints)|
|**General**|**RETURN**,**ORDER BY**,**SKIP**,**LIMIT**,**WITH**,**UNWIND**,<br>**UNION**,**CALL**|

#### **Operators (with the slide corrections):**

|Type|Operators|
|---|---|
|Mathematical|`+ - * / % ^`|
|Comparison|`=`,<br>`<>`(not equal;**no space**),<br>`<`,<br>`>`,<br>`<=`,<br>`>=`,<br>`IS`|
||`NULL`,<br>`IS NOT NULL`|
|Boolean|`AND`,<br>`OR`,<br>`XOR`(**true when exactly one side is**<br>**true**),<br>`NOT`|
|String|`=~`(regular-expression match),<br>`CONTAINS`,<br>`STARTS`|
||`WITH`,<br>`ENDS WITH`|
|List|`IN`,<br>`+`(concatenate)|

**Data types (Cypher values):** Boolean, Integer, Float, String, plus spatial/temporal types and **lists** . Property values may be primitives or **homogeneous arrays** ; **null is not a valid property value** (setting a property to NULL **removes** it); a collection with **mixed types cannot be stored** as a property.

### 4.7.4 Cypher syntax with the slide examples

#### **Nodes**

```
CREATE (n)                    // one node; n is a variable (identifier), not
the node's name
CREATE (n), (m)               // two nodes
MATCH (n) RETURN n;           // all nodes
MATCH (n) RETURN n LIMIT 2;   // first 2 nodes
MATCH (n) WHERE id(n) = 1 RETURN n;               // by internal id
MATCH (n) WHERE id(n) <= 6 RETURN n;              // comparison operators
MATCH (n) WHERE id(n) IN [1,2,0] RETURN n;        // IN
MATCH (n) WHERE id(n) = 1 DELETE n;               // delete one
MATCH (n) WHERE id(n) IN [2,3] DELETE n;          // delete several
MATCH (n) DELETE n;           // delete all nodes (only if no relationships
exist)
```

To remove the **entire database** the slides stop the server, remove the data directory, and restart it ( `sudo service neo4j stop` , delete the data files, `sudo service neo4j start` ; `status` checks running/stopped).

**Labels** (group nodes; optional; multiple allowed; CamelCase by convention; apply to **nodes** with `:` )

```
CREATE (n:Person)                      // one label
CREATE (n:Person:Indian)               // two labels
MATCH (n) WHERE n:Person RETURN n;     // nodes with label Person
MATCH (n) WHERE n:Person:Indian RETURN n;          // both labels
MATCH (n) WHERE n:Person OR n:Indian RETURN n;     // either
MATCH (n) SET n:Employee RETURN n;                 // add label to all nodes
MATCH (n) WHERE id(n) = 1 SET n:Manager RETURN n;  // add to one node
MATCH (n) WHERE id(n) IN [2,3] SET n:TeamLeader RETURN n;
MATCH (n) REMOVE n:Person RETURN n;                // remove label
MATCH (n) WHERE n:Manager REMOVE n:Manager SET n:Director RETURN id(n),
labels(n);  // rename label
MATCH (n) RETURN DISTINCT labels(n);               // list labels
MATCH (n:Director) RETURN id(n), labels(n);
MATCH (n) WHERE n:TeamLeader DELETE n;             // delete nodes by label
```

**Trap (slide correction):** `MATCH (n) RETURN DISTINCT count(labels(n))` counts **labelled nodes, not distinct labels** . To count distinct labels use `MATCH (n) UNWIND labels(n) AS lbl RETURN count(DISTINCT lbl)` .

#### **Properties**

```
CREATE (x:Book {title: "The Bengal Tiger"}) RETURN x;
// one property
CREATE (x:Book {title: "Database System", author: "Navathe", publisher:
"Pearson"}) RETURN x;
CREATE (x:Book {title: "the lives of others", author: "Neel",
                publisher: ["Chato & Windos", "w norton"],   // string array
                price: 285.00, pages: 528, instock: false}) RETURN x;   //
float, int, boolean
MATCH (n:Book {author: "MahiBhumi"}) RETURN n;
// filter by property
MATCH (n:Book) WHERE n.price < 1000 AND (n.author = "Neel" OR n.author =
"Navathe") RETURN n;
MATCH (n) WHERE n.title = "the lives of others" SET n.title = "The lives"
RETURN n;   // update
MATCH (n:Book {title: "The lives"}) SET n.author = "Gosh", n.price = 324.00,
n.stock = true RETURN n;  // many
MATCH (n:Book {author: "Neel"}) SET n.publisher = NULL RETURN n;   // removes
the property
MATCH (n) REMOVE n.pages RETURN n;                                  // remove
property from all nodes
MATCH (n) WHERE n.instock = false DELETE n;                         // delete
nodes by property
MATCH (n) WHERE id(n) = 1 REMOVE n.price RETURN n;
```

Property names are **case sensitive** and must start with a letter. `{author: "Navathe", Author: "Elmasri"}` creates **two different** properties. A mixed list such as `["the nagas", 172.00, 398]` **cannot** be stored; an all-string list `["the nagas","172.00","398"]` works.

**MERGE** (find the pattern; **create it only if it does not exist** )

```
MERGE (n:Player) RETURN n;
MERGE (CT2013:Tournament {name: "ICC Champions Trophy 2013"}) RETURN CT2013,
labels(CT2013);
```

#### **Relationships and paths**

```
CREATE (a:author {name:"MahiBhumi", dob:1980, age:41})
CREATE (b:Book {title:"AdiG", price:200, publication:"westland publication"})
MATCH (a:author {name:"MahiBhumi"}), (b:Book {title:"AdiG"})
CREATE (a)-[r:author_of]->(b)                       // relationship between
existing nodes

CREATE p = (n1 {..})-[:REL_TYPE]->(n2 {..})-[:REL_TYPE]->(n3 {..})
RETURN p                                            // a complete path
```

Format: `(node1)-[:RelationshipType]->(node2)` ; the relationship goes between the hyphen and the arrow inside square brackets. In a path template each relationship needs its leading `-` (slide correction).

#### **[Supplement] Traversal and deletion with relationships:**

```
MATCH (e:EMPLOYEE)-[:WorksFor]->(d:DEPARTMENT {Dname:'Research'}) RETURN
e.Lname;   // pattern matching
MATCH (e)-[w:WorksOn]->(p:PROJECT) WHERE w.Hours > 20 RETURN e.Lname,
p.Pname;   // relationship property
MATCH (n {name:'X'}) DETACH DELETE n;    // a node with relationships cannot
be DELETEd; DETACH DELETE removes them too
```

**Retrieval clauses recap:** `RETURN` (what to show), `LIMIT` (restrict rows), `ORDER BY` , `SKIP` , `DISTINCT` , `WHERE` (filter).

### 4.7.5 Neo4j vs relational model

||Neo4j (graph)|Relational database|
|---|---|---|
|Storage|Nodes, relationships, properties|Tables, rows, columns|
|Relationships|**First-class**, stored as direct links|Implied via**foreign keys and JOINs**|
|Schema|**Optional**/ flexible|Fixed|
|Query|**Cypher**pattern matching|**SQL**|
|Strength|**Highly connected data**, deep<br>traversals, shortest path|Structured, tabular data,<br>aggregation|
|Weakness|Less suited to bulk tabular<br>aggregation|Many JOINs get slow on deep<br>relationship queries|

**Slide Q&A (answers):** nodes represent **entities** (people, places, things); Neo4j's storage model is **nodes and relationships** ; key advantage is **efficient handling of highly connected data** .

## 4.8 Introduction to Vector Databases

### 4.8.1 Background: from SQL types to vectors

Relational databases store numbers, text, URLs, image metadata, **BLOBs** (images, video) and **CLOBs** (large character data such as PDF/Word text). Traditional SQL types are integer, float, Boolean and so on. Platforms are evolving to support **vectors** (float arrays from multimedia files) to serve **machine-learning** applications, usually as an **extension** rather than a default data type.

### 4.8.2 Full-text search vs vector (semantic) search

**Full-text search (FTS)** lets users efficiently search words or phrases in large text columns and returns **relevance-ranked** results (better than `LIKE` or regular expressions). **Vector search** goes further: it retrieves by **meaning** , not exact words, using **high-dimensional embeddings** (handles synonyms, context and intent).

||**Full-text search (FTS)**|**Vector database (semantic search)**|
|---|---|---|
|Matches|**Exact words or stems**|**Meaning**, using vector similarity|
|Works on|**Tokens (lexemes)**: "run" =<br>"running"|**Embeddings (dense vectors)**<br>capturing context|
|Index|**Inverted index**(word →<br>document IDs)|**Vector index**(HNSW, IVF): clusters<br>and distance search|
|Synonyms/context|Not understood unless added<br>explicitly|Handled naturally ("doctor" ≈<br>"physician")|
|Ranking|**TF-IDF, BM25**(term<br>frequency)|**Vector distance**(cosine similarity,<br>Euclidean)|
|Data types|Text mainly|**Text, images, audio**: anything<br>convertible to vectors|

### 4.8.3 Definition and idea

A **vector database** is a specialised system for **storing, indexing, querying and retrieving high-dimensional vector data** (embeddings of multimedia and text), enabling **similarity search** beyond traditional databases.

- **Analogy:** a super-smart librarian who knows how all books are connected by themes, or a magical sorter that, given your blue crayon, finds the closest crayons without searching the whole box.

- It **organises data by similarity or context** and finds information by **semantic relevance** rather than exact criteria.

- **Integration with AI:** combined with **LLMs** to build **Retrieval Augmented Generation (RAG)** systems.

- Examples: **Chroma, Weaviate** ; many companies add vector search to existing SQL/NoSQL databases.

**Characteristics:** store data as **high-dimensional embeddings** ; use **specialised indexing** (hashing, quantization, graph-based) for fast similarity search; ideal for **unstructured** data; efficient **semantically similar retrieval** ; scale with AI workloads (though computationally intensive).

**Capabilities and applications (slides):** efficient **similarity search** ; **high-dimensional data** (NLP, computer vision, genomics); **machine learning/AI** (store embeddings for clustering, classification, anomaly detection); **real-time** use (e-commerce recommendations, fraud detection, IoT monitoring); **personalisation** ; **spatial/geographic** data; **healthcare/life sciences** ; **data fusion** across text, image and numbers; **multilingual search** in a shared vector space.

### 4.8.4 What is a vector?

A **vector** is an ordered array of numbers. In maths/physics it has **magnitude and direction** (e.g. the 2-D vector (2.45, 1.28) is a point in 2-D space). In ML each number describes a **feature** of a data point; a vector is simply **a list of attributes of an object** (e.g. house = [bedrooms, bathrooms, area, price]).

**Slide example (flowers; petal length, petal width):**

|ID|Vector|
|---|---|
|FL1|(2.5, 1.6)|
|FL2|(1.1, 0.9)|
|FL3|(2.7, 0.4)|
|FL4|(2.7, 1.4)|
|FL5|(0.9, 1.3)|
|FL6|(0.5, 0.5)|
|FL7|(1.8, 0.9)|
|FL8|(0.4, 1.1)|

**FL1 and FL4 are the closest** (Euclidean distance ≈ **0.283** ), so they are the most similar. A single vector corresponds to **one object** (a word, image, video, document). A **vector embedding** is a vector **produced by an embedding model** . It is stored in the vector database **alongside the original content** it represents (page URL, image link, product SKU). _Product example:_ clusters of clothing, phones and board games sit far apart; within clothing, "black t-shirt" and "black polo shirt" are very close and "blue jeans" is farther.

### 4.8.5 Embeddings and embedding models

- An **embedding model** converts complex data (text, images, documents, audio, video) into **numerical vectors** that **retain the important features and contextual meaning** ; objects that are more similar lie **closer** in the vector space (e.g. "Boats" and "Ferries").

- Each dimension corresponds to some aspect of meaning (context, semantics, syntax). The procedure of generating vectors is called **embedding** .

- **Embedding models (vectorizers):** OpenAI **text-embedding-ada-002** , Google **embedding-gecko-001** , Meta/open-source models such as **sentence-transformers** (all-MiniLM-L6-v2, paraphrase-MiniLM-L6-v2).

- **Why not just store arrays in SQL?** Searching arrays in SQL is **memory-intensive and slow** : every search retrieves all vectors and computes cosine similarity. Image/video/audio vectors are much longer than text vectors. So most RDBMSs now provide a **wrapper/add-on (e.g. pgvector)** that adds a **vector data type** and **similarity operators** instead of manual distance calculation.

#### **Steps to generate embeddings for a relational table (slide):**

|Step|Action|
|---|---|
|1|**Extract**data (e.g. product description text) from<br>the relational table, e.g. in PostgreSQL|
|2|**Generate embeddings**with a sentence-<br>embedding model (sentence-transformers)|
|3|**Store**embeddings: use**pgvector**to add a vector<br>column and update each record|
|4|**Query using similarity**operators (like<br>`<=>`) to find<br>nearest neighbours|

#### **How a vector database works with embeddings:**

```
Raw data (text, image, audio, video; structured or unstructured)
        |
        v
 Embedding model (neural network)  --->  high-dimensional vector embeddings
        |
        v
 Stored (with original content/metadata) in the vector database + vector
index
        |
   user query --> embedded with the SAME model --> query vector
        |
        v
 Similarity search (nearest neighbours by distance metric) --> most relevant
results
```

### 4.8.6 Similarity metrics

|Metric|Idea|Formula|Interpretation|
|---|---|---|---|
|**Cosine**|Cosine of the**angleθ**|cosθ= (A·B) /|**Close to 1**(angle near 0) =|
|**similarity**|between two vectors|(|A| |B|)|very similar;**0**(angle 90°,<br>orthogonal) = unrelated|
|**Euclidean**|Straight-line distance|d = √Σ(aᵢ−|**Smaller = more similar**|
|**distance (L2)**||bᵢ)²||
|**Cosine**|1 − cosine similarity||**0 = identical direction**, range|
|**distance**|||0 to 2|

_Slide example:_ compare the new sentence **"Rainy days make me sad"** with a list of sentences using cosine similarity (model paraphrase-MiniLM-L6-v2). **"Rainy days can be gloomy"** scores highest (both express negative feelings about rain); **"Success is rewarding"** scores lowest (positive sentiment). Finding such similar entries quickly is exactly the goal of a vector database.

### 4.8.7 Vector search, indexing and ANN

Vector search **compares similarity between vectors** (each vector = an object) using a **distance metric** (Euclidean, cosine) and returns nearest, most relevant results. **Vector indexing** organises vectors (clustering/indexing) to speed retrieval on large datasets.

|Algorithm|Description|
|---|---|
|**KNN (k-nearest neighbours)**|Finds the**exact**nearest neighbours; compares<br>with every vector; good for small/medium<br>datasets (**FLAT / brute force**)|
|**ANN (approximate nearest neighbours)**|Finds**approximate**neighbours; far more<br>efficient on large, high-dimensional data|
|**HNSW (Hierarchical Navigable Small**<br>**World)**|Builds a**hierarchical, multi-layer graph**at<br>indexing time; search starts at a**sparse top**<br>**layer**and navigates down to denser layers; fast,<br>balances**accuracy and speed**|

**Trap:** KNN = exact (slow, O(N)); ANN/HNSW = approximate (fast, usually high recall). HNSW is a **graph-based** index.

**Vector DB vs traditional DB:** traditional DBs match **exact values** (keys, predicates) and sort/filter on structured columns; vector DBs return the **k most similar** items by **distance** in high-dimensional space.

## 4.9 Vector Search with Relational Databases (PostgreSQL + pgvector)

**Case study:** a **movie recommendation system** where each movie has a **title** , **synopsis** and an **embedding** generated from the natural-language description; we do CRUD and **semantic search** to recommend similar movies.

#### **Step 1: create the database**

```
CREATE DATABASE dbms_class;
\list            -- check
\c dbms_class     -- connect
```

#### **Step 2: enable pgvector, create the table and an HNSW index**

```
CREATE EXTENSION vector;

CREATE TABLE movies (
  id        SERIAL PRIMARY KEY,
  title     TEXT,
  synopsis  TEXT,
  embedding VECTOR(8)            -- 8-dimensional vector (demo; real models
use 384+)
);

CREATE INDEX ON movies USING hnsw (embedding vector_cosine_ops);   -- cosine;
use vector_l2_ops for Euclidean
```

- `VECTOR(8)` = column type holding **8 float values** .

- `hnsw` = **Hierarchical Navigable Small World** index for fast semantic search.

- `vector_cosine_ops` = index uses **cosine** distance; `vector_l2_ops` = **Euclidean (L2)** .

- psql helpers: `\dt` (tables), `\d movies` (schema), `SELECT * FROM movies;` .

#### **Step 3: insert rows (embeddings NULL for now)**

```
INSERT INTO movies (title, synopsis, embedding) VALUES
 ('Inception',   'A skilled thief leads a team into dreams to steal
secrets.', NULL),
 ('The Matrix',  'A hacker discovers the reality is a simulated world.',
NULL),
 ('Interstellar','Explorers travel through a wormhole in search of a new
home.', NULL),
 ('Ex Machina',  'A programmer interacts with an AI to test its
consciousness.', NULL);
-- (10 movies in the lab, including The Martian, Her, Avatar, Blade Runner
2049, ...)
```

#### **Steps 6 and 7: generate embeddings with Python and search**

- Install: `pip install psycopg2 sentence-transformers` ; test the connection first (update the password in the provided script).

- The model **all-MiniLM-L6-v2** produces the embedding of each synopsis. For the demo the vector is **restricted to 8 dimensions** (the model can give **384 or more** ).

- `psycopg2` (PostgreSQL connector) updates each row's `embedding` column.

- Then the query text (e.g. **"Movies about technology"** ) is embedded **with the same model** and the **top 3** nearest movies are fetched with cosine distance.

```
-- the similarity search used by the Python code
SELECT title, embedding <=> '[0.12, -0.05, ...]' AS distance
FROM   movies
ORDER  BY embedding <=> '[0.12, -0.05, ...]'
LIMIT  3;

SELECT id, embedding FROM movies;     -- verify the stored embeddings
```

#### **Operators and score interpretation:**

|pgvector operator|Meaning|
|---|---|
|**`<->`**|**Euclidean (L2) distance**|
|**`<=>`**|**Cosine distance**(not similarity)|
|`<#>`|Negative inner product**[Supplement]**|

Because the index was built with `vector_cosine_ops` , **`<=>` means cosine distance** . Range **0.0 (perfect match) to 2.0 (worst)** , so **smaller is better** . Cosine similarity = **1 − (embedding <=> query)** .

**Trap:** ORDER BY ascending distance returns the **most similar first** .

#### **[Supplement] minimal Python outline:**

```
from sentence_transformers import SentenceTransformer
import psycopg2
model = SentenceTransformer('all-MiniLM-L6-v2')
conn = psycopg2.connect(dbname='dbms_class', user='postgres', password='...',
host='127.0.0.1', port=5432)
cur = conn.cursor()
cur.execute('SELECT id, synopsis FROM movies')
for id_, text in cur.fetchall():
    vec = model.encode(text)[:8].tolist()          # demo: keep 8 dimensions
    cur.execute('UPDATE movies SET embedding = %s WHERE id = %s', (vec, id_))
conn.commit()
q = model.encode('Movies about technology')[:8].tolist()
cur.execute('SELECT title, embedding <=> %s::vector AS dist FROM movies ORDER
BY dist LIMIT 3', (q,))
```

**Why use a relational database for vectors:** one system for both **business data and embeddings** , SQL **filters + joins + transactions** (ACID) alongside similarity search.

## 4.10 Vector Search with NoSQL (Redis)

**Why NoSQL for vector search?** Embedding and search operations are **computationally intensive** ; **in-memory NoSQL stores such as Redis** are built for **low-latency reads/writes** for real-time results.

1. **Unified data model:** store the **vector embedding alongside operational metadata** (price, user ID); no separate vector DB.

2. **Built for AI scale: flexible schema** suits recommendation systems; supports RAG/LLM chatbots with **semantic caching** to cut cost and latency.

3. **Hybrid search:** combine **semantic vector search with precise metadata filtering** in a **single query** .

**Use cases:** e-commerce personalisation, geospatial analysis, recommendation systems, biometrics and anomaly detection, RAG/NLP/image/video recognition. **Examples: MongoDB Atlas Vector Search, Redis (Redis Stack), Azure Cosmos DB for NoSQL, Aerospike** .

### 4.10.1 Redis vector search

- Provided by **Redis Stack** , which bundles Redis with modules such as **RediSearch** .

- **VECTOR** is a native, **binary-safe** data type (faster than text strings).

- Indexes can combine **VECTOR** fields with **text, numeric and TAG** fields, enabling **hybrid search** .

Supports **ANN algorithms** letting you trade **speed, memory and accuracy** .

**Two index methods:**

||**FLAT**|**HNSW**|
|---|---|---|
|How|**Brute-force**: compare query with<br>**every**vector|**Multi-layered graph**: start at a sparse<br>top layer and navigate to denser layers|
|Accuracy|**100% exact**nearest neighbour|**Approximate**, recall typically**over 99%**|
|Speed|**Slow, O(N)**; only for**small datasets**|**Very fast (logarithmic)**, even with<br>billions of vectors|
|Use|Small data|**Standard production choice**|

### 4.10.2 Three-step process in Redis

#### **Step 1: create an index schema**

```
FT.CREATE idx:user_prefs ON JSON PREFIX 1 user:
  SCHEMA

    $.user             AS user             TEXT
    $.descr            AS descr            TEXT
    $.labels           AS labels           TAG
    $.vector_embedding AS vector_embedding VECTOR HNSW 6 TYPE FLOAT32 DIM 3
DISTANCE_METRIC COSINE
```

- `FT.CREATE` creates the index; `idx:user_prefs` is the **index name** .

- `ON JSON` = index JSON documents (HASH via HSET can also be used).

- `PREFIX 1 user:` = **one prefix** ; only keys starting with `user:` are indexed ( `user:1`

- yes, `session:1` no).

- `SCHEMA` = list of indexed fields (like table columns); `$.path` is the field path, followed

- by its **type** .

- **TAG** = **exact-match** , not tokenised (used for **filtering** ).

- `VECTOR HNSW` = vector field with the **HNSW** algorithm; `TYPE FLOAT32` ; **`DIM 3`** = 3

- dimensions; **`DISTANCE_METRIC COSINE`** .

**Step 2: add data and vectors** (vector passed as a **binary blob** , not a string, when searching)

```
JSON.SET user:1 $ '{"user":"samuel","descr":"Samuel likes mountain and kids
bikes.",
                    "labels":"mountain, kids","vector_embedding":[0.9, 0.7,
0.2]}'
```

Pattern: `JSON.SET <key> <path> <JSON data>` . Key `user:1` ; fields: user name, descr (text search), labels (tags for exact match), vector_embedding (3-D).

#### **Step 3: vector (semantic) search**

```
FT.SEARCH idx:user_prefs "(*)=>[KNN 2 @vector_embedding $input_vector]"
  PARAMS 2 input_vector <binary-blob-of-query-embedding>
  DIALECT 2
```

- `FT.SEARCH` (RediSearch) performs the query; `idx:user_prefs` = index name.

- `(*)` = **wildcard** : match all documents because we filter by **similarity** , not exact

- text/tag. (A hybrid query puts a filter there, e.g. `(@labels:{mountain})=>[KNN 2 ...]` .)

- **`KNN 2`** = return the **2 most similar** documents.

- `@vector_embedding` = the **vector field** indexed; `$input_vector` = **parameter**

- **placeholder** .

- `PARAMS 2 input_vector <blob>` = **2 items** (name and value) are passed; the blob is

- the model-generated query embedding.

- `DIALECT 2` = query dialect that supports vector search.

#### **Relational vs NoSQL vector search:**

||pgvector (PostgreSQL)|Redis Stack|
|---|---|---|
|Model|Relational +<br>`VECTOR(n)`column|In-memory NoSQL (JSON/hash) +<br>`VECTOR`field|
|Index|**HNSW**(and IVFFlat)|**FLAT**or**HNSW**|
|Query|SQL<br>`ORDER BY embedding <=> q`<br>`LIMIT k`|`FT.SEARCH ... KNN k ...`|
|Strength|ACID, joins, SQL filters|**Very low latency**, caching, hybrid<br>queries|
|Setup|`CREATE EXTENSION vector`|Redis Stack with RediSearch|

## 4.11 Web Application Development with Next.js and SQL

### 4.11.1 The Hello World application (MyApp)

- A simple app that shows a greeting when a user clicks **Get Greeting** . Flow:

1. **User** clicks **Get Greeting** on the page.

2. **Next.js** receives the request and coordinates data retrieval.

3. **Redis** is checked for a cached greeting.

4. **PostgreSQL** supplies the greeting if it is **not** in Redis.

5. **Redis** stores the greeting temporarily for **60 seconds** .

6. **Next.js** returns the greeting to the browser.

```
 Browser --GET /api/hello--> Next.js Route Handler
                                 |
                       redis.get("hello")
                          |hit            |miss
                          v                v
                 return cached      SELECT message FROM greetings ...
(PostgreSQL)
                 (source: Redis)         |
                                   redis.set("hello", msg, EX 60)
                                         |
                                   return msg (source: PostgreSQL)
```

**Key takeaway:** Next.js handles requests, **PostgreSQL stores persistent data** , **Redis speeds up repeated requests by caching** . Redis does **not** synchronise with PostgreSQL automatically; **application code decides when to read and update the cache** (this is the **cache-aside** pattern **[Supplement name]** ).

**Important terms: Cache hit:** value exists in Redis. **Cache miss:** value absent from Redis. **TTL (time to live):** time before a cached key expires.

### 4.11.2 Technology stack

|Component|Technology|Purpose|
|---|---|---|
|User interface|**Next.js (React)**|Displays the page and button|
|Backend/API|**Next.js Route**<br>**Handler**|Processes requests, coordinates data<br>access|
|Database|**PostgreSQL**|Stores the greeting**permanently**|
|Cache|**Redis**|Stores the greeting**temporarily**for<br>faster retrieval|
|DB connector|Node.js<br>**`pg`**package|Connects the app to PostgreSQL|
|Cache connector|Node.js<br>**`redis`**<br>package|Connects the app to Redis|
|Runtime/package<br>manager|**Node.js and npm**|Runs the app, manages dependencies|

**Next.js:** a **React framework** for web apps with **frontend and server-side** capabilities. Features: **server-side execution** , **Route Handlers** (API endpoints returning JSON), **React components** (interactive UI), dev/production tooling, and **file-based routing** (files and folders define routes).

**PostgreSQL:** open-source **relational DBMS** ; tables, **SQL** , **persistent** data across restarts, relationships/constraints/ **transactions** ; the **source of truth** .

**Redis: in-memory** key-value store; very fast reads/writes; **automatic key expiry with TTL** ; used for caching, sessions, counters, queues; reduces repeated database queries.

### 4.11.3 Setup and project structure

- Install **Node.js (LTS)** (npm included), **PostgreSQL** (set a password), **Redis** (Windows via **WSL + Ubuntu** : `sudo apt install redis-server` , `sudo service redis-server start` ; macOS: `brew install redis` , `brew services start redis` ), and **VS Code** . Check Redis with **`redis-cli ping`** , expected output **PONG** .

- Create the project: `npx create-next-app@latest .` (creates `app/` , `public/` , `package.json` , config files and installs React/Next.js). Run with **`npm run dev`** at

- **http://localhost:3000** .

Install connectors: **`npm install pg redis`** .

##### `MyApp/`

```
├── app/
│   ├── page.tsx            # UI page
│   └── api/hello/route.ts  # API endpoint (Route Handler)
├── lib/
│   ├── db.ts               # PostgreSQL connection pool
│   └── redis.ts            # Redis connection
├── .env.local              # local connection settings
└── package.json            # dependencies and commands
```

|File|Responsibility|
|---|---|
|`page.tsx`|Displays the user interface|
|`route.ts`|Handles the API request|
|`db.ts`|Creates the PostgreSQL**connection pool**|
|`redis.ts`|Connects to Redis|

|File|Responsibility|
|---|---|
|`.env.local`|Stores local connection settings|
|`package.json`|Defines dependencies and commands|
|`layout.tsx`|Shared page layout|

### 4.11.4 Database and environment

```
CREATE DATABASE myapp_db;
\c myapp_db
CREATE TABLE greetings (
  id      SERIAL PRIMARY KEY,
  message TEXT NOT NULL
);
INSERT INTO greetings (message) VALUES ('Hello World from PostgreSQL!');
SELECT * FROM greetings;
```

`.env.local` :

```
DATABASE_URL=postgresql://postgres:YOUR_PASSWORD@127.0.0.1:5432/myapp_db
REDIS_URL=redis://127.0.0.1:6379
```

PostgreSQL runs on port **5432** , Redis on **6379** , Next.js on **3000** . Environment variable changes require a **Next.js restart** .

### 4.11.5 Code

#### **lib/db.ts**

```
import { Pool } from "pg";
export const pool = new Pool({ connectionString: process.env.DATABASE_URL });
```

#### **lib/redis.ts**

```
import { createClient } from "redis";
const redis = createClient({ url: process.env.REDIS_URL });
redis.on("error", console.error);
export async function getRedis() {
  if (!redis.isOpen) { await redis.connect(); }
  return redis;
}
```

#### **app/api/hello/route.ts** (the cache-aside logic)

```
import { pool } from "@/lib/db";
import { getRedis } from "@/lib/redis";
export async function GET() {
  try {
    const redis = await getRedis();
    const cached = await redis.get("hello");                 // 1. check
cache
    if (cached) {
      return Response.json({ message: cached, source: "Redis" });   // cache
hit
    }
    const result = await pool.query(                          // 2. cache
miss: ask PostgreSQL
      "SELECT message FROM greetings ORDER BY id LIMIT 1"
    );
    if (result.rows.length === 0) {
      return Response.json({ error: "No greeting found" }, { status: 404 });
    }
    const message = result.rows[0].message;
    await redis.set("hello", message, { EX: 60 });          // 3. cache for
60 seconds
    return Response.json({ message, source: "PostgreSQL" });
  } catch (error) {
    console.error(error);
    return Response.json({ error: "Database or Redis connection failed" }, {
status: 500 });
  }
}
```

#### **app/page.tsx** (client component)

```
"use client";
import { useState } from "react";
export default function Home() {
  const [message, setMessage] = useState("");
  const [source, setSource] = useState("");
  async function getHello() {
    const response = await fetch("/api/hello");
    const data = await response.json();
    setMessage(data.message || data.error);
    setSource(data.source || "");
  }
  return (
    <main style={{ padding: 40, fontFamily: "Arial" }}>
      <h1>Hello World!</h1>
      <button onClick={getHello}>Get Greeting</button>
      <h2>{message}</h2>
      <p>Data source: {source}</p>
    </main>
  );
}
```

**Key points:** `"use client"` marks a **client component** (uses `useState` and event handlers); the page **fetches** **`/api/hello`** with a GET request; the route handler is **serverside** and talks to PostgreSQL and Redis; `{ EX: 60 }` sets a **60-second expiry** .

### 4.11.6 Running and verifying

1. `npm run dev` , open `http://localhost:3000` .

2. Click **Get Greeting** : shows **"Hello World from PostgreSQL!"** with **Data source: PostgreSQL** (cache miss).

3. Click again **within 60 seconds** : same text with **Data source: Redis** (cache hit).

4. After 60 s the key expires; the next click reads PostgreSQL again.

#### **Common errors:**

|Problem|What to check|
|---|---|
|PostgreSQL connection refused|Service status and**port 5432**|
|Password authentication failed|PostgreSQL**username and password**|

|Problem|What to check|
|---|---|
|Redis connection refused|Redis service and**port 6379**|
|Module not found|Run<br>**`npm install pg redis`**|
|Environment variable not updated|**Restart Next.js**|

## 4.12 Comparison Tables and Frequently Confused Concepts (Unit 4)

#### **Table 1: ACID properties and who enforces them**

|Property|Violated by|Enforced by|
|---|---|---|
|Atomicity|Crash midway through a transfer|Recovery system (log, rollback)|
|Consistency|Breaking constraints (e.g. CHECK<br>bal >= 0)|Integrity constraints + correct<br>transaction code|
|Isolation|Interleaving (dirty read, lost<br>update)|Concurrency-control system (locks)|
|Durability|Crash after commit loses data|Recovery system (log, persistent<br>storage)|

#### **Table 2: Types of schedule equivalence**

||Result equivalent|Conflict equivalent|View equivalent|
|---|---|---|---|
|Based|Same final DB state|**Same order of every**|Same final state**and**|
|on||**conflicting pair**|same reads|
|Reliable?|**No**(may be|**Yes**, testable by precedence|Yes (weaker, harder to|
||accidental)|graph|test)|

#### **Table 3: Shared vs exclusive lock**

||Shared (S)|Exclusive (X)|
|---|---|---|
|Operation|**Read**only|**Read and write**|

||Shared (S)|Exclusive (X)|
|---|---|---|
|Compatible with S?|**Yes**|No|
|Compatible with X?|No|No|
|Instruction|lock-S(Q)|lock-X(Q)|

#### **Table 4: Deadlock vs starvation**

||Deadlock|Starvation|
|---|---|---|
|Cause|**Circular wait**: each holds what the<br>other needs|A transaction**repeatedly**<br>**overtaken**by others|
|Everyone|The cycle's members are all|Only the starved transaction waits|
|blocked?|blocked||
|Resolution|**Roll back one**(victim); consistent<br>lock ordering|**FIFO / fair**lock manager|

#### **Table 5: Dirty read vs non-repeatable read vs phantom**

||Dirty read|Non-repeatable read|Phantom|
|---|---|---|---|
|What T1 sees|**Uncommitted**data<br>of T2|**Changed value**of a<br>row it read earlier|**New/missing rows**in a<br>repeated query|
|Operation by T2|UPDATE (not yet|UPDATE (committed)|INSERT/DELETE|
||committed)||(committed)|
|First level that|READ|REPEATABLE READ|SERIALIZABLE|
|prevents it|COMMITTED||(standard)|

#### **Table 6: SQL vs NoSQL (summary)**

||SQL (relational)|NoSQL|
|---|---|---|
|Schema|Fixed|Flexible, dynamic|
|Scalability|Vertical (mainly)|**Horizontal**(sharding, replication)|
|Consistency|**ACID**, strong|**Eventual**(CAP trade-off)|

||SQL (relational)|NoSQL|
|---|---|---|
|Queries|SQL, joins|API calls, limited queries|

#### **Table 7: Four NoSQL families**

|Family|Best for|Example|Unit of storage|
|---|---|---|---|
|Document|Semi-structured JSON content|MongoDB,<br>CouchDB|Document|
|Key-value|**Caching, sessions, fast lookups**|**Redis, DynamoDB**|Value by key|
|Wide-<br>column|Huge write-heavy tables|Cassandra, HBase|Column families|
|Graph|**Connected data**,<br>recommendations|**Neo4j**|Nodes,<br>relationships|

#### **Table 8: Redis structures: pick the right one**

|Need|Use|Key commands|
|---|---|---|
|Counter, cached value|String|SET, GET, INCR|
|Queue / stack / log|List|LPUSH, RPUSH, LPOP, RPOP, LRANGE|
|Object with fields|Hash|HSET, HGET, HGETALL|
|Unique members, tags|Set|SADD, SMEMBERS, SINTER|
|Ranking|Sorted set|ZADD, ZREVRANGE, ZREVRANK|

**Table 9: Neo4j terminology confusion**

|Term|Is|Not|
|---|---|---|
|`n`in<br>`CREATE`|A**variable**(identifier)|The node's name or label|
|`(n)`|||
|Label|Group tag for**nodes**(colon syntax)|A property|
|Relationship|Name of a relationship (WORKS_FOR)|A label on a node|
|type|||

|Term|Is|Not|
|---|---|---|
|`DELETE`|Removes nodes/relationships (**node must**<br>**have no relationships**)|`DETACH DELETE`removes<br>both|
|`SET n.p =`<br>`NULL`|**Removes**the property|Stores NULL|

#### **Table 10: Vector terms**

|Term|Meaning|
|---|---|
|Vector|Array of numbers (features)|
|Embedding|Vector**produced by an embedding model**|
|Embedding model|Converts text/image/audio into vectors (e.g. all-<br>MiniLM-L6-v2)|
|Cosine similarity|Closer to**1**= more similar|
|Cosine distance (<br>`<=>`)|**Smaller**= more similar; 0 to 2|
|KNN|**Exact**neighbours|
|ANN / HNSW|**Approximate**neighbours via layered graph|
|RAG|LLM combined with vector retrieval|

#### **Table 11: Cosine vs Euclidean**

||Cosine|Euclidean|
|---|---|---|
|Measures|**Angle**(direction)|**Straight-line distance**|
|Perfect match|similarity 1 (distance 0)|distance 0|
|pgvector operator|`<=>`|`<->`|
|Index ops class|`vector_cosine_ops`|`vector_l2_ops`|

#### **Common confusions:**

- **Trap:** Isolation is **not** the same as consistency; a schedule can be serializable yet violate an integrity constraint only if the transactions themselves are incorrect.

- **Trap:** Serial = no interleaving. Serializable = interleaved but **equivalent to serial** . **Trap:** 2PL gives **serializability, not deadlock freedom** .

**Trap:** Redis is **in-memory** ; data can be lost on a crash unless persistence is configured. **Trap:** Neo4j is a **graph** database, not column-based and not key-value.

- **Trap:** pgvector `<=>` is **distance** , not similarity.

- **Trap:** In the Next.js app, **Redis is not auto-synchronised** with PostgreSQL; the code manages the cache.

## 4.13 Likely 4-Mark Questions with Model Answers (Unit 4)

**Q1. Explain the states of a transaction with a diagram. (4)** _Answer:_ A transaction enters the **active** state on start and performs READ/WRITE. When it ends it moves to **partially committed** , where concurrency-control/recovery checks are done. If they succeed it reaches its commit point and enters **committed** (changes permanent). If a check fails or it is aborted while active it enters **failed** , then **terminated** (rolled back). A failed transaction may be restarted as a **new** transaction (only for hardware/software errors not due to its own logic) or killed (logic error, bad input). [Draw: Active → Partially committed → Committed → Terminated; Active/Partially committed → Failed → Terminated.]

**Q2. Explain the ACID properties with the funds-transfer example. (4)** _Answer:_ Transfer $50 from A to B: read(A); A := A−50; write(A); read(B); B := B+50; write(B). **Atomicity:** all steps happen or none; failure after write(A) would lose money, so the recovery system uses the log to undo. **Consistency:** A+B is unchanged and constraints hold. **Isolation:** a concurrent transaction must not see A reduced but B not yet increased (sum would be wrong); ensured by concurrency control. **Durability:** once committed, the transfer survives crashes, ensured by the recovery system.

**Q3. Define conflicting operations. How is conflict serializability tested? (4)** _Answer:_ Two operations conflict if they belong to different transactions, access the same item, and at least one is a write (RW, WR, WW). To test, build a **precedence graph** : a node for each transaction; an edge Ti→Tj for each conflicting pair where Ti's operation comes first. The schedule is conflict serializable **iff the graph has no cycle** ; a topological order gives an equivalent serial schedule.

**Q4. Check whether S: R1(A), R2(A), R1(B), R2(B), R3(B), W1(A), W2(B) is conflict serializable. (4)** _Answer:_ Conflicts: R2(A)–W1(A) gives T2→T1; R1(B)–W2(B) gives T1→T2; R3(B)–W2(B) gives T3→T2. The graph has a **cycle T1↔T2** , so S is **not conflict serializable** .

**Q5. Explain lock modes, compatibility and the two-phase locking protocol. (4)** _Answer:_ **Shared (S)** lock: read only; **exclusive (X)** lock: read and write. Compatibility: only S–S is compatible. **2PL:** growing phase (acquire only), shrinking phase (release only); once a lock is released no new lock can be acquired. 2PL guarantees conflict serializability (serial order = lock-point order) but **not freedom from deadlock** . **Strict 2PL** holds exclusive locks until commit/abort (no cascading rollbacks); **rigorous 2PL** holds all locks until commit/abort.

**Q6. What are deadlock and starvation? How are they handled? (4)** _Answer:_ **Deadlock:** T3 holds lock-X(B) and waits for A; T4 holds lock-S(A) and waits for B; neither proceeds, so one is **rolled back** (MySQL detects it and returns error 1213 and aborts one victim). Prevent by updating items in a consistent order. **Starvation:** an X request keeps waiting while a stream of compatible S requests is granted. A lock manager that grants a request only if it conflicts with no held lock and **no earlier waiting request** (FIFO queue in the lock table) guarantees freedom from starvation.

**Q7. Explain the problems caused by uncontrolled concurrent execution. (4)** _Answer:_ **Lost update:** two transactions overwrite each other's update. **Dirty read (temporary update):** a transaction reads uncommitted data that is later rolled back. **Incorrect summary:** an aggregate computed during updates reads partial results. **Unrepeatable read:** the same item read twice gives different values. (Also **phantom:** new rows appear in a repeated query.) Concurrency control prevents these.

**Q8. Describe SQL isolation levels and the anomalies each allows. (4)** _Answer:_ READ UNCOMMITTED (dirty reads, non-repeatable reads, phantoms possible); READ COMMITTED (no dirty reads; non-repeatable reads and phantoms possible); REPEATABLE READ (only phantoms possible per standard); SERIALIZABLE (none; highest isolation). Standard default is SERIALIZABLE; MySQL/InnoDB default is REPEATABLE READ. Set with `SET TRANSACTION ISOLATION LEVEL ...` .

**Q9. What are the characteristics of NoSQL systems? Explain the CAP theorem. (4)** _Answer:_ Distributed characteristics: **horizontal scalability, replication (masterslave/master-master) with eventual consistency, sharding, hashing or range partitioning** . Data-model characteristics: **no schema, simple query API, versioning** . **CAP:** Consistency, Availability, Partition tolerance; a replicated distributed system cannot guarantee all three at once, so NoSQL systems typically give up strong consistency for availability and partition tolerance (eventual consistency).

**Q10. Describe the categories of NoSQL systems with examples. (4)** _Answer:_ **Document** (JSON documents; MongoDB, CouchDB), **key-value** (fast access to a value by key; DynamoDB, Redis), **column-based/wide-column** (column families; Cassandra, HBase;

BigTable), **graph** (nodes and edges traversed by path expressions; Neo4j).

**Q11. Explain Redis: features, use cases, limitations and data structures. (4)** _Answer:_ Redis is an open-source, **in-memory** key-value store with strings, lists, hashes, sets and sorted sets, Lua scripting, replication (master-slave) and clustering. Uses: caching, message broker (pub/sub), real-time analytics, session storage, leaderboards. Limitations: expensive RAM, manual eviction policy, possible data loss on crash, poor for complex queries. Keys are unique binary-safe strings with `:` for hierarchy; `EXPIRE/TTL/PERSIST/DEL` manage lifetime.

**Q12. Write Redis commands for a leaderboard of players. (4)** _Answer:_ `ZADD leaderboard 100 "player1" 200 "player2" 300 "player3"` ; `ZCARD leaderboard` ; `ZREVRANGE leaderboard 0 -1 WITHSCORES` (high to low); `ZINCRBY leaderboard 500 "player1"` ; `ZREVRANK leaderboard "player1"` (position from top); `ZREM leaderboard "player1"` .

**Q13. Explain the Neo4j data model and write Cypher to create nodes and a relationship. (4)** _Answer:_ Nodes (entities) with optional **labels** (entity types) and **properties** (keyvalue); **directed relationships** with a type and optional properties; paths are traversals. Example: `CREATE (a:author {name:"MahiBhumi"}), (b:Book {title:"AdiG"})` then `MATCH (a:author {name:"MahiBhumi"}), (b:Book {title:"AdiG"}) CREATE (a)[r:author_of]->(b)` . `MATCH (n:Book) RETURN n` retrieves nodes.

**Q14. Compare Neo4j with the relational model and the ER model. (4)** _Answer:_ Neo4j stores **nodes and relationships** with direct links, so traversals avoid joins; relational databases use tables and foreign keys. Neo4j schema is **optional** , queried with Cypher; SQL is used for relational. Versus ER: Neo4j relationships are **directed** , nodes **may have no label** , and the graph model underlies an actual high-performance database, whereas ER relationships are undirected, every entity has a type, and ER is mainly for **design** .

**Q15. What is a vector database? Compare it with full-text search. (4)** _Answer:_ A vector database stores, indexes and searches **high-dimensional embeddings** to find items by **semantic similarity** . FTS matches **exact words/stems** with an **inverted index** ranked by **TF-IDF/BM25** , and does not understand synonyms; vector search uses **embeddings** and a **vector index (HNSW/IVF)** ranked by **cosine/Euclidean distance** , handles synonyms and context, and works for text, images and audio. Used for recommendations, QA and RAG.

**Q16. Explain how embeddings are generated and stored in PostgreSQL for semantic search. (4)** _Answer:_ Extract the text from a table; use an embedding model (sentencetransformers, e.g. all-MiniLM-L6-v2) to convert each row into a vector; `CREATE EXTENSION`

`vector` , add a `VECTOR(n)` column, and update rows; create an `hnsw` index with `vector_cosine_ops` ; embed the query with the same model and run `ORDER BY embedding <=> query LIMIT k` . `<=>` is cosine distance (0 best, 2 worst).

**Q17. Explain vector search in Redis. (4)** _Answer:_ Uses **Redis Stack (RediSearch)** with a native binary-safe VECTOR type. Indexing: **FLAT** (exact brute force, O(N), small data) or **HNSW** (multi-layer graph, approximate, fast). Steps: (1) `FT.CREATE` with a schema including `VECTOR HNSW ... DIM n DISTANCE_METRIC COSINE` ; (2) `JSON.SET` documents with embeddings; (3) `FT.SEARCH` with `KNN k @field $vec PARAMS 2 vec <blob> DIALECT 2` . Metadata fields (TAG) enable hybrid search.

**Q18. Describe the Next.js, PostgreSQL and Redis Hello World workflow. (4)** _Answer:_ The browser calls `GET /api/hello` . The Next.js route handler checks Redis for key `hello` . On a **cache hit** it returns the cached value (source Redis). On a **miss** it queries PostgreSQL ( `SELECT message FROM greetings ...` ), stores the result in Redis with `EX: 60` (TTL), and returns it (source PostgreSQL). PostgreSQL is the persistent store, Redis the cache; the application code manages the cache.

## 4.14 Important Definitions, Facts, Keywords and Formulae (Unit 4)

- **Transaction:** unit of program execution that reads/writes data items; all-or-none. **ACID:** Atomicity, Consistency, Isolation, Durability.

- **States:** active, partially committed, committed, failed, terminated.

- **Schedule:** order of interleaved operations preserving each transaction's own order.

- **Conflict:** different transactions, same item, at least one write.

- **Serializable schedule:** equivalent to some serial schedule. **Conflict serializable:** precedence graph acyclic.

- **Lock:** S (read) / X (read-write); **only S–S compatible** .

- **2PL:** growing then shrinking; ensures conflict serializability, **not** deadlock freedom. **Strict:** X locks until commit. **Rigorous:** all locks until commit.

- **Deadlock:** circular wait; one transaction rolled back. **Starvation:** indefinite waiting; FIFO lock manager prevents it.

- **Isolation levels:** RU < RC < RR < SERIALIZABLE. Standard default SERIALIZABLE; InnoDB default REPEATABLE READ.

- **Lost update, dirty read, incorrect summary, unrepeatable read, phantom.**

- **NoSQL:** non-tabular; document, key-value, wide-column, graph; horizontal scale; eventual consistency.

- **CAP:** Consistency, Availability, Partition tolerance; pick two.

- **Sharding:** distribute records across nodes. **Replication:** copies for availability (master-slave, master-master).

- **Redis:** in-memory key-value; strings (512 MB max), lists, hashes, sets, sorted sets; TTL keys.

- **Neo4j:** graph DB (Java); node, label, relationship (directed, typed), property, path; Cypher.

- **Vector:** array of numbers; **embedding:** vector from an embedding model; **cosine similarity = A·B / (|A||B|)** ; **cosine distance = 1 − similarity** ; **Euclidean = √Σ(aᵢ−bᵢ)²** .

- **KNN** exact; **ANN** approximate; **HNSW** hierarchical navigable small world graph index; **FLAT** brute force.

- **pgvector:** `VECTOR(n)` ; `<->` L2; `<=>` cosine distance; `hnsw` index with `vector_cosine_ops` .

- **Redis vector search:** `FT.CREATE` , `JSON.SET` , `FT.SEARCH ... KNN k ... PARAMS ... DIALECT 2` .

- **Next.js stack:** Next.js (React UI + Route Handler), PostgreSQL (persistent), Redis (cache, TTL 60 s), `pg` , `redis` packages; ports 3000/5432/6379.

**Numbers to remember:** $50 transfer; A=1000, B=2000 totals 3000; schedules 855/2145 and 850/2150; bad schedule 950/2100 (total 3050); S+S compatible only; 512 MB max string; FL1–FL4 distance 0.283; cosine distance range 0 to 2; Redis TTL 60 s.

## 4.15 Practice MCQs: Unit 4

### 4.15.1 Two-mark MCQs (application and numeric)

**1.** A = 1000, B = 2000. T1 transfers $50 from A to B; T2 transfers 10% of A's balance to B. Final A and B after the serial schedule T1 then T2? (a) 850, 2150 (b) 855, 2145 (c) 950, 2100 (d) 900, 2100 **Answer: (b).** T1: A=950, B=2050; T2 moves 95: A=855, B=2145. Order T2 then T1 would give 850, 2150.

**2.** Schedule S: R1(A), R2(A), R1(B), R2(B), R3(B), W1(A), W2(B). Which is true? (a) conflict serializable, order T3 T1 T2 (b) not conflict serializable (c) serial (d) conflict serializable, order T1 T2 T3 **Answer: (b).** Edges T2→T1 (R2(A), W1(A)) and T1→T2 (R1(B), W2(B)) form a cycle.

**3.** Schedule: r1(A), w1(A), r2(A), w2(A), r1(B), w1(B), r2(B), w2(B). Which is correct? (a) not serializable (b) conflict serializable, equivalent serial order T1 then T2 (c) equivalent to T2 then T1 (d) deadlocked **Answer: (b).** Every conflicting pair has T1 first, so the only edge is T1→T2; no cycle.

**4.** A session counts 2 rows with a condition; another session inserts a matching row and commits; the first session repeats and gets 3. Which anomaly and which is the lowest standard level that prevents it? (a) dirty read, READ COMMITTED (b) non-repeatable read, REPEATABLE READ (c) phantom, SERIALIZABLE (d) lost update, READ COMMITTED **Answer: (c).** New rows appearing is a phantom; the standard prevents it only at SERIALIZABLE.

**5.** Under a FIFO lock manager: T1 holds S(A). In order T2 requests S(A), T3 requests X(A), T4 requests S(A). How many of the three requests are granted immediately? (a) 0 (b) 1 (c) 2 (d) 3 **Answer: (b).** T2 is compatible and granted. T3 conflicts and waits. T4 is compatible with the held lock but an earlier request (T3) is waiting, so it queues (prevents starvation of T3).

**6.** `ZADD lb 100 a 200 b 300 c` then `ZINCRBY lb 500 a` then `ZREVRANK lb a` . Output? (a) 0 (b) 1 (c) 2 (d) 3 **Answer: (a).** a becomes 600, the highest; ZREVRANK counts from the top starting at 0.

**7.** `RPUSH t x LPUSH t y RPUSH t z LPOP t LRANGE t 0 -1` . Output? (a) y x z (b) x z (c) x (d) z x **Answer: (b).** After the pushes the list is [y, x, z]; LPOP removes y; remaining [x, z].

**8.** `SET h 120 INCRBY h 10 DECR h GET h` . Output? (a) 129 (b) 130 (c) 131 (d) 119 **Answer: (a).** 120 + 10 = 130, minus 1 = 129.

**9.** In pgvector the query `embedding <=> q` returns 0.2 for a movie. Its cosine similarity is (a) 0.2 (b) 0.8 (c) 1.2 (d) −0.2 **Answer: (b).** Similarity = 1 − cosine distance = 0.8.

**10.** Euclidean distance between vectors (1, 2) and (4, 6)? (a) 3 (b) 4 (c) 5 (d) 7 **Answer: (c).** √(3² + 4²) = √25 = 5.

**11.** Cosine similarity of (1, 0) and (0, 1)? (a) 1 (b) 0 (c) −1 (d) 0.5 **Answer: (b).** Dot product 0; the vectors are orthogonal (90°).

**12.** `CREATE (a:Person {name:'A'}), (b:Person:Indian {name:'B'}), (c:Indian {name:'C'})` then `MATCH (n) WHERE n:Person:Indian RETURN n` . How many nodes are returned? (a) 0 (b) 1 (c) 2 (d) 3 **Answer: (b).** Only B carries both labels.

**13.** `HSET user:2 name "Bob" age "23"` , `HINCRBY user:2 age 2` , `HLEN user:2` , `HDEL user:2 age` , `HEXISTS user:2 age` . The final output? (a) 1 (b) 0 (c) 25 (d) 2 **Answer: (b).** After HDEL the field is gone, so HEXISTS returns 0.

### 4.15.2 One-mark MCQs (concept and recall)

**1.** Atomicity and durability are ensured by the (a) concurrency-control system (b) recovery system (c) query optimizer (d) parser **Answer: (b).**

**2.** Isolation is ensured by the (a) recovery system (b) concurrency-control system (c) buffer manager (d) compiler **Answer: (b).**

**3.** After END but before the commit point, a transaction is in the (a) active (b) partially committed (c) failed (d) terminated state **Answer: (b).**

**4.** A transaction aborted because of an internal logical error is usually (a) restarted (b) killed (c) committed (d) ignored **Answer: (b).** Restart is for hardware/software errors not due to its own logic.

**5.** Two operations conflict if they are from different transactions, on the same item, and (a) both are reads (b) at least one is a write (c) both are commits (d) on different items **Answer: (b).**

**6.** A cycle in the precedence graph means the schedule is (a) serial (b) conflict serializable (c) not conflict serializable (d) recoverable **Answer: (c).**

**7.** The only compatible pair of lock modes is (a) S–X (b) X–X (c) S–S (d) X–S **Answer: (c).**

**8.** In two-phase locking, after the first unlock the transaction (a) can acquire new locks (b) cannot acquire new locks (c) must abort (d) is committed **Answer: (b).** It enters the shrinking phase.

**9.** Basic 2PL does NOT guarantee (a) conflict serializability (b) freedom from deadlock (c) serial order by lock point (d) legal schedules **Answer: (b).**

**10.** Strict 2PL holds (a) no locks (b) exclusive locks until commit/abort (c) shared locks only (d) locks until the next transaction **Answer: (b).** Rigorous 2PL holds all locks.

**11.** Starvation in a lock manager is avoided by (a) random granting (b) FIFO granting (c) lock upgrading (d) larger buffers **Answer: (b).**

**12.** MySQL InnoDB resolves a deadlock by (a) hanging both (b) rolling back one transaction (c) committing both (d) restarting the server **Answer: (b).** (ERROR 1213.)

**13.** A phantom is caused by (a) an update of a read row (b) newly inserted or deleted rows matching a condition (c) a crashed disk (d) a dropped index **Answer: (b).**

**14.** The default isolation level of MySQL InnoDB is (a) READ UNCOMMITTED (b) READ COMMITTED (c) REPEATABLE READ (d) SERIALIZABLE **Answer: (c).** SQL-standard default is SERIALIZABLE.

**15.** Which level allows dirty reads? (a) READ UNCOMMITTED (b) READ COMMITTED (c) REPEATABLE READ (d) SERIALIZABLE **Answer: (a).**

**16.** The "A" in CAP stands for (a) Atomicity (b) Availability (c) Accuracy (d) Access **Answer: (b).**

**17.** Which is a key-value store? (a) MongoDB (b) Neo4j (c) DynamoDB (d) CouchDB **Answer: (c).**

**18.** Distributing records of a collection across nodes is called (a) replication (b) sharding (c) caching (d) indexing **Answer: (b).** Horizontal partitioning.

**19.** Redis stores its working data primarily in (a) disk (b) RAM (c) tape (d) cloud only **Answer: (b).**

**20.** The maximum size of a Redis string value is (a) 1 MB (b) 64 MB (c) 512 MB (d) 4 GB **Answer: (c).**

**21.** Which Redis command removes the expiry from a key? (a) TTL (b) EXPIRE (c) PERSIST (d) DEL **Answer: (c).**

**22.** Which Redis structure suits a leaderboard? (a) list (b) set (c) hash (d) sorted set **Answer: (d).**

**23.** `LPUSH` adds an element to the (a) tail (b) head (c) middle (d) random position **Answer: (b).**

**24.** Neo4j's query language is (a) SQL (b) Cypher (c) SPARQL (d) XQuery **Answer: (b).**

**25.** In Neo4j a relationship (a) is undirected (b) has a direction, a type and optional properties (c) cannot have properties (d) is a label **Answer: (b).**

**26.** The not-equal operator in Cypher is (a) != (b) <> (c) =/= (d) NOT= **Answer: (b).**

**27.** A vector embedding is (a) a SQL table (b) a vector produced by an embedding model (c) an index type (d) a foreign key **Answer: (b).**

**28.** In pgvector, `<=>` is the (a) Euclidean distance (b) cosine distance (c) inner product (d) Hamming distance **Answer: (b).** `<->` is Euclidean.

**29.** HNSW stands for (a) Hashed Node Search Window (b) Hierarchical Navigable Small World (c) High-Normalised Search Weights (d) Hybrid Network Storage Wrapper **Answer: (b).**

**30.** Which Redis vector index is exact brute force? (a) HNSW (b) FLAT (c) TAG (d) TEXT **Answer: (b).**

**31.** In the Next.js app, the API logic lives in (a) `page.tsx` (b) `app/api/hello/route.ts` (c) `.env.local` (d) `package.json` **Answer: (b).**

**32.** The greeting is cached in Redis for (a) 6 s (b) 60 s (c) 600 s (d) forever **Answer: (b).** `EX: 60` .

## 4.16 Compact Revision Sheet: Unit 4

**Transactions:** unit of work (read/write); states Active → Partially committed → Committed → Terminated; Failed → Terminated. Restart (system error) vs kill (logic error). SQL: START TRANSACTION, COMMIT/ROLLBACK, READ ONLY/READ WRITE.

**ACID:** A all-or-none (recovery/log), C preserves constraints, I hidden intermediate state (concurrency control), D survives crash (recovery).

**Schedules:** conflict = different Tx + same item + at least one write. Serial (no interleave) vs serializable (equivalent to serial). Precedence graph edge earlier→later; acyclic iff conflict serializable. Bad schedule gives 950/2100.

**Locking:** S = read, X = read-write; only S–S compatible. 2PL = growing then shrinking; lock point; serializable not deadlock-free. Strict = X until commit; rigorous = all until commit. Lock table: queue per item, FIFO, no starvation. Upgrade S→X in growing, downgrade X→S in shrinking.

**Problems:** lost update, dirty read, incorrect summary, unrepeatable read, phantom. **Isolation levels:** RU (all three), RC (non-repeatable + phantom), RR (phantom per standard), SERIALIZABLE (none). Standard default SERIALIZABLE; InnoDB RR.

**NoSQL:** non-tabular; document / key-value / column / graph; horizontal scale, replication (master-slave, master-master), sharding, hash/range partitioning; no schema; CAP (pick two); eventual consistency. Examples: MongoDB/CouchDB, DynamoDB/Redis, Cassandra/HBase, Neo4j.

**Redis:** in-memory; String (512 MB), List, Hash, Set, Sorted set. Keys unique, `:` hierarchy; EXPIRE/TTL/PERSIST/DEL. Commands: SET/GET/MSET/MGET/INCR/INCRBY/APPEND; LPUSH/RPUSH/LPOP/RPOP/LRANGE/LLEN/LTRIM/BLPOP; HSET/HGET/HGETALL/HDEL/HINCRBY/HLEN/HEXISTS; SADD/SREM/SMEMBERS/SCARD/SISMEMBER; ZADD (score

first)/ZRANGE/ZREVRANGE/ZRANK/ZREVRANK/ZINCRBY/ZREM/ZCARD. Uses: cache, pub/sub, sessions, leaderboards. Limits: RAM cost, eviction, data loss, no complex queries.

**Neo4j:** node, label, property, directed relationship, path; Cypher; CREATE, MATCH, WHERE, RETURN, SET, REMOVE, DELETE, MERGE; `<>` , `=~` , XOR; `SET n.p = NULL` removes; mixed arrays not allowed; ER differences: directed, labels optional, real database.

**Vector DB:** embeddings via embedding models; similarity: cosine (angle; 1 best; distance = 1−sim, range 0–2), Euclidean (straight line); KNN exact vs ANN approximate; HNSW layered graph. pgvector: `CREATE EXTENSION vector` , `VECTOR(n)` , `hnsw (embedding vector_cosine_ops)` , `ORDER BY embedding <=> q LIMIT k` . Redis: FT.CREATE (VECTOR HNSW/FLAT, DIM, DISTANCE_METRIC), JSON.SET, FT.SEARCH KNN k, PARAMS, DIALECT 2. FTS = exact words/inverted index; vector = meaning.

**Next.js + SQL:** browser → `/api/hello` → Redis check → hit returns cache; miss queries PostgreSQL, caches 60 s. Packages `pg` , `redis` ; files page.tsx, route.ts, db.ts, redis.ts, .env.local; ports 3000/5432/6379; `redis-cli ping` returns PONG; restart after env changes.

# MOCK EXAMINATION (40 Marks)

**Units covered:** Unit 3 (Design Concepts and Implementation) and Unit 4 (Next-Gen Data Management). **Suggested time:** 90 minutes. **Pattern:** 16 × 1-mark MCQs (8 per unit) + 4 × 2-mark MCQs (2 per unit) + 4 × 4-mark descriptive questions (2 per unit) = 16 + 8 + 16 = **40 marks** . Attempt everything before looking at the answer key at the very end of this document.

## Section A: One-Mark MCQs (16 × 1 = 16)

### Unit 3 (Q1 to Q8)

**Q1.** Which module checks that relation and attribute names in a query exist in the schema? (a) Scanner (b) Parser (c) Validator (d) Code generator

**Q2.** A `NOT EXISTS` subquery is unnested by the optimizer into a (a) semi-join (b) anti-join (c) outer union (d) Cartesian product

**Q3.** Which join algorithm needs an index on the join attribute of the inner relation? (a) J1 nested-loop (b) J2 index-based nested-loop (c) J3 sort-merge (d) J4 partition-hash

**Q4.** In MySQL `EXPLAIN` , `type = ALL` with `key = NULL` indicates (a) index-only access (b) a full table scan (c) a primary-key lookup (d) a range scan

**Q5.** Which of the following is a trivial functional dependency? (a) AB → C (b) AB → B (c) A → AB (d) C → AB

**Q6.** A relation is in 3NF but not in BCNF only if there is an FD X → A such that (a) X is a superkey (b) X is not a superkey and A is a prime attribute (c) X is not a superkey and A is non-prime (d) A ⊆ X

**Q7.** Which normal form removes transitive dependencies of non-prime attributes on the key? (a) 1NF (b) 2NF (c) 3NF (d) 4NF

**Q8.** Which property of a decomposition must be achieved "at any cost"? (a) dependency preservation (b) lossless (non-additive) join (c) BCNF (d) minimal cover

### Unit 4 (Q9 to Q16)

**Q9.** After a transaction commits, its changes survive a system crash. This is (a) Atomicity (b) Consistency (c) Isolation (d) Durability

**Q10.** If the precedence graph of a schedule contains a cycle, the schedule is (a) conflict serializable (b) not conflict serializable (c) serial (d) deadlock-free

**Q11.** Which pair of lock modes is compatible? (a) X–X (b) S–X (c) S–S (d) X–S

**Q12.** Two-phase locking guarantees (a) conflict serializability (b) freedom from deadlock (c) freedom from starvation (d) that no transaction aborts

**Q13.** Which isolation level prevents dirty reads but still allows non-repeatable reads? (a) READ UNCOMMITTED (b) READ COMMITTED (c) REPEATABLE READ (d) SERIALIZABLE

**Q14.** In the CAP theorem, "P" stands for (a) Persistence (b) Partition tolerance (c) Performance (d) Parallelism

**Q15.** Correct syntax to add a member to a Redis sorted set: (a) ZADD key member score (b) ZADD key score member (c) ZADD score key member (d) ZADD member key score

**Q16.** The pgvector operator for **cosine distance** is (a) `<->` (b) `<=>` (c) `<#>` (d) `<@>`

## Section B: Two-Mark MCQs (4 × 2 = 8)

**Q17 (Unit 3).** R(A, B, C, D) with F = {A → B, B → C, C → D}. What is the candidate key and the highest normal form of R? (a) A; 3NF (b) A; 2NF (c) AB; 2NF (d) A; BCNF

**Q18 (Unit 3).** F = {A → B, B → C} and G = {A → B, A → C}. Which statement is true? (a) F and G are equivalent (b) F covers G but G does not cover F (c) G covers F but F does not cover G (d) neither covers the other

**Q19 (Unit 4).** Schedule S: W1(A), R2(A), W2(B), R3(B). Which statement is correct? (a) not conflict serializable (b) conflict serializable; equivalent serial order T1, T2, T3 (c) conflict serializable; equivalent serial order T3, T2, T1 (d) conflict serializable; equivalent serial order T2, T1, T3

**Q20 (Unit 4).** `RPUSH q a b c` then `LPUSH q z` then `RPOP q` then `LLEN q` . The output of `LLEN q` is (a) 2 (b) 3 (c) 4 (d) 5

## Section C: Descriptive Questions (4 × 4 = 16)

**Q21 (Unit 3, 4 marks).** For R(A, B, C, D) with F = {A → B, B → C, AC → D, A → C}: (i) find a minimal cover of F, showing the steps; (ii) find the candidate key of R.

**Q22 (Unit 3, 4 marks).** R(P, Q, R, S, T) has F = {PQ → R, R → S, S → T}. Find the candidate key, state the highest normal form of R with reasons, and decompose R into 3NF. State whether your decomposition is lossless and dependency preserving.

**Q23 (Unit 4, 4 marks).** Consider the schedule S: R1(A), W2(A), R2(B), W3(B), R3(C), W1(C). Define conflicting operations, build the precedence graph, and state whether S is conflict serializable, with justification.

**Q24 (Unit 4, 4 marks).** (a) Write Cypher statements to create two Person nodes (Alice, 25 and Bob, 27) with properties name and age, create a FRIEND_OF relationship from Alice to Bob, return the names of Alice's friends, and delete all Person nodes. (b) State two advantages of a graph database over a relational database for connected data.

# ANSWER KEY AND MODEL ANSWERS

**Stop. Only continue after you have attempted the mock examination.**

## Answer key: MCQs

|Q|Ans|Explanation|
|---|---|---|
|1|**c**|Scanner = tokens, Parser = syntax,**Validator =**<br>**schema/semantic check**|
|2|**b**|EXISTS/IN/ANY → semi-join;**NOT EXISTS/NOT**<br>**IN/ALL → anti-join**|
|3|**b**|J2 uses an index on the inner relation's join<br>attribute; J3 needs sorted inputs; J4 needs a hash<br>function|
|4|**b**|`ALL`is a full scan and<br>`NULL`key means no index<br>used|
|5|**b**|Trivial when RHS⊆LHS: B⊆AB. A → AB has AB⊄<br>A, so it is non-trivial|
|6|**b**|BCNF violation with prime RHS is exactly the 3NF<br>loophole; (c) would violate 3NF|
|7|**c**|3NF removes transitive dependencies; 2NF<br>removes partial dependencies|
|8|**b**|Lossless join is mandatory; dependency<br>preservation is desirable|
|9|**d**|Durability is handled by the recovery system|
|10|**b**|A cycle means no equivalent serial schedule by<br>conflicts|
|11|**c**|Only S–S is compatible|
|12|**a**|2PL ensures conflict serializability but not<br>deadlock freedom or starvation freedom|
|13|**b**|READ COMMITTED removes dirty reads only; non-<br>repeatable reads and phantoms remain|

|Q|Ans|Explanation|
|---|---|---|
|14|**b**|Consistency, Availability, Partition tolerance|
|15|**b**|Score comes before member:<br>`ZADD leaderboard`<br>`100 "player1"`|
|16|**b**|`<->`is Euclidean (L2),<br>`<=>`cosine distance,<br>`<#>`<br>negative inner product|
|17|**b**|A⁺ = {A,B,C,D} so A is the only key. Single-attribute<br>key means no partial dependency (2NF). B → C and<br>C → D have non-superkey determinants with non-<br>prime RHS, so not 3NF|
|18|**b**|Under F: A⁺ = {A,B,C} so A → C holds, F covers G.<br>Under G: B⁺ = {B}, so B → C is not implied, G does<br>not cover F|
|19|**b**|Conflicts: W1(A)–R2(A) gives T1→T2; W2(B)–R3(B)<br>gives T2→T3. No cycle; topological order T1, T2, T3|
|20|**b**|[a,b,c] → LPUSH z gives [z,a,b,c] → RPOP removes c<br>→ [z,a,b]; LLEN = 3|

**Scoring:** Q1–16: 1 mark each; Q17–20: 2 marks each.

## Model answers: Descriptive

#### **Q21 (4 marks)**

#### _(i) Minimal cover._

1. **Split RHS (1 mark):** all RHS are already single attributes: A → B, B → C, AC → D, A → C. 2. **Remove extraneous LHS attributes (1 mark):** in AC → D, A → C holds in F, so A⁺ contains C; therefore **C is extraneous** and AC → D is replaced by **A → D** . F′ = {A → B, B → C, A → D, A → C}.

3. **Remove redundant FDs (1 mark): A → C is redundant** , since A → B and B → C give it by transitivity (A⁺ without it = {A,B,C,D}).

4. **Minimal cover = {A → B, B → C, A → D}.**

_(ii) Candidate key (1 mark):_ A is never on a RHS, so it is essential; A⁺ = {A,B,C,D} = R, so **A is the (only) candidate key** .

#### **Q22 (4 marks)**

- **Key (1):** P and Q never appear on a RHS, so both are essential. (PQ)⁺: PQ → R, R → S, S → T gives {P,Q,R,S,T}. **Candidate key = PQ.** Prime: P, Q. Non-prime: R, S, T.

- **Highest normal form (1):** P⁺ = {P}, Q⁺ = {Q}, so there is **no partial dependency** : R is in **2NF** . But R → S has R not a superkey and S non-prime, and S → T has S not a superkey and T non-prime (transitive dependencies), so R is **not in 3NF** (and hence not BCNF).

- **3NF decomposition (1.5): R1(P,Q,R), R2(R,S), R3(S,T).** Each is in 3NF/BCNF (determinants PQ, R, S are keys of their own relations).

- **Properties (0.5): Lossless** : R2 ∩ R3 = S is the key of R3, and R1 ∩ (R2 ⋈ R3) = R is the key of R2. **Dependency preserving** : PQ → R in R1, R → S in R2, S → T in R3.

#### **Q23 (4 marks)**

- **Conflicting operations (1):** two operations of **different transactions** on the **same item** , at least one of which is a **write** (RW, WR, WW).

- **Conflicts and edges (2):**

   - Item A: R1(A) before W2(A) gives **T1 → T2** .

   - Item B: R2(B) before W3(B) gives **T2 → T3** .

   - Item C: R3(C) before W1(C) gives **T3 → T1** .

- **Conclusion (1):** the precedence graph has the **cycle T1 → T2 → T3 → T1** , so S is **not conflict serializable** (no equivalent serial schedule exists). A schedule is conflict serializable if and only if its precedence graph is acyclic.

#### **Q24 (4 marks)**

#### _(a) Cypher (3 marks)_

```
CREATE (a:Person {name:'Alice', age:25}), (b:Person {name:'Bob', age:27});

MATCH (a:Person {name:'Alice'}), (b:Person {name:'Bob'})
CREATE (a)-[:FRIEND_OF]->(b);

MATCH (:Person {name:'Alice'})-[:FRIEND_OF]->(f:Person)
RETURN f.name;

MATCH (n:Person) DETACH DELETE n;
```

Note: a plain `DELETE` fails while the nodes still have relationships; `DETACH DELETE` removes the relationships as well.

_(b) Advantages (1 mark):_ (1) **Efficient traversal of highly connected data** : relationships are stored as direct links, so deep connections need no costly JOINs. (2) **Flexible/optional schema and intuitive modelling** with Cypher pattern matching (also built-in graph algorithms such as shortest path and PageRank).

**Total: 16 + 8 + 16 = 40.**

# SYLLABUS COVERAGE CROSS-CHECK

|Syllabus topic|Where it is covered|
|---|---|
|**Unit 3:**Strategies of query processing|3.1 (pipeline, query blocks, semi/anti-join,<br>SELECT S1–S4, JOIN J1–J4, external sorting)|
|Query optimization|3.2 (causes of slow queries, techniques,<br>heuristic rules, EXPLAIN, indexing,<br>sargability)|
|Functional dependencies|3.3 (definition, instance vs meaning,<br>prime/non-prime,<br>full/partial/transitive/trivial), 3.6 (design<br>guidelines, anomalies)|
|Inference rules|3.4 (Armstrong's axioms, derived rules,<br>closure, derivations), 3.5 (equivalence,<br>minimal cover, keys)|
|Normal forms based on primary keys (1NF,<br>2NF, 3NF)|3.7|
|General definitions of 2NF and 3NF|3.8.2 and 3.8.3|
|Boyce-Codd Normal Form|3.8.4 to 3.8.8 (including TEACH, lossless test,<br>BCNF algorithm)|
|Higher normal forms|3.9 (MVD, 4NF, JD, 5NF)|
|**Unit 4:**Database transactions|4.1 (states, SQL support, ACID), 4.2<br>(schedules, serializability)|
|Concurrency control|4.3.1, 4.3.2, 4.4 (problems, isolation levels)|
|Locking|4.3.3 to 4.3.9 (S/X, 2PL variants, conversions,<br>deadlock, starvation, lock table)|

|Syllabus topic|Where it is covered|
|---|---|
|Introduction to NoSQL|4.5 (characteristics, categories, CAP)|
|Key-value database|4.6 (Redis: keys, data structures, commands,<br>case studies)|
|Graph databases (Neo4j)|4.7 (data model, Cypher, operators,<br>relationships)|
|Introduction to vector databases|4.8 (vectors, embeddings, similarity,<br>KNN/ANN/HNSW, FTS comparison)|
|Vector search with relational and NoSQL<br>databases|4.9 (pgvector), 4.10 (Redis vector search)|
|Web application development using<br>Next.js and SQL|4.11 (architecture, stack, setup, code,<br>verification)|

**Exam deliverables per unit:** detailed notes (3.1 to 3.11, 4.1 to 4.11); 4-mark questions with model answers (3.12 with 15, 4.13 with 18); definitions, facts and formulae (3.13, 4.14); confusion tables (3.11, 4.12); 2-mark MCQs (13 per unit, 3.14.1 and 4.15.1); 1-mark MCQs (28 and 32, 3.14.2 and 4.15.2); revision sheets (3.15, 4.16); mock exam with separate answer key (above).

#### **Notes on the source material:**

- A few slides were image-only (query-tree diagrams, some tables). For those I used standard textbook content and tagged it **[Supplement]** .

- Slide errors I corrected: the EXPLAIN "before index" reading ( `type = ALL` , `key = NULL` ); the BCNF decomposition R1(W,X,Y) (must be (X,Y)); the Cypher label-count query and path syntax; the "X→Y" cautionary note on splitting composite determinants.

- In the 2NF/LOTS table in 3.7.7, the first row mixes two ideas; the clean rule is: 2NF with respect to the primary key (Property_id#) has no partial dependency, but under the **general definition** County_name → Tax_rate violates 2NF, giving LOTS1 and LOTS2.
