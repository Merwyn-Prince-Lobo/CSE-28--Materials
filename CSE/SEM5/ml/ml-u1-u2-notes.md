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


# Unit 2 — Artificial Neural Networks (ANN) & Support Vector Machines (SVM)

---

## 1. Artificial Neural Networks — Introduction

**Idea:** ANNs are computing systems loosely inspired by biological neurons, made of layers of interconnected "nodes" (neurons) that transform inputs into outputs through weighted sums and non-linear activations.

**Analogy:** Think of an ANN as an assembly line in a factory. Raw materials (input features) pass through several workstations (layers); at each station, a worker (neuron) combines the materials in a specific weighted way and passes a transformed product to the next station, until a final product (prediction) comes out.

---

## 2. The Perceptron

**Structure:** The simplest neural unit — takes inputs x₁...xₙ, multiplies each by a weight, sums them with a bias, and passes the result through an activation function (originally a step function).

`output = f(Σ wᵢxᵢ + b)`

**Analogy:** A perceptron is like a single voter who weighs different pieces of evidence (inputs) by how much they trust each one (weights), sums up their "gut feeling" (weighted sum + bias), and decides yes/no (activation).

**Limitation:** A single perceptron can only learn **linearly separable** functions (e.g., it cannot learn XOR) because it only draws a single straight-line/hyperplane boundary.

**How to solve a perceptron problem:**
1. Given weights, bias, and inputs, compute `z = Σ wᵢxᵢ + b`.
2. Apply activation (e.g., step function: output 1 if z ≥ 0, else 0).
3. Perceptron Learning Rule (for updating weights during training):
   `wᵢ ← wᵢ + η(target - output) × xᵢ`
   where η is the learning rate. Repeat over all examples until convergence (if linearly separable).

---

## 3. Multi-Layer Networks and Backpropagation

**Why multi-layer:** Stacking layers with non-linear activations allows the network to represent **non-linear decision boundaries** (solves problems like XOR that a single perceptron cannot).

**Structure:** Input layer → one or more Hidden layers → Output layer. Each connection has a weight; each neuron (except input) has a bias and an activation function.

**Forward Pass:** Compute outputs layer by layer, from input to output, using weighted sums + activation at each neuron.

**Backpropagation (Backprop):** The algorithm to train multi-layer networks by computing how much each weight contributed to the final error, and updating weights to reduce that error.

**Analogy:** Imagine a factory production line that made a defective final product. Backpropagation is like management going backward station-by-station from the final assembly point to the raw-material station, figuring out *how much each station's mistake contributed to the final defect*, and then adjusting each station's procedure (weights) accordingly to reduce future defects.

**Steps of Backpropagation:**
1. **Forward pass:** compute predicted output ŷ for given input.
2. **Compute loss:** e.g., `L = (1/2)(y - ŷ)²` (Mean Squared Error) or Cross-Entropy for classification.
3. **Backward pass:** apply the **chain rule** of calculus to compute the gradient of the loss with respect to each weight, propagating error gradients from the output layer back toward the input layer.
4. **Update weights:** `w ← w - η × (∂L/∂w)` for every weight (Gradient Descent step).
5. Repeat over many training examples/epochs until loss converges.

**How to solve a small backprop numeric problem:**
- Compute forward pass values (pre-activation `z` and activation `a` at each neuron).
- Compute output error = (target - output) × derivative of activation at output.
- Propagate this error backward: hidden layer error = (weight × output error) × derivative of activation at hidden neuron.
- Update each weight using: `Δw = η × error_of_receiving_neuron × input_to_that_weight`.

---

## 4. Activation Functions

Activation functions introduce **non-linearity**, without which stacking layers would collapse into a single linear function (no matter how many layers).

| Function | Formula | Range | Notes / Analogy |
|---|---|---|---|
| **Step** | 1 if z≥0 else 0 | {0,1} | Like a light switch — fully on or off, no in-between. Not differentiable, rarely used in training. |
| **Sigmoid** | 1/(1+e⁻ᶻ) | (0,1) | Smooth "S-curve"; good for probabilities. Analogy: a dimmer switch. Problem: **vanishing gradients** for large \|z\| — like a dimmer that barely responds once turned nearly full-on or full-off. |
| **Tanh** | (eᶻ-e⁻ᶻ)/(eᶻ+e⁻ᶻ) | (-1,1) | Like Sigmoid but zero-centered, often trains faster. Still suffers vanishing gradient at extremes. |
| **ReLU** | max(0, z) | [0, ∞) | "Off if negative, straight pass-through if positive." Analogy: a one-way valve — blocks negative flow completely, lets positive flow through unchanged. Fast to compute, avoids vanishing gradient for positive z, but can suffer "dying ReLU" (neurons stuck outputting 0). |

**How to solve:** Given z, plug into the formula. E.g., ReLU(-3)=0, ReLU(5)=5; Sigmoid(0)=0.5; Tanh(0)=0.

---

## 5. Optimizers

Optimizers decide **how weights are updated** during training to minimize loss efficiently.

- **Gradient Descent (GD / Batch GD):** Uses the *entire* training dataset to compute the gradient before each update.
  - Analogy: Deciding your next hiking step only after surveying the *entire* mountain's slope — accurate but very slow for large mountains (datasets).

- **Stochastic Gradient Descent (SGD):** Updates weights using **one training example at a time**.
  - Analogy: Taking a step based on just the ground right under your feet — fast, but a bit jittery/noisy path downhill.
  - (Mini-batch SGD, a middle ground using small batches, is the practical standard.)

- **Momentum-based GD:** Adds a fraction of the previous update to the current one, so updates build "velocity" in a consistent direction and dampen oscillations.
  - Analogy: A ball rolling downhill gains momentum — it doesn't stop or change direction abruptly at every bump, smoothing the path to the minimum.
  - Formula: `v ← βv + (1-β)∇L ; w ← w - ηv`

- **Adagrad:** Adapts the learning rate **per parameter**, giving smaller updates to frequently-updated (large gradient history) parameters and larger updates to infrequent ones.
  - Analogy: A student who has already practiced one topic a lot slows down on it, and spends more relative effort on the topic they've barely touched.
  - Weakness: learning rate keeps shrinking and can become too small over time (accumulated squared gradients only grow).

- **Adam (Adaptive Moment Estimation):** Combines **Momentum** (first moment - mean of gradients) and **Adagrad/RMSProp-style** (second moment - variance of gradients) ideas — adapts learning rate per parameter *and* uses momentum.
  - Most widely used in practice due to fast, stable convergence.
  - Analogy: Adam is like a smart hiker who both remembers their recent direction of travel (momentum) AND adjusts step size based on how rugged that specific patch of terrain has historically been (adaptive learning rate).

**How to answer an optimizer comparison question:** Identify what mechanism is being described (uses whole dataset? one sample? adapts learning rate? uses velocity?) and match to GD/SGD/Momentum/Adagrad/Adam accordingly.

---

## 6. Support Vector Machines (SVM)

**Idea:** Find the hyperplane that separates two classes with the **maximum margin** (largest possible gap) between the closest points of each class (the "support vectors").

**Analogy:** Imagine drawing a road (the decision boundary) between two neighborhoods (classes) as **wide as possible** without touching any house on either side. The houses closest to the road's edges are the "support vectors" — they are the only ones that determine where and how wide the road can be; houses further back don't matter.

### Margin and Maximization
- The margin is the distance between the hyperplane and the nearest data point of any class.
- Maximizing this margin leads to better generalization (a "safer," wider road is less likely to misclassify new nearby points).
- Margin width = `2/‖w‖`, so maximizing margin ⇔ minimizing `‖w‖` (or `(1/2)‖w‖²` for convenience in optimization).

### The Primal Problem (Hard Margin)
Minimize: `(1/2)‖w‖²`
Subject to: `yᵢ(w·xᵢ + b) ≥ 1` for all training points i.

- This says every point must be correctly classified **and** lie outside (or on) the margin boundary.

**Analogy:** The primal problem is like an engineer trying to make the road (margin) as wide as possible (minimize `‖w‖²`, i.e., maximize width) while still ensuring every house stays clearly on its correct side (the constraint).

### The Lagrangian Dual
Since the primal has inequality constraints, we use **Lagrange multipliers (αᵢ ≥ 0)** to convert it into the dual problem — often easier to solve, especially with kernels.

`L(w,b,α) = (1/2)‖w‖² - Σαᵢ[yᵢ(w·xᵢ+b) - 1]`

Setting derivatives to zero gives:
- `w = Σαᵢyᵢxᵢ`
- `Σαᵢyᵢ = 0`

Substituting back gives the **Dual Problem**: maximize
`Σαᵢ - (1/2)ΣΣαᵢαⱼyᵢyⱼ(xᵢ·xⱼ)`
subject to `αᵢ ≥ 0` and `Σαᵢyᵢ = 0`.

**Key insight:** The dual only depends on **dot products between data points** (xᵢ·xⱼ), which is exactly what allows the **Kernel Trick** (replace dot product with a kernel function to handle non-linear boundaries without explicitly transforming data).

### Solution to the Lagrangian Dual
- Solve the dual (a quadratic programming problem) for α values.
- Points with **αᵢ > 0** are the **support vectors** — only they influence the final decision boundary (all other points have αᵢ = 0 and can be discarded).
- Recover `w = Σαᵢyᵢxᵢ`, and `b` from any support vector using `yᵢ(w·xᵢ+b) = 1`.
- Classify a new point x using: `f(x) = sign(w·x + b) = sign(Σαᵢyᵢ(xᵢ·x) + b)`.

**Analogy:** Among all houses (data points), the Lagrangian dual "highlights" only the few houses right at the edge of the neighborhood (support vectors, α>0) as the ones that actually define where the road goes — all houses further inside the neighborhood are irrelevant to defining the road's position.

### Hard Margin vs Soft Margin
- **Hard Margin:** Assumes data is perfectly linearly separable — no points allowed inside the margin or misclassified. Very sensitive to outliers/noise.
- **Soft Margin:** Introduces slack variables `ξᵢ ≥ 0` to allow some misclassification/margin violations, trading off margin width against classification errors:

  Minimize: `(1/2)‖w‖² + C·Σξᵢ`
  Subject to: `yᵢ(w·xᵢ+b) ≥ 1 - ξᵢ`, `ξᵢ ≥ 0`

  - **C** is a hyperparameter controlling the trade-off:
    - Large C → less tolerance for margin violations (behaves closer to hard margin, risk of overfitting).
    - Small C → more tolerance for violations (wider margin, more regularization, risk of underfitting).

**Analogy for Soft Margin:** Instead of insisting *no* house may ever touch the road (hard margin — impossible if the neighborhood layout is messy), the soft margin allows a few houses to poke slightly into the road's edge, paying a "penalty" (ξᵢ, scaled by C) for every centimeter of intrusion — as long as the *total* penalty stays low and the road stays reasonably wide.

**How to solve an SVM classification numeric problem:**
1. Given support vectors and their α, y values, compute `w = Σαᵢyᵢxᵢ`.
2. Pick a support vector on the margin, solve `b = yᵢ - w·xᵢ` (from `yᵢ(w·xᵢ+b)=1`).
3. To classify a new point x, compute `sign(w·x + b)`: positive → Class +1, negative → Class -1.

---

## 7. Kernel Functions

**Why kernels:** Many real datasets are **not linearly separable** in their original feature space. Kernels implicitly map data into a higher-dimensional space where it *becomes* linearly separable — **without ever explicitly computing that transformation** (this is the "kernel trick").

**Analogy:** Imagine red and blue marbles scattered on a flat table in a circular pattern (blue in the center, red around it) — no straight line can separate them in 2D. But if you **lift** the marbles into 3D (say, height = distance from center), suddenly a flat plane easily separates them. The kernel trick computes the *effect* of this lifting (the dot product in the higher dimension) directly from the original 2D coordinates, without actually doing the lift.

### Linear Kernel
`K(xᵢ, xⱼ) = xᵢ · xⱼ`
- Just the standard dot product — used when data is already linearly (or near-linearly) separable. No actual "kernel trick" transformation happening; it's the baseline case.

### Polynomial Kernel
`K(xᵢ, xⱼ) = (xᵢ · xⱼ + c)^d`
- Allows curved (polynomial-degree-d) decision boundaries by implicitly considering interaction terms and higher powers of features.

**Derivation for the Linear Kernel case (as specified in syllabus):**
Since `K(xᵢ,xⱼ) = xᵢ·xⱼ` directly, no explicit mapping φ(x) is required — φ(x) = x itself (identity mapping). The dual objective:
`Σαᵢ - (1/2)ΣΣαᵢαⱼyᵢyⱼK(xᵢ,xⱼ)`
becomes exactly the standard linear-SVM dual:
`Σαᵢ - (1/2)ΣΣαᵢαⱼyᵢyⱼ(xᵢ·xⱼ)`
This shows the linear kernel is simply the "no trick" special case where the kernel trick reduces back to ordinary dot products — a useful sanity check before moving to non-linear kernels like polynomial or RBF.

**How to solve a kernel numeric problem:**
- Given two vectors xᵢ, xⱼ, and kernel type/parameters, just plug into the formula.
  - Example: xᵢ=(1,2), xⱼ=(3,4), Linear kernel → K = 1×3+2×4 = 11.
  - Polynomial kernel with c=1, d=2 → K = (11+1)² = 144.

---

## Quick Revision Table

| Topic | Key takeaway |
|---|---|
| Perceptron | Single linear unit; only solves linearly separable problems |
| Backpropagation | Chain rule to propagate error backward, update weights via gradient descent |
| Activation functions | Add non-linearity; ReLU most common today, avoids vanishing gradient for z>0 |
| GD vs SGD | Full dataset vs single-sample updates; trade-off between stability and speed |
| Momentum | Smooths updates using past gradients ("velocity") |
| Adagrad | Per-parameter adaptive learning rate; shrinks over time |
| Adam | Momentum + adaptive learning rate combined; most popular optimizer |
| SVM Margin | Maximize distance between hyperplane and nearest points (support vectors) |
| Primal vs Dual | Dual depends only on dot products → enables kernel trick |
| Hard vs Soft Margin | Soft margin allows violations via slack ξ and penalty C for real-world noisy data |
| Kernels | Implicitly map to higher dimension to achieve linear separability; Linear = plain dot product, Polynomial = curved boundaries |