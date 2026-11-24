# Software Engineering (UE23CS341A) – Exam Notes: Unit 3 & Unit 4

Exam-oriented, application-focused notes built from your Unit 3 and Unit 4 slide decks, with gaps in the syllabus filled from standard SE knowledge (marked **\[Supplement\]**). Exam format: 40 marks, all MCQ.

**How to use:** Read the notes → memorise the *Key Facts* boxes → study the *Comparison Tables* → read the *MCQ Traps* → attempt the practice MCQs → finish with the Revision Sheet and Mock Test (answer key at the very end).

---

# UNIT 3: AI, IMPLEMENTATION, SCM, DEVOPS

## PART A. AI FOR SOFTWARE ENGINEERING (AI4SE)

### A.1 Background: AI, ML, DL, GenAI

- **AI** – machines doing tasks that normally need human intelligence.
- **ML** – subset of AI; systems *learn patterns from data* instead of being explicitly programmed.
- **DL** – subset of ML using multi-layer neural networks.
- **Generative AI (GenAI)** – models (e.g., LLMs) that *generate* new content: text, code, images. Powers Copilot, ChatGPT, Claude.
- **\[Supplement\] AI agent** – an LLM that can plan and use tools (run code, search) to achieve a goal. **Agentic AI** – systems of multiple/autonomous agents pursuing goals with minimal human prompts.

**Supervised learning in one slide:** lots of *labelled data* (input, output pairs) → *training* → *model*; new input → model predicts output (e.g., image → 'dog').

**ML trade-offs (qualities you must balance):** accuracy; capabilities (classification, recommendation, clustering); amount of training data needed; inference latency; learning (training) latency; model size; explainability.

**Black-box view:** ML models are often opaque; we see inputs and outputs but cannot easily explain internal reasoning.

### A.2 SE vs ML – how they differ

| Aspect | Software Engineering (traditional) | Machine Learning |
| --- | --- | --- |
| Focus | Structured, **process-oriented** (Agile, Waterfall) | **Data-focused**; data pre-processing is crucial |
| Nature | Structured methodology | **Experimental** – train, test, refine on empirical data |
| Emphasis | Design → deployment lifecycle | **Algorithms** (supervised, unsupervised) for pattern recognition |
| Evaluation | **Functional correctness** (does it meet spec? pass/fail) | **Accuracy** – precision, recall, etc. (statistical) |
| Specification | Clear pre/post-conditions | Often **no specification** of exact behaviour |

**MCQ Trap:** SE judges *correctness*; ML judges *accuracy*. An ML model is an *unreliable function* – it may make mistakes, its confidence is unreliable, and it is inscrutable.

### A.3 The two connections: AI4SE and SE4AI

|  | **AI for SE (AI4SE)** | **SE for AI (SE4AI)** |
| --- | --- | --- |
| Question | How do we *use AI to do SE* better? | How do we *engineer systems that contain AI/ML components*? |
| Examples | Copilot, AI test generation, AI code review | MLOps, hazard analysis for ML, deciding where the model lives |
| Role of AI | **Tool** for engineers | **Component** inside a product |

### A.4 AI across the SDLC (core slide content)

AI is a **force-multiplier**: it accelerates routine tasks and augments human judgement – it is **not a wholesale replacement of the SDLC**. Areas: code generation, bug detection/fixing, testing automation, project management, documentation, refactoring/optimisation, security enhancement, DevOps/CI-CD, UX design, architecture design.

| SDLC phase | Old-fashioned way | With AI | What humans must still do |
| --- | --- | --- | --- |
| **Project management** | Manual estimates | ML on *historical project data* forecasts timelines, resource allocation, budget (e.g., Microsoft Project, Asana Intelligence) | Make proactive decisions, own the plan |
| **Requirements** | Stakeholder interviews, manual drafts, long reviews | Chatbots for elicitation; LLMs draft specs from user stories/transcripts; **gap analysis** against past projects | **Validate with real stakeholders**, resolve conflicts, manage scope |
| **Architecture & design** | Whiteboards, hand-drawn UML, manual pattern catalogues | Pattern-recommendation engines; diagrams from text; risk tools flag **single points of failure** and security exposure | Weigh trade-offs (performance vs maintainability, monolith vs microservices); peer review |
| **Implementation** | Write every line; IDE does syntax | AI pair-programmer (GitHub Copilot) autocompletes, suggests whole functions, converts pseudo-code to code; style enforcers; AI code review | Understand code you accept; enforce business logic and security |
| **Testing & QA** | Hand-written tests, ad-hoc bug hunts | Test generators infer edge cases; **AI-driven fuzzers**; coverage analysis with remediation hints | Define acceptance criteria, interpret results, handle false positives/negatives |
| **Deployment & DevOps** | Manual scripts/approvals, reactive monitoring | Pipeline optimisers; **predictive anomaly detection**; automated rollback decision support | Set **SLOs**, define rollback policies, 'you broke it, you fix it' culture |
| **Maintenance & evolution** | Manual bug backlogs, calendar-based refactoring | **Bug-triage assistants**; code-health dashboards predicting **tech-debt hotspots**; automated refactoring suggestions | Strategic road-mapping, prioritisation, balance short-term fixes vs long-term health |

**AI in automated testing (active research area):** ML-based test generation (uses code changes, defect history, user behaviour); designing metrics for test effectiveness; intelligent orchestration/prioritisation of test execution; enhancing the CI pipeline.

### A.5 Worked examples from the slides (College Event Management App) – APPLICATION QUESTIONS

**1. AI-generated requirements draft:** browse events, register, organisers create/edit, cancel registration, notifications.

- *Missing requirements (examples):* authentication/roles & access control; event capacity/waitlist; payment (if paid events); admin moderation; data privacy; non-functional requirements (performance, availability).
- *Incomplete requirement:* 'Notifications will be sent' – no channel (email/SMS/push), no timing, no trigger.
- *Why AI misses these:* it works only from the prompt summary, lacks domain/stakeholder context, tends to produce generic requirements.
- *If not caught:* costly rework later (bugs are cheapest to fix early). **Human review is necessary because AI can suggest, but humans must accept/reject.**

**2. AI-generated architecture:** 'client-server with a *single monolithic server* handling event management, registration, notifications, persistence.'

- *Issues:* **single point of failure**; poor scalability under peak registration; tight coupling (notification failure can hurt registration); hard to deploy/maintain independently; security exposure; no caching/queue.
- *Improvements:* modular/microservices split, message queue for notifications, load balancer, database replication, caching.

**3. AI-generated C code:** `strcpy(event_name, input)` into `char event_name[50]`.

- *Vulnerability:* **buffer overflow** (no length check).
- *Mitigation:* validate length; use bounded functions (`strncpy`/`snprintf`) and null-terminate; reject NULL input.

**4. AI-generated tests** (valid registration, empty event name, empty user, event name 500 chars).

- *Gaps:* no NULL input, special characters/SQL-injection strings, **duplicate registration**, capacity-full case, boundary exactly at the limit (e.g., 50/51 chars), concurrency, unicode, performance.
- *Humans must:* define acceptance criteria, handle false positives/negatives, ensure business-critical scenarios are tested.

### A.6 Benefits and limitations of AI coding tools

**Benefits:** increased productivity; assists new programmers; automates boilerplate, documentation, tests.

**Limitations & risks:**

1. **Incorrect / non-optimal code** (Copilot code quality studies show errors and sub-optimal solutions).
2. **Security weaknesses** – roughly **40–45%** of AI-generated snippets contain security flaws. Reported rates: **XSS insecure ≈ 86%**, **log injection ≈ 88%**, **SQL injection ≈ 20%**, **cryptographic weaknesses ≈ 14%**. A study of **1,689 Copilot programs** found **≈ 40%** had exploitable security bugs/design flaws.
3. **Why the faults occur:** trained on public repositories that contain vulnerable code (learns insecure patterns); limited semantic/contextual understanding (authentication, session management, input validation); no application-specific knowledge (user roles, threat model); risk of leaking private keys/sensitive data embedded in generated code.
4. **Over-reliance** – research on knowledge workers shows reduced cognitive effort and critical thinking; 'Copilot making programmers worse'.
5. IP/licensing concerns (see Unit 4 ethics).

### A.7 When should an LLM be used? (decision framework – frequent application MCQ)

Ask four questions:

| Question | Meaning | Slide example |
| --- | --- | --- |
| **Alternative solutions** | Is there a deterministic method that gives better results? | Type-checking Java → **use the compiler, not an LLM** |
| **Error probability** | How often will the LLM be right? (changes over time) | Grading mathematical proofs → error too likely |
| **Risk tolerance** | Cost of a mistake? | Emergency medical answers → cost too high |
| **Risk-mitigation strategies** | Can outputs be verified / errors made cheap? | **Unit test generation → good fit** (run & review tests) |

### A.8 Key takeaways

- **SDLC still governs the process** – AI automates subtasks only.
- **Human oversight is non-negotiable** – validate every AI suggestion for domain context, security/regulation, business impact.
- **New skills, not new fundamentals** – prompting, critical evaluation, toolchain integration; core engineering principles remain your leverage.
- AI is a powerful *team member*, but one among many.

### A.9 \[Supplement\] Vibe coding

**Definition:** Term popularised by Andrej Karpathy (2025): building software by *describing what you want in natural language* to an LLM/agent, accepting generated code largely *without reading or fully understanding it*, then iterating by running it and pasting errors back ('give in to the vibes').

**Typical workflow:** describe idea → AI generates code → run → observe result/error → give feedback prompt → repeat.

| Aspect | Vibe coding | AI-assisted (responsible) engineering |
| --- | --- | --- |
| Code review | Little or none | Every suggestion reviewed |
| Understanding | Not required | Developer understands and owns code |
| Best for | Prototypes, throw-away scripts, hackathons, learning | Production, safety/security-critical systems |
| Risks | Hidden bugs, security holes, unmaintainable code, tech-debt, no tests | Slower but safer |

**Advantages:** very fast prototyping; lowers entry barrier for non-programmers. **Disadvantages:** security flaws, 'black-box' dependencies you cannot maintain, no design/architecture thinking, hard debugging when AI is stuck, licensing issues.

**Good practice:** keep humans in the loop, write tests first, use static analysis and code review, small incremental prompts, version control every step, never paste secrets in prompts.

**MCQ Trap:** Vibe coding is *not* recommended for safety-critical production code without review; it does not remove the need for testing.

### A.10 \[Supplement\] Prompt-engineering basics

Be **specific** (language, framework, constraints); give **context and examples** (few-shot); state the **role and output format**; break big tasks into steps; **iterate**; always ask for tests and edge cases; verify output independently.

---

## PART B. SOFTWARE ENGINEERING FOR AI/ML-BASED SYSTEMS (SE4AI)

### B.1 ML model vs ML system

- **ML model** – the trained artefact (e.g., an object-detection network). Focus: build model from data, evaluate accuracy.
- **ML system** – the *whole product*: model + data pipeline + non-ML code + UI + infrastructure + monitoring + safety mechanisms. **The model is only one component.** Real-world ML systems are mostly *non-ML code* (Sculley et al., 'Hidden Technical Debt in ML Systems'). Many ML systems have *many* ML components (e.g., Apollo autonomous-driving stack; AR smart-glasses with object detection + motion tracking).
- **System thinking:** understand system goals and environment; design and integrate ML and non-ML parts; many roles/stakeholders – an interdisciplinary effort.

**Key phrase:** *ML systems ≠ ML models.*

### B.2 Typical ML pipeline

**Static (offline) phase:** get labelled data (collection, cleaning, labelling) → feature engineering → split into training/evaluation sets → model training → model evaluation → repeat, revising features.

**In production:** evaluate on production data and **monitor** (model monitoring) → select production data for retraining → update model regularly (deployment).

### B.3 DevOps vs MLOps

|  | DevOps | MLOps |
| --- | --- | --- |
| Meaning | Practices for **continuous delivery**; heavy automation | Automation around the **ML pipeline**: training, evaluation, versioning, deployment, monitoring |
| Relationship | General | **Specialised subset of DevOps** for ML |
| Extra concerns | Code, build, deploy | **Data versioning, model versioning, retraining, drift monitoring** |

There is more to ML systems than MLOps – requirements, architecture, safety, UX, and process also matter.

### B.4 What changes with ML (contrast with SE)

- **Experimental** – experiment-driven; **data-driven**; **algorithmic focus**; evaluation by accuracy/precision/recall (continuous refinement).
- **Process/metrics/mindset change needed.** Example from industry study: engineers used to unit-test thinking expect a 100% pass rate; in ML-enabled products, a test suite may legitimately have failures (e.g., '63 failures is OK') because quality is statistical.

**Lack of specification in ML** (high-yield):

- Traditional code has a specification (e.g., `@ensures result = sum of values`) and a deterministic test (`sum({2,3,4}) == 9`).
- For `detectObjects(image)` there is **no precise specification** – you can only test example images.
- Consequences: **breaks modular reasoning, challenges quality assurance, inhibits safety and fairness reasoning, hinders coordination across teams.**

**ML model = unreliable function:** gives no guarantees, may make mistakes, confidence unreliable, opaque, evaluated by accuracy not correctness. Mistakes can cause real harm.

### B.5 Planning for mistakes (fault-tolerant design)

Assume the model *will* be wrong or the environment will violate assumptions.

**Smart-toaster example:**

- *In the model:* limit maximum toasting time; use heat sensor and past outputs – but **hard to make guarantees**.
- *Outside the model (safety assurance):* simple **code check for max toasting time**; **non-ML rule** to shut down if too hot; **hardware thermal fuse**.

**Strategies:**

| Strategy | Idea | Example |
| --- | --- | --- |
| **Human in the loop** | Human reviews/approves final decision | AI cancer diagnosis supports, does not replace, pathologists |
| **Undoable actions** | Make mistakes reversible | Undo/cancel, soft delete |
| **Mistake detection & recovery** | Monitoring, **doer-checker**, fail-over, redundancy | Independent checker validates the ML output |
| **Guardrails** | Hard limits outside the model | Max-time check, non-ML shutdown rule |
| **Containment & isolation** | Limit blast radius of failure | Sandbox the ML component |

**Safety needs multiple layers – never rely solely on the model.**

### B.6 Architecting ML systems

**Hazard analysis:** anticipate mistakes and consequences – *what is the worst that can happen? Is there a backup? Can it be undone? Is non-technical compensation possible?*

**Architecture decisions to make:** major components and their jobs; where components live (**monolith vs microservices**); communication (**synchronous vs asynchronous**); APIs published and who can access; **where inference happens (client-side vs server-side)**; where telemetry is stored; database size/centralised vs decentralised.

**Quality requirements drive architecture:** development/operational cost, time to release; scalability, availability, response time, throughput; security, safety, usability, fairness; ease of modification; for ML – accuracy, ability to collect data, training latency.

**Where should the model live? (cloud / phone / device)** Considerations: input data size; output data size; execution speed/energy; required **latency**; model size and update frequency; cost of operating (distribution + execution); **what happens if the user is offline**; **bandwidth** and latency analysis.

| Placement | Pros | Cons |
| --- | --- | --- |
| **On device (edge)** | Low latency, works offline, privacy | Limited compute/battery, harder model updates |
| **Cloud** | Powerful, easy updates | Network latency, bandwidth cost, needs connectivity |

*Activity answer (AR glasses):* **Motion tracking** needs very low latency → run on glasses/phone. **Object detection** is heavy → phone or cloud, depending on bandwidth/latency/battery.

### B.7 Summary

ML systems ≠ ML models · failures are inevitable (design for graceful degradation) · quality trade-offs are complex (accuracy isn't everything) · architecture matters (where components live) · safety requires multiple layers.

> **MCQ Traps for Part A & B:** (1) AI4SE = AI as a tool; SE4AI = AI as a component. (2) MLOps is a *subset* of DevOps. (3) Humans-in-loop, guardrails and undo are *outside-the-model* safety techniques. (4) LLM is a poor choice when a deterministic tool exists (type checking). (5) AI code security flaw rate ≈ 40–45%.

---

## PART C. IMPLEMENTATION

### C.1 Implementation overview (Design → Code)

- **Input:** design document. **Output:** executable software, which is the input to testing.
- **Key numbers (slides):** **40–50%** of project effort goes to implementation; **1000s–10,000s** of configuration items are managed; **\~80%** of development time is spent *reading* existing code.
- **Before coding decide:** compiled vs interpreted language; development environment; a **Configuration Management Plan**.
- **During coding:** follow coding standards & guidelines, language syntax; address **quality, security, testability**; deliver **unit-tested, peer-reviewed, working code**.
- **Characteristics of the phase:** produces a *high volume of configuration items* (source files, test cases); uses extensive CS knowledge (algorithms); closely tied to quality (clean, maintainable code); **tool-intensive** (compilers, debuggers, GUI builders).
- **Cost of fixing a bug rises steeply the later it is found** – hence the focus on code quality *now*.

**Programming personalities (slide):** *Cryptic Ninja* (one-line unreadable code), *Verbose Novelist* (comment on every line, over-long names), *Chaos Demon* (messy formatting), *The Professional* (clean, consistent). Other types: under-documenter, CYA specialist, CIO type (passes responsibility to other modules), dynamic types (create variables on the fly), fakers (repetitive code), multitasker (wrappers/glue instead of rewriting), true believer (extensive documentation). *Program style reflects the programmer.*

### C.2 Choice of programming language

**Questions to ask:** (1) ecosystem and long-term vendor support? (2) project environment – web, mobile, embedded? (3) new hardware/infrastructure/deployment? (4) client preference? (5) required libraries, features, tools? (6) developer availability/experience – or hire/train? (7) constraints – time, budget, resources? (8) performance needs? (9) security needs / third-party tools?

**By level of abstraction:**

| Type | Idea |
| --- | --- |
| **Assembly** | Maps directly onto CPU architecture |
| **Procedural** (C) | Modest abstraction over the underlying layer |
| **Aspect-oriented** | Separates *aspects* (a sub-program tied to a specific property, e.g., logging) during development |
| **Object-oriented** (Java, C++, Python) | Code in terms of objects |

**Decision factors (with slide one-liners):** performance (Java vs Python for high-frequency trading), development speed (startup with 3-month runway), team expertise (5 C++ experts vs 1 Python beginner), platform constraints (iOS app in JavaScript?), maintenance (who maintains it in 5 years?), integration (must talk to Oracle DB), scalability (100 vs 100 million users), security (banking app in assembly?), cost (Oracle licences vs open source).

**Application scenarios (slide):**

- *Scenario 1 – real-time notification service* (5000+ concurrent users, push to mobile/browser, integrates with **existing Java student portal**, limited budget/existing infrastructure) → **Java** (or Node.js): concurrency, reuse of existing Java ecosystem and infrastructure.
- *Scenario 2 – analytics dashboard* (statistics, visualisation, non-technical users, 6-week prototype) → **Python** (pandas, matplotlib, Streamlit/Dash) for rapid development and libraries.

**MCQ Trap:** There is no 'best' language – choice is driven by project context, team skills and constraints. Compiled = faster execution, errors found at compile time; interpreted = faster development, portable, slower.

### C.3 Factors for effective coding (characteristics of good code)

**1. Simple and clear.** Think before coding. Keep a *reasonable* number of: lines per function, lines/functions per file, arguments per function, levels of nesting, conditions.

**2. Naming and naming conventions.** Names must be meaningful and descriptive; avoid names similar to keywords; do not start variable names with `_` (slide rule); use roughly **2–3 minimum to 30 maximum characters**; do not embed numeric values used elsewhere.

| Style | Rule | Example | Usage (per slide) |
| --- | --- | --- | --- |
| **Pascal casing** | First letter of every word upper case | `BackColor` | Class/variable names, method parameters |
| **Camel casing** | First word lower, others upper | `backColor` | Method names |
| **Underscore prefix** | `_` then camel case | `_backColor` | Internal use in class/methods |

*Slide exercise:* `int uglyduck(int timer_count)` is a **misleading function name and parameter name** – it is really `int factorial(int n)`. Similarly `process_str(INPUT_STR, FLAG)` → `transform_string(input_string, should_transform)`.

**3. Structuring.** Show dependencies between components via naming and layout (put related interface/implementation declarations close). File structure: logically grouped `#includes`, `#defines`, structures, functions; decide what goes in header vs C file and what is exposed; data structures well encapsulated and properly initialised.

**4. Ease of reading.** Whitespace is free; vertical space aids parsing; meaningful names reduce the need for comments; replace magic numbers with constants (e.g., `40` → `MAX_HEX_CHARS`); extract helper functions (`is_hex_digit`). Comments should be concise, clear, not cryptic or misleading; explain *why/logic*, not the obvious. (Thought question: a function-level comment is usually better than commenting every line; inline comments help for tricky logic.)

**Good programming style:**

| Do | Don't |
| --- | --- |
| Use few standard, agreed control constructs | Be too clever |
| Use GOTO in a *disciplined* manner | Write dangling if / dangling else |
| Use user-defined types to model domain entities | Nest too deeply |
| Hide data structures behind access functions | Use the same identifier for multiple purposes |
| Use appropriate names, indentation, parentheses, blank lines | Leave routines with **more than five formal parameters** unexamined |

**Dangling else:** in `if(ch>=3) if(ch<=10) k++; else o++;` the `else` binds to the **nearest (inner) if** regardless of indentation, so `o++` runs when `ch>=3` but `ch>10` – unlike what indentation suggests. **Fix: always use braces `{ }`.**

### C.4 Coding standards vs coding guidelines

|  | **Coding standards** | **Coding guidelines** |
| --- | --- | --- |
| Nature | **Mandatory rules** | **Generic suggestions** (recommended) |
| Purpose | Uniform appearance of code from different engineers; readability, maintainability, lower complexity; address common issues; enable reuse; promote sound practices | Improve understanding/readability; reduce cost via early error detection; ease maintenance |
| Examples | Rules for data usage (local/global); **standard module headers**; naming conventions; **error/exception handling conventions** | Document well; indent code; keep functions short; reduce GOTO; avoid reusing identifiers |

Common coding practices: **testable, secure and defensive programming**.

### C.5 Defensive programming

**Idea:** *Murphy's Law – if anything can go wrong, it will.* Code as if the user is actively trying to break your system. 'Defensive programming is like insurance – costs a little upfront, saves massive headaches later.'

**Slide case – `register_for_event()` that appends to `registrations.txt`.** Analysis categories and findings:

| Category | Problems found |
| --- | --- |
| **What-if failures** | **Race condition / TOCTOU** (time-of-check to time-of-use: two users take the last seat); file doesn't exist on first run; `strstr` **partial substring collision** ('Tech' matches 'TechFest'); **buffer truncation** on lines >99 chars; disk full / permission denied |
| **Missing checks** | No `file == NULL` check after `fopen`; no NULL/empty/length validation of arguments; **no delimiter injection sanitisation** (commas/newlines corrupt CSV); **no duplicate registration check**; ignored `fprintf`/`fclose` return values |
| **Resource management** | `fclose(NULL)` = undefined behaviour; file opened *before* knowing if event is full; **no file locking** (`flock`/`fcntl`); O(N) full-file scan per write |
| **Data integrity** | Interleaved concurrent writes mangle rows; attacker input like `eve,HackCon\nadmin` injects new records; commas inside names shift columns |

**Defensive techniques:** validate all inputs; check return values and NULL pointers; fail safely with clear errors; use assertions; avoid assumptions; limit resources; lock shared resources; use safe library functions.

### C.6 Secure coding

**Definition:** developing software to guard against *accidental introduction of security vulnerabilities* (defects, bugs, logic flaws).

**Practices (slide list):** **validate input** from all untrusted sources; **heed compiler warnings** (highest warning level, eliminate warnings); use **static and dynamic analysis** tools; **default deny** (access decided by permissions); **principle of least privilege** (run with the least set of privileges, elevated permissions for the least time); **sanitise data sent to other systems** (complex subsystems such as SQL, shell, browsers).

**Defensive vs Secure vs Testable:**

|  | Defensive | Secure | Testable |
| --- | --- | --- | --- |
| Goal | Survive unexpected conditions/misuse | Prevent exploitable vulnerabilities | Make bugs easy to find and fix |
| Typical techniques | Input checks, NULL checks, error handling | Least privilege, default deny, sanitisation | Assertions, test points, stubs, harness, instrumentation |

### C.7 Testable coding

Code that is easy to test and debug. Techniques:

- **Assertions** – detect out-of-range values.
- **Test points** – methods to set/retrieve module status and variable contents.
- **Scaffolding** – any code written *only to support* unit/integration testing (not part of the product).
- **Test harness** – code that *drives* objects/modules/components as part of the complete system.
- **Test stubs** – code returning **known, fixed values** to emulate functions not under test.
- **Instrumenting** – execution logging/messages to visualise execution.
- **Building test data sets** – valid *and* invalid data.

**MCQ Trap:** Stub = *called by* the unit under test (replaces a dependency); Driver/harness = *calls* the unit under test. Both are scaffolding.

### C.8 Managing construction – quality in Agile / sprint management

| Metric | Meaning |
| --- | --- |
| **Sprint burndown** | Chart of *work remaining (story points)* vs time in the sprint; slopes **downward**; shows progress toward the sprint goal in terms of *work left, not time spent* |
| **Team velocity** | Amount of software/stories **completed per sprint** (story points or hours); used for **estimation and planning** |
| **Throughput** | Total value-added work output – units of work completed in a period |
| **Cycle time** | Total time from **work started to completion** of an item |

**Velocity example:** velocity 20 points/sprint and 160-point backlog → **8 sprints**.

**\[Supplement\] Sprint management cycle:** product backlog → sprint planning (select stories, define sprint goal) → daily stand-up → development → sprint review (demo) → sprint retrospective. Sprint length typically 1–4 weeks; Scrum roles: Product Owner, Scrum Master, Development Team. **Lead time** (request → delivery) ≠ **cycle time** (work start → done).

**Construction technical-quality activities:** peer review (informal advice from other developers); unit testing; **test-first** (build test harness before code – clarifies requirements, prevents untestable design); **code stepping** (execute one statement at a time, watch variables); **pair programming** (one focuses on logic, other on syntax); debugging (locate a *known* defect); **code inspection** (formal peer review); static analysis.

### C.9 Code reviews and pull requests

**Definition:** constructive review of a teammate's code – often a *required sign-off* before check-in. Feedback → revision/refactoring → loop → approval.

**Why review (even though we test)?** catches bugs and design flaws early; builds teamwork and trust; **more than one person has seen every piece of code** (insurance against 'winning the lottery', i.e., author leaving); accountability; forces documentation and improvement (prospect of review raises quality); less experienced devs learn.

**Data (memorise):**

- Average defect detection: **unit testing 25%**, **integration testing 45%**, **design inspection 55%**, **code inspection 60%**.
- 11 programs by the same group: **no reviews 4.5 errors/100 LOC vs with reviews 0.82/100 LOC**.
- **AT&T** after introducing reviews: **14% productivity increase, 90% fewer defects**.

**Industry practice:** *Google* – every change reviewed by at least one other person, and author or reviewer must have 'readability' in the language (tool: Mondrian → Critique). *Yelp* – Review Board, inline comments, reviewer clicks 'Ship It!' then author merges. *Facebook* – tool notifies reviewers (and people who touched the changed code); all versions retained; merged for weekly push.

| Holistic review | Incremental review |
| --- | --- |
| Big picture: goal, architecture, design, whole component | **Each change reviewed before commit** |
| Reviewers focus on own expertise; meeting; author addresses *all* comments | Needs sign-off by a **committer** who knows the codebase |

**Review checklist:** Design, Functionality, Complexity, Tests, Naming, Comments, Style, Documentation.

**Slide code example:** `login_user` with hard-coded admin password `"123"`, plain-text password comparison, loading *all* users from DB, no hashing → **'Do Not Ship'** (security issue); also performance (full scan), maintainability.

**Constructive reviewing:** *criticise code, not the coder*; **explain the why**; suggest improvements; pick your battles (high-impact first); ask questions ('Could we simplify this?').

**Writing a good Pull Request (PR):** descriptive but concise title/summary; describe **context, rationale and alternatives considered**; link specs/issues/previous PRs; screenshots or recordings for UI changes; keep PRs small. Think like a reviewer.

### C.10 Understanding an existing codebase and contributing

- **You will never understand the entire system!** Goal: build a *working model* (components + connections) via observation, probes and hypothesis testing.
- **Steps:** clone repo → read **README.md** → understand folder/file structure → **build** it → **run** it → choose what to change → attach a **debugger**, view source, find the **logs** → search for constants (strings, colours, magic integers like `0xDEADBEEF`). Also: check CONTRIBUTING.md, run the test suite, start with a small 'good first issue'.
- **Novice vs expert comprehension:** novice reads line-by-line, revisits code, trial-and-error, tests only the *happy path*; expert works **top-down**, recognises patterns, forms hypotheses, checks upstream/downstream consequences.
- **Observations:** software is full of patterns and massively redundant (copy a starting point); code must run and so must exist *somewhere*.

| Where code lives | Probe/Understand/Edit |
| --- | --- |
| Source built locally (open source) | White-box: **P+U+E** |
| Binaries running locally | Grey-box: P (closed) / U |
| Server-side app running remotely | Black-box: observe outputs only |
| Client-side JS in browser | Source downloaded – inspect via **browser dev tools** |

- **Static information gathering:** IDE, grep/find, code navigation. **Dynamic information gathering:** build → run → change → run again; **probes**: print statements, structured logging, debuggers (breakpoints, step over/into, eval), Chrome DevTools.
- **Document findings:** update README/wiki, Mermaid diagrams, include negative results.
- **Redis case:** clone → README → `server.c` (main, command table) → `t_zset.c` (sorted sets: skiplist + dict) → `zaddCommand` → inspect memory allocation/NULL handling/cleanup → `tests/`, `make test`, **valgrind** for memory checking. *Always start at the failing command and trace through handler → type code → error management.*

### C.11 Software analysis (static analysis)

**Static analysis** = examining source code **without executing** it (a form of *white-box* analysis). Finds: programming errors, coding-standard violations, undefined values, syntax violations, security vulnerabilities. **Dynamic analysis** scans running applications (black-box-like).

**Can / cannot:** type-checking is well established; syntactic pattern checks are fast; **termination is undecidable (Halting problem)**; exact values are hard – conservative abstraction gives **'MAYBE'** answers (false positives). Good at: buffer overruns, unvalidated input, null dereference, uninitialised data, resource leaks.

**Good tool qualities:** fast; **few false positives** (else developers ignore warnings); continuous (in CI, diff-based); informative (suggest/apply fixes).

| Category | What it does | Example |
| --- | --- | --- |
| **Linters** | Cheap, shallow syntax checks for style (indentation, naming, line size) | ESLint, pylint |
| **Pattern-based bug detectors** | Simple syntax/API rules for common mistakes (bad practice, correctness, performance, security, dodgy code, multithreaded correctness) | FindBugs/SpotBugs |
| **Type-annotation validators** | Enforce user-defined types, e.g., `@NonNull`/`@Nullable` | Checker Framework, MyPy, TypeScript |
| **Data-flow / abstract interpretation** | Deep analysis, e.g., can array index go out of bounds? | Costly, finds tricky bugs |

**Mars Climate Orbiter ($327 M)** lost because of metric (newtons) vs imperial (pound-force) mismatch – the kind of error unit-typed annotations can catch.

**Limitations of type-based analysis:** looks only at signatures/annotations; poor with dynamically generated code (Spring, templates); false positives; needs annotated libraries.

| Static analysis | Dynamic analysis |
| --- | --- |
| Needs only source code | Needs successful **build + test inputs** |
| Reasons about **all** inputs/paths | Observes **individual executions** |
| May report **false positives** | Reported problems are **real** (witness input) |
| Can report all warnings of a class | Subject to **false negatives**; depends on test inputs |

**Best QA = combination of tools.**

### C.12 Bad vs good code – survey of the slide examples

| Example | Bad code problems | Good version | Real incident |
| --- | --- | --- | --- |
| `copy_string` (C) | No NULL check for dest/src, no `maxlen<=0` check, assumes buffer size, no error report | Validate pointers & `maxlen`, copy `maxlen-1`, null-terminate | **Heartbleed** (unchecked length), Apple **goto fail** |
| `format_name` (C) | `strcat` buffer overflow; **mutates input**; no return value | `const` inputs, size checks, `toupper`, `snprintf` to output buffer, return error code | OpenSSL API misuse; Microsoft GDI buffer corruption |
| `delete_file` (C) | `sprintf` + `system("rm -f ...")` → **command injection**, buffer overflow | Validate length/characters, use `unlink()` | **Shellshock**, CUPS injection |
| `safe_concat` (C, good) | – | Checks NULL & sizes, `snprintf`, return codes, no global state, testable | – |
| `safe_divide` (Python, good) | – | Type hints, `TypeError`/`ValueError` instead of silent failure, EAFP style | – |
| `normalize_scores` (Python, bad) | Mutates input list, **no check for sum == 0** (ZeroDivisionError), mixes logic with printing | Return new list; handle empty/zero sum; compute sum once | Shared-state mutation bugs in ML pipelines |

**Qualities of good code:** input validation, safe APIs, clear error signalling, no side effects, separation of concerns, testable, readable naming.

---

## PART D. CI/CD, DEVOPS, DEVSECOPS AND BUILD SYSTEMS

### D.1 Why automate? (the 'code is written – now what?' slide)

Steps after coding: get source → install dependencies → run static analysis → compile → generate documentation → run tests → create artefacts for customers → ship → operate/monitor/repeat. **Which should be manual? NONE** – orchestrate with tools (build system, test tools, code review/analysis tools, deployment). Tasks that cannot be automated are covered by **code review**.

### D.2 Build system

**Three roles of a build system:** (1) **define tasks** (and external resources such as libraries); (2) **define dependencies among tasks** (a graph); (3) **execute the tasks**.

**Good build system:** *dependency management* (identifies dependencies incl. external, compiles in the right order, **compiles only what is needed**); *efficiency and reliability* (all deliverables come together, removes 'tribal knowledge' and missed steps).

**Event-manager example:** tasks – install dependencies, set up DB, run static analysis, execute tests. Dependencies – DB setup before running app, dependencies before tests, static analysis can run **in parallel**. Execution – same environment every time, clear pass/fail, automatic retry on temporary failure.

**Key algorithm:** task dependencies form a **Directed Acyclic Graph (DAG)**; build tools use **topological sort** to order tasks – compute the **in-degree** (number of incoming edges) of each node; nodes with in-degree 0 go first; remove them and repeat. A *cycle* means a build cannot be ordered.

**Best practices for build:** automate everything feasible; **one-step build**; CI tools build and test on **every commit**; all dependencies captured in build files; **never break the build** – use git branching to keep `main` working.

**Tools:** Make (Makefiles), Apache Ant, **Maven** (project object model – POM), Gradle, CruiseControl, FinalBuilder.

### D.3 Continuous Integration (CI)

**Definition:** developers merge code into the mainline/master branch *frequently (multiple times a day)*; each merge triggers an automated build and tests.

**Pipeline stages:** code commit → **continuous build** (compile, resolve dependencies, run lint/static analysis, generate executable) → **sanity tests** (is the build executable, dependencies resolved?) → unit/integration tests → feedback.

**Benefits:** small change sets → small, incremental conflict resolution; early issue detection; avoids painful big-bang integration; more visibility; less debugging time. *If code is not merged frequently:* large merge conflicts, late bug discovery ('integration hell').

| Pros | Cons |
| --- | --- |
| Interaction problems between developers found and fixed early | Very large systems take long to build and test |
| Latest mainline is the **definitive working system** | If dev platform differs from target platform, system tests may not run in the developer's workspace |

### D.4 Continuous Delivery vs Continuous Deployment

|  | **Continuous Delivery** | **Continuous Deployment** |
| --- | --- | --- |
| Meaning | Code is **always ready** to release; frequent shipping to a given (test/staging) environment | Validated build is **automatically released to production** without manual approval |
| Trigger | Often last step of CI | After automated tests pass |
| Manual step | Usually a **manual approval** before production | **None** |
| Maturity | Common | Typically **highly mature DevOps teams** |

**Continuous Testing:** mostly automated tests executed with minimal waiting to give earliest feedback; entry criteria = unit and integration tests passed; may include static analysis and functional + non-functional validation in the QA environment.

### D.5 DevOps

**Definition:** combination of cultural philosophies, practices and tools that increases an organisation's ability to **deliver (deploy and support) applications at high velocity**. Blend of *Development + Operations*; shared/collaborative approach between the SDLC (dev) team and IT operations. Removes repetitive, error-prone manual work; an **extension of the Agile way of working** (deliver incrementally, faster). Related to the Agile principle of *delivering working software frequently*.

**Related concepts:** SDLC models; operations methodologies (**ITSM, ITIL**); version control (Git – commit, compare, merge, restore); **DevOps pipeline** = set of automated processes/tools that let developers and operations build and deploy code to production.

**Pipeline flow (draw this):**

`Plan → Code → Build → Test → Release → Deploy → Operate → Monitor → (feedback to Plan)`

**Four pillars (slide):** **Collaboration, Affinity, Tools, Scaling.**

| Pillar | Meaning |
| --- | --- |
| **Collaboration** | Working toward an outcome through supportive interactions; needs **theory of mind** (recognise others hold different perspectives), equal participation, communication; shared goals, empathy, trust, reciprocity, less hierarchy |
| **Affinity** | **Measure of strength of relationships** between individuals/teams/units; built via shared values, consistent team culture, cohesion; measured by shared time, intensity of relationships, reciprocity of shared stories and support |
| **Tools** | Accelerators that drive change; become the *common language* between teams; poor tools raise cost of collaboration (provisioning, build, automation, testing, monitoring, log extraction, deployment) |
| **Scaling** | Processes organisations adopt as they grow/mature/shrink – scaling organisation, infrastructure, teams (hiring, retention, outsourcing), complexity, workload |

**Agile vs DevOps:**

|  | Agile | DevOps |
| --- | --- | --- |
| Focus | Collaboration between **customers and developers**; iterative development | Collaboration between **development and operations** |
| Scope | Development/requirements side | Build, release, deploy, operate, monitor |
| Goal | Respond to changing requirements | Fast, reliable delivery and operation |
| Feedback | From customer | From monitoring and operations |

**Typical tools \[Supplement\]:** Git/GitHub (VCS), Jenkins/GitHub Actions/GitLab CI (CI/CD), Maven/Gradle (build), Docker (containers), Kubernetes (orchestration), Ansible/Terraform (infrastructure as code), Selenium/JUnit (testing), Prometheus/Grafana/Nagios (monitoring), SonarQube (static analysis).

### D.6 DevSecOps

**Definition:** Development + **Security** + Operations – an application-development practice that **automates the integration of security at every phase** of the SDLC, from design through integration, testing, delivery and deployment. Previously security was 'tacked on' at the end by a separate team.

- **Shift left:** move security testing earlier, toward developers.
- **Shared responsibility** of development, security and IT ops (not a silo).
- **Motto:** *'software, safer, sooner.'*

**Benefits:** find issues early; fix issues faster (automation + feedback loops); **reduce the window of attack opportunity**; increase ability to scale.

**Challenges:** diverse technologies/frameworks; mis-configured security tests make pipelines **brittle**; managing many triggered events/policies; coordinating security checks across a distributed pipeline.

| DevOps | DevSecOps |
| --- | --- |
| Dev + Ops | Dev + **Sec** + Ops |
| Security often late | Security **shifted left**, continuous |
| Speed and reliability | Speed, reliability **and** security |

**Typical DevSecOps checks \[Supplement\]:** SAST (static code scan), SCA/dependency scanning and **SBOM**, secret scanning, container image scanning, DAST (dynamic scan of running app), infrastructure-as-code scanning.

> **MCQ Traps (Part C & D):** (1) Static analysis does NOT execute code; its warnings may be false positives. (2) Code inspection detects 60% of defects vs 25% for unit tests. (3) Continuous *delivery* needs manual approval for production; continuous *deployment* does not. (4) Topological sort needs a DAG. (5) Sprint burndown shows *remaining work*, velocity shows *completed work*. (6) Standards are mandatory; guidelines are suggestions. (7) DevSecOps's key idea is *shift-left*.

---

## PART E. TESTING

### E.1 Why testing matters – Disney+ launch

Disney+ expected a traffic spike and tested massive concurrent *video streaming*. On launch day the stress fell on **other paths – account creation, login/authentication, browsing old titles**. Lesson: **they tested the wrong thing**; test the *whole* user journey and realistic usage profile.

### E.2 Seven principles of testing (high-yield, one MCQ each)

| # | Principle | Meaning / memory hook |
| --- | --- | --- |
| 1 | **Testing shows the presence, not absence, of defects** | 1000 passing tests don't prove bug-free. *Ariane 5* exploded despite tests. |
| 2 | **Exhaustive testing is impossible** | Even tiny inputs explode combinatorially (slide: \~**72 billion** test cases for a small ride-fare function). Use heuristics: equivalence classes, boundaries, risk. |
| 3 | **Start testing early** | Tests guide design; early feedback; bugs cheapest to fix and cause least damage early. |
| 4 | **Defects cluster** | **10–20% of modules hold \~80% of bugs** (Pareto). Focus on 'hot' components, tricky logic, frequently changing code. |
| 5 | **Pesticide paradox** | Running the same tests repeatedly stops finding new bugs – *vary and update tests*. |
| 6 | **Testing is context dependent** | Pacemaker ≠ food app: risk and consequences differ. |
| 7 | **Verification ≠ Validation** | Verification: *'Are we building the product right?'* (meets spec). Validation: *'Are we building the right product?'* (meets real user needs). |

**Case – Ariane 5 (1996):** self-destructed \~**37–39 s** after launch (loss ≈ $370–400 M). Cause: unhandled **64-bit float → 16-bit integer overflow** in inertial reference software *reused from Ariane 4* (Ariane 5 had higher horizontal velocity); the backup had the **same** fault, so redundancy of identical software failed (**software common-mode/single point of failure**); diagnostic data was misread as flight data. Violated: principle 1 (and 6), missing **boundary/robustness tests**, assumption validation on reuse.

### E.3 Test design techniques

- **Opportunistic/exploratory testing:** add tests with little planning.
- **Specification-based (black-box):** derive tests from the specification – avoids implementation bias, robust to implementation change, no code familiarity needed, tests can be written *before* code.
- Techniques: **equivalence partitioning, boundary value analysis, combinatorial/pairwise testing, random testing.**

**Equivalence partitioning:** divide inputs into classes that behave the same; test **one value per class** (valid and invalid classes). Needs domain knowledge.

**Boundary Value Analysis (BVA):** errors cluster at boundaries. For each variable test **min, min+1, nominal (medium), max−1, max** and often invalid **min−1, max+1**.

*Worked example:* age field valid 18–60 → partitions: <18 (invalid), 18–60 (valid), >60 (invalid). BVA values: **17, 18, 19, 39 (nominal), 59, 60, 61**. Standard BVA = 4 or 5 values per variable (**4n+1** for n variables, robust BVA **6n+1**).

**Pairwise (all-pairs) testing:** many failures arise from *interaction of parameters* (e.g., senior citizen + weekend). Covering all **pairs** of parameter values finds an estimated **50–90%** of defects with far fewer tests than all combinations.

### E.4 V-Model and test levels (preview)

Each development phase has a matching test phase: requirements ↔ **acceptance** test; system design ↔ **system** test; architecture/high-level design ↔ **integration** test; module design ↔ **unit** test; coding at the bottom of the V. Test planning starts as soon as the matching development phase starts.

### E.5 Test case writing

**Test case:** documents *what to test, how to check it, and expected results*. Benefits: lets anyone step into the tester role; ensures coverage; helps think through details from multiple angles; reusable; source of truth on how the feature works.

**Typical fields:** ID, title, description/objective, preconditions, test data/inputs, steps, **expected result**, actual result, status (pass/fail), priority, environment.

| Type | Verifies | Example (login) |
| --- | --- | --- |
| **Positive** | Software does what it *should* | Valid credentials → login allowed |
| **Negative** | Software does *not* do what it should not | Invalid credentials → login denied |
| **Destructive** | How much abuse the software handles *before it breaks* | Enormous input, killing the process mid-save |

**Good test cases:** short title; strong description; clear and concise; include expected result; reusable.

**Test scenario vs test case:** scenario = *what* to test (high-level, e.g., 'verify registration'); case = *how* (detailed steps, data, expected result).

**Test suite:** a container of tests that helps execute and report status. Three states: **Active, In-Progress, Completed**. A test case may belong to multiple suites/plans; suites are created by cycle or scope; can hold functional and non-functional tests.

**Software reliability:** probability of **failure-free operation for a specified period in a specified environment**.

### E.6 Inspection vs testing

|  | **Software inspection** | **Software testing** |
| --- | --- | --- |
| Type | **Static verification** – analysis of static representation | **Dynamic verification** – executing with test data |
| Execution | **Not required**, so possible before implementation | Requires executable system |
| Applies to | Any artefact: requirements, design, config data, test data, code | Executable code |
| Strength | Finds many errors; **errors don't mask each other**; incomplete versions can be inspected; also checks standards, portability, maintainability | Finds runtime behaviour, performance, emergent problems |
| Weakness | Cannot check non-functional runtime characteristics (performance) | Errors can **mask** other errors; incomplete systems need harnesses |

They are **complementary** – not opposed.

### E.7 Stages of testing

1. **Development testing** – by the development team (unit, component, system).
2. **Release testing** – separate team tests a complete version before release.
3. **User testing** – users test in their own environment.

### E.8 Development testing

**Unit testing:** testing individual components **in isolation**; a *defect-testing* process. Units: functions/methods; **object classes**; composite components with interfaces. **Object-class testing** = test all operations, set and read all attributes, exercise all **states**; inheritance makes it harder (information not localised). **Automate** unit tests with frameworks such as **JUnit** (extend generic test classes; run all tests; report pass/fail).

**Component (integration) testing:** unit-tested units combined into composite components; focus on **component interfaces** behaving per specification.

**Interface types:** *parameter* (data passed between methods); *shared memory*; *procedural* (sub-system exposes procedures); *message passing* (sub-system requests services).

**Interface errors:** **misuse** (wrong parameter order), **misunderstanding** (wrong assumptions about behaviour), **timing errors** (different speeds → stale data).

**Interface testing guidelines:** test parameters at **extreme ends of range**; always test pointer parameters with **NULL**; design tests that make the component **fail**; use **stress testing** in message-passing systems; in shared-memory systems **vary the order of component activation**.

**System testing (during development):** integrate components and test the whole; focus on **interactions**, compatibility, correct data at the right time; tests **emergent behaviour**; collective process (may involve a separate team).

**Regression testing:** re-run tests to check changes have not **'broken' previously working code**; expensive manually, cheap if automated; all tests re-run on every change; tests must pass *before commit*.

### E.9 Release testing

Testing a release meant for use **outside the development team**. Goal: **convince the supplier the system is good enough for use** – shows it delivers specified functionality, performance and dependability and does not fail in normal use. Usually **black-box** (from the specification only).

| System testing (development) | Release testing |
| --- | --- |
| Done by the **development team** | Done by a **separate team** not involved in development |
| Goal: discover bugs (**defect testing**) | Goal: check requirements are met and system is good enough for external use (**validation testing**) |

### E.10 User testing

Needed even after thorough system and release testing, because the **user's working environment** strongly affects reliability, performance, usability and robustness and cannot be fully replicated in test labs.

| Type | Where / who | Key point |
| --- | --- | --- |
| **Alpha testing** | Users work **with the development team at the developer's site** | Internal, controlled |
| **Beta testing** | Release to selected users **in their own environment** to experiment and report problems | External, uncontrolled; for products |
| **Acceptance testing** | **Customer** decides whether to accept and deploy | Mainly **custom systems**; final quality gate |

**Agile:** the customer is part of the team and defines acceptance tests, which run automatically; there is **no separate acceptance phase**. Problem: is the embedded user *typical* of all stakeholders?

### E.11 Performance testing and debugging

- **Performance bugs:** unexpectedly bad performance on some inputs; degradation over time; differences across versions/platforms. They usually do **not** produce wrong results or crashes, are hard to diagnose (load, hardware, network, workflows) and have a big UX impact. Challenge: what is the baseline/threshold? How to get reliable measurements?
- **Performance regression testing:** measure and log execution time of critical components and compare over time (trend analysis).
- **Profiling:** analyse and measure performance of a program or functions (where time/memory goes). **Tracing:** record the sequence of events (function calls) to understand execution flow. Both find bottlenecks.
- Performance test plan: increase load steadily until performance becomes unacceptable; tests should mirror the **usage profile**.
- **Load test** = expected/above-average load; **Stress test** = beyond maximum load to find the breaking point and failure behaviour; **Soak (endurance) test** = significant load over a **long period** to expose **memory leaks** and resilience issues (time-consuming; run at big milestones or infrastructure changes).

### E.12 Chaos engineering

**Definition:** *'the discipline of experimenting on a system in order to build confidence in the system's capability to withstand turbulent conditions in production.'*

- **Testing in production**: purposely take down components in a live deployment and observe whether **failovers** and fallbacks work. Prepares for disasters and cyberattacks.
- **Why break things on purpose?** In microservices, networks partition, instances die, calls are delayed, servers overload or run out of memory/CPU – find weaknesses before real outages.
- **Google examples:** shutting down the São Paulo network revealed a **hidden dependency** that took down links in Mexico; turning off a data centre showed machines wouldn't come back because they **ran out of DHCP leases**.
- **Netflix:** **Chaos Monkey** randomly kills AWS instances, breaks network connections or changes config files. Key health metric: **Stream Starts per Second (SPS)** – measures availability ('are we still good?').
- Steps: define steady state (metric) → hypothesise it continues → inject failure (small blast radius) → compare → fix weaknesses.

### E.13 Static and dynamic testing

|  | **Static testing** | **Dynamic testing** |
| --- | --- | --- |
| Slide association | **'Verification'** | **'Validation'** |
| Code executed? | **No** | **Yes** – give input, observe output, compare with expected |
| When | Early; defects easier to locate and fix | After build |
| Techniques | Document reviews, **walkthroughs, inspections**, feasibility analysis, code review, **static analysis** | Unit/integration/system tests, white/black-box techniques |
| Advantage | Early, cheap | Finds difficult, complex defects |
| Disadvantage | Misses runtime issues | Cost in **time and budget** |

**Static analysis metrics:** code quality, data flow, control flow, **cyclomatic complexity**.

**Control Flow Graph (CFG):** directed, process-oriented graph; **nodes = basic blocks**, **edges = control flow**; has an **Entry block** and an **Exit block**; shows all paths that may be traversed.

**Cyclomatic complexity (McCabe):** number of **linearly independent paths**.

- **M = E − N + 2P** (E = edges, N = nodes, P = connected components; P = 1 for one program).
- Shortcut: **M = number of decision (predicate) nodes + 1** (each if/while/for/case branch counts; compound conditions `&&`/`||` add extra).
- Also = number of regions of a planar CFG.

*Worked examples from the slides:*

1. Graph with 7 nodes and 7 edges: M = 7 − 7 + 2 = **2**.
2. Insertion-sort-like code: E = 16, N = 14 → M = 16 − 14 + 2 = **4**.
3. Power function (`if y<0`, `while power!=0`, `if y<0` again): E = 16, N = 14 → **4** (three decisions + 1).
4. Nested loops `while(i<n-1){ j=i+1; while(j<n){ if(A[i]<A[j]) swap } i++ }`: E = 9, N = 7 → M = 9 − 7 + 2 = **4** (two whiles + one if + 1).

**Interpretation \[Supplement\]:** M 1–10 simple/low risk; 11–20 moderate; 21–50 complex; >50 untestable. **Minimum number of test cases for basis-path coverage = M.**

**Disadvantages:** measures *control* complexity, not data complexity; nested structures are harder to understand than non-nested ones but score the same; may be misleading for simple comparisons/decision structures.

**Dynamic testing – code-based approaches:**

- *Control-flow criteria:* statement, branch and condition coverage (path testing). **Branch coverage** – every branch outcome executed; **statement coverage** – every statement executed at least once. (Details in Unit 4.)
- *Data-flow criteria:* select paths to explore the sequence of events on variables (define–use).

**Dynamic testing – fault-based approaches:**

| Technique | Idea |
| --- | --- |
| **Error guessing** | Uses tester's **experience/intuition** and history to target the most plausible faults |
| **Fault seeding (bebugging)** | Deliberately **inject faults** (without telling testers) to **measure how effective the testing process is**; detection rate of seeded faults estimates remaining real faults |
| **Mutation testing** | Create **mutants** by changing single statements via mutation operators; a good test suite should make tests **fail on (kill) every mutant**; measures **quality of the test cases**; mutation score = killed / total mutants |

**By approach used:** *specification-based* (functionality per requirements), *equivalence partitioning & BVA* (reduce test cases), *intuition-based* (experience), *usage-based* (how users actually use the product), *application-domain* (domain knowledge).

> **MCQ Traps (Part E):** (1) Alpha = at developer's site; Beta = at user's site. (2) Release testing is black-box, by an independent team. (3) Cyclomatic complexity = E − N + 2P, *not* E − N + P. (4) Mutation testing tests the *tests*, fault seeding evaluates the *testing process*. (5) Static = verification (no execution); dynamic = validation (execution). (6) Soak test = long duration; stress test = beyond capacity; load test = expected capacity. (7) Inspections can be done before code exists.

---

## PART F. SOFTWARE CONFIGURATION MANAGEMENT (SCM)

### F.1 Overview

**Motivating scenario (slide):** 2 AM, three developers (Alice – registration bug, Bob – payment, Charlie – UI) merge changes and the system crashes; nobody knows whose change broke what or which version worked. Symptoms SCM prevents: accidental overwrites, merge conflicts, 'works on my machine', `final_final_v3_REAL.py`.

**Definition:** SCM is a software-engineering discipline of standard processes and techniques to **manage the changes** introduced into software products – identifying elements and configurations, tracking changes, **version selection, control and baselining**. It systematically organises, manages and controls changes in documents, code and other entities, and provides a framework for **maintenance and support**.

**Goals:** increase productivity through planned coordination and fewer mistakes; avoid configuration problems; manage **simultaneous updates** of source files; build management; defect tracking.

**Why needed:** many people work concurrently on the same software; multiple versions; released systems; changing requirements/budget; custom-configured systems; different machines; stakeholder coordination; controlling cost of change.

**Four basic SCM elements:**

| Element | Purpose |
| --- | --- |
| **Configuration item identification** | Define baseline components |
| **Configuration control** | Mechanism to *initiate, prepare, evaluate, approve or disapprove* change proposals |
| **Configuration auditing** | Determine how well the current state reflects its **baselines** (requirements/planning documents) |
| **Configuration status accounting** | Record of how the system has evolved and its state relative to published documents |

**Benefits:** orderly development and release; **only approved changes** implemented; changes follow approved specifications; documentation stays accurate; impact of changes evaluated and communicated; **prevents unauthorised changes**.

**Google monorepo case:** \~**2 billion lines** of code, **40,000+ commits/day**, automated testing on every commit, instant rollback, complete audit trail – SCM at scale.

### F.2 SCM roles

| Role | Responsibility |
| --- | --- |
| **Configuration Manager** | Identifies configuration items; defines procedures for creating, promotions, releases |
| **Developer** | Creates versions triggered by change requests; checks in changes; resolves conflicts |
| **Auditor** | Validates promotion/release selection process; ensures consistency and completeness |
| **Change Control Board (CCB) member** | **Approves or rejects** change requests |

### F.3 SCM planning

Done by the **Configuration Manager**, starting in the **early phases**; result is the **SCM Plan (SCMP)**, which may follow a public or internal standard and: defines **configuration items and a naming scheme**; assigns responsibility for CM procedures and baseline creation; defines **change-control and version-management policies**; describes tools and limitations; defines the **configuration management database (CMDB)**.

### F.4 Configuration items (CIs)

**Definition:** independent or aggregated hardware/software/both, designated for configuration management and treated as a **single entity**. Examples: all source/code files and test drivers; requirements, analysis, design and test documents (non-temporary); user and developer manuals; system configurations.

**Challenges:** which items need control (some must be kept for the product lifetime); *when* to place entities under control – **too early = bureaucracy, too late = chaos**.

### F.5 Baselining

**Baseline:** a specification or product that has been **formally reviewed and agreed**, serves as a basis for further development and **can be changed only through formal change control**. Baselines are created after a review. Examples: *Baseline A* – APIs defined, method bodies empty; *Baseline B* – all data-access methods implemented and tested.

**\[Supplement\]** Typical baselines: requirements baseline, design baseline, **product baseline** (tested code), release baseline. A baseline is a **snapshot** you can always return to.

### F.6 SCM activities

CI identification · configuration directories · baselining · **branch management** · **version management** · **build management** · **install management** · promotion management · change management · release management · defect management.

**Codeline:** group of files that evolve together over the life of an application/component (source, build scripts, images, compiled code). **Branch:** copy/clone of all or part of the source. **Merge:** integrating changes from branches back together.

**Reasons for branching:** concurrent development; capturing solution configurations; supporting multiple versions; isolated experimentation; keeps the main product stable. **Frequent merging reduces merge-conflict likelihood/complexity.** Branching strategies: single branch; by customer/organisation; by developer/workspace; by module/component.

|  | **GitFlow** | **Trunk-Based Development** |
| --- | --- | --- |
| Best for | **Large products** (\~50 devs), strict release cycles, multiple versions in production | **Small teams** (\~4 people), rapid iteration, weekly releases |
| Branches | `main` (production-ready), `develop` (integration), `feature/*`, `release/*`, `hotfix/*` | Single `main`, **short-lived** feature branches |
| Integration | Less frequent | **Very frequent**; **feature flags** hide incomplete work |

**Version management:** tracks versions of components/systems (using Git etc.). **Semantic versioning MAJOR.MINOR.PATCH** (e.g., 7.5.4): **major** = incompatible/big change, **minor** = backward-compatible feature, **patch** = bug fix. Slide example: v1.0.0 initial; v1.0.1 bug fix for registration; v1.1.0 payment feature; **v2.0.0 complete UI redesign**.

**Version control features:** changes attributable/traceable; history recorded and revertible; better conflict resolution; easier maintenance and quality monitoring; less regression; better organisation and communication.

**Version vs Revision vs Release:**

| Term | Meaning |
| --- | --- |
| **Version** | Initial release or re-release of a CI with a complete (re)compilation; **different versions have different functionality** |
| **Revision** | Change to a version that **only corrects errors**, not affecting documented functionality |
| **Release** | **Formal distribution of an approved version** |

**Build management:** creating the application by **compiling and linking** source and libraries into binaries/executables (Make, Ant, Maven); correct order; no recompilation if no change. **Build process:** fetch code from repository → compile and check dependencies → link → run tests and build artefacts → archive logs, send notifications → may change the version number.

**Install management:** installation is the **customer's first interaction** – failure creates a negative impression. Involves placing executables, libraries, config files; OS interaction for resources, permissions, versions, **licence enforcement**; localisation; may be automated (zip, shell scripts, InstallAware, Jenkins). **Installers vs bootstrapper:** a *bootstrapper* is a small installer that installs prerequisites and updates the big bundle. Tools: DeployMaster, InstallShield (de facto for MSI), InstallAware, WiseInstaller. **Install testing** checks all components are installed/removed correctly.

**Promotion management:** moving a change from the programmer's private environment to the **central master directory** according to **promotion policies** (baselining criteria, verification, authorisation). *(Slide question: can code with warnings but no errors be promoted? Depends on policy – strict policies forbid it.)*

**Controlling changes:** at two points – **promotion policy** and **release policy**. *Informal* policy suits research environments; *formal* policy suits externally developed items and releases. Enforced via tools, audited.

**Release management:** managing, planning, scheduling and controlling a build through stages/environments, testing and deploying releases. **Release policy** = gating quality criteria with metrics; when met, release is authorised. **\[Supplement\]** Release types: major, minor, patch/hotfix; strategies: blue-green, canary, rolling; release notes; rollback plan.

### F.7 Change control, change management and impact analysis

**Change management:** ensures evolution is a *managed* process and that priority goes to the **most urgent and cost-effective** changes; analyses cost/benefit, approves worthwhile changes and tracks which components changed.

**Change-management process (Sommerville):** change request form (CRF) → **change analysis** (affected components) → CCB decision (accept/reject) → implement → QA → configuration management → release.

CRF fields (slide example): project, number, requester, date, **requested change**, analyser, **components affected, associated components**, assessment, **priority** (e.g., Medium), implementation estimate (e.g., 2 hours), **CCB decision** (accept, target release 1.2), implementor, QA decision.

**Factors in change analysis:** consequences of **not** making the change; benefits; **number of users affected**; **cost**; the **product release cycle**.

**Agile:** customers participate in change management (propose change, assess impact, decide priority vs planned features); **refactoring** is a necessary part of development, not overhead.

**\[Supplement\] Impact analysis:** identifying the **potential consequences of a change** – which components, documents, tests and interfaces are affected, and the **effort, cost, risk and schedule** involved – *before* approving it. Methods: **traceability matrices** (requirements ↔ design ↔ code ↔ tests), dependency graphs, code-search/static-analysis tools, expert judgement. Output: list of affected CIs, estimate, risk, regression-test scope. **Ripple effect** = a change in one module forcing changes elsewhere. **Traceability** links artefacts to make impact analysis possible.

| Change control | Impact analysis |
| --- | --- |
| *Process* of approving/rejecting and tracking changes | *Activity* of assessing what a change will affect |
| Done by CCB | Done by analysts/developers *for* the CCB |

### F.8 Defect (bug) management

- **Bug:** consequence of a **coding fault**. **Defect:** variation/deviation from expected business requirements. (Related chain: error → fault → failure.)
- **Defect lifecycle (slide):** **Discover → Report → Validate (reproduce) → Analyse (severity: Critical, High, Medium, Low) → Assign → Fix → Verify → Close → Release.**
- **\[Supplement\] Typical states:** New → Assigned → Open → Fixed → Retest → Verified → Closed; side states Rejected, Duplicate, Deferred, Reopened.
- **Severity vs priority:** severity = technical impact (crash = high); priority = urgency to fix (business). A typo in the company logo can be low severity, high priority.
- **Tools:** Bugzilla, Jira, FogBugz, Trac, Backlog – provide a central overview of bugs and improvements across their lifecycle.

### F.9 \[Supplement\] Patching and patch management

- **Patch:** a small update that fixes bugs/vulnerabilities (or adds minor improvements) without a full new release. **Hotfix** = urgent patch for a production issue; **service pack** = bundle of patches; **security patch** fixes a vulnerability.
- **Patch management:** the process of **identifying, acquiring, testing, deploying and verifying** patches. Steps: inventory assets → monitor vendor advisories/CVE feeds → assess risk and prioritise (critical first) → **test in staging** → schedule deployment (maintenance window) → deploy (automated tools) → verify → **keep rollback plan** → document.
- **Why important:** unpatched known vulnerabilities are a leading attack vector (e.g., Equifax breach due to an unpatched Apache Struts flaw; Log4Shell). **Risk:** patches can introduce regressions – hence testing and rollback.
- Patch versions use the **PATCH** number in MAJOR.MINOR.PATCH.

### F.10 SCM tools

| Category | Purpose | Examples |
| --- | --- | --- |
| **Source code administration** | Version/source control | **RCS** (very old, file-locking), **CVS** (based on RCS, concurrent working *without locking*, web front end), **ClearCase** (multiple servers, process modelling, policy checks), **Git/GitHub**, SVN |
| **Software build** | Convert source to artefacts | **Make** (Makefiles), Cruise Control (open-source continuous builds), FinalBuilder, **Maven** (project object model), Ant |
| **Software installation** | Packaging and installers | DeployMaster, InstallShield, InstallAware, WiseInstaller |
| **Bug tracking** | Track defects | Bugzilla, FogBugz, Trac, Backlog, Jira |

### F.11 SCM in Scrum/Agile

SCM is the responsibility of the **whole team** and automated as far as possible. Definitive versions live in a **shared repository**; developers copy to workspaces, change, build locally; after testing, push to the repository.

**Typical Agile SCM flow:** pull latest code → create **feature branch** → change locally → run tests locally → commit and push → **CI pipeline** runs automatically → **code review via pull request** → merge to main after approval → automatic deployment to **staging**.

### F.12 Software Bill of Materials (SBOM)

**Definition:** a detailed **inventory of all software components, libraries and modules** (open-source and proprietary) in a product – the **'parts list'** for software (like a car manufacturer tracking every part).

**Typical contents:** package **name, version, origin (supplier), checksum/hash, licence, relationship to other components** (dependency tree), package URL (purl).

**Why it matters:**

- **Security:** when a vulnerability like **Log4Shell** appears, you quickly learn whether you ship the vulnerable component; reveals **transitive dependencies**; enables proactive patch management.
- **Compliance/licensing:** track licences (GPL, MIT, Apache) to avoid incompatible-licence legal risk.
- **Traceability/audit:** which version of `libxyz`? when introduced? Governments (e.g., US) increasingly **mandate SBOMs**.

| Standard | Owner / note |
| --- | --- |
| **SPDX** (Software Package Data Exchange) | **Linux Foundation**; open standard, widely supported |
| **CycloneDX** | **OWASP**; lightweight, security-focused |
| **SWID** tags | **ISO** standard; enterprise asset management |

**Tools:** **Syft** (generate SBOMs, e.g., `syft myapp:latest -o cyclonedx-json`), **Trivy** (container vulnerability scan + SBOM), **OWASP Dependency-Check**, GitHub Actions/native SBOM export.

CycloneDX entry example: `bomFormat: CycloneDX`, component `log4j-core` version `2.14.1` (a Log4Shell-vulnerable version), purl `pkg:maven/org.apache.logging.log4j/log4j-core@2.14.1`, licence `Apache-2.0`.

> **MCQ Traps (Part F):** (1) Baseline changes only via formal change control. (2) Revision ≠ version ≠ release. (3) CVS = concurrent working *without locking*; RCS = locking. (4) GitFlow = large/strict; trunk-based = small/fast with feature flags. (5) CCB approves/rejects; Configuration Manager plans. (6) SBOM = inventory of components (NOT a build artefact list of tests). (7) SPDX → Linux Foundation; CycloneDX → OWASP. (8) Defect lifecycle ends with *release* of the fix after close.

---

## PART G. UNIT 3 – KEY FACTS, CONFUSED CONCEPTS, TRAPS AND PRACTICE

### G.1 Key numbers and keywords to memorise

| Fact | Value |
| --- | --- |
| Implementation share of effort | **40–50%**; **80%** of time reading code |
| Defect detection | Unit **25%**, integration **45%**, design inspection **55%**, code inspection **60%** |
| Reviews study | **4.5 → 0.82** errors/100 LOC; AT&T **+14% productivity, −90% defects** |
| AI code security | **40–45%** flawed; XSS **86%**, log injection **88%**, SQL injection **20%**, crypto **14%**; Copilot study **1,689** programs, **40%** flawed |
| Defect clustering | **10–20%** modules → **80%** bugs |
| Pairwise testing | finds **50–90%** defects |
| Cyclomatic complexity | **E − N + 2P** |
| Ariane 5 | 64-bit float → 16-bit int overflow; \~37–39 s; backup had the same bug |
| Mars Climate Orbiter | **$327 M**, newtons vs pound-force |
| Google monorepo | **2 B lines, 40,000+ commits/day** |
| Defect lifecycle | Discover, Report, Validate, Analyse, Assign, Fix, Verify, Close, Release |
| DevOps pillars | **Collaboration, Affinity, Tools, Scaling** |
| Chaos metric (Netflix) | **Stream Starts per Second (SPS)** |
| SBOM standards | **SPDX, CycloneDX, SWID** |

### G.2 Frequently confused pairs

| Pair | Distinction |
| --- | --- |
| Verification vs Validation | Right product *built right* (spec) vs *right product* (user needs). Static ≈ verification; dynamic ≈ validation |
| Static vs dynamic analysis | No execution, may have false positives vs execution on inputs, may have false negatives |
| Standards vs guidelines | Mandatory vs advisory |
| Defensive vs secure coding | Survive anything unexpected vs prevent vulnerabilities |
| Stub vs driver/harness | Replaces a *called* module vs *calls* the module under test |
| Unit vs component vs system testing | Single unit in isolation vs interfaces of composite component vs interactions of whole integrated system |
| System vs release testing | Dev team, defect testing vs independent team, validation testing |
| Alpha vs beta vs acceptance | Developer site vs user site vs customer sign-off |
| Load vs stress vs soak | Expected load vs beyond max vs long duration |
| Inspection vs testing | Static review of artefacts vs executing software |
| Continuous delivery vs deployment | Manual release gate vs fully automatic to production |
| DevOps vs DevSecOps | Dev+Ops vs Dev+Sec+Ops (shift-left security) |
| Agile vs DevOps | Customer–developer collaboration vs dev–ops collaboration |
| Version vs revision vs release | New functionality vs error-only change vs formal distribution |
| Baseline vs branch | Frozen reviewed reference vs parallel copy for development |
| Bug vs defect vs failure | Coding fault vs deviation from requirement vs observed incorrect behaviour |
| Fault seeding vs mutation testing | Evaluates testing *process/testers* vs evaluates *test-case quality* |
| Linter vs deep static analyser | Shallow style check vs data-flow/abstract interpretation |
| AI4SE vs SE4AI | AI as tool vs AI as component |
| DevOps vs MLOps | General continuous delivery vs ML pipeline automation (subset) |
| Velocity vs throughput vs cycle time | Points per sprint vs units of work per period vs start-to-finish time per item |

### G.3 Potential MCQ traps (consolidated)

1. 'Passing all tests proves correctness' – **false** (presence, not absence).
2. 'AI-generated code can be merged if it compiles' – **false**; human review is non-negotiable.
3. 'ML systems are just trained models' – **false**; model is one component.
4. 'Static analysis finds only real bugs' – **false**; false positives are common.
5. 'Dangling else binds to the outer if' – **false**; nearest if.
6. 'Cyclomatic complexity measures data complexity' – **false**; control complexity only.
7. 'Beta testing is at developer site' – **false**; alpha is.
8. 'Continuous delivery means auto-deploy to production' – **false**; that is continuous *deployment*.
9. 'Revision adds new functionality' – **false**; only corrects errors.
10. 'Integration is hardest in the end – big-bang is best' – **false**; frequent/incremental integration.
11. 'DevSecOps adds security after testing' – **false**; shift-left.
12. 'Chaos engineering is done only in test labs' – **false**; testing **in production**.
13. 'The GPL-like clause: baseline can be modified freely' – baselines change only by formal review.

### G.4 UNIT 3 PRACTICE MCQs (with answers and explanations)

**Q1.** A startup wants to check that a Java project contains no type errors. Using the *LLM decision framework*, the best choice is: A) Ask an LLM to read the code  B) Use the Java compiler/type checker  C) Use vibe coding  D) Ask three LLMs and vote **Answer: B.** A deterministic alternative exists that gives better results than an LLM (first question: alternative solutions).

**Q2.** Which scenario is an example of **SE for AI (SE4AI)** rather than AI for SE? A) Copilot autocompleting a function  B) Using an LLM to generate unit tests  C) Deciding whether an object-detection model runs on a phone or in the cloud  D) AI summarising bug reports **Answer: C.** SE4AI = engineering systems that *contain* AI components; the rest use AI as a tool.

**Q3.** An AI tool generates `char name[50]; strcpy(name, input);`. The main risk is: A) Memory leak  B) Buffer overflow  C) Deadlock  D) Integer underflow **Answer: B.** No length check on `input`; use bounded copy with validation.

**Q4.** In a smart toaster, a hardware thermal fuse that cuts power if the ML-predicted time fails is an example of: A) Model retraining  B) Guardrail / safety mechanism outside the model  C) Human in the loop  D) Feature engineering **Answer: B.** Safety must not rely solely on the model.

**Q5.** The Ariane 5 crash, where all tests passed in earlier use, best illustrates which testing principle? A) Pesticide paradox  B) Defect clustering  C) Testing shows presence, not absence, of defects  D) Early testing **Answer: C.** Also shows unvalidated reuse assumptions.

**Q6.** A control flow graph has 12 edges, 10 nodes and one connected component. Cyclomatic complexity is: A) 2  B) 3  C) 4  D) 5 **Answer: C.** 12 − 10 + 2 = 4.

**Q7.** A function has one `while` loop and three independent simple `if` statements (no compound conditions). Minimum number of test cases for basis-path coverage? A) 4  B) 5  C) 3  D) 7 **Answer: B.** Decisions (4) + 1 = 5.

**Q8.** An input field accepts ages 18–60. Which set is the **standard boundary value** set (min−1 to max+1)? A) 0, 18, 40, 60, 100  B) 17, 18, 19, 59, 60, 61  C) 18, 30, 45, 60  D) 1, 17, 61, 99 **Answer: B.** Values at and around both boundaries.

**Q9.** According to the slides, which technique has the highest average defect detection rate? A) Unit testing  B) Integration testing  C) Design inspection  D) Code inspection **Answer: D.** Code inspection ≈ 60% > design 55% > integration 45% > unit 25%.

**Q10.** In which pipeline does every change that passes automated tests go to production with **no manual approval**? A) Continuous integration  B) Continuous delivery  C) Continuous deployment  D) Continuous testing **Answer: C.**

**Q11.** A build tool orders tasks by repeatedly picking tasks with **in-degree 0**. This algorithm is: A) Dijkstra  B) Topological sort on a DAG  C) Depth-limited search  D) Kruskal **Answer: B.** It requires a cycle-free dependency graph.

**Q12.** A company releases a pre-release version to selected external customers who use it in their own environments and report problems. This is: A) Alpha testing  B) Beta testing  C) Regression testing  D) Release testing by the dev team **Answer: B.** Alpha = at developer site with developers.

**Q13.** Netflix's Chaos Monkey experiment judges system health mainly by: A) Lines of code  B) Stream Starts per Second (SPS)  C) Cyclomatic complexity  D) Number of commits **Answer: B.** SPS measures availability.

**Q14.** Mutation testing primarily evaluates: A) The developers' productivity  B) The quality of the test suite  C) The server capacity  D) Code readability **Answer: B.** A good suite should detect (kill) mutants.

**Q15.** A regression suite has passed untouched for a year, yet users keep finding new bugs. Which principle explains this? A) Exhaustive testing is impossible  B) Pesticide paradox  C) Defect clustering  D) Verification ≠ validation **Answer: B.** Repeating the same tests loses effectiveness – update and vary them.

**Q16.** Which statement about a **baseline** is correct? A) It can be edited by any developer  B) It is a formally reviewed reference that changes only via formal change control  C) It is a temporary branch  D) It is the latest nightly build **Answer: B.**

**Q17.** Current release is 1.4.2. A backward-compatible new feature is added. Next semantic version? A) 1.4.3  B) 1.5.0  C) 2.0.0  D) 1.4.2.1 **Answer: B.** New feature → minor increases, patch resets. (Bug fix would be 1.4.3; breaking change 2.0.0.)

**Q18.** Which is **not** a typical SBOM field? A) Component version  B) Licence  C) Checksum  D) Unit-test pass percentage **Answer: D.**

**Q19.** In `if(a>=3) if(a<=10) k++; else o++;` with `a = 20`, what executes? A) `k++`  B) `o++`  C) Neither  D) Compile error **Answer: B.** The `else` pairs with the inner `if`; `a>=3` true, `a<=10` false → `o++`.

**Q20.** Backlog is 120 story points and team velocity is 30 points/sprint. Expected sprints? A) 3  B) 4  C) 5  D) 6 **Answer: B.**

**Q21.** In the defect lifecycle, which step directly follows *Report*? A) Fix  B) Validate (reproduce/confirm)  C) Close  D) Release **Answer: B.** Then Analyse severity, Assign, Fix, Verify, Close, Release.

**Q22.** Which is a difference between release testing and development system testing? A) Release testing is white-box and done by developers  B) Release testing is done by an independent team and is validation-oriented black-box testing  C) They are identical  D) Release testing finds unit-level bugs **Answer: B.**

**Q23.** A web server slowly consumes memory and crashes after 3 days of normal load. The best test to find this is: A) Smoke test  B) Soak (endurance) test  C) Boundary value test  D) Usability test **Answer: B.**

**Q24.** A pipeline runs SAST and dependency scans on every commit so developers fix security issues early. This practice is: A) Waterfall review  B) Shift-left DevSecOps  C) Big-bang integration  D) Chaos engineering **Answer: B.**

**Q25.** To unit-test `registerStudent()` before the email service exists, you write a small class that always returns 'sent'. This is a: A) Driver  B) Stub  C) Mutant  D) Baseline **Answer: B.**

**Q26.** Vibe coding is **most** acceptable for: A) A pacemaker firmware  B) A hackathon prototype/throw-away script  C) A banking payment gateway  D) Flight-control software **Answer: B.** Low-risk prototypes only; safety/security-critical software needs review and tests.

**Q27.** A static analyser flags a possible null dereference that is actually guarded by logic it cannot track. This is a: A) False negative  B) False positive  C) True positive  D) Mutation **Answer: B.** Static analysis is conservative, so false positives occur.

**Q28.** Which review comment follows constructive-review principles? A) 'You write terrible code.'  B) 'This function is complex; could we split it into two? It will ease testing.'  C) 'Rewrite everything.'  D) 'Looks fine' without reading **Answer: B.** Criticise code not coder, explain the why, suggest improvement.

**Q29.** A team of 4 releases weekly with short-lived branches and feature flags. The suited strategy is: A) GitFlow  B) Trunk-based development  C) Branch per customer  D) No version control **Answer: B.**

**Q30.** A change request is approved or rejected by the: A) Auditor  B) Configuration manager  C) Change Control Board  D) End user **Answer: C.**

---

# UNIT 4: USABILITY, QUALITY AND SYSTEM TESTING

## PART H. MEASUREMENTS, METRICS AND RISK MANAGEMENT FOR QUALITY

### H.1 Why quality matters – Ariane 5 (again)

4 June 1996, inaugural flight lost (payload ≈ **$400 M**). Primary and backup **inertial reference systems were reused from Ariane 4**; Ariane 5 had higher horizontal velocity → **64-bit float to 16-bit integer overflow** in the backup, then the same overflow in the primary → both failed → loss of control. **Software was a single point of failure; redundancy of identical software does not protect against a design fault.**

### H.2 ISO 9126 quality model (six characteristics)

| Characteristic | Question | Event-app example |
| --- | --- | --- |
| **Functionality** | Does it work correctly? | Registration saves to DB correctly |
| **Reliability** | Does it work consistently? | No crash during peak registration |
| **Usability** | Is it easy to use? | Register in < 3 clicks |
| **Efficiency** | Uses resources well? | Handles 1000 concurrent users without slowdown |
| **Maintainability** | Can we fix/enhance easily? | New event type added in < 2 hours |
| **Portability** | Can it run elsewhere? | Works on iOS, Android, web |

**Memory aid: FRUEMP** (Functionality, Reliability, Usability, Efficiency, Maintainability, Portability). **\[Supplement\]** ISO 9126 sub-characteristics: functionality – suitability, accuracy, interoperability, security, compliance; reliability – maturity, fault tolerance, recoverability; usability – understandability, learnability, operability, attractiveness; efficiency – time behaviour, resource utilisation; maintainability – analysability, changeability, stability, testability; portability – adaptability, installability, co-existence, replaceability. ISO 9126 was superseded by **ISO/IEC 25010** (adds *security* and *compatibility* as top-level characteristics).

**Usability (syllabus title):** ease of learning, efficiency of use, memorability, error rate/recovery, satisfaction (Nielsen). Measured via **usability testing** (task success rate, time on task, errors, SUS score), heuristic evaluation, user surveys.

### H.3 What is measurement?

- **Measurement** = *empirical, objective assignment of numbers, according to a rule derived from a model or theory, to attributes of objects/events with the intent of describing them* (Craner & Bond). Hubbard: *a quantitatively expressed **reduction of uncertainty** based on observations.*
- **Metric (IEEE 1061):** *a function whose inputs are software data and whose output is a **single numerical value** interpretable as the degree to which software possesses a given attribute that affects its quality.*
- Before SE (1960s–70s): no predictability. After SE: 'based on velocity 20 story points/sprint → 8 weeks'; '96% test coverage, <1 critical bug per 10k LOC'. **Measurement turned software from craft into engineering.**
- Quotes: Kelvin – *'To measure is to know; if you cannot measure it, you cannot improve it.'* Galileo – *'Measure what is measurable, and make measurable what is not so.'*
- **Everything is measurable** (Hubbard): if X matters it must be detectable; if detectable, in some amount; if in some amount, measurable.

**Entities and qualities:**

| Entity | Qualities to measure (examples) |
| --- | --- |
| **Product/modules** | Functionality, performance, security, portability, scalability, bugginess, availability, documentation, regulatory compliance |
| **Process** | Development efficiency, conformance, reliability of predictions, on-time release, fairness in decisions |
| **People** | Developers: maintainability of work, well-being, communication, flow. Customers: satisfaction, ease of use, feature usage |

Non-trivial qualities: code elegance, maintainability, fairness, collaboration, creativity – measured through **proxies/indirect indicators**.

### H.4 Code complexity measures

- **Lines of Code (LOC/SLOC):** easy but crude. *Normalise:* ignore comments/blank lines, ignore lines <2 characters, pretty-print first, count **logical** statements; normalise by language.
- **Halstead metrics:** from counts of operators/operands. Program **volume V = N × log₂(n)** where N = total operators + operands, n = distinct operators + operands. \[Supplement\] Also difficulty, effort.
- **Cyclomatic complexity:** M = E − N + 2P (see Part E).
- **Object-oriented metrics (Chidamber–Kemerer style):** number of methods per class (WMC), **Depth of Inheritance Tree (DIT)**, **Number of Children (NOC)**, **Coupling Between Objects (CBO)**, calls to methods in unrelated classes. High coupling and deep inheritance → harder maintenance.

**Using metrics for decisions:** fund the project? more testing? fast/secure enough? code quality sufficient? which feature next? developer bonus? time and cost estimation? **Trend analysis**, **benchmarking**, monitoring many modules to find typical values and **report deviations**.

### H.5 Test measurements (software test metrics)

**Software test metric:** quantitative measure of the testing process showing progress, quality, productivity and the degree to which the system possesses an attribute. **Goal:** improve efficiency/effectiveness of testing; make better decisions from reliable data. Used in SQA, project planning and monitoring, evaluating product quality and **defect analysis**. Measures represent **quality** and **test coverage**.

#### Function Points (FP)

Measure **size from the functional (user) view**: count inputs, outputs, queries, internal files, external files and adjust for complexity. Originated at **IBM**. Advantage: measures functionality *delivered to the end user*, independent of language.

| Component | Abbrev | Counts | Example |
| --- | --- | --- | --- |
| External Inputs | **EI** | Data-input screens | Registration form |
| External Outputs | **EO** | Reports, displays | Event calendar view |
| External Inquiries | **EQ** | User queries | Search events |
| Internal Logical Files | **ILF** | Database tables | Events, Users tables |
| External Interface Files | **EIF** | Shared external data | Google Calendar API |

**Slide calculation:** EI 5×7 = 35; EO 3×5 = 15; EQ 2×3 = 6; ILF 4×4 = 16; EIF 1×3 = 3 → **Total 75 FP**. With productivity ≈ **10 FP/person-month** → ≈ **7–8 person-months** (75/10 = 7.5). *General formula:* FP = Σ(count × weight) (unadjusted); adjusted FP = UFP × VAF (value adjustment factor, 0.65 + 0.01×Σ of 14 general system characteristics).

#### Product-related test measures

| Metric | Meaning |
| --- | --- |
| **SLOC** | Size in lines of code |
| **Fault density** | Faults found ÷ size (e.g., faults/KLOC) |
| **MTBF** | Mean Time Between Failures – statistical indicator of failure probability |
| **Failure rate** | **Inverse of MTBF** (λ = 1/MTBF) |
| **Defect distribution** | % of defects attributed to a given SDLC phase |
| **Defect density of modules** | Faults in module ÷ total faults in product |
| **Defect leakage** (slide formula) | **\[(defects found in UAT) ÷ (defects found before UAT)\] × 100** |

**\[Supplement\] more metrics:** **MTTR** (mean time to repair); **Availability = MTBF / (MTBF + MTTR)**; **MTTF** (non-repairable); **Defect Removal Efficiency (DRE) = defects found before release ÷ (before + after release)**; test case pass %; requirements coverage; defect age; defects open/closed trend; test execution progress.

*Example:* 40 defects found before UAT, 10 in UAT → slide leakage = 10/40 × 100 = **25%**. (Many textbooks define leakage = UAT defects ÷ total defects; if the exam gives a formula, use it.) MTBF 100 h → failure rate 0.01/h. MTBF 90 h, MTTR 10 h → availability 90%.

### H.6 Using metrics – autonomous-vehicle (AV) software case

Candidate safety metrics and their weaknesses:

1. **Code coverage** – e.g., 75% branch coverage = 3 of 4 outcomes executed. Not proof of safety.
2. **Model accuracy** – 90% accuracy on a held-out labelled test set. Hides rare dangerous errors.
3. **Failure rate** – crashes/fatalities per 1,000 rides or per million miles.
4. **Mileage** – total miles driven; easy but weak evidence of safety.

### H.7 Risks and challenges of measurement

- **Coverage paradox:** studies show only **low-to-moderate correlation** between coverage and test-suite effectiveness *when suite size is controlled*. **Confounding variable**: more tests → higher coverage *and* better bug detection. **Correlation ≠ causation** – coverage is a side effect; **diversity of test cases** catches bugs. (Analogy: coffee → cancer correlation confounded by smoking.)
- **Averages mislead:** API average 500 ms looks fine, but 95% at 200 ms, 4% at 1,000 ms, 1% at 30,000 ms (timeouts). **Use percentiles (p50, p95, p99).**
- **Spurious correlations:** more commits → more bugs? longer functions → more bugs? more meetings → higher productivity? Always ask for the **confounding variable**.

**Goodhart's Law:** *'When a measure becomes a target, it ceases to be a good measure.'*

| Case | Metric target | Result |
| --- | --- | --- |
| Bank fake accounts | Number of accounts opened (with bonuses) | **3.5 million fake accounts**, **$3 B** fines + brand damage |
| Bug bounty | Number of bugs fixed (pay per fix) | Developers **introduced bugs** to fix later; moved to team bonuses |
| Coverage mandates | 100% coverage required to merge | **Meaningless tests** written |

Bad incentives: lines of code/day (bloat), amount of documentation, low reported bugs (hiding bugs), many fixed bugs (creating bugs), accuracy of estimates (padding).

**Good practice:** use **multiple metrics**, combine with qualitative judgement, align metrics with **goals** (Goal–Question–Metric, GQM: Goal → Questions → Metrics), review and rotate metrics, never use a single metric to reward individuals.

### H.8 \[Supplement\] Risk management for quality

**Risk** = possibility of a loss; **Risk Exposure (RE) = Probability (P) × Impact/Loss (L)**. Example: P = 0.2, L = ₹50 lakh → RE = ₹10 lakh.

**Risk management process:**

1. **Identify** (checklists, brainstorming, past projects, threat modelling).
2. **Analyse/assess** (probability, impact, exposure).
3. **Prioritise** (risk matrix; top-10 list).
4. **Plan response:** **avoid**, **mitigate** (reduce probability/impact), **transfer** (insurance, outsourcing), **accept** (contingency plan).
5. **Monitor and control** (risk register, triggers, review at every sprint/milestone).

**Types:** project risks (schedule, staff), **product risks** (quality failures, security flaws), business risks.

**Risk-based testing:** test the highest-risk areas first and deepest (e.g., payments, security, complex modules) – consistent with *defect clustering* and Google's focus on **high-risk code, complex logic, public APIs**. **Quality risk examples:** insufficient testing, unreviewed AI code, third-party vulnerabilities, unstable requirements, performance under load. Mitigation: reviews, static analysis, automated regression, staged rollout, chaos testing, SBOM tracking.

> **MCQ Traps (Part H):** (1) Failure rate = 1/MTBF. (2) FP measures *functionality*, not LOC. (3) Percentiles beat averages. (4) Goodhart's law → targets distort metrics. (5) High coverage ≠ good tests. (6) ISO 9126 has **six** characteristics (FRUEMP); ISO 25010 has eight. (7) Risk exposure = probability × impact.

---

## PART I. SECURITY AND PRIVACY ASPECTS

### I.1 Why privacy matters – GDPR fines (case studies)

| Company | Fine | Reason |
| --- | --- | --- |
| **Meta** (May 2023) | **€1.2 billion** (record) | Transferred EU users' personal data to the **US** without adequate safeguards |
| **TikTok** (2025) | **€530 million** | Transferred EU data to **China**; engineers there had access; failed to assess surveillance-law risk (third-largest GDPR fine) |
| **Amazon** (2021) | **€746 million** | Targeted advertising/tracking **without proper consent** (Luxembourg regulator; complaint by a French privacy group) |

Cumulative GDPR fines since 2018: **€5.88 billion** (Jan 2025); Ireland alone ≈ €3.5 billion. Lesson: **build privacy-respecting software from day one.** (Amazon's defence 'no data breach' failed – the *advertising system itself* was the violation.)

### I.2 Security: attacks on the network

Assume the **network is not secure** and may be compromised.

| Attack | Description |
| --- | --- |
| **Person-in-the-middle (MITM)** | Intercepts traffic; can **read and modify** messages before forwarding |
| **Sniffing / eavesdropping** | Passively listening to traffic (typically on your subnet) |
| **Spoofing** | Pretending to be someone/something else: **IP spoofing**, **e-mail spoofing**, **DNS spoofing** (poisoning a DNS server so names map to wrong IPs) |

**The 'Big-3' of network security:**

| Concept | Meaning |
| --- | --- |
| **Authentication** | Knowing *with whom* you are communicating (user↔server) |
| **Authorisation** | Whether the user has **privilege** to perform an operation |
| **Confidentiality** | Others cannot read *or alter* what is said; includes protection from **replay**; does **not** hide the fact that communication occurred |

**Which protects against what (slide):** Sniffing → **confidentiality**; Spoofing → **authentication**; MITM → **authentication + confidentiality**.

**\[Supplement\] CIA triad:** Confidentiality, Integrity, Availability. Related: encryption (TLS/HTTPS), digital certificates, hashing, MFA, OWASP Top 10 (injection, broken access control, XSS...).

### I.3 Privacy

**Concept:** the **moral right of individuals to be left alone**, free from surveillance or interference by individuals, organisations or the state (Warren & Brandeis, 1890). Everyone has concerns (e.g., sharing GPS history, driving speed, sleep data, health history with an insurer). 'Is privacy dead?' – quotes by Scott McNealy ('You have zero privacy anyway. Get over it', 1999) and Eric Schmidt (2009) show the debate, but regulation shows privacy still matters.

**How privacy is protected:** laws, self-regulation, technology – through **notice and access control** over collection, use, deletion, sharing; collection limitation; use limitation; security and accountability.

**US FTC Fair Information Practice Principles (FIPPs):**

1. **Notice/Awareness** (core) – disclose practices.
2. **Choice/Consent** (core) – opt-in / opt-out.
3. **Access/Participation** – users can review and correct data.
4. **Integrity/Security** – secure and limited access.
5. **Enforcement** – mechanism to handle violations.

**Laws and regulations (slide list):**

- Government data: Freedom of Information Act (1966), Privacy Act (1974), Electronic Communications Privacy Act (1986).
- Sector laws: **COPPA** (children's online privacy), **HIPAA** (health), **FERPA** (education records).
- State laws: **CCPA** (California). EU: **GDPR**. India: **Digital Personal Data Protection (DPDP) Act**.

**Why software engineers should care:** legal compliance; consumer expectations/transparency; building trust; competing on privacy; privacy as a societal value; ethical duty.

### I.4 Privacy engineering mechanisms

- **Selective (purpose-driven) data collection** – minimise personal information.
- **Data minimisation** – de-identification, **pseudonymisation**, **anonymisation**; remove sensitive data.
- **Data retention policies**; **cryptographic tools** for confidentiality; access-controlled secure storage.
- **Socio-technical processes and audits**; **threat modelling**.
- **Privacy by Design (PbD)** – build privacy in from the start, *not bolted on late*.

| Term | Meaning |
| --- | --- |
| Pseudonymisation | Replace identifiers with tokens; can be re-linked with a key |
| Anonymisation | Irreversibly remove identifiers so individuals cannot be identified |
| Data minimisation | Collect/keep only what is necessary |

### I.5 Threat modelling and LINDDUN

**Threat modelling:** systematic methodology to identify possible threats and mitigations, using a **taxonomy of threats**. For security: **STRIDE** \[Supplement\] (Spoofing, Tampering, Repudiation, Information disclosure, Denial of service, Elevation of privilege). For privacy: **LINDDUN**.

| LINDDUN letter | Privacy threat | Event-app example question (slide) |
| --- | --- | --- |
| **L** | **Linking** | Can someone correlate a student's event registrations to learn interests, schedule, social circle? |
| **I** | **Identifying** | Does check-in expose attendee identities to others? |
| **N** | **Non-repudiation** | Does attendance create a permanent record the student cannot deny/delete? |
| **D** | **Detecting** | Can an observer detect that a person attended/registered? |
| **D** | **Data disclosure** | Are we collecting phone numbers when email would suffice? |
| **U** | **Unawareness** | Do users know their data is collected? Can they delete their account? |
| **N** | **Non-compliance** | Have we documented GDPR compliance measures? |

*(Mapping of slide questions to letters is indicative – remember the seven threat names and the acronym.)*

> **MCQ Traps (Part I):** (1) Confidentiality ≠ anonymity – it does not hide that communication occurred. (2) Spoofing is solved by authentication, not encryption alone. (3) Pseudonymised data is *still personal data* under GDPR. (4) Meta €1.2 B is the largest fine. (5) PbD = privacy at design time. (6) LINDDUN = privacy; STRIDE = security.

---

## PART J. CODE COVERAGE AND BRANCH COVERAGE

### J.1 Code coverage basics

**Code coverage** = what fraction of the code under test is **reached by existing tests** (white-box adequacy measure). **Structural coverage metrics:** **statement, decision (branch), condition** (and combinations). Procedure: build the **control flow graph**, align it with the code, and choose tests to cover nodes/edges.

**Definitions:**

- **Condition:** an **atomic** boolean expression (cannot be decomposed), e.g., `d < 0`.
- **Decision:** a boolean expression made of conditions joined by **0 or more logical connectors**, e.g., `numbers == null || numbers.length == 0`. A decision with no connector is itself a condition.

| Coverage | Requirement | Formula |
| --- | --- | --- |
| **Statement (line)** | Every statement executed at least once | executed statements ÷ total statements × 100 |
| **Decision (branch)** | Every decision evaluates to **both TRUE and FALSE** at least once (every branch/edge of the CFG taken) | branches executed ÷ total branches × 100 |
| **Condition** | **Every atomic condition** takes both TRUE and FALSE at least once | – |
| **\[Supplement\] Condition/decision (C/DC)** | Both of the above | – |
| **\[Supplement\] MC/DC** | Each condition independently affects the decision outcome (used in avionics, DO-178C) | n+1 tests for n conditions |
| **\[Supplement\] Multiple-condition** | All combinations of condition outcomes | 2ⁿ tests – most expensive |
| **\[Supplement\] Path** | Every distinct path (loops make it infinite) | – |

**Strength ordering (generally):** statement < branch/decision < condition + decision < MC/DC < multiple-condition < path. **Which requires the most tests?** Among the three on the slide, *condition (and especially multiple condition) coverage*; path coverage is strongest overall.

**Key facts:** 100% statement coverage does **not** guarantee 100% branch coverage (e.g., an `if` without `else` – the false branch may never run). 100% branch coverage **implies** 100% statement coverage. Condition coverage alone does **not** guarantee decision coverage (e.g., `A || B` with tests (T,F) and (F,T) → both conditions take T and F, but the decision is always TRUE).

### J.2 Worked example from the slides – `avgAbs(int... numbers)`

Code structure: `if (numbers == null || numbers.length == 0) throw exception;` then `for (i < numbers.length) { d = numbers[i]; if (d < 0) d = -d; sum += d; }` then return average.

Atomic conditions: **A** `numbers == null`, **B** `numbers.length == 0`, **C** `i < numbers.length` (loop), **D** `d < 0`.

| Test | A | B | C | D | Effect |
| --- | --- | --- | --- | --- | --- |
| `avgAbs(null)` | T | not evaluated (short-circuit) | – | – | Exception |
| `avgAbs()` (empty) | F | T | – | – | Exception |
| `avgAbs(-5, 3)` | F | F | T then F at exit | **T (for −5) and F (for 3)** | Normal path |

**These 3 tests give full decision coverage *and* condition coverage.** (Using `avgAbs(-5)` and `avgAbs(3)` separately would also work but needs more tests.)

**Take note:** short-circuit evaluation means B is not evaluated when A is TRUE.

### J.3 How much coverage is enough?

- **Law of diminishing returns:** as coverage → 100%, effort grows steeply – the last 10% may need \~50% more test code (rare edge cases, rare error conditions, **dead/unreachable code**).
- **Google's approach:** aim for **\~80%** target; document exceptions; focus on **high-risk code (security, payments, data), complex logic (multiple conditions), public APIs**.
- **Takeaways:** coverage gives insights into code and test adequacy; intuitive; many tools; **but not sufficient to ensure correctness** (tests may lack assertions); widely used in industry.

### J.4 Coverage tools \[Supplement – the slides link to tool lists\]

| Language/platform | Tools |
| --- | --- |
| **Java** | **JaCoCo**, Cobertura, Clover, EMMA |
| **Python** | **coverage.py**, pytest-cov |
| **JavaScript/TypeScript** | **Istanbul/nyc**, Jest (built-in) |
| **C/C++** | **gcov/lcov**, Bullseye |
| **.NET** | dotCover, Coverlet, OpenCover |
| **Platforms/integration** | **SonarQube**, Codecov, Coveralls (reports in CI), Atlassian/BrowserStack lists |

Typical workflow: instrument code (or use bytecode agent) → run test suite → generate report (line/branch %, highlighted uncovered lines) → enforce a **coverage gate** in CI.

> **MCQ Traps (Part J):** (1) Branch coverage ⇒ statement coverage, not vice-versa. (2) Condition coverage ≠ decision coverage. (3) 100% coverage ≠ bug-free. (4) Google target ≈ 80%. (5) Coverage is a *side effect* of diverse tests, not a cause of quality.

---

## PART K. ETHICS (INCLUDING USE OF AI)

### K.1 Case studies

| Case | What happened | Ethical lesson |
| --- | --- | --- |
| **Volkswagen emissions scandal** | Engineers wrote code that **detected emissions testing** and made the diesel engine run clean; on the road it emitted up to **40× the legal limit** of pollutants. \~**$30 B** fines/settlements, criminal charges against engineers, premature deaths, reputational destruction | Engineers are responsible for what code *does*; 'following orders' is no defence; deceptive code (a 'defeat device') is unethical and illegal |
| **Therac-25** | **Race condition** in radiation-therapy software caused massive overdoses – at least **6 deaths**. Causes: poor bug reporting, lack of due diligence, **overconfidence; hardware safety interlocks removed**; an \~**8-second** race window; engineers thought it 'unlikely' | Safety-critical software needs rigorous testing, independent safeguards, honest incident reporting; negligence vs bad luck |
| **Uber** (slide prompt) | Engineer discovers a serious bug/unethical behaviour: report, quit or fix quietly? | Duty to report; whistle-blowing; accountability |
| **Instagram algorithm** | Engagement metric up 15% after a change – do you ask *why*? Toxic content may be amplified for profit | Don't optimise engagement blindly; examine societal impact (Goodhart's law) |
| **Twitter image cropping** | Algorithm favoured lighter-skinned faces – trained on **biased data**, not tested for fairness, deployed to millions | **Intent doesn't excuse impact**; test for fairness |

### K.2 Algorithmic bias

Algorithms influence **where we go to school, access to money (credit), access to health care, parole, bail, risk scores**. Sources of bias: biased/unrepresentative **training data**, biased labels, proxy variables, feedback loops, lack of diverse teams, no fairness testing. **Mitigation:** diverse and audited datasets, **fairness metrics**, bias testing across demographic groups, explainability, human oversight, monitoring after deployment, transparency and appeal mechanisms.

### K.3 Ethics of using AI (open IP concerns)

- Was the **training data obtained legally** (copyrighted code/text)?
- **Who owns the IP** of LLM outputs?
- Should **sensitive/confidential information** be pasted into LLM prompts (data leakage, privacy)?
- **\[Supplement\]** Further issues: licence contamination (generated code resembling GPL code), accountability for AI mistakes, transparency about AI use (disclose AI-generated work), academic integrity, over-reliance and skills erosion, hallucinations, environmental cost, job displacement, deepfakes/misuse. Regulations: **EU AI Act** (risk-based), India DPDP Act for personal data.
- **Principle:** the **engineer remains accountable** for AI-assisted output.

### K.4 Codes of ethics – ACM

As an ACM member I will: **contribute to society and human well-being; avoid harm; be honest and trustworthy; be fair and not discriminate; honour property rights (copyrights, patents); give proper credit for IP; respect privacy; honour confidentiality.** \[Supplement\] The **ACM/IEEE-CS Software Engineering Code of Ethics** has 8 principles: *Public, Client & Employer, Product, Judgment, Management, Profession, Colleagues, Self* – with the **public interest** first.

**Limitation:** research shows a code of ethics alone **does not appear to change developers' decisions**. And law lags technology (the ADA, written in 1990 before websites, was invoked in the **Domino's** accessibility case).

### K.5 Human flourishing – three guiding questions

Harvard's human-flourishing programme: five domains – **happiness and life satisfaction; mental and physical health; meaning and purpose; character and virtue; close social relationships.**

Three questions to ask about any software:

1. Does my software **respect the humanity of its users**?
2. Does it **amplify positive or negative behaviour** for users and society?
3. Will my software's **quality impact the humanity of others**?

*Exercise – predicting which students attend events from their social-media activity:* (1) respect – involves covert profiling without consent; (2) amplification – may reinforce exclusion/stereotypes; (3) quality – inaccurate predictions may wrongly label students. Better: aggregate opt-in data, minimal collection, transparency.

> **MCQ Traps (Part K):** (1) Therac-25 → race condition + removed hardware interlocks. (2) VW → software detecting test conditions. (3) Intent does not excuse impact (Twitter crop). (4) Having a code of ethics does not by itself change behaviour. (5) Pasting confidential code into a public LLM is an ethical/IP/privacy risk.

---

## PART L. OPEN SOURCE AND SOFTWARE PATENTS

### L.1 Legal background

- **Copyright** protects *expression* of creative works including **software**; arises **automatically**; the holder can license it. **Public domain** = nobody holds exclusive rights.
- **Trademark** protects name/logo.
- **Patent** protects an *idea/invention* (see L.8).
- OSS is still **copyrighted**; copyright held by contributors or assigned to the maintaining entity; the **licence** grants permissions.

### L.2 What is open-source software?

**OSS:** source code is available; **right to modify** and create derivative works; (often) right to **redistribute** derivatives. Contrast with **proprietary software** – a black box: binaries only, governed by an **EULA** (End User License Agreement) which may forbid reverse engineering.

### L.3 History timeline (memorise order)

| Year | Event |
| --- | --- |
| Early | **UNIX** created at **AT&T Bell Labs** in portable language **C**; source distributed (AT&T barred from entering new telecom businesses) |
| **1978** | **UC Berkeley** distributes own UNIX derivative (**BSD**) – **permissive BSD licence** (credit the university, limited liability) |
| **1983** | AT&T broken up; UNIX licensing changed (no more source). **Richard Stallman** announces **GNU** ('GNU's Not Unix') – free Unix-compatible system |
| **1985** | Free Software Foundation (FSF) \[Supplement\] |
| **1991** | **Linux** (Linus Torvalds) built around GNU utilities, **GPL**-licensed; IBM invested >$1 B later; Red Hat |
| **Jan 1998** | **Netscape** opens source of its browser (**Mozilla**) – first company to open a proprietary product; created new licence/model |
| **1998** | Term **'Open Source'** coined; **Open Source Initiative (OSI)**; Tim O'Reilly's Freeware Summit → Open Source Summit |

**Free software philosophy ('free as in speech, not as in beer'):** four freedoms – **Freedom 0:** run the program for any purpose; **Freedom 1:** study and change it; **Freedom 2:** redistribute copies; **Freedom 3:** distribute modified versions. **Stallman:** *open source is a development methodology; free software is a social movement.*

### L.4 Copyleft vs permissive

|  | **Copyleft** (GPL) | **Permissive** (MIT, BSD, Apache) |
| --- | --- | --- |
| Derivative works | Must be released under **same licence** (viral) | May be released under **any licence**, even proprietary |
| Combine with restrictive licences? | **No** (not even with BSD per slide) | Yes |
| Philosophy | 'Protects the commons' | Encourages adoption |
| Company attitude | Often avoided in proprietary products ('ten-foot pole') | Preferred |

### L.5 Software licences

| Licence | Key points |
| --- | --- |
| **GNU GPL** | Copyleft; four freedoms; **source must be made available**; modifications **relicensed under GPL**; risk: **viral effect** on proprietary code |
| **LGPL** (Lesser GPL) | For **libraries**; dynamic linking is **not** a derivative work, so proprietary code may use an unmodified LGPL library; modifications to the library itself must be shared. (Related: GPL with **classpath exception**, e.g., Oracle JDK) |
| **MIT** | Simple, commercial-friendly; **retain copyright notice**; 'as is'; no liability; no other restrictions |
| **Apache 2.0** | Like MIT, **not copyleft**; no obligation to release source or use same licence; **no trademark grant**; \[Supplement\] explicit **patent grant** |
| **BSD** | 'As is', no liability; **copyright notice kept** in source and binary; **no endorsement** using the holder's name without written consent |
| **Creative Commons (CC)** | For **data, documents, images, media** (not code): **CC-BY** (attribution), **CC-BY-SA** (share-alike), **CC-BY-ND** (no derivatives) |
| **Dual licensing** | Release under GPL, **and** sell a commercial licence to firms that don't want GPL obligations – e.g., **MySQL** (about $2,000–10,000 per year) |

**Licence incompatibility example:** Sun's **OpenOffice** → Oracle acquired Sun and paused it → community forked **LibreOffice**; Oracle later gave OpenOffice to **Apache**; LibreOffice could copy OpenOffice changes but not the reverse because of **licence conflicts**.

**Copyright vs patents vs trademarks:** copyright = particular expression (books, music, source code; automatic); patents = ideas/inventions (granted on application; expiry ≈ **20 years**); trademarks = brand names/logos.

### L.6 Open-source ecosystem

**Cathedral vs Bazaar (Eric Raymond):** *Cathedral* – developed centrally by a small core group, released at milestones (GNU Emacs, GCC in the 1990s). *Bazaar* – developed **openly and organically with wide participation** (Linux). **'The Bazaar won.'**

**Stakeholders:** **core members/maintainers** (push access; admins and developers); **external contributors** (bug reports, PRs, docs); **supporters** (beta testers, sponsors, steering committees); **spin-offs** (maintainers of **forks**).

**Contributing process:** read **CONTRIBUTING.md**; follow coding style/linters and static checks; **include tests**; required number of **code reviews** by core developers; documentation standards; sign a **Contributor Licensing Agreement (CLA)** – scoped to that project; gives maintainers specific rights over your contribution (without it, you own copyright even for tiny fixes, causing legal headaches).

**Governance:** *Corporate-led* – Chromium (Google), Moby (Docker), Ubuntu (Canonical), TensorFlow (Google), PyTorch (Meta), Java (Oracle): company controls roadmap. *Foundation/community-led* – Apache projects (Hadoop, Spark, Kafka, Tomcat), Firefox (Mozilla), Python (PSF), NumPy; **benevolent dictators** (Python, Linux).

**Using OSS inside companies:** licence compatibility; versioning/update policy (assign internal **owners** of third-party dependencies); customisation (**internal forks** hard to sync with upstream → assign owner or contribute upstream); **supply-chain attacks** (use SBOM, scanning).

| Advantages of going open source | Disadvantages |
| --- | --- |
| Transparency, user trust | Reveals implementation secrets |
| 'Many eyes': crowd-sourced bug reports/fixes, vulnerabilities found sooner | Many eyes also let **attackers** find vulnerabilities first |
| Community, adoption, contributions, forks | Loss of control over long-term direction |

### L.7 Open source vs proprietary vs freeware

|  | Open source | Proprietary | Freeware |
| --- | --- | --- | --- |
| Source code | Available | Hidden (binary only) | Usually hidden |
| Modify/redistribute | Allowed per licence | Forbidden (EULA) | Usually forbidden |
| Cost | Often free | Paid | Free of charge |
| Example | Linux, Firefox | Windows, MS Office | Adobe Reader |

### L.8 Software patents

**Definition:** an **exclusive right granted for an invention** – a product or process providing a **new way of doing something** or a new technical solution; the inventor must **publicly disclose** technical information in the application. Criteria: **new, useful, non-obvious**. Term: **20 years**.

**What it grants:** NOT the right to make/use/sell; **the right to exclude others** from making, using, selling the invention – allows suing, **licensing and royalties**.

| Patents | Copyright |
| --- | --- |
| Cover **ideas** and their use | Cover **expression**, never ideas |
| **Issued by a patent office** after application | **Automatic** |
| Independent invention is **no defence** | Independent creation is a defence |
| Expensive (US software patents \~**$15,000–45,000**) | Free |

**Why patents exist:** encourage disclosure; reward invention; protect R&D investment; encourage 'design around'; protect small firms.

**Problems with software patents (slide):** *Alice v. CLS Bank (2014)* – US Supreme Court ruling limiting abstract-idea patents; weak **non-obviousness** standards; long pendency; **incompatibility** (PNG created to avoid GIF patent issues; **Opus** vs MP3; AV1 vs H.265); **independent discovery doesn't matter** (John Carmack quote); only large organisations benefit (cost, legal fees); **patent trolls / Non-Practicing Entities (NPEs)** – e.g., Innovation (WiFi in public spaces), NPHJ ('scanning and emailing documents'), Boadin (autocomplete); innovation stifled; even open source threatened.

**Possible remedies:** **Defensive Patent License (DPL)**, **Open Invention Network (OIN)**, **License on Transfer (LOT)**, bogus-patent bounties, Unified Patents, commonsense reform, abolishing software patents.

*'Patent or not?' discussion* (bingo on a computer, ordering pizza with custom toppings, CAPTCHA selection, SMS bank balance...): under *Alice*, merely implementing an abstract idea on a generic computer is typically **not** patentable; a **specific technical improvement** may be.

**India \[Supplement\]:** the Patents Act 1970, **Section 3(k)** excludes 'a mathematical or business method or a computer programme *per se* or algorithms'; software with a **technical effect** may be patentable; software is mainly protected by **copyright**.

> **MCQ Traps (Part L):** (1) GPL = copyleft; MIT/BSD/Apache = permissive. (2) LGPL allows dynamic linking from proprietary code. (3) Patents give the right to *exclude*, not the right to *use*. (4) Copyright is automatic, patents are granted. (5) CLA ≠ licence of the project; it's the contributor's rights agreement. (6) Cathedral = centralised; Bazaar = open. (7) CC licences are for data/content, not primarily code. (8) Open source ≠ free of cost necessarily; free software = liberty.

---

## PART M. SYSTEM TESTING AND SECURITY VALIDATION

### M.1 Levels of testing (with the event-app example)

| Level | Who | What | Example | Notes |
| --- | --- | --- | --- | --- |
| **1. Unit** | Developer | Individual functions/methods/classes in isolation | `registerStudent()` alone | Verifies the unit |
| **2. Integration** | Developer or QA | Interactions between two or more components; **interface errors** missed at unit level | `registerStudent() → saveToDatabase() → sendConfirmationEmail()` | **Most bugs hide here** |
| **3. System** | **Independent QA team** | Entire integrated system **end-to-end**, black-box | Login → Search event → Register → Pay → Confirm | Verifies compliance with **SRS**; finds bugs not attributable to a single component; information directs release |
| **4. Acceptance** | Customer/end-user representatives | Does it meet business requirements? | 'Can a student register for exactly 3 events per week?' | **Final quality gateway** before release |

### M.2 Integration testing approaches

**Integration** = combining 2+ units and getting expected results. It matters because many modules that have never been together now interact; if done badly, all problems appear at once and are hard to diagnose.

| Approach | Description | Needs | Drawback |
| --- | --- | --- | --- |
| **Phased / Big-bang** | Develop and test units separately, combine all at once, 'hope for the best' | – | Hard to isolate faults; late discovery |
| **Incremental** | Add modules step by step | Stubs/drivers | Benefits: errors easy to isolate; system always (relatively) working; good for morale |
| **Top-down** | Start with outer **UI layers**, work inward | Many **lower-level stubs** | Postpones hard design decisions |
| **Bottom-up** | Start with lower data/logic layers, work outward | **Upper-level drivers/stubs** | High-level/UI design flaws found late |
| **Sandwich (hybrid)** | Build a **skeleton**; connect top-level UI to crucial bottom components, then add middle layers | Both | **Most common/agile** |

**Choosing:** UI requirements clear → **top-down**; DB schema critical → **bottom-up**; most agile projects → **sandwich**.

### M.3 Stubs

**Stub:** a **controllable replacement** for a software unit – simulates components that are *not yet developed, hard to control (third-party service, DB, network), slow (real DB calls) or have side effects (sending emails, charging cards)*. Example: a `StubDatabase` returns fake data so registration logic can be tested in isolation with no risk of data corruption. **Key point:** minimise code changes when you remove the stub. (Stub ≠ mock – mocks also verify interactions \[Supplement\].)

### M.4 System testing – definition and process

Tests a **completely integrated system** to verify compliance with the **Software Requirement Specification (SRS)**; finds defects between assemblages and in the system as a whole; **black-box**, **end-to-end** flow.

**Process:** test environment setup → create test cases → create test data → execute test cases → defect reporting and logging.

### M.5 Types of system testing (frequent MCQ source)

| Type | Purpose |
| --- | --- |
| **Smoke / sanity testing** | Check the **most important functions work**; decide if the build is fit for further testing. *Smoke = broad and shallow on a new build; sanity = narrow and deep on a specific fix* \[Supplement\] |
| **Performance testing** | Responsiveness, **scalability, stability, reliability** |
| **Functional testing** | Features and scenarios incl. failure and boundary cases |
| **Non-functional testing** | Attributes like performance, robustness |
| **Recovery testing** | **Restart capability** after disaster/unanticipated shutdown |
| **Destructive testing** | Make the app fail in an **uncontrolled manner** to find the **point of failure** and robustness |
| **Installation testing** | Software installed with all components and works; **all components removed** on uninstall |
| **Usability testing** | Ease of use from the **user's perspective**; skill needed and time to learn |
| **Cloud testing** | Testing using cloud services |
| **Localization testing** | Quality of product localisation for a **target culture** (language, date/currency formats) |
| **Compliance testing** | Compliance with internal/external **standards** and regulations |
| **Scalability testing** | Ability to scale **up or down** |
| **Boundary tests** | Black-box using boundary values |
| **Regression testing** | Changes caused no **unintended side effects** |
| **Startup & shutdown tests** | Shutdown leaves no stale lock files/inconsistent data; startup reaches a **deterministic, consistent state** |
| **Platform (cross-platform) tests** | Behaviour in different OS/browsers/devices |
| **Load tests** | Behaviour under **expected or above-average** load |
| **Stress tests** | Push **beyond maximum design load** to find the limit and failure behaviour |
| **Security testing** | Find vulnerabilities; ensure data and resources are protected |

**Load vs stress:** load = expected/above-average usage; stress = **extreme** load until the system breaks.

**Acceptance testing:** suite of tests on the completed system; each case exercises a particular operating condition/feature of the users' environment; tests created collaboratively by **customers, business analysts, testers and developers**; verifies a **user story is complete in a sprint**; confidence that business requirements are met; **final quality gateway**. \[Supplement\] Categories: **UAT**, **business/contract acceptance**, **regulatory acceptance**, **operational acceptance**, **alpha and beta**.

**Regression testing:** re-run tests after each change; automated; tests pass before commit. **Release testing** = a form of system testing by a separate team (validation). **Performance testing** in release testing: increase load until performance becomes unacceptable; tests reflect the **usage profile**; **stress testing** deliberately overloads to examine failure behaviour.

*Scenario-based release test (slide: patient-record system):* test features in the order a real user would: authentication → download/upload patient records to laptop → home-visit scheduling → encryption/decryption of records on a mobile device → record retrieval/modification → drugs database links → call prompting.

### M.6 Penetration testing (pentesting)

**Definition:** an **authorised, simulated cyberattack** on a system to evaluate its security. Goal: **identify and exploit vulnerabilities before real attackers do.** Uses the same tools/techniques as attackers; combines **automated scanning with manual exploitation**; reports vulnerabilities, how they are exploited and **recommended fixes**. Complements vulnerability scans; finds weaknesses in networks, applications, configurations and **human factors**.

| Type | Tester knowledge | Notes |
| --- | --- | --- |
| **White-box** | **Full** knowledge (source code, architecture) | Most thorough; typical for industry products (e.g., hardware) |
| **Black-box** | **Only public information** (like an external attacker) | Best simulates a real outsider |
| **Gray-box** | **Partial** knowledge (some credentials/architecture) | Balance of both |

**Five-phase process (OWASP-style):**

1. **Reconnaissance** – gather information (domains, IPs, tech stack) via public sources (**WHOIS, Shodan**) to find attack surface.
2. **Scanning** – probe open ports, services, known vulnerabilities (**Nmap**, scanners).
3. **Vulnerability assessment** – analyse results, identify and **prioritise** high-risk issues.
4. **Exploitation** – exploit confirmed vulnerabilities (SQL injection, XSS, misconfigurations) to **prove impact**.
5. **Reporting** – findings, evidence (screenshots/logs), remediation advice.

*Memory aid:* **R-S-V-E-R** (Recon, Scan, Vulnerability assessment, Exploit, Report). \[Supplement\] Real-world pentests also include post-exploitation and re-test after fixes.

**Value:** prevents breaches; **compliance** (e.g., **PCI DSS** mandates regular pentests) and assurance; red-teaming and risk assessment reveal *process* issues (weak passwords, unpatched servers).

**Open-source tools:** **Nmap** (network mapper/scanner – hosts, ports, services); **Metasploit Framework** (exploitation platform with payloads); **OWASP ZAP** (web-app scanner, DAST); **SQLMap** (automates SQL-injection discovery/exploitation); **Wireshark** (packet capture/analysis).

**Case studies:** global enterprise (2023) – **372 vulnerabilities** found, estimated **$2.591 M/day** downtime cost if exploited; **Marriott breach** (2020) – **5.2 M** guest records, intrusion mid-Jan discovered late Feb; IoT devices – hard-coded credentials and vulnerable third-party components.

### M.7 Fuzzing (fuzz testing)

**Definition:** automated testing by feeding a program **random, malformed or unexpected inputs** to trigger **crashes, assertion failures, memory errors** and other (often security-relevant) bugs. A **dynamic analysis** technique; explores **thousands to millions of inputs**; especially effective for **low-level C/C++ memory bugs and poorly validated parsers**.

**Types of fuzzers:**

| By input strategy | Meaning |
| --- | --- |
| **Mutation-based** | Start from **valid samples** and modify them (flip bits, change lengths, insert special values) |
| **Generation-based** | Use a **model/grammar of the input format** to generate inputs from scratch |

| By knowledge of target | Meaning |
| --- | --- |
| **White-box** | Full knowledge of source/structure; can use **symbolic execution** or instrumentation |
| **Gray-box** | Uses **feedback (e.g., code coverage)** with limited instrumentation, treats target as black box – **coverage-guided** |
| **Black-box** | No knowledge of internals; random inputs to an external interface ('dumb' fuzzer) |

**Fuzzing process (loop):** (1) **Input generation** (mutate seeds/generate); (2) **Program execution** under a monitor (**sanitisers** such as AddressSanitizer); (3) **Monitoring/analysis** – crashes, hangs, leaks, assertion failures logged; (4) **Feedback-driven loop** – inputs that reach **new code paths** are favoured for further mutation.

**Value:** finds deep memory-safety bugs invisible to code review; works on anything with a programmable interface (libraries, parsers, network servers, kernels); **continuous fuzzing** (Google **OSS-Fuzz**, \~10,000 bugs found); complements pentesting (fuzzing is low-level; pentest is high-level behaviour).

**Tools:** **AFL / AFL++** (coverage-guided mutation fuzzer, C/C++); **libFuzzer** (in-process, LLVM); **Honggfuzz**; **Syzkaller** (Linux kernel fuzzer using system-call models); **Radamsa** (gray-box dumb mutational); **Hypothesis** (Python **property-based testing**); BooFuzz (network protocols); python-afl.

**Case studies:** **Mozilla Firefox 2022–23** – fuzzers found 20 vulnerabilities (5 critical, 15 high) before release; **OpenSSL 2022** – OSS-Fuzz found two critical CVEs (**CVE-2022-3602, CVE-2022-3786**, stack buffer overflows in the **Punycode** parser); **Linux kernel** – Syzkaller found thousands of bugs since 2015 (e.g., privilege escalation CVE-2023-0386).

| Pentesting | Fuzzing |
| --- | --- |
| Simulates **attacker behaviour** | Bombards code with **many abnormal inputs** |
| Network/application-level, partly manual | Automated, low-level code paths |
| Findings: misconfigurations, injection, weak credentials | Findings: crashes, memory-safety bugs |
| Tools: Nmap, Metasploit, ZAP, SQLMap | Tools: AFL, libFuzzer, Hypothesis |

They are **complementary** security-validation techniques; fuzzing should run in CI. *Class exercise:* modules that benefit most from fuzzing – input parsers, file upload handlers, API request deserialisers, anything handling untrusted data.

> **MCQ Traps (Part M):** (1) System testing is black-box and end-to-end; acceptance testing is the final gate. (2) Stress ≠ load. (3) Top-down needs stubs; bottom-up needs drivers. (4) White-box pentest = full knowledge; black-box = only public info. (5) Mutation-based fuzzers start from valid inputs; generation-based use a grammar. (6) Fuzzing = dynamic analysis. (7) Recon comes first, exploitation after vulnerability assessment.

---

## PART N. SOFTWARE EVOLUTION, MAINTENANCE AND SUPPORT

### N.1 Software evolution / re-engineering

**Re-engineering** = rebuilding an existing system; **takes time, significant cost, absorbs resources** (not done in a few months). *House analogy:* inspect before rebuilding; be sure the structure is actually weak; understand how the original was built; use modern long-lasting materials; be disciplined.

**Re-engineering process model – six cyclic activities:**

| Activity | Meaning |
| --- | --- |
| **1. Inventory analysis** | Maintain an **inventory of all applications** (business criticality, age, maintainability) to prioritise candidates |
| **2. Document restructuring** | Choose appropriate documentation option: leave as is, document only critical parts, or rewrite completely |
| **3. Reverse engineering** | Analyse a program to create a **higher-level abstraction** than source code (design, data, process models) |
| **4. Code refactoring/restructuring** | Restructure hard-to-understand/test/maintain modules **without changing external behaviour** |
| **5. Data restructuring (refactoring)** | Dissect data architecture, define data models, identify objects/attributes, review structures for quality |
| **6. Forward engineering** | Regenerate the system in a form with the **best aspects of quality** (new architecture/technology) |

*Memory aid:* **I-D-R-C-D-F** (Inventory, Document, Reverse, Code, Data, Forward). **Refactoring** changes internal structure only (no behaviour change).

### N.2 Software maintenance

- Maintenance begins almost **immediately after release** (bug reports within days; environment-specific changes within weeks; new enhancements within months).
- **Definition:** activities needed to **keep software operational after it has been accepted and delivered** to end users; continues for the life of the product; often the **majority of money** is spent on maintenance.
- **Maintainable software:** effective **modularity**; design patterns for easy understanding; coding standards and conventions (self-documenting); QA that found maintenance problems before release; written by engineers who realise they may not be around; design and implementation that *assist* the person making the change.

**Four types of maintenance (very high-yield):**

| Type | Nature | Purpose | Example |
| --- | --- | --- | --- |
| **Corrective** | **Reactive** | **Repair problems** (bugs) found after delivery | Fix crash in registration |
| **Adaptive** | **Reactive** | Keep software usable in a **changing environment** (OS, hardware, regulations) | Support new Android version / new tax rule |
| **Perfective** | **Proactive** | Add **new features**, improve code structure, performance or documentation | Add dark mode; optimise queries |
| **Preventive** | **Proactive** | **Detect and correct latent faults** before users find them | Refactor, fix potential overflow, update libraries |

Proactive maintenance can be scheduled/planned; **reactive maintenance is 'firefighting'** – cannot be planned. \[Supplement\] Typical effort split: perfective ≈ 50%, adaptive ≈ 25%, corrective ≈ 20%, preventive ≈ 5%. **Lehman's laws:** continuing change, increasing complexity, declining quality unless maintained.

**Maintenance in Agile (slide list):** use sprints to organise maintenance; allow urgent customer requests to interrupt; experienced devs mentor newcomers; multiple members accept requests; balance written documentation with face-to-face communication; informal use cases; developers test each other's work; share knowledge; short, frequent, focused planning meetings.

### N.3 Software support

**Umbrella activity** including: **change management, proactive risk management, process management, configuration management, quality assurance, release management.**

**Release management:** brings **high-quality code changes from a developer's workspace to the end user**: code-change integration, **continuous integration**, build-system specification, **infrastructure as code**, deployment and release. **Ultimately software must be retired** (end-of-life; release and retirement timeline).

**Effective support requires:** corrections, adaptations, enhancements (maintenance) **plus** ongoing **operational support**, **end-user support** (answering queries on installation, operation, use) and **re-engineering** over the whole life. The **iterative software support model** cycles through these.

**Supportability:** *the capability of supporting a software system over its whole product life* – including equipment, support infrastructure, extra software, facilities, labour and any resources needed to keep it operational.

**Support facilities:** support staff need a **database of previously encountered defects** (characteristics, cause, cure) so they can examine *similar* defects and diagnose rapidly; end-user support staff answer day-to-day questions.

| Maintenance | Support |
| --- | --- |
| Modifying software after delivery (4 types) | Umbrella: maintenance + operational + end-user support + re-engineering + CM/QA/release mgmt |

> **MCQ Traps (Part N):** (1) Corrective & adaptive = reactive; perfective & preventive = proactive. (2) New feature after release = perfective, not adaptive. (3) Re-engineering has six activities; reverse engineering = *higher-level abstraction*. (4) Forward engineering is the last step. (5) Software must eventually be retired.

---

## PART O. UNIT 4 – KEY FACTS, CONFUSED CONCEPTS, TRAPS AND PRACTICE

### O.1 Key numbers and keywords

| Fact | Value |
| --- | --- |
| ISO 9126 | **6** characteristics (FRUEMP) |
| Failure rate | **1 / MTBF** |
| Availability | **MTBF / (MTBF + MTTR)** |
| Function points example | **75 FP**, \~10 FP/person-month → 7–8 months |
| Goodhart's law | Measure becomes target → stops being a good measure |
| Google coverage target | **\~80%** |
| GDPR fines | Meta **€1.2 B**, Amazon **€746 M**, TikTok **€530 M**; total **€5.88 B** |
| FIPPs | **5** principles; Notice and Choice are core |
| Privacy threat taxonomy | **LINDDUN** |
| Therac-25 | Race condition, ≥ **6 deaths** |
| VW | Up to **40×** legal NOx limit; **$30 B** |
| Patent term | **20 years**; right to **exclude** |
| Pentest phases | Recon → Scan → Vulnerability assessment → Exploit → Report |
| Re-engineering | **6** activities (IDRCDF) |
| Maintenance | **4** types: corrective, adaptive, perfective, preventive |
| OSS-Fuzz | \~10,000 bugs found |

### O.2 Frequently confused concepts

| Pair / group | Distinction |
| --- | --- |
| Statement vs branch vs condition coverage | Each statement run / each decision T and F / each atomic condition T and F |
| Condition vs decision | Atomic boolean vs expression of conditions joined by connectors |
| Load vs stress vs soak | Expected load / beyond limit / long duration |
| Smoke vs sanity vs regression | Build acceptance (broad) / focused check after fix (narrow) / unchanged features still work |
| Corrective vs preventive | Fix known bug (reactive) vs fix latent fault before it is noticed (proactive) |
| Adaptive vs perfective | Environment change vs new features/improvement |
| Reverse vs forward engineering | Abstract design from code vs regenerate improved system |
| Refactoring vs re-engineering | Internal cleanup, same behaviour vs rebuilding the whole system (process with six activities) |
| Pentest vs fuzzing vs vulnerability scan | Simulated attack by humans+tools vs random-input automation vs automated scan for known weaknesses |
| White vs gray vs black box (pentest/fuzz) | Full / partial / no internal knowledge |
| Mutation-based vs generation-based fuzzing | Modify valid seeds vs create from grammar |
| Copyright vs patent vs trademark | Expression (automatic) vs idea (granted) vs brand |
| Copyleft vs permissive | Same-licence requirement vs few restrictions |
| GPL vs LGPL | Strong copyleft vs library-friendly |
| Free software vs open source | Ethical/social movement (freedoms) vs development methodology |
| Cathedral vs Bazaar | Central core team vs open, organic |
| Authentication vs authorisation vs confidentiality | Who are you / what may you do / who can read it |
| Pseudonymisation vs anonymisation | Reversible with key vs irreversible |
| Function points vs LOC | Functional size vs code size |
| MTBF vs MTTR vs MTTF | Between failures (repairable) / to repair / to failure (non-repairable) |
| Verification vs validation (quality) | Built right vs right product |

### O.3 Potential MCQ traps (consolidated)

1. 'Higher coverage means better tests' – **false** (confounded by suite size; need assertions and diversity).
2. 'Average response time is a good performance metric' – **false**; use percentiles.
3. 'Pseudonymised data is out of GDPR scope' – **false**.
4. 'Confidentiality hides that communication happened' – **false**.
5. 'A patent gives the right to use the invention' – **false**; right to exclude.
6. 'Copyright must be registered to exist' – **false**; automatic.
7. 'GPL allows closed-source derivatives' – **false**.
8. 'MIT/BSD require derivatives to remain open' – **false**.
9. 'Adding new features is adaptive maintenance' – **false**; perfective.
10. 'Fuzzing is static analysis' – **false**; dynamic.
11. 'Black-box pentesters have source code' – **false**.
12. 'Acceptance testing is done by developers' – **false**; customers/users.
13. 'Ethics codes alone fix developer behaviour' – **false** per slides.
14. 'Failure rate = MTBF' – **false**; inverse.
15. 'FIPPs include only notice' – five principles.

### O.4 UNIT 4 PRACTICE MCQs (with answers and explanations)

**Q1.** A server has an MTBF of 200 hours. Its failure rate is: A) 200 per hour  B) 0.05 per hour  C) 0.005 per hour  D) 2 per hour **Answer: C.** Failure rate = 1/MTBF = 1/200.

**Q2.** An application has 3 external inputs (average weight 7), 2 external outputs (average weight 5), 4 simple queries (weight 3) and 2 internal files (average weight 4). Unadjusted function points? A) 41  B) 46  C) 56  D) 51 **Answer: D.** 21 + 10 + 12 + 8 = 51.

**Q3.** A company pays developers per bug fixed and later finds developers introducing bugs on purpose. This is an example of: A) Goodhart's law  B) Pareto principle  C) Pesticide paradox  D) Brooks's law **Answer: A.** The measure became a target.

**Q4.** Average API latency is 500 ms yet 1% of users see 30-second timeouts. The better reporting practice is: A) Report the mean only  B) Report percentiles (p95/p99)  C) Report the maximum only  D) Ignore outliers **Answer: B.** Averages hide tail latency.

**Q5.** Which statement is TRUE? A) 100% statement coverage guarantees 100% branch coverage  B) 100% condition coverage always implies decision coverage  C) Coverage proves absence of bugs  D) 100% branch coverage implies 100% statement coverage **Answer: D.** Taking every branch executes every statement; the reverse can fail (e.g., an `if` without `else`).

**Q6.** For the decision `A || B`, tests (A=T,B=F) and (A=F,B=T) achieve: A) Decision coverage but not condition coverage  B) Neither  C) Condition coverage but not decision coverage  D) Both **Answer: C.** Each condition is T and F, yet the decision is always TRUE.

**Q7.** 'The app must run on iOS, Android and browsers' relates to which ISO 9126 characteristic? A) Portability  B) Efficiency  C) Reliability  D) Functionality **Answer: A.**

**Q8.** Which of the 'Big-3' primarily defends against **sniffing**? A) Authentication  B) Confidentiality  C) Authorisation  D) Availability **Answer: B.** Encryption prevents eavesdroppers reading traffic.

**Q9.** An attacker poisons DNS so users reach a fake bank site. This is: A) Sniffing  B) Replay  C) Spoofing  D) Fuzzing **Answer: C.** Authentication (e.g., certificates) mitigates it.

**Q10.** The record GDPR fine of €1.2 billion (2023) was imposed on: A) Meta  B) Amazon  C) TikTok  D) Google **Answer: A.** For EU→US data transfers.

**Q11.** A hospital replaces patient names with random tokens but keeps a secure lookup table. This is: A) Anonymisation  B) Encryption in transit  C) Hashing passwords  D) Pseudonymisation **Answer: D.** Reversible with the key, therefore still personal data.

**Q12.** The primary technical cause of the Therac-25 accidents was: A) SQL injection  B) A race condition (with hardware interlocks removed)  C) Buffer overflow in the network stack  D) Floating-point rounding **Answer: B.**

**Q13.** A proprietary product needs to use an open-source library **dynamically, unmodified**, without releasing its own source. The safest licence for that library is: A) GPL  B) AGPL  C) LGPL  D) A licence requiring share-alike for linked code **Answer: C.** LGPL permits proprietary code to link to an unmodified library.

**Q14.** If you modify a GPL project and distribute it, you must: A) Release your modifications under GPL  B) Keep it secret  C) Pay royalties  D) Obtain patent approval **Answer: A.** Copyleft.

**Q15.** Which statement about patents and copyright is correct? A) Patents arise automatically  B) Copyright protects ideas  C) Patents last for life of author  D) Copyright arises automatically while patents are granted by an office on application **Answer: D.**

**Q16.** Which integration approach requires writing many lower-level **stubs**? A) Bottom-up  B) Top-down  C) Big-bang  D) Regression **Answer: B.** The UI is built first, so lower layers must be simulated.

**Q17.** In penetration testing the first phase is: A) Exploitation  B) Reporting  C) Reconnaissance  D) Scanning **Answer: C.**

**Q18.** A tool takes valid PDF files, flips bits, and keeps inputs that reach new code paths. It is a: A) Coverage-guided mutation-based fuzzer  B) Generation-based black-box fuzzer  C) Static analyser  D) Load generator **Answer: A.** (e.g., AFL).

**Q19.** A tester is given source code and architecture diagrams before a pentest. This is: A) Black-box  B) Gray-box  C) Red team only  D) White-box **Answer: D.**

**Q20.** After Android 15 is released the team updates the app so it still works. This is: A) Corrective  B) Adaptive  C) Perfective  D) Preventive **Answer: B.** Environment changed; no new user feature.

**Q21.** Developers refactor a module to remove an overflow risk nobody has reported yet. This is: A) Corrective  B) Adaptive  C) Preventive  D) Perfective **Answer: C.**

**Q22.** Deriving a design model and data-flow diagrams from legacy source code is: A) Reverse engineering  B) Forward engineering  C) Inventory analysis  D) Regression testing **Answer: A.**

**Q23.** 40 defects were found before UAT and 10 during UAT. Using the slide's formula (UAT ÷ before UAT × 100), defect leakage is: A) 10%  B) 20%  C) 80%  D) 25% **Answer: D.**

**Q24.** MTBF = 90 h, MTTR = 10 h. Availability? A) 80%  B) 90%  C) 95%  D) 99% **Answer: B.** 90/(90+10).

**Q25.** A risk has probability 0.3 and loss of ₹20 lakh. Risk exposure is: A) ₹2 lakh  B) ₹50 lakh  C) ₹6 lakh  D) ₹20 lakh **Answer: C.** 0.3 × 20 = 6.

**Q26.** The VW emissions scandal is an ethical failure because: A) Software detected test conditions to cheat regulators  B) The code had a memory leak  C) The UI was confusing  D) Licences were violated **Answer: A.**

**Q27.** A developer pastes proprietary source code into a public chatbot to debug it. The main concern is: A) Slower compile time  B) Code style  C) Test coverage  D) Confidentiality/IP leakage **Answer: D.**

**Q28.** Who typically performs **acceptance testing**? A) Developers  B) Customers/end-user representatives  C) Compilers  D) Build engineers **Answer: B.**

**Q29.** Pushing a system to 3× its maximum design load until it fails is: A) Load testing  B) Soak testing  C) Stress testing  D) Smoke testing **Answer: C.**

**Q30.** Google's recommended coverage target (per the slides) is about: A) 80%  B) 100%  C) 50%  D) 95% **Answer: A.** Diminishing returns beyond; focus on high-risk code.

**Q31.** Which licence is best for open data, images and slides? A) GPL  B) Creative Commons  C) BSD  D) EULA **Answer: B.**

**Q32.** Under the 'three human-flourishing questions', which question is **not** asked? A) Does it respect users' humanity?  B) Does it amplify positive or negative behaviour?  C) Will quality impact others' humanity?  D) Will it maximise quarterly profit? **Answer: D.**

---

# CONCISE REVISION SHEET (UNITS 3 & 4)

## Unit 3 – one-page recall

**AI4SE:** force-multiplier, not SDLC replacement; use across requirements, design, coding, testing, DevOps, maintenance; **humans validate everything**. AI code security flaws ≈ **40–45%** (XSS 86%, log injection 88%, SQLi 20%, crypto 14%). LLM decision framework: **alternatives, error probability, risk tolerance, risk mitigation**. **Vibe coding** = prompt, accept, run, iterate with little review → prototypes only.

**SE4AI:** ML system ≠ ML model; ML = unreliable function, no specification, accuracy not correctness; MLOps ⊂ DevOps; safety outside the model (human-in-loop, undo, doer-checker, guardrails, containment); hazard analysis; model placement (latency, bandwidth, offline, size).

**Implementation:** 40–50% effort; 80% reading code. Language choice = ecosystem, team skills, performance, platform, constraints. Naming: Pascal (classes), camel (methods), `_` prefix (internal). Standards = mandatory; guidelines = advisory. Defensive (Murphy's law, TOCTOU, NULL checks, locks) · Secure (validate input, default deny, least privilege, sanitise) · Testable (assertions, test points, stubs, harness, instrumentation). Dangling else → nearest if → use braces. Sprint metrics: burndown (remaining), velocity (completed/sprint), throughput, cycle time.

**Reviews:** code inspection 60% vs unit 25%; 4.5→0.82 errors/100 LOC; AT&T +14%/−90%; checklist (design, functionality, complexity, tests, naming, comments, style, docs); criticise code not coder; good PR = title, context, rationale, links, screenshots.

**Codebase:** README → structure → build → run → debugger/logs → search constants; novice vs expert; white/grey/black-box access; Redis example.

**Static analysis:** no execution; linters → pattern detectors → type annotation → data-flow; false positives; halting problem. Dynamic: real but input-dependent.

**CI/CD/DevOps:** build system = tasks + dependencies (DAG, topological sort, in-degree) + execution; CI = frequent merge+build+sanity; delivery (manual release) vs deployment (automatic); DevOps pillars **Collaboration, Affinity, Tools, Scaling**; DevSecOps = shift-left security, 'software, safer, sooner'.

**Testing:** 7 principles; BVA (min, min+1, nominal, max−1, max); EP; pairwise (50–90%); test case types positive/negative/destructive; suite states active/in-progress/completed; inspection (static) vs testing (dynamic); unit → component → system; interface errors misuse/misunderstanding/timing; regression; release (independent, black-box) ; alpha (dev site) / beta (user site) / acceptance (customer); performance: load / stress / soak; profiling/tracing; chaos engineering (Chaos Monkey, SPS); **CC = E − N + 2P**; mutation (tests the tests), fault seeding (tests the process), error guessing.

**SCM:** CI identification, control, auditing, status accounting; roles (CM, developer, auditor, CCB); SCMP; baseline (formally reviewed, formal change); branching (GitFlow vs trunk-based); semantic versioning MAJOR.MINOR.PATCH; version vs revision vs release; build/install/promotion/release management; change request → analysis → CCB; impact analysis via traceability; defect lifecycle (Discover→Report→Validate→Analyse→Assign→Fix→Verify→Close→Release); patch management (test, deploy, rollback); SBOM (SPDX, CycloneDX, SWID; Syft, Trivy).

## Unit 4 – one-page recall

**Quality & metrics:** ISO 9126 = Functionality, Reliability, Usability, Efficiency, Maintainability, Portability. Metric (IEEE 1061) = function → single number. LOC, Halstead, CC, OO metrics (DIT, NOC, CBO). **Function points** (EI, EO, EQ, ILF, EIF; slide = 75 FP). MTBF, failure rate = 1/MTBF, availability = MTBF/(MTBF+MTTR), defect density, DRE, leakage. Pitfalls: coverage paradox/confounders, averages vs percentiles, spurious correlation, **Goodhart's law**. Risk exposure = P × L; avoid / mitigate / transfer / accept; risk-based testing.

**Security & privacy:** MITM, sniffing, spoofing (IP/e-mail/DNS); Big-3 authentication, authorisation, confidentiality (sniffing→confidentiality; spoofing→authentication; MITM→both). Privacy = right to be left alone; FIPPs (notice, choice, access, integrity/security, enforcement); laws COPPA, HIPAA, FERPA, CCPA, GDPR, DPDP; privacy engineering (minimisation, anonymisation, retention, PbD); LINDDUN; fines Meta €1.2 B, Amazon €746 M, TikTok €530 M.

**Coverage:** statement < branch/decision < condition+decision < MC/DC; branch ⇒ statement; condition ⇚ decision; Google ≈ 80%; tools JaCoCo, coverage.py, Istanbul, gcov, Cobertura; coverage ≠ correctness.

**Ethics:** VW (defeat device), Therac-25 (race condition, ≥6 deaths), Uber, Instagram, Twitter crop (bias); ACM code; AI ethics (training-data legality, output ownership, sensitive inputs); human flourishing 3 questions.

**Open source & patents:** BSD(1978) → GNU(1983) → Linux(1991) → Netscape/OSI(1998); four freedoms; copyleft (GPL, LGPL) vs permissive (MIT, BSD, Apache); CC for content; dual licensing (MySQL); CLA; Cathedral vs Bazaar; patent = new, useful, non-obvious, 20 years, right to exclude; copyright automatic; patent trolls; Alice (2014); DPL, OIN, LOT.

**System testing:** levels unit/integration/system/acceptance; integration big-bang, top-down (stubs), bottom-up (drivers), sandwich; stubs; types (smoke, performance, recovery, destructive, installation, usability, localisation, compliance, scalability, startup/shutdown, platform, load, stress, security, regression). **Pentest** (white/black/gray; Recon→Scan→Assess→Exploit→Report; Nmap, Metasploit, ZAP, SQLMap, Wireshark) · **Fuzzing** (mutation/generation; white/gray/black; AFL, libFuzzer, Syzkaller, Hypothesis; OSS-Fuzz).

**Evolution & maintenance:** re-engineering: Inventory, Document restructuring, Reverse eng., Code refactoring, Data refactoring, Forward eng.; maintenance: corrective & adaptive (reactive), perfective & preventive (proactive); support = change mgmt + risk + process + CM + QA + release mgmt; defect database for support staff; software is eventually retired.

---

# 40-MARK MOCK TEST (ALL MCQ, 1 MARK EACH)

Time suggestion: 40 minutes. Answer key is at the very end of this document.

**1.** A reviewer finds that an AI-generated login function stores passwords in plain text. The correct action is: A) Merge because tests pass  B) Block the merge and raise a security defect  C) Ask the AI to add comments  D) Ignore as low priority

**2.** An ML model's accuracy drops after six months because user behaviour changed. Which activity addresses this? A) Cyclomatic analysis  B) Code inspection  C) Baselining  D) Model monitoring and retraining (MLOps)

**3.** 85% of the defects discovered so far are in the payment module. Which principle suggests testing it more heavily? A) Defects cluster  B) Pesticide paradox  C) Exhaustive testing impossible  D) Verification ≠ validation

**4.** A control-flow graph has 14 edges, 11 nodes and one component. Cyclomatic complexity is: A) 3  B) 4  C) 5  D) 6

**5.** A field accepts integers 1–100. Using equivalence partitioning, the minimum number of test cases (one per partition) is: A) 2  B) 3  C) 100  D) 5

**6.** Which technique had the highest average defect-detection rate in the slides? A) Unit testing  B) Integration testing  C) Design inspection  D) Code inspection

**7.** Which is the safest way to prevent dangling-else errors? A) More comments  B) Use GOTO  C) Always use braces for if/else blocks  D) Avoid functions

**8.** A 50-developer product with strict release cycles and multiple production versions should prefer: A) GitFlow  B) Trunk-based development  C) No branching  D) Branch per developer only

**9.** The current version is 2.3.7 and only a bug is fixed. The next semantic version is: A) 2.4.0  B) 2.3.8  C) 3.0.0  D) 2.3.7.1

**10.** When Log4Shell was announced, an SBOM helps an organisation mainly to: A) Compile faster  B) Write unit tests  C) Measure velocity  D) Quickly identify which products contain the vulnerable component

**11.** In which approach does a human approve the production release even though all automated tests pass? A) Continuous deployment  B) Chaos engineering  C) Continuous delivery  D) Big-bang integration

**12.** 500 customers are invited to use a pre-release version at home and report issues. This is: A) Beta testing  B) Alpha testing  C) Unit testing  D) Smoke testing

**13.** Randomly terminating production instances to verify failover is called: A) Mutation testing  B) Chaos engineering  C) Soak testing  D) Fault seeding

**14.** A suite kills 15 of 20 mutants. The mutation score is: A) 15%  B) 20%  C) 5%  D) 75%

**15.** A web service runs as root although it only needs to read one folder. Which secure-coding principle is violated? A) Principle of least privilege  B) Default deny  C) Input validation  D) Sanitisation

**16.** To test an order module without charging real credit cards, you should use: A) The real payment gateway  B) A mutant  C) A stub of the payment gateway  D) A baseline

**17.** Static analysis cannot reliably detect all infinite loops because of: A) Pesticide paradox  B) The Halting problem  C) Goodhart's law  D) Defect clustering

**18.** A sprint burndown chart that stays flat for a week indicates: A) Velocity is increasing  B) Cycle time is zero  C) Throughput is high  D) Little or no work was completed

**19.** A build system finds task A depends on B, B on C and C on A. The result is: A) Parallel build  B) Faster build  C) The tasks cannot be topologically ordered (cycle)  D) Incremental build

**20.** A tester cannot reproduce a reported bug. Which defect-lifecycle step is affected? A) Validate  B) Release  C) Close after fix  D) Assign

**21.** A company mandates 100% coverage before merge and gets meaningless tests. This illustrates: A) Moore's law  B) Goodhart's law  C) Amdahl's law  D) Conway's law

**22.** Which element is counted in function-point analysis? A) Number of classes  B) Lines of code  C) Internal logical files  D) Compile time

**23.** Which metric is the inverse of failure rate? A) MTTR  B) Defect density  C) Velocity  D) MTBF

**24.** Man-in-the-middle attacks are best countered by: A) Authentication plus confidentiality (encryption)  B) Logging only  C) Larger passwords only  D) Faster hardware

**25.** Collecting phone numbers when e-mail would suffice violates: A) Branch coverage  B) Data minimisation  C) Soak testing  D) Forward engineering

**26.** In LINDDUN, the first letter L stands for: A) Logging  B) Latency  C) Linking  D) Licensing

**27.** A method has two independent if-else statements (4 branches). Tests exercise 3 of the 4 branches. Branch coverage is: A) 25%  B) 50%  C) 66%  D) 75%

**28.** The Twitter image-cropping case teaches that: A) Intent doesn't excuse impact; test for fairness  B) Biased data is harmless  C) Accuracy alone is enough  D) Fairness testing is optional

**29.** A company ships a modified GPL-licensed kernel in its product. It must: A) Keep modifications secret  B) Pay royalties to the author  C) Make the corresponding source available under GPL  D) Obtain a patent

**30.** Which statement about patents is correct? A) They grant the right to use the invention  B) Independent invention is no defence against infringement  C) They arise automatically  D) They last 5 years

**31.** The MySQL model of offering GPL plus a paid commercial licence is: A) Public domain  B) Creative Commons  C) EULA-only  D) Dual licensing

**32.** Which integration approach builds a skeleton connecting UI to crucial bottom components, then fills in the middle? A) Sandwich  B) Big-bang  C) Bottom-up only  D) Top-down only

**33.** Restarting the system after a power failure and verifying data integrity is: A) Smoke testing  B) Recovery testing  C) Localisation testing  D) Usability testing

**34.** A pentester with only public information about the target performs: A) White-box testing  B) Gray-box testing  C) Black-box testing  D) Mutation testing

**35.** A fuzzer that uses a grammar of the input format to create inputs from scratch is: A) Generation-based  B) Mutation-based  C) A linter  D) A profiler

**36.** Adding a new analytics report after release is which type of maintenance? A) Corrective  B) Adaptive  C) Preventive  D) Perfective

**37.** The first activity of the software re-engineering process model is: A) Forward engineering  B) Inventory analysis  C) Reverse engineering  D) Data refactoring

**38.** Buying cyber-insurance for a risk is an example of risk: A) Avoidance  B) Acceptance  C) Transfer  D) Mitigation by redesign

**39.** Which is true of release testing? A) It is done by an independent team and is validation-oriented black-box testing  B) It is done by the developers to find unit bugs  C) It needs source code  D) It replaces acceptance testing in all cases

**40.** Support staff keep a database of previously encountered defects with their cause and cure mainly to: A) Increase LOC  B) Reduce coverage  C) Delete bugs  D) Diagnose similar defects rapidly

---

# MOCK TEST ANSWER KEY (with one-line reasons)

1-B (security flaw must block merge) · 2-D (monitoring + retraining) · 3-A (defect clustering) · 4-C (14−11+2=5) · 5-B (<1, 1–100, >100) · 6-D (code inspection ≈ 60%) · 7-C (braces) · 8-A (GitFlow) · 9-B (patch increments) · 10-D (find affected components) · 11-C (delivery has manual gate) · 12-A (beta at user site) · 13-B (chaos engineering) · 14-D (15/20=75%) · 15-A (least privilege) · 16-C (stub) · 17-B (halting problem) · 18-D (no work done) · 19-C (cycle) · 20-A (Validate = reproduce/confirm) · 21-B (Goodhart) · 22-C (ILF) · 23-D (failure rate = 1/MTBF) · 24-A (authentication + confidentiality) · 25-B (data minimisation) · 26-C (Linking) · 27-D (3/4) · 28-A (impact over intent) · 29-C (copyleft) · 30-B (patents don't care about independent invention) · 31-D (dual licensing) · 32-A (sandwich) · 33-B (recovery testing) · 34-C (black-box) · 35-A (generation-based) · 36-D (perfective) · 37-B (inventory analysis) · 38-C (transfer) · 39-A (independent validation testing) · 40-D (rapid diagnosis).

---

# COVERAGE CROSS-CHECK AGAINST YOUR SYLLABUS

**Unit 3**

- AI for SE; vibe coding; SE for AI/ML systems → Parts A, B.
- Programming-language choice, factors for effective coding, defensive/secure/testable coding, standards and guidelines, sprint management, code reviews/PRs, understanding codebases, software analysis, bad vs good code, CI/CD, DevSecOps, build system → Parts C, D.
- Principles of testing, test case writing, test measurements, inspection and testing, development, release, user, performance testing, chaos engineering, static and dynamic testing → Part E (test measurements detailed in H.5).
- SCM overview, planning, baselining, versioning, change control, impact analysis, defect management, patching, install, release management, SBOM → Part F.

**Unit 4**

- Measurements and metrics, test measurements, risk management for quality → Part H.
- Security and privacy → Part I; code and branch coverage with tools → Part J; ethics including AI → Part K.
- Open source and software patents → Part L.
- System testing types, penetration testing, fuzzing → Part M; software evolution, maintenance and support → Part N.

**Notes on sourcing:** items tagged **\[Supplement\]** (vibe coding, patch management, impact analysis, risk management, coverage tools, extra metrics, Indian patent law, etc.) are not detailed in your slides; verify against your textbook if your instructor uses different definitions. Where formulas differ across sources (e.g., defect leakage, function-point weights), use the version your instructor gives.
