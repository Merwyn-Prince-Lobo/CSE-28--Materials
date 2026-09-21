# Unit 1 — Introduction, Performance Metrics, Classification with Decision Trees and KNN

---

## 1. Introduction to Machine Learning

**What it is:** Machine Learning (ML) is the study of algorithms that improve their performance on a task **T**, measured by **P** (performance metric), through **experience E** (data), without being explicitly programmed with rules.

**Analogy:** Traditional programming is like giving someone a recipe (rules) to cook a dish. Machine learning is like giving someone hundreds of tasted dishes (data) and letting them *figure out the recipe themselves*.

**Types of ML models:**
| Type | Idea | Example |
|---|---|---|
| Supervised Learning | Learn from labeled data (input → output) | Spam detection |
| Unsupervised Learning | Find structure in unlabeled data | Customer segmentation |
| Reinforcement Learning | Learn by trial, error, and reward | Game-playing agents |

---

## 2. Concept Learning

**Concept learning** = inferring a boolean-valued function (a "concept") from training examples labeled positive or negative.

- **Hypothesis (h):** A candidate rule/function that maps instances to {Yes, No}. Represented usually as a conjunction of constraints on attributes.
  - Example: `<Sunny, Warm, ?, Strong, ?, ?>` means "Sky=Sunny AND Temp=Warm AND (any Humidity) AND Wind=Strong AND (any two remaining attributes)".
  - `?` = any value acceptable, `∅` = no value acceptable (reject everything).

**Analogy:** Think of a hypothesis as a **stencil**. You slide it over the data; only instances that "fit through the holes" are classified positive. `?` is a hole open to everything; a specific value is a narrow slit; `∅` is a solid patch (nothing passes).

- **Version Space (VS):** The set of *all* hypotheses consistent with the training data seen so far.
  - As more examples arrive, the version space shrinks (like narrowing down suspects with more clues).
  - Represented efficiently using **Candidate Elimination Algorithm** via two boundary sets:
    - **S (Specific boundary):** most specific hypotheses consistent with data.
    - **G (General boundary):** most general hypotheses consistent with data.
  - Every hypothesis strictly between S and G (more general than S, more specific than G) is a member of the version space.

**How to solve Candidate-Elimination by hand:**
1. Initialize S = most specific hypothesis (`∅,∅,...`), G = most general hypothesis (`?,?,...`).
2. For each training example:
   - If **positive**: remove from G any hypothesis inconsistent with it; generalize S minimally so it covers the example (but stays consistent with all previous negatives).
   - If **negative**: remove from S any hypothesis inconsistent with it; specialize G minimally so it excludes the example (but stays consistent with all previous positives).
3. Final S and G define the version space boundary.

**Inductive Bias:** The set of assumptions a learner uses to predict outputs for unseen inputs, beyond the training data. Without bias, a learner cannot generalize (it could only memorize).
- Example: Decision trees have inductive bias toward *shorter trees* (Occam's razor: prefer simpler theories).
- KNN's inductive bias: similar instances (near in feature space) have similar labels.

**Analogy:** Inductive bias is like a person's "gut assumptions" when guessing the rest of a story after reading only the first page — different people (algorithms) fill gaps differently.

---

## 3. Performance Metrics

Given a confusion matrix for binary classification:

| | Predicted Positive | Predicted Negative |
|---|---|---|
| **Actual Positive** | TP | FN |
| **Actual Negative** | FP | TN |

- **Accuracy** = (TP + TN) / (TP + TN + FP + FN)
  → "Out of everything, how much did I get right?"
  - **Weakness:** misleading on imbalanced data. If 95% of emails are not spam, predicting "never spam" gives 95% accuracy but is useless.

- **Precision** = TP / (TP + FP)
  → "Of everything I *called* positive, how many actually were?"
  - Analogy: A fisherman who casts a net — precision asks, "of the fish caught, how many are the target species?" High precision = few false alarms.

- **Recall (Sensitivity)** = TP / (TP + FN)
  → "Of all the actual positives, how many did I catch?"
  - Analogy: Of all target fish in the lake, how many did the net actually catch? High recall = few misses.

- **Specificity** = TN / (TN + FP)
  → "Of all actual negatives, how many did I correctly say are negative?"

- **Trade-off:** Precision and Recall usually trade off. Raising the classification threshold → fewer positive predictions → higher precision, lower recall.

**Worked example:**
TP=40, FP=10, FN=5, TN=45
- Accuracy = (40+45)/100 = 0.85
- Precision = 40/(40+10) = 0.80
- Recall = 40/(40+5) = 0.888
- Specificity = 45/(45+10) = 0.818

- **ROC Curve (Receiver Operating Characteristic):** Plots **TPR (Recall)** on y-axis vs **FPR (1-Specificity)** on x-axis at varying thresholds.
- **AUC (Area Under Curve):** Single number summarizing ROC — probability that the classifier ranks a random positive example higher than a random negative example.
  - AUC = 1 → perfect classifier. AUC = 0.5 → random guessing (diagonal line).

**Analogy for ROC/AUC:** Imagine a bouncer at a club deciding who is "over 21" using a fake ID detector. As the bouncer becomes stricter (raises the threshold), fewer minors get in (fewer false positives) but also more real adults get rejected (fewer true positives). The ROC curve traces this trade-off across all strictness levels; AUC tells you how good the detector is *regardless of chosen strictness*.

---

## 4. Bias-Variance Decomposition & Trade-off

Expected prediction error can be decomposed as:

**Error = Bias² + Variance + Irreducible Noise**

- **Bias:** Error from wrong assumptions in the model (underfitting). High bias = model is too simple to capture the pattern.
- **Variance:** Error from sensitivity to small fluctuations in training data (overfitting). High variance = model changes drastically with different training sets.

**Analogy — Archery targets:**
- **High bias, low variance:** All arrows land in a tight cluster, but far from the bullseye (consistently wrong).
- **Low bias, high variance:** Arrows are scattered all over, but their average is near the bullseye (correct on average, inconsistent per shot).
- **Low bias, low variance:** Tight cluster near the bullseye — ideal.

**Trade-off:** As model complexity increases (more flexible), bias decreases but variance increases. The goal is to find the "sweet spot" of complexity minimizing total error.

**How to reason through a bias-variance question:**
1. Identify if the model is simple (linear) or complex (deep tree, high-degree polynomial).
2. Simple → likely high bias, underfitting → training AND test error both high.
3. Complex → likely high variance, overfitting → training error low, test error high.

---

## 5. Cross-Validation Strategies

Purpose: estimate how a model will generalize to unseen data, using only the training set.

- **Hold-out validation:** Split data into train/test once (e.g., 70/30). Simple but result depends heavily on the split.
- **k-Fold Cross Validation:** Split data into k equal folds. Train on (k-1) folds, test on remaining fold; repeat k times, average the results.
  - **Analogy:** Like a class of k students taking turns being "the judge" while the rest study together — every student judges once, giving a fair overall grade.
- **Leave-One-Out CV (LOOCV):** Special case where k = n (number of samples). Very thorough but computationally expensive.
- **Stratified k-Fold:** Ensures each fold preserves the overall class proportion (important for imbalanced datasets).

**How to solve a CV numeric problem:**
- If asked "5-fold CV on 100 samples," each fold has 20 samples; the model is trained 5 times, each time on 80 samples and tested on the remaining 20; final performance = average across 5 runs.

---

## 6. Decision Trees

**Idea:** Split the dataset repeatedly based on the attribute that best separates classes, forming a tree of decisions ending in leaf nodes (class predictions).

**Analogy:** Playing "20 Questions" — at each step you ask the most informative yes/no question to narrow down the possibilities fastest.

### ID3 Algorithm (basic steps)
1. If all examples belong to one class → return a leaf with that class.
2. Otherwise, choose the attribute **A** that best classifies examples (highest Information Gain).
3. Create a decision node for A; split data into subsets by A's values.
4. Recurse on each subset with remaining attributes.

**Entropy** (measure of impurity):
`Entropy(S) = -Σ pᵢ log₂(pᵢ)`
- 0 when all examples belong to one class (pure), 1 (max) when classes are equally split (for binary case).

**Information Gain:**
`IG(S, A) = Entropy(S) - Σ (|Sᵥ|/|S|) × Entropy(Sᵥ)`
Choose the attribute with the **highest IG** to split on.

**Worked example:**
Suppose S has 14 examples: 9 "Yes", 5 "No".
`Entropy(S) = -(9/14)log2(9/14) - (5/14)log2(5/14) ≈ 0.940`

If splitting on attribute "Outlook" gives subsets with entropies 0, 0.971, 0.971 (weighted appropriately), compute weighted entropy and subtract from 0.940 → gives Information Gain for Outlook. Compare IG across all attributes → pick the max.

### Decision Boundaries of Decision Trees
- Decision trees create **axis-aligned rectangular** decision boundaries (splits are always parallel to one feature axis, e.g., x ≤ 5 or y > 3), unlike KNN's curved boundaries.
- **Analogy:** Imagine slicing a cake only with straight horizontal or vertical cuts (never diagonal) — that's how decision trees partition feature space.

### Hypothesis Search & Inductive Bias in Decision Trees
- ID3 searches a **complete hypothesis space** (can represent any boolean function) using a **greedy, hill-climbing** search (no backtracking).
- **Inductive Bias:** Prefers shorter trees, and trees that place attributes with higher information gain closer to the root ("Occam's Razor" preference for simplicity).

### Overfitting in Decision Trees
- **Symptom:** Tree perfectly fits training data (very deep, many leaves) but performs poorly on unseen data.
- **Causes:** Noise in data, too few examples per leaf, overly complex tree.

**Solutions:**
1. **Pre-pruning:** Stop growing tree early (e.g., limit max depth, minimum samples per leaf, minimum information gain threshold).
2. **Post-pruning:** Grow full tree, then prune back branches that don't improve validation accuracy (e.g., Reduced Error Pruning, Cost-Complexity Pruning).
3. Use ensemble methods (Random Forest) to average out overfitting — mentioned as broader context.

**Analogy:** Overfitting is like a student who memorizes every past exam question verbatim instead of understanding the concept — does great on old exams (training data), fails on the new exam (test data). Pruning is like the teacher saying "you don't need to memorize every case, just learn the key concept."

### Handling Continuous Attributes
- Sort the continuous attribute values; consider candidate thresholds at midpoints between consecutive values with different class labels.
- Convert into a boolean test: `Attribute ≤ threshold` vs `Attribute > threshold`.
- Pick the threshold that maximizes Information Gain, just like a categorical split.

**Worked example:** Temperature values 40, 48, 60, 72, 80 with labels No, No, Yes, Yes, No.
Candidate thresholds are midpoints between label changes: (48+60)/2=54, (80+72)/2=76.
Compute Information Gain for splits at Temp ≤ 54 and Temp ≤ 76, choose the best.

---

## 7. Logistic Regression

**Idea:** A linear model for **classification** (not regression, despite the name) that outputs a probability using the **sigmoid function**:

`P(y=1|x) = 1 / (1 + e^-(w·x + b))`

- Output is squashed between 0 and 1, interpreted as probability of class 1.
- Decision boundary: predict class 1 if P ≥ 0.5, else class 0 — this corresponds to `w·x + b = 0`, a **linear (straight-line/hyperplane) boundary**.

**Analogy:** Think of logistic regression as a dimmer switch instead of an on/off switch — it doesn't just say "yes/no" abruptly, but computes *how confident* it is, then a threshold (usually 0.5) converts confidence into a decision.

**How to solve:**
- Given weights w and bias b, compute z = w·x + b, then apply sigmoid to get probability, then threshold at 0.5.
- Training uses **Maximum Likelihood Estimation** / **Cross-Entropy Loss**, minimized via Gradient Descent (weights updated in direction that reduces loss).

---

## 8. Instance-Based Learning: k-Nearest Neighbors (KNN)

**Idea:** "Lazy learning" — no explicit training phase. To classify a new point, look at its **k closest neighbors** (by some distance metric, usually Euclidean) in the training data and vote (classification) or average (regression).

**Analogy:** "You are the average of your 5 closest friends." To guess a new person's taste in movies, ask their k nearest friends and go with the majority opinion (classification) or average rating (regression).

**Algorithm:**
1. Compute distance from the query point to every training point.
2. Select the k closest points.
3. **Classification:** majority vote among k neighbors' labels.
4. **Regression:** average (or weighted average) of the k neighbors' target values.

**Distance metric (Euclidean):**
`d(x, x') = √Σ(xᵢ - x'ᵢ)²`

**Worked example (classification):**
Query point Q. Distances to 5 training points: 1.2(Class A), 1.5(Class A), 2.0(Class B), 2.3(Class B), 2.5(Class B).
For k=3: nearest 3 are 1.2(A), 1.5(A), 2.0(B) → majority = Class A.

**Choosing k:**
- Small k → sensitive to noise (high variance, low bias, overfitting risk).
- Large k → smoother boundary but may blur class distinctions (high bias, underfitting risk).
- Typically choose k via cross-validation; often an odd number for binary classification to avoid ties.

### Decision Boundary for KNN
- KNN boundaries are **non-linear, irregular, and adapt locally** to the data density (not axis-aligned like decision trees, not a single hyperplane like logistic regression).
- As k increases, the boundary becomes **smoother**; as k decreases (k=1), boundary becomes very jagged, tightly wrapping around individual points (Voronoi-like regions).

**Analogy:** Decision tree boundary = cutting a cake only in straight rectangular slices. Logistic regression boundary = one clean straight knife cut across the whole cake. KNN boundary = tracing the boundary by hand around clusters of frosting colors — wiggly and localized.

---

## Quick Revision Table

| Topic | Key takeaway |
|---|---|
| Version Space | Set of hypotheses consistent with data; shrinks with more examples |
| Inductive Bias | Assumptions enabling generalization beyond seen data |
| Precision vs Recall | Precision = trustworthy positives; Recall = catching all positives |
| ROC/AUC | Performance across all thresholds; AUC=1 perfect, 0.5 random |
| Bias-Variance | Simple model→high bias; complex model→high variance |
| Decision Tree | Greedy splits by Information Gain; axis-aligned boundary; prone to overfitting; pruning fixes it |
| Logistic Regression | Sigmoid + linear boundary; outputs probability |
| KNN | Lazy learner; local, non-linear boundary; k controls smoothness |



