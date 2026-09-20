# DBMS — Unit 1 & Unit 2 — Complete Study Notes
*(Definitions → Why it matters → Analogy → Diagram/Example → Syntax)*

> How to use this file: every topic follows the same rhythm —
> **What it is → Why we care → Everyday analogy → Example/Diagram → Syntax (if SQL)**.
> Read it top to bottom once for the story, then use it as a lookup table before the exam.

---
# UNIT 1 — Database Fundamentals

## 1. Data vs Information vs Database vs DBMS

| Term | Meaning | Analogy |
|---|---|---|
| **Data** | Raw, unorganised facts (puzzle pieces) | Scattered LEGO bricks on the floor |
| **Information** | Data that's processed/organised so it's *useful* | The LEGO bricks assembled into a house |
| **Database** | A logically coherent, purpose-built collection of related data representing a "mini-world" | A filing cabinet organised for one purpose (e.g. only student records) |
| **DBMS** | General-purpose software to **define, construct, manipulate, share** a database | The librarian + the filing system rules, all in one |

**Key rule:** Information depends on data; data does NOT depend on information.

### The 4 DBMS functions (DCMS mnemonic: Define, Construct, Manipulate, Share)
| Function | Meaning | Example |
|---|---|---|
| Define | Specify types/structure/constraints | `Students(ID, Name, Email, DOB, Major)` |
| Construct | Physically store data | Data sits on a server |
| Manipulate | Query/update/report | "Get all CS students" |
| Share | Multiple users/apps access concurrently, with permissions | Faculty sees their courses; admin sees all |

### Why study databases?
Store large data → file/disk structures · Understand data → data models · Keep it secure → security/recovery · Find/use data → query languages · Get accurate info → constraints · Maintain integrity → consistency rules.

### OLTP vs OLAP
| OLTP (transaction processing) | OLAP (analytics) |
|---|---|
| Many users, **small** frequent read/writes | Processing big data to find patterns |
| Banking, airlines, most apps | Loan approval scoring, targeted ads, data mining |

**Analogy:** OLTP = a cashier ringing up sale after sale. OLAP = the manager at month-end asking "which product sold best?"

### View of Data (abstraction)
```
Users → Abstract View (given by DB system)
      → Data Model (describes structure, relationships, constraints)
      → Physical Database
```
- **Data model**: conceptual tools to describe data/relationships/constraints.
- **Data abstraction**: hiding storage complexity behind levels — you don't need to know how a table is stored on disk to run a query.

---
## 2. Why Not Just Use Files? (Purpose of Database Systems)

**File-based system**: each application owns its own files — nothing is shared.

**Analogy:** Every department in a college keeps its own paper register of the same students → if a student changes address, you must find and correct EVERY register by hand.

### The 7 Drawbacks of file systems → How a DB fixes them

| # | Drawback | Example | DB Solution |
|---|---|---|---|
| 1 | **Redundancy & inconsistency** | Double-major student stored twice | Centralised store, one place to update |
| 2 | **Difficulty accessing data** | Need a new program for every new question | Powerful query language |
| 3 | **Data isolation** | Data spread across incompatible file formats | Unified access, consistent view |
| 4 | **Integrity problems** | "balance > 0" hard-coded into every program | Constraints declared once in schema |
| 5 | **Atomicity problems** | Money leaves A but fails before reaching B | Transactions: all-or-nothing + rollback |
| 6 | **Concurrent-access anomalies** | Two clerks read $10,000, both deduct → lost update | Locking/concurrency control |
| 7 | **Security problems** | No central access control | DBA-defined authentication/permissions |

**The classic lost-update anomaly (memorize this diagram):**
```
time →
Clerk1(-500): read 10,000 ---------------- write 9,500
Clerk2(-100):        read 10,000 ---------------- write 9,900
```
Whoever writes LAST wins → correct answer should be 9,400, but you get 9,500 or 9,900. This is *why* we need transactions/locking.

### Characteristics of the Database Approach
- **Self-describing**: DBMS catalog stores metadata (structure + constraints) along with data.
- **Program-data independence**: file structure lives in catalog, separate from access programs.
- **Multiple views**: different users see different subsets.
- **Multi-user transaction processing**: concurrent access controlled correctly.

### Advantages of DB systems (just recognize these, don't need to memorize all)
Controlling redundancy · Data sharing · Restricting unauthorized access · Persistent storage · Efficient query processing · Backup/recovery · Multiple interfaces · Complex relationships · Integrity enforcement · Triggers/active rules · Standards · Faster app development · Flexibility · Current info · Economies of scale.

### Quick "spot the problem" table (common exam pattern)
| Scenario | Problem Type |
|---|---|
| Two withdrawals same account at once, overdrawn | Concurrent access anomaly |
| Each dept keeps own spreadsheet, addresses disagree | Redundancy & inconsistency |
| Inventory/orders not synced, overselling | Data isolation |
| Invalid grade 'x' accepted, no check | Integrity problem |

---
## 3. Database Design: Abstraction, Independence, Users, Architecture

### Three-Schema Architecture
```
VIEW level      → what a specific user/app sees (many views)
   ↕ (logical data independence)
LOGICAL level   → whole DB structure (entities, relationships) — ONE logical schema
   ↕ (physical data independence)
PHYSICAL level  → how data is actually stored on disk
```
**Analogy:** View = the menu you see at a restaurant. Logical = the recipe book in the kitchen. Physical = the actual pantry shelf layout. You can rearrange the pantry (physical) without changing the recipe (logical); you can add a new recipe (logical) without reprinting every menu that doesn't use it (view)... unless it's relevant.

| Type of Independence | Definition | Example |
|---|---|---|
| **Physical data independence** | Change storage/physical schema without touching logical schema | Move from sequential file to indexed file — queries still work |
| **Logical data independence** | Change logical schema without breaking apps/external schema | Add a new column — old queries not using it still work |

### Schema vs Instance (this is a FAVORITE exam question)
| Schema | Instance / State |
|---|---|
| The overall **design** (like a variable's type declaration) | The actual **data at a moment in time** (like the variable's current value) |
| Changes rarely | Changes with every INSERT/UPDATE/DELETE |
```
Schema defined → Empty state → (load) Initial state → (update) Current state → (further updates) ...
```

### Database Users
| Type | Who they are |
|---|---|
| **Naïve/parametric** | No DB knowledge, use a fixed app UI (e.g. booking a movie ticket) |
| **Application programmers** | Write backend code/queries |
| **Sophisticated users** | Use SQL/DDL/DML directly (data engineers) |
| **DBA (Database Administrator)** | Central control: schema definition, storage/index setup, schema mods, granting access, backups |

### Typical DBMS component modules (just recognize the flow)
```
DDL statements → DDL Compiler ──┐
App programs → Pre-compiler ────┼──→ Stored Data Manager ↔ Catalog + Stored DB
Interactive queries → Query Compiler → Runtime DB Processor ┘
Privileged commands ────────────┘
```

### Database Architectures
| Type | Idea |
|---|---|
| **Centralized** | Everything on one server. Good for small/legacy apps. |
| **Client–Server (2-tier)** | Client (UI) ↔ ODBC/JDBC ↔ Server (DB). |
| **Client–Server (3-tier)** | Client ↔ App/Web server (business logic) ↔ DB server. Clients can't touch the DB directly → more secure. Common for web apps. |
| **Parallel** | Cluster of machines, shared memory/disk — big data warehouses. |
| **Distributed** | Data spread across multiple physical locations — scalability + fault tolerance via replication. |

**Analogy for 3-tier:** Client = customer at a restaurant, Application server = waiter who takes the order and decides how to phrase it to the kitchen, Database server = the kitchen. The customer never walks into the kitchen directly.

---
## 4. Entity-Relationship (E-R) Model

### Why E-R diagrams?
Graphical, simple, catch redundancy/errors before building the real DB, and serve as documentation.

**Design flow:** requirements → functional spec → choose data model → logical design → refine → physical design.

### Entities & Entity Sets
- **Entity** = a specific "thing" (physical: person, car; or conceptual: a job, a course).
- **Entity Set** = all entities of that type, sharing attributes (e.g. all "Student" rows).

### Attribute Types (Fig 4.1 tree — MEMORIZE this classification)
```
Attribute
 ├─ Simple  vs  Composite
 ├─ Single-valued  vs  Multivalued
 ├─ Stored  vs  Derived
 └─ Complex (nested composite + multivalued)  &  Key
```
| Type | Meaning | Example | E-R Symbol |
|---|---|---|---|
| Simple | Can't be split further | Roll number | Single oval |
| Composite | Splits into parts | `Address → street, city, state, pin` | Oval with sub-ovals |
| Single-valued | One value per entity | Age | Single oval |
| Multivalued | Can have many values | Phone numbers | **Double oval** |
| Stored | Physically saved | Date_of_Birth | Solid oval |
| Derived | Computed from other attrs | Age (from DOB) | **Dashed oval** |
| Key | Uniquely identifies the entity | SSN | **Underlined** |
| Complex | Nested composite+multivalued | `PreviousDegrees{College, Year, Degree, Field}` (rare) | — |

**Null value meanings:** Not applicable (doesn't exist) · Missing (exists but not recorded) · Not known (unsure if it even exists).

### Relationships
- **Relationship** = a connection between entities (drawn as a **diamond**).
- **Relationship Set** = collection of relationships of same type.
- **Degree** = number of entity sets involved: binary (2, most common), ternary (3).
- **Recursive relationship**: same entity set plays two roles (e.g. Employee–Supervisor, Course prereq). Needs **role names** on the connecting lines.
- **Descriptive/Relationship attribute**: belongs to the relationship itself, not either entity (e.g. `grade` belongs to `enroll` between Student & Course, because a grade only exists because of the enrollment).

**Analogy:** Think of "marriage" as a relationship diamond between two "Person" entities — the wedding date belongs to the *marriage*, not to either person alone.

### Cardinality Ratios (how many can pair with how many)
| Ratio | Meaning | Company Example |
|---|---|---|
| 1:1 | At most one on each side | MANAGES — an employee manages at most 1 dept, a dept has at most 1 manager |
| 1:N | One side unlimited, other side max 1 | WORKS_FOR — a dept has many employees, an employee has 1 dept |
| N:1 | Same as 1:N, reversed viewpoint | — |
| M:N | Unlimited both sides | WORKS_ON — employee↔project many-to-many |

### Participation Constraints
| Type | Meaning | Line style |
|---|---|---|
| **Total** | EVERY entity must participate (existence depends on it) | Double line |
| **Partial** | Some entities need not participate | Single line |

**Example:** Every student must have an advisor → student's participation in `advisor` is TOTAL. Not every instructor advises someone → instructor's participation is PARTIAL.

### Min-Max Notation
Written as `(min, max)` on the line to a relationship, e.g. `Student (2,5) Enrolls (0,N) Course` = every student enrolls in 2–5 courses; a course can have 0 to unlimited students.
- min = 1 → total participation
- max = 1 → participates in at most one relationship
- max = N → no limit

### Weak Entities
| Concept | Meaning |
|---|---|
| **Weak entity** | Can't be uniquely identified by its own attributes alone; depends on an "owner"/identifying (strong) entity |
| **Partial key (discriminator)** | Distinguishes weak entities that belong to the SAME owner (dashed underline) |
| **Identifying relationship** | Connects weak entity to its owner (drawn as **double diamond**) |
| Primary key of weak entity | = owner's PK + its own partial key |
| Participation | ALWAYS total in the identifying relationship |

**Notation:** Weak entity = **double rectangle**. Identifying relationship = **double diamond**. Total participation = **double line**.

**Classic example:** `DEPENDENT` is weak, depends on `EMPLOYEE`. Key = `{Essn, Dependent_name}` since a dependent's name is only unique *within* one employee's family.

### Full E-R Notation Summary
```
Entity            = single rectangle
Weak Entity       = double rectangle
Relationship      = diamond
Identifying Rel.  = double diamond
Attribute         = oval
Key Attribute     = underlined oval
Multivalued Attr  = double oval
Derived Attr      = dashed oval
Composite Attr    = oval with child ovals
Total Participation = double line
Cardinality Ratio (1,N,M) labels the line
(min,max) = structural constraint label
```

---
## 5. Converting an E-R Diagram → Relational Schema (the 7-step algorithm)

**Analogy:** Think of this as a recipe — E-R diagram is the "storyboard", the relational schema is the actual "table blueprint" you hand to the database.

| Step | Rule | Example (COMPANY DB) |
|---|---|---|
| **1. Strong entities** | One relation per strong entity, simple attributes become columns, key attr → PK | EMPLOYEE(Ssn PK...), DEPARTMENT(Dnumber PK...), PROJECT(Pnumber PK...) |
| **2. Weak entities** | New relation with owner's PK as FK; PK = owner PK + partial key | DEPENDENT(**Essn** FK, **Dependent_name**, ...) PK={Essn, Dependent_name} |
| **3. Binary 1:1** | 3 choices: (a) FK in the side with total participation (b) merge both into 1 relation if both total (c) 3rd cross-ref relation | MANAGES → put Mgr_ssn as FK in DEPARTMENT (dept has total participation) |
| **4. Binary 1:N** | Put PK of the "1" side as FK into the "N" side relation | WORKS_FOR → Dno (FK) goes into EMPLOYEE |
| **5. Binary M:N** | Create NEW relationship-relation with both PKs as FKs (combined = new PK) + any relationship attrs | WORKS_ON(**Essn** FK, **Pno** FK, Hours) |
| **6. Multivalued attribute** | New relation: {attribute, owner's PK as FK}; PK = both together | DEPT_LOCATIONS(**Dnumber** FK, **Dlocation**) |
| **7. n-ary (n>2) relationship** | New relation with all n PKs as FKs + relationship attrs | SUPPLY(**Sname**, **Proj_name**, **Part_no**, Quantity) |

### Resulting COMPANY schema (the model answer to memorize)
```
EMPLOYEE(Fname, Minit, Lname, Ssn, Bdate, Address, Sex, Salary, Super_ssn, Dno)
DEPARTMENT(Dname, Dnumber, Mgr_ssn, Mgr_start_date)
DEPT_LOCATIONS(Dnumber, Dlocation)
PROJECT(Pname, Pnumber, Plocation, Dnum)
WORKS_ON(Essn, Pno, Hours)
DEPENDENT(Essn, Dependent_name, Sex, Bdate, Relationship)
```
Underlined = primary key. Arrows/FKs point from the "many" side to the "one" side's PK.

### Mapping Cheat-Sheet
| ER construct | Relational construct |
|---|---|
| Entity type | A relation |
| 1:1 or 1:N relationship | Foreign key |
| M:N relationship | New relation + 2 FKs |
| n-ary relationship | New relation + n FKs |
| Simple attribute | Column |
| Composite attribute | Broken into simple component columns |
| Multivalued attribute | New relation + FK |
| Key attribute | Primary/secondary key |

---
## 6. The Relational Model — Formal Terms & Constraints

| Formal term | Informal term |
|---|---|
| Relation | Table |
| Tuple | Row |
| Attribute | Column |

- **Domain**: set of permitted values (e.g. phone numbers formatted `(ddd)ddd-dddd`).
- **Relation schema**: `R(A1, A2, ..., An)` — the "intension" (design).
- **Relation state**: actual set of tuples at a moment — the "extension".

### Key concepts (ladder from broad to narrow)
```
Super Key  ⊇  Key (minimal super key)  ⊇  one is chosen as → Primary Key
                    ↑
             Every key is a "Candidate Key"
```
| Term | Definition |
|---|---|
| **Super key** | Set of attrs where no two tuples agree on all of them |
| **Key (minimal super key)** | Remove any attribute → it stops being a super key |
| **Candidate key** | Each valid key a table could use (a table may have several) |
| **Primary key** | ONE candidate key chosen to identify rows, underlined in schema |
| **Surrogate/artificial key** | An assigned ID (row-id) used purely for identification |

### The 5 Schema-Based (Explicit) Constraints
1. **Domain constraint** — every value must come from its attribute's domain (or NULL if allowed).
2. **Key constraint** — no two tuples share the same key value.
3. **NULL constraint** — whether NULL is allowed for an attribute (e.g. `Name NOT NULL`).
4. **Entity integrity** — primary key attributes can NEVER be NULL.
5. **Referential integrity (foreign key)** — FK value in referencing table must either match a PK value in the referenced table, or be NULL.

**Analogy for referential integrity:** A foreign key is like a "return address" on a form — it must point to a REAL address that exists (or be left blank), never to a made-up address.

### Constraint categories (theory level)
| Type | Meaning |
|---|---|
| Inherent/implicit | Built into the model itself (e.g. relational model disallows list-valued attributes) |
| Schema-based/explicit | Declared in schema (the 5 above) |
| Application-based/semantic | Enforced only by application code (beyond DB's expressive power) |

---
## 7. Relational Algebra — Operations

**Analogy:** Relational Algebra is a toolbox of LEGO-like operations. Every operation takes table(s), outputs a NEW table — so you can chain them like pipes.

### Unary Operators (1 table in)

**SELECT (σ)** — filters ROWS (horizontal cut). "WHICH rows do I want?"
```
σ condition (Relation)
σ Dept_Name="Physics" (Instructor)
σ (Dno=4 ∧ Salary>25000) ∨ (Dno=5 ∧ Salary>30000) (EMPLOYEE)
```
Properties: same schema as input · commutative (order of chained selects doesn't matter) · result size ≤ input size.

**PROJECT (Π)** — picks COLUMNS (vertical cut) and **removes duplicate rows** (since relations are sets).
```
Π ID, name, salary (instructor)
```
If the projected columns include a key → no duplicates possible, size stays same. Otherwise size can shrink.

**RENAME (ρ)** — renames a relation and/or its attributes.
```
ρ S(B1,...,Bn) (R)   -- rename table AND columns
ρ S (R)              -- rename table only
ρ (B1,...,Bn) (R)    -- rename columns only
ρ Father→Parent (Paternity)   -- rename ONE column
```

**Composition:** Since results are relations, you can nest: `Π name (σ dept_name="Physics" (instructor))` = "Names of Physics instructors."

### Binary Operators (2 tables in)

**Set operators — need Union Compatibility** (same # of columns, compatible domains):
| Operator | Meaning |
|---|---|
| ∪ Union | All tuples in either, duplicates removed |
| ∩ Intersection | Only tuples in BOTH |
| − Difference | In first but NOT second (order matters! R−S ≠ S−R) |

**Cartesian Product (×)** — combines EVERY row of R with EVERY row of S. Result has `n_R × n_S` rows, `attrs_R + attrs_S` columns. Rarely useful alone — needs a SELECT afterward to filter meaningful pairs, which is essentially what JOIN automates.

**Join (▷◁)** = SELECT + Cartesian Product combined:
```
r ▷◁ θ s  ≡  σ θ (r × s)
```
**Analogy:** Cartesian product is "introduce every guy to every girl at a party" (messy, most pairs make no sense); JOIN is "introduce only couples who are actually a match."

**Division (÷)** — "for ALL" queries. `R(Z) ÷ S(X)` returns values of `Y = Z−X` that appear in R paired with **every single** tuple in S.
> Example: `SSN_PNOS ÷ SMITH_PNOS` = SSNs of employees who worked on **every** project that Smith worked on.

### Aggregate Functions (need a special F/g notation in algebra)
| Function | Meaning |
|---|---|
| MIN() / MAX() | smallest / largest |
| SUM() | total (numeric only) |
| AVG() | average of non-null values |
| COUNT() | number of rows (keeps duplicates, works on any type) |

```
F MAX Salary (EMPLOYEE)                    -- single aggregate over whole relation
Dno F COUNT Ssn, AVERAGE Salary (EMPLOYEE) -- grouped: count & avg salary PER department
```
Grouping attribute(s) go LEFT of F; aggregate functions go RIGHT.

### Relational Algebra "recipe patterns" (from the hands-on chapter — VERY exam relevant)
| Query pattern | Algebra pattern |
|---|---|
| Names of X who did Y (join then project) | `Π name (σ cond (A) ▷◁ B)` |
| "Have NO..." (nobody / never) | `ALL − THOSE_WITH_IT` (set difference) |
| "Both A and B" | Intersect the two `sid`/key sets separately, THEN join back to get names — **not** intersecting on the wrong table! |
| "At least two of something" (self-comparison) | Self Cartesian product of a relation with itself (renamed), then filter `id1=id2 ∧ attr1≠attr2` |
| "ALL of a category" (universal) | DIVISION |
| "Cheapest/most expensive with ties" | Self-join eliminating anyone beaten by someone cheaper, OR aggregate MIN + join back |

**⚠️ Common trap:** "Red AND green boat" is NOT `(σcolor=red Boats) ∩ (σcolor=green Boats)` — no boat is both colors, so that's always empty! You must find sailors who reserved a red boat, INTERSECT with sailors who reserved a green boat (intersect the **sid** sets, not the boats).

---
## 8. SQL — Data Definition (DDL) — Unit 1

### SQL language categories
| Category | Purpose | Example commands |
|---|---|---|
| **DDL** | Define structure | CREATE, ALTER, DROP, TRUNCATE, RENAME |
| **DML** | Manipulate data | SELECT, INSERT, UPDATE, DELETE |
| **DCL** | Control access | GRANT, REVOKE |
| **TCL** | Run/undo changes | COMMIT, ROLLBACK |

### Data Types cheat sheet
| Category | Types |
|---|---|
| Integer | INT, INTEGER, SMALLINT |
| Floating point | FLOAT, REAL, DOUBLE PRECISION |
| Fixed string | CHAR(n) |
| Variable string | VARCHAR(n) |
| Bit string | BIT(n), BIT VARYING(n) |
| Boolean | TRUE/FALSE |
| DATE | YYYY-MM-DD |
| TIMESTAMP | date+time+fraction of seconds |
| INTERVAL | relative time increment |
| numeric(p,d) | p total digits, d after decimal |
| ENUM (MySQL) | restrict to a fixed list of values |

**CHAR vs VARCHAR:**
| CHAR | VARCHAR |
|---|---|
| Fixed length, padded with blanks | Variable length, no padding |
| Use when size is always same (pincode) | Use when size varies (name) |
| Slightly faster | Slightly less storage |

**BLOB vs CLOB (Large Objects):**
| BLOB | CLOB |
|---|---|
| Binary data (images, video, audio) | Character/text data (long articles) |
| Not human readable | Human readable, uses text functions |
| MySQL: TINYBLOB→LONGBLOB | MySQL: TEXT family (TINYTEXT→LONGTEXT) |

### DDL Syntax
```sql
-- Create
CREATE TABLE table_name (
  col1 datatype,
  col2 datatype
);
CREATE TABLE new_t AS SELECT * FROM existing_t;  -- copies structure + data
CREATE TABLE new_t LIKE existing_t;              -- copies structure only

-- Alter
ALTER TABLE t ADD col_name datatype;
ALTER TABLE t MODIFY col_name new_datatype;
ALTER TABLE t RENAME COLUMN old_name TO new_name;
ALTER TABLE t DROP COLUMN col_name;

-- Drop / Truncate / Rename
DROP TABLE table_name;         -- removes data AND structure
TRUNCATE TABLE table_name;     -- removes data, KEEPS structure
RENAME TABLE old_name TO new_name;
```

**DROP vs TRUNCATE (classic exam Q):**
| Feature | DROP | TRUNCATE |
|---|---|---|
| Removes data | Yes | Yes |
| Removes structure | Yes (table gone) | No |
| Rollback | Cannot | Cannot |
| AUTO_INCREMENT | N/A (gone) | Resets to 1 |
| Use case | Table no longer needed | Empty table, keep structure |

### Constraints in SQL
```sql
CREATE TABLE EMPLOYEE (
  Ssn CHAR(9) NOT NULL,
  Dno INT NOT NULL DEFAULT 1,
  CONSTRAINT EMPPK PRIMARY KEY (Ssn),
  CONSTRAINT EMPSUPERFK FOREIGN KEY (Super_ssn) REFERENCES EMPLOYEE(Ssn)
      ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT EMPDEPTFK FOREIGN KEY (Dno) REFERENCES DEPARTMENT(Dnumber)
      ON DELETE SET DEFAULT ON UPDATE CASCADE
);
```
| Constraint | Purpose |
|---|---|
| NOT NULL | disallow NULL |
| UNIQUE | no duplicate values |
| PRIMARY KEY | NOT NULL + UNIQUE, identifies row |
| FOREIGN KEY | referential integrity link to another table |
| CHECK | custom rule, e.g. `CHECK (Dnumber > 0)` |
| DEFAULT | value used when none supplied |

### Referential Triggered Actions (what happens to child rows on parent DELETE/UPDATE)
| Action | ON DELETE | ON UPDATE |
|---|---|---|
| **NO ACTION** (default) | Reject the operation | Reject the operation |
| **CASCADE** | Delete matching child rows too | Update matching FK values too |
| **SET NULL** | Set child FK to NULL | Set child FK to NULL |
| **SET DEFAULT** | Set child FK to its default | Set child FK to its default |

**Analogy:** CASCADE = "if the parent leaves, the kids go too." SET NULL = "if the parent leaves, kids just don't have a parent listed anymore." NO ACTION = "you can't kick the parent out while kids still depend on them."

### Database Modification (DML basics)
```sql
INSERT INTO EMPLOYEE VALUES ('Richard','K','Marini','653298653', '30-DEC-52', ...);
INSERT INTO EMPLOYEE (Fname, Lname, Dno, Ssn) VALUES ('Richard','Marini',4,'653298653');

UPDATE EMPLOYEE SET Salary = Salary * 1.1 WHERE Dno = 5;

DELETE FROM EMPLOYEE WHERE Lname = 'Zelaya';
DELETE FROM EMPLOYEE;      -- empties table, table still exists
DROP TABLE EMPLOYEE;       -- removes rows AND definition
```

---
# UNIT 2 — Advanced SQL

## 1. Additional Basic Operations
```sql
SELECT Fname, Lname FROM EMPLOYEE;                       -- projection
SELECT DISTINCT Fname, Lname FROM EMPLOYEE;               -- remove duplicates
SELECT fname, salary/2 AS monthly_salary FROM EMPLOYEE;   -- arithmetic + AS alias
SELECT fname, salary FROM employee WHERE dno=5 AND salary<70000;  -- filter
SELECT fname, lname, salary FROM EMPLOYEE ORDER BY salary DESC;   -- sort

-- LIKE: % = any substring, _ = any single char
SELECT Fname, Lname FROM EMPLOYEE WHERE Address LIKE '%Houston,TX%';
SELECT Fname FROM EMPLOYEE WHERE Bdate LIKE '196_______';  -- born in 1960s
```
**Analogy for `%` and `_`:** `%` is a wildcard for "any length of stuff", `_` is a wildcard for "exactly one character" — like a single blank tile in Scrabble.

## 2. Set Operations & NULL Handling
| Operator | Behaviour |
|---|---|
| UNION | combine, remove duplicates |
| UNION ALL | combine, keep duplicates |
| INTERSECT | common rows only |
| EXCEPT | rows in Q1 but not Q2 |
(ALL variants keep duplicate counts using min/sum/max rules)

```sql
SELECT grade FROM student_course WHERE course='Physics'
UNION
SELECT grade FROM student_course WHERE course='Mathematics';
```

### NULL — the tricky part
- **3-valued logic**: TRUE / FALSE / **UNKNOWN**. Any comparison with NULL → UNKNOWN.
- A row is kept in WHERE only if the condition is TRUE (not FALSE, not UNKNOWN).
- Arithmetic with NULL → NULL.
- `NULL = NULL` → UNKNOWN (NOT true!) — but for DISTINCT/UNION/INTERSECT/EXCEPT, two NULLs ARE treated as identical (special exception!).
```sql
SELECT Fname FROM EMPLOYEE WHERE Super_ssn IS NULL;      -- correct way to test NULL
SELECT Essn FROM WORKS_ON WHERE Hours > 10 IS UNKNOWN;    -- testing 3-valued result
```
**Analogy:** NULL is like an empty question mark box — you can't say two "?"s are equal in a normal comparison, because you genuinely don't know if they'd match. But when just *grouping* things (DISTINCT), all the "?" boxes get lumped together for convenience.

## 3. Aggregate Functions, GROUP BY, HAVING
```sql
SELECT SUM(Salary), MAX(Salary), MIN(Salary), AVG(Salary) FROM EMPLOYEE;
SELECT Dno, COUNT(*), AVG(Salary) FROM EMPLOYEE GROUP BY Dno;

SELECT Pnumber, Pname, COUNT(*) FROM PROJECT, WORKS_ON
WHERE Pnumber = Pno
GROUP BY Pnumber, Pname
HAVING COUNT(*) > 2;
```
**Execution order (memorize!):** `FROM → WHERE → GROUP BY → HAVING → SELECT → ORDER BY`

| WHERE | HAVING |
|---|---|
| Filters individual rows BEFORE grouping | Filters whole GROUPS after aggregation |
| Cannot use aggregate functions | Can use aggregate functions |

**Analogy:** WHERE is a bouncer checking IDs before people enter the club (per-person). HAVING is checking if a *group's table* has enough people sitting at it after they've already been seated.

NULL handling in aggregates: NULLs are discarded before computing. If ALL values are NULL → COUNT returns 0, others return NULL.

## 4. Join Expressions
```sql
-- Natural Join: auto-matches same-named columns
SELECT employee_name, Department_name FROM employee NATURAL JOIN Department;

-- Inner Join (explicit condition)
SELECT e.Employee_ID, d.Department_Name
FROM employee e INNER JOIN department d ON e.Employee_ID = d.Employee_ID;

-- Outer Joins
SELECT ... FROM A LEFT OUTER JOIN B ON A.id=B.id;   -- all of A, NULL-pad unmatched B
SELECT ... FROM A RIGHT OUTER JOIN B ON A.id=B.id;  -- all of B, NULL-pad unmatched A
-- FULL OUTER JOIN not in MySQL — emulate:
SELECT ... FROM A LEFT OUTER JOIN B ON ...
UNION
SELECT ... FROM A RIGHT OUTER JOIN B ON ...;

-- Cross Join (pure Cartesian product)
SELECT * FROM loan CROSS JOIN borrower;
```
**Analogy:**
- INNER JOIN = "only show couples where BOTH sides matched" (like a Venn diagram intersection).
- LEFT JOIN = "show every person from the left list, plus their partner IF they have one, else blank."
- CROSS JOIN = "everyone meets everyone" (no filtering at all).

```
    A         B
   ┌──┐     ┌──┐
   │██│▓▓▓▓▓│██│     Inner join = only the middle ▓▓▓ overlap
   └──┘     └──┘
```

## 5. Nested Subqueries
```sql
-- IN / NOT IN
SELECT DISTINCT Essn FROM WORKS_ON
WHERE (Pno, Hours) IN (SELECT Pno, Hours FROM WORKS_ON WHERE Essn='123456789');

-- ANY/ALL/SOME
SELECT Lname FROM EMPLOYEE
WHERE Salary > ALL (SELECT Salary FROM EMPLOYEE WHERE Dno=5);
-- '=SOME' behaves like IN;  '<>ALL' behaves like NOT IN

-- Correlated subquery (inner query refers to outer row)
SELECT Lname, Fname FROM EMPLOYEE
WHERE (SELECT COUNT(*) FROM DEPENDENT WHERE Ssn=Essn) >= 2;

-- EXISTS / NOT EXISTS
SELECT Fname FROM EMPLOYEE
WHERE NOT EXISTS (SELECT * FROM DEPENDENT WHERE Ssn=Essn);  -- employees with NO dependents

-- Subquery in FROM (derived table) — must be aliased
SELECT Dno, avg_salary FROM
  (SELECT Dno, ROUND(AVG(Salary),2) AS avg_salary FROM EMPLOYEE GROUP BY Dno) AS dept_avg
WHERE avg_salary > 32000;
```
**Correlated vs Non-correlated:**
| Non-correlated | Correlated |
|---|---|
| Inner query runs ONCE, independent of outer row | Inner query re-runs for EACH outer row (references outer table) |

**Naming rule:** an unqualified column refers to the *innermost* query that declares it — use table aliases to avoid ambiguity, especially in correlated queries.

## 6. Database Modification (with subqueries)
```sql
INSERT INTO EMPLOYEE (Fname, Lname, Ssn, Dno) VALUES ('Ravi','Kumar','999887776',5);
INSERT INTO Marks_history SELECT * FROM Marks_sample WHERE SRN=2;  -- insert via query

UPDATE EMPLOYEE SET Salary = Salary * 1.1 WHERE Dno = 5;

DELETE FROM Marks_sample WHERE SRN = 2;
```
⚠️ Forgetting WHERE in UPDATE/DELETE affects **every row** — always double-check!

## 7. Views, Users & Privileges

**View** = a virtual table defined by a stored SQL query; it doesn't store data itself, just re-runs the query when accessed.
```sql
CREATE VIEW view1 AS SELECT Fname, Lname FROM EMPLOYEE WHERE Dno=5;
```
**Updatable view rules — ALL must hold:**
- Based on a SINGLE table (no joins/subqueries)
- No aggregate functions
- No DISTINCT / GROUP BY / HAVING / UNION
- Not recursive

**Views vs CTE:**
| Aspect | View | CTE |
|---|---|---|
| Persistence | Stored until dropped | Exists only during that query |
| Reusability | Across many queries | Only within its own query |
| Scope | Global (permission-based) | Local |
| Updatable | Yes, if simple | No, read-only |

**Analogy:** A VIEW is a saved recipe card pinned on the fridge — anyone can use it anytime. A CTE is a sticky-note you scribble mid-cooking and throw away once the dish is done.

### Users, Roles, Privileges (MySQL)
```sql
CREATE USER 'student'@'localhost' IDENTIFIED BY 'stud123';
DROP USER 'student'@'localhost';

GRANT SELECT ON school.students TO 'alice'@'localhost';
GRANT ALL PRIVILEGES ON school.* TO 'bob'@'localhost';
REVOKE SELECT ON school.* FROM 'alice'@'localhost';

CREATE ROLE Assistant_employee;
GRANT SELECT ON courses TO Assistant_employee;
GRANT Assistant_employee TO 'alice'@'localhost';
```
A **role** = a named bundle of privileges you can grant/revoke from many users at once (like an "access group").

## 8. Triggers

**Trigger** = code that runs automatically as a SIDE-EFFECT of INSERT/UPDATE/DELETE. Defined by Event + Condition + Action.
```sql
CREATE TRIGGER trigger_name (AFTER|BEFORE) (INSERT|UPDATE|DELETE) ON table_name
FOR EACH ROW
BEGIN
  -- code; NEW.col = new row values, OLD.col = previous row values
END;
```
```sql
DELIMITER //
CREATE TRIGGER CheckMarks
BEFORE INSERT ON Marks_sample
FOR EACH ROW
BEGIN
  IF NEW.marks < 0 OR NEW.marks > 100 THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Invalid marks';
  END IF;
END;//
DELIMITER ;
```
**Analogy:** A trigger is like a smoke alarm — it doesn't wait to be asked, it fires automatically the moment a specific event (smoke = INSERT/UPDATE/DELETE) happens, checking a condition and taking action.

## 9. User-Defined Functions
```sql
CREATE FUNCTION Dept_size(deptno INT) RETURNS VARCHAR(7)
BEGIN
  DECLARE No_of_emps INT;
  SELECT COUNT(*) INTO No_of_emps FROM EMPLOYEE WHERE Dno = deptno;
  IF No_of_emps > 3 THEN RETURN 'HUGE';
  ELSEIF No_of_emps > 2 THEN RETURN 'LARGE';
  ELSE RETURN 'SMALL';
  END IF;
END;

SELECT Dname, Dept_size(Dnumber) FROM department;  -- can be called INSIDE a SELECT
```
Function = takes parameters, returns exactly **ONE value**, can be embedded in SELECT.

## 10. Stored Procedures
```sql
DELIMITER &&
CREATE PROCEDURE display_max_mark(OUT highestmark INT)
BEGIN
  SELECT MAX(marks) INTO highestmark FROM student;
END&&
DELIMITER ;

CALL display_max_mark(@output);
SELECT @output;
```
| Mode | Meaning |
|---|---|
| IN (default) | value passed in, unchanged for caller |
| OUT | procedure sets a value returned to caller |
| INOUT | value passed in AND can be modified |

**Function vs Procedure:**
| Function | Procedure |
|---|---|
| Returns exactly 1 value | May return 0, 1, or many result sets |
| Used INSIDE a SELECT | Invoked with `CALL`, not inside SELECT |

## 11. Common Table Expressions (CTE)
```sql
WITH cte_name AS (SELECT ...)
SELECT ... FROM cte_name;

-- Recursive CTE (hierarchies, e.g. org chart)
WITH RECURSIVE orghierarchy AS (
  SELECT ssn, fname, super_ssn, 1 AS level FROM employee WHERE super_ssn IS NULL  -- anchor
  UNION ALL
  SELECT e.ssn, e.fname, e.super_ssn, oh.level+1
  FROM employee e JOIN orghierarchy oh ON e.super_ssn = oh.ssn                     -- recursive
)
SELECT * FROM orghierarchy;
```
**Rule:** Anchor member must NOT reference the CTE itself; the recursive member MUST have a stopping condition or it loops forever.

**Analogy:** Recursive CTE is like "climbing down a family tree" — start with the CEO (anchor = no boss), then keep asking "who reports to the last person I found?" (recursive step) until nobody's left.

### CASE expression
```sql
SELECT Fname, Salary,
  CASE
    WHEN Salary >= 55000 THEN 'High'
    WHEN Salary >= 40000 THEN 'Medium'
    ELSE 'Low'
  END AS Salary_Level
FROM employee;
```
No matching WHEN + no ELSE → returns NULL.

## 12. Window Functions

**Key idea:** unlike GROUP BY (which COLLAPSES rows into one per group), window functions keep EVERY row but let you compute group-level stats alongside them.
```sql
SELECT column_list, window_function(col)
  OVER ([PARTITION BY partition_col] [ORDER BY order_col])
  AS new_column
FROM table_name;
```
**Analogy:** GROUP BY is like handing out ONE report card per class (average only). A window function is like giving EVERY student their own row that ALSO shows the class average next to their personal score.

| Category | Functions |
|---|---|
| Aggregate | SUM(), AVG(), COUNT(), MAX(), MIN() — used with OVER() |
| Ranking | ROW_NUMBER(), RANK(), DENSE_RANK() |
| Value | LAG(), LEAD(), FIRST_VALUE(), LAST_VALUE() |

**Ranking differences (ties matter!):**
| Function | Behaviour on ties |
|---|---|
| RANK() | same rank, next rank SKIPPED (1,1,3,...) |
| DENSE_RANK() | same rank, next rank consecutive (1,1,2,...) |
| ROW_NUMBER() | always unique, no ties possible |

```sql
SELECT emp_name, department, salary,
  RANK() OVER (PARTITION BY department ORDER BY salary DESC) AS rank_in_dept
FROM Employees;

-- LAG/LEAD: peek at previous/next row within the partition
SELECT emp_name, salary,
  LAG(salary,1) OVER (PARTITION BY department ORDER BY salary DESC) AS prev_salary
FROM Employees;

-- Practical: Top-2 sales per employee
SELECT * FROM (
  SELECT *, RANK() OVER (PARTITION BY EmpID ORDER BY Amount DESC) AS SaleRank
  FROM Sales
) t WHERE SaleRank <= 2;

-- Running total
SELECT EmpID, Month, Amount,
  SUM(Amount) OVER (PARTITION BY EmpID ORDER BY SaleDate) AS RunningTotal
FROM Sales;
```

## 13. Full-Text Search (MySQL)

For searching large TEXT columns intelligently (relevance-ranked), instead of slow `LIKE '%word%'`.
```sql
CREATE TABLE posts (id INT AUTO_INCREMENT PRIMARY KEY, title VARCHAR(200), body TEXT) ENGINE=InnoDB;
ALTER TABLE posts ADD FULLTEXT(title, body);

-- Natural language mode (default) — ranked by relevance
SELECT * FROM posts WHERE MATCH(title, body) AGAINST('database' IN NATURAL LANGUAGE MODE);

-- Show relevance score, sort by it
SELECT id, title, MATCH(title, body) AGAINST('database') AS relevance
FROM posts WHERE MATCH(title, body) AGAINST('database')
ORDER BY relevance DESC;

-- Boolean mode — explicit operators
SELECT * FROM posts WHERE MATCH(title, body) AGAINST('+mysql -oracle' IN BOOLEAN MODE);
```
| Operator | Meaning | Example |
|---|---|---|
| `+` | must be present | `+mysql` |
| `-` | must NOT be present | `-database` |
| `"..."` | exact phrase | `"mysql tutorial"` |
| `*` | suffix wildcard | `data*` → data, database |
| `()` | group terms | `+(mysql database)` |

**Analogy:** LIKE '%word%' is like reading every page of a book cover to cover looking for a word. FULLTEXT search is like using the book's professionally-built index at the back — much faster, and it also tells you HOW relevant each hit is.

---
# Quick-Fire Q&A Bank (high-yield, from question bank)

**Q: Why do we need a database system (not just files)?**
Data independence, reduced redundancy, better sharing, integrity/security enforcement, backup & recovery.

**Q: Three levels of data abstraction?**
Physical (how stored) → Logical (what's stored + relationships) → View (user-specific slice).

**Q: Schema vs State?**
Schema = structure/design (rarely changes). State/Instance = actual data now (changes every update).

**Q: Types of DB users?**
Naïve, Application programmers, Sophisticated users, DBA.

**Q: 2-tier vs 3-tier architecture?**
2-tier: client talks directly to DB server (simple, less scalable, security mixed in).
3-tier: client → app/business-logic server → DB server (scalable, secure, DB hidden from client).

**Q: Types of entities?**
Strong (own identifier) vs Weak (depends on a strong entity + partial key).

**Q: Explain recursive relationship.**
Same entity type participates twice in different ROLES, e.g. Employee `manages` Employee.

**Q: Basic relational algebra operations?**
SELECT (σ, filter rows), PROJECT (Π, filter columns), UNION, DIFFERENCE, CARTESIAN PRODUCT, RENAME (ρ).

**Q: Use of DIVISION?**
Finds tuples related to **ALL** tuples of another relation — the algebra way to express "for every".

**Q: Types of database languages?**
DDL (define), DML (manipulate), DCL (control access), TCL (transaction control — commit/rollback).

**Q: List SQL constraints.**
NOT NULL, UNIQUE, PRIMARY KEY, FOREIGN KEY, CHECK, DEFAULT.

**Q: CLOB vs BLOB?**
CLOB = large text; BLOB = large binary (images/video), not searchable by text functions.

**Q: DROP vs TRUNCATE?**
DROP kills structure+data (no rollback). TRUNCATE clears data, keeps structure (also no rollback in MySQL).

**Q: Aggregate functions?**
COUNT, SUM, AVG, MAX, MIN — usually paired with GROUP BY.

**Q: Referential actions?**
CASCADE (propagate change), SET NULL, SET DEFAULT, NO ACTION (reject).

**Q: Structure of a SQL query (execution order)?**
```
SELECT <cols>
FROM <table>
WHERE <row filter>
GROUP BY <cols>
HAVING <group filter>
ORDER BY <cols>;
```
Logical execution order: FROM → WHERE → GROUP BY → HAVING → SELECT → ORDER BY.

---
# Exam Strategy Tips
1. **Definitions first** — most Unit-1 theory questions want the textbook definition + one real example. State the definition crisply, then give the COMPANY/university example from the notes.
2. **E-R → Relational mapping** — practice drawing the diagram AND writing out the 7-step conversion; this is asked almost every semester in some form.
3. **Relational Algebra vs SQL** — know both notations for the same query; many papers ask "write in relational algebra" for one query and "write in SQL" for a similar one.
4. **NULL logic and set operations** — a favorite trick question area (3-valued logic, NULL treated as equal only in DISTINCT/UNION/etc., not in WHERE).
5. **WHERE vs HAVING**, **DROP vs TRUNCATE**, **CHAR vs VARCHAR**, **Views vs CTE**, **Function vs Procedure** — these "X vs Y" pairs are recurring 2-mark/5-mark questions — the tables above are exam-ready answers.
6. **SQL Question Bank patterns** — practice writing: INSERT/UPDATE/DELETE with subqueries, correlated subqueries with EXISTS, GROUP BY + HAVING, window functions for ranking/running totals, and CTEs for multi-step aggregation. These map directly to the "Easy/Medium/Hard" question bank sections.