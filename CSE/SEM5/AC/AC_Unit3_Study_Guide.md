# 🔐 Applied Cryptography — Unit 3 Study Guide
**AES → Public-Key Crypto → RSA → DH → ECC → Signatures → Certificates → Hashes**
*(UE24CS342AA4, Dr. Shruti Jadon, all 432 slides condensed. Read this = skip the PPT.)*

> **How to use:** Section 0 is the panic-sheet (read it last night). Sections 1–12 are the full topics with **"How to solve"** boxes and worked examples. Section 13 has practice problems with answers. Everything numeric in here was re-checked by actually computing it.

> ⚠️ **Slide errors I found (so you don't copy them in the exam)** — details are in the relevant sections:
> 1. Slide 85 (prime gen, 223): says `2^111 mod 223 = -1`. It's actually **1**. (223 still passes MR, since 1 is also a "pass" value.)
> 2. Slide 213 (RSA-CRT ex #3): says `C = 20^7 mod 187 = 98`. Correct is **C = 147**. (The Mp = 9, Mq = 3 on that slide are correct for 147.)
> 3. Slide 124 (3rd bullet group): "p=3, q=13, e=3, d=7" is invalid — φ = 24 and gcd(3,24) = 3, so e = 3 has no inverse.
> 4. MITM slide 30: Eve–Bob key is `8^8 mod 11 = 5`, slide prints `8^6`. Final answer (5) is right, the exponent is a typo.
> 5. Slide 340 (DSA vs RSA) makes sketchy claims ("DSA more secure than RSA"). Write what the slide says if asked "as per slides", but know it's not really true in general.

---

# 0. 🚨 PANIC SHEET (formulas & facts)

| Topic | Must-know |
|---|---|
| Symmetric keys for n users | **n(n−1)/2** → O(n²). Public-key: **n key pairs** → O(n) |
| 10 / 100 / 1000 / 10,000 users | 45 / 4,950 / 499,500 / 49,995,000 keys |
| Hybrid idea | "Protect the **key**, not the **file**": PKC/DH sets up session key Ks, AES encrypts bulk data |
| Trapdoor one-way fn | f(x) easy; f⁻¹(y) infeasible **unless** you know trapdoor |
| 3 hard problems | Integer **factoring** (RSA), **DLP** (DH/ElGamal/DSA), **ECDLP** (ECC) |
| Euler φ | φ(p)=p−1 ; φ(pq)=(p−1)(q−1) ; φ(p^k)=p^k−p^(k−1) |
| Euler's thm | gcd(a,n)=1 ⇒ a^φ(n) ≡ 1 (mod n) |
| Fermat | p prime, p∤a ⇒ a^(p−1) ≡ 1 (mod p) |
| Miller–Rabin | n−1 = 2^s·d (d odd); x=a^d; pass if x=1 or x=n−1 (or any squaring hits n−1); else **composite**. Error ≤ (1/4)^k |
| RSA keygen | p,q primes → n=pq → φ=(p−1)(q−1) → pick e, gcd(e,φ)=1 → d = e⁻¹ mod φ |
| RSA | Public (e,n), Private (d,n). **C = M^e mod n**, **M = C^d mod n**, need M < n |
| RSA signature | **s = h^d mod n**, verify **s^e mod n == h** |
| CRT formula | X = (Σ aᵢ·Mᵢ·Mᵢ⁻¹) mod M, M=∏mᵢ, Mᵢ=M/mᵢ |
| RSA-CRT | dP=d mod (p−1), dQ=d mod (q−1); Mp=C^dP mod p; Mq=C^dQ mod q; recombine → M. **≈4× faster** |
| Square-and-multiply | O(log d) mults instead of O(d) |
| OAEP | P₁=M⊕G(r); P₂=H(P₁)⊕r; P=P₁‖P₂; C=P^e mod n. Fixes **determinism** |
| CRT = performance, OAEP = security | (slide's "take-home" line) |
| DH | A=g^a mod p, B=g^b mod p, K=B^a=A^b=g^(ab) mod p. **No authentication ⇒ MITM** |
| DLP attacks | Generic: BSGS, Pollard-ρ ~ √|G| · Index-calculus (subexp, **not** for ECC) · Pohlig–Hellman (small factors of p−1) |
| ECC | y² = x³+ax+b; Q=kP easy, find k hard (ECDLP); **double-and-add** |
| Key sizes (128-bit sec.) | **RSA 3072 ≈ ECC 256** (≈ AES-128) |
| Signature gives | Authentication + Integrity + Non-repudiation (**NOT** confidentiality by itself) |
| Sign = | hash message → encrypt hash with **private** key |
| Certificate | CA-signed binding of identity ↔ public key. Fixes MITM / key authenticity |
| Hash props | preimage-res, 2nd-preimage-res, collision-res, avalanche, deterministic, fast, fixed output |
| Birthday | collision cost ≈ 2^(m/2); 128-bit "inadequate", 160-bit "suspect" |
| Merkle–Damgård | pad + length → blocks → Hᵢ = f(Hᵢ₋₁, Mᵢ), H₀ = IV, digest = H_t |
| MD5 | 128-bit, 512-bit blocks, 4 rounds × 16 steps = 64, ABCD init, **broken** (Wang 2004) |
| SHA-1 | 160-bit, 512-bit blocks, 80 rounds (4×20), ABCDE, **deprecated** (browsers dropped 2017) |
| Padding rule | append `1`, then `0`s till length ≡ **448 mod 512**, then 64-bit length |
| MD weakness | **length-extension** → use HMAC |

---

# 1. Why Public-Key Crypto? (the AES lecture)

## 1.1 The two questions crypto must answer
1. How do we **encrypt a large amount of data efficiently?** → symmetric (AES) ✅
2. How do two parties **establish a shared secret** over an insecure network if they've never met? → ❌ not solved by AES → **public-key crypto**

**Key-establishment problem:** Alice & Bob need the same key K_AB. Sending K_AB over an insecure network defeats the purpose. Symmetric encryption has "encryption solved, key-establishment still open."

## 1.2 Symmetric: pros and cons
- ✅ Very secure, relatively fast
- ❌ Key **exchange** problem
- ❌ **Number of keys** needed
- ❌ More damage if a key is compromised

## 1.3 Key scalability (very examinable)
Pairwise keys = **C(n,2) = n(n−1)/2** → grows **quadratically, O(n²)**.

| Users | Keys |
|---|---|
| 4 | 6 (A-B, A-C, A-D, B-C, B-D, C-D) |
| 10 | 45 |
| 100 | 4,950 |
| 1,000 | 499,500 |
| 10,000 | 49,995,000 |
| 2000 students | ≈ 2 million |

Adding a 5th user to 4 users → **4 new keys**.
Each user must store **n−1** keys. Problems: adding users, compromised keys, key lifetime/refresh.

> The problem is **not** that symmetric encryption is weak. Managing pairwise secrets just doesn't scale.

**Partial fix: Trusted Third Party (TTP / KDC).** Each user shares one key with the TTP; TTP helps create a **session key**. Drawbacks: must trust TTP, still need to distribute one key per user to the TTP.

**PKC fix:** each user has **one key pair** (public + private). n users → n key pairs (2n individual keys) → **linear O(n)**. Anyone encrypts with your public key.

### ⚠️ The "only two keys" trap
- ✔ Say: "Each new user has one public/private key pair" / "Total grows **linearly**".
- ✘ Don't say: "The number of keys doesn't increase." (it does)
- Symmetric ≈ n(n−1)/2 vs Public-key ≈ n. Quadratic → linear.

## 1.3b Why not throw away AES?
They solve **different problems** → *complementary*, not replacements.
- AES: bulk data. PKC: key establishment, authentication, digital signatures.
- Public-key is ≈ **1000× slower** than symmetric (slide figure).

## 1.4 Why is RSA slow while the formula looks tiny?
`C = M^e mod n` *looks* short, but:
- needs modular exponentiation on **huge integers** (RSA-2048 modulus = 2048 bits vs AES block = 128 bits)
- each big-int multiply/reduce is far costlier than AES's small ops
- **Formula length ≠ computational cost.**

**AES is fast** because: fixed 128-bit blocks; simple regular ops (substitute, shift rows, mix columns, XOR round key); hardware support (AES-NI). AES-128/192/256 differ in key length; block size always 128 bits.

**Think:** RSA = few expensive big-number ops. AES = many cheap small ops.

**1 MB file example:** 1,000,000 / 16 ≈ **62,500 AES blocks**. RSA's message size is limited by modulus & padding and each op is big-int arithmetic → using RSA for bulk is the wrong design.

## 1.5 Hybrid cryptography ("Protect the key, not the file")
1. Generate random session key **Ks**
2. Use public-key mechanism (RSA or DH) to establish/protect Ks
3. Use Ks with **AES** for the actual data

RSA-based simplified version: Alice picks Ks → encrypts Ks with **Bob's public key** → Bob decrypts with **his private key** → both use Ks with AES.

**Real-world refinement:** modern protocols mostly use **Diffie–Hellman variants (incl. elliptic-curve)** to *agree* on the secret rather than "RSA transfers the AES key". So don't claim "RSA transfers the AES key" universally.

**TLS/HTTPS:** public-key for authentication and/or key establishment, then symmetric session keys for data. Exact algorithms depend on protocol version & cipher suite.

**Classic experiment:** *"Send a 10 GB video securely — RSA-encrypt it?"* → **No.** *"Then why RSA?"* → to establish/protect the secret & authenticate. *"Who handles the 10 GB?"* → AES.

## 1.6 "Is AES secure?" wording
- ✘ "AES has never been attacked." (too absolute)
- ✔ "**No practical attack is known against properly implemented full-round AES.**" There are attacks on reduced-round variants and theoretical attacks with impractical requirements. Real systems break via **implementation** issues.

**Algorithm vs Implementation:** Side channels (power/EM, timing, cache), weak RNG, poor key storage, wrong IV/nonce use, insecure modes.
**Security = algorithm + implementation + key management + protocol.**

**"If AES has a key problem, is AES defective?"** No — AES is an **encryption primitive**, not a key-management system. Analogy: a lock can be excellent even though it doesn't deliver the key.

## 1.7 Misconceptions table (likely 2–3 mark Qs)
| ❌ Wrong | ✅ Correct |
|---|---|
| PKC replaces symmetric | They're complementary |
| AES insecure because of key distribution | Key distribution is a system-level problem; AES is a primitive |
| RSA faster because formula is shorter | RSA does expensive large-integer modular arithmetic |
| AES has never been attacked | Distinguish full-round AES, reduced-round cryptanalysis, implementation attacks |
| New PK user adds only two keys to whole system | User adds one key pair; total grows linearly |

---

# 2. Public-Key Cryptography — Concepts

## 2.1 Idea
- User generates a **related pair**: public + private. Private key can't be efficiently computed from public.
- **Public key**: used for encryption, signature verification; distributed freely.
- **Private key**: used for decryption, signing; kept secret.
- Algorithm = **KeyGen + Encrypt + Decrypt**.

## 2.2 History (frequent 1-markers)
- **1970** James Ellis (GCHQ, UK) — classified; made public **1997**.
- **1976** Diffie & Hellman (& Merkle) — "**New Directions in Cryptography**": public-key encryption idea, key distribution, **DH key agreement**, **digital signature** concept.
- **1977** Rivest, Shamir, Adleman (MIT) — **RSA**, first public-key cipher still in use.

## 2.3 What PKC gives (mechanisms)
1. **Key distribution/agreement** without pre-shared secret (DH, RSA)
2. **Digital signatures / non-repudiation** (RSA, DSA, ECDSA)
3. **Identification** via challenge-response with signatures
4. **Encryption** (RSA, ElGamal)
- Disadvantage: **computationally intensive (~1000× slower)**.

PKC solved two problems: **key distribution** and **digital signatures**.

## 2.4 Three families in wide use
Based on: (1) **integer factorization** — RSA, (2) **discrete log** — DH, ElGamal, DSA, (3) **elliptic-curve DL** — ECDH, ECDSA.

## 2.5 One-way & trapdoor functions
- **One-way function:** f(x) easy to compute, but for essentially all y it is computationally infeasible to find x with f(x)=y.
- **Trapdoor one-way function:** one-way, **but** with secret trapdoor info T, inversion is easy.
   - Y = f_k(X) easy if k, X known
   - X = f_k⁻¹(Y) easy if k and Y known
   - X = f_k⁻¹(Y) **infeasible** if Y known but **k not known**
- **Strong OWF:** inverted only with negligible probability on random input. **Weak OWF:** slightly hard to invert; easy on some non-negligible fraction.
- Secure encryption & MACs **imply** OWFs exist; from OWF you can build PRG, PRF, PRP → secure encryption/MAC. OWFs = foundation of modern crypto theory.
- **Examples:** hash functions; RSA function (multiply primes easy, factor hard); modular exponentiation (DLP).
- Intuition (Diffie–Hellman paper): padlock (lock easy, unlock hard), shattering a vase.

## 2.6 Security of PK schemes
- Brute force always *theoretically* possible, but keys are huge (>512 bits).
- Security relies on a big **difficulty gap** between easy (en/decrypt) and hard (cryptanalysis).

---

# 3. Math Toolkit (with "how to solve")

## 3.1 Modular arithmetic quickies
- a ≡ b (mod n) means n | (a−b). Reduce early and often.
- Digit-sum trick: divisible by 3 if digit sum is (231 → 2+3+1=6 ✓ so 231 composite).

## 3.2 Extended Euclid → modular inverse (⭐ used in RSA keygen, CRT)
**Goal:** find d with e·d ≡ 1 (mod φ). Exists iff gcd(e, φ)=1.

**Method (back-substitution):**
1. Run Euclid: φ = q₁e + r₁, e = q₂r₁ + r₂, … until remainder 1.
2. Write 1 as a combination of φ and e going backwards.
3. Coefficient of e (mod φ, make positive) is d.

**Example: 7⁻¹ mod 60** (slide's RSA ex 1)
```
60 = 8·7 + 4
 7 = 1·4 + 3
 4 = 1·3 + 1
1 = 4 − 3
  = 4 − (7 − 4)        = 2·4 − 7
  = 2·(60 − 8·7) − 7   = 2·60 − 17·7
→ 7⁻¹ ≡ −17 ≡ 43 (mod 60)    ✔ 7·43 = 301 = 5·60 + 1
```

**Example: 11⁻¹ mod 26**
```
26 = 2·11 + 4 ; 11 = 2·4 + 3 ; 4 = 1·3 + 1
1 = 4 − 3 = 4 − (11 − 2·4) = 3·4 − 11 = 3(26 − 2·11) − 11 = 3·26 − 7·11
→ 11⁻¹ ≡ −7 ≡ 19 (mod 26)
```
**Quick method for small numbers:** try d = (1 + k·φ)/e for k = 0,1,2,… until it divides. e.g. 3d ≡ 1 mod 40 → (1+40k)/3: k=2 → 81/3 = **27** ✓.

## 3.3 Euler's totient & theorems
- φ(n) = count of 1..n coprime to n.
- φ(p)=p−1 · φ(pq)=(p−1)(q−1) · φ(p^k)=p^k−p^(k−1). e.g. φ(35)=4·6=24.
- **Euler:** gcd(a,n)=1 ⇒ a^φ(n) ≡ 1 (mod n).
  - Corollary: a^(φ(n)−1) mod n = a⁻¹ mod n.
  - Corollary: x ≡ y (mod φ(n)) ⇒ aˣ ≡ aʸ (mod n).  ← **why RSA works** (exponents live mod φ).
- **Fermat:** p prime, p ∤ a ⇒ a^(p−1) ≡ 1 (mod p). Applications: primality testing, RSA.

**Why RSA decryption works:** ed ≡ 1 (mod φ) ⇒ ed = 1 + kφ ⇒ (M^e)^d = M·(M^φ)^k ≡ M·1 = M (mod n).

## 3.4 Square-and-Multiply (fast modular exponentiation) ⭐
Computing C^d directly is impossible for real d. Use the **binary expansion** of d: need C, C², C⁴, C⁸, … (one squaring each) and multiply the ones where the bit is 1. **O(log d)** multiplications instead of O(d).

**How to solve a^e mod n:**
1. Write e in binary / as sum of powers of 2.
2. Make table: a¹, a², a⁴, a⁸, … each = previous² mod n.
3. Multiply the needed ones (reducing mod n after each multiply).

**Slide example: 13²⁷ mod 55** (decrypt C=13, d=27)
```
27 = 16+8+2+1
13¹ = 13
13² = 169 mod 55 = 4
13⁴ = 4²  = 16
13⁸ = 16² = 256 mod 55 = 36
13¹⁶= 36² = 1296 mod 55 = 31
13²⁷ = 31·36·4·13 mod 55 :  31·36=1116→16 ; 16·4=64→9 ; 9·13=117→ 7   ⇒ M = 7 ✔
```
**My extra:** 7¹³ mod 11, 13=8+4+1. 7²=49→5, 7⁴=25→3, 7⁸=9 → 9·3·7=189 → **2**.

(Same idea reappears as **double-and-add** in ECC.)

---

# 4. Prime Numbers & Miller–Rabin

## 4.1 Why large primes?
RSA, DH, signatures need huge primes. Trial division is far too slow → generate random candidates and test with **Miller–Rabin**.

## 4.2 Prime generation process (slide 82)
1. Generate random number with required bit length
2. Set **highest bit** = 1 (ensures size)
3. Make it **odd** (set lowest bit)
4. **Small-prime check** (divisible by 3,5,7,11…? discard)
5. Run **Miller–Rabin** with enough rounds
6. Composite → new candidate
7. Passes → accept as **probable prime**

**Slide example (8-bit prime):** 01100110 → set top bit 11100110 (230) → make odd 11100111 (**231**) → 2+3+1=6, divisible by 3 → composite, retry. Attempt 2: 11011111 = **223** → not divisible by 3,5,7,11 → MR with a=2: 222 = 2¹·111 → passes (slide claims 2¹¹¹ ≡ −1; real value is 1; still a pass) → **223 accepted**.

## 4.3 Miller–Rabin algorithm ⭐⭐ (HIGH probability in exam)
For odd n:
1. Write **n − 1 = 2^s · d** with d **odd**.
2. Pick base **a**, 2 ≤ a ≤ n−2.
3. x = a^d mod n. If **x = 1 or x = n−1** → passes this round ("probably prime").
4. Repeat up to **s−1** times: x = x² mod n. If x = **n−1** → passes.
5. If never n−1 → **COMPOSITE** (definitely).

**Results:** "Composite" = **certain**. "Probably prime" = high confidence only (composite that passes for some base = **MR pseudoprime / strong liar**).

**Accuracy:** a composite passes a random round with prob **≤ 1/4**. After k rounds: false-positive ≤ **(1/4)^k**.
- k=5 → 1/1024 · k=10 → 1/1,048,576.

**No fixed max size**: works for hundreds/thousands of bits (32, 64, 128, 256, 512, 1024, 2048+); limit = memory/CPU/mod-exp speed.

### Worked examples (all from slides, verified)
**n = 29, a = 2:** 28 = 2²·7 → s=2, d=7. x = 2⁷ mod 29 = 128 mod 29 = **12** (≠1,28). Square: 12² = 144 mod 29 = **28 = n−1** ✓ → **probably prime**.

**n = 53, a = 2:** 52 = 2²·13 → s=2, d=13. x = 2¹³ mod 53 = 8192 mod 53 = **30**. Square: 900 mod 53 = **52 = −1** ✓ → probably prime (it is).

**n = 561, a = 2:** 560 = 2⁴·35 → s=4, d=35. x = 2³⁵ mod 561 = **263**. Squarings: 263² → **166**, 166² → **67**, 67² → **1**. Never got 560 (we got 1 *after* a non-±1 value, which is a nontrivial square root of 1 ⇒ composite) → **COMPOSITE** (561 = 3·11·17, a **Carmichael number**, fools Fermat test but not MR).

**My extra (n=133, a=2):** 132 = 2²·33. 2³³ mod 133 = 50; 50² = 2500 mod 133 = 106. Never 132 → **composite** (133=7·19).
**My extra (n=97, a=2):** 96 = 2⁵·3. 2³=8 → 64 → 22 → **96** ✓ → probably prime.

> **How to solve MR in the exam:** (1) get s,d, (2) compute x=a^d with square-and-multiply, (3) check 1 / n−1, (4) square up to s−1 times, (5) conclude. Write all steps; examiners give method marks.

## 4.4 Other primality / prime generation methods
| Method | Type | Speed | Notes |
|---|---|---|---|
| **Trial division** | Deterministic | Slow | Divide up to √n. Only small numbers / learning |
| **Sieve of Eratosthenes** | Deterministic | Very efficient for a range | All primes up to a limit; not for single huge primes |
| **Fermat test** | Probabilistic | Fast | Based on Fermat's little thm; **fooled by Carmichael numbers**; less reliable than MR |
| **Solovay–Strassen** | Probabilistic | Fast | Uses Jacobi symbol; better than Fermat; MR generally preferred |
| **Miller–Rabin** | Probabilistic | Fast | Industry standard for crypto |

**Advantages of MR:** fast, suitable for crypto, efficient mod-exp, works for 1000s of bits, more rounds → higher confidence.
**Limitations:** basic MR is probabilistic (small false-positive chance).
**Applications:** RSA key gen, DH, digital signatures, general key generation.

---

# 5. RSA ⭐⭐⭐

## 5.1 Facts
- Rivest–Shamir–Adleman, MIT, **1977**. Most widely used PK algorithm.
- Security: **difficulty of factoring large integers** (IFP).
- Uses: PKI, SSL/TLS certificates & key exchange, secure email (PGP), digital signatures.
- Enc key public ≠ dec key private.

## 5.2 Key generation
1. Choose two large primes **p, q**
2. **n = p·q**, **φ(n) = z = (p−1)(q−1)**
3. Choose **e**: 1 < e < φ(n), **gcd(e, φ(n)) = 1**
4. Compute **d = e⁻¹ mod φ(n)** (extended Euclid), i.e. ed ≡ 1 (mod φ)
5. **Public key = (n, e)**, **Private key = (n, d)**

## 5.3 Encrypt / Decrypt
- **Encrypt:** C = M^e mod n
- **Decrypt:** M = C^d mod n
- Plaintext as integer must be **< n** (same for ciphertext). Real RSA: n ≥ 1024 bits (slides: "at least 1024", p,q ≈ 512 bits; 2048 now standard).
- e = "public/encryption exponent", d = "private/decryption exponent".
- Alice → Bob: Alice uses **Bob's public key**; Bob decrypts with **his private key**.

## 5.4 Worked examples

### Slide Ex 1: Encrypt plaintext **9**, p=7, q=11
```
n = 77, φ = 6·10 = 60
e = 7 (gcd(7,60)=1) → public (7,77)
C = 9^7 mod 77 = 37
d: 7d ≡ 1 mod 60 → d = 43 → private (43,77)
M = 37^43 mod 77 = 9 ✔
```
### Slide Ex (lecture 2): p=5, q=11, e=3, M=7
```
n=55, φ=40, e=3 → d=27 (3·27=81=2·40+1). Public (3,55), Private (27,55)
C = 7³ mod 55 = 343 mod 55 = 13
M = 13²⁷ mod 55 = 7 (square-and-multiply, see §3.4)
```
### Slide MCQ type
- **p=13, q=17, public key e=35 → d?** φ=12·16=192. 35d≡1 (mod 192) → **d = 11** (35·11=385=2·192+1). ✔
- **p=3, q=11, private d=7, plaintext 5 → ciphertext?** n=33, φ=20, 7e≡1 mod 20 → **e=3** (the slide's "E=3"). C = 5³ mod 33 = 125 mod 33 = **26**. Check: 26⁷ mod 33 = 5 ✔
- *(slide's "p=3, q=13, e=3, d=7" is inconsistent — ignore.)*

### My extra (textbook): p=61, q=53, e=17, M=65
n=3233, φ=3120, d=17⁻¹ mod 3120 = **2753**. C = 65¹⁷ mod 3233 = **2790**. M = 2790²⁷⁵³ mod 3233 = 65 ✔.

> **How to solve any RSA numeric:** (1) n=pq (2) φ=(p−1)(q−1) (3) d via ext. Euclid (4) C=M^e mod n using square-and-multiply (5) verify by decrypting if time.

## 5.5 Designing a good RSA system (slides 129–131)
1. **Choosing p,q:** pick random large odd p, test with randomized primality test; if not prime try p+2, p+4, …
2. **Relation between p and q:**
   - |p − q| neither too small nor too large (too small → Fermat factoring)
   - gcd(p−1, q−1) should be **small**
   - p−1 and q−1 should have **large prime factors**
   - Ideal: **safe primes** (p and (p−1)/2 both prime; e.g. 83, 107)
3. **Choosing e, d:** neither should be **small**. **d should not be smaller than n^(1/4)** (else polynomial-time attack on d).
   - Common e values are small-ish (3, 65537) *but* with **padding** (OAEP) it's fine; tiny e + no padding is attack bait.

## 5.6 RSA Challenge (trivia)
- Martin Gardner, *Scientific American* 1977. n = 129 digits, e = 9007.
- Solved **1994** by factoring n into a **64-bit and 65-bit prime**.
- Plaintext: **"THE MAGIC WORDS ARE SQUEMISH OSSIFRAGE"**.

## 5.7 RSA security — 4 attack approaches
1. **Brute force key search** — infeasible
2. **Mathematical attacks** — three forms, *all believed equivalent to factoring*:
   - factor n=pq → compute φ(n) → d
   - determine φ(n) directly → d
   - find d directly
   - Progress comes from better **algorithms** (QS → GNFS / "LS"); slide: 200 decimal digits (**663-bit**) factored at time of slides. Assume **1024–2048-bit RSA secure** (use 2048+ in practice).
3. **Timing attacks** (Paul **Kocher**, mid-1990s) — time varies with operand size / branches during exponentiation → infer private exponent.
   **Countermeasures:** constant-time exponentiation, random delays, **blinding**.
4. **Chosen-ciphertext attacks (CCA)** — attacker gets decryptions of chosen ciphertexts exploiting RSA's algebra. **Counter:** random padding / **OAEP**.

---

# 6. Chinese Remainder Theorem & RSA-CRT ⭐⭐

## 6.1 CRT statement
If m₁, m₂, … pairwise **coprime**, the system
x ≡ a₁ (mod m₁), x ≡ a₂ (mod m₂), … has a **unique solution mod M = m₁m₂…**.
RSA view: since n=pq, gcd(p,q)=1: **x mod pq ⇄ {x mod p, x mod q}** (a number mod n can be represented by residues mod p and q and reconstructed).

## 6.2 Solving method ⭐ (the table method used in the slides)
**X = (a₁M₁M₁⁻¹ + a₂M₂M₂⁻¹ + …) mod M**
1. M = ∏mᵢ
2. Mᵢ = M/mᵢ
3. Mᵢ⁻¹ = inverse of Mᵢ **mod mᵢ** (Mᵢ·Mᵢ⁻¹ ≡ 1 mod mᵢ)
4. Plug in and reduce mod M.

### Slide Ex 1: X≡2 (mod 3), X≡3 (mod 5), X≡2 (mod 7)
| i | aᵢ | mᵢ | Mᵢ | Mᵢ⁻¹ |
|---|---|---|---|---|
| 1 | 2 | 3 | 35 | 2 (35≡2 mod 3, 2·2=4≡1) |
| 2 | 3 | 5 | 21 | 1 (21≡1 mod 5) |
| 3 | 2 | 7 | 15 | 1 (15≡1 mod 7) |

M=105. X = (2·35·2 + 3·21·1 + 2·15·1) mod 105 = (140+63+30)=233 mod 105 = **23** ✔ (23 mod 3=2, mod 5=3, mod 7=2).

### Slide Ex 2: X≡8 (mod 9), X≡3 (mod 20)
M=180; M₁=20, M₁⁻¹: 20x≡1 mod 9 → 20≡2 → x=5; M₂=9, M₂⁻¹: 9x≡1 mod 20 → x=9.
X = (8·20·5 + 3·9·9) mod 180 = (800+243)=1043 mod 180 = **143** ✔

### Slide "Try it yourself": X≡5 (mod 3), X≡2 (mod 5), X≡1 (mod 11) → **122**
(5≡2 mod 3.) M=165; M₁=55→55≡1 mod 3 → inv 1; M₂=33→33≡3 mod 5 → inv 2; M₃=15→15≡4 mod 11 → inv 3 (4·3=12≡1).
X = (2·55·1 + 2·33·2 + 1·15·3) = 110+132+45 = 287 mod 165 = **122** ✔

### Slide Ex (small, by inspection): X≡2 (mod 5), X≡7 (mod 11)
List 2,7,12,17,… numbers ≡2 mod 5; 7 mod 11 = 7 ✓ → **x ≡ 7 (mod 55)**.

### Two-modulus shortcut formula (slide 207)
x ≡ a (mod m₁), x ≡ b (mod m₂): **x = a + m₁ · [ (b − a) · m₁⁻¹ mod m₂ ]**
Test with a=2,m₁=5,b=7,m₂=11: 5⁻¹ mod 11 = 9 → (7−2)·9=45 mod 11 = 1 → x = 2 + 5·1 = **7** ✔

**CRT uses in crypto:** RSA decryption/signing speedup, secret sharing, residue number systems, homomorphic-encryption arithmetic.

## 6.3 RSA with CRT (decrypt/sign faster)
n=pq known to key owner, so decrypt in two **small** worlds:

1. **dP = d mod (p−1)**, **dQ = d mod (q−1)** (precompute)
2. **Mp = C^dP mod p**, **Mq = C^dQ mod q** *(slide writes M_p / M_q; call them a₁, a₂)*
3. Recombine with CRT: **M ≡ Mp (mod p), M ≡ Mq (mod q)** → M mod n
   Using slide notation: m₁=p, m₂=q, M₁=q, M₂=p → **P = (a₁·q·q⁻¹ + a₂·p·p⁻¹) mod n** where q⁻¹ is mod p and p⁻¹ is mod q.

### Slide Ex A: p=5, q=11, e=3, d=27, **C=13**
- dP = 27 mod 4 = **3**, dQ = 27 mod 10 = **7**
- Mp = 13³ mod 5 = 3³ mod 5 = 27 mod 5 = **2**
- Mq = 13⁷ mod 11 = 2⁷ mod 11 = 128 mod 11 = **7**
- Solve M≡2 (mod 5), M≡7 (mod 11) → **M = 7** (same as direct) ✔

### Slide Ex B (slide left blank — I filled it): p=7, q=13, e=5, d=29, M=12
- C = 12⁵ mod 91 = **38**
- dP = 29 mod 6 = **5**, dQ = 29 mod 12 = **5**
- Mp = 38⁵ mod 7 = 3⁵ mod 7 = 243 mod 7 = **5**
- Mq = 38⁵ mod 13 = 12⁵ mod 13 = (−1)⁵ = −1 = **12**
- M≡5 (mod 7), M≡12 (mod 13) → candidates ≡12 mod 13: 12 (12 mod 7 = 5 ✓) → **M = 12** ✔

### Slide Ex C (use **corrected** numbers): p=11, q=17, e=7, d=23, M=20
- **C = 20⁷ mod 187 = 147** (slide wrongly prints 98)
- dP = 23 mod 10 = **3**, dQ = 23 mod 16 = **7**
- Mp = 147³ mod 11 = 4³ mod 11 = 64 mod 11 = **9**
- Mq = 147⁷ mod 17 = 11⁷ mod 17 = **3**
- M≡9 (mod 11), M≡3 (mod 17) → **M = 20** ✔ (20 mod 11 = 9, 20 mod 17 = 3)

### Slide Ex D (full walkthrough): p=11, q=13, e=7, d=103, P=42
- n=143, φ=120. C = 42⁷ mod 143 = **81**.
- **Without CRT:** 81¹⁰³ mod 143 with 103 = 64+32+4+2+1. Squares: 81¹=81, 81²=126, 81⁴=3, 81⁸=9, 81¹⁶=81, 81³²=126, 81⁶⁴=3. Product: 3·126·3·126·81 → running reductions 3, 92, 133, 27, **42** ✔
- **With CRT:** dP = 103 mod 10 = 3, dQ = 103 mod 12 = 7. a₁ = 81³ mod 11 = 4³ mod 11 = **9**; a₂ = 81⁷ mod 13 = 3⁷ mod 13 = **3**.
  Table: M=143; (a₁=9, m₁=11, M₁=13, M₁⁻¹=6); (a₂=3, m₂=13, M₂=11, M₂⁻¹=6).
  P = (9·13·6 + 3·11·6) mod 143 = (702+198)=900 mod 143 = **42** ✔

| | Without CRT | With CRT |
|---|---|---|
| Exponentiation | 81¹⁰³ mod 143 | 81³ mod 11 and 81⁷ mod 13 |
| Modulus size | 143 (8 bits) | 11 and 13 (4 bits each) |
| Exponent size | 103 (7 bits) | 3 (2 bits), 7 (3 bits) |
| Recombination | none | 900 mod 143 |
| Result | 42 | 42 |
**Advantage = smaller operands.**

> **How to solve RSA-CRT in exam:** dP,dQ → two small exponentiations → CRT table → final M. **State that it equals direct result.**

## 6.4 WHY is RSA-CRT faster? ⭐ (theory question)
**Answer is NOT "fewer exponentiations" — it's "smaller arithmetic".** CRT trades **one** big problem for **two** smaller ones. (n is L bits; p, q ≈ L/2 bits each; dP, dQ also ≈ L/2 bits.)

**Slide model A (single-multiplication view):** schoolbook multiply cost O(k²).
One 2048-bit mult ≈ 2048² = 4,194,304 units; one 1024-bit mult ≈ 1024² = 1,048,576 → ~**4× cheaper per multiplication**. Two half-size exps: 2×1024² vs 1×2048² → ratio **2×** if you only count same number of mults.

**Slide model B (full exponentiation, ⭐ the headline figure):**
Mod-exp ≈ O(L) multiplications each O(L²) ⇒ **T(L) ≈ aL³**.
- Direct RSA: aL³
- RSA-CRT: 2·T(L/2) = 2·a(L/2)³ = **aL³/4**
- ⇒ **≈ 4× speed-up** (assuming p,q similar size). Real-world quoted "3–4×".

| RSA n | Direct | CRT per branch |
|---|---|---|
| 2048 bits | 2048-bit | ~1024-bit |
| 4096 bits | 4096-bit | ~2048-bit |
The ciphertext/plaintext stays the same size. The two branches can run **in parallel**.

**Square-and-Multiply vs CRT:** *complementary, not competing.* S&M reduces **number** of operations (O(d)→O(log d)); CRT reduces **size** of each operation. S&M can be used **inside** each CRT branch.

**Trade-offs:** needs two exponentiations + recombination; more complex implementation; **fault attacks** (a faulty branch result can leak the factorization). CRT also speeds up RSA **signing** (private exponent). Only the **private-key** holder can use CRT (needs p,q).

---

# 7. Textbook RSA is insecure → OAEP ⭐⭐

**Textbook RSA:** C = M^e mod n. Math trapdoor ≠ secure encryption. It's **deterministic**, **malleable**, no semantic security.

## 7.1 Weakness #1 — Deterministic
Same M + same public key ⇒ same C. If message space small (e.g. "YES"/"NO", or a 4-digit PIN: 0000–9999 = 10,000 options) the attacker **encrypts every candidate with the public key and compares** to the intercepted C (**short-message / dictionary attack**). No private key needed.

## 7.2 Weakness #2 — Small message / low exponent
If e = 3 and M³ < n, **no modular reduction** happens: C = M³ as an integer → attacker takes **integer cube root**.
Example: n=55, e=3, M=2 → C=8 → ∛8 = 2.

## 7.3 Weakness #3 — Multiplicative (malleability / homomorphism)
**Enc(M₁)·Enc(M₂) mod n = Enc(M₁M₂ mod n)** since (M₁^e)(M₂^e) = (M₁M₂)^e.
Attacker turns Enc(M) into Enc(2M): **C·2^e mod n = Enc(2M)** without the private key → enables chosen-ciphertext tricks.

**Fix:** randomized padding/encoding **before** RSA → **OAEP** (Optimal Asymmetric Encryption Padding). Plain "append extra bits" makes guessing harder but doesn't guarantee security.

## 7.4 OAEP — Version 1 (lecture 22 notation, formulas!)
**Notation:** M = padded message (m bits); r = one-time random (k bits, k<m); P₁ = masked message (m bits); P₂ = recovery info (k bits); P = P₁‖P₂ (m+k bits); ⊕ = XOR; ‖ = concatenation.
**Public functions:** **G: {0,1}ᵏ → {0,1}ᵐ** (expands r into an m-bit mask; a public one-way fn) and **H: {0,1}ᵐ → {0,1}ᵏ** (a cryptographic hash).

**Encryption (Alice):**
1. Pad message → M (m bits)
2. Fresh random r (k bits), used once
3. **P₁ = M ⊕ G(r)**
4. **P₂ = H(P₁) ⊕ r**
5. **P = P₁ ‖ P₂** (must satisfy P < n)
6. **C = P^e mod n**

**Decryption (Bob):**
1. P = C^d mod n (keep fixed length incl. leading zeros)
2. Split P → P₁ (m bits), P₂ (k bits)
3. **r = H(P₁) ⊕ P₂**   (since H(P₁)⊕[H(P₁)⊕r] = r)
4. **M = P₁ ⊕ G(r)**   (since [M⊕G(r)]⊕G(r) = M)
5. Remove padding → original message

**XOR reverses itself:** (A⊕B)⊕B = A.

| | Direct RSA | RSA-OAEP |
|---|---|---|
| Input to RSA | original plaintext | P₁‖P₂ |
| Fresh randomness | none | k-bit r |
| Masking | none | M ⊕ G(r) |
| Repeated message | same ciphertext | fresh r changes encoding |
| Guessing attack | encrypt & compare | randomized encoding |

## 7.5 OAEP — Version 2 (lecture 21 — RSAES-OAEP / PKCS#1 style)
- **DB = lHash ‖ PS ‖ 0x01 ‖ M** (label hash, zero padding string, delimiter, message)
- **seed** = fresh random per encryption (this makes encryption **probabilistic**)
- **MGF1** = Mask Generation Function based on a hash (expands a short input into a mask of needed length)
- **maskedDB = DB ⊕ MGF(seed)**
- **maskedSeed = seed ⊕ MGF(maskedDB)**
- **EM = maskedSeed ‖ maskedDB**, then **C = EM^e mod n**

**Decrypt:** RSA private op → EM → split → seed = maskedSeed ⊕ MGF(maskedDB) → DB = maskedDB ⊕ MGF(seed) → parse DB = lHash‖PS‖0x01‖M & **verify structure** → if invalid **reject** (don't return arbitrary plaintext).

**Same message, two encryptions:** M=HELLO, seed r₁ → EM₁ → C₁; seed r₂ ≠ r₁ → EM₂ → C₂; **C₁ ≠ C₂** (overwhelming probability).

**Toy XOR (slide):** DB=10110010, mask=01100101 → maskedDB = **11010111**. XOR reversible: DB = maskedDB ⊕ mask. *Changing the seed changes the mask → changes EM → changes C.*

## 7.6 Putting it together (modern RSA)
**Plaintext M → OAEP → EM → RSA public op (C = EM^e mod n) → ciphertext.**
Decrypt: RSA private op (usually **CRT-optimized**, with S&M inside) → OAEP decode → M.

| Technique | Problem solved | Type |
|---|---|---|
| **Square-and-Multiply** | exponentiation too expensive | efficiency (fewer ops) |
| **CRT / RSA-CRT** | private op uses huge modulus | efficiency (smaller ops) |
| **OAEP** | textbook RSA deterministic & structurally unsafe | **security** |

**Take-home:** RSA = trapdoor math · S&M = efficient exponentiation · CRT = efficient private-key op · OAEP = randomized secure encoding.

## 7.7 Misconceptions (slide 235)
- "CRT reduces one exponentiation to one" ❌ → makes **two smaller** ones
- "CRT changes RSA's plaintext" ❌ → reconstructs the **same** result mod n
- "CRT is a padding scheme" ❌ → efficiency
- "S&M and CRT are competing" ❌ → S&M works inside each CRT branch
- "OAEP makes RSA faster" ❌ → security encoding; adds processing
- "Padding = adding random zeroes" ❌ → structured randomized encoding (hash, MGF1, XOR)
- "Textbook RSA secure because RSA math is hard" ❌ → needs proper padding

---

# 8. Diffie–Hellman Key Exchange (DHKE) ⭐⭐⭐

- **Whitfield Diffie & Martin Hellman, 1976**; first published public-key (asymmetric) protocol. Purpose: two parties establish a **shared secret over an insecure channel**, no prior secret.
- Security rests on **DLP / Diffie–Hellman Problem (DHP)** in a finite cyclic group.
- Color analogy: agree public colour → each adds private colour → exchange mixtures → add own private colour again → same final colour. (Eve sees mixtures but can't "un-mix".)

## 8.1 Algorithm (notation varies: (p,g) = (n,g) = (q,α))
**Public:** large prime **p** and generator/base **g**.
1. Alice picks secret **a**, Bob picks secret **b**
2. Alice sends **A = gᵃ mod p**; Bob sends **B = gᵇ mod p**
3. Alice computes **K = Bᵃ mod p**; Bob computes **K = Aᵇ mod p**
4. **K = g^(ab) mod p** — same for both: (gᵇ)ᵃ = (gᵃ)ᵇ.

Eve sees p, g, A, B but would have to solve DLP to get a or b.

## 8.2 Worked examples (all slide examples, verified)

**Ex 1 (n=11, g=7, x=3, y=5)**
- A = 7³ mod 11 = 343 mod 11 = **2**
- B = 7⁵ mod 11 = 16807 mod 11 = **10**
- Alice: K = 10³ mod 11 = **10** · Bob: K = 2⁵ mod 11 = **10** ✔

**Ex 2 (P=23, G=9, a=4, b=3)**
- x = 9⁴ mod 23 = 6561 mod 23 = **6** · y = 9³ mod 23 = 729 mod 23 = **16**
- ka = 16⁴ mod 23 = 65536 mod 23 = **9** · kb = 6³ mod 23 = 216 mod 23 = **9** → shared secret **9**

**Exercise 1 (q=353, α=3, X_A=97, X_B=233)**
- Y_A = 3⁹⁷ mod 353 = **40** · Y_B = 3²³³ mod 353 = **248**
- K = 248⁹⁷ mod 353 = **160** = 40²³³ mod 353 ✔

**Exercise 2 (q=23, α=5, X_A=6, X_B=15)**
- Y_A = 5⁶ mod 23 = **8** · Y_B = 5¹⁵ mod 23 = **19**
- K = 19⁶ mod 23 = **2** = 8¹⁵ mod 23 ✔

> **How to solve DH:** compute both public values (square-and-multiply), exchange, compute the key **both ways** and check they match. Matching = built-in verification ✅.

## 8.3 Man-in-the-Middle (MITM) attack ⭐ (slides 265–268)
**DH gives no authentication.** Eve sits in between and runs a separate DH with each side.
Setup n=11, g=7. Alice x=3; Eve picks x=8 (toward Bob) and y=6 (toward Alice); Bob y=9.
- Alice's A = 7³ = **2** → Eve intercepts, sends Bob **A = 7⁸ mod 11 = 9**
- Bob's B = 7⁹ mod 11 = **8** → Eve intercepts, sends Alice **B = 7⁶ mod 11 = 4**
- **Alice's key K1 = 4³ mod 11 = 9** ← Eve computes 2⁶ mod 11 = **9** (same) ✔
- **Bob's key K2 = 9⁹ mod 11 = 5** ← Eve computes 8⁸ mod 11 = **5** (same) ✔
- So Alice↔Eve share 9, Eve↔Bob share 5. Eve decrypts, reads/modifies, re-encrypts. Alice & Bob think they share a key with each other. 😈

(Also the colour-mixing demo with z=4, T=7⁴ mod 11 = 3 gives K_A=10⁴ mod 11=1, K_B=2⁴ mod 11=5 — keys differ, nobody agrees, and Eve has both.)

**Solution:** **User authentication required** → digital signatures + **certificates** (signed DH / authenticated key exchange). Same caveat for **ECDH**.

## 8.4 ElGamal link
**ElGamal encryption** is an extension of DHKE: the derived session key is used as a **multiplicative mask** on the message. Security = DHP/DLP. (ECDSA derives from ElGamal signature scheme per slides.)

---

# 9. Discrete Logarithm Problem (DLP) ⭐

- Security of DH, ElGamal, DSA relies on **intractability of DLP**; DLP is defined over a **finite cyclic group G**.
- **DLP in Zp\*:** given α, β, p find integer x with **αˣ ≡ β (mod p)**. Modular exponentiation is easy, finding x is hard → one-way function.
- x = "discrete logarithm of β to base α".
- **Tiny example:** 2ˣ ≡ 3 (mod 5): table x=1→2, 2→4, 3→3 ⇒ **x = 3** (2³=8≡3).
- **My extra:** 3ˣ ≡ 13 (mod 17): 3,9,10,13 → **x = 4** (3⁴ = 81 = 4·17 + 13).
- **Generalization:** any cyclic group; e.g. **ECDLP**: find d with **dP = T** (P + P + … + P). Point multiplication via **double-and-add** (analog of square-and-multiply). ECC needs **160–256-bit** operands vs **1024–3072-bit** for Zp\* at same security.

## 9.1 Attacks on DLP
| Attack | Class | Cost | Notes |
|---|---|---|---|
| **Index-calculus** | Non-generic (uses structure) | **Subexponential** | Strongest vs Zp\* and GF(2^m)\*. Forces **large p** (1024-bit for 80-bit security). **NOT applicable to elliptic-curve groups** → why ECC keys are short |
| **BSGS (Shanks), Pollard's ρ** | Generic (any group) | ~**√\|G\|** | Pollard-ρ = best known vs EC groups. Need group order ≥ **2¹⁶⁰ (80-bit)**, **2²⁵⁶ (128-bit)** |
| **Pohlig–Hellman** | Exploits small prime factors of group order \|Zp\*\|=p−1 | | Largest prime factor of order must be big (≥160 bits) → DH often uses a **prime-order subgroup q** |

---

# 10. Elliptic Curve Cryptography (ECC) ⭐⭐

## 10.1 Basics
- Modern PK family based on **elliptic curves over finite fields** & hardness of **ECDLP**. Natural successor to RSA: **smaller keys/signatures, very fast keygen, key agreement, signatures**.
- Elliptic curves are **not ellipses**; they're **cubic** curves: **y² = x³ + ax + b** (need non-singular: 4a³+27b² ≠ 0).
- Properties: **symmetric about x-axis**; a non-vertical line meets the curve in **at most 3 points**; points form a group: P₃ = P₁ + P₂.
- Over **reals** for intuition; in crypto, over **Z_p** (all values in a finite field).

## 10.2 Point addition (geometric = slide "dot")
Draw line through A and B → meets curve at third point **−C** → **reflect over x-axis** → **C = A + B**. For doubling, use the **tangent** at A.

**Formulas over Z_p (bonus — handy for numerics):**
- P=(x₁,y₁), Q=(x₂,y₂), P≠Q: **λ = (y₂−y₁)/(x₂−x₁) mod p**
- Doubling (P=Q): **λ = (3x₁² + a)/(2y₁) mod p**
- **x₃ = λ² − x₁ − x₂ , y₃ = λ(x₁ − x₃) − y₁** (all mod p)
- P + (−P) = **O** (point at infinity = identity); −P = (x, −y)

**Worked mini-example (curve y² = x³ + 2x + 2 over Z₁₇, P = (5,1))**
Doubling: λ = (3·25 + 2)/(2·1) = 77/2 mod 17 = 9·9 = 81 mod 17 = **13**. x₃ = 13² − 5 − 5 = 159 mod 17 = **6**; y₃ = 13(5−6) − 1 = −14 mod 17 = **3**. So **2P = (6,3)**. Continuing: 3P=(10,6), 4P=(3,1), 5P=(9,16), 6P=(16,13), 7P=(0,6), … the group has **order 19**.

## 10.3 ECDLP & trapdoor
- **Easy:** given k and P compute Q = kP. **Hard:** given Q and P find k (**ECDLP**). Private key = k (big), public = point Q.
- **Hops analogy (slides):** start at A, apply "dot" repeatedly to hop around curve to E. **Public key:** start A and end E. **Private key:** the **number of hops**. Knowing hops → easy; knowing only A and E → nearly impossible.

## 10.4 Double-and-Add (point multiplication T = dP) ⭐
Analogue of square-and-multiply (square → double, multiply → add).
```
T = P
for i = t−1 down to 0:
    T = T + T            (DOUBLE)
    if d_i == 1: T = T + P   (ADD)
return T
```
**Slide example 26P (26 = 11010₂, so t=4):**
| Step | Bit | Action | Result |
|---|---|---|---|
| #0 | d₄=1 | init | P |
| #1 | d₃=1 | DOUBLE, ADD | 2P → 3P |
| #2 | d₂=0 | DOUBLE | 6P |
| #3 | d₁=1 | DOUBLE, ADD | 12P → 13P |
| #4 | d₀=0 | DOUBLE | **26P** |
Only **4 doublings + 2 additions** instead of 25 additions.

## 10.5 ECC algorithms
- **ECDSA** — digital signatures
- **ECDH** — key agreement
- **ECIES** — hybrid encryption scheme

### ECDSA
ECC + hashing (SHA-256 / SHA-3); curves like **secp256k1**, **Curve25519**. Gives authenticity, integrity, non-repudiation.
- **Sign:** uses message hash + sender's **private key** + **random nonce k** → signature (r, s)
- **Verify:** receiver uses sender's **public key** to check (r,s) for that message
- Used in **Bitcoin/Ethereum** transactions, TLS certs, smart cards / digital IDs.
- **Weaknesses:** **nonce k reuse or poor randomness ⇒ attacker recovers private key instantly**; complex math → coding mistakes; **side channels** (timing, power, EM).

### ECDH
Classic DH in multiplicative group mod p; **ECDH in additive group of curve points**. Security = ECDLP; no sub-exponential algorithm known → same security with much smaller keys. Used in **TLS handshakes**, VPNs (OpenVPN, IKEv2/IPsec, **WireGuard**).
- **Weaknesses:** **invalid-curve attacks** (implementation must validate received points are on the curve); **only key agreement, not authentication → MITM** unless combined with signatures/certificates.

## 10.6 Why ECC? Security equivalence
| Symmetric security | RSA / DL key | ECC key |
|---|---|---|
| 128-bit (e.g. AES-128) | **3072 bits** | **256 bits** |
- "How to strengthen RSA? Increase key length — impractical." ECC scales better.
- **Benefits:** shorter keys, faster ops (esp. sign/verify), storage & bandwidth savings; same security services (confidentiality, integrity, authentication, non-repudiation).
- **Applications:** wireless devices, smart cards, busy web servers, constrained devices (IoT).

---

# 11. Digital Signatures & Certificates ⭐⭐

## 11.1 Why signatures?
**Scenario 1:** SoftAreUs releases a software update U. Customers download from public site / P2P. How do they verify U truly came from SoftAreUs?
**Scenario 2:** Bob gets an email "from Alice" — how does he verify?
**Electronic records:** easy to copy, fast to distribute, copies as good as originals, easily modifiable → lack **integrity & authenticity**.

**Digital signature** = mathematical scheme to authenticate sender of an electronic document; an attachment that represents document content **and** signer identity uniquely. Provides **Authenticity, Integrity, Non-repudiation** (for e-commerce / e-governance).
Used by: HR, governments, cryptocurrencies, financial sector.

## 11.2 Process (3 steps)
**Key generation → Signing → Verification**

**Sign:** (1) **hash** the message → digest; (2) **encrypt digest with signer's PRIVATE key** → signature. Send message + signature.
**Verify:** (1) hash received message; (2) **decrypt signature with signer's PUBLIC key** → get digest; (3) **compare**. Match ⇒ valid.

Why sign the **digest**, not the message? Efficiency (PKC is slow) & fixed-size input.

> **Key roles flip!** Encryption: **receiver's public** → receiver's private. Signature: **sender's private** → sender's public.

**Key point (slide):** signatures are **numbers**, they're **document-content dependent, not person dependent** (different docs → different signature, even from same person).

## 11.3 Services
| Service | Provided by signature? |
|---|---|
| Message **authentication** | ✅ |
| Message **integrity** | ✅ |
| **Non-repudiation** | ✅ (using a **trusted center** to settle disputes) |
| **Confidentiality** | ❌ not by default — add encryption on top |

Importance (slide): authenticity, non-repudiation (can't deny signing at that time), security vs tampering, efficiency (less paperwork), compliance.

## 11.4 RSA signatures
- **Sign:** compute hash h of message, **s = h^d mod n** (private exponent). Signature is an integer.
- **Verify:** compute h, compute **s^e mod n**, compare to h.
- **My extra (p=5,q=11,e=3,d=27):** h=9 → s = 9²⁷ mod 55 = **4**; verify 4³ mod 55 = **9** ✔
- Security: IFP. CRT speeds up signing too.

## 11.5 DSA & others
**DSA (Digital Signature Algorithm):** signature-only (no encryption). Private key signs, public key verifies. (NIST/NSA, used with SHA-1 originally.)
Slide cons: limited key length; rigid key management; (slide claims limited certificate support); "not incremental"; relatively new/less vetted; **larger signatures** → storage/transmission impact.

| | DSA (as per slide) | RSA |
|---|---|---|
| Purpose | **Signature only** | **Signature + encryption** |
| Speed | "faster, simpler" (slide) | "slower, harder to implement" (slide) |
| Security claim | slide says "more secure" | slide says "less secure" |
> Real world: it's not that clear-cut (RSA verify is fast; DSA signing is fast). If the question says "according to slides," write the slide claim.

**ECDSA** derived from the **ElGamal signature scheme** (slides). **Applications:** e-mail, data storage, EFT, software distribution, smart cards, ISDN, time-stamped signatures, **blind signatures**.

## 11.6 Digital Certificates & PKI
**PKI recap:** two keys; only the public key encrypts / only the matching private key decrypts; infeasible to derive private from public.

**Certificate** = electronic file tied to a key pair that **authenticates identity** (website, person, org, device, server) — binds **identity ↔ public key**.
**Contains:** user's name; company/department; IP or device serial number; **copy of holder's public key**; **validity period**; **domain** it's authorized for. *(+ CA's signature.)*

**Types:**
- **TLS/SSL certificates** — server presents it to prove it's authentic
- **Client certificates** — identify a user/machine (access DBs, email)
- **Code-signing certificates** — publisher signs software so downloaders can trust it

**Issuer:** **Certifying Authority (CA)** — trusted third party that verifies identities then issues certificates (legally licensed under the law).

**How it works (7 steps):**
1. Entity generates key pair
2. Keeps private key secret; submits **public key + identity info** to CA
3. CA **verifies identity** (documents, phone, in-person…)
4. CA creates certificate containing public key + identity
5. **CA signs the certificate with its own private key**
6. Signed cert issued; can be freely distributed
7. Others use the public key in it to encrypt / verify signatures

**Challenges:**
| Challenge | Risk if poorly handled |
|---|---|
| Certificate management | Wrong/expired certs get trusted |
| **Private key security** | Forged signatures |
| **CA trustworthiness** | Fraudulent identity-to-key bindings |
| **Revocation effectiveness** | Compromised keys still accepted |

**Link back to DH:** DH/ECDH is vulnerable to MITM → certificates + signatures authenticate the public values.

---

# 12. Hash Functions ⭐⭐

## 12.1 Basics
Hash function maps **arbitrary-size input → fixed-size output** (hash value / digest / hash code). Used for: password storage, integrity, digital signatures, authentication, file organization.
Example (slide): SHA-256("Hello") = 2cf24dba5fb0a30e…; "hello" (one letter changed) → completely different hash.

## 12.2 Properties of a strong hash
| Property | Meaning |
|---|---|
| **Determinism** | same input → same output; **fixed-size output** regardless of input size |
| **Pre-image resistance** | infeasible to recover input from hash (**one-way**) |
| **Second-preimage resistance** | given x, infeasible to find y≠x with H(y)=H(x) |
| **Collision resistance** | infeasible to find any two inputs with same hash; a collision = broken/insecure |
| **Avalanche effect** | tiny input change ⇒ massive output change |
| **Speed** | reasonably fast to compute |

**Attacks:** brute force & cryptanalysis. Preimage/2nd-preimage: find y with H(y)=given hash. Collision: find x,y with H(x)=H(y).
**Birthday bound:** collision strength ≈ **2^(m/2)** for m-bit hash → **128-bit hash inadequate, 160-bit suspect**.

**Why collisions are dangerous (slide 377):** A makes two messages with same hash → shows B the **innocent** one → gets B's **signature** → attaches signature to the **malicious** one (same hash ⇒ valid!).

**Why not CRC/checksums?** Only detect accidental errors (hardware/transmission), no real integrity protection, easily circumvented.

## 12.3 Uses
- **Passwords:** store hash not password (Unix, Windows NT); **salt** to deter precomputation (**rainbow tables**).
- **Integrity / intrusion & virus detection:** keep hashes of files, re-check (**Tripwire**).
- **Digital signatures:** sign the hash.
- **PRF / PRNG:** generate session keys, nonces; derive keys from password / master key.
- **Lamport one-time passwords:** Alice picks password P; hashes it **N times**; server stores (Alice, N, **H^N(P)**). Server compromise or interception doesn't reveal P (can't invert H). Next login Alice sends H^(N−1)(P); server hashes once & compares, then stores that.

## 12.4 Iterated hashes & compression function
**Compression function** f: compresses n-bit input to m-bit output (n > m); used repeatedly on fixed-size blocks.
Hash families: **Merkle–Damgård** (MD, SHA) and **block-cipher based** (Rabin, Davies–Meyer, Matyas–Meyer–Oseas, Miyaguchi–Preneel).

## 12.5 Merkle–Damgård (MD) construction ⭐⭐
Builds a hash for arbitrary-length messages from a collision-resistant **fixed-size compression function f** (Ralph Merkle). Used in **MD5, SHA-1, SHA-2 (SHA-256)**.

**Steps:**
1. **Pad** message + append **length** (**length strengthening**) so total length is a multiple of block size b.
2. Split into **t blocks M₁…M_t**.
3. Start with fixed **IV = H₀**. For i = 1..t: **Hᵢ = f(Hᵢ₋₁, Mᵢ)**.
4. **Final H_t = digest.**

**Why:** compression fn takes fixed-size inputs; MD chains it so output of each block becomes the state input of the next.
**Why padding + length?** Makes message fit blocks; length strengthening removes ambiguity & is needed for the security proof.
**Security theorem:** if f is collision-resistant, a properly padded MD hash is collision-resistant. (IV & padding rules matter.)
**Pros:** handles any length; simple iterative; clear security reduction. 
**Limitation:** **length-extension attack** → for MACs use **HMAC**. Also MD5/SHA-1 are MD-based but broken → distinguish *construction* from *specific algorithm*.

**Slide Q&A:**
- *Message bigger than compression input?* → split into blocks, process sequentially; each output feeds next.
- *Change one bit in the first block?* → changes chaining state → affects **all later blocks** → final hash changes a lot (avalanche).
- *Why padding + length strengthening?* → fit block size + record length to avoid ambiguity.

## 12.6 MD5 ⭐
- Ron **Rivest, 1991**, **RFC 1321**. Intended for signature apps (compress large file before signing with RSA).
- **128-bit** digest, MD construction, **512-bit blocks** (sixteen 32-bit words).
- State: **A,B,C,D** (four 32-bit words), init (hex): A=`01234567`, B=`89abcdef`, C=`fedcba98`, D=`76543210` (shown low-order bytes first in RFC).
- **4 rounds × 16 steps = 64 steps**; each uses non-linear fn, modular addition, left rotation.

| Round | Fn | Formula | Steps |
|---|---|---|---|
| 1 | F | (B∧C) ∨ (¬B∧D) | 1–16 |
| 2 | G | (B∧D) ∨ (C∧¬D) | 17–32 |
| 3 | H | B⊕C⊕D | 33–48 |
| 4 | I | C ⊕ (B ∨ ¬D) | 49–64 |

**Algorithm steps:**
1. **Append padding bits:** make length ≡ **448 (mod 512)**: one `1` bit then `0`s. **Always** pad (even if already 448 mod 512); 1 to 512 bits added.
2. **Append length:** 64-bit length of original message (low-order 64 bits if > 2⁶⁴) → total multiple of 512.
3. **Initialize** MD buffer ABCD.
4. **Process each 16-word (512-bit) block** through the 4 rounds.
5. Output = A‖B‖C‖D.

**MD5 vs MD4:** extra (4th) round; **unique additive constant per step**; function g in round 2 changed from (XY∨XZ∨YZ) to (XZ ∨ Y¬Z); each step adds result of previous step; word access order changed in rounds 2,3; shift amounts optimized/distinct.

**MD5 flaws:** **Collisions** (Wang et al. **2004**: hours on an IBM p690 cluster; Lenstra et al. **2005**: hours on a single laptop) and **rainbow tables** for passwords. A digest algorithm doesn't give integrity if collisions are easy. → use SHA/Whirlpool. MD5 only OK as a checksum vs **accidental** corruption.

## 12.7 SHA-1 ⭐
- Designed by **NIST & NSA**; SHA (1993, "SHA-0") → revised **SHA-1 (1995)**; FIPS 180-1 (1995), RFC 3174; now FIPS PUB 180-4. Standard (SHS) for use with **DSA**. Based on MD5 design.
- **160-bit** digest (**20 bytes = 40 hex digits**), **512-bit blocks**, **80 rounds** (4 rounds × 20 steps), MD structure, **5 chaining variables A–E**.
- **Insecure since 2005** (theoretical collision results); Microsoft/Google/Apple/Mozilla **stopped accepting SHA-1 TLS certs by 2017**.

**Algorithm:**
1. **Pad** to 448 mod 512 (+ 64-bit length), divide into **512-bit blocks**.
2. **Initialize chaining variables** A,B,C,D,E.
3. For each block: copy A–E → a–e; split block into **16 × 32-bit words**; **expand to 80 words W[t]**.
4. **80 iterations**, 4 constants: **K₁ (0–19), K₂ (20–39), K₃ (40–59), K₄ (60–79)**, each round with its own logical function P (process P).
   **One iteration:** (a,b,c,d,e) ← ( e + P(b,c,d) + **S⁵(a)** + W[t] + K[t] , a , **S³⁰(b)** , c , d ) where Sⁿ = circular **left shift by n bits**.
5. After 80 rounds, **add** a–e to previous chaining values → new A–E. Final A‖B‖C‖D‖E = digest.

## 12.8 MD5 vs SHA-1 vs SHA-256 quick table
| | MD5 | SHA-1 | SHA-256 |
|---|---|---|---|
| Digest | 128 | 160 | 256 |
| Block | 512 | 512 | 512 |
| Steps | 64 | 80 | 64 |
| State | 4 words (A–D) | 5 words (A–E) | 8 words |
| Status | **Broken** | **Deprecated** | OK |
| Birthday cost | 2⁶⁴ (but real attacks far cheaper) | 2⁸⁰ | 2¹²⁸ |

**Padding how-to (exam numeric!):**
- Message length L bits. Need **L + 1 + k + 64 ≡ 0 (mod 512)** (k zero bits, smallest k ≥ 0).
- Equivalent: pad to ≡448 mod 512, then add 64-bit length.
- **"abc" (24 bits):** k = 448 − 24 − 1 = **423** zeros → 24+1+423+64 = **512** → **1 block**.
- **1000-bit message:** 1000+1 = 1001 → next ≡448 (mod 512) is 1472 → k = **471** → total 1472+64 = **1536 = 3 blocks**.
- If L mod 512 ≥ 448 you need an **extra block**.

---

# 13. 🧪 Exam Kit

## 13.1 Likely short-answer questions + crisp answers
1. **Why PKC?** Key distribution (n(n−1)/2 keys, secure channel needed) & digital signatures.
2. **Symmetric vs asymmetric?** Same key / key pair; fast / ~1000× slower; key distribution issue / solves it; O(n²) keys vs O(n) key pairs.
3. **What is a trapdoor one-way function?** Easy forward; hard inverse without secret trapdoor.
4. **Why hybrid encryption?** PKC slow, AES fast: PKC/DH protects/agrees key, AES encrypts data.
5. **Why is RSA slower than AES?** Big-integer modular exponentiation (2048-bit) vs small fixed 128-bit block ops.
6. **MR: why probabilistic?** Composite may pass for some bases (strong liars); error ≤ ¼ per round → (¼)^k.
7. **RSA correctness proof?** ed = 1+kφ + Euler.
8. **Why does RSA need φ(n) secret?** Knowing φ gives d (and equivalent to factoring n).
9. **Why CRT speeds RSA?** Two exponentiations on half-size operands: aL³ → 2a(L/2)³ = aL³/4 (≈4×).
10. **Textbook RSA flaws?** Deterministic, small-m/small-e cube-root, malleable (multiplicative). Fix → OAEP.
11. **What does OAEP do?** Randomized masking before RSA: P₁=M⊕G(r), P₂=H(P₁)⊕r; same M → different C.
12. **DH weakness?** No authentication → MITM; fix with signatures/certificates.
13. **Why are ECC keys shorter?** Index-calculus doesn't apply; only exponential generic attacks (Pollard-ρ) → 256-bit ECC ≈ 3072-bit RSA.
14. **ECDSA danger?** Nonce reuse leaks private key.
15. **What do digital signatures provide?** Authentication, integrity, non-repudiation.
16. **Role of a CA?** Trusted third party verifying identity & signing the certificate binding identity↔public key.
17. **Hash properties?** preimage, 2nd-preimage, collision resistance, avalanche, determinism/fixed length, speed.
18. **MD construction & weakness?** Hᵢ=f(Hᵢ₋₁,Mᵢ), IV, padding+length; length-extension; use HMAC.
19. **Why is MD5 broken?** Practical collisions (Wang 2004); SHA-1 deprecated (2005 results, certs dropped 2017).
20. **Padding in MD5/SHA-1?** `1` then zeros to 448 mod 512 + 64-bit length.

## 13.2 Compare tables (draw these in exam)
**RSA vs DH vs ECC**
| | RSA | DH | ECC (ECDH/ECDSA) |
|---|---|---|---|
| Hard problem | Factoring | DLP/DHP in Zp\* | ECDLP |
| Does | Encrypt, sign, key transport | Key agreement only | Key agreement, signatures, (ECIES) |
| Key size (128-bit sec.) | 3072 | 3072 | 256 |
| Main attack | GNFS factoring, timing, CCA | Index-calculus, MITM | Pollard-ρ (generic only) |

**Encryption vs Signature roles**
| | Uses sender's… | Uses receiver's… |
|---|---|---|
| Confidential message | – | **Public** (to encrypt), **Private** (to decrypt) |
| Signed message | **Private** (to sign) | – (any verifier uses sender's **Public**) |

## 13.3 Practice problems (with answers ✅)

**P1 (RSA):** p=7, q=17, e=5, M=19. Find n, φ, d, C, and decrypt using CRT.
✅ n=119, φ=96, **d=77** (5·77=385=4·96+1), **C=19⁵ mod 119 = 66**. CRT: dP=77 mod 6=**5**, dQ=77 mod 16=**13**; Mp=66⁵ mod 7=**5**; Mq=66¹³ mod 17=**2**; x≡5 (mod 7), x≡2 (mod 17) → **19** ✔

**P2 (RSA keys):** p=61, q=53, e=17. Find d and encrypt 65. ✅ d=**2753**, C=**2790**.

**P3 (DH):** p=19, g=2, a=5, b=7. ✅ A=2⁵=**13**, B=2⁷ mod 19=**14**, K=14⁵ mod 19 = 13⁷ mod 19 = **10**.

**P4 (Miller–Rabin):** Test n=133 with a=2. ✅ 132=2²·33; 2³³≡50; 50²≡106; never 1/132 → **composite** (7·19).
Test n=97, a=2 ✅ passes (8→64→22→96).

**P5 (CRT):** x≡2 (3), 3 (5), 2 (7) → **23**. x≡8 (9), 3 (20) → **143**. x≡5 (3), 2 (5), 1 (11) → **122**.

**P6 (DLP):** 3ˣ ≡ 13 (mod 17). ✅ x = **4**.

**P7 (Square-and-multiply):** 7¹³ mod 11 → **2**. 13²⁷ mod 55 → **7**.

**P8 (Inverse):** 11⁻¹ mod 26 → **19**. 7⁻¹ mod 40 → **23**. 3⁻¹ mod 11 → **4**.

**P9 (RSA signature):** p=5, q=11, e=3, d=27, h=9. ✅ s = 9²⁷ mod 55 = **4**; verify 4³ mod 55 = 9 ✔

**P10 (ECC):** On y² = x³+2x+2 mod 17 with P=(5,1), find 2P. ✅ **(6,3)**; and 3P = (10,6).

**P11 (Key count):** How many symmetric keys for 500 users? ✅ 500·499/2 = **124,750**; PKC: **500 key pairs**.

**P12 (Hash padding):** How many 512-bit blocks does a 1000-bit message take in SHA-1/MD5? ✅ **3**.

**P13 (MITM numeric):** n=11, g=7; Alice x=3, Bob y=9; Eve uses 8 toward Bob and 6 toward Alice. Keys? ✅ Alice–Eve **9**, Eve–Bob **5**.

**P14 (Textbook RSA flaw):** n=55, e=3, M=2. Why is it breakable? ✅ C=8 = 2³ with no reduction; cube root gives M. Also deterministic.

**P15 (Short-answer):** Why does CRT *not* change RSA's output? ✅ It reconstructs the unique solution mod pq of M≡Mp (mod p), M≡Mq (mod q), which is exactly C^d mod n.

## 13.4 Last-minute checklist ✅
- [ ] n(n−1)/2 vs n; hybrid idea; PKC ~1000× slower
- [ ] Trapdoor OWF def; 3 hard problems
- [ ] Ext. Euclid inverse; Euler φ; Fermat/Euler
- [ ] Square-and-multiply by hand
- [ ] MR steps + (¼)^k + 561 Carmichael
- [ ] RSA keygen/enc/dec by hand; design rules (|p−q|, safe primes, d>n^¼); attacks (factoring, timing, CCA)
- [ ] CRT table method; RSA-CRT dP,dQ; why 4× faster; fault-attack trade-off
- [ ] 3 textbook-RSA weaknesses; OAEP formulas P₁,P₂ (+ MGF1 version)
- [ ] DH algorithm, numeric, **MITM**, fix = authentication/certs
- [ ] DLP attacks: index calculus / BSGS+ρ / Pohlig–Hellman
- [ ] ECC: curve eq., ECDLP, double-and-add (26P), ECDSA nonce issue, ECDH MITM, 256↔3072
- [ ] Signatures: sign hash w/ private key; services; RSA sig; DSA vs RSA; certificates & CA & 7 steps
- [ ] Hash props; MD scheme; MD5 (128/512/64 steps); SHA-1 (160/512/80 rounds); padding 448 mod 512; why broken; HMAC/length-extension

Good luck bro 🍀 you got this 🔥
