# Software Engineering — Complete Study Notes (Unit 1 & Unit 2)


---

# UNIT 1: Introduction to SE & Requirements Engineering

## 1. What is Software Engineering?

**Software** = programs + documentation + configuration data needed to make it work correctly.

**Software Engineering** = systematic, disciplined, quantifiable approach to developing, operating, and maintaining software.

**Analogy:** Think of building a house. A "program" alone is like stacking bricks without a blueprint. Software Engineering is the discipline of architects + engineers + project managers working together — blueprints (requirements/design), inspection (testing), and maintenance plans (support) all included.

**Why it matters — real numbers to remember (case-study bait):**
- Boeing 787: 6.5 million lines of code (more than all its mechanical parts combined)
- Facebook: 62 million lines of code
- A 90-minute Amazon outage = $2.8 million loss

**Software Crisis (1968 NATO Conference)** — term coined to describe the gap between rising computing power/complexity and our ability to build correct, reliable software on time and budget.
- Symptoms: over-budget, over-time, low quality, unmet requirements, unmanageable code, undelivered software
- **Analogy:** Like ordering a custom-built car and getting different specs than promised, 2 years late, over budget, and it breaks down in a week.

**Stat to remember:** Standish Group — only 1 in 3 software projects are truly successful; 66% end in partial/total failure.

### Computer Science vs Software Engineering
| CS | SE |
|---|---|
| Theory: algorithms, computation | Practice: building, designing, testing |
| "Can it be computed?" | "Can we build it reliably, on time, on budget?" |

---

## 2. Case Study: CrowdStrike Outage (2024) — HIGH-YIELD for case-study MCQs

**What happened:** July 18, 2024 — CrowdStrike pushed a Falcon software update with a corrupted library to production **without adequate testing**. It auto-applied to 8.5 million Windows machines worldwide → global outage (airlines, banks, hospitals, 911 centers affected). Delta alone canceled 7,000+ flights, lost ~$550 million.

**Root causes (memorize these 5):**
1. **Inadequate staging & testing** — no sandbox test caught the bug
2. **Monolithic deployment** — ALL customers got the update simultaneously (no canary/phased rollout)
3. **Poor change management** — no Change Control Board (CCB)
4. **Over-reliance on a single upstream vendor/library** — single point of failure
5. **Insufficient monitoring** — no automatic rollback triggered by error spikes

**Fixes proposed:** phased/canary rollouts, automated rollback on threshold breach, semantic versioning, release-readiness gates, diversify dependencies.

**Analogy:** Imagine a chef sending the same (untested) new recipe to every restaurant in a chain on the same night, instead of trialing it in one restaurant first. One bad ingredient batch and the entire chain gets sick simultaneously.

**Likely question angle:** "Which principle would have prevented this?" → Answer: Canary/phased rollout, or a Change Control Board.

---

## 3. Software Development Life Cycle (SDLC) Models

All SDLCs generally follow: **Requirements → Design → Implementation → Testing → Release → Maintenance**

### Waterfall Model
- Sequential, non-overlapping, each phase signed off and frozen before the next
- **Analogy:** Building a house floor by floor — you can't add plumbing to the 2nd floor once it's built and inspected.

| Pros | Cons |
|---|---|
| Simple, clear phases | Assumes frozen requirements |
| Easy to manage/departmentalize | Poor for long projects, "Big Bang" delivery |
| Each phase has deliverables + reviews | High risk if requirements change late |

**When to use:** Short projects, stable requirements, regulatory requirements (e.g., **University Payroll System** — case study answer in your notes).

### V-Model
- Like Waterfall but each development phase has a **corresponding testing phase** planned in parallel (requirements↔system testing, design↔integration testing, code↔unit testing)
- **Analogy:** Building a bridge where every engineering decision has a matching safety inspection planned from day one — not tested only at the end.

**When to use:** Safety-critical systems (medical devices, aviation) — e.g., **Patient Monitoring System** case study.

### Spiral Model (Boehm)
- Combines Waterfall + prototyping in repeating "spirals" (loops)
- Each loop = 4 activities: **1) Determine objectives 2) Risk analysis 3) Develop & test 4) Plan next iteration**
- **Analogy:** A mountain climber who checks for danger (risk analysis), takes one step (build/test), re-evaluates, then takes the next step — rather than sprinting up blindly.

**When to use:** High-risk, high-uncertainty, complex projects (e.g., **Autonomous Vehicle Control System**).

| Pros | Cons |
|---|---|
| Early warning of problems | Complex to manage |
| Accommodates change | Needs skilled risk assessment |

### Incremental Model
- Complete system divided into functional pieces; each **increment adds NEW functionality** to what's already delivered
- **Analogy:** Launching a shopping mall floor by floor — Floor 1 (grocery) opens while Floor 2 (electronics) is still being built.
- Case study: **University Management System** — registration first, then attendance, then exams...

### Iterative Model
- Same features get **revisited and refined repeatedly** based on feedback (not new features — improving existing ones)
- **Analogy:** A sculptor repeatedly refining the same statue — smoothing, adjusting proportions — not adding a new statue each time.
- Case study: **Food Delivery App** — search feature refined in cycle 1, ordering refined in cycle 2, etc.

**KEY DIFFERENCE (very testable):**
| Incremental | Iterative |
|---|---|
| Adds NEW functionality each cycle | REFINES existing functionality each cycle |
| "Build more" | "Build better" |

### Agile (covered separately below)

### Quick "Identify the Model" table (case-study style questions love this):
| Clue in scenario | Model |
|---|---|
| Requirements frozen, heavy documentation, regulatory | Waterfall |
| Safety-critical, test case for every requirement | V-Model |
| High risk/uncertainty, need prototypes before committing | Spiral |
| Deliver working pieces in stages, don't wait for full system | Incremental |
| Build → get feedback → refine same features → repeat | Iterative |
| Requirements uncertain, frequent releases, customer collaboration | Agile |

---

## 4. Security Development Life Cycle (SecDLC)

Maps security into every phase of SDLC:

| Phase | Activity |
|---|---|
| Pre-SDL | Security training (threat modeling, secure coding, privacy) |
| Requirements | Identify what needs security review/pen testing; Threat Modeling + Security Risk Assessment (SRA) |
| Design | Attack surface reduction, defense in depth, risk analysis |
| Implementation | Approved tool lists, prohibit unsafe functions, static analysis |
| Verification | Dynamic analysis, fuzz testing, update threat model |
| Release | Incident response plan, Final Security Review (FSR) |

**Security Risk Assessment (SRA) steps:** Identify Assets & Stakeholders → Identify Threats/Attackers → Analyze & categorize Risk (Likelihood × Impact) → Define mitigations

**Analogy:** Building a bank vault — you don't just add a lock at the end. You plan the walls (design), choose fireproof materials (implementation), test if a drill can break in (verification), and have a guard on call after opening (monitoring).

**STRIDE (from your Unit 12 notes) — threat categories:**
- **S**poofing, **T**ampering, **R**epudiation, **I**nformation Disclosure, **D**enial of Service, **E**levation of Privilege

Example: attacker changes URL from `/customer/1001` to `/customer/1002` to see someone else's data → **Information Disclosure**

---

## 5. Agile, Scrum, XP

### Why Agile?
Traditional models struggle when requirements change fast. Agile = umbrella term for methods (Scrum, XP, DSDM, FDD, RUP) built on:
1. Continual realignment with customer needs
2. Reducing planning overhead for fast reaction to change

**Agile Manifesto:** *"While there is value in the items on the right, we value items on the left more"* — i.e., prioritizes: Individuals & interactions > processes/tools; Working software > documentation; Customer collaboration > contract negotiation; Responding to change > following a plan.

### 5 Agile Principles (know all 5)
1. **Customer involvement** — throughout development
2. **Incremental delivery** — software built in increments
3. **People not process** — trust the team's own working style
4. **Embrace change** — design for changing requirements
5. **Maintain simplicity** — eliminate unnecessary complexity

**Analogy:** Agile is like cooking for a picky guest who keeps tasting each dish as you make it and telling you what to adjust — rather than cooking the entire 5-course meal blind and hoping they like it.

| Pros | Cons |
|---|---|
| Adapts to change | Hard to predict final cost/timeline |
| Frequent working releases | Needs constant customer availability |
| High customer satisfaction | Documentation can suffer |

### Scrum — Key Activities (memorize durations!)
| Activity | Duration | Purpose |
|---|---|---|
| Sprint Planning | 30–60 min, biweekly | What can be delivered this sprint? |
| Daily Standup | 15 min, daily | Yesterday/today/blockers |
| Sprint Review | 30–60 min, biweekly | Demo to product owner |
| Sprint Retrospective | 30–60 min, biweekly | What went well/improve |

**Roles:** Product Owner (manages backlog, priorities), Scrum Master (facilitates, removes impediments — NOT a boss), Development Team.

**Product Backlog vs Sprint Backlog:**
| Product Backlog | Sprint Backlog |
|---|---|
| ALL planned work, continuously evolving | Work selected for ONE sprint |
| Managed by Product Owner | Created by dev team during Sprint Planning |

### Extreme Programming (XP) — 12 Core Practices (recognize names + one-liners)
1. **Planning Game** — release planning + iteration planning
2. **Simple Design** — no big upfront design, evolve it; use "spikes" (small POCs)
3. **Test-Driven Development (TDD)** — write test before code
4. **Code Standard** — team-agreed coding style
5. **Refactoring** — restructure code without changing behavior
6. **Pair Programming** — 2 devs, 1 keyboard (driver/navigator roles, swap periodically)
7. **Collective Code Ownership** — anyone can fix any code
8. **Continuous Integration** — merge & auto-test frequently
9. **Small Releases** — frequent MVP releases
10. **System Metaphor** — shared naming/understanding (e.g., `Order_Food()`)
11. **Onsite Customer** — customer representative embedded with team (like Scrum's Product Owner)
12. **Sustainable Pace** — no burnout, buffer time built in

**Analogy for Pair Programming:** Like a driving instructor (navigator, watches big picture/mistakes) and student driver (pilot, focuses on the road) — periodically swapping seats.

### INVEST Criteria for User Stories
**I**ndependent, **N**egotiable, **V**aluable, **E**stimable, **S**mall, **T**estable

Example bad story: *"As a student, I want the app to be fun and easy to use"* → fails **Testable** (no acceptance criteria).

### Lean-Agile Principles (6, know the names)
Eliminate waste, Amplify learning, Decide as late as possible, Deliver as fast as possible, Empower the team, Build integrity in, Optimize the whole.

---

## 6. Requirements Engineering

**Requirement** = property that must be exhibited by the software; describes externally visible behavior — **what**, not **how**.

### The "4+1" Process (iterative)
**Elicitation → Analysis → Specification → Validation** (+ Management, ongoing)

### Elicitation Techniques
| Active (ongoing interaction) | Passive (infrequent interaction) |
|---|---|
| Interviews | Use cases |
| Facilitated meetings | Business process analysis |
| Role-playing | Workflows |
| Prototypes | Questionnaires |
| Ethnography | Checklists |
| Scenarios | Documentation |

**Ethnography example (Flipkart case study):** Riding along with delivery drivers revealed real bottlenecks (traffic, COD cash counting, gated communities) that interviews/GPS data alone missed. **Analogy:** Like a manager who works a shift on the shop floor instead of just reading reports — sees the *real* problems.

### Functional vs Non-Functional vs Domain Requirements
| Type | Definition | Example |
|---|---|---|
| **Functional** | What the system should DO | "System sends confirmation email after order" |
| **Non-Functional** | Constraints/quality attributes | "System must respond within 2 seconds" |
| **Domain** | Constraints from the application domain | "Must comply with IEC 60601 medical equipment standard" |

**Analogy:** Functional = "the car must be able to turn left." Non-functional = "the car must turn within 3 seconds and not exceed 40 dB noise." Domain = "the car must meet emissions regulations for the country it's sold in."

### Properties of a GOOD Requirement (memorize + practice transformations)
Concise, Clear, Unambiguous, Verifiable, Measurable, Feasible, Traceable

**Practice example from your notes:**
- Bad: "All screens must appear quickly"
- Good: "Screen must appear within 2 seconds" (Clear, Measurable, Verifiable)

### User Requirements vs System Requirements
| User Requirements | System Requirements |
|---|---|
| Natural language, written for/by customers | Structured, detailed technical document |
| "What the user wants" | "What must be implemented" — may be part of a contract |

### SRS (Software Requirements Specification) — IEEE 830 structure
1. Introduction (Purpose, Scope, Definitions, References, Overview)
2. Overall Description (Product perspective, functions, users, constraints)
3. Specific Requirements (External interfaces, Functional, Non-functional, Design constraints)

### Requirements Traceability Matrix (RTM)
Tracks each requirement through: Architecture → Design → Implementation → Unit Test → Functional Test → System Test → Acceptance Test.
**Analogy:** Like a package tracking number that follows one requirement through the entire "shipping" journey of development so nothing gets lost.

### Requirements Change Management
Requirements change due to: better problem understanding, evolving tech, customer needs shifting. Uncontrolled change → cost/schedule/quality damage → needs a **formal change management process**.

### MoSCoW Prioritization
**M**ust have, **S**hould have, **C**ould have, **W**on't have (this time)

---

## 7. UML, Use Cases, Personas

### Use Case Diagram Basics
- **Actor** = person/device/system interacting with the system
- **Use Case** = a piece of functionality (oval)
- **Include** vs **Extend**:
  - **Include** = A always triggers B (e.g., "Place Order" always includes "Process Payment")
  - **Extend** = B optionally adds to A (e.g., "Make Payment" can be extended by "Apply Discount")

**Analogy:** Include = "you must show ID to enter the building" (mandatory sub-step). Extend = "you may use the express lane if you have <10 items" (optional add-on).

### Use Case Flow
Main success scenario (3–9 numbered steps) + Alternate flows (variations) + Exception flows (errors).

### Personas & User Scenarios
**Persona** = fictional but realistic user archetype (e.g., "Persona A: first-year undergrad").
**User Scenario/Story format:** *"As a [persona], I want to [goal] so that [benefit]."*

**Analogy:** Personas are like character sheets in a play — giving the dev team a concrete "who" to design for instead of a vague "the user."

---

## 8. Requirements Discovery Techniques

| Technique | Description | Weakness |
|---|---|---|
| **Interviewing** | Closed (fixed Qs) or open (exploratory) | Domain jargon gaps; tacit knowledge hard to articulate |
| **Ethnography** | Observe people working, don't interrupt | Time-consuming, expensive |

---

## 9. Testing Introduction

### Verification vs Validation (Boehm) — VERY high-yield
| Verification | Validation |
|---|---|
| "Are we building the product RIGHT?" | "Are we building the RIGHT product?" |
| Checks conformance to spec | Checks it meets user needs |
| Static (no code execution) | Dynamic (executes code) |
| Reviews, walkthroughs, inspections | Black box, white box testing |
| Happens BEFORE validation | Happens after |

**Analogy:** Verification = checking the cake recipe was followed exactly. Validation = tasting the cake to see if the customer actually likes it.

### Error / Defect / Bug / Fault / Failure Terminology (VERY testable)
| Term | Meaning |
|---|---|
| **Error** | Human mistake by a developer (misunderstanding, typo in logic) |
| **Fault** | Incorrect code/design resulting FROM an error |
| **Defect** | Deviation between expected & actual, found during review/testing (BEFORE production, generally) |
| **Bug** | Tester's term for a defect found in dev environment before shipping |
| **Failure** | Observable incorrect behavior when defect reaches the END CUSTOMER |
| **Issue** | Raised by end user when product doesn't meet expectations |

**Memory chain:** Error (human mistake) → Fault (bad code from that mistake) → Defect/Bug (found by tester before release) → Failure (reaches customer).

### Levels of Testing
| Level | Focus |
|---|---|
| Unit | Individual component |
| Integration | Interfaces between components |
| System | Whole system behavior |
| Acceptance | Done by customer/user — "does it meet the need?" |

### Test Planning Process (9 steps, know the flow)
Understand scope → Establish test adequacy criteria → Evolve test strategy → List deliverables → Create test schedule → Allocate resources → Identify milestones → Risk management → Establish metrics

### Security Validation Planning
Ensures security controls are **measurable, verifiable, tested** throughout SDLC. Includes: Security Requirements Traceability, Validation Objectives, Validation Methods (review/pentest/fuzzing), Ownership, Timing, Tools.

---

# UNIT 2: Project Management, Architecture, Design & Quality

## 1. Software Project Management

**Project** = temporary effort creating unique value, with a beginning/end, team, budget, schedule.

**PM Roles (3, know all):**
1. **Leader** — direction, builds team
2. **Liaison** — link between client, team, upper management
3. **Mentor** — guides team, provides advice

**Analogy:** A PM is like an orchestra conductor — not playing an instrument themselves, but ensuring every musician (developer) knows their part, stays on tempo (schedule), and the whole piece (product) comes together.

### Project Management Triangle (Triple Constraint)
**Scope, Cost, Time** — you cannot change one without affecting the other two, while maintaining quality.

**Analogy:** "Good, Fast, Cheap — pick two." If you want it fast and cheap, quality/scope suffers.

---

## 2. Agile Planning

**Release Planning** (months ahead, what features in a release) vs **Iteration Planning** (2–4 weeks, next increment).

**Planning Game (from XP):** Team estimates stories in **effort points**; **velocity** = points completed per day/sprint, used to forecast total effort.

**Task Allocation:** Stories broken into tasks (4–16 hours each); developers self-select tasks (ownership → motivation).

**Key Rule:** Delivery schedule is NEVER extended — if features can't be completed, **scope is reduced** instead.

---

## 3. Time Estimation — COCOMO (HIGH-YIELD, MEMORIZE FORMULAS)

**COCOMO** = Constructive Cost Model — regression model based on **KLOC** (thousand lines of code).

**Formula (Basic Model):**
```
Effort (E) = a × (KLOC)^b   [person-months]
Time (T)   = c × (Effort)^d [months]
Staff      = Effort / Time
```

**Constants table (MEMORIZE):**
| Mode | a | b | c | d | Example |
|---|---|---|---|---|---|
| **Organic** | 2.4 | 1.05 | 2.5 | 0.38 | Inventory Management System (small team, familiar problem) |
| **Semi-detached** | 3.0 | 1.12 | 2.5 | 0.35 | Developing an OS/DBMS (moderate complexity) |
| **Embedded** | 3.6 | 1.20 | 2.5 | 0.32 | ATM software (high complexity, real-time, hardware-tied) |

**Memory trick:** As complexity rises (Organic → Semi-detached → Embedded), **a and b go UP** (more effort per KLOC), **d goes DOWN** (schedule compresses less easily relative to effort).

**Worked Example (practice this pattern):**
*10 KLOC, Organic mode:*
- E = 2.4 × (10)^1.05 ≈ 27 person-months
- T = 2.5 × (27)^0.38 ≈ 8.8 months
- Staff = 27 / 8.8 ≈ 3 people

**Analogy:** COCOMO is like a recipe scaling calculator — tell it how many "servings" (KLOC) and what "difficulty level" (Organic/Semi-detached/Embedded), and it estimates cooking time (schedule) and how many chefs (staff) you need.

---

## 4. Project Scheduling & Risk Management

**Scheduling principles (7):** Compartmentalization, Interdependency, Time allocation, Effort validation, Defined responsibilities, Defined outcomes, Defined milestones.

### Risk Classification (2 dimensions)
**By type:** technical, organizational, etc.
**By what's affected:**
| Risk Type | Affects | Example |
|---|---|---|
| **Project risk** | Schedule/resources | Staff turnover, hardware unavailable |
| **Product risk** | Quality/performance | Size underestimate, CASE tool underperformance |
| **Business risk** | The organization | Competitor releases first, tech becomes obsolete |

**Analogy:** Project risk = the moving truck breaks down (delays the move). Product risk = the furniture doesn't fit through the door (quality issue). Business risk = you find out the new house is in a declining neighborhood (bad investment).

### Risk Management Process (4 steps)
**Identification → Analysis (likelihood × impact) → Planning (mitigation) → Monitoring**

---

## 5. RACI Matrix

| Letter | Meaning | Rule |
|---|---|---|
| **R** – Responsible | Does the work | Can have multiple |
| **A** – Accountable | Owns the outcome, final say | **ONLY ONE per task** |
| **C** – Consulted | Gives input (two-way) | Before task completion |
| **I** – Informed | Kept updated (one-way) | After decision/action |

**Analogy:** In a restaurant kitchen — the line cook is **R** (cooking the dish), the head chef is **A** (owns whether it's served), a supplier consulted about an ingredient substitution is **C**, and the waiter told the dish is ready is **I**.

---

## 6. Gantt Chart & Critical Path

**Gantt Chart** = bars showing task start/duration/end against a timeline.

**Critical Path** = longest sequence of dependent tasks; **any delay here delays the whole project**. Tasks on the critical path have **zero slack/float**.

**Analogy:** Making a 3-course meal — if the main course takes 40 min and dessert takes 10 min (started after), but the appetizer only takes 5 min, the critical path is Appetizer→Main→Dessert timing, not the appetizer itself (which has slack).

---

## 7. Teamwork

**Group Cohesiveness Benefits:** shared quality standards, mutual learning, knowledge continuity (if someone leaves), collective refactoring.

**3 factors for effective teams:** People (diverse mix), Group organization, Technical/managerial communication.

---

## 8. Software Architecture

**Definition:** Top-level decomposition of a system into major components + how they interact.

**Levels of Abstraction (4, same as Unit 1 — reused):**
Requirements (high "what") → Architecture (high "how"/mid "what") → Design (mid "how"/low "what") → Code (low "how")

**Analogy:** Requirements = "I need a place to live." Architecture = "3-bedroom house with a garage and kitchen layout." Design = "kitchen counter is granite, cabinets are oak, positioned near the window." Code = the actual construction/nails/wiring.

### Design vs Architecture Questions
| Design Question | Architecture Question |
|---|---|
| "How do I add a menu item?" | "How do I extend the app with a plugin?" |
| "What lock protects this data?" | "How does the system scale to billions of hits?" |

---

## 9. Case Study: Therac-25 — HIGH-YIELD

**What:** Computer-controlled radiation therapy machine. Software **replaced hardware safety interlocks** present in older models (Therac-6/20).

**What went wrong:**
1. **Race conditions** — fast operator input caused misconfiguration
2. **Integer overflow** — safety-check variable overflowed 255→0, tricking system into thinking it was safe
3. **Removed hardware interlocks** — pure software reliance = lethal bugs
4. **Cryptic errors** ("Malfunction 54") operators learned to bypass
5. **Inadequate testing** — software/UI barely tested vs hardware

**Result:** At least 6 incidents, multiple deaths/injuries.

**Architecture principles violated:** No separation of concerns, no fault isolation, reuse without redesign, no defensive programming/redundancy.

**Analogy:** Like removing a car's physical seatbelt because "the airbag software will handle it" — and then the airbag software has a bug.

**Likely question:** "What architectural principle would have prevented Therac-25?" → Fault isolation / defense in depth / hardware interlocks as backup.

---

## 10. Common Architectural Patterns (SCENARIO-MATCHING TABLE — memorize this cold)

| Clue in Scenario | Pattern |
|---|---|
| Divided into presentation/business/database layers | **Layered** |
| Browser talks to a web server | **Client–Server** |
| Data passes through sequential processing stages | **Pipe-and-Filter** |
| Components respond to events (pub/sub) | **Event-Driven** |
| Independently deployable small services | **Microservices** |
| Several modules access ONE shared central database | **Repository** |
| AI components share a common workspace, controller decides who acts next | **Blackboard** |
| Middleware coordinates distributed component communication | **Broker** |
| Model / View / Controller separation | **MVC** |
| Business functionality exposed as reusable enterprise services | **SOA** |
| Nodes act as both client AND server | **P2P** |
| System built from reusable pluggable modules | **Component-Based** |

### Pattern Details with Analogies

**Layered** — like floors in a building; each floor (layer) only talks to the one directly above/below. *Example: Banking app (Presentation → Business Logic → Data Access → Database).*

**Client-Server** — like a restaurant: customer (client) orders, kitchen (server) prepares and returns the meal. *Example: online banking.*

**Pipe-and-Filter** — like an assembly line, each station (filter) transforms the product and passes it via a conveyor belt (pipe) to the next. *Example: Compilers (lexical→syntax→semantic→code generation).* Filters must NOT share state; no cycles allowed.

**Event-Driven** — like a fire alarm system: when smoke is detected (event), multiple systems react independently (sprinklers, alarms, fire dept notification) without the smoke detector directly calling each one. *Example: E-commerce OrderPlaced event triggers Payment, Inventory, Notification services.*

**Blackboard** — like a group of doctors (knowledge sources) looking at one shared patient chart (blackboard) on a wall, each contributing their specialty's findings, with a senior doctor (controller) deciding who examines next. *Example: speech recognition, medical diagnosis.*

**Microservices** — like a food court with independent stalls (services), each with its own menu/staff/kitchen — one stall's problems don't shut down the others. *Example: Netflix, Amazon, Uber.*

**Repository** — like a shared company filing cabinet that every department (HR, Finance, Sales) accesses. *Example: Hospital system (Doctor, Pharmacy, Billing modules all hitting one DB).*

**Broker** — like a real estate agent (broker) who connects buyers and sellers without them directly knowing each other's exact location/identity. *Example: distributed systems, middleware.*

**MVC** — like a restaurant: Model = the kitchen/recipe data, View = the plated dish presentation, Controller = the waiter coordinating between kitchen and customer.

**SOA vs Microservices (differentiate!):**
| SOA | Microservices |
|---|---|
| Larger, enterprise-wide services | Smaller, independently deployable |
| Centralized governance (Enterprise Service Bus) | Decentralized, lightweight communication |

---

## 11. Monolith → Microservices Migration Patterns

**Strangler Pattern:** New microservices built alongside the old monolith; a gateway routes requests gradually to new services until the monolith is fully "strangled" (retired).
**Analogy:** Renovating a house room by room while still living in it, instead of demolishing and rebuilding all at once.

**Anti-Corruption Layer:** A translation layer between old and new systems so new services aren't "corrupted" by legacy concepts.
**Analogy:** A translator at a UN meeting — ensures two systems speaking "different languages" can still communicate without either changing their native language.

### Reasons to break up a monolith
Faster deployment, independent scaling, different tech per component, codebase too big/complex, security isolation, better org alignment.

### Microservices Challenges (know examples)
Operational complexity, inter-service communication failures (retry storms), data consistency (Saga pattern — local commits + compensating transactions), testing difficulty, governance/standardization gaps, DevOps overhead, versioning/backward compatibility, security/compliance (e.g., HIPAA).

---

## 12. Security Architecture

**4 Key Goals:** Reduce attack surface, Separation of concerns, Defense-in-depth, Align with threat models.

**Trust Boundary:** A boundary within which components are "mutually trusted." Example: Patient + doctor/nurse are inside the trust boundary; other patients/other staff are outside.

**Analogy:** Trust boundary = the VIP section of a concert — only people with the right wristband (credentials) are inside; everyone else, even other concert-goers, is "untrusted" relative to that zone.

---

## 13. Software Design Principles

### Cohesion (WITHIN a module) — HIGH is GOOD
**7 types, weakest → strongest (MEMORIZE ORDER):**
**Coincidental → Logical → Temporal → Procedural → Communicational → Sequential → Functional**

| Type | Meaning | Analogy |
|---|---|---|
| Coincidental (worst) | No relation at all | A junk drawer — random unrelated items |
| Logical | Similar category, selected by a flag | A drawer labeled "input devices" — mouse, keyboard, all "inputs" but unrelated internally |
| Temporal | Grouped by WHEN they run | Morning routine — brush teeth, make coffee — unrelated tasks, same time slot |
| Procedural | Must run in a specific ORDER | Recipe steps — must follow sequence, but don't share data directly |
| Communicational | Operate on the SAME data | Everyone editing the same shared spreadsheet |
| Sequential | Output of one = input of next | Assembly line — each station's output feeds the next |
| Functional (best) | Everything serves ONE clear purpose | A calculator's "add" button — everything inside does exactly one job |

**Memory sentence:** *"Cats Like To Purr, Cuddle, Sleep, Fully"* (Coincidental, Logical, Temporal, Procedural, Communicational, Sequential, Functional)

### Coupling (BETWEEN modules) — LOW is GOOD
**7 types, worst (tightest) → best (loosest) (MEMORIZE ORDER):**
**Content → Common → External → Control → Stamp → Data → Message**

| Type | Meaning | Analogy |
|---|---|---|
| Content (worst) | Directly modifies another module's internal data | Reaching into someone's private diary and editing it yourself |
| Common | Shared global variable | Multiple people writing in the same shared notebook |
| External | Depend on an externally imposed format/protocol | Following a foreign country's customs form format |
| Control | One module tells another WHAT to do via a flag | Handing someone a note saying "do task #3" |
| Stamp | Passing a whole object when only part is needed | Handing someone your entire wallet when they just need your ID |
| Data | Pass ONLY the exact data needed | Handing someone just your ID number, nothing else |
| Message (best) | Communicate via well-defined messages/APIs, no shared internals | Sending a text message — you don't need to know how their phone works internally |

**Memory sentence:** *"Careful Crooks Enter Carefully, Stealing Data Messages"* (Content, Common, External, Control, Stamp, Data, Message)

**CRITICAL DISTINCTION:** Cohesion = relationships WITHIN one module (want HIGH). Coupling = relationships BETWEEN modules (want LOW).

---

## 14. API vs ABI

| API (Application Programming Interface) | ABI (Application Binary Interface) |
|---|---|
| Source-level contract: functions, inputs, outputs | Binary-level: calling conventions, data sizes, symbol names |
| Governs how code CALLS other code (compile-time) | Governs how COMPILED binaries interoperate (runtime) |
| Example: `open()`, `read()`, `close()` function signatures | Example: struct layout, function calling convention |

**Analogy:** API is like a restaurant menu (what you can order and how to ask for it). ABI is like the actual kitchen equipment standard (if the stove's plug shape changes, even though the menu is the same, the kitchen breaks).

**Real breakage example:** OpenSSL 1.1.0 changed internal structures → broke apps that hadn't recompiled (ABI break), even though their high-level API usage hadn't changed.

**Versioning:** Semantic Versioning = MAJOR.MINOR.PATCH (MAJOR = breaking change, MINOR = backward-compatible addition, PATCH = bug fix).

---

## 15. Error Handling

**Patterns:**
- **Return code checks** (C-style) — caller must explicitly check
- **Exception-based flow** (Java/Python) — throw/catch at boundaries
- **Result/Option types** (Rust/Swift) — forces caller to handle both success/failure explicitly
- **HTTP status codes** (REST) — 200 OK, 400 Bad Request, 401 Unauthorized, 404 Not Found, 500 Server Error

**Best practices:** Fail Fast, Graceful Degradation, Consistent strategy, Automated tests for error paths.

**Analogy — Graceful Degradation:** If a car's GPS fails, a well-designed car still lets you drive (just without navigation) — it doesn't shut the engine off.

---

## 16. Usability Engineering

**LESEM — 5 aspects of usability (memorize acronym):**
**L**earnability, **E**fficiency, **S**atisfaction, **E**rrors, **M**emorability

**Case study angle — Hawaii false missile alert:** Confusing UI (test vs. real alert not clearly separated) → real panic. Lesson: no safeguard separation, no quick cancellation mechanism.

**Testing methods:** Lab study, A/B testing, Shadowing ("fly on the wall" — observe real work), Prototyping (mockups, paper prototypes, Wizard of Oz).

**Dogfooding** = using your own product internally before public release.

**Analogy:** A/B testing is like a bakery quietly offering two versions of a cupcake to different customers and counting which one sells more — without asking opinions directly.

---

## 17. Software Quality

### FLURPS+ Quality Attributes
**F**unctionality, **L**ocalization, **U**sability, **R**eliability, **P**erformance, **S**upportability (+Extensibility)

### Cost of Software Quality (COSQ) — HIGH-YIELD
```
Cost of Quality = Cost of Conformance + Cost of Non-Conformance
```

| Cost of Conformance | Cost of Non-Conformance |
|---|---|
| **Prevention** — training, standards, planning (BEFORE defects occur) | **Internal Failure** — rework, debugging, retesting (found BEFORE release) |
| **Appraisal** — testing, reviews, audits (CHECKING for defects) | **External Failure** — support calls, patches, refunds, reputation loss (found AFTER release, by customer) |

**Memory trick:** 
- Conformance = "doing it right" (Prevention = stopping defects; Appraisal = checking for them)
- Non-Conformance = "it went wrong" (Internal = caught in-house; External = customer found it — MORE expensive)

**Analogy:** Prevention = teaching a chef proper food safety (before cooking). Appraisal = a health inspector checking the kitchen (during/before serving). Internal failure = throwing out a spoiled dish before it's served. External failure = a customer gets food poisoning and sues (most expensive, includes reputation damage).

### Case Study: Heartbleed (OpenSSL, 2014)
**Bug:** TLS Heartbeat didn't validate that the requested payload length matched actual data sent → attacker could request 64KB but send a few bytes → server leaked memory contents (private keys, session tokens).

**Quality failures:** No formal code review, insufficient testing (no fuzzing/boundary tests), unstructured release process, poor documentation.

**Lesson:** A single missing bounds-check (`memcpy` without length validation) caused a global security crisis — shows why Appraisal costs (testing) are cheap compared to External Failure costs (worldwide emergency patching).

---

## 18. Technical Debt

**Definition:** Design/implementation shortcuts that are expedient short-term but make future changes more costly.

**Analogy (the classic one — use this in exam):** Like financial debt — a shortcut gives you speed now (like a loan), but you pay "interest" later through harder maintenance.

**Types:**
1. **Deliberate** — knowingly chosen shortcut ("we'll fix it after launch")
2. **Accidental** — unintentional, due to inexperience/poor design
3. **Environmental/Contextual** — external changes make old decisions problematic (e.g., deprecated library)

**Causes:** Tightly-coupled components, poorly-specified requirements, business pressure, lack of process/tests/documentation, delayed refactoring, long-lived branches.

**Case study: Knight Capital (2012)** — New code deployed to only 7 of 8 servers; the 8th ran old test-mode code from 2003 → sent erroneous stock orders → **$460 million lost in 45 minutes**, near-bankruptcy.
**Lesson:** Deployment inconsistency + no rollback safety = catastrophic technical debt realized instantly.

**Managing debt:** Refactoring, code reviews, automated testing, documentation, maintain a "debt backlog," prioritize by Impact × Frequency × Cost of Delay.

---

## 19. Anti-Patterns (recognize names + one example each)

| Anti-Pattern | Meaning | Example |
|---|---|---|
| **God Object/Class** | One class does everything | `UserManager` handling login+payment+email+reports |
| **Spaghetti Code** | Tangled, hard-to-follow control flow | Excessive nested if/goto |
| **Copy-Paste Programming** | Duplicated code instead of reuse | `calculateTax()` copied into 3 modules |
| **Golden Hammer** | Overusing a familiar tech for everything | "We used microservices before, so use it for everything" |
| **Big Ball of Mud** | No clear architecture, everything coupled to everything | System where every module depends on every other |
| **Premature Optimization** | Optimizing before identifying real bottlenecks | Optimizing a rarely-used function while ignoring the real slow DB query |
| **Lava Flow** | Obsolete code nobody dares remove | Old reporting module nobody understands anymore |
| **Poltergeists** | Many useless intermediate classes that just pass data along | Controller→Helper→Manager→Handler chain doing nothing meaningful |
| **Singleton Abuse** | Overusing Singleton pattern | Global managers everywhere → hidden dependencies, hard to test |

---

## 20. Test-Driven Development (TDD)

**Cycle: RED → GREEN → REFACTOR**
1. **RED** — write a failing test first
2. **GREEN** — write minimum code to pass it
3. **REFACTOR** — improve code without changing behavior (tests still pass)

**Analogy:** Like setting a target on a dartboard (test) BEFORE throwing the dart (writing code) — you know exactly what "success" looks like before you act.

**Benefits:** Code coverage (every line has a test), regression protection, simplified debugging, tests = documentation, supports safe refactoring.

**Drawbacks:** Extra upfront effort, poor tests give false confidence, UI testing is harder, requires testing skill.

---

# QUICK-REFERENCE: High-Yield Formulas & Sequences

1. **COCOMO:** E = a(KLOC)^b, T = c(E)^d | Organic(2.4,1.05,2.5,0.38) / Semi-detached(3.0,1.12,2.5,0.35) / Embedded(3.6,1.20,2.5,0.32)
2. **Cohesion (worst→best):** Coincidental→Logical→Temporal→Procedural→Communicational→Sequential→Functional
3. **Coupling (worst→best):** Content→Common→External→Control→Stamp→Data→Message
4. **RACI:** R=Responsible(does), A=Accountable(owns, only 1), C=Consulted(2-way), I=Informed(1-way)
5. **INVEST:** Independent, Negotiable, Valuable, Estimable, Small, Testable
6. **LESEM:** Learnability, Efficiency, Satisfaction, Errors, Memorability
7. **FLURPS+:** Functionality, Localization, Usability, Reliability, Performance, Supportability(+)
8. **STRIDE:** Spoofing, Tampering, Repudiation, Information Disclosure, DoS, Elevation of Privilege
9. **Verification vs Validation:** Right product vs Right way; Static vs Dynamic
10. **Error→Fault→Defect/Bug→Failure** (mistake → bad code → found in-house → reaches customer)
11. **Cost of Quality:** Conformance(Prevention+Appraisal) + Non-Conformance(Internal+External Failure)
12. **TDD:** Red → Green → Refactor
13. **Agile 5 principles:** Customer involvement, Incremental delivery, People not process, Embrace change, Maintain simplicity
14. **XP 12 practices:** Planning Game, Simple Design, TDD, Code Standard, Refactoring, Pair Programming, Collective Ownership, CI, Small Releases, System Metaphor, Onsite Customer, Sustainable Pace
