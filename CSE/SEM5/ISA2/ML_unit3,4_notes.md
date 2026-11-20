# Machine Learning (UE24CS352A) – Exam Notes: Unit 3 & Unit 4

Built from your two lecture PDFs (Unit 3 – Ensembles, Bayesian Learning, EM/GMM, HMM; Unit 4 – Clustering, RL, CNN, Hyperparameter Tuning, MLOps) and your syllabus. Where the slides were thin (e.g., Gradient Boost text, Deep Learning intro, Data/Feature pipelines, Experiment tracking, CI/CD/CT, Orchestration) I have supplemented with standard, accurate material and marked it **\[Supplement\]**.

**Exam pattern (40 marks):** 4 × 4-mark descriptive (2 per unit) + 4 × 2-mark MCQ (2 per unit) + 16 × 1-mark MCQ (8 per unit).

**How to use:** Read the notes → learn the revision sheet → attempt the practice MCQs → attempt the mock exam at the very end *before* looking at the answer key.

**Legend:** ⚠ = MCQ trap / common mistake. ★ = high-probability exam item.

---

# UNIT 3 – BOOSTING, BAYESIAN LEARNING, HMM

## 3.1 Ensemble Learning – Combining Weak Learners

### 3.1.1 Idea and definitions

- **Ensemble learning**: build *multiple* models (base learners) and **combine their predictions** to get a more accurate, more stable model than any single one.
- **Strong learner** (e.g., SVM, deep NN): high accuracy but expensive to train/test.
- **Weak learner** (e.g., decision stump, perceptron): simple, fast, only slightly better than random guessing (low accuracy).
- **Goal:** boost accuracy while keeping individual models simple and fast. *Many cheap weak learners ≈ one strong learner.*
- **Decision stump** = a decision tree with only one split (root only). Too simple to overfit.

**Real-life analogies (from slides):** a second/third medical opinion; a panel of judges; **spam detection** – separate weak checks (suspicious phrases like "earn a prize", image-only mail, sender, excessive CAPS, subject line) each unreliable alone, strong together.

### 3.1.2 How do we get many *different* weak learners?

| Way of creating diversity | Example |
| --- | --- |
| Different **data subsets** (resampling – *most popular*) | Bagging, Boosting |
| Different **features** | Random Forest (random feature subsets) |
| Different **algorithms** | stump + perceptron + kNN |
| Different **hyperparameters** of same algorithm | trees of different depth |
| Different **class labels** | Error-correcting output coding |

**Key requirement:** learners must make **independent, random errors** (be diverse). If all make the same mistakes, combining them gives nothing. It is *not* necessary that they disagree every time – only that the **combined** opinion beats the individual ones.

**How many learners?** Empirically \~**100** is usually enough (depends on problem; fewer often work; bagging gains saturate around \~50 trees on the slide's graph).

### 3.1.3 Combining predictions

1. **Simple majority voting** (mode) for classification / **averaging** (mean) for regression.
2. **Weighted voting** – better models get more weight (used in boosting).

### 3.1.4 Why does an ensemble help? (Bias–variance view)

- Single basic models perform poorly because of **high bias** (too simple) or **high variance** (too sensitive).
- Combined learner is expected to have **low bias AND low variance**.
- **Overfitting is generally not a problem with weak classifiers** (e.g., stumps have no scope to overfit), which helps the combined classifier avoid overfitting.
- Costs: more **algorithmic/model complexity**; **no absolute guarantee** that an ensemble beats a single model.

### 3.1.5 ★ Why confidence increases with the number of models (formula)

Let N models each have accuracy **A** (assumed equal & independent).

- P(one model wrong) = 1 − A
- P(all N wrong) = (1 − A)^N
- **P(at least one of N is correct) = 1 − (1 − A)^N**

*Example:* A = 0.7, N = 5 → 1 − 0.3⁵ = 1 − 0.00243 = **0.9976**. ⚠ In practice learners are not equally accurate/independent, and we use *majority* (not "at least one") – the formula is only to show why confidence rises.

### 3.1.6 Types of ensemble methods

| Type | Manipulates | Examples |
| --- | --- | --- |
| Bagging | data distribution (bootstrap samples) | Bagged trees |
| Boosting | data weights (sequential) | AdaBoost, Gradient Boost |
| Random Forest | data **and** input features | Random Forest |
| ECOC | class labels | Error-correcting output coding |

**In syllabus:** Bagging (as a base for RF), Boosting, Gradient Boost, Random Forest.

---

## 3.2 Bagging (Bootstrap AGGregatING)

**Working:**

1. From the original dataset (n points) create **bootstrap samples** – sampling **with replacement**, usually of the same size n (so duplicates occur).
2. Train one base model on each bootstrap sample (models are **independent → can run in parallel**).
3. **Aggregate**: voting (classification) or averaging (regression).

**★ The 63% / 37% fact:**

- P(a given record is *not* picked in one draw) = 1 − 1/n; over n draws = (1 − 1/n)^n → **1/e ≈ 0.368** for large n.
- So each bootstrap sample contains about **63.2%** of the distinct original records; about **36.8% (≈ 1/3)** are left out.

**Out-of-Bag (OOB) set:** the records *not* chosen for a given bootstrap sample.

- Each OOB record is out-of-bag only for the models that did not see it.
- **OOB error (free validation, no separate test set needed):**
  1. For each record, find all models that did **not** train on it.
  2. Take the **majority vote** of those models; compare with true label.
  3. Compile the error over all records.

**Advantages:** **reduces variance**; makes unstable classifiers (e.g., deep trees) robust; parallelisable; OOB gives built-in error estimate. **Limitations:** does little for high-bias models; loses interpretability; more computation/memory.

---

## 3.3 Boosting

**Idea:** train models **sequentially**, each new model concentrating on the examples the previous models got wrong.

**Working (as in slides):**

1. Fit an initial model (tree / linear regression).
2. Build a second model focused on cases where the first performs poorly.
3. Repeat many times; each successive model corrects the shortcomings of the combined ensemble so far.
4. Each model gets a **performance measure** that decides its **voting right** (better model → more say).

**Instance weights:**

- Every training instance has a weight = *importance of classifying it correctly*.
- After each round: **misclassified → weight ↑; correctly classified → weight ↓**; weights normalised (sum = 1).
- Weights are used either by resampling (more copies of high-weight points) or directly by weight-aware weak learners.

**\[Supplement\] AdaBoost equations (binary, labels ±1):**

```
1. Init w_i = 1/N
2. For t = 1..T:
     train weak learner h_t on weighted data
     error  ε_t = Σ w_i · [h_t(x_i) ≠ y_i]
     weight α_t = ½ · ln((1 − ε_t)/ε_t)      (smaller error → larger α)
     update w_i ← w_i · exp(−α_t · y_i · h_t(x_i)) ; normalise so Σw_i = 1
3. Final: H(x) = sign( Σ α_t · h_t(x) )
```

### ★ Bagging vs Boosting

| Aspect | Bagging | Boosting |
| --- | --- | --- |
| Training | **Parallel**, independent | **Sequential**, dependent |
| Sampling | Uniform, with replacement – every point equal chance | **Weighted** – hard points appear more |
| Combination | Equal vote / average | **Weighted** vote (by performance) |
| Main effect | Reduces **variance** | Reduces **bias** (and some variance) |
| Base learners | Strong/unstable (deep trees) | Weak (stumps) |
| Overfitting/noise | Robust | Can overfit noisy data / outliers |
| Example | Bagged trees, Random Forest | AdaBoost, Gradient Boost |

⚠ MCQ trap: "Boosting can be parallelised" – **False** (sequential). "Bagging reduces bias" – mostly **False** (it reduces variance).

---

## 3.4 Improving Performance with Gradient Boost

**Definition:** Gradient Boosting is an ensemble method that builds trees **sequentially**, where each new tree is fit to the **errors (residuals) of the current ensemble**, and its (scaled) output is **added** to the model. It performs *gradient descent in function space*: for squared-error loss, the **residual y − F(x)** is exactly the **negative gradient** of the loss ½(y − F)² with respect to F – hence "gradient" boost.

### 3.4.1 Gradient Boost vs AdaBoost (slide topic)

|  | AdaBoost | Gradient Boost |
| --- | --- | --- |
| What focuses next learner | **Re-weights samples** (misclassified ↑) | Fits **residuals / negative gradient** of loss |
| Base tree size | Very short trees – **stumps** (1 split) | Larger trees (typically 8–32 leaves) |
| Weight of each tree | Different per tree (**amount of say** α\_t) | **Same learning rate** for every tree |
| Loss | Exponential loss | **Any differentiable loss** (squared, log-loss, …) |
| Starts with | Stump on equal weights | **Constant prediction** (average of target) |

### 3.4.2 ★ Algorithm (regression, squared loss)

```
Input: data (x_i, y_i), learning rate ν (0 < ν ≤ 1, usually 0.1), M trees
Step 1: F0(x) = mean(y)                      # initial leaf = average
For m = 1..M:
  Step 2: r_i = y_i − F_{m−1}(x_i)           # residuals (pseudo-residuals)
  Step 3: fit a regression tree h_m to r_i;  leaf value = average of residuals in that leaf
  Step 4: F_m(x) = F_{m−1}(x) + ν · h_m(x)    # scaled update
  (recompute residuals using F_m and repeat)
Stop at M trees or when residuals stop shrinking.
Final model: F_M(x) = F0 + ν·h_1 + ν·h_2 + … + ν·h_M
```

### 3.4.3 ★ Worked example from the slides (predict Weight)

| Height | Colour | Gender | Weight |
| --- | --- | --- | --- |
| 1.6 | Blue | M | 88 |
| 1.6 | Green | F | 76 |
| 1.5 | Blue | F | 56 |
| 1.8 | Red | M | 73 |
| 1.5 | Green | M | 77 |
| 1.4 | Blue | F | 57 |

1. **Step 1 – initial prediction** = average weight = 427/6 ≈ **71.2** for everyone.
2. **Step 2 – residuals** (observed − predicted): 88−71.2 = **16.8**; 4.8; −15.2; 1.8; 5.8; −14.2.
3. **Step 3 – fit a tree to residuals.** Where a leaf holds several residuals, replace them by their **average** (e.g., (−14.2 + −15.2)/2 = **−14.7**; (1.8 + 5.8)/2 = **3.8**).
4. **Why learning rate?** Predicting 71.2 + 16.8 = 88 fits the training data perfectly → **low bias but high variance (overfit)**. So scale each tree by **ν = 0.1** (value between 0 and 1) to take small steps.
5. **Step 5 – new prediction & residuals** = 71.2 + 0.1 × (tree output). E.g., row 1: 71.2 + 0.1×16.8 = 72.88 → residual 88 − 72.88 ≈ **15.1** (was 16.8). New residuals: 15.1, 4.3, −13.7, 1.4, 5.4, −12.7 – **all smaller in magnitude** → "heading in the right direction".
6. Keep adding trees (+0.1 × tree₂, +0.1 × tree₃, …) until the maximum number of trees or residuals no longer shrink meaningfully.

**Prediction for a new point** = 71.2 + 0.1·(leaf of tree 1) + 0.1·(leaf of tree 2) + …

### 3.4.4 Role of learning rate (shrinkage)

- Small ν → slower learning, needs more trees, but **better generalisation**; large ν → fast but may overfit.
- Trade-off: **learning rate ↔ number of trees**.

**Advantages:** very accurate on tabular data; handles different loss functions; handles mixed features. **Limitations:** sequential (slow to train); sensitive to hyperparameters (trees, depth, ν); can overfit noisy data; less interpretable.

⚠ MCQ traps: Gradient boost's first model is a **constant (mean)**, not a stump. Trees are fit to **residuals, not to the original target**. Every tree shares the **same** learning rate (unlike AdaBoost's per-stump weight).

---

## 3.5 Random Forest (RF)

**Definition:** An **ensemble classifier using many decision trees**; usable for classification (majority vote) and regression (average). Provides **accuracy and variable-importance** information automatically.

**Decision tree vs RF:** one tree = one opinion (high variance); RF = many decorrelated trees voting.

### 3.5.1 ★ Algorithm (slide version)

1. **Bootstrap**: draw a random sample (with replacement) from training data, preserving class distribution.
2. **Random feature subset**: if there are **M** input attributes, choose only **R < M** at random (user-defined) – not all, to limit computation and overfitting and to **decorrelate trees**. *(Standard practice: R ≈ √M for classification, M/3 for regression, chosen afresh at each split.)*
3. Use the chosen attributes to find the **best split** and grow the tree; repeat for each branch until nodes are too small to split (**no pruning**).
4. Repeat to build many trees; **classify by majority vote** (average for regression).
5. Compute **Out-of-Bag error**.

**Heart-disease example from slides:** original data (Chest Pain, Good Blood Circulation, Blocked Arteries, Weight → Heart Disease). Bootstrapped dataset is the same size but with duplicates (e.g., row "Yes, No, Yes, 167" appears twice, row "Yes Yes No 210" is left out → it is **OOB**). At each node only a **random subset (e.g., 2 of 4 features)** is considered, e.g., {Good Blood Circ, …}. Repeating gives many different trees; a new patient is run through all trees and the class with most votes wins.

**OOB error in RF:** about 1/3 of samples are not used per tree. Predict the i-th observation using only trees where it was OOB; compute overall OOB MSE (regression) or classification error.

### 3.5.2 ★ Handling missing data (slide topic)

**Case 1 – missing in training data**

1. **Initial guess:** categorical → **most common value** among rows of the same class; numeric → **median** of those rows (e.g., Weight guessed 167.5).
2. Build a Random Forest with the guessed data.
3. **Proximity matrix:** run *all* samples through all trees; if two samples end in the **same leaf**, add 1 to their proximity; **divide by number of trees** (so values lie in 0–1).
4. **Refine the guess** using proximity as weights:
   - Categorical: **weighted frequency** = (class frequency) × (proximity weight of that class). Slide: Yes → 1/3 × 0.1 = 0.03; No → 2/3 × 0.9 = 0.6 → choose **No**.
   - Numeric: **weighted average** of the column, e.g., 0.1×125 + 0.1×180 + 0.8×210 = **198.5**.
5. Repeat (rebuild forest → recompute) several (\~6–7) times until values stabilise.

**Case 2 – missing in a test sample:** create copies of the sample, one for **each class label**, impute each copy with the above method, run both through the forest and **pick the copy that gets the most correct-looking votes**.

### 3.5.3 Advantages / limitations

| Advantages | Limitations |
| --- | --- |
| No need for **pruning** | Less interpretable than one tree |
| Accuracy and **variable importance** generated automatically | Many trees → memory/time (but parallelisable) |
| Overfitting is generally not a problem (more trees ≠ overfit) | Weaker on very sparse/high-dim text than linear models |
| Not very sensitive to **outliers**; handles missing data | Predictions cannot extrapolate beyond training range (regression) |
| Easy to set parameters; good performance | Biased towards features with many levels |

### 3.5.4 ★ Decision Tree vs Bagging vs Random Forest vs Boosting

|  | Decision Tree | Bagging | Random Forest | Gradient Boost |
| --- | --- | --- | --- | --- |
| # models | 1 | many | many | many |
| Data per model | all | bootstrap | bootstrap | all (residuals) |
| Features per split | all | all | **random subset R\<M** | all |
| Training | – | parallel | parallel | **sequential** |
| Reduces | – | variance | variance (more, decorrelated) | bias |
| Pruning | needed | optional | **not needed** | shallow trees |

⚠ MCQ traps: RF = **bagging + random feature selection**. RF trees are grown **fully (unpruned)**. OOB samples ≈ **1/3** of data. R must be **< M**.

---

## 3.6 Bayesian Learning – Foundations

**Bayesian reasoning** gives a **probabilistic approach to inference**: quantities of interest follow probability distributions, and optimal decisions come from reasoning with probabilities + observed data. It is useful because data is **small, noisy, incomplete** ("why uncertainty?").

- Forms the **basis of algorithms that use probabilities directly** (Naive Bayes, HMM).
- Provides a **framework to analyse other algorithms** even if they don't use probabilities (Find-S, Candidate Elimination, Neural Nets, Decision Trees; e.g., softmax output = probability vector).

### 3.6.1 Probability basics (revise)

| Concept | Statement |
| --- | --- |
| **Event** | A set of outcomes of a random experiment; all subsets of the sample space are events (2ⁿ events for n outcomes). Coin: P(∅)=0, P(H)=P(T)=½, P(H or T)=1 |
| **Random variable** | Assigns a numerical value to each outcome (denoted X, Y, Z) |
| **Conditional probability** | **P(A\|B) = P(A∩B) / P(B)** – once B has occurred, the sample space shrinks to B. Die: P(2) = 1/6 but P(2 \| even) = 1/3 |
| **Addition theorem** | **P(A∪B) = P(A) + P(B) − P(A∩B)**. Two dice, doublet (6/36) or sum 6 (5/36), overlap (3,3) = 1/36 → 10/36 |
| **Multiplication theorem** | Dependent: **P(A∩B) = P(A)·P(B\|A) = P(B)·P(A\|B)**. Independent: P(A∩B)=P(A)P(B). Chain: P(A∩B∩C)=P(A)P(B\|A)P(C\|A∩B) |
| **Independent events** | P(B\|A)=P(B). *Mutually* independent needs pairwise **and** triple conditions |
| **Mutually exclusive** | Cannot occur together: P(A∩B)=0 |
| **Collectively exhaustive** | Union = whole sample space. Both properties together = a **partition** |
| **IID** | Independent and Identically Distributed – same distribution, mutually independent. Many ML algorithms assume it; often violated (time series, sentences, DNA) |
| **Total probability** | If A₁..Aₙ partition the space: **P(B) = Σ P(Aᵢ)·P(B\|Aᵢ)** |

⚠ **Mutually exclusive ≠ independent.** Mutually exclusive events with non-zero probabilities are *dependent* (one occurring rules out the other).

**Total probability examples (slides):**

1. Assignment on time: P(internet)=0.45, P(on time|internet)=0.90, P(on time|no internet)=0.45 (slide used 0.45; question text says 0.42). P(B)=0.45×0.9+0.55×0.45 = **0.6525** (with 0.42 it would be 0.636).
2. Voters: 0.6×0.40 + 0.3×0.65 + 0.1×0.55 = 0.24+0.195+0.055 = **0.49**.

### 3.6.2 ★ Bayes theorem

**P(h | D) = P(D | h) · P(h) / P(D)**   i.e. **Posterior = Likelihood × Prior / Evidence**

| Term | Meaning |
| --- | --- |
| **P(h)** | **Prior** – belief in hypothesis *before* seeing data |
| **P(D)** | **Evidence** – probability of data regardless of hypothesis (normaliser) |
| **P(D\|h)** | **Likelihood** – probability of the data if h is true |
| **P(h\|D)** | **Posterior** – belief in h *after* seeing data |

General form: for mutually exclusive, exhaustive E₁..Eₙ: P(Eᵢ|A) = P(A|Eᵢ)P(Eᵢ) / Σⱼ P(A|Eⱼ)P(Eⱼ).

**Use for classification:** compute posterior of each hypothesis and choose the maximum, or find the most probable *classification* (Bayes optimal).

---

## 3.7 Maximum A Posteriori (MAP) and Maximum Likelihood (ML)

- **h\_MAP = argmax\_h P(h|D) = argmax\_h P(D|h)·P(h)** (P(D) dropped – constant for all h). "A posteriori" = reasoning backward from effect to cause.
- **h\_ML = argmax\_h P(D|h)** – used when **all hypotheses are equally likely a priori**, P(hᵢ)=P(hⱼ).
- **When is h\_MAP = h\_ML?** When priors are uniform.

|  | MAP | ML |
| --- | --- | --- |
| Maximises | Posterior P(h\|D) | Likelihood P(D\|h) |
| Uses prior? | **Yes** | **No** (equal priors) |
| Better when | Prior knowledge available / small data | Lots of data, no prior info |

### ★ Example – cancer test (Mitchell)

Test +ve 98% if cancer, −ve 97% if no cancer; **0.8% (0.008)** of population has cancer; patient tests **positive**.

- P(+|cancer)·P(cancer) = 0.98 × 0.008 = **0.0078**
- P(+|¬cancer)·P(¬cancer) = 0.03 × 0.992 = **0.0298**
- 0.0298 > 0.0078 → **h\_MAP = no cancer**.
- Normalise: P(cancer|+) = 0.0078/(0.0078+0.0298) = **0.21**; P(¬cancer|+) = **0.79**.
- Lesson: **result depends strongly on the prior**; a positive test still leaves only 21% chance of cancer.

### 3.7.1 ★ Maximum Likelihood ⇒ Least-Squared-Error

For continuous targets (linear regression, neural nets, curve fitting): *minimising the sum of squared errors yields the maximum-likelihood hypothesis*, **if noise is Gaussian**.

**Derivation (assumptions: dᵢ = h(xᵢ) + eᵢ, noise eᵢ \~ Normal(0, σ²), independent examples):**

1. h\_ML = argmax Πᵢ p(dᵢ|h) = argmax Πᵢ (1/√(2πσ²)) exp( −(dᵢ − h(xᵢ))² / 2σ² )
2. Take **ln** (monotonic, products → sums): argmax Σ \[ ln(1/√(2πσ²)) − (dᵢ − h(xᵢ))²/2σ² \]
3. The first term is a **constant independent of h** → drop it.
4. Maximising a negative quantity = minimising the positive one; drop constants (1/2σ²).
5. **h\_ML = argmin\_h Σᵢ (dᵢ − h(xᵢ))²**  ← least squared error.

⚠ Valid only for **Gaussian noise** in target values (not in the features). Different noise → different loss.

---

## 3.8 Bayes Theorem in Concept Learning – Brute-Force MAP Learning

**Algorithm:** for every h in H compute P(h|D) = P(D|h)P(h)/P(D); output the h with the highest posterior. Impractical (needs every h) but a **gold standard** to judge other algorithms.

**Simplifying assumptions:** (1) hypothesis space H is finite/conjunctive; (2) training data **noise-free**; (3) **target concept ∈ H**; (4) **equal priors** → **P(h) = 1/|H|**.

**Likelihood:** P(D|h) = **1** if h is consistent with D (h∈ version space VS\_{H,D}), **0** otherwise.

**Posterior:** P(h|D) = 0 if inconsistent; **= 1/|VS\_{H,D}|** if consistent (P(D) = |VS|/|H|).

**Conclusions:** every consistent hypothesis is a MAP hypothesis with the **same posterior**; as data increases, inconsistent hypotheses go to 0 and probability is shared equally among the shrinking version space. **Bayesian justification of Find-S and Candidate-Elimination** (they output consistent hypotheses).

---

## 3.9 ★ Bayes Optimal Classifier and Gibbs Algorithm

**Why not just use h\_MAP?** The most probable *hypothesis* is not always the most probable *classification*. Combine all hypotheses:

**v\_BO = argmax\_{vⱼ ∈ V} Σ\_{hᵢ ∈ H} P(vⱼ|hᵢ) · P(hᵢ|D)**

It gives the most probable classification of a new instance = **prediction of all hypotheses weighted by their posteriors**. No other classifier with the same hypothesis space and prior knowledge can do better on average → **minimum expected error** ("optimal"). **Impractical** because H is huge – every hypothesis must be applied to every test instance.

**Example 1 (3 classes, 4 hypotheses):** P(hᵢ|D) = 0.4, 0.3, 0.2, 0.1; each h gives class probabilities \[v1,v2,v3\]: h1 \[0.5,0.3,0.2\], h2 \[0.1,0.4,0.5\], h3 \[0.5,0.4,0.1\], h4 \[0.1,0.3,0.6\].

- v1: 0.4×0.5+0.3×0.1+0.2×0.5+0.1×0.1 = **0.34**
- v2: 0.4×0.3+0.3×0.4+0.2×0.4+0.1×0.3 = **0.35** ← max
- v3: 0.4×0.2+0.3×0.5+0.2×0.1+0.1×0.6 = **0.31**
- Answer **v2**, though **no single hypothesis ranks v2 first**!

**Example 2 (binary):** P(h₁)=0.4 says +, P(h₂)=0.3 says −, P(h₃)=0.3 says − → P(+)=0.4, P(−)=0.6 → **negative**, although the MAP hypothesis h₁ says +.

**Example 3 (3 NN models for sentiment):** accuracies 90, 80, 60 → normalised weights 0.39, 0.34, 0.26. P(+|h)=0.8, 0.4, 0.2 and P(−|h)=0.2, 0.6, 0.8. P(+)=0.8×0.39+0.4×0.34+0.2×0.26 = **0.50**; P(−)= 0.2×0.39+0.6×0.34+0.8×0.26 = **0.49** → **positive**.

### Gibbs Algorithm (cheaper alternative)

1. **Randomly pick one h from H according to the posterior P(h|D).**
2. Use that h to classify the new instance.

Under certain conditions: **expected error(Gibbs) ≤ 2 × expected error(Bayes Optimal)**.

|  | Bayes Optimal | Gibbs | MAP classifier |
| --- | --- | --- | --- |
| Uses | All hypotheses, weighted | One random hypothesis \~ posterior | Single best hypothesis |
| Prediction | Deterministic | **Stochastic** | Deterministic |
| Error | Lowest possible | ≤ 2× optimal | Can be worse than BO |
| Cost | Very high | Low | Low |

---

## 3.10 ★★ Naïve Bayes Classifier

**Setting:** each instance x = ⟨a₁,…,aₙ⟩ (conjunction of attribute values); target f(x) takes values in finite set V (binary or multiclass).

**Derivation:** v\_MAP = argmax\_{vⱼ} P(vⱼ | a₁..aₙ) = argmax P(a₁..aₙ | vⱼ)P(vⱼ). Estimating the joint P(a₁..aₙ|vⱼ) needs exponentially many probabilities. **Naïve assumption: attributes are conditionally independent given the class:**

**v\_NB = argmax\_{vⱼ} P(vⱼ) · Πᵢ P(aᵢ | vⱼ)**

⚠ Independence is **conditional on the class**, not unconditional. The "naive" part = this assumption; it is often false but NB still works well (comparable to NN/decision trees in some domains, and good with small data).

### 3.10.1 Learning = counting (no search)

- P(vⱼ) = frequency of class in training data; P(aᵢ|vⱼ) = count(aᵢ, vⱼ)/count(vⱼ).
- The hypothesis = the set of estimated probabilities; formed **without searching**, by counting frequencies.

### 3.10.2 ★ Number of parameters

n features, v values per feature, k classes:

|  | Full joint (dependent) | Naïve Bayes |
| --- | --- | --- |
| Free parameters | **k(vⁿ − 1) + (k − 1)** | **k·n(v − 1) + (k − 1)** |
| Growth | exponential in n | **linear** in n |
| n=2, v=2, k=2 | 2(4−1)+1 = **7** | 2·2·1+1 = **5** |

(k−1 priors because they sum to 1; v−1 per feature value distribution because they sum to 1.)

### 3.10.3 Types of Naïve Bayes

| Type | Feature type | Use |
| --- | --- | --- |
| **Gaussian** | Continuous, assumed normal: P(x\|c)=N(x;μ\_c,σ\_c²) | Real-valued data |
| **Multinomial** | Discrete **counts** (word frequency) | Text classification |
| **Bernoulli** | **Binary** (word present/absent) | Short text, bag-of-words 0/1 |

("Linear Naive Bayes" does not exist – a quiz favourite.)

### 3.10.4 ★ Zero-frequency problem and smoothing

If an attribute value never occurs with a class in training, P(aᵢ|vⱼ)=0 and the **whole product becomes 0**.

- **m-estimate:** **P(aᵢ|vⱼ) = (n\_c + m·p) / (n + m)**, n = examples with class vⱼ, n\_c = those with aᵢ, p = prior estimate (uniform: p = 1/#values), m = equivalent sample size (virtual rows; designer's choice, not too large).
- **Laplace (add-one) smoothing:** m = number of possible values, p = 1/m → **(n\_c + 1)/(n + m)**.
- Text: **P(w|c) = (count(w,c)+1) / (total words in c + |V|)**, where **V = vocabulary of all words in all classes**.

### 3.10.5 Numerical underflow

Multiplying many numbers < 1 (100+ features) underflows to 0. **Fix: work in log space** – products become sums: argmax \[ log P(vⱼ) + Σ log P(aᵢ|vⱼ) \] (or scale values up).

### 3.10.6 ★ Worked example – Play Tennis

New day: **Outlook=Sunny, Temp=Cool, Humidity=High, Wind=Strong.** 14 samples: 9 Yes, 5 No.

| Attribute value | P(·\|Yes) | P(·\|No) |
| --- | --- | --- |
| Sunny | 2/9 | 3/5 |
| Cool | 3/9 | 1/5 |
| High | 3/9 | 4/5 |
| Strong | 3/9 | 3/5 |

- Yes: 9/14 × 2/9 × 3/9 × 3/9 × 3/9 = **≈ 0.0053**
- No: 5/14 × 3/5 × 1/5 × 4/5 × 3/5 = **≈ 0.0206**
- 0.0206 > 0.0053 → **PlayTennis = No** (normalised ≈ 0.80 No).

### 3.10.7 Worked example – Text classification (slides)

**Bag-of-Words (BoW):** word order ignored; every word equal weight; position-independent. Training: Campaigning = {inspiring address; aggressive speech; speech coward}; Law&Order = {cowardly act; threatening speech; act aggression}. Test D7 = "aggression threatening address". Without smoothing P(threatening|Campaigning)=0 → product 0; P(address|Law&Order)=0 → also 0 (useless). With **Laplace** (class word totals 6+7=13):

- Campaigning: 2/13 × 1/13 × 2/13 × 3/6
- Law&Order: 2/13 × 2/13 × 1/13 × 3/6
- **Equal → tie** (NB returns arbitrary class). A tie shows how smoothing can still leave an ambiguous case.

**Sports example:** training: "A great game"(sports), "election is over"(not), "very clean match"(sports), "a clean but forgettable game"(sports), "it was a close election"(not). P(sports)=3/5, P(not)=2/5; sports has 11 words, not-sports 9; |V| = 14. Classify "a very close game":

- Sports: (3/25)(2/25)(1/25)(3/25)×0.6 = **2.76×10⁻⁵**
- Not sports: (2/23)(1/23)(2/23)(1/23)×0.4 = **5.72×10⁻⁶**
- → **Sports**. Note the full sentence never appears in training – independence lets us use individual word probabilities.

**Text-pre-processing notes:** **unseen words** in test data → ignore; **stop words** – often better to keep the whole vocabulary; **stemming** (election/elected → one word); **n-grams** (word sequences); **TF-IDF**: TF = (occurrences in doc)/(words in doc), IDF = log(#documents / #documents containing the word).

### 3.10.8 Advantages / limitations / applications

| Advantages | Limitations |
| --- | --- |
| Simple, fast, easy to implement | **Conditional independence rarely true** |
| Works with **small data**; few parameters (linear in n) | Zero-frequency problem needs smoothing |
| Handles multiclass, discrete & continuous features | Probability estimates (not just ranking) can be poor |
| Scales to high-dimensional text | Cannot learn feature interactions |

**Applications:** spam filtering, sentiment analysis, document categorisation, medical diagnosis, real-time prediction.

---

## 3.11 ★ Expectation Maximization (EM)

**Problem:** estimate model parameters by maximum likelihood when some variables are **hidden / latent / missing** (e.g., which Gaussian generated a point). It is a "chicken-and-egg" problem: if we knew the hidden values, estimating parameters is easy; if we knew parameters, estimating hidden values is easy.

**EM = iterate two steps** (start from a random/initial parameter guess θ):

| Step | What it does |
| --- | --- |
| **E-step (Expectation)** | Using current parameters θ and observed data, **estimate the latent variables** (their expected values / posterior probabilities) |
| **M-step (Maximization)** | Treat those estimates as if they were the real missing data and **maximise the likelihood** to get new parameters |

Repeat until parameters/likelihood stop changing (**convergence**).

**Properties:** likelihood **never decreases** each iteration; converges to a **local** (not necessarily global) maximum → depends on initialisation; each iteration is cheap; generic framework (GMM, K-means, **Baum-Welch for HMMs** are all EM instances).

---

## 3.12 ★★ Gaussian Mixture Models (GMM)

### 3.12.1 Background – Gaussian (Normal) distribution

- **Univariate** N(μ, σ²): p(x) = (1/√(2πσ²)) exp(−(x−μ)²/2σ²). ML estimates: **μ = (1/n)Σxᵢ**, **σ² = (1/n)Σ(xᵢ−μ)²** (set partial derivatives of log-likelihood to 0).
- **Multivariate** (d dimensions): p(x) = 1/((2π)^{d/2}|Σ|^{1/2}) · exp(−½ (x−μ)ᵀ Σ⁻¹ (x−μ)); μ is a d-vector, **Σ is a d×d covariance matrix**. ML: μ = sample mean, Σ = sample covariance.

### 3.12.2 Model

Data come from **K Gaussians**, each with mean μₖ, covariance Σₖ and **mixing proportion (prior) πₖ** (Σπₖ = 1):

**p(x) = Σ\_{k=1..K} πₖ · N(x | μₖ, Σₖ)**

**Generative story:** pick a Gaussian k with probability πₖ → draw x from it. Each point comes from exactly one Gaussian, but **which one is unknown**. Introduce **latent Boolean vector** zᵢ = (zᵢ₁..zᵢₖ) with exactly one 1.

If labels were known: μⱼ = mean of points labelled j. Labels unknown → **use EM**.

### 3.12.3 ★ EM for GMM

- **E-step – responsibilities** (soft assignment): **γᵢⱼ = E\[zᵢⱼ\] = πⱼ N(xᵢ|μⱼ,Σⱼ) / Σₖ πₖ N(xᵢ|μₖ,Σₖ)** – a number in \[0,1\], not 0/1. With equal known variance, N ∝ exp(−(x−μ)²/2σ²).
- **M-step (weighted ML estimates):**
  - **μⱼ = Σᵢ γᵢⱼ xᵢ / Σᵢ γᵢⱼ**
  - **Σⱼ = Σᵢ γᵢⱼ (xᵢ−μⱼ)(xᵢ−μⱼ)ᵀ / Σᵢ γᵢⱼ**
  - **πⱼ = Eⱼ / n**, where Eⱼ = Σᵢ γᵢⱼ ("effective size" of cluster j)
- Repeat until the Gaussians stop changing. Points "are not completely blue or yellow – they belong to both with some probability < 1" → **soft clustering**.
- Derivation idea: set ∂(log-likelihood)/∂μⱼ = 0 → μⱼ = Σ zᵢⱼ xᵢ/Σ zᵢⱼ (weighted sample mean). Priors don't interact with other parameters; maximising ΣEⱼ ln πⱼ subject to Σπ=1 gives πⱼ = Eⱼ/n.

### 3.12.4 ★ Numerical example (from slides)

K=2, univariate, same variance σ² = ½ (so weight = exp(−(x−μ)²)), **data = 2, 4, 7**, priors (0.5, 0.5), initial **μ₁=3, μ₂=6**.

**E-step:**

- x=7: exp(−(7−3)²)=exp(−16)=1.1×10⁻⁷; exp(−(7−6)²)=0.368 → **z₃₁ ≈ 0, z₃₂ ≈ 1**
- x=4: exp(−1)=0.368; exp(−4)=0.0183 → **z₂₁ = 0.953, z₂₂ = 0.047**
- x=2: **z₁₁ ≈ 1.0, z₁₂ ≈ 0**

**M-step:**

- μ₁ = (1×2 + 0.953×4 + 0×7)/(1+0.953) = **≈ 2.98**
- μ₂ = (0×2 + 0.047×4 + 1×7)/(0.047+1) = **≈ 6.87**
- Priors: E₁ = 1.953, E₂ = 1.047 → **π₁ = 0.651, π₂ = 0.349**.

**Effect of priors (slide):** change μ₂ to 5 and x₃ to 4.1: weights exp(−1.21)=0.298 (μ=3) and exp(−0.81)=0.445 (μ=5) → z₃₁=0.4, z₃₂=0.6 with equal priors. With π₁=0.9, π₂=0.1 → z₃₁ = 0.9×0.298/(0.9×0.298+0.1×0.445) = **0.86** (flips!). With π = 0.6/0.4 → 0.501 / 0.499.

### 3.12.5 GMM vs K-means (preview – see Unit 4)

|  | K-means | GMM (EM) |
| --- | --- | --- |
| Assignment | **Hard** (0/1) | **Soft** (probabilities) |
| Cluster shape | Spherical | Elliptical (covariance Σₖ) |
| Parameters | Centroids | μₖ, Σₖ, πₖ |
| Cluster sizes | Implicit equal | Different priors πₖ |

---

## 3.13 Hidden Markov Models (HMM)

### 3.13.1 From IID to Markov chains

**IID fails** for time series, speech, text, DNA. A **stochastic process** is a collection of random variables indexed by time over a common state space. A **Markov process** obeys the **Markov property:** *the future depends only on the present state, not on the past:* **P(qₜ | qₜ₋₁,…,q₁) = P(qₜ | qₜ₋₁)**.

**Markov chain** = discrete-time Markov process with: **set of states Q**, **initial distribution π** (πᵢ = P(q₁=i)), **N×N transition matrix A** (aᵢⱼ = P(next=j | current=i); each row sums to 1).

- **Sequence likelihood:** P(q₁..qₜ) = π\_{q₁} · Π a\_{qₜ₋₁,qₜ}.
- **Finding parameters from data (ML counting):** aᵢⱼ = count(i→j)/count(i→any); πᵢ = fraction of sequences starting in i. Slide example (Sunny/Cloudy/Windy): rows Sunny = (2/15, 4/15, 9/15), Cloudy = (9/19, 4/19, 6/19), Windy = (9/22, 12/22, 1/22) – each row sums to 1.
- Uses: weather, stock states, patient health progression, machine failure, vehicle trajectory.

### 3.13.2 HMM definition

In a Markov chain all states are visible. In an **HMM the states are hidden; only observations are visible.** An HMM is a Markov process that, at each time step, **emits a symbol from an alphabet according to an emission probability that depends on the (hidden) state.**

**Parameters λ = (A, B, π):**

| Symbol | Meaning |
| --- | --- |
| N | number of hidden states |
| M | number of distinct observation symbols |
| **A** (N×N) | transition probabilities aᵢⱼ = P(qₜ₊₁=Sⱼ \| qₜ=Sᵢ) |
| **B** (N×M) | emission probabilities bⱼ(k) = P(oₜ=vₖ \| qₜ=Sⱼ) |
| **π** (N) | initial state distribution |

**Emission probabilities** show the observations are **stochastic** – the same hidden state can produce different observations (e.g., "mild illness" → low fever 0.6, headache 0.3, no symptom 0.1), which gives the model realistic flexibility.

**Two assumptions:**

1. **Markov assumption:** P(qₜ|qₜ₋₁, …) = P(qₜ|qₜ₋₁).
2. **Output independence:** P(oₜ|q₁..qₜ, o₁..oₜ₋₁) = P(oₜ|qₜ) – observation depends **only on the current state**.

### 3.13.3 ★ Running example (slides): Sunny/Rainy hidden, Happy/Grumpy observed

- π(S)=2/3, π(R)=1/3
- A: S→S 0.8, S→R 0.2; R→S 0.4, R→R 0.6
- B: Sunny: Happy 0.8, Grumpy 0.2; Rainy: Happy 0.4, Grumpy 0.6
- Observation sequence **O = (Happy, Grumpy, Happy)**.

### 3.13.4 ★ The three fundamental problems

| # | Problem | Question | Algorithm | Complexity |
| --- | --- | --- | --- | --- |
| 1 | **Likelihood** (evaluation) | P(O \| λ)? | **Forward** (or Backward) | **O(N²T)** |
| 2 | **Decoding** | Best hidden state sequence Q\* given O? | **Viterbi** (DP) | **O(N²T)** |
| 3 | **Learning** (training) | Find λ maximising P(O\|λ) | **Baum-Welch** (EM) | O(N²T) per iteration |

### 3.13.5 Problem 1 – Likelihood

**Brute force:** sum P(O, Q∣λ) over all **Nᵀ** state sequences. Each sequence needs \~2T multiplications → **O(T·Nᵀ)** – exponential. For N=2, T=3: 8 sequences (SSS, SSR, …, RRR); e.g., term = π(S)·b\_S(H)·a\_SS·b\_S(G)·a\_SS·b\_S(H).

**Key idea of Forward algorithm:** many terms repeat → **dynamic programming**; at each time step compress all history into **N numbers αₜ(j)**.

- **αₜ(j) = P(o₁..oₜ, qₜ = Sⱼ | λ)** (observed first t symbols AND now in state j).
- **Initialisation:** α₁(j) = πⱼ · bⱼ(o₁)
- **Recursion:** **αₜ(j) = \[ Σᵢ αₜ₋₁(i) · aᵢⱼ \] · bⱼ(oₜ)**
- **Termination:** **P(O|λ) = Σⱼ α\_T(j)**
- Each α needs N multiplications for the sum + 1 → N terms per step → O(N²) per step → **O(N²T)** total (vs O(T·Nᵀ)).

**★ Forward example:**

- t=1 (Happy): α₁(S)=2/3×0.8 = **0.5333**; α₁(R)=1/3×0.4 = **0.1333**
- t=2 (Grumpy): α₂(S)=(0.5333×0.8+0.1333×0.4)×0.2 = **0.096**; α₂(R)=(0.5333×0.2+0.1333×0.6)×0.6 = **0.112**
- t=3 (Happy): α₃(S)=(0.096×0.8+0.112×0.4)×0.8 = **0.09728**; α₃(R)=(0.096×0.2+0.112×0.6)×0.4 = **0.03456**
- **P(O|λ) = 0.09728 + 0.03456 = 0.13184 ≈ 0.1318.** (Slide table prints 0.3456 for α₃(R) – a typo; correct is 0.03456.)

**Backward algorithm:** **βₜ(i) = P(oₜ₊₁..o\_T | qₜ = Sᵢ, λ)**.

- Init: **β\_T(i) = 1**
- Recursion: **βₜ(i) = Σⱼ aᵢⱼ · bⱼ(oₜ₊₁) · βₜ₊₁(j)**
- Termination: **P(O|λ) = Σⱼ πⱼ · bⱼ(o₁) · β₁(j)**
- Example: β₃ = (1,1); β₂(S)=0.8×0.8+0.2×0.4 = **0.72**; β₂(R)=0.4×0.8+0.6×0.4 = **0.56**; β₁(S)=0.8×0.2×0.72+0.2×0.6×0.56 = **0.1824**; β₁(R)=0.4×0.2×0.72+0.6×0.6×0.56 = **0.2592**.

**★ Important result (any t):** **P(O|λ) = Σⱼ αₜ(j)·βₜ(j)** (dot product of the α and β columns at the same t). At t=1: 0.5333×0.1824 + 0.1333×0.2592 = 0.09728 + 0.03456 = **0.1318** ✓ (same answer).

### 3.13.6 Problem 2 – Decoding: Viterbi

**Brute force:** compute P(O,Q∣λ) for all Nᵀ sequences and take **argmax** (instead of the sum used in likelihood) → exponential.

**Viterbi = same trellis as Forward but with MAX instead of SUM, plus back-pointers.**

- **δₜ(j)** = probability of the **best path** ending in state j at time t having produced o₁..oₜ.
- Init: **δ₁(j) = πⱼ bⱼ(o₁)**, ψ₁(j)=0
- Recursion: **δₜ(j) = maxᵢ \[ δₜ₋₁(i) aᵢⱼ \] · bⱼ(oₜ)**; **back-pointer ψₜ(j) = argmaxᵢ \[δₜ₋₁(i) aᵢⱼ\]**
- Termination: P\* = maxⱼ δ\_T(j); q\*\_T = argmaxⱼ δ\_T(j)
- **Backtracking:** q*ₜ = ψₜ₊₁(q*ₜ₊₁)
- **Complexity O(N²T).**

**★ Viterbi example (H, G, H):**

|  | t=1 (H) | t=2 (G) | t=3 (H) |
| --- | --- | --- | --- |
| Sunny δ | 8/15 = 0.5333 | max(0.5333×0.8, 0.1333×0.4)×0.2 = **0.0853** (32/375), from Sunny | max(0.0853×0.8, 0.064×0.4)×0.8 = **0.0546** (512/9375), from Sunny |
| Rainy δ | 2/15 = 0.1333 | max(0.5333×0.2, 0.1333×0.6)×0.6 = **0.064** (8/125), from Sunny | max(0.0853×0.2, 0.064×0.6)×0.4 = **0.01536** (144/9375), from Rainy |

Best final state = Sunny (0.0546) → backtrack → **Q* = Sunny → Sunny → Sunny*\*.

⚠ Forward **sums** over paths (gives total likelihood); Viterbi **maximises** (gives best path and its probability). Viterbi probability ≤ forward probability (0.0546 < 0.1318).

### 3.13.7 Problem 3 – Learning: Baum-Welch (special case of EM)

Given observation sequence(s) and N, find λ=(A,B,π). States are hidden → use **EM**: start with an estimate of A, B, π, compute expected counts, re-estimate; iterate.

- **γₜ(j) = P(qₜ=Sⱼ | O, λ) = αₜ(j)βₜ(j) / P(O|λ)** – probability of being in state j at time t given whole sequence.
- **ξₜ(i,j) = P(qₜ=Sᵢ, qₜ₊₁=Sⱼ | O, λ) = αₜ(i) aᵢⱼ bⱼ(oₜ₊₁) βₜ₊₁(j) / P(O|λ)**
- **E-step:** compute α, β → γ, ξ (expected counts).
- **M-step (re-estimation):**
  - **āᵢⱼ = Σₜ ξₜ(i,j) / Σₜ γₜ(i)** (expected transitions i→j / expected visits to i)
  - **b̄ⱼ(k) = Σ\_{t: oₜ=vₖ} γₜ(j) / Σₜ γₜ(j)**
  - **π̄ᵢ = γ₁(i)**
- Iterate until P(O|λ) converges (local maximum). For **K sequences** sum the numerators/denominators across sequences.

### 3.13.8 Typical HMM workflow & practical issues

1. Define the problem (is HMM suitable?). 2. Decide the observation sequence. 3. Prepare training/validation sequences. 4. Decide meaning and **number of states** (no unique answer; domain knowledge, trial & error). 5. **Train** (Problem 3). 6. **Validate** (Problem 1). 7. Iterate. 8. Use to classify/decode new data (Problems 1/2).

- **Numerical instability** (underflow) → use **log-probabilities** (or scaling).
- Architecture (fully connected vs left-to-right) is the designer's choice.
- **Applications:** speech/word recognition, POS tagging, DNA/gene finding, gait & facial-expression recognition, text modelling, patient progression.

### 3.13.9 Markov chain vs HMM

|  | Markov Chain | HMM |
| --- | --- | --- |
| States | Directly **observable** | **Hidden** |
| Parameters | Q, π, A | **A, B, π** |
| Output | State sequence itself | Observation sequence emitted from states |
| Extra assumption | Markov | Markov + **output independence** |
| Learning | Counting | Baum-Welch (EM) |

---

## 3.14 Unit 3 – Important Definitions, Facts, Formulae & Algorithms

| Term | One-line definition / formula |
| --- | --- |
| Weak learner | Simple model slightly better than chance (stump, perceptron) |
| Ensemble | Combination of multiple models' predictions |
| Bootstrap sample | Sample of size n **with replacement**; \~63.2% distinct points |
| OOB | Points left out of a bootstrap (\~36.8% ≈ 1/e) – used for free error estimate |
| P(all N models wrong) | (1 − A)^N |
| Boosting | Sequential; reweights data/fits errors; weighted vote |
| Residual | y − F(x) = negative gradient of squared loss |
| Gradient boost update | F\_m = F\_{m−1} + ν·h\_m ; F₀ = mean(y) |
| Random Forest | Bagging + random subset of R < M features; unpruned trees; vote |
| Proximity | Fraction of trees in which two samples land in same leaf |
| Conditional prob. | P(A\|B) = P(A∩B)/P(B) |
| Total probability | P(B) = Σ P(Aᵢ)P(B\|Aᵢ) |
| Bayes theorem | P(h\|D) = P(D\|h)P(h)/P(D) |
| h\_MAP | argmax P(D\|h)P(h) |
| h\_ML | argmax P(D\|h) (equal priors) |
| h\_ML (Gaussian noise) | argmin Σ (dᵢ − h(xᵢ))² |
| Brute-force MAP | P(h\|D) = 1/\|VS\| for consistent h, else 0 |
| Bayes Optimal | argmax\_v Σ\_h P(v\|h)P(h\|D) |
| Gibbs | Sample h \~ P(h\|D); error ≤ 2× BO error |
| Naïve Bayes | argmax P(v)ΠP(aᵢ\|v); conditional independence |
| NB parameters | k·n(v−1) + (k−1) |
| Laplace | (count+1)/(n + #values); text: (count+1)/(N\_c + \|V\|) |
| m-estimate | (n\_c + m·p)/(n + m) |
| EM | E-step: expected latent values; M-step: maximise likelihood |
| GMM responsibility | γᵢⱼ = πⱼN(xᵢ\|μⱼ,Σⱼ)/ΣₖπₖN(xᵢ\|μₖ,Σₖ) |
| GMM M-step | μⱼ = Σγx/Σγ ; πⱼ = Σγ/n |
| Markov property | P(qₜ\|past) = P(qₜ\|qₜ₋₁) |
| HMM | λ = (A, B, π) |
| Forward | αₜ(j) = \[Σᵢαₜ₋₁(i)aᵢⱼ\]bⱼ(oₜ); P(O) = Σα\_T |
| Backward | βₜ(i) = Σⱼaᵢⱼbⱼ(oₜ₊₁)βₜ₊₁(j); β\_T = 1 |
| Viterbi | δₜ(j) = maxᵢ\[δₜ₋₁(i)aᵢⱼ\]bⱼ(oₜ) + back-pointers |
| Baum-Welch | EM for HMM using γ and ξ |
| Complexities | Brute force O(T·Nᵀ); Forward/Viterbi O(N²T) |

## 3.15 Unit 3 – Frequently Confused Concepts

| Confusion | Clarification |
| --- | --- |
| Bagging vs Boosting | Parallel/variance vs sequential/bias |
| AdaBoost vs Gradient Boost | Re-weight samples + stumps vs fit residuals + learning rate |
| Bagging vs Random Forest | RF adds **random feature subsets** |
| Prior vs Likelihood vs Posterior | P(h) vs P(D\|h) vs P(h\|D) |
| MAP vs ML | MAP uses prior; ML assumes equal priors |
| MAP hypothesis vs Bayes optimal classification | Single best h vs weighted vote of all h (BO can disagree with MAP) |
| Bayes optimal vs Gibbs | All hypotheses vs one sampled; Gibbs ≤ 2× error |
| Independent vs mutually exclusive | Independent: P(A∩B)=P(A)P(B); exclusive: P(A∩B)=0 |
| NB independence | **Conditional** on class |
| Forward vs Viterbi | Sum vs max (+ back-pointers) |
| Likelihood vs Decoding vs Learning | P(O\|λ) / best states / best λ |
| EM in GMM vs K-means | Soft vs hard assignment |
| Markov chain vs HMM | Visible vs hidden states |
| Transition vs emission probability | State→state vs state→observation |

## 3.16 Unit 3 – Potential 4-Mark Questions with Model Answers

**Q1. Explain Bagging and Boosting and differentiate between them. (4)** *Answer:* **Bagging** (Bootstrap Aggregating) draws several bootstrap samples (with replacement, same size as the data), trains an independent weak/unstable model on each (in parallel) and combines by majority vote or averaging; \~63% of points are in each sample, the rest (\~37%) are OOB and give a free error estimate. It mainly **reduces variance**. **Boosting** trains models **sequentially**; after each model, weights of misclassified instances are increased and correct ones decreased so the next learner focuses on hard cases; models are combined by a **weighted vote** where better models get more say. It mainly **reduces bias**. *Differences:* parallel vs sequential; equal vs weighted sampling; equal vs weighted vote; variance vs bias reduction; boosting is more prone to noise/outliers. *(1 mark each: bagging, boosting, 2 marks for 4 differences.)*

**Q2. Explain Gradient Boosting with a numerical illustration. How does it differ from AdaBoost? (4)** *Answer:* Gradient boosting builds an additive model F\_m = F\_{m−1} + ν·h\_m. Steps: (1) F₀ = average of target (e.g., weights 88,76,56,73,77,57 → 71.2); (2) residuals = actual − predicted (16.8, 4.8, −15.2, 1.8, 5.8, −14.2); (3) fit a regression tree to residuals, leaf = average of residuals (e.g., 3.8, −14.7); (4) new prediction = old + ν×tree output with ν = 0.1 (row 1: 71.2 + 0.1×16.8 = 72.88, residual 15.1 < 16.8); (5) repeat for M trees. The learning rate prevents overfitting (low bias/high variance if ν=1). *Vs AdaBoost:* AdaBoost uses stumps, re-weights samples and gives each stump its own weight; gradient boost uses larger trees fitted to residuals (negative gradient of any differentiable loss) with a common learning rate.

**Q3. Describe the Random Forest algorithm. How are OOB error and missing values handled? (4)** *Answer:* (i) Draw bootstrap sample; (ii) at each split consider only R\<M random features; (iii) grow tree fully (no pruning); (iv) repeat to get many trees; (v) predict by majority vote (average for regression). **OOB error:** \~1/3 of data not in a tree's sample; each observation is predicted only by trees where it was OOB and the error aggregated – no separate validation set needed. **Missing values:** make an initial guess (most common category / median), build the forest, compute the **proximity matrix** (fraction of trees in which two samples share a leaf), and re-estimate using proximity-weighted frequency (categorical) or weighted average (numeric); iterate until stable. Advantages: no pruning, variable importance, robust to outliers.

**Q4. State Bayes theorem. Explain MAP and ML hypotheses. Using the cancer-test example (prior 0.008, P(+|cancer)=0.98, P(+|¬cancer)=0.03) find the diagnosis. (4)** *Answer:* P(h|D)=P(D|h)P(h)/P(D). h\_MAP = argmax P(D|h)P(h); h\_ML = argmax P(D|h) (equal priors). Example: cancer: 0.98×0.008=0.00784; no cancer: 0.03×0.992=0.02976 → h\_MAP = **no cancer**; normalised P(cancer|+)=0.21, P(¬cancer|+)=0.79. The posterior depends strongly on the prior.

**Q5. Show that the maximum-likelihood hypothesis under Gaussian noise minimises the sum of squared errors. (4)** *Answer:* Let dᵢ = h(xᵢ)+eᵢ, eᵢ\~N(0,σ²). h\_ML = argmax Π (1/√(2πσ²))exp(−(dᵢ−h(xᵢ))²/2σ²). Taking ln (monotonic) gives Σ\[ln(1/√(2πσ²)) − (dᵢ−h(xᵢ))²/2σ²\]. The first term and 1/2σ² are constants independent of h and dropped; maximising a negative quantity = minimising its positive: **h\_ML = argmin Σ(dᵢ−h(xᵢ))².** Hence NN/linear regression that minimise squared error output the ML hypothesis (given Gaussian noise).

**Q6. Explain the Bayes Optimal Classifier with an example. Why is it impractical and what is the Gibbs algorithm? (4)** *Answer:* BO outputs argmax\_v Σ\_h P(v|h)P(h|D). Example: P(h)=0.4,0.3,0.3; h₁ says +, h₂,h₃ say − → P(+)=0.4, P(−)=0.6 → − although MAP hypothesis says +. It has lowest expected error but must evaluate every hypothesis → impractical. **Gibbs:** pick one h at random according to P(h|D) and classify with it; expected error ≤ 2× that of BO.

**Q7. Explain the Naïve Bayes classifier. Why "naïve"? How are the zero-frequency and underflow problems solved? (4)** *Answer:* v\_NB = argmax P(vⱼ)ΠP(aᵢ|vⱼ); estimates by counting. "Naïve" = attributes assumed **conditionally independent given class**, reducing parameters from k(vⁿ−1)+(k−1) to k·n(v−1)+(k−1). **Zero-frequency:** unseen value gives zero product → use Laplace/m-estimate smoothing (n\_c+mp)/(n+m). **Underflow:** product of many small numbers → use sum of log-probabilities.

**Q8. Classify (Sunny, Cool, High, Strong) with Naïve Bayes (PlayTennis). (4)** *Answer:* P(Yes)∏ = 9/14·2/9·3/9·3/9·3/9 = 0.0053; P(No)∏ = 5/14·3/5·1/5·4/5·3/5 = 0.0206 → **No**.

**Q9. Explain EM and apply it to a Gaussian Mixture Model (E and M steps). (4)** *Answer:* Latent zᵢⱼ indicates which Gaussian produced xᵢ. E-step: γᵢⱼ=πⱼN(xᵢ|μⱼ,Σⱼ)/ΣₖπₖN(xᵢ|μₖ,Σₖ). M-step: μⱼ=Σγx/Σγ, Σⱼ=Σγ(x−μ)(x−μ)ᵀ/Σγ, πⱼ=Σγ/n. Iterate until convergence (local maximum). Soft clustering. *Numerical:* data 2,4,7; μ=(3,6) → γ₂₁=0.953 etc. → μ₁≈2.98, μ₂≈6.87, π=(0.651,0.349).

**Q10. What is an HMM? Define its components, assumptions and three problems. (4)** *Answer:* HMM = Markov process with hidden states that emit observations. λ=(A,B,π). Assumptions: Markov and output independence. Problems: likelihood (Forward/Backward), decoding (Viterbi), learning (Baum-Welch/EM).

**Q11. Explain the Forward algorithm and why it is efficient; compute P(O) for the Happy-Grumpy-Happy example. (4)** *Answer:* Brute force sums over Nᵀ paths (O(T·Nᵀ)). Forward compresses history into αₜ(j) (O(N²T)). Init α₁=πb(o₁), recursion, termination Σα\_T. Example: α₁=(0.5333,0.1333), α₂=(0.096,0.112), α₃=(0.09728,0.03456) → **P(O)=0.1318**.

**Q12. Explain the Viterbi algorithm with an example. How does it differ from Forward? (4)** *Answer:* δₜ(j)=maxᵢ\[δₜ₋₁(i)aᵢⱼ\]bⱼ(oₜ) with back-pointers; end at argmaxδ\_T and backtrack. Example result: Sunny→Sunny→Sunny (0.0546). Forward sums over paths (P(O)), Viterbi takes max (best path).

**Q13. Explain Baum-Welch re-estimation. (4)** *Answer:* EM for HMM: compute α,β → γₜ(j)=αβ/P(O), ξₜ(i,j); update aᵢⱼ=Σξ/Σγ, bⱼ(k)=Σ\_{oₜ=k}γ/Σγ, πᵢ=γ₁(i); repeat until P(O|λ) converges to a local maximum.

**Q14. Why does an ensemble of weak learners improve accuracy? How do we create and combine them? (4)** *Answer:* Independent errors → P(all N wrong)=(1−A)^N; reduces bias/variance; create via data subsets, features, algorithms, hyperparameters; combine via majority vote/weighted vote/averaging.

## 3.17 Unit 3 – Practice 2-Mark MCQs (with explanations)

**U3-2M-1.** Five independent weak classifiers each have accuracy 0.8. Probability that **all five are wrong** on an instance is: (a) 0.2 (b) 0.00032 (c) 0.00064 (d) 0.99968 **Ans: (b).** (1−0.8)⁵ = 0.2⁵ = 0.00032. (d) is the probability that at least one is right.

**U3-2M-2.** A bootstrap sample of size n (n very large) is drawn with replacement. The expected fraction of original records **never selected** is approximately: (a) 63.2% (b) 36.8% (c) 50% (d) 1/n **Ans: (b).** (1−1/n)ⁿ → 1/e ≈ 0.368; 63.2% are selected.

**U3-2M-3.** In gradient boosting with initial prediction 71.2, learning rate 0.1 and a first tree outputting 16.8 for a sample with actual weight 88, the **new prediction and residual** are: (a) 72.88 and 15.12 (b) 88 and 0 (c) 71.2 and 16.8 (d) 73.0 and 15.0 **Ans: (a).** 71.2 + 0.1×16.8 = 72.88; 88 − 72.88 = 15.12 (≈15.1).

**U3-2M-4.** A disease affects 1% of people. A test is +ve for 90% of diseased and 10% of healthy people. P(disease | +ve) ≈ (a) 0.90 (b) 0.50 (c) 0.083 (d) 0.01 **Ans: (c).** 0.9×0.01 = 0.009; 0.1×0.99 = 0.099; 0.009/(0.108) = 0.083. (Low prior dominates.)

**U3-2M-5.** Posteriors: P(h₁|D)=0.4, P(h₂|D)=0.3, P(h₃|D)=0.3. h₁ predicts −; h₂, h₃ predict +. The **Bayes optimal** class is: (a) − (b) + (c) tie (d) cannot be determined **Ans: (b).** P(+)=0.6, P(−)=0.4; the MAP hypothesis predicts − but BO predicts +.

**U3-2M-6.** Naïve Bayes with 3 binary features and 3 classes needs how many free parameters? (a) 11 (b) 23 (c) 9 (d) 8 **Ans: (a).** k·n(v−1)+(k−1) = 3·3·1+2 = 11 (full joint would be 3(2³−1)+2 = 23).

**U3-2M-7.** Class "spam" has 20 total word occurrences; vocabulary size |V| = 30; the word "offer" never occurs in spam. With Laplace smoothing P(offer|spam) = (a) 0 (b) 1/20 (c) 1/50 (d) 1/30 **Ans: (c).** (0+1)/(20+30) = 1/50.

**U3-2M-8.** In a 2-component GMM, a point has equal density under both Gaussians; priors are π₁=0.8, π₂=0.2. The responsibility γ₁ is: (a) 0.5 (b) 0.8 (c) 0.2 (d) 1.0 **Ans: (b).** γ₁ = 0.8·f/(0.8·f+0.2·f) = 0.8.

**U3-2M-9.** An HMM has N=5 states and sequence length T=10. The number of operations for the Forward algorithm is of the order: (a) 5¹⁰ (b) N²T = 250 (c) T·Nᵀ (d) 2¹⁰ **Ans: (b).** Forward is O(N²T)=250; brute force is O(T·Nᵀ) ≈ 10×5¹⁰.

**U3-2M-10.** For the Sunny/Rainy HMM: α₂(S)=0.096, β₂(S)=0.72, P(O|λ)=0.13184. Then γ₂(S) ≈ (a) 0.096 (b) 0.72 (c) 0.524 (d) 0.476 **Ans: (c).** γ = αβ/P(O) = 0.06912/0.13184 ≈ 0.524.

**U3-2M-11.** In Viterbi (H,G,H example) the best-path probability 0.0546 compared with P(O|λ)=0.1318 is: (a) greater (b) smaller, because Forward sums all paths (c) equal (d) unrelated **Ans: (b).** The best single path is one term of the sum.

## 3.18 Unit 3 – Practice 1-Mark MCQs (with explanations)

1. A decision stump is: (a) tree with one split (b) tree with 2 levels (c) SVM (d) random forest. **Ans (a)** – root node only.
2. Ensembles work best when base learners’ errors are: (a) identical (b) independent (c) zero (d) perfectly correlated. **Ans (b).**
3. Bagging mainly reduces: (a) bias (b) variance (c) both equally (d) noise in labels. **Ans (b).**
4. Which is **sequential**? (a) Bagging (b) Random Forest (c) Boosting (d) bootstrap. **Ans (c).**
5. OOB samples per tree are about: (a) 63% (b) 50% (c) 1/3 (d) 10%. **Ans (c)** – ≈36.8%.
6. In gradient boosting each new tree is fit to: (a) original target (b) residuals (c) random labels (d) feature means. **Ans (b).**
7. Gradient boost’s initial prediction (regression) is: (a) 0 (b) a stump (c) mean of target (d) median feature. **Ans (c).**
8. Learning rate in gradient boosting is typically: (a) >1 (b) between 0 and 1 (c) negative (d) 10. **Ans (b).**
9. In Random Forest, number of features per split R satisfies: (a) R = M (b) R < M (c) R > M (d) R = 1 always. **Ans (b).**
10. RF trees are generally: (a) pruned heavily (b) unpruned (c) stumps (d) linear. **Ans (b).**
11. Proximity in RF counts samples that: (a) have equal features (b) end in the same leaf (c) have same label (d) are OOB. **Ans (b).**
12. P(A∪B) = (a) P(A)+P(B) (b) P(A)+P(B)−P(A∩B) (c) P(A)P(B) (d) P(A|B). **Ans (b).**
13. Posterior = (a) prior×evidence (b) likelihood×prior/evidence (c) likelihood/prior (d) evidence/likelihood. **Ans (b).**
14. If priors are equal, h\_MAP equals: (a) h\_ML (b) h\_Gibbs (c) random h (d) none. **Ans (a).**
15. ML hypothesis for Gaussian noise minimises: (a) absolute error (b) sum of squared errors (c) entropy (d) max error. **Ans (b).**
16. In brute-force MAP concept learning, P(h|D) for consistent h is: (a) 1 (b) 1/|H| (c) 1/|VS| (d) 0. **Ans (c).**
17. Bayes optimal classifier uses: (a) the MAP hypothesis only (b) all hypotheses weighted by posterior (c) one random h (d) prior only. **Ans (b).**
18. Gibbs error is at most \_\_\_ times Bayes optimal error: (a) 1 (b) 2 (c) 4 (d) 0.5. **Ans (b).**
19. Naïve Bayes assumes features are: (a) independent given class (b) identical (c) linearly dependent (d) uniformly distributed. **Ans (a).**
20. Which is NOT a Naïve Bayes variant? (a) Gaussian (b) Bernoulli (c) Multinomial (d) Linear. **Ans (d).**
21. Bernoulli NB uses features that are: (a) counts (b) binary (c) continuous (d) ordinal. **Ans (b).**
22. Zero-frequency problem is fixed by: (a) log (b) Laplace smoothing (c) pruning (d) normalising. **Ans (b).**
23. Underflow in NB is avoided by: (a) log-probabilities (b) bigger data (c) removing priors (d) pruning. **Ans (a).**
24. EM stands for: (a) Error Minimisation (b) Expectation Maximization (c) Estimate Mean (d) Ensemble Method. **Ans (b).**
25. In the E-step of GMM we compute: (a) new means (b) responsibilities (c) priors only (d) loss gradient. **Ans (b).**
26. GMM performs \_\_\_ clustering: (a) hard (b) soft (c) hierarchical (d) density-based only. **Ans (b).**
27. The Markov property says the future depends on: (a) whole past (b) present only (c) future only (d) nothing. **Ans (b).**
28. HMM parameters are: (a) A, B, π (b) A only (c) μ, Σ (d) W, b. **Ans (a).**
29. Algorithm for HMM decoding: (a) Forward (b) Viterbi (c) Backward (d) Baum-Welch. **Ans (b).**
30. Baum-Welch is a special case of: (a) Gibbs (b) EM (c) K-means (d) boosting. **Ans (b).**
31. β\_T(i) is initialised to: (a) 0 (b) 1 (c) πᵢ (d) bᵢ. **Ans (b).**
32. Brute-force HMM likelihood complexity: (a) O(N²T) (b) O(T·Nᵀ) (c) O(NT) (d) O(T). **Ans (b).**

## 3.19 Unit 3 – Compact Revision Sheet

**Ensembles:** weak learners + diversity + vote. P(all wrong)=(1−A)^N. Bagging = bootstrap (63/37) + parallel + variance↓ + OOB. Boosting = sequential + weights + bias↓. **GB:** F₀=mean → residuals → tree on residuals (leaf avg) → +ν·tree (ν=0.1); 71.2, 16.8 → 15.1. **RF** = bagging + random R\<M features + unpruned + OOB + proximity for missing data.

**Bayes:** posterior = likelihood×prior/evidence. MAP vs ML (equal priors). Cancer: 0.0078 vs 0.0298 → 21%/79%. ML+Gaussian noise ⇔ least squares. Brute-force MAP: 1/|VS|. **BO**: ΣP(v|h)P(h|D); **Gibbs**: sample h, error ≤ 2×BO.

**Naïve Bayes:** argmax P(v)ΠP(a|v); parameters k·n(v−1)+(k−1); types Gaussian/Multinomial/Bernoulli; Laplace (n\_c+1)/(n+m); m-estimate (n\_c+mp)/(n+m); log-space; PlayTennis: Yes 0.0053 vs No 0.0206 → **No**; sports text 2.76×10⁻⁵ > 5.7×10⁻⁶.

**EM/GMM:** E: γ (soft); M: μ=Σγx/Σγ, π=Σγ/n; example 2,4,7 → μ≈2.98/6.87, π=0.651/0.349; local maximum.

**HMM:** λ=(A,B,π); Markov + output independence. Likelihood → Forward/Backward O(N²T) (P(O)=Σα\_T=Σαₜβₜ=0.1318); Decoding → Viterbi (max, backpointers; SSS, 0.0546); Learning → Baum-Welch (γ, ξ; EM).

---

# UNIT 4 – UNSUPERVISED LEARNING, ML DEPLOYMENT & ADVANCED LEARNING PARADIGMS

## 4.1 Clustering – Introduction

**Definition:** Clustering is an **unsupervised** learning task that groups objects so that items **within** a cluster are **similar** and items in **different** clusters are **dissimilar**.

- **Minimise intra-cluster distance; maximise inter-cluster distance.**
- No labels are used.

**Applications:** customer segmentation (location, spending, income); document clustering (semantic similarity); recommendation engines (cluster similar items/users); image segmentation; anomaly detection.

### 4.1.1 Types of clustering

| Type | Idea | Examples |
| --- | --- | --- |
| **Hierarchical** | Builds clusters from existing clusters (tree) | **Agglomerative (AGNES)** – bottom-up; **Divisive (DIANA)** – top-down |
| **Partitional (non-hierarchical)** | Forms all K clusters at once, flat | **K-means** |
| **Density-based** | Clusters = dense regions | DBSCAN |
| **Hard clustering** | Each point in exactly one cluster | K-means |
| **Soft clustering** | Point belongs to several clusters with probabilities | GMM (EM) |

### 4.1.2 ★ Hierarchical vs Non-hierarchical (Partitional)

| Aspect | Hierarchical | Non-hierarchical (K-means) |
| --- | --- | --- |
| Output | **Dendrogram** (nested clusters) | **Flat** partition into K clusters |
| K needed in advance? | **No** (choose by cutting dendrogram) | **Yes** (K must be specified) |
| Reversibility | **Merge/split cannot be undone** | Points can **move** between clusters in later iterations |
| Complexity | Time **O(N³)** (O(N² log N) with heaps), space **O(N²)** | **O(N·K·D·I)** – scales to large data |
| Determinism | Deterministic | Depends on **random initial centroids** |
| Shapes | Depends on linkage | Tends towards **spherical** clusters |
| Best for | Small data, need hierarchy | Large data |

### 4.1.3 Distance measures

**Between points – Minkowski distance:** d(x,y) = ( Σᵢ |xᵢ − yᵢ|^r )^{1/r}

- **r = 1 → Manhattan (taxicab)**, **r = 2 → Euclidean**.

**Between clusters (linkage):**

| Linkage | Definition | Behaviour |
| --- | --- | --- |
| **Single (min)** | Distance between the **closest** pair of points, one from each cluster | Can create long "chains"; handles non-elliptical shapes; **sensitive to noise** |
| **Complete (max)** | Distance between the **farthest** pair | Compact clusters; sensitive to outliers; may break large clusters |
| **Group average** | Average of **all pairwise** distances | Compromise; less sensitive to noise |
| **Centroid** | Distance between cluster **centroids** | Simple; can give inversions in dendrogram |

---

## 4.2 Hierarchical Clustering – Agglomerative (AGNES) & Divisive (DIANA)

**Dendrogram:** tree diagram of the hierarchy; each merge is a **branch connection** and the **height** of the branch = distance at which the clusters merged. **Cut** the dendrogram horizontally at a chosen height to obtain the desired number of clusters (e.g., cut where it crosses 4 branches → 4 clusters).

### 4.2.1 ★ Agglomerative Nesting (AGNES) – bottom-up

```
1. Treat every point as its own cluster; build the N×N distance (proximity) matrix
2. Find the two closest clusters (minimum distance)
3. Merge them into one cluster
4. Update the distance matrix (distances to the new cluster by chosen linkage)
5. Repeat 2–4 until all points are in one cluster
```

Choosing the number of clusters: **domain knowledge/heuristics**, **elbow method**, or cut the dendrogram where there is a **large vertical gap**.

### 4.2.2 ★ Worked example (slides) – single linkage, Euclidean distance

Points: **A(1,1), B(1.5,1.5), C(5,5), D(3,4), E(4,4), F(3,3.5)**.

Distance matrix (rounded):

|  | A | B | C | D | E | F |
| --- | --- | --- | --- | --- | --- | --- |
| A | 0 | 0.71 | 5.66 | 3.61 | 4.24 | 3.20 |
| B | 0.71 | 0 | 4.95 | 2.92 | 3.54 | 2.50 |
| C | 5.66 | 4.95 | 0 | 2.24 | 1.41 | 2.50 |
| D | 3.61 | 2.92 | 2.24 | 0 | 1.00 | **0.50** |
| E | 4.24 | 3.54 | 1.41 | 1.00 | 0 | 1.12 |
| F | 3.20 | 2.50 | 2.50 | 0.50 | 1.12 | 0 |

1. Smallest = **D–F = 0.50** → cluster {D,F}.
2. Next smallest = **A–B = 0.71** → cluster {A,B}.
3. {D,F} to E = min(D–E 1.00, F–E 1.12) = **1.00** → {D,F,E}.
4. {D,F,E} to C = min(2.24, 1.41, 2.50) = **1.41** → {D,F,E,C}.
5. {A,B} to {D,F,E,C}: smallest cross distance = **B–F = 2.50** → one cluster.

**Merge heights:** 0.50, 0.71, 1.00, 1.41, 2.50 (these are the dendrogram branch heights). Cutting between 1.41 and 2.50 gives 2 clusters: {A,B} and {C,D,E,F}.

**R code (slide):** `hclust()` on USArrests (50 US states) → dendrogram → `cutree(k=4)` / cut dendrogram at 4 branches. Python equivalent: `scipy.cluster.hierarchy.linkage(X,'single')`, `sklearn.cluster.AgglomerativeClustering(n_clusters=4, linkage='single')`.

**Complexity (slide):** **Space O(N²)** (proximity matrix); **Time O(N³)** (N steps, each updates/searches an N² matrix); can be reduced to **O(N² log N)**.

**Limitations:** ① **Greedy – once merged, cannot be undone** ② sensitive to **noise and outliers** ③ difficulty with **different-sized clusters and non-convex shapes** (depends on linkage) ④ may **break large clusters** ⑤ high time/space cost.

### 4.2.3 Divisive clustering (DIANA) – top-down

```
1. Start with all points in one cluster
2. Choose a cluster to split (e.g., largest diameter / largest SSE)
3. Split it into two (e.g., by K-means with K=2, or move the most dissimilar point into a 'splinter group' and keep moving points closer to the splinter than to the rest)
4. Repeat until every point is a singleton or desired K is reached
```

|  | Agglomerative (AGNES) | Divisive (DIANA) |
| --- | --- | --- |
| Direction | **Bottom-up** (merge) | **Top-down** (split) |
| Start | N singleton clusters | 1 cluster with all points |
| Operation | Merge closest pair | Split a cluster |
| Cost | Simpler, widely used | Costlier (2ⁿ⁻¹ possible splits → heuristics needed) |
| Good at | Small clusters / local structure | Large clusters / global structure |

---

## 4.3 ★★ K-Means Clustering

**Idea:** partitional, hard clustering. Two common-sense ideas: (1) every point belongs to its **nearest centroid**; (2) every centroid should be the **mean of its cluster's points** (as close as possible to them).

### 4.3.1 Objective (optimisation view)

Minimise **WCSS / SSE / inertia**: **J = Σᵢ Σₖ rᵢₖ · ‖xᵢ − μₖ‖²**, rᵢₖ ∈ {0,1}, exactly one rᵢₖ = 1 per point. (Pizza-shop analogy: place shops so total squared distance from houses to nearest shop is smallest.)

### 4.3.2 ★ Algorithm

```
1. Choose K; pick K random data points as initial centroids μ₁..μₖ
2. Repeat until convergence (assignments stop changing / centroids stop moving):
     a. ASSIGN (E-step): each point → nearest centroid   rᵢₖ = 1 if k = argmin_j ‖xᵢ − μⱼ‖²
     b. UPDATE (M-step): μₖ = (1/|Cₖ|) Σ_{xᵢ∈Cₖ} xᵢ      (mean of its points)
```

J **never increases** each iteration → guaranteed to converge, but to a **local** minimum.

### 4.3.3 ★ Worked example (slides) – K=2

Points: **A(1,1), B(2,1), C(4,3), D(5,4)**; initial centroids **c₁ = A(1,1), c₂ = B(2,1)**.

**Iteration 0 – distances**

|  | A | B | C | D |
| --- | --- | --- | --- | --- |
| to c₁(1,1) | 0 | 1 | 3.61 | 5 |
| to c₂(2,1) | 1 | 0 | 2.83 | 4.24 |
| Assignment: A → c₁; B, C, D → c₂. New centroids: c₁ = (1,1); c₂ = ((2+4+5)/3, (1+3+4)/3) = **(11/3, 8/3)**. |  |  |  |  |

**Iteration 1 – distances**

|  | A | B | C | D |
| --- | --- | --- | --- | --- |
| to c₁(1,1) | 0 | 1 | 3.61 | 5 |
| to c₂(3.67,2.67) | 3.14 | 2.36 | 0.47 | 1.89 |
| Assignment: **A, B → cluster 1; C, D → cluster 2.** New centroids: c₁ = (1.5, 1), c₂ = **(4.5, 3.5)**. |  |  |  |  |

**Iteration 2:** assignments unchanged → **converged.** Final clusters {A,B} and {C,D}.

### 4.3.4 Choosing K – Elbow method

Plot **WCSS vs K**; choose the "elbow" where WCSS stops dropping sharply. ⚠ **K = N gives WCSS = 0** (each point its own cluster) – so minimum WCSS alone cannot choose K. \[Supplement: silhouette score also used.\]

### 4.3.5 Complexity

**O(N · K · D · I)** for N points, D dimensions, K clusters, I iterations – linear in N.

### 4.3.6 Advantages / Drawbacks

| Advantages | Drawbacks |
| --- | --- |
| Simple, easy to implement | Result depends on **initial centroids** (local minima) |
| **Parallelisable** (MapReduce), scalable | Tends to produce **spherical** clusters; poor on varied sizes/densities/non-convex shapes |
| Can handle non-spherical data after transformation (kernel trick) | **Sensitive to scale** of features and **outliers** (mean) |
| Tighter, well-separated clusters than hierarchical | K must be given; slow in very high dimension |

**Fixes (slides):** initial centroids – pick more than K candidates then choose the most widely separated; or over-cluster then apply hierarchical clustering; run multiple times \[Supplement: **K-means++**\]. Scaling – **rescale features to \[0,1\]** (normalise).

---

## 4.4 ★ Bisecting K-Means

**Idea:** **combines K-means with divisive hierarchical clustering** – repeatedly splits **one** cluster into two using 2-means until K clusters are obtained.

```
1. Put all data in ONE cluster
2. Split it into two using K-means with K=2
3. Among the clusters pick the one with the LARGEST SSE (sum of squared distances)
4. Split that cluster using 2-means
5. Repeat 3–4 until the number of leaf clusters = K
```

*Slide example (K=4):* all data → {A,B}; A has larger SSE → split into C, D; now {C,D,B}: B has largest SSE → split into E, F → 4 leaves {C,D,E,F} → stop.

**Advantages:** more efficient for large K (only 2-means each time); roughly logarithmic number of levels (divide-and-conquer); **clusters of more similar size** than plain K-means; less sensitive to initialisation; gives a hierarchy.

|  | K-means | Bisecting K-means |
| --- | --- | --- |
| Approach | Partitional, all K at once | Divisive hierarchical using 2-means |
| Split choice | – | Cluster with largest SSE |
| Cluster sizes | Widely varying | More uniform |
| Large K | Costly | **More efficient** |
| Output | Flat | Hierarchy |

---

## 4.5 ★ K-Means as a Special Case of Expectation Maximization

GMM-EM (Unit 3): **E-step** computes soft responsibilities; **M-step** updates μ, Σ, π. K-means is the same loop with simplifying choices:

| EM for GMM | K-means |
| --- | --- |
| Latent variable zᵢₖ = soft probability | Latent variable rᵢₖ ∈ {0,1} (**hard**) |
| E-step: γᵢₖ ∝ πₖN(xᵢ\|μₖ,Σₖ) | E-step: **assign each point to the closest centroid** (minimum distance) |
| M-step: μₖ, Σₖ, πₖ weighted estimates | M-step: **centroid = mean of assigned points** |
| Covariance learned | Covariance fixed = σ²I, equal for all (spherical) |
| Priors learned | Priors equal (πₖ = 1/K) |
| Limit | – |

**Slide framing:** E-step = cluster assignments (latent variable); M-step = find the parameters (centroids) given the assignments. ⚠ K-means = *hard-assignment EM*; both converge only to **local** optima.

---

## 4.6 ★★ Introduction to Reinforcement Learning (RL)

**Definition:** RL is a type of ML in which an **agent learns by interacting with an environment**, receiving **rewards (positive or negative feedback)** for its actions; the objective is to learn a **policy** that **maximises cumulative reward over time**. No labelled data – **trial and error**.

### 4.6.1 The interaction loop

At each time t: the **agent observes state sₜ** → takes **action aₜ** → the **environment returns reward rₜ₊₁ and next state sₜ₊₁** → repeat (feedback loop).

### 4.6.2 Elements of RL

| Element | Meaning | Robot example |
| --- | --- | --- |
| **Agent** | Learner / decision-maker | The robot |
| **Environment** | Everything the agent interacts with | Office building |
| **State (s)** | Representation of current situation | Room A, B, C, D, E, G |
| **Action (a)** | What the agent can do to change state | Up, Down, Left, Right |
| **Reward (r)** | Immediate scalar feedback | +10 goal, −5 obstacle, −1 ordinary step |
| **Reward function** | Maps state (or state-action) → real number | R(s,a) |
| **Policy (π)** | Map from states to actions; deterministic a=π(s) or stochastic π(a\|s) | At A: Right; at B: Down |
| **Value function** | Expected total (discounted) future reward from a state / state-action | V(s), Q(s,a) |
| **Episode** | One run from initial to terminal state | A → B → G |

### 4.6.3 Supervised vs Unsupervised vs Reinforcement

|  | Supervised | Unsupervised | Reinforcement |
| --- | --- | --- | --- |
| Feedback | Correct labels | None | **Reward signal (delayed)** |
| Goal | Predict labels | Find structure | **Maximise cumulative reward** |
| Data | Fixed dataset | Fixed dataset | **Generated by agent's own actions** |
| Key issue | Generalisation | Cluster quality | **Exploration vs exploitation, delayed reward** |

### 4.6.4 ★ Markov Decision Process (MDP)

Math framework for RL. Hierarchy: **Markov Process (chain)** → **+ rewards = Markov Reward Process** → **+ actions = MDP**. Transition table becomes a cube indexed (i, j, a). Markov property: future depends only on current state (and action).

**MDP = (S, A, P, R, γ):**

| Symbol | Meaning |
| --- | --- |
| S | set of states |
| A | set of actions |
| **P(s'\|s,a)** | transition probability (e.g., A + Right → B 0.8, D 0.2 – stochastic environment) |
| R(s,a) | reward after action a in state s |
| **γ** | **discount factor**, 0 ≤ γ ≤ 1 (slides: 0 ≤ γ < 1) |

**Discount factor:** γ = 0 → only immediate reward (myopic); γ → 1 → future as important as present; makes infinite returns finite; prefers shorter routes (γ=0.9 → future reward discounted 10% per step). If episodes are finite, γ = 1 is allowed.

**Return:** **Gₜ = rₜ₊₁ + γrₜ₊₂ + γ²rₜ₊₃ + … = Σₖ γᵏ rₜ₊ₖ₊₁**. Goal: find optimal policy **π\*** that maximises expected return.

### 4.6.5 ★ Value functions & Bellman equations

- **State-value:** **Vᵖ(s) = Eᵖ\[ Gₜ | sₜ = s \]**
- **Action-value:** **Qᵖ(s,a) = Eᵖ\[ Gₜ | sₜ = s, aₜ = a \]**
- **Bellman expectation equation:** **Vᵖ(s) = Σₐ π(a|s) Σₛ' P(s'|s,a) \[ R(s,a) + γ Vᵖ(s') \]** – value of a state = immediate reward + discounted value of successor.
- **Bellman optimality equation:** **V\*(s) = maxₐ Σₛ' P(s'|s,a)\[R + γV\*(s')\]**
- **For action values (slides):** **Q\*(s,a) = R(s,a) + γ · maxₐ' Q\*(s',a')** (deterministic transitions; in general Q\*(s,a)=ΣP\[R+γ max Q\*(s',a')\]).
- Optimal policy: **π\*(s) = argmaxₐ Q\*(s,a)**.

The Bellman optimality equation is what the optimal Q-values **must satisfy**; Q-learning **learns estimates** of those values from experience.

### 4.6.6 ★★ Q-Learning

**Definition:** a **model-free, off-policy** RL algorithm that learns the quality Q(s,a) of actions in each state by interacting with the environment (no need to know P or R).

**Q-table:** rows = states, columns = actions, entries = estimated long-term value. Initialised to **0**. Q(A,Right)=0 means "not yet learned", not "reward is zero".

**★ Update rule:** **Q(s,a) ← Q(s,a) + α \[ r + γ · maxₐ' Q(s',a') − Q(s,a) \]**

- **α** = learning rate (how fast to update), **γ** = discount, **r + γ max Q(s',a')** = **TD target**, bracket = **TD error**.
- At a **terminal state** future Q = 0.

**Algorithm:**

```
Initialise Q(s,a)=0 for all s,a
For each episode:
   s = start state
   Repeat until s is terminal:
      choose a using ε-greedy on Q(s,·)
      take a → observe r, s'
      Q(s,a) ← Q(s,a) + α [ r + γ max_a' Q(s',a') − Q(s,a) ]
      s ← s'
```

**★ Worked example (slides): α = 0.5, γ = 0.9; route A → (Right, r=−1) → B → (Down, r=+10) → G (terminal)**

- **Episode 1** (update the later step first, as in the slide):
  - Q(B,Down) = 0 + 0.5\[10 + 0.9×0 − 0\] = **5**
  - Q(A,Right) = 0 + 0.5\[−1 + 0.9×5 − 0\] = 0.5×3.5 = **1.75**
- **Episode 2:** Q(B,Down) = 5 + 0.5(10 − 5) = **7.5**; Q(A,Right) = 1.75 + 0.5\[−1 + 0.9×7.5 − 1.75\] = 1.75 + 0.5×4.0 = **3.75**
- **Episode 3:** Q(B,Down)=8.75; Q(A,Right)=**5.3125**
- After many episodes Q(B,Down) → **10** and Q(A,Right) → **≈ 8** (= −1 + 0.9×10).
- Learned table (illustrative): A: Up 1, Down 2, Left 0, **Right 8**; B: Up 3, **Down 10**, Left 2, Right 1 → **π\*(A)=Right, π\*(B)=Down**.

**Why is Q(B,Down)=5 not 10?** Because α=0.5 – learning is gradual. **Can Q increase after a −1 reward?** Yes, if the discounted future value is large enough (Q(A,Right) rose to 1.75).

**★ Exploration vs exploitation – ε-greedy:** with probability **ε** choose a **random** action (explore); with probability **1−ε** choose the **highest-Q** action (exploit). E.g., ε = 0.2 → 20% random. Needed to discover better routes. (Commonly ε decays over time.)

**Why "off-policy"?** The target uses **max over next actions**, not necessarily the action actually taken next (behaviour policy ε-greedy, learned policy greedy). \[Supplement\] **SARSA** is on-policy: target uses the next action actually taken: r + γ Q(s',a').

|  | Q-learning | SARSA |
| --- | --- | --- |
| Type | **Off-policy** | On-policy |
| Target | r + γ **max** Q(s',a') | r + γ Q(s',**a'** taken) |
| Behaviour | Learns optimal policy regardless of exploration | Learns value of the policy being followed (safer) |

Other algorithms (slide list): SARSA, **DQN** (Deep Q-Network – neural net replaces Q-table), Policy Gradient, Actor-Critic.

**Applications:** robotics, game playing (Go, Atari), self-driving, recommendation, resource management, finance trading, LLM alignment (RLHF). **Limitations:** sample inefficient; Q-table infeasible for large state spaces (need function approximation); reward design is hard; exploration is risky.

---

## 4.7 Introduction to Deep Learning \[Supplement – slides only motivate via computer vision\]

**Definition:** Deep learning (DL) = machine learning with **neural networks having many hidden layers** that **automatically learn hierarchical feature representations** from raw data (representation learning). Early layers learn simple features (edges), later layers combine them (shapes, objects).

**Why now?** Big data, GPUs, better algorithms (ReLU, dropout, batch norm, Adam), open frameworks (TensorFlow/PyTorch).

**Core ideas:** layers of neurons: output = activation(W·x + b); **forward pass → loss → backpropagation (chain rule) → gradient descent update**; activations: sigmoid, tanh, **ReLU**, softmax (output probabilities).

**Architectures:** MLP/Feed-forward, **CNN** (images), RNN/LSTM (sequences), Transformers (language), Autoencoders, GANs.

|  | Traditional ML | Deep Learning |
| --- | --- | --- |
| Features | **Hand-crafted** | **Learned automatically** |
| Data needed | Smaller | Very large |
| Compute | Low | High (GPU) |
| Interpretability | Higher | Lower ("black box") |
| Best on | Tabular data | Images, speech, text |

---

## 4.8 ★★ Convolution Operation and Convolutional Neural Networks (CNN)

### 4.8.1 Why CNNs? Problem with fully connected networks

- A 1000×1000 RGB image = **3 million inputs**; 1000 hidden units → first weight matrix = **3 billion parameters** → huge computation, **overfitting**, ignores spatial structure. (CIFAR 32×32×3 = 3072 weights per neuron; 300×300×3 = 270,000 per neuron.)
- **Convolution** merges two sets of information; a **filter** slides over the input and produces a **feature map**; each neuron connects to only a **small local region** (**sparse connectivity**) and **filter weights are shared** (**parameter sharing**).

|  | Fully Connected NN | CNN |
| --- | --- | --- |
| Connectivity | Dense, every neuron to every neuron | **Local receptive field** |
| Weights | No sharing | **Shared filters** |
| Spatial structure | Ignored | **Captured hierarchically** |
| Parameters | Very many | **Far fewer** |
| Overfitting | Prone | Less prone |

### 4.8.2 Convolution operation & filters

- A **filter/kernel** is a small matrix (3×3, 5×5) of **learnable weights** that detects patterns (edges, corners, textures).
- **Steps:** slide the filter over the input; at each position **multiply element-wise with the overlapping patch and sum** → one output number. Input cells contributing to one output = **receptive field**. All outputs form a **feature map**. **Multiple filters → multiple feature maps** (stacked to depth).
- 1D (time series), 2D (images), 3D (volumes/point clouds) convolution.
- **Edge detection example:** vertical-edge filter **\[\[1,0,−1\],\[1,0,−1\],\[1,0,−1\]\]** on a 6×6 image with left half = 10 and right half = 0 gives a 4×4 output whose middle columns are **30** (edge) and others 0. Horizontal filter = transpose pattern. In CNNs the filter values are **learned** by backpropagation.
- **3D convolution (RGB):** filter has the **same depth as input** (e.g., 3×3×3): multiply & sum across all channels → **one** 2D output channel per filter.
- &#91;Note: deep-learning "convolution" is technically cross-correlation (no kernel flip).\]

### 4.8.3 ★ Hyperparameters, padding, stride and output size

- **Padding (p):** add p rows/columns of zeros around the border. **Valid** = no padding (p = 0) → output shrinks; **Same** = pad so output size = input size → **p = (f − 1)/2** (f usually odd). Padding keeps border information and prevents shrinkage.
- **Stride (s):** number of pixels the filter moves each step. Larger stride → smaller output, less computation, larger field of view, acts as down-sampling.
- **★ Output size:** **O = ⌊ (n − f + 2p) / s ⌋ + 1** (for n×n input, f×f filter).
  - Examples: 6×6, f=3, p=0, s=1 → 4×4. 8×8 (after p=1) → 6×6. 32×32, f=5, p=0, s=1 → 28. 7×7, f=3, s=2 → 3.
- **Number of output channels = number of filters k.**

### 4.8.4 ★ Parameter calculation

- **Conv layer parameters = (f × f × c\_in + 1) × k** = f·f·c\_in·k **+ k (biases)**, c\_in = input depth, k = number of filters. (Slide's no-bias version: f×f×c×k.)
- **Padding/stride/input size do NOT change the parameter count** (only output size).
- **Fully connected layer:** (inputs × outputs) + outputs (biases).
- **Pooling layers: 0 parameters.**

**★ Example (slides):** Conv1: 5×5, input depth 3, 8 filters; Conv2: 5×5, 16 filters; flatten = 400; FC3 = 100 neurons; FC4 = 84; Softmax = 10.

| Layer | Calculation | Parameters |
| --- | --- | --- |
| Conv1 | (5×5×3 + 1) × 8 = 76 × 8 | **608** |
| Conv2 | (5×5×8 + 1) × 16 = 201 × 16 | **3,216** |
| Pooling | – | **0** |
| FC3 | 400×100 + 100 | **40,100** |
| FC4 | 100×84 + 84 | **8,484** |
| Softmax | 84×10 + 10 | **850** |
| **Total** |  | **53,258** |

**Observations (slide):** pooling has no parameters; conv layers have relatively **few** parameters; **FC layers have the most**; activation size decreases gradually with depth.

### 4.8.5 ★ Pooling

Purpose: **reduce size of representation, speed up computation, make features more robust** (translation-tolerant). Pooling is a **fixed operation with NO learnable parameters** – hyperparameters: filter size f, stride s, type.

- **Max pooling** (most common): output = **maximum** in each window → keeps the strongest feature.
- **Average pooling** (less used): output = **mean** of window.
- **Output size:** ⌊(n − f)/s⌋ + 1 per dimension. Slide: 5×5 input, f=3, s=1 → **3×3**.
- **Channels unchanged:** pooling reduces height/width (n\_H, n\_W) but **keeps number of channels n\_C** (applied per channel).
- Example: 4×4 input \[\[1,3,2,1\],\[2,9,1,1\],\[1,3,2,3\],\[5,6,1,2\]\], f=2, s=2: max pool → \[\[9,2\],\[6,3\]\]; average pool → \[\[3.75,1.25\],\[3.75,2\]\].

|  | Max pooling | Average pooling |
| --- | --- | --- |
| Operation | max of window | mean of window |
| Keeps | Most prominent features | Smoothed overall presence |
| Usage | Most common | Less common (e.g., global avg pooling at the end) |
| Parameters | 0 | 0 |

### 4.8.6 Typical CNN architecture

**INPUT → \[CONV → ReLU → POOL\] × n → FLATTEN → FC → … → SOFTMAX.** Convolution and pooling layers alternate; **depth (number of filters) increases** and spatial size decreases with depth; last stage = fully connected layer(s) + **softmax**. Early layers learn edges, middle layers textures/parts, deep layers objects. Applications: image classification, object detection, face recognition, segmentation, style transfer.

⚠ MCQ traps: pooling has **no** parameters; padding/stride **do not change** parameter count; "same" padding p=(f−1)/2; max pooling keeps channels; FC layers contain most parameters; number of output channels = number of filters.

---

## 4.9 ★★ Hyperparameter Tuning – Grid Search, Random Search, Bayesian Optimization

### 4.9.1 Parameters vs hyperparameters

|  | Parameters | Hyperparameters |
| --- | --- | --- |
| Set by | **Learned from data** during training | **Chosen by us before** training |
| Decision tree | Split feature & threshold at each node | max\_depth, criterion, min\_samples\_split |
| Random forest | The individual trees | n\_estimators, max\_features, max\_samples |
| SVM (RBF) | Support vectors & weights | C, gamma, kernel |
| Neural network | Weights and biases | Learning rate, #layers, batch size |

**Training optimises parameters; tuning optimises hyperparameters.**

**Why tune?** Defaults are generic. Too little capacity → **underfitting**; too much → **overfitting**. (Slide: Decision Tree on Breast Cancer data – default unlimited depth: train 100% but CV only 91.6%; max\_depth = 2 → CV 93.2%.)

### 4.9.2 Tuning as a search problem

1. Define **search space** (which hyperparameters, which values/ranges)
2. Choose a configuration
3. Train + **5-fold cross-validation**; score = mean CV accuracy (the **objective**)
4. Keep best so far; repeat until the **budget** (trials/time) is used. Methods differ only in **how step 2 is done**.

**⚠ Rule:** tune using **CV on the training data only**; use the **test set once, at the very end** (otherwise leakage/optimistic estimate). Keep preprocessing inside a **Pipeline**. **Nested CV** gives an unbiased estimate when comparing tuned models.

**k-fold CV:** each round, k−1 folds train, 1 fold validates; average the k scores (e.g., 0.963 ± 0.018).

### 4.9.3 ★ Grid Search

- Choose a **finite list of values for each hyperparameter**; train and cross-validate **every combination**; pick the best CV score.
- **Cost = (product of number of values) × (number of folds)** – grows **multiplicatively** (curse of dimensionality). Never looks *between* grid points.
- Example grid: criterion {gini, entropy} (2) × max\_depth {3,5,7,10} (4) × min\_samples\_split {2,5,10} (3) = **24 combinations × 5 folds = 120 fits**. Adding one hyperparameter with 5 values → 5× bigger. 5 hyperparameters × 5 values = 3,125 combos = **15,625 fits**.

```python
from sklearn.model_selection import GridSearchCV
from sklearn.tree import DecisionTreeClassifier
param_grid = {'criterion':['gini','entropy'],
              'max_depth':[3,5,7,10],
              'min_samples_split':[2,5,10]}
dt   = DecisionTreeClassifier(random_state=42)
grid = GridSearchCV(estimator=dt, param_grid=param_grid, cv=5)
grid.fit(X_train, y_train)            # 24 x 5 = 120 fits; refit=True retrains best on full train set
print(grid.best_params_, grid.best_score_)
print(grid.score(X_test, y_test))      # test set used ONCE
```

Slide result: best {gini, depth 5, split 5}; CV 0.938; test 0.921 (untuned: CV 0.916, test 0.912). **Pros:** simple, exhaustive on the grid, reproducible, easily parallel. **Cons:** exponential cost; wastes trials repeating unimportant values; limited to grid resolution; can't handle continuous ranges well.

### 4.9.4 ★ Random Search

- Define a **range or distribution** for each hyperparameter (can be continuous / log scale, e.g., C \~ loguniform(1e−2, 1e2)); **draw n\_iter random combinations** and evaluate each by CV.
- **Budget is fixed by n\_iter**, independent of space size.
- **Why it can beat grid (Bergstra & Bengio, 2012):** usually only **a few hyperparameters really matter**. A grid repeats the same few values of the important one; **random search tries a new value of every hyperparameter in each trial.** With 25 trials grid explores only 5 distinct values per hyperparameter; random explores **25 distinct values**.

```python
from sklearn.model_selection import RandomizedSearchCV
rs = RandomizedSearchCV(RandomForestClassifier(random_state=42),
                        param_distributions=param_dist,   # 3x3x3x2x2 = 108 combos
                        n_iter=20, cv=5, random_state=42)  # try only 20
rs.fit(X_train, y_train)
```

**Slide comparison (Random Forest):** Grid: 108 configs, 540 fits, 124 s, CV 0.9648, test 0.947. Random: 20 configs, **100 fits, 24 s**, CV 0.9648, test 0.956. Rule of thumb: **GridSearchCV for small problems; RandomizedSearchCV for large datasets/many hyperparameters.** Cons: no guarantee of hitting the optimum; ignores past results.

### 4.9.5 ★ Bayesian Optimization

- Grid/random are **memoryless** (trial k+1 ignores trials 1..k). When each evaluation is **expensive** (minutes–hours), wasted trials are costly. **Bayesian optimization learns from past trials to decide where to look next** (like the hot/cold game).
- **Components:**
  1. **Surrogate model** – cheap probabilistic model of score vs hyperparameters fitted to all (config, score) pairs; predicts **mean μ(x) and uncertainty σ(x)**. Examples: **Gaussian Process**, **TPE** (Tree-structured Parzen Estimator).
  2. **Acquisition function** – turns μ and σ into "how promising is this point?". Examples: **Expected Improvement (EI)**, **Upper Confidence Bound (UCB = μ + κσ)**.
- **Loop:** (1) fit surrogate to history → (2) maximise acquisition to choose the next config → (3) evaluate for real (CV) → (4) add result to history; repeat.
- **Exploration vs exploitation:** exploit = sample where predicted score is high; explore = sample where uncertainty is high; acquisition balances both (larger κ → more exploration). EI = expected amount a point beats the current best.
- **Slide example:** SVM CV accuracy vs log₁₀(C): after only **8 evaluations** best = 0.976, equal to the best of 101 grid points.
- **Optuna (TPE default):** study = optimisation session, trial = one evaluation; TPE models "good" and "bad" configs with two densities; **pruning** stops unpromising trials early; handles int/float/log/categorical.

```python
import optuna
def objective(trial):
    model = RandomForestClassifier(
        n_estimators=trial.suggest_int('n_estimators', 50, 300),
        max_depth=trial.suggest_int('max_depth', 2, 20),
        max_features=trial.suggest_categorical('max_features', ['sqrt','log2']),
        random_state=42)
    return cross_val_score(model, X_train, y_train, cv=5).mean()
study = optuna.create_study(direction='maximize')   # TPE sampler
study.optimize(objective, n_trials=30)
print(study.best_params, study.best_value)
```

### 4.9.6 ★ Comparison

| Method | Next trial chosen by | Uses past results? | Many hyperparameters | Best when |
| --- | --- | --- | --- | --- |
| **Grid** | Fixed grid | **No** | Poor (cost multiplies) | Few hyperparameters, cheap model |
| **Random** | Random sampling | **No** | Good | Large space, limited budget |
| **Bayesian** | Model of past results | **Yes** | Good | **Expensive** evaluations |

Slide results (RF): Grid 108 evals → 0.9648; Random(sklearn) 20 → 0.9648; Random(Optuna) 30 → 0.9621; Bayesian TPE 30 → 0.9643 (differences small on easy data). **Practical tips:** log scale for C, gamma, learning rate; **coarse-to-fine** search; fix a budget (n\_iter/n\_trials/time); prune early; tuning helps most when defaults are poor (DT 0.916 → 0.938; RF 0.963 → 0.965). **Review Q:** grid 3×4×5 with 5-fold = 60×5 = **300 fits**; 2-hour model → Bayesian optimisation.

---

## 4.10 Introduction to MLOps

### 4.10.1 ★ MLOps Overview

- **MLOps (Machine Learning Operations)** = engineering discipline of **systematic workflows, best practices and tools** supporting ML across **Scoping, Data, Modeling and Deployment**.
- **Unifies DevOps (software engineering) + DataOps (data pipeline governance) + ModelOps (model lifecycle)** so teams iterate rapidly instead of manual hand-offs.
- **POC-to-production gap:** a notebook model with good hold-out accuracy is only a *proof of concept*; reaching reliable production often needs another **\~6 months** of engineering. Live systems face **shifting data, latency/compute budgets and pipeline failures**. "Not an ML problem" is not an acceptable answer – the job is sustained end-to-end value.
- **★ The 5–10% rule (Sculley et al., NeurIPS 2015, *Hidden Technical Debt in ML Systems*):** ML model code is only **\~5–10% (or less)** of the codebase; >90% is surrounding infrastructure – configuration, data collection, data verification, feature extraction, resource management, analysis tools, serving infrastructure, monitoring.
- Case: **smartphone visual defect inspection** – camera → edge inspection software → API call with image X → prediction server returns Y (pass/reject). **Edge deployment** because the factory line cannot stop when the internet drops. Model trained on well-lit images fails when lighting darkens → **data drift**.

### 4.10.2 ★ ML Lifecycle & Production Challenges

**Four iterative stages:**

| Stage | Activities |
| --- | --- |
| **1. Scoping** | Define project goals, inputs X, outputs Y, key metrics, resources (accuracy, latency e.g. <500 ms, throughput QPS, compute, timeline) |
| **2. Data** | Define data, **establish baseline**, **label and organise** data (consistent labels!) |
| **3. Modeling** | Select/train model (Code + Hyperparameters + Data), **error analysis**, pre-deployment audit; loop back |
| **4. Deployment** | Deploy, monitor, maintain, retrain – first deployment = only **\~50% of the journey** |

**Label inconsistency (speech example):** the same audio "Um, today's weather" is labelled three ways (with "Um", without, with \<noise> tag) → confuses the learner → define **labelling conventions** (also silence padding 100/300/500 ms, volume normalisation).

**Model-centric vs Data-centric AI:** AI system = **Code + Hyperparameters + Data**. *Model-centric*: hold data fixed, improve model (academic). *Data-centric*: hold open-source code fixed, **improve data quality via error analysis** – usually faster in practice.

**Three milestones of modelling:** (1) good on **training set**; (2) good on **dev/test set**; (3) good on **business metrics / project goals** (low average test error is often *not enough*).

**Why low average error isn't enough:**

| Failure | Example | Fix |
| --- | --- | --- |
| Disproportionately important examples | **Navigational queries** ("youtube", "stanford") must be rank #1, informational queries ("apple pie recipe") tolerate #2–3 | Evaluate navigational slice separately |
| Fairness on key slices | Loan approval must not discriminate by ethnicity, gender, location, language; recommender must not ignore small brands | Slice-based audits, fairness checks |
| **Skewed data** | 99% negative → **print(0) gets 99% accuracy** | Use **precision/recall/F1**, not accuracy |
| **Rare classes** | Hernia (\~100 images) vs Effusion (\~10,000) – poor rare-class accuracy hides in average | Per-class metrics; collect targeted data |

**★ Statistical challenges – Drift:**

| Drift | What changes | Example |
| --- | --- | --- |
| **Data drift (covariate shift)** | Input distribution **P(X)** changes; X→Y mapping same | Larger houses built; new celebrity names in voice search; new phone microphones; darker factory lighting |
| **Concept drift** | Mapping **P(Y\|X)** changes; same X now has different Y | Inflation raises price of same-size house; COVID changes fraud patterns |

- Speed: **gradual** (vocabulary evolves) vs **sudden shock** (COVID-19). Consumer (B2C) data usually **slow** (months–years); enterprise/factory (B2B) can shift in **minutes–days** (change in casing, new workflow).
- **Temporal validation rule:** build dev/test sets from the **most recent months**.

**★ Software engineering checklist (6 points):**

| # | Dimension | Trade-off / example |
| --- | --- | --- |
| 1 | **Real-time vs Batch** | Real-time: ms API (voice search \~300 ms of 500 ms budget); batch: overnight (hospital EHR scoring) |
| 2 | **Cloud vs Edge/Browser** | Cloud: elastic GPUs; Edge: works offline (factory, in-car voice) |
| 3 | **Compute resources** | Compress/distil models trained on big GPUs to fit production CPU/RAM |
| 4 | **Latency & Throughput (QPS)** | Latency = single-request speed; QPS (e.g., 1,000) sizes the cluster |
| 5 | **Logging** | Log inputs X and predictions Y – for debugging and **retraining data** |
| 6 | **Security & Privacy** | Encryption, access control, consent (EHR, voice) |

### 4.10.3 Data & Feature Pipelines \[Supplement\]

- **Data pipeline:** automated flow **ingest → validate → clean/transform → label → store** (batch or streaming). **Data validation** checks schema, null rates, ranges, distributions (e.g., Great Expectations, TFX Data Validation).
- **Feature pipeline:** transforms raw data into model features (scaling, encoding, aggregates).
- **Feature store:** central repository of curated features with an **offline store** (training, historical) and **online store** (low-latency serving). Ensures **same feature logic for training and serving** → prevents **training–serving skew**. (Examples: Feast, Tecton.)
- Other: **data versioning** (DVC, lakeFS), lineage, point-in-time-correct joins to avoid **data leakage**, ETL/ELT.

### 4.10.4 Experiment Tracking & Model Versioning \[Supplement\]

- **Model = Code + Hyperparameters + Data**, so reproducibility needs **all three versioned**.
- **Experiment tracking** logs for each run: code commit, data version, hyperparameters, metrics, artifacts, environment, so runs can be compared and reproduced (tools: **MLflow**, Weights & Biases, Neptune, TensorBoard).
- **Versioning:** code → Git; data → DVC; models → **Model Registry** (versions with stages *Staging / Production / Archived*, metadata, lineage, approval workflow).
- Benefits: reproducibility, audit/compliance, quick **rollback**, collaboration.

### 4.10.5 ★ Model Deployment & Serving, Inference Systems

**Serving styles (slides):**

| Mode | Latency | Description | Example |
| --- | --- | --- | --- |
| **Batch inference** | minutes–hours | Score large batches (e.g., nightly), store predictions in DB for lookup | EHR scoring, daily recommendation tables |
| **Real-time (online) inference** | **milliseconds** | Synchronous API; user waits | Voice search, fraud check |
| **Streaming** \[Supplement\] | seconds | Scores events continuously from a stream (Kafka) | Anomaly detection |

Serve via **REST/gRPC API** (FastAPI/Flask, TF Serving, TorchServe, Triton), packaged in **Docker containers**, scaled by **Kubernetes**, run in **cloud** or at the **edge**. **Hybrid edge–cloud** example: phone runs lightweight **Voice Activity Detection (VAD)** at the edge and streams only speech to the cloud ASR server.

**★ Safe deployment patterns:** three scenarios: new capability; automating/assisting a human task; replacing an existing model.

| Pattern | How it works |
| --- | --- |
| **Shadow mode** | Model runs **in parallel** with human/old system but **its outputs are not used** for decisions – compare and gather data |
| **Canary deployment** | Model handles a **small fraction (e.g., 5%)** of live traffic; monitor, then **ramp up gradually** |
| **Blue-Green** | Two environments: **Blue = old, Green = new**; router switches traffic instantly and can **roll back in one step** |

**Rule:** never switch 100% of traffic to an unproven model; ramp gradually; keep a rollback path.

**Degrees of automation (Human-in-the-Loop):** **Human only → Shadow mode → AI assistance → Partial automation → Full automation.** AI assistance = UI highlights candidates, human decides; Partial automation = confident predictions auto-executed, uncertain ones **routed to humans (whose labels are high-value)**. Consumer web search needs full automation; factories often use human-in-the-loop.

### 4.10.6 ★ CI/CD/CT for ML Systems \[Supplement\]

| Term | Meaning in ML |
| --- | --- |
| **CI** (Continuous Integration) | Automatically **build & test** on every commit: unit tests, **data validation tests, model tests**, code quality |
| **CD** (Continuous Delivery/Deployment) | Automatically **package and deploy** the pipeline/model service (containers, canary/blue-green) |
| **CT** (Continuous Training) | Automatically **retrain** models on new data (scheduled, on drift trigger, or on new-data arrival) – **unique to ML** |
| (CM) Continuous Monitoring | Track performance and drift in production |

- **Google MLOps maturity levels:** **Level 0** – manual process; **Level 1** – **ML pipeline automation** (continuous training, automated data/model validation); **Level 2** – **CI/CD pipeline automation** (build/test/deploy the pipelines themselves).
- Tools: GitHub Actions, Jenkins, GitLab CI; Kubeflow, TFX, SageMaker Pipelines.

### 4.10.7 ★ Monitoring, Drift & Observability

- **Brainstorm what could go wrong**, track many metrics, prune useless ones.
- **Three metric families:**
  1. **Software metrics** – memory, CPU/GPU, **latency, throughput (QPS), server load**.
  2. **Input metrics (detect shift in X)** – average input length/volume, **fraction of missing values**, image brightness.
  3. **Output metrics (shift in Ŷ / user behaviour)** – null/empty ASR outputs, user repeating a search, switching from voice to typing, **CTR**.
- **Alarm thresholds** (e.g., server load > 0.91, missing-value spikes) refined over time. Triage: **software alerts → scale infrastructure; accuracy degradation → error analysis, data update, retraining.**
- **Retraining:** **manual** (engineer retrains, analyses, verifies) is far more common than **automatic** (mostly consumer internet).
- **★ 6-layer observability stack:**

  | Layer | Needs ground truth? |
  | --- | --- |
  | 1. Service health (latency, QPS, load) | **No** – seconds |
  | 2. Data quality (null rates, schema checks) | **No** |
  | 3. Input drift P(X) | **No** |
  | 4. Prediction drift P(Ŷ) | **No** |
  | 5. Model performance (accuracy, F1, precision, recall) | **Yes** – labels arrive days/weeks later |
  | 6. Business KPIs (CTR, retention, ROI) | **Yes** (delayed) |
  | Because labels arrive late, monitoring **only accuracy** means serving degraded predictions for weeks → layers 1–4 give **early warning**. Triage runbook: check **upstream data pipelines first**, then slice error analysis, then retrain. |  |
- **Observability** = ability to understand system internals from outputs (**metrics, logs, traces**) to find *why* something is wrong, beyond just *that* it is wrong.
- **Pipeline (cascading) monitoring:** a shift in an early module silently changes the input of downstream models – e.g., new microphone → VAD clips silence differently → ASR accuracy drops; clickstream shift → more "Unknown" in user-profile model → recommender degrades. Monitor **each module** and the end-to-end system.
- Drift detection \[Supplement\]: statistical tests (KS test, chi-square), PSI, KL divergence between training and live distributions.

### 4.10.8 ML Orchestration \[Supplement\]

- **Orchestration** = scheduling, coordinating and monitoring the **steps of an ML workflow** (ingest → validate → features → train → evaluate → register → deploy) as a **DAG (directed acyclic graph)**, with dependencies, retries, parameters, logging and alerts.
- **Tools:** Apache **Airflow**, **Kubeflow Pipelines**, Prefect, Dagster, Argo Workflows, TFX, SageMaker/Vertex AI Pipelines. Containers (**Docker**) + **Kubernetes** provide portable, scalable execution.
- **Triggers:** schedule, new data arrival, drift alarm, code change. Benefits: automation, reproducibility, fewer manual hand-offs, supports CT.

---

## 4.11 Unit 4 – Important Definitions, Facts, Formulae & Algorithms

| Term | Definition / formula |
| --- | --- |
| Clustering | Unsupervised grouping: min intra-cluster, max inter-cluster distance |
| Minkowski | (Σ\|xᵢ−yᵢ\|^r)^{1/r}; r=1 Manhattan, r=2 Euclidean |
| Single / Complete / Average linkage | min / max / mean of pairwise inter-cluster distances |
| AGNES / DIANA | Bottom-up merge / top-down split |
| Dendrogram | Tree of merges; branch height = merge distance; cut to get K |
| AGNES complexity | Space O(N²), time O(N³) (O(N²logN) possible) |
| K-means objective | J = ΣΣ rᵢₖ‖xᵢ−μₖ‖² (WCSS) |
| K-means steps | Assign to nearest centroid → recompute mean; repeat |
| K-means complexity | O(N·K·D·I) |
| Elbow method | Plot WCSS vs K; pick the bend; K=N gives WCSS=0 |
| Bisecting K-means | Repeatedly 2-means split the cluster with largest SSE until K |
| K-means vs EM | K-means = hard-assignment EM with equal spherical covariances |
| MDP | (S, A, P, R, γ) |
| Return | Gₜ = Σ γᵏ rₜ₊ₖ₊₁ |
| Vᵖ(s), Qᵖ(s,a) | Expected return from s / from (s,a) under π |
| Bellman optimality (Q) | Q\*(s,a) = R + γ max Q\*(s',a') |
| Q-learning update | Q ← Q + α\[r + γ max Q(s',a') − Q\] |
| ε-greedy | Random action w.p. ε, greedy w.p. 1−ε |
| Conv output size | ⌊(n−f+2p)/s⌋ + 1 |
| Same padding | p = (f−1)/2 |
| Conv parameters | (f·f·c\_in + 1)·k |
| Pooling | No parameters; output ⌊(n−f)/s⌋+1; channels unchanged |
| Grid search cost | Π(values) × folds |
| Random search | n\_iter samples from distributions |
| Bayesian opt. | Surrogate (GP/TPE) + acquisition (EI/UCB = μ+κσ) |
| Data drift / Concept drift | P(X) changes / P(Y\|X) changes |
| 5–10% rule | ML code is only 5–10% of production system |
| Shadow / Canary / Blue-Green | Parallel, no impact / small % traffic / instant switch + rollback |
| CI / CD / CT | Test+integrate / deliver+deploy / retrain automatically |

## 4.12 Unit 4 – Frequently Confused Concepts

| Confusion | Clarification |
| --- | --- |
| Hierarchical vs K-means | Dendrogram, no K needed, irreversible vs flat, K needed, reassignable |
| AGNES vs DIANA | Merge bottom-up vs split top-down |
| Single vs complete linkage | Min distance (chaining) vs max distance (compact) |
| K-means vs Bisecting K-means | All K at once vs repeated 2-means on largest-SSE cluster |
| K-means vs GMM | Hard vs soft; spherical vs elliptical |
| Hard vs soft clustering | One cluster vs probabilities |
| Reward vs return vs value | Immediate vs discounted sum vs expected return |
| Policy vs value function | Action rule vs expected return |
| V(s) vs Q(s,a) | State value vs state-action value |
| Q-learning vs SARSA | Off-policy (max) vs on-policy (taken action) |
| Exploration vs exploitation | Random vs greedy action |
| Learning rate α vs discount γ | Update speed vs weight of future reward |
| Parameter vs hyperparameter | Learned vs chosen |
| Valid vs same padding | p=0 vs p=(f−1)/2 |
| Max vs average pooling | Max vs mean; neither learns parameters |
| Grid vs random vs Bayesian | Exhaustive vs random vs model-guided |
| Data drift vs concept drift | P(X) vs P(Y\|X) |
| Shadow vs canary vs blue-green | No decisions vs small traffic vs full parallel environments |
| Batch vs real-time inference | Hours vs milliseconds |
| CI vs CD vs CT | Integrate/test vs deliver vs retrain |
| Model-centric vs data-centric | Improve code vs improve data |
| Software vs input vs output metrics | System health vs X shifts vs Ŷ/user behaviour |

## 4.13 Unit 4 – Potential 4-Mark Questions with Model Answers

**Q1. Differentiate hierarchical and non-hierarchical clustering. Explain agglomerative clustering with a dendrogram. (4)** *Answer:* Hierarchical builds a tree (dendrogram) of nested clusters, needs no K in advance (cut dendrogram), merges are irreversible, cost O(N³)/O(N²) space; non-hierarchical (partitional, e.g., K-means) produces a flat partition into K clusters, needs K, can reassign points, scales better. **Agglomerative (AGNES):** start with N singleton clusters; compute distance matrix; merge the two closest clusters; update distances (single/complete/average linkage); repeat until one cluster. Dendrogram branch height = merge distance; cutting at a level gives clusters.

**Q2. Perform single-linkage agglomerative clustering on A(1,1), B(1.5,1.5), C(5,5), D(3,4), E(4,4), F(3,3.5). (4)** *Answer:* Closest pair D–F (0.50) → {D,F}; then A–B (0.71) → {A,B}; {D,F}–E = 1.00 → {D,F,E}; with C = 1.41 → {D,F,E,C}; finally {A,B}–{D,F,E,C} = 2.50 (B–F) → single cluster. Heights 0.50, 0.71, 1.00, 1.41, 2.50.

**Q3. Explain the K-means algorithm with objective function. Apply to A(1,1), B(2,1), C(4,3), D(5,4) with K=2 and initial centroids A and B. (4)** *Answer:* J=Σ‖x−μ‖². Iter 0: A→c₁; B,C,D→c₂; c₁=(1,1), c₂=(11/3,8/3). Iter 1: distances to c₁: 0,1,3.61,5; to c₂: 3.14,2.36,0.47,1.89 → {A,B},{C,D}; new centroids (1.5,1), (4.5,3.5). Next iteration unchanged → converged.

**Q4. Discuss limitations of K-means and how they can be addressed. Explain the elbow method. (4)** *Answer:* Limitations: dependent on initial centroids (local minima), spherical clusters, unequal size/density, scale sensitivity, outliers, K must be given. Fixes: multiple/more-spread initial centroids (K-means++), over-cluster then hierarchical, rescale to \[0,1\], kernel transform. **Elbow:** plot WCSS vs K; WCSS falls as K grows (0 at K=N); choose K at the bend beyond which reduction is small.

**Q5. Explain Bisecting K-means and compare with K-means. (4)** *Answer:* Start with all data as one cluster; split with 2-means; pick the cluster with largest SSE; split again; repeat until K leaves. Advantages: efficient for large K, balanced cluster sizes, hierarchy produced, less initialisation sensitivity. K-means forms all K at once and may give very different sizes.

**Q6. Show that K-means is a special case of EM. (4)** *Answer:* In GMM-EM, E-step computes soft responsibilities, M-step updates μ, Σ, π. If all Gaussians have equal spherical covariance σ²I and equal priors and σ²→0, responsibilities become 0/1 (nearest centroid) – E-step = assign to closest centroid; M-step = centroid is mean of assigned points. Hence K-means = hard EM.

**Q7. Define RL and explain agent, environment, state, action, reward, policy and value function with the interaction loop. (4)** *Answer:* (definition + loop: agent observes sₜ, takes aₜ, environment gives rₜ₊₁, sₜ₊₁). Define each element as in 4.6.2; policy maps states to actions (deterministic/stochastic); value function = expected cumulative discounted reward; goal = optimal policy maximising return.

**Q8. Explain MDP, discount factor, value functions and the Bellman equation. (4)** *Answer:* MDP=(S,A,P,R,γ). Discount γ∈\[0,1) weights future rewards (γ=0 myopic). Vᵖ(s)=E\[Gₜ|s\], Qᵖ(s,a)=E\[Gₜ|s,a\]. Bellman: Vᵖ(s)=Σπ(a|s)ΣP(s'|s,a)\[R+γVᵖ(s')\]; optimality: Q\*(s,a)=R+γ max Q\*(s',a'); π\*(s)=argmax Q\*(s,a).

**Q9. Explain Q-learning and compute two updates (α=0.5, γ=0.9, A→Right (−1)→B→Down (+10)→G). (4)** *Answer:* Rule Q←Q+α\[r+γ max Q(s',·)−Q\]. Q(B,Down)=0+0.5(10+0−0)=5; Q(A,Right)=0+0.5(−1+0.9×5−0)=1.75; episode 2: 7.5 and 3.75. Mention ε-greedy, off-policy, model-free.

**Q10. Explain convolution, padding, stride and derive output size. Compute output for 32×32 input, 5×5 filter, p=2, s=1. (4)** *Answer:* Filter slides, multiply-sum to form feature map; padding adds zeros (valid p=0, same p=(f−1)/2); stride = step size. O=⌊(n−f+2p)/s⌋+1 = (32−5+4)/1+1 = **32** (same convolution).

**Q11. Calculate the number of parameters: Conv(5×5, input depth 3, 8 filters) → Conv(5×5, 16 filters) → flatten 400 → FC100 → FC84 → Softmax10. (4)** *Answer:* 608 + 3,216 + 40,100 + 8,484 + 850 = **53,258** (pooling = 0; show each formula).

**Q12. Explain max pooling and average pooling with an example; why use pooling? (4)** *Answer:* 4×4 input, f=2, s=2: max pool picks 9,2,6,3; average gives 3.75,1.25,3.75,2. Pooling reduces size, speeds computation, adds robustness; no parameters; channels preserved; output ⌊(n−f)/s⌋+1.

**Q13. Why CNNs instead of fully connected networks for images? (4)** *Answer:* 1000×1000×3 = 3M inputs → 3 billion weights; overfitting; spatial structure ignored. CNN: local receptive fields, parameter sharing, hierarchical features, translation tolerance via pooling, far fewer parameters.

**Q14. Compare Grid Search, Random Search and Bayesian Optimization. (4)** *Answer:* Table in 4.9.6 + why random beats grid (few hyperparameters matter, new values each trial), Bayesian uses surrogate+acquisition and is best for expensive evaluations; always tune with CV, never on test set.

**Q15. Explain Bayesian optimization components and the exploration–exploitation trade-off. (4)** *Answer:* Surrogate (GP/TPE) gives μ and σ; acquisition (EI, UCB=μ+κσ) picks next point; evaluate, update, repeat; exploit high μ vs explore high σ.

**Q16. What is MLOps? Explain the ML lifecycle and why the POC-to-production gap exists. (4)** *Answer:* MLOps = DevOps+DataOps+ModelOps; stages Scoping, Data, Modeling, Deployment (iterative; deployment is \~50%); ML code only 5–10% of system; live data shifts, latency, pipeline failures.

**Q17. Explain data drift and concept drift with examples; how are they detected/handled? (4)** *Answer:* P(X) vs P(Y|X) with examples (lighting, house size / inflation, COVID fraud); monitor input metrics, compare distributions, rolling dev/test sets from recent data, retrain with new data.

**Q18. Explain shadow, canary and blue-green deployment and degrees of automation. (4)** *Answer:* As in 4.10.5; spectrum Human only → Shadow → AI assistance → Partial → Full.

**Q19. Explain monitoring and the 6-layer observability stack. (4)** *Answer:* Software/input/output metrics; layers 1–4 need no labels (early warning); layers 5–6 need ground truth (delayed); thresholds and triage; cascading drift in pipelines.

**Q20. Explain CI/CD/CT, experiment tracking, model versioning and orchestration. (4)** *Answer:* CI=test/integrate (code, data, model tests); CD=deploy; CT=retrain automatically; tracking logs code/data/hyperparameters/metrics (MLflow); registry versions models with stages; orchestration = DAG scheduling (Airflow/Kubeflow).

**Q21. List the software-engineering checklist for deploying a prediction service. (4)** *Answer:* Real-time vs batch; cloud vs edge; compute; latency/QPS; logging; security/privacy – with one example each.

**Q22. Why isn't low average test error enough? (4)** *Answer:* Navigational queries, fairness slices, skewed data (print(0) = 99%), rare classes (Hernia) – use slice analysis, precision/recall, per-class metrics.

## 4.14 Unit 4 – Practice 2-Mark MCQs (with explanations)

**U4-2M-1.** 1-D points 1, 2, 6, 7; K-means with K=2 and initial centroids 1 and 2. The final centroids are: (a) (1, 6) (b) (1.5, 6.5) (c) (2, 6) (d) (1, 5) **Ans: (b).** Iter 1: 1→c₁; 2,6,7→c₂ → (1, 5). Iter 2: 1,2→c₁ (|2−1|=1<|2−5|=3); 6,7→c₂ → (1.5, 6.5); stable.

**U4-2M-2.** Clusters {A,B}, {C,D} with distances AC=4, AD=6, BC=3, BD=5. Single-link, complete-link and average-link distances are: (a) 3, 6, 4.5 (b) 6, 3, 4.5 (c) 4.5, 3, 6 (d) 3, 4.5, 6 **Ans: (a).** min=3, max=6, mean=(4+6+3+5)/4=4.5.

**U4-2M-3.** Q-learning: Q(s,a)=2, α=0.5, r=4, γ=0.9, max Q(s',a')=10. Updated Q(s,a)= (a) 6.5 (b) 7.5 (c) 11 (d) 5.5 **Ans: (b).** 2 + 0.5(4 + 9 − 2) = 2 + 5.5 = 7.5.

**U4-2M-4.** Input 28×28, filter 3×3, padding 1, stride 2. Output size = (a) 13×13 (b) 14×14 (c) 15×15 (d) 28×28 **Ans: (b).** ⌊(28−3+2)/2⌋+1 = ⌊13.5⌋+1 = 14.

**U4-2M-5.** A conv layer has 32 filters of size 3×3 on an input with 16 channels (with biases). Parameters = (a) 4,608 (b) 4,640 (c) 288 (d) 9,248 **Ans: (b).** (3·3·16+1)·32 = 145·32 = 4,640 (4,608 without biases).

**U4-2M-6.** GridSearchCV over 3, 4, 5 and 2 values of four hyperparameters with 5-fold CV needs how many model fits? (a) 120 (b) 600 (c) 24 (d) 14 **Ans: (b).** 3·4·5·2 = 120 combinations × 5 = 600.

**U4-2M-7.** Rewards rₜ₊₁=1, rₜ₊₂=2, rₜ₊₃=3, γ=0.5. Return Gₜ = (a) 6 (b) 2.75 (c) 3.5 (d) 1.75 **Ans: (b).** 1 + 0.5×2 + 0.25×3 = 2.75.

**U4-2M-8.** Clusters {1,3} and {8,10} (1-D). The WCSS is: (a) 2 (b) 4 (c) 8 (d) 16 **Ans: (b).** Centroids 2 and 9: (1+1)+(1+1) = 4.

**U4-2M-9.** 7×7 input, 3×3 max-pool, stride 2 gives output: (a) 2×2 (b) 3×3 (c) 4×4 (d) 5×5 **Ans: (b).** ⌊(7−3)/2⌋+1 = 3.

**U4-2M-10.** Which monitoring layers need **no ground-truth labels**? (a) Service health, data quality, input drift, prediction drift (b) Accuracy and F1 (c) CTR and ROI (d) None **Ans: (a).** Layers 1–4 are early-warning; accuracy/F1 and business KPIs need delayed labels.

**U4-2M-11.** Using Bisecting K-means to get K=4 clusters from one cluster needs how many 2-means splits? (a) 2 (b) 3 (c) 4 (d) 5 **Ans: (b).** Each split adds one cluster: 1→2→3→4 = 3 splits.

**U4-2M-12.** A 7×7 filter must give “same” convolution at stride 1. Padding p = (a) 2 (b) 3 (c) 4 (d) 7 **Ans: (b).** p=(f−1)/2 = 3.

## 4.15 Unit 4 – Practice 1-Mark MCQs (with explanations)

1. Clustering is: (a) supervised (b) unsupervised (c) reinforcement (d) semi-supervised. **Ans (b).**
2. Good clustering: (a) min inter, max intra (b) max inter, min intra distance (c) both max (d) both min. **Ans (b).**
3. Minkowski with r=1 is: (a) Euclidean (b) Manhattan (c) Cosine (d) Hamming. **Ans (b).**
4. Single linkage uses: (a) farthest pair (b) closest pair (c) mean of pairs (d) centroids. **Ans (b).**
5. AGNES is: (a) top-down (b) bottom-up (c) partitional (d) density-based. **Ans (b).**
6. DIANA is: (a) agglomerative (b) divisive (c) K-means (d) DBSCAN. **Ans (b).**
7. AGNES time complexity: (a) O(N) (b) O(N²) (c) O(N³) (d) O(log N). **Ans (c).** Space is O(N²).
8. In hierarchical clustering a merge: (a) can be undone (b) cannot be undone (c) is random (d) needs K. **Ans (b).**
9. A dendrogram branch height represents: (a) cluster size (b) merge distance (c) number of features (d) K. **Ans (b).**
10. K-means is a \_\_\_ clustering: (a) hierarchical (b) partitional/hard (c) soft (d) density. **Ans (b).**
11. K-means centroid update: (a) median (b) mean (c) mode (d) random. **Ans (b).**
12. K = N gives WCSS: (a) maximum (b) 0 (c) 1 (d) undefined. **Ans (b).**
13. Elbow method plots: (a) WCSS vs K (b) accuracy vs K (c) loss vs epoch (d) K vs time. **Ans (a).**
14. K-means result depends on: (a) initial centroids (b) nothing (c) labels (d) dendrogram. **Ans (a).**
15. Bisecting K-means splits the cluster with: (a) smallest SSE (b) largest SSE (c) fewest points (d) random. **Ans (b).**
16. K-means is a special case of: (a) Gibbs (b) EM (c) boosting (d) Viterbi. **Ans (b).**
17. GMM clustering is: (a) hard (b) soft (c) hierarchical (d) none. **Ans (b).**
18. In RL the agent receives: (a) labels (b) rewards (c) clusters (d) gradients only. **Ans (b).**
19. A policy maps: (a) states to actions (b) actions to rewards (c) states to rewards (d) rewards to states. **Ans (a).**
20. MDP tuple: (a) (S,A,P,R,γ) (b) (X,Y) (c) (A,B,π) (d) (S,π). **Ans (a).**
21. γ = 0 means the agent: (a) cares only about immediate reward (b) values future equally (c) never learns (d) explores only. **Ans (a).**
22. Q-learning is: (a) model-based on-policy (b) model-free off-policy (c) supervised (d) unsupervised. **Ans (b).**
23. ε-greedy with ε=0.2 explores: (a) 80% (b) 20% (c) 0% (d) 100%. **Ans (b).**
24. Q-table entries represent: (a) rewards (b) estimated long-term action values (c) probabilities of states (d) policies. **Ans (b).**
25. Future Q at a terminal state is: (a) max (b) 0 (c) 1 (d) −1. **Ans (b).**
26. Output size of conv: (a) (n−f+2p)/s+1 (b) (n+f)/s (c) n·f/s (d) n−f. **Ans (a).**
27. Same padding requires: (a) p=0 (b) p=(f−1)/2 (c) p=f (d) p=s. **Ans (b).**
28. Pooling layer has \_\_\_ learnable parameters: (a) many (b) f² (c) zero (d) k. **Ans (c).**
29. Max pooling keeps: (a) mean (b) maximum value in window (c) minimum (d) median. **Ans (b).**
30. Which layer typically has the most parameters? (a) conv (b) pooling (c) fully connected (d) ReLU. **Ans (c).**
31. Number of output channels of a conv layer equals: (a) input channels (b) number of filters (c) filter size (d) stride. **Ans (b).**
32. Hyperparameters are: (a) learned from data (b) set before training (c) weights (d) gradients. **Ans (b).**
33. Grid search cost: (a) additive (b) multiplicative in hyperparameter values (c) constant (d) log. **Ans (b).**
34. Random search is often better than grid because: (a) it is exhaustive (b) it tries more distinct values of important hyperparameters (c) it uses the test set (d) it needs fewer features. **Ans (b).**
35. Bayesian optimization uses: (a) surrogate + acquisition (b) exhaustive grid (c) random only (d) none. **Ans (a).**
36. UCB acquisition = (a) μ − σ (b) μ + κσ (c) μσ (d) σ². **Ans (b).**
37. Hyperparameter tuning should use: (a) test set (b) cross-validation on training data (c) validation of final test only (d) no validation. **Ans (b).**
38. ML code share in production system: (a) 90% (b) 50% (c) 5–10% (d) 100%. **Ans (c).**
39. Change in P(X) only is: (a) concept drift (b) data drift (c) label drift only (d) no drift. **Ans (b).**
40. Change in P(Y|X) is: (a) data drift (b) concept drift (c) no drift (d) overfitting. **Ans (b).**
41. Model runs in parallel without affecting decisions: (a) canary (b) shadow mode (c) blue-green (d) full automation. **Ans (b).**
42. Small percentage of live traffic goes to new model: (a) shadow (b) canary (c) batch (d) edge. **Ans (b).**
43. Blue-green deployment provides: (a) instant switch and rollback (b) no testing (c) only batch (d) A/B only. **Ans (a).**
44. Continuous Training (CT) means: (a) auto-retraining (b) auto-testing code (c) manual deploy (d) logging. **Ans (a).**
45. A skewed dataset (99% negative) “print(0)” accuracy is: (a) 1% (b) 50% (c) 99% (d) 0%. **Ans (c)** – so use precision/recall.
46. Which is NOT a software metric? (a) latency (b) QPS (c) server load (d) fraction of missing values. **Ans (d)** – an input metric.
47. Feature store prevents: (a) training–serving skew (b) overfitting (c) vanishing gradients (d) bagging. **Ans (a).**
48. Experiment tracking tool example: (a) MLflow (b) Optuna only (c) Docker (d) NumPy. **Ans (a).**

## 4.16 Unit 4 – Compact Revision Sheet

**Clustering:** unsupervised; min intra/max inter. Minkowski r=1/2. Linkages: single=min, complete=max, average. **AGNES** bottom-up, space O(N²), time O(N³), irreversible; **DIANA** top-down. Dendrogram cut → K. Example merges: DF 0.50, AB 0.71, +E 1.00, +C 1.41, join 2.50. **K-means:** J=Σ‖x−μ‖²; assign → mean; O(NKDI); local minimum; elbow; K=N→0; spherical/scale sensitive; example A,B,C,D → {A,B} (1.5,1) & {C,D} (4.5,3.5). **Bisecting:** 2-means on largest SSE until K. **K-means = hard EM** (equal spherical σ²→0).

**RL:** agent–environment–state–action–reward; policy; V, Q; MDP (S,A,P,R,γ); G=Σγᵏr; Bellman Q\*=R+γmaxQ\*; **Q-learning** Q←Q+α\[r+γmaxQ'−Q\], α=0.5,γ=0.9: 5→7.5→8.75 and 1.75→3.75→5.31; ε-greedy; off-policy, model-free.

**DL/CNN:** DL = many-layer learned features. FC on 1000×1000×3 = 3B weights. O=⌊(n−f+2p)/s⌋+1; same p=(f−1)/2; params=(f·f·c+1)k; pooling 0 params, channels kept; example total 53,258 (608+3,216+40,100+8,484+850).

**Tuning:** parameters learned vs hyperparameters set. Grid = Πvalues×folds (24×5=120); Random = n\_iter, distributions (more distinct values); Bayesian = surrogate (GP/TPE)+acquisition (EI/UCB), for expensive models. Tune with CV, test once.

**MLOps:** DevOps+DataOps+ModelOps; 5–10% rule; lifecycle Scoping→Data→Modeling→Deployment (50%); data-centric AI; drift: P(X) vs P(Y|X); 6-point checklist (real-time/batch, cloud/edge, compute, latency/QPS, logging, security); shadow→canary→blue-green; HITL spectrum; metrics: software/input/output; 6-layer observability (1–4 no labels); CI/CD/CT; feature store, MLflow/registry, Airflow/Kubeflow orchestration.

---

# SYLLABUS COVERAGE CROSS-CHECK

| Syllabus topic | Where covered | Source |
| --- | --- | --- |
| Combining weak learners | 3.1 (ensembles), 3.2 (bagging), 3.3 (boosting) | Slides |
| Improving performance with Gradient Boost | 3.4 (worked example, vs AdaBoost) | Slides (images) + supplement for formulas |
| Random Forest | 3.5 (algorithm, OOB, missing data, proximity) | Slides |
| Bayes theorem | 3.6 (probability basics, total probability, Bayes) | Slides |
| Maximum likelihood | 3.7 (MAP, ML, least squares), 3.8 (brute-force MAP) | Slides |
| Bayes optimal classifier | 3.9 (+ Gibbs) | Slides |
| Naïve Bayes | 3.10 (parameters, types, smoothing, underflow, text, examples) | Slides |
| Expectation Maximization & GMM | 3.11, 3.12 (numerical example) | Slides |
| HMMs | 3.13 (Markov chain, 3 problems, Forward/Backward/Viterbi/Baum-Welch) | Slides |
| Hierarchical vs non-hierarchical | 4.1.2 | Slides |
| Agglomerative and divisive | 4.2 (AGNES example, DIANA) | Slides + supplement for DIANA |
| K-means | 4.3 (objective, algorithm, example, elbow) | Slides |
| Bisecting K-means | 4.4 | Slides |
| K-means as special case of EM | 4.5 | Slides + derivation note |
| RL: agent, environment, states, actions, rewards, policy | 4.6.1–4.6.4 | Slides |
| Value functions, Bellman equation | 4.6.5 | Slides (partial) + supplement |
| Q-learning fundamentals | 4.6.6 | Slides |
| Introduction to Deep Learning | 4.7 | **Supplement** (not in slides) |
| Convolution operation & CNN (parameter calc, max/avg pooling) | 4.8 | Slides |
| Grid / Random / Bayesian optimisation | 4.9 | Slides |
| MLOps overview | 4.10.1 | Slides |
| ML lifecycle & production challenges | 4.10.2 | Slides |
| Data & feature pipelines | 4.10.3 | **Supplement** |
| Experiment tracking & model versioning | 4.10.4 | **Supplement** |
| Model deployment & serving; inference systems | 4.10.5 | Slides |
| CI/CD/CT | 4.10.6 | **Supplement** |
| Monitoring, drift & observability | 4.10.7 | Slides |
| ML orchestration | 4.10.8 | **Supplement** |

**Slide quirks to know:** total-probability slide uses 0.45 where the text says 0.42; the alpha table prints 0.3456 where the correct value is 0.03456; the HMM slides mention M=3 observation symbols but the example uses only Happy/Grumpy; Bayes-optimal Example 3 probabilities (0.5 + 0.49) don't sum to 1 because of rounding of the weights.

---

# MOCK EXAMINATION (40 Marks) – Unit 3 & Unit 4

**Time:** 90 minutes. **Do not scroll to the Answer Key until you have finished.** Pattern: Section A – 16 × 1 mark; Section B – 4 × 2 marks; Section C – 4 × 4 marks.

## Section A – 1-mark MCQs (16 marks; 8 from each unit)

**Unit 3** **A1.** Which statement is **not** true of bagging? (a) uses bootstrap samples (b) base models can be trained in parallel (c) each model's vote is weighted by its training accuracy (d) reduces variance **A2.** In gradient boosting for regression with squared loss, the initial model F₀ is: (a) zero (b) a decision stump (c) the mean of the target (d) a random tree **A3.** The OOB error of a Random Forest for an observation uses: (a) a separate test set (b) only the trees that did not see that observation in their bootstrap sample (c) all trees (d) the training error **A4.** Under Gaussian noise, the maximum-likelihood hypothesis minimises: (a) sum of absolute errors (b) sum of squared errors (c) maximum error (d) cross-entropy only **A5.** In Naïve Bayes, “naïve” refers to the assumption that: (a) priors are equal (b) features are conditionally independent given the class (c) classes are independent (d) data is noise-free **A6.** Laplace smoothing for a categorical attribute: (a) subtracts 1 from counts (b) adds 1 to each count (c) multiplies by the prior (d) takes logs **A7.** The E-step of EM for a GMM outputs: (a) new covariance matrices (b) responsibilities (soft assignments) (c) cluster centroids only (d) a final label **A8.** The Viterbi algorithm has time complexity: (a) O(Nᵀ) (b) O(N²T) (c) O(T) (d) O(N T²)

**Unit 4** **A9.** Agglomerative clustering begins with: (a) one cluster containing all points (b) N singleton clusters (c) K random centroids (d) a density threshold **A10.** The K-means objective minimises: (a) distances between centroids (b) the sum of squared distances of points to their cluster centroid (c) cluster count (d) variance between clusters **A11.** Bisecting K-means next splits the cluster that has: (a) the fewest points (b) the largest SSE (c) the smallest SSE (d) a random choice **A12.** Q-learning is called off-policy because its update target uses: (a) random actions (b) the max over next-state actions (c) no reward (d) the policy gradient **A13.** A 5×5 filter with stride 1 needs which padding for “same” output size? (a) 0 (b) 1 (c) 2 (d) 5 **A14.** The number of learnable parameters in a max-pooling layer is: (a) f² (b) f·c (c) 0 (d) equal to the number of filters **A15.** Compared with grid search at the same budget, random search often wins because it: (a) is exhaustive (b) tries more distinct values of each hyperparameter (c) uses the test set (d) learns a surrogate **A16.** A change in the relationship P(Y|X) while P(X) stays the same is called: (a) data drift (b) concept drift (c) covariate shift (d) skew

## Section B – 2-mark MCQs (8 marks; 2 from each unit)

**B1 (Unit 3).** In a spam filter P(spam)=0.4; a word occurs in 50% of spam and 10% of non-spam mails. P(spam | word) = (a) 0.50 (b) 0.77 (c) 0.20 (d) 0.40 **B2 (Unit 3).** Gradient boosting: target y=10, F₀=6, learning rate 0.5, first tree outputs 4 for this sample. New prediction and residual: (a) 8 and 2 (b) 10 and 0 (c) 8 and 4 (d) 6 and 4 **B3 (Unit 4).** A conv layer applies 10 filters of size 5×5 to a 32×32×3 input (with biases). Number of parameters: (a) 750 (b) 760 (c) 7,600 (d) 250 **B4 (Unit 4).** GridSearchCV with hyperparameters having 3, 3 and 2 values and 4-fold CV performs how many fits (without the final refit)? (a) 18 (b) 24 (c) 72 (d) 8

## Section C – Descriptive (16 marks; 4 × 4)

**C1 (Unit 3, 4 marks).** Explain the Random Forest algorithm. State how it differs from bagging and from boosting, and explain how the out-of-bag error is obtained.

**C2 (Unit 3, 4 marks).** An HMM has states H and L, π = (0.6, 0.4); transitions a\_HH=0.7, a\_HL=0.3, a\_LH=0.4, a\_LL=0.6; emissions b\_H(A)=0.8, b\_H(B)=0.2, b\_L(A)=0.3, b\_L(B)=0.7. (a) Name the parameters of an HMM and its two assumptions (1 mark). (b) Use the Forward algorithm to compute P(O) for O = (A, B) (3 marks).

**C3 (Unit 4, 4 marks).** Apply K-means (K=2) to P1(1,2), P2(2,1), P3(6,5), P4(7,7) using P1 and P3 as initial centroids. Show the assignments and updated centroids until convergence.

**C4 (Unit 4, 4 marks).** (a) Write the Q-learning update rule and explain each term, and why Q-learning is off-policy and uses ε-greedy (2 marks). (b) Given Q(s,a)=4, r=−1, γ=0.9, α=0.5 and max Q(s',a')=6, compute the updated Q(s,a) (2 marks).

---

# ANSWER KEY & MODEL ANSWERS (attempt the mock first!)

## Section A answers

| Q | Ans | Reason |
| --- | --- | --- |
| A1 | **(c)** | Bagging uses equal votes/average; weighted votes belong to boosting |
| A2 | **(c)** | F₀ = mean(y) |
| A3 | **(b)** | Each observation is predicted only by trees where it was out-of-bag |
| A4 | **(b)** | ML + Gaussian noise ⇔ least squares |
| A5 | **(b)** | Conditional independence given the class |
| A6 | **(b)** | Add-one smoothing |
| A7 | **(b)** | γᵢⱼ = soft responsibilities |
| A8 | **(b)** | O(N²T) |
| A9 | **(b)** | N singletons, merge bottom-up |
| A10 | **(b)** | WCSS / SSE |
| A11 | **(b)** | Largest SSE is split |
| A12 | **(b)** | max over next actions |
| A13 | **(c)** | p=(5−1)/2=2 |
| A14 | **(c)** | Pooling is a fixed operation |
| A15 | **(b)** | A new value of every hyperparameter each trial |
| A16 | **(b)** | Concept drift |

## Section B answers

- **B1 = (b) 0.77.** Spam: 0.4×0.5 = 0.20; non-spam: 0.6×0.1 = 0.06; P(spam|word) = 0.20/0.26 = **0.769**.
- **B2 = (a).** F₁ = 6 + 0.5×4 = **8**; residual 10 − 8 = **2**.
- **B3 = (b) 760.** (5·5·3 + 1)×10 = 76×10 = 760 (750 without biases).
- **B4 = (c) 72.** 3×3×2 = 18 combinations × 4 folds = 72.

## Section C model answers (with marking scheme)

**C1 (4 marks)**

- *Algorithm (2):* (1) draw a bootstrap sample (with replacement, same size); (2) grow a decision tree, considering at each split only a random subset of R < M features, choose the best split among them; grow fully (no pruning); (3) repeat to build many trees; (4) predict by majority vote (average for regression).
- *Differences (1):* **Bagging** = bootstrap + all features at each split, trees can be highly correlated; RF adds **random feature subsets** to decorrelate trees, giving a larger variance reduction. **Boosting** builds models **sequentially** on reweighted data/residuals with weighted voting and reduces bias; RF builds trees **in parallel**, independently, with equal votes.
- *OOB (1):* \~1/3 of samples are left out of each bootstrap. Predict each observation using only the trees for which it was OOB (majority vote), compare with the true label and aggregate over all observations → an unbiased internal error estimate without a separate validation set.

**C2 (4 marks)**

- *(a) (1):* λ = (A, B, π): transition matrix A, emission matrix B, initial distribution π. Assumptions: **Markov** (state depends only on the previous state) and **output independence** (observation depends only on the current state).
- *(b) (3):*
  - α₁(H) = 0.6×0.8 = **0.48**; α₁(L) = 0.4×0.3 = **0.12**
  - α₂(H) = (0.48×0.7 + 0.12×0.4)×b\_H(B) = (0.336 + 0.048)×0.2 = **0.0768**
  - α₂(L) = (0.48×0.3 + 0.12×0.6)×b\_L(B) = (0.144 + 0.072)×0.7 = **0.1512**
  - **P(O) = 0.0768 + 0.1512 = 0.228**
- *Bonus check (Viterbi):* δ₂(H) = 0.336×0.2 = 0.0672, δ₂(L) = 0.144×0.7 = 0.1008 → best path H → L with probability 0.1008 (≤ 0.228).

**C3 (4 marks)**

- Initial c₁ = P1(1,2), c₂ = P3(6,5).
- *Iteration 1:* distances to c₁: P1 0, P2 √2 = 1.41, P3 √34 = 5.83, P4 √61 = 7.81. To c₂: P1 5.83, P2 5.66, P3 0, P4 √5 = 2.24. Clusters: **{P1,P2}** and **{P3,P4}**. New centroids: c₁ = (1.5, 1.5), c₂ = (6.5, 6).
- *Iteration 2:* to c₁(1.5,1.5): P1 0.71, P2 0.71, P3 5.70, P4 7.78. To c₂(6.5,6): P1 6.80, P2 6.73, P3 1.12, P4 1.12. Assignments unchanged → **converged**.
- Final: {P1,P2} with centroid (1.5,1.5); {P3,P4} with centroid (6.5,6).
- Marks: initial assignment (1), centroid update (1), second iteration (1), convergence statement (1).

**C4 (4 marks)**

- *(a) (2):* **Q(s,a) ← Q(s,a) + α\[ r + γ maxₐ' Q(s',a') − Q(s,a) \]**; α = learning rate, γ = discount factor, r = immediate reward, max Q(s',a') = best estimated future value (0 at terminal state), bracket = TD error. **Off-policy:** the target uses the max over next actions, not the action the behaviour policy actually takes. **ε-greedy:** with probability ε take a random action (explore), otherwise the highest-Q action (exploit), so the agent can discover better routes.
- *(b) (2):* Q\_new = 4 + 0.5×\[−1 + 0.9×6 − 4\] = 4 + 0.5×(−1 + 5.4 − 4) = 4 + 0.5×0.4 = **4.2**.

## Scoring guide

**Section A /16 + Section B /8 + Section C /16 = /40.** Below 24 → revisit notes (start with the ★ sections); 24–32 → revise formulas and worked examples; above 32 → exam-ready, practise the numericals (Forward/Viterbi, K-means, Q-learning, CNN parameters, GMM).
