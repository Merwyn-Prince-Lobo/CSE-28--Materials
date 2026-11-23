# Applied Cryptography – Units 3 & 4: Complete Exam Notes (MCQ Prep)

_Built from your lecture PDFs (Unit 3: 432 slides, Unit 4: 233 slides) and the syllabus you gave, in syllabus order. Where slides were thin I supplemented (marked_ **_[+]_** _). Where a slide contains an error I flag it (marked_ **_[Slide correction]_** _)._

**Legend:** 🔑 = key fact to memorise | ⚠ = MCQ trap | **[+]** = supplemented beyond slides

#### **Contents**

- **UNIT 3:** 3.1 Public-key paradigm → 3.2 Math toolkit II (primes/Miller–Rabin, CRT, DLP, GF(2^n)) → 3.3 RSA & OAEP → 3.4 Diffie–Hellman & MITM → 3.5 ECC → 3.6 Digital signatures & PKI → 3.7 Cryptographic hashes → Unit 3 definitions, comparison tables, traps, 20+ MCQs

- **UNIT 4:** 4.1 Entity authentication (passwords, challenge–response, ZKP) → 4.2 Homomorphic encryption, HMAC, X.509 → 4.3 Hardware security & physical attacker → 4.4 Side-channel attacks (timing, power, DPA) → 4.5 TPM & PUF → 4.6 Post-quantum crypto (lattices, Kyber/ML-KEM) → definitions, tables, traps, 20+ MCQs

- **Revision sheet → 40-mark mock test → answer key (at the very end) → syllabus coverage checklist**

# UNIT 3: ASYMMETRIC CRYPTOGRAPHY & DATA INTEGRITY

## 3.1 The Public-Key Paradigm: Symmetric vs Asymmetric Tradeoffs

3.1.1 The two questions cryptography must answer

1. **Confidentiality** – who can read the message?

2. **Authentication / integrity / non-repudiation** – who sent it, was it altered, can the sender deny it?

AES/DES (symmetric) answer the first one very well, but struggle with the second and with _how two strangers get a shared key_ .

### 3.1.2 Problems of symmetric cryptography

|Problem|Explanation|
|---|---|
|**Key distribution**|Both parties must already share a secret key.<br>Sending the key over an insecure channel defeats<br>the purpose (chicken-and-egg). Needs a secure<br>channel, courier, or a trusted KDC.|
|**Key scalability**|In a network of_n_users where every pair needs its<br>own key:**n(n−1)/2 keys**.|
|**No non-repudiation**|Both Alice and Bob hold the same key, so either<br>could have produced a message; a third party<br>cannot decide who did.|
|**Key storage**|Each user stores n−1 keys securely.|


#### **Key growth table (** 🔑 **memorise formula):**

|Users n|Symmetric keys n(n−1)/2|Public-key system (key pairs)|
|---|---|---|
|4|6|4 pairs (4 public + 4 private)|
|10|45|10 pairs|
|100|4,950|100 pairs|
|1,000|499,500|1,000 pairs|


⚠ **Trap – 'only two keys':** In public-key crypto, **each user** has two keys (public + private). The _system_ does not have only two keys. Total key pairs = n (2n keys). Symmetric growth is **quadratic** ; public-key growth is **linear** .

### 3.1.3 Idea of public-key (asymmetric) cryptography

- Each user has a **key pair** : **public key (PU)** – freely published; **private key (PR)** – kept secret, never shared.

- Keys are **mathematically related** , but deriving PR from PU must be computationally infeasible.

- **Encryption for confidentiality:** sender encrypts with the **receiver's public key** ; only the receiver's private key decrypts.

- **Digital signature (authentication/non-repudiation):** sender **signs with own private key** ; anyone verifies with the **sender's public key** .

- ⚠ **Rule:** _Encrypt with receiver's PUBLIC; sign with sender's PRIVATE._ Never 'encrypt with your own private key to keep secret' (anyone can undo it using your public key).

**History (slides):** Public-key idea published by **Diffie & Hellman (1976, 'New Directions in Cryptography')** ; **RSA (1977/78) by Rivest, Shamir, Adleman** ; **Ralph Merkle** independently proposed public-key distribution ideas (Merkle puzzles); **[+]** GCHQ's Ellis, Cocks and Williamson discovered equivalent ideas earlier (1970s) but kept them classified.

### 3.1.4 Three uses (security mechanisms) of public-key crypto

|Mechanism|How|Algorithms|
|---|---|---|
|Encryption / decryption|Encrypt with receiver's<br>PU|RSA, ElGamal, ECIES|
|Digital signature|Sign with sender's PR|RSA, DSA, ECDSA|
|Key exchange /|Derive shared secret|Diffie–Hellman, ECDH, RSA key|
|agreement||transport|


⚠ **DH = key exchange only** (not encryption, not signature). **DSA = signature only. RSA = all three.**

### 3.1.5 Requirements for a public-key scheme (Diffie–Hellman requirements)

1. Easy to generate a key pair (PU, PR).

2. Easy for sender to encrypt: C = E(PU, M).

3. Easy for receiver to decrypt: M = D(PR, C).

4. Infeasible for an adversary to find PR from PU.

5. Infeasible to recover M from C and PU.

6. (Optional) Either key can be used for encryption and the other for decryption (needed for signatures; true for RSA).

**All of this needs a trapdoor one-way function.**

### 3.1.6 One-way and trapdoor one-way functions

- **One-way function (OWF):** f(x) is **easy** to compute for all x, but for essentially all y it is **computationally infeasible** to find x with f(x)=y.

- **Trapdoor one-way function (TOWF):** an OWF that becomes **easy to invert if you know secret trapdoor information T** . Formally: Y=f_k(X) easy if k and X known; X=f_k⁻¹(Y) easy if k and Y known; **infeasible if Y known but k not** .

- **Strong OWF:** hard to invert on all but a **negligible fraction** of inputs. **Weak OWF:** easy to invert on a **non-negligible fraction** of inputs.

- OWFs are the foundation of modern cryptography: from a OWF you can build PRG, PRF, PRP → secure encryption and MACs.

|Function|Easy<br>direction|Hard direction|Trapdoor?|
|---|---|---|---|
|RSA|x → x^e mod<br>n|e-th root mod n (needs<br>factoring n)|**Yes**– d (or p, q)|
|Modular exponentiation<br>(DH/DSA)|g^x mod p|Discrete log|No (one-way)|
|EC scalar multiplication|k·P|ECDLP|No (one-way)|
|Cryptographic hash|H(M)|Pre-image|No (one-way<br>only)|
|Multiplying primes|p·q|Factoring n|–|


⚠ **Trap:** A hash is one-way but **not trapdoor** (nothing makes inversion easy). RSA is **trapdoor** one-way.

### 3.1.7 Number-theory review used throughout (from slides)

- **Euler totient φ(n):** count of integers 1..n coprime to n. φ(p)=p−1 (p prime); **φ(pq)= (p−1)(q−1)** ; φ(p^k)=p^k − p^(k−1).

- **Euler's theorem:** if gcd(a,n)=1 then **a^φ(n) ≡ 1 (mod n)** . Corollary: a^(φ(n)−1) is the multiplicative inverse of a mod n; if x ≡ y (mod φ(n)) then a^x ≡ a^y (mod n) – _this is why RSA decryption works_ .

- **Fermat's little theorem:** p prime, p∤a ⇒ **a^(p−1) ≡ 1 (mod p)** . Applications: primality testing, RSA.

#### **Extended Euclidean algorithm** gives modular inverses (e.g., d = e⁻¹ mod φ(n)).

### 3.1.8 Why is public-key crypto slower? (the trade-off)

- Public-key uses **huge-integer modular arithmetic** (1024–4096-bit numbers; modular exponentiation) – costly. AES uses simple byte substitutions, XORs, shifts and has **hardware support (AES-NI)** .

- Public-key operations are typically **hundreds to thousands of times slower** than AES for bulk data.

- RSA plaintext must be **< n** (cannot directly encrypt big files); ciphertext has the size of n.

- Public-key needs **authentication of public keys** (PKI) – otherwise MITM.

### 3.1.9 Comparison: Symmetric vs Asymmetric ( 🔑 high-yield)

|Feature|Symmetric (AES, DES)|Asymmetric (RSA, ECC, DH)|
|---|---|---|
|Keys|One**shared secret**key|**Key pair**: public + private|
|Speed|**Very fast**|**Slow**(100–1000× or more)|
|Key length for 128-bit<br>security|128 bits|RSA 3072 bits / ECC 256 bits|
|Key distribution|**Hard**– needs secure channel|**Easy**– publish public key<br>(needs authenticity)|
|Number of keys for n<br>users|n(n−1)/2|n pairs (2n)|
|Non-repudiation|**No**|**Yes**(digital signatures)|
|Best for|Bulk data encryption|Key establishment, signatures,<br>small data|
|Security basis|Confusion/diffusion, brute-force<br>resistance|Hard math: factoring, DLP,<br>ECDLP|
|Quantum threat|Grover: halves effective key (use<br>AES-256)|**Shor: breaks RSA/ECC**<br>**completely**|


#### **Key-size equivalence (NIST) [+]:**

|Security (bits)|Symmetric|RSA/DH modulus|ECC key|
|---|---|---|---|
|80|–|1024|160|
|112|3DES|2048|224|
|128|AES-128|3072|256|
|192|AES-192|7680|384|
|256|AES-256|15360|512|


### 3.1.10 Hybrid cryptography – 'Protect the key, not the file'

Since each type is strong where the other is weak, real systems combine them:

1. Sender generates a random **session key K** (e.g., AES-256).

2. Data is encrypted with **AES using K** (fast).

3. K is encrypted with the receiver's **public key** (RSA-OAEP) – or K is derived by **ECDH/DH key agreement** .

4. Receiver decrypts K with private key, then decrypts the data with AES.

   - **Real-world refinement:** modern protocols (TLS 1.3) prefer **ephemeral key agreement (ECDHE)** over RSA key transport → **forward secrecy** .

   - **Where you see it:** HTTPS/TLS, PGP/GPG, S/MIME, SSH, VPNs, messaging apps. Example: 1 GB file → AES encrypts 1 GB; RSA encrypts only 32 bytes (the key).

#### **Slide clarifications (common misconceptions):**

- 'AES has a key problem so AES is defective' → ⚠ **Wrong.** AES (the algorithm) is still considered secure; the **problem is key distribution/management** , not AES.

- **AES algorithm vs implementation:** secure algorithm can leak via a bad implementation (side channels, Unit 4).

Public-key does **not** replace symmetric; it **complements** it.

## 3.2 Mathematical Toolkit II

### 3.2.1 Prime number generation

**Why large primes?** RSA needs two secret large primes p,q (each ≥ 1024 bits for a 2048-bit modulus); DH/DSA need a large prime p (and subgroup prime q). Security rests on the difficulty of factoring/DLP with large primes.

**Prime Number Theorem [+]:** number of primes ≤ N ≈ N/ln N, so a random n-bit number is prime with probability ≈ 1/ln(2^n) ≈ **1/(0.693·n)** . For 1024 bits ≈ 1/710 (≈ 1/355 if only odd candidates). So primes are **plentiful** – just generate random candidates and test.

#### **Prime generation process (slide algorithm):**

1. Generate a **random number with required bit length** .

2. **Set the highest bit** to 1 (ensures exact size).

3. **Set the lowest bit** to 1 (make it **odd** ).

4. **Small-prime trial division** (3, 5, 7, 11…) to discard obvious composites quickly.

5. Apply **Miller–Rabin** (several rounds).

6. If composite → generate a new candidate.

7. If passes → accept as **probable prime** .

#### **Slide example (8-bit prime, range 128–255):**

- Attempt 1: random 01100110=102 → set top bit → 11100110=230 → make odd → 11100111= **231** . Digit sum 2+3+1=6 ⇒ divisible by 3 ⇒ composite, discard.

- Attempt 2: 11011110=222 → odd → 11011111= **223** . Not divisible by 3,5,7,11. Miller–Rabin: 222=2¹×111 (s=1, d=111), base a=2 → passes. Accept **223** .

- **[Slide correction]** The slide says 2^111 mod 223 = 222 (= −1). The correct value is **1** (since 223 ≡ 7 mod 8, 2 is a quadratic residue, so 2^111 ≡ +1). The conclusion is unchanged – x = 1 also passes the Miller–Rabin first check – and 223 is indeed prime.

### 3.2.2 Miller–Rabin primality test ( 🔑 very exam-likely)

A **probabilistic** test based on the fact that if n is an odd prime, the only square roots of 1 mod n are **+1 and −1** .

**Setup:** For odd n>2 write **n − 1 = 2^s · d** with **d odd** . (e.g., 29−1=28=2²×7 → s=2, d=7.)

#### **One round with random base a, 2 ≤ a ≤ n−2:**

1. Compute **x = a^d mod n** .

2. If **x = 1 or x = n−1** → n **passes** this round (probably prime).

3. Otherwise repeat up to **s−1** times: x = x² mod n. If x becomes **n−1** → passes.

4. If x never becomes n−1 → **n is definitely composite** (a is a _witness_ ).

**Outcomes:** 'Composite' = **certainly composite** . 'Probably prime' = passed all rounds, tiny chance of error.

**Error probability:** For a composite n, at most **1/4** of bases are 'strong liars'. After k independent rounds: **error ≤ (1/4)^k** . k=5 → 1/1024; k=10 → 1/1,048,576. (Practical libraries use e.g. 40 rounds or fewer for random candidates; error is far smaller than the bound.)

#### **Slide worked examples (base a=2):**

|n|n−1 =<br>2^s·d|x=a^d mod n|Squarings|Result|
|---|---|---|---|---|
|**29**|28 = 2²·7|2⁷ mod 29 = 128<br>mod 29 =**12**|12² mod 29 = 144 mod<br>29 =**28 = n−1**|Passes → probably prime|
|**53**|52 =|2¹³ mod 53 = 8192|30² mod 53 = 900 mod|Passes → probably prime|
||2²·13|mod 53 =**30**|53 =**52 = n−1**|(indeed prime)|
|**561**|560 =|2³⁵ mod 561 =**263**|263²→**166**, 166²→**67**,|Never hits 560 →|
||2⁴·35||67²→**1**|**composite**(561 = 3×11×17)|


🔑 **561 is the smallest Carmichael number** – it fools the plain **Fermat test** (a^560 ≡ 1 mod 561 for all a coprime to 561) but **Miller–Rabin catches it** . Note the chain ended in 1 without passing through −1: that exposes a non-trivial square root of 1.

**Terminology:** _Witness_ = base that proves compositeness. _Strong liar_ = base for which a composite passes. _Strong pseudoprime to base a_ = composite passing MR for base a. **Other facts [+]:** MR cost ≈ one modular exponentiation per round (O(k·log³ n)). With fixed small base sets it can be **deterministic** for bounded n (e.g., bases 2,3,5,7 are enough for n < 3,215,031,751). The **AKS test (2002)** is deterministic polynomial-time but too slow in practice.

#### **Comparison of prime generation/testing methods (slides):**

|Method|Type|Speed on large<br>numbers|Weakness / use|
|---|---|---|---|
|**Trial division**|Deterministic|Very slow (up to √n)|Small numbers, learning;<br>used as pre-filter|
|**Sieve of**<br>**Eratosthenes**|Deterministic|Efficient for**all**<br>**primes up to a limit**|Not for one huge prime|
|**Fermat test**|Probabilistic|Fast|Fooled by**Carmichael**<br>**numbers**|
|**Solovay–**<br>**Strassen**|Probabilistic<br>(Jacobi symbol)|Fast|Weaker bound (≤1/2 per<br>round); historical|
|**Miller–Rabin**|Probabilistic|Fast; hundreds–<br>thousands of bits|≤1/4 error per round;<br>**industry standard**|


**Advantages of MR:** fast, uses efficient modular exponentiation, works on 1000s of bits, error shrinks exponentially with rounds. **Limitation:** only gives _probable_ prime. **Applications:** RSA key generation, DH parameters, DSA/ElGamal parameters, ECC field primes, any large-prime generation.

⚠ **Traps:** (a) 'Composite' from MR is **certain** ; 'prime' is **probable** . (b) Base range is **2 ≤ a ≤ n−2** . (c) Error bound is **(1/4)^k** , not (1/2)^k (that is Solovay–Strassen). (d) Fermat test can be fooled by Carmichael numbers; MR cannot be fooled by _all_ bases.

### 3.2.3 Chinese Remainder Theorem (CRT)

**Statement:** If m₁, m₂, …, m_k are **pairwise coprime** moduli, then the system x ≡ a₁ (mod m₁), x ≡ a₂ (mod m₂), …, x ≡ a_k (mod m_k) has a **unique solution modulo M = m₁·m₂·… ·m_k** .

**Formula (** 🔑 **):** x = Σ aᵢ · Mᵢ · yᵢ (mod M), where **Mᵢ = M/mᵢ** and **yᵢ = Mᵢ⁻¹ mod mᵢ** .

**Worked example:** x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7). M=105. M₁=35, M₂=21, M₃=15. y₁=35⁻¹ mod 3 = 2⁻¹ = 2; y₂=21⁻¹ mod 5 = 1⁻¹ = 1; y₃=15⁻¹ mod 7 = 1⁻¹ = 1. x = 2·35·2 + 3·21·1 + 2·15·1 = 140+63+30 = 233 ≡ 233−210 = **23 (mod 105)** . Check: 23 mod 3=2 ✓, mod 5=3 ✓, mod 7=2 ✓.

**Two-modulus (Garner) form used in RSA [+]:** x = x_q + q·[(q⁻¹ mod p)·(x_p − x_q) mod p].

**Applications in cryptography:** speeding up **RSA decryption/signing (RSA-CRT)** ; handling large numbers via residues; secret sharing; fault-attack awareness (a single fault in RSACRT can leak a factor – Bellcore attack **[+]** ).

**RSA-CRT summary (slides, covered fully in 3.3.6).** Instead of one exponentiation mod n (2048-bit), do two exponentiations mod p and mod q (1024-bit each) → about **4× faster** . ⚠ **Trap:** CRT requires **pairwise coprime** moduli. Solution is unique **mod M (product)** , not mod any single mᵢ.

### 3.2.4 Discrete Logarithm Problem (DLP)

**Definition (** 🔑 **):** Given a prime p, a generator (primitive root) g of Zp*, and y = g^x mod p, **find x** . x = log_g y (mod p). Computing g^x is **easy** (square-and-multiply); recovering x is **hard** (for large p).

**Easy example:** 3^x ≡ 13 (mod 17) → try x=1..: 3,9,10,13 → **x = 4** . For tiny numbers brute force works; for 2048-bit p it is infeasible.

**Where used:** Diffie–Hellman (Zp*), ElGamal, DSA, Schnorr signatures; the **elliptic-curve version (ECDLP)** : given P and Q=kP find k.

**Generalization:** DLP can be defined in any cyclic group G: given g and h=g^x find x. Hardness depends on the group. **ECDLP** is harder than DLP in Zp* for the same size – no sub-exponential attack known.

#### **Attacks on DLP:**

|Attack|Type|Complexity|Notes|
|---|---|---|---|
|**Brute force**|Generic|O(|G|
|**Baby-step**<br>**giant-step**<br>**(BSGS)**|Generic|O(√|G|
|**Pollard's rho**|Generic|O(√|G|
|**Pohlig–**<br>**Hellman**|Generic (uses<br>factorisation of group<br>order)|Reduces DLP to the<br>**largest prime factor**of<br>the order|⇒choose group of<br>**prime order**(or order<br>with a large prime<br>factor)|


|Attack|Type|Complexity|Notes|
|---|---|---|---|
|**Index-calculus**|**Non-generic**, sub-<br>exponential|Much faster than<br>generic|Works in**Zp***and<br>**GF(2^m)***—**not**on<br>well-chosen elliptic<br>curves; 'most powerful<br>attack against DLP in<br>Zp*'|


🔑 This is why ECC gets away with **smaller keys** : no index-calculus-type attack exists; best attacks are generic √n (so 256-bit curve ≈ 128-bit security).

⚠ **Traps:** Pohlig–Hellman is **not** a defence – it is an attack that is defeated by using a prime-order subgroup. Index-calculus is **not generic** ; it exploits integer structure and does not apply to ECC. DLP and factoring are **different problems** (both broken by Shor's algorithm).

### 3.2.5 Finite fields GF(2^n) **[+]** (slides do not cover this; AES uses it)

**Field:** a set with +, −, ×, ÷ (except ÷0) obeying usual laws. Finite fields exist only with **p^n elements** (p prime). **GF(p)** = integers mod prime p. **GF(2^n)** = field with **2^n elements** .

- **Elements of GF(2^n)** are **polynomials of degree ≤ n−1 with coefficients in GF(2) = {0,1}** – i.e., n-bit strings. Example GF(2³): x²+x+1 ↔ 111.

- ⚠ **Z_(2^n) (integers mod 2^n) is NOT a field** (even numbers have no inverse). GF(2^n) is built using polynomial arithmetic.

- **Addition = subtraction = bitwise XOR** (coefficients mod 2). Example: (x²+x+1)+(x+1) = x² → 111 ⊕ 011 = 100.

- **Multiplication:** multiply polynomials, then **reduce modulo an irreducible polynomial P(x)** of degree n.

- **GF(2³)** with P(x)=x³+x+1: (x²+x+1)(x+1) = x³+1 → x³ = x+1 → (x+1)+1 = **x** (010).

- **GF(2⁸) in AES:** P(x)= **x⁸+x⁴+x³+x+1 (0x11B)** . Example from FIPS-197: {57}·{83} = **{C1}** . **xtime** (multiply by x = 02): shift left 1; if the shifted-out bit was 1, XOR with 0x1B.

- **Inverses:** found by the **extended Euclidean algorithm for polynomials** (AES S-box uses the inverse in GF(2⁸)).

- **Why GF(2^n) in cryptography:** efficient in hardware (XOR/shift), used in **AES (SubBytes, MixColumns), GCM (GF(2¹²⁸)), binary-field elliptic curves** , errorcorrecting codes. DLP can also be posed in GF(2^m)*, where index-calculus applies.

|Feature|GF(p)|GF(2^n)|
|---|---|---|
|Elements|0..p−1|n-bit polynomials|
|Addition|mod p|**XOR**|
|Multiplication|mod p|polynomial mult. mod irreducible P(x)|
|Characteristic|p|2|
|Typical use|RSA/DH/ECC prime curves|AES, GCM, binary EC curves|


⚠ **Traps:** the polynomial must be **irreducible** ; AES's is x⁸+x⁴+x³+x+1 (not x⁸+x⁴+x³+x²+1); in GF(2^n), **x + x = 0** so every element is its own additive inverse.

## 3.3 Public-Key Algorithm 1: RSA (Key Generation, Encryption, OAEP)

### 3.3.1 Facts about RSA

- Invented by **Rivest, Shamir, Adleman (MIT, 1977; published 1978)** . Turing Award 2002. Security based on the **integer factorisation problem** (given n=pq, find p,q). It is a **trapdoor one-way function** : x → x^e mod n is easy; inverting needs the trapdoor d (equivalently p,q).

- Supports **encryption, digital signatures and key transport** . Block cipher style: plaintext is an integer **M with 0 ≤ M < n** .

- Most widely deployed public-key algorithm; typical modulus **2048 bits** (3072 for 128bit security).

### 3.3.2 Key generation ( 🔑 learn the 5 steps)

1. Choose two **large distinct primes p and q** (Miller–Rabin; each ≈ half the modulus size, e.g., 1024 bits each).

2. Compute **n = p·q** (the modulus).

3. Compute **φ(n) = (p−1)(q−1)** .

4. Choose **public exponent e** with **1 < e < φ(n)** and **gcd(e, φ(n)) = 1** . Common: **e = 65537 = 2¹⁶+1** (also 3, 17 – small e are risky without padding).

5. Compute **private exponent d = e⁻¹ mod φ(n)** (extended Euclid), i.e., **e·d ≡ 1 (mod φ(n))** .

**Public key = (n, e). Private key = d** (with p, q, φ(n) kept secret / destroyed).

### 3.3.3 Encryption and decryption

- **Encrypt:** C = M^e mod n (anyone with the public key)

- **Decrypt:** M = C^d mod n (only owner of d)

- **Why it works:** e·d = 1 + k·φ(n) ⇒ C^d = M^(ed) = M·(M^φ(n))^k ≡ M·1 = M (mod n) by **Euler's theorem** (also true when gcd(M,n)≠1, via CRT).

- RSA is symmetric in roles: (M^e)^d = (M^d)^e ⇒ sign with d, verify with e.

**Worked example (p=11, q=17):** n=187, φ=10·16=160, choose e=7 (gcd(7,160)=1), d=7⁻¹ mod 160 = **23** (7·23=161=1+160 ✓). Public (187,7), private 23.

- Encrypt M=88: C = 88⁷ mod 187 = **11** .

- Decrypt: 11²³ mod 187 = **88** ✓.

### 3.3.4 Fast exponentiation – square-and-multiply

Computing M^e with e of 2048 bits directly is impossible; we reduce mod n at every step.

- **Left-to-right method:** write exponent in binary; start with r=M for the leading 1; for each next bit: **square** r (r = r² mod n) and, **if the bit is 1, multiply** by M.

- **Example 88⁷ mod 187** , 7 = 111₂: r=88 → bit 1: 88²=7744→77, ×88=6776→ **44** → bit 1: 44²=1936→66, ×88=5808→ **11** ✓.

- **Cost:** about **log₂ e squarings + (number of 1-bits − 1) multiplications** . For a k-bit exponent: k squarings + ~k/2 multiplications on average.

- 🔑 **e = 65537 (binary 1 0000 0000 0000 0001)** needs only **16 squarings + 1 multiplication** → **encryption/verification is fast; decryption/signing (d ≈ 2048 bits) is slow** .

- ⚠ If the multiply step is done only for 1-bits, the running time/power depends on the key → **timing/SPA leakage** (see Unit 4); countermeasure: **square-and-multiply-always / Montgomery ladder / constant-time** .

### 3.3.5 Designing a good RSA system (slide topics)

|Choice|Requirement|Reason|
|---|---|---|
|p, q size|Both large,**similar bit length**(n/2<br>each)|Prevents easy factoring of<br>unbalanced n|
|Relation p vs<br>q|**Not close together**(|p−q|
|p−1, q−1|Have a**large prime factor**;<br>gcd(p−1,q−1) small|**Pollard p−1**attack|
|p, q<br>generation|**Random**, independent, from a good<br>RNG;**never reuse a prime across**<br>**keys**|Shared prime⇒gcd(n₁,n₂) reveals it|
|e|gcd(e,φ)=1; use**65537**|Small e + no padding = attacks; large<br>random e slows encryption|
|d|**Large**(≥ n^0.3)|**Wiener**(d<n^0.25) /**Boneh–Durfee**<br>(d<n^0.292) attacks|
|Modulus size|**≥ 2048 bits**today|1024-bit considered unsafe|


**How important is factoring?** Breaking RSA (recovering d) is **equivalent** to factoring n; whether decrypting a single ciphertext without factoring is as hard (the 'RSA problem') is unproven but believed hard.

### 3.3.6 Speeding up decryption with CRT (RSA-CRT) ( 🔑 slide-heavy topic)

The private-key holder knows p, q, so work mod p and mod q separately and recombine with the **Chinese Remainder Theorem** .

**Precompute:** d_p = d mod (p−1), d_q = d mod (q−1), q_inv = q⁻¹ mod p. **Decrypt C:**

1. m_p = C^(d_p) mod p

2. m_q = C^(d_q) mod q

3. h = q_inv·(m_p − m_q) mod p

4. **M = m_q + h·q**

**Example (n=187, p=17, q=11, d=23, C=11):** d_p=23 mod 16= **7** , d_q=23 mod 10= **3** , q_inv=11⁻¹ mod 17= **14** . m_p=11⁷ mod 17= **3** ; m_q=11³ mod 11= **0** ; h=14·(3−0) mod 17=42 mod 17= **8** ; M=0+8·11= **88** ✓ (same as direct).

**Why faster?** Modular exponentiation cost grows roughly as (bit-length)³. Two exponentiations with **half-size modulus and half-size exponent** : 2·(1/2)³ = **1/4 the work → about 4× faster** (with square-and-multiply; the slide's 'CRT does TWO exponentiations – does that cancel the gain?' → **No** , because each is far cheaper). Gain depends on multiplication algorithm (less than 4× with Karatsuba/FFT) – 'improvement, but not magic'.

- Only **private-key operations** benefit (needs p and q). Public-key operation (e small) gains nothing.

- ⚠ RSA-CRT is vulnerable to **fault attacks** (one faulty half reveals a factor via gcd).

### 3.3.7 RSA security and attacks

|Attack|Idea|Defence|
|---|---|---|
|**Factoring**|Factor n using**General Number**<br>**Field Sieve (GNFS)**– sub-<br>exponential. Records: RSA-768<br>(2009), RSA-829/RSA-250 (2020)|≥ 2048-bit n|
|**Brute-force d**|Try all d|Large d|
|**Timing attack**(Kocher<br>1996)|Measure time of decryption to infer<br>bits of d|Constant-time code,<br>**blinding**|
|**Chosen-ciphertext**<br>**attack**|Use malleability: ask decrypt of<br>C·r^e to get M·r|OAEP, validate padding,<br>blinding|
|**Bleichenbacher (1998)**|Padding-oracle on**PKCS#1 v1.5**|Use OAEP, uniform error<br>handling|
|**Short-message / small-e**<br>**attack**|If M^e < n no modular reduction⇒<br>take ordinary e-th root (e.g., e=3)|Pad (OAEP)|
|**Håstad broadcast**|Same M, e=3, three different n⇒<br>CRT then cube root|Random padding|
|**Common modulus**|Two users share n with different e|Never share n|
|**Dictionary /**<br>**deterministic**|Textbook RSA is deterministic: same<br>M⇒same C; attacker encrypts<br>guesses|Randomised padding|


#### **Textbook RSA weaknesses (slides):**

1. **Deterministic** – identical plaintexts give identical ciphertexts (not semantically secure).

2. **Small-message / low-exponent attack.**

3. **Multiplicative (homomorphic) structure:** E(M₁)·E(M₂) mod n = E(M₁·M₂) ⇒ **malleable** , enables chosen-ciphertext tricks.

3.3.8 OAEP – Optimal Asymmetric Encryption Padding ( 🔑 diagram + formulas)

**Purpose:** convert deterministic textbook RSA into a **randomised, CCA-secure** scheme ( **IND-CCA2 in the random-oracle model** , Bellare–Rogaway 1994).

#### **Notation (as in slides):**

- **M** = padded message of **m bits** ; **r** = fresh one-time random value of **k bits** (k < m)

- **G** : {0,1}^k → {0,1}^m – public **mask generation function** (expands r)

- **H** : {0,1}^m → {0,1}^k – public **hash function** (compresses)

- ⊕ = XOR, ∥ = concatenation; the integer encoded by P must be **< n**

#### **Encryption:**

1. Pad message to M (m bits).

2. Pick fresh random **r** (k bits).

3. **P₁ = M ⊕ G(r)** (masked message, m bits)

4. **P₂ = H(P₁) ⊕ r** (recovery information, k bits)

5. **P = P₁ ∥ P₂** (m+k bits)

6. **C = P^e mod n**

#### **Decryption:**

1. **P = C^d mod n** ; split into P₁ (m bits) and P₂ (k bits).

2. **r = H(P₁) ⊕ P₂**

3. **M = P₁ ⊕ G(r)**

4. Remove padding → message. (Works because **(A⊕B)⊕B = A** .)

`M (m bits) ----------⊕--------------> P1 = M ⊕ G(r) ----+----→ [ P1` ∥ `P2 ] → RSA: C = P^e mod n ^                                   | r (k bits) --→ G(r)--+            +-- H(P1) -----------→⊕ ←-- r  → P2 = H(P1) ⊕ r`

OAEP is a **two-round Feistel-like** network: G expands r to mask the message, H compresses the masked message to mask r.

#### **Why OAEP works:**

- **Randomised** (fresh r every time) ⇒ same plaintext gives different ciphertexts → defeats dictionary/deterministic attack.

- **Short-message & low-e attacks fail:** P is a full-length pseudo-random string, so P^e always wraps mod n.

- **Destroys multiplicative structure** ⇒ blocks malleability.

- **All-or-nothing:** you need **all** of P to recover r, and r to recover M.

- **Standard version [+] (PKCS#1 v2.x, RFC 8017):** uses **MGF1** + hash (SHA-1/SHA-256) and a label hash; EM = 0x00 ∥ maskedSeed ∥ maskedDB. **Max message length = k − 2·hLen − 2 bytes** (2048-bit RSA + SHA-256 → 256−64−2 = **190 bytes** ).

- ⚠ Implementation must give **no distinguishable errors** (Manger's attack on OAEP).

#### **Textbook RSA vs RSA-OAEP:**

|Feature|Textbook RSA|RSA-OAEP|
|---|---|---|
|Randomised|**No**<br>(deterministic)|**Yes**|
|Semantic security|No|**Yes (IND-CPA/CCA2 in ROM)**|
|Malleable|**Yes**|No (CCA-secure)|
|Small-message<br>attack|Vulnerable|Protected|
|Ciphertext<br>expansion|none (|C|
|Use today|**Never**|Recommended RSA encryption mode (PKCS#1<br>v1.5 padding is legacy/risky)|


⚠ **Traps:** OAEP is **padding for encryption** (signatures use **PSS** , see 3.6). OAEP doesn't change RSA's key size or the factoring assumption. CRT = **speed** ; OAEP = **security** (slide 'what problem does each technique solve?'). Square-and-multiply = speed of exponentiation.

## 3.4 Diffie–Hellman Key Exchange & the Man-in-the-Middle Vulnerability

**Goal:** two parties who share nothing beforehand establish a **shared secret over an insecure channel** . (Whitfield Diffie & Martin Hellman, **1976** , first published public-key concept; Merkle credited.) Security basis: **Discrete Logarithm Problem / Computational Diffie–Hellman (CDH)** .

**Colour-mixing analogy (slides):** public common colour (yellow) → each adds a secret colour → swap mixtures → each adds own secret again → both get the same final colour; Eve can't un-mix paint.

### Protocol ( 🔑 )

**Public parameters:** a large prime **p** and a **generator g** of Zp* (g primitive root mod p).

1. Alice picks secret **a** , computes **A = g^a mod p** , sends A.

2. Bob picks secret **b** , computes **B = g^b mod p** , sends B.

3. Alice computes **K = B^a mod p** ; Bob computes **K = A^b mod p** .

4. Both equal **K = g^(ab) mod p** .

**Example (p=23, g=5):** a=6 → A=5⁶ mod 23= **8** ; b=15 → B=5¹⁵ mod 23= **19** . Alice: 19⁶ mod 23= **2** ; Bob: 8¹⁵ mod 23= **2** . Shared key **K=2** . Eve sees p,g,8,19 only.

**Why secure:** to get K Eve must compute a from A (DLP) or solve CDH (given g^a, g^b find g^ab). No efficient classical algorithm for large p (≥ 2048 bits).

**Properties:** needs **no prior shared secret** ; produces a key, **cannot encrypt or sign by itself** ; each run with fresh a,b ( **ephemeral DH = DHE** ) gives **perfect forward secrecy** ; both parties contribute to the key; **no authentication** .

### The Man-in-the-Middle (MITM) attack ( 🔑 favourite exam topic)

DH does **not authenticate** who is on the other end. An active attacker Mallory/Eve sits between Alice and Bob.

**Slide setup n=11 (this is p), g=7.** My numbers: Alice a=3 → A=7³ mod 11= **2** ; Bob b=6 → B=7⁶ mod 11= **4** ; Mallory m=4 → M=7⁴ mod 11= **3** .

1. Alice sends A=2 → Mallory intercepts, sends **M=3** to Bob.

2. Bob sends B=4 → Mallory intercepts, sends **M=3** to Alice.

3. **Alice–Mallory key K₁** = M^a = 3³ mod 11 = **5** (= A^m = 2⁴ mod 11 = 5).

4. **Bob–Mallory key K₂** = M^b = 3⁶ mod 11 = **3** (= B^m = 4⁴ mod 11 = 3).

5. Alice encrypts with K₁; Mallory **decrypts, reads/modifies, re-encrypts with K₂** and forwards. Alice and Bob think they share one key but each shares a **different** one with Mallory.

#### **Fixes (authenticated key exchange):**

- **Digital signatures + certificates** on the DH values (e.g., **TLS** : server signs its ephemeral DH/ECDH share; **STS protocol** ).

- **MAC with pre-shared key / password-authenticated key exchange (PAKE)** . **Out-of-band verification** of fingerprints (e.g., safety numbers in messengers). **PKI** to bind public keys to identities.

**Other DH issues [+]:** small-subgroup attacks (validate public values, use prime-order subgroup/safe primes); weak/standard 512/1024-bit groups ( **Logjam** ); parameters ≥ 2048bit or use ECDH.

|DH vs RSA|Diffie–Hellman|RSA|
|---|---|---|
|Purpose|**Key agreement only**|Encryption, signatures, key transport|
|Hard problem|DLP / CDH|Factoring|
|Who picks the key|**Both**contribute (derived)|Sender picks session key|
|Forward secrecy|**Yes**if ephemeral|No for RSA key transport (static)|
|Authentication|**None built in → MITM**|Signatures give authentication|


⚠ **Traps:** DH is **not** broken by eavesdropping, but is by **active** MITM. 'Authenticated DH' needs signatures/certificates – not just a bigger prime. K = g^(ab), **not** g^(a+b) or g^a·g^b. Private values a, b are **never transmitted** .

## 3.5 Elliptic Curve Cryptography (ECC): Point Addition, Doubling, ECDH, ECDSA

Proposed independently by **Neal Koblitz and Victor Miller (1985)** . Public-key cryptography whose hard problem is the **Elliptic Curve Discrete Logarithm Problem (ECDLP)** .

### 3.5.1 Elliptic curves

- **Weierstrass equation over a prime field GF(p): y² ≡ x³ + ax + b (mod p)** , with **4a³ + 27b² ≢ 0 (mod p)** (non-singular: no repeated roots).

- The set of points (x,y) satisfying the equation **plus a special point at infinity O** forms an **abelian group** under point addition. **O = identity element** (like 0).

- **Inverse:** −P = (x, −y) (reflection about the x-axis). P + (−P) = O.

- Curves can also be defined over **GF(2^m)** (binary curves) or in Montgomery/Edwards form ( **Curve25519** , Ed25519).

- Standard curves: **secp256r1 (NIST P-256)** , **secp256k1 (Bitcoin/Ethereum)** , **Curve25519 (X25519/Ed25519)** , P-384, P-521.

- **Domain parameters:** (p, a, b, **G** = base point, **n** = order of G, **h** = cofactor).

### 3.5.2 Point addition and doubling ( 🔑 formulas)

Geometric rule: draw the line through P and Q; it meets the curve at a third point; **reflect it over the x-axis** to get **P+Q** . For doubling use the **tangent** at P.

For P=(x₁,y₁), Q=(x₂,y₂) (all mod p):

|Case|Slopeλ|Result|
|---|---|---|
|**Addition**P ≠ Q|**λ = (y₂ − y₁)/(x₂ − x₁)**|x₃ =λ² − x₁ − x₂ ; y₃ =λ(x₁ − x₃) − y₁|
|**Doubling**P = Q|**λ = (3x₁² + a)/(2y₁)**|same x₃, y₃ formulas (with x₂=x₁)|
|Q = −P (x₁=x₂, y₁=−y₂)|vertical line|**P + Q = O**|
|y₁ = 0 in doubling|tangent vertical|**2P = O**|


(Division means multiplication by the **modular inverse** .) ⚠ The **'a' term appears only in doubling** ; **b never appears** in the formulas.

**Worked example (curve y² = x³ + 2x + 2 mod 17, G=(5,1)):**

- **Doubling 2G:** λ=(3·25+2)/(2·1)=77/2 mod 17 = 9·9 = 81 mod 17 = **13** . x₃=13²−5−5=159 mod 17= **6** ; y₃=13(5−6)−1=−14 mod 17= **3** ⇒ **2G=(6,3)** .

- **Addition G+2G:** λ=(3−1)/(6−5)=2; x₃=4−5−6=−7 mod 17= **10** ; y₃=2(5−10)−1=−11 mod 17= **6** ⇒ **3G=(10,6)** .

- This curve has **19 points (prime order)** with G generating all: 19G = O, 20G = G.

### 3.5.3 Scalar (point) multiplication – Double-and-Add

**kP = P + P + … + P (k times)** , computed in O(log k) steps, the ECC analogue of squareand-multiply: **doubling ↔ squaring, addition ↔ multiplication** .

- Scan bits of k from the MSB: for each next bit **double** ; if bit =1 **add P** .

- **Example 13G** (13=1101₂): start G → bit1: 2G, +G=3G → bit0: 6G → bit1: 12G, +G = **13G = (16,4)** .

- Same side-channel risk (key-dependent add) → use Montgomery ladder / constanttime.

### 3.5.4 ECDLP and ECC's trapdoor

**Easy:** given integer k and point P, compute **Q = kP** (double-and-add).

- **Hard (ECDLP):** given P and Q, find k. For large group order n (≈ 2^256) infeasible. **Key pair: private key d** (random integer 1 ≤ d < n); **public key Q = d·G** .

- **'Hopping' analogy (slides):** start at A, hop around the curve using the chord-tangent rule k times to reach E. Knowing A and k, E is easy; knowing only A and E, the number of hops is nearly impossible to find. **Public key = (start, end) points, private key = number of hops.**

- **Best attack:** generic **Pollard's rho ≈ √n** operations; **no index-calculus** (subexponential) attack known ⇒ a 256-bit curve gives **128-bit security** .

### 3.5.5 Security benefits of smaller key sizes ( 🔑 )

|Security level|RSA / DH|ECC|Ratio|
|---|---|---|---|
|80-bit|1024|160|6.4×|
|112-bit|2048|224|9×|
|**128-bit**|**3072**|**256**|**12×**|
|192-bit|7680|384|20×|
|256-bit|15360|512|30×|


**Benefits:** shorter keys & signatures; faster key generation, signing, ECDH; less memory, storage, bandwidth, power. **Applications (slides):** wireless/mobile devices, **smart cards** , IoT, web servers handling many TLS sessions, Bitcoin/Ethereum, SSH, Signal/WhatsApp

(X25519). **Limitations:** more complex maths and implementation traps (invalid curves, weak curves, nonce reuse), patents history, **quantum computers (Shor) break ECC as well as RSA** ; for 128-bit PQ security neither helps.

### 3.5.6 ECC algorithms

**ECDH (key agreement) (** 🔑 **):** both agree on curve & G. Alice picks d_A, sends Q_A = d_A·G; Bob picks d_B, sends Q_B = d_B·G. **Shared point S = d_A·Q_B = d_B·Q_A = (d_A·d_B)·G.** Use the **x-coordinate of S** (through a KDF) as the session key. Works in the **additive group of points** (vs DH in **multiplicative group Zp** *). Uses: **TLS handshakes (ECDHE)** , VPNs (IPsec/IKEv2, WireGuard uses X25519), Signal.

- **Weaknesses:** only **key agreement, no authentication ⇒ MITM unless combined with signatures/certificates** ; **invalid-curve attacks** if received public point isn't validated to lie on the curve.

**ECDSA (digital signature) (** 🔑 **):** ECC + hash (SHA-256); provides **authenticity, integrity, non-repudiation** . Curves e.g. secp256k1, P-256.

- **Keys:** d private, Q = dG public. h = Hash(m).

- **Sign:** (1) choose **fresh random nonce k** , 1≤k<n; (2) (x₁,y₁)=k·G; (3) **r = x₁ mod n** (if 0 redo); (4) **s = k⁻¹(h + d·r) mod n** (if 0 redo). **Signature = (r, s).**

- **Verify (with Q):** (1) check 1≤r,s<n; (2) **w = s⁻¹ mod n** ; (3) **u₁ = h·w, u₂ = r·w mod n** ; (4) **X = u₁G + u₂Q** ; (5) valid iff **x-coordinate of X ≡ r (mod n)** .

- **Why it works:** u₁G + u₂Q = (h + rd)s⁻¹G = kG.

- **Weaknesses (slides):** a **reused or predictable nonce k reveals the private key** (Sony PS3 failure; Android Bitcoin wallets) → use **deterministic nonces (RFC 6979)** or EdDSA; implementation bugs / **side-channel leaks** (timing, power, EM) during signing; complexity.

Uses: Bitcoin/Ethereum transactions, TLS certificates, smart cards/ID.

**ECIES [+]:** hybrid public-key encryption: ephemeral ECDH → KDF → symmetric cipher + MAC.

|RSA vs ECC|RSA|ECC|
|---|---|---|
|Hard problem|Integer factoring|ECDLP|
|Best attack|GNFS (sub-exponential)|Pollard rho (exponential √n)|
|128-bit security key|3072 bits|256 bits|


|RSA vs ECC|RSA|ECC|
|---|---|---|
|Signature size (128-bit sec.)|384 bytes|64 bytes (ECDSA/Ed25519)|
|Key generation|Slow (find primes)|**Fast**(random d)|
|Private-key op|Slow|**Faster**|
|Public-key op|**Very fast**(small e)|Slower than RSA verify|
|Encryption directly|Yes (OAEP)|Via**ECIES**|


⚠ **Traps:** ECDLP is **not** solved by index-calculus; 'ECC key 256 bits ≈ AES-128 security', **not** AES-256. ECDH ≠ ECDSA (agreement vs signature). **ECDSA nonce must be secret & unique** every signature. Point addition ≠ adding coordinates.

## 3.6 Digital Signatures & PKI

### 3.6.1 Concept and security goals

A **digital signature** is a value computed from a message using the **signer's private key** that anyone can verify with the **signer's public key** ; it is the electronic analogue of a handwritten signature but **depends on the message content** .

**Security goals (** 🔑 **):**

|Goal|Meaning|
|---|---|
|**Authentication**|Message really comes from the claimed signer|
|**Integrity**|Message not altered after signing|
|**Non-repudiation**|Signer cannot later deny signing (third party can<br>verify)|


⚠ **A digital signature does NOT provide confidentiality** (message is sent in the clear unless also encrypted). Non-repudiation in practice needs **trusted timestamping/certificates (trusted centre)** so that the key's ownership can be proven.

#### **Process (generic model, slides):**

1. **Sender hashes the message** : digest = H(M).

2. **Signs the digest with private key** : S = Sign(PR_sender, digest).

3. Sends **(M, S)** (+ certificate).

4. **Receiver** recomputes H(M’), **verifies S with sender's public key** to recover/compare the digest.

5. Match ⇒ **valid** ; mismatch ⇒ reject.

**Why sign the digest, not the message?** Efficiency (public-key ops are slow), fixed-size input (RSA can't handle long messages), and security (prevents some forgeries, e.g., multiplicative/existential). Needs a **collision-resistant hash** ; a hash collision lets an attacker transfer a signature to a different message.

**Forgery types (weakest→strongest ability):** _existential_ (some message), _selective_ (chosen message), _universal_ (any message / key recovery). **Attack models:** key-only, known-message, **chosen-message** . Goal of secure schemes: **EUF-CMA** (existentially unforgeable under chosen-message attack).

#### **Digital signature vs MAC vs handwritten:**

|Feature|Digital signature|MAC/HMAC|Handwritten|
|---|---|---|---|
|Key type|Asymmetric (private signs, public|Shared symmetric|–|
||verifies)|key||
|Non-|**Yes**|**No**(both can|Yes (weak)|
|repudiation||create)||
|Speed|Slow|**Fast**|–|
|Verifier needs|Public key/cert|Secret key|Comparison|


### 3.6.2 RSA signatures

- **Sign: s = (padded H(m))^d mod n** (private key). **Verify:** compute **s^e mod n** and compare with the padded hash. (Roles of e and d are **reversed** compared with encryption.)

**Example (n=187, e=7, d=23):** H(m)=10 → s=10²³ mod 187 = **175** ; verify 175⁷ mod 187 = **10** ✓.

- **Textbook RSA signatures are forgeable:** pick any s, compute m = s^e (existential forgery); multiplicative property σ(m₁)·σ(m₂)=σ(m₁m₂). Hence always **hash-then-sign with padding** : **PKCS#1 v1.5** (legacy) or **RSA-PSS (randomised, provably secure, recommended)** . (OAEP is for **encryption** ; PSS is for **signatures** .)

- RSA signing is **slow** (private exponent); verification **fast** (small e). Signature size = modulus size (256 B for 2048-bit).

### 3.6.3 DSA and ECDSA (ECC-based signatures)

**DSA (Digital Signature Algorithm, NIST FIPS 186, 1991–1994)** – based on the **DLP** ; **signature only (no encryption)** .

- **Parameters:** prime **p** (1024–3072 bits), prime **q** (160–256 bits) dividing p−1, **g = h^((p−1)/q) mod p** with order q. **Private x ∈ [1,q−1]** , **public y = g^x mod p** .

- **Sign:** random **k** ; **r = (g^k mod p) mod q** ; **s = k⁻¹(H(m) + x·r) mod q** ; signature **(r,s)** .

- **Verify:** w = s⁻¹ mod q; u₁ = H(m)·w mod q; u₂ = r·w mod q; **v = ((g^u₁·y^u₂) mod p) mod q** ; valid iff **v = r** .

- **Toy example (p=23, q=11, g=4, x=3 → y=18, k=7, H=5):** r=(4⁷ mod 23) mod 11= **8** , s=7⁻¹(5+3·8) mod 11 = 8·7 mod 11 = **1** . Verify: w=1, u₁=5, u₂=8, v = (4⁵·18⁸ mod 23) mod 11 = **8 = r** ✓.

- **Advantages (slides):** small signature (2×|q| = 320–512 bits), fast signing, standardised, royalty-free. **Cons: verification slower than RSA** , needs **high-quality random k** (reuse/leak ⇒ private key exposed), signature only, 1024-bit DSA is now weak; FIPS 186-5 no longer approves DSA for new signatures.

#### **DSA vs RSA signatures:**

||DSA|RSA|
|---|---|---|
|Hard problem|DLP|Factoring|
|Functionality|**Signatures only**|Signatures + encryption|
|Signing|Faster|Slower|
|Verification|**Slower**|**Faster**|
|Signature size|2×|q|
|Randomness at signing|**Mandatory (k)**|Not needed (unless PSS salt)|


**ECDSA** = DSA over elliptic curves (see 3.5.6): same structure, shorter keys/signatures (64 bytes at 128-bit security). **Other schemes (slide):** ElGamal, Schnorr, **EdDSA/Ed25519** (deterministic nonce), BLS, blind/group/ring signatures, Lamport & hash-based one-time signatures, and PQ signatures ( **ML-DSA/Dilithium, SLH-DSA/SPHINCS+** ).

**Applications:** software/firmware signing & secure boot, TLS/X.509 certificates, S/MIME & PGP email, legal e-signatures (e.g., India’s IT Act 2000 recognises digital signature certificates), PDFs/contracts, blockchain transactions, code repositories.

### 3.6.4 PKI – Public Key Infrastructure

**Problem:** how do you know a public key really belongs to Bob? (Else MITM.) **Solution:** a **digital certificate** = document binding **identity ↔ public key** , **signed by a trusted Certificate Authority (CA)** .

**PKI components:**

|Component|Role|
|---|---|
|**CA (Certificate Authority)**|Issues & signs certificates; the 'point of trust'|
|**RA (Registration Authority)**|Verifies identity of applicants on CA's behalf|
|**Certificate / key repository**<br>**(directory)**|Publishes certs|
|**CRL / OCSP**|Revocation checking (Certificate Revocation List /<br>Online Certificate Status Protocol)|
|**End entities / relying parties**|Users, servers, devices|
|**Policies (CP/CPS)**|Rules of issuing|


**How a certificate works (slides):** (1) Applicant generates key pair; (2) sends **CSR** (public key + identity); (3) CA verifies identity; (4) CA **signs** the certificate with **its private key** ; (5) subject presents certificate; (6) verifier checks **CA signature using CA's public key** , validity dates, revocation, usage.

**Chain of trust: Root CA** (self-signed, pre-installed in OS/browser trust store) → **Intermediate CA(s)** → **leaf (end-entity) certificate** . Verification walks the chain up to a trusted root.

**Types of certificates (slides):** SSL/TLS server certs ( **DV / OV / EV** validation levels), client/personal, **code-signing** , **email (S/MIME)** , document-signing, CA certs. **Who can issue:** trusted public CAs (e.g., DigiCert, Let’s Encrypt, GlobalSign, licensed CAs under national controllers) or private/enterprise CAs for internal use.

**Trust models [+]: hierarchical PKI** (CAs) vs **web of trust** (PGP: users sign each other’s keys). **Challenges (slides):** CA compromise ( **DigiNotar 2011** ), mis-issuance, revocation latency, key protection, cost, trust-store management, expiry/renewal. **Revocation:** CRL (periodic list), **OCSP** (real-time query), **OCSP stapling** .

⚠ **Traps:** The CA signs with **its private key** ; you verify with the **CA’s public key** (not the subject’s). A certificate contains the **public** key, never the private. **Root certificates are self-signed** and trusted by being pre-installed. PKI solves **key authenticity** , not key secrecy. The detailed **X.509 structure** is in Unit 4 (4.2.3).

## 3.7 Cryptographic Hashes: Properties, Merkle–Damgård, SHA256

### 3.7.1 What is a cryptographic hash function?

_H: {0,1} → {0,1}^n_ * maps a message of **any length** to a **fixed-length digest** (fingerprint / message digest). Requires **no key** . Output sizes: MD5 128, SHA-1 160, SHA-256 256, SHA512 512 bits.

**General (non-crypto) properties:** (1) **variable-length input** , (2) **fixed-length output** , (3) **efficient** to compute, (4) **deterministic** (same input → same digest).

### 3.7.2 Security properties ( 🔑 most-tested)

|Property|Meaning|Brute-force effort<br>for n-bit hash|
|---|---|---|
|**Pre-image resistance (one-**<br>**way)**|Given digest h, infeasible to find<br>**any**m with H(m)=h|**2^n**|
|**Second pre-image resistance**<br>**(weak collision resistance)**|Given m₁, infeasible to find**m₂ ≠**<br>**m₁**with H(m₂)=H(m₁)|**2^n**|
|**Collision resistance (strong)**|Infeasible to find**any pair**m₁ ≠<br>m₂ with H(m₁)=H(m₂)|**2^(n/2)**(birthday<br>bound)|
|**Avalanche effect**|Flipping 1 input bit changes<br>~**50%**of output bits|–|


**Relations (** 🔑 **): Collision resistance ⇒ second pre-image resistance** (generally); collision resistance does **not** imply pre-image resistance in general. Collision resistance is the **strongest/most demanding** requirement and the first to fall (MD5, SHA-1). Because of collisions always exist ( **pigeonhole principle** : infinite inputs, finite outputs) – security means they are **hard to find** .

#### **Which property is needed where:**

|Application|Needs|
|---|---|
|Password storage|Pre-image resistance (plus salting, slow hash)|
|File integrity (known original)|Second pre-image resistance|
|**Digital signatures**|**Collision resistance**(attacker could get benign<br>doc signed, swap malicious one)|
|Commitments, blockchain|Pre-image & collision resistance|


### 3.7.3 Birthday paradox / birthday attack (slides)

- **Paradox:** in a group of just **23 people** the probability that two share a birthday is **≈ 50.7%** (slide computes P(no match)≈0.492 → ~50.8%); with 70 people ≈ 99.9%. Pigeonhole guarantees a match only with 366 people.

- **P(no collision) = ∏ (1 − i/N)** for i=1..k−1; collision probability ≈ 1 − e^(−k²/2N).

- **Birthday attack on hashes:** to find a collision for an n-bit hash you need only **≈ 1.18·√(2^n) ≈ 2^(n/2)** hashes, not 2^n. So an n-bit hash has only **n/2 bits of collision security** .

   - 64-bit hash → 2³² tries (trivial). **MD5 (128) → 2⁶⁴** by brute force (much less with cryptanalysis). **SHA-256 → 2¹²⁸.**

- **Mechanics (contract-fraud style):** attacker writes a legitimate and a fraudulent contract; makes **2^(n/2) cosmetic variants** (spaces, synonyms) of each; finds a pair with equal hash; victim signs the benign one; the signature is valid for the fraudulent one.

- **Defences / best practices:** use **≥ 256-bit digests** , collision-resistant modern hashes (SHA-2/SHA-3), **randomise before signing** (salt/prefix), don’t sign untrusted content blindly, avoid MD5/SHA-1.

- **Real-world:** MD5 rogue-CA certificate (2008); **SHA-1 'SHAttered' collision (2017, Google/CWI)** ; Flame malware used MD5 collision to forge Windows Update signature.

**Random Oracle Model (ROM) (slides):** an **idealised hash** = a magic oracle holding a table. On a **new** query it **flips fair coins** to produce a random n-bit answer and records it; on a **repeated** query it returns the **same** recorded answer. Used for **security proofs** (OAEP, RSA-FDH, Fiat–Shamir). ⚠ No real hash is a true random oracle; ROM proofs are heuristic evidence, not guarantees.

### 3.7.4 Uses of hash functions

Message integrity/fingerprint · digital signatures (hash-then-sign) · **password storage** · **HMAC/MAC** · key derivation (HKDF, PBKDF2) · data-corruption detection · **blockchain (block hashing, Merkle trees, proof-of-work)** · commitments · deduplication · **Lamport one-time passwords** .

**Lamport one-time password (S/KEY) (slide):** pick secret s; server stores **Hⁿ(s)** . 1st login user sends **Hⁿ⁻¹(s)** ; server hashes it once, compares with stored value, then stores Hⁿ⁻¹(s). Next login uses Hⁿ⁻²(s), etc. **Replay-proof** because of one-wayness; list is used in **reverse** ; a finite n means re-seeding after n logins.

### 3.7.5 Attacks on hash functions

|Attack|Target|Complexity / note|
|---|---|---|
|Brute-force /<br>exhaustive|Pre-image, 2nd pre-image|2^n|
|**Birthday**|Collision|2^(n/2)|
|**Cryptanalytic**<br>**(differential)**|Collisions in MD5 (Wang 2004),<br>SHA-1 (2017)|far below generic bound|
|**Length-extension**|MD-type unkeyed hashes (MD5,<br>SHA-1, SHA-256, SHA-512)|Given H(m) and|
|**Rainbow table /**<br>**dictionary**|Password hashes|Defeated by**salt**|
|**Multicollision (Joux)**|Iterated hashes|Concatenating hashes<br>adds little|


### 3.7.6 Merkle–Damgård (MD) construction ( 🔑 diagram + steps)

A method to build a **hash for arbitrary-length input from a fixed-size compression function f** . Used by **MD5, SHA-1, SHA-2** .

**Compression function:** f: {0,1}^(n+b) → {0,1}^n – takes the **n-bit chaining value** and a **bbit message block** , outputs a new n-bit chaining value (MD5/SHA-1/SHA-256: b=512).

**Steps (slides):**

1. **Padding + length strengthening:** append a '1' bit, then '0' bits, then the **message length (64-bit)** so total is a multiple of b.

2. **Split** padded message into blocks **M₁, M₂, …, M_t** .

3. **Initialise** chaining value **H₀ = IV** (fixed public constant).

4. **Iterate: Hᵢ = f(Hᵢ₋₁, Mᵢ)** for i = 1..t.

5. **Output** digest = H_t (optionally a final transform).

```
 IV=H0 --→[ f ]--H1--→[ f ]--H2--→ ... --→[ f ]--H_t = digest
          ↑            ↑                     ↑
         M1           M2                    M_t (+ padding + length)
```

**Main theorem (** 🔑 **): If the compression function f is collision-resistant, then the MD hash H is collision-resistant** (this is why **length padding** is essential). It reduces the problem to designing a small fixed-input f – usually from a block cipher in **Davies–Meyer** mode: Hᵢ = E_{Mᵢ}(Hᵢ₋₁) ⊕ Hᵢ₋₁.

**Advantages:** simple, proven reduction, efficient, streaming. **Limitations: lengthextension attack** (digest = full internal state); **multicollisions** ; long-message secondpreimage attacks (Kelsey–Schneier); strictly **sequential** . Alternatives: **HAIFA, wide-pipe, SHA-3 sponge (Keccak)** .

**Number of blocks:** a message of L bits needs **⌈(L + 65)/512⌉** blocks (1 bit '1' + 64 bit length minimum). E.g., 1000 bits → 3 blocks; 447 bits → 1 block; 448 bits → 2 blocks.

### 3.7.7 Hash families (MD5, SHA-1, SHA-2)

**MD5 (Rivest, 1991):** 128-bit digest; 512-bit blocks; **4 rounds × 16 steps = 64 steps** ; 32-bit words; state **A,B,C,D** ; **little-endian** ; padding to 448 mod 512 + 64-bit length. **MD4 → MD5:** MD4 had 3 rounds/48 steps; MD5 added a 4th round, unique additive constant per step, modified round functions and more feedback. **Broken:** collisions in seconds (Wang 2004), rogue CA certs (2008). Still used for non-security checksums only.

**SHA-1 (NIST/NSA, 1995):** 160-bit; 512-bit block; **80 rounds** (4 groups of 20); 5 registers **A–E** with IV 67452301, EFCDAB89, 98BADCFE, 10325476, C3D2E1F0; word expansion Wₜ = ROTL¹(Wₜ₋₃⊕Wₜ₋₈⊕Wₜ₋₁₄⊕Wₜ₋₁₆); constants 5A827999, 6ED9EBA1, 8F1BBCDC, CA62C1D6. **Broken in practice 2017 (SHAttered, ≈2⁶³ ops)** ; deprecated.

### 3.7.8 SHA-256 in detail ( 🔑 syllabus topic; supplemented **[+]** )

**SHA-2 family** (FIPS 180-4, NSA/NIST 2001). **SHA-256: 256-bit output** , **512-bit message blocks** , **32-bit words** , **64 rounds** , **8 working variables a–h** , Merkle–Damgård with **Davies– Meyer-style feed-forward** ; collision security **2¹²⁸** , pre-image **2²⁵⁶** .

**Constants:** initial hash values H₀…H₇ = first 32 bits of the **fractional parts of the square roots of the first 8 primes** (2,3,5,…,19): **6a09e667, bb67ae85, 3c6ef372, a54ff53a, 510e527f, 9b05688c, 1f83d9ab, 5be0cd19** . Round constants K₀…K₆₃ = fractional parts of **cube roots of the first 64 primes** (428a2f98, 71374491, …). ('Nothing-up-my-sleeve numbers'.)

#### **Algorithm:**

1. **Pad:** append '1', then zeros until length ≡ **448 mod 512** , then 64-bit big-endian message length.

2. For each 512-bit block: **message schedule** W₀…W₁₅ = the 16 words of the block; for t = 16..63: **Wₜ = σ₁(Wₜ₋₂) + Wₜ₋₇ + σ₀(Wₜ₋₁₅) + Wₜ₋₁₆** (mod 2³²).

3. Set (a,b,c,d,e,f,g,h) = current H₀..H₇.

4. **64 rounds:**

   - **T₁ = h + Σ₁(e) + Ch(e,f,g) + Kₜ + Wₜ**

   - **T₂ = Σ₀(a) + Maj(a,b,c)**

h=g, g=f, f=e, **e = d + T₁** , d=c, c=b, b=a, **a = T₁ + T₂**

5. **Feed-forward:** H₀ = H₀ + a, …, H₇ = H₇ + h (mod 2³²).

6. Digest = H₀∥H₁∥…∥H₇ (8 × 32 = 256 bits).

**Functions: Ch(e,f,g) = (e∧f)⊕(¬e∧g)** ('choose': e selects f or g bitwise); **Maj(a,b,c) = (a∧b)⊕(a∧c)⊕(b∧c)** (majority); **Σ₀(x)=ROTR²⊕ROTR¹³⊕ROTR²²** ; **Σ₁(x)=ROTR⁶⊕ROTR¹¹⊕ROTR²⁵** ; **σ₀(x)=ROTR⁷⊕ROTR¹⁸⊕SHR³** ;

**σ₁(x)=ROTR¹⁷⊕ROTR¹⁹⊕SHR¹⁰** (note σ uses a **shift** SHR, Σ uses only rotations).

**Test vectors:** SHA-256('abc') = **ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad** ; SHA-256('') = e3b0c442 98fc1c14… b855.

#### **SHA-2/SHA-3 comparison:**

|Algorithm|Digest|Block|Word|Rounds|Structure|Status|
|---|---|---|---|---|---|---|
|MD5|128|512|32|64|MD|**Broken**|
|SHA-1|160|512|32|80|MD|**Broken (collisions)**|
|SHA-224|224|512|32|64|MD|OK|
|**SHA-256**|**256**|**512**|**32**|**64**|MD|**Secure, ubiquitous**|
|SHA-384|384|1024|64|80|MD|OK|
|SHA-512|512|1024|64|80|MD|OK|
|SHA-3 (Keccak)|224–512|rate varies|64-bit lanes|24|**Sponge**|Secure;**no length-extension**|


⚠ **Hash traps:** Digest length is fixed **independent of input** . Collision security = **n/2** , preimage = **n** . **SHA-256 is NOT encryption** (not reversible). **Hash ≠ MAC** (no key). SHA-256 is vulnerable to **length extension** (use HMAC, SHA-512/256, SHA-3). Hash alone does not give authentication – anyone can recompute it. SHA-256 block 512 bits; SHA-512 block **1024** .

UNIT 3 – DEFINITIONS, FACTS, KEYWORDS & FORMULAE (quick list)

- **Symmetric keys for n users:** n(n−1)/2; **asymmetric:** n key pairs.

- **Encrypt with receiver's public key; sign with sender's private key; verify with sender's public key.**

- **TOWF:** easy forward, hard inverse **without trapdoor** , easy with it.

- **φ(pq)=(p−1)(q−1)** , **a^φ(n)≡1** , **a^(p−1)≡1 (mod p)** .

- **Miller–Rabin:** n−1=2^s·d; x=a^d; pass if x∈{1,n−1} or some x² chain hits n−1; error ≤ (1/4)^k. 561 is Carmichael (fools Fermat).

- **CRT:** pairwise coprime moduli → unique solution mod product; **x=ΣaᵢMᵢyᵢ** .

- **DLP:** given g, p, y=g^x find x; attacks: BSGS, rho, Pohlig–Hellman, index-calculus (not on EC).

- **GF(2^n):** polynomial arithmetic mod irreducible poly; **addition=XOR** ; AES poly **x⁸+x⁴+x³+x+1** .

- **RSA:** n=pq; φ=(p−1)(q−1); gcd(e,φ)=1; **d=e⁻¹ mod φ** ; C=M^e; M=C^d; e=65537; CRT ≈ 4× speed-up.

- **OAEP:** P₁=M⊕G(r); P₂=H(P₁)⊕r; C=(P₁∥P₂)^e; randomised, CCA-secure.

- **DH:** A=g^a, B=g^b, K=g^(ab) mod p; vulnerable to **active MITM** → authenticate.

- **ECC:** y²=x³+ax+b; doubling λ=(3x²+a)/2y; addition λ=(y₂−y₁)/(x₂−x₁); Q=dG; 256-bit ECC ≈ 3072-bit RSA.

- **ECDSA:** r=x(kG) mod n, s=k⁻¹(h+dr) mod n; reuse of k leaks d.

- **Signature goals:** authentication, integrity, non-repudiation (not confidentiality).

- **Hash:** pre-image 2^n; 2nd pre-image 2^n; collision 2^(n/2); SHA-256: 512-bit block, 64 rounds, 8×32-bit registers.

- **MD construction:** Hᵢ=f(Hᵢ₋₁,Mᵢ), IV, length padding; f collision-resistant ⇒ H collisionresistant.

## UNIT 3 – FREQUENTLY CONFUSED CONCEPTS (comparison tables)

|Confusable pair|Distinction|
|---|---|
|**Pre-image vs 2nd pre-image vs**<br>**collision**|Given**hash**(find message) / given**message**(find<br>another) /**free choice**(find any two)|
|**Primality: Fermat vs Miller–Rabin**|Fermat fooled by Carmichael numbers; MR is<br>stronger (≤1/4 error)|
|**Encryption padding vs signature**<br>**padding**|OAEP (encrypt) vs PSS / PKCS#1 v1.5 (sign)|
|**CRT in math vs RSA-CRT**|Generic theorem vs private-key speed-up by<br>splitting mod p, q|
|**DH vs RSA**|Key agreement (both derive) vs key<br>transport/encryption/sign|
|**DH vs ECDH**|Zp* multiplicative group vs EC additive group;<br>same logic|
|**DLP vs ECDLP**|Zp* (index-calculus applies) vs EC (only generic<br>attacks)|
|**DSA vs ECDSA**|Over Zp*/subgroup vs over EC group|
|**Digital signature vs MAC**|Asymmetric+non-repudiation vs symmetric key<br>only integrity/authentication|
|**Hash vs encryption**|Irreversible, no key vs reversible with key|


|Confusable pair|Distinction|
|---|---|
|**Certificate vs signature**|Signed binding of identity+public key by CA vs<br>signature on message|
|**MD5 vs SHA-1 vs SHA-256**|128/160/256-bit; 64/80/64 steps;<br>broken/broken/secure|
|**Σ vsσ in SHA-256**|Σ= 3 rotations (round function);σ= 2 rotations +<br>1 shift (message schedule)|


## UNIT 3 – POTENTIAL MCQ TRAPS (checklist)

1. 'Public-key cryptography is faster than symmetric' → **False** .

2. 'Each user has two keys' vs 'system has two keys' → only the first is correct.

3. 'Encrypt with sender's private key for confidentiality' → **False** ; that's a signature.

4. Miller–Rabin 'prime' answer is **probable** ; 'composite' is **certain** .

5. **561 = 3×11×17** Carmichael.

6. In RSA, **d is inverse of e mod φ(n)** , not mod n; e needs gcd(e,φ)=1, not gcd(e,n)=1.

7. RSA message must be **< n** ; OAEP reduces usable message length.

8. CRT speeds up **decryption/signing only** (needs p,q), ~ **4×** ; it doesn't improve security.

9. OAEP provides **randomisation/CCA security** , not speed.

10. DH gives **no authentication** → MITM; DH is **not** an encryption algorithm.

11. ECDSA nonce reuse ⇒ private key leak; DSA same.

12. ECC: 'smaller key = weaker' → **False** (256-bit ECC ≈ 3072-bit RSA).

13. Signature does **not** encrypt; non-repudiation ≠ confidentiality.

14. Collision resistance is hardest to achieve; birthday bound **2^(n/2)** .

15. MD construction needs **length padding** ; theorem direction: f CR ⇒ H CR (not converse).

16. SHA-256: **64 rounds** , 512-bit block, 8 registers; SHA-512: 80 rounds, 1024-bit block.

17. Root CA certificates are **self-signed** ; CA signs with its **private** key.

18. Pohlig–Hellman & index-calculus are **attacks on DLP** , not hash functions.

## UNIT 3 – PRACTICE MCQs (29 questions, answers with explanations)

_Mix of factual (F), conceptual (C), application/numerical (A) and tricky (T) questions._ **Q1 (A).** Number of distinct symmetric keys needed for 50 users to communicate pairwise? (a) 100 (b) 1225 (c) 2450 (d) 2500

✅ **(b)** n(n−1)/2 = 50·49/2 = 1225. (c) is n(n−1), forgetting to halve.

**Q2 (F).** Alice wants to send Bob a confidential message using RSA. Which key does she use? (a) Alice’s private (b) Alice’s public (c) Bob’s public (d) Bob’s private

✅ **(c)** Encrypt with the receiver’s public key; only Bob’s private key decrypts.

**Q3 (C).** Which of the following is a _trapdoor_ one-way function? (a) SHA-256 (b) g^x mod p (c) AES-128 encryption (d) x^e mod n with secret d (RSA)

✅ **(d)** Knowing the trapdoor (d or the factors of n) makes inversion easy. Hashes and g^x mod p are one-way but have no trapdoor.

**Q4 (A).** For Miller–Rabin on n = 97, n−1 = 2^s·d with d odd gives: (a) s=4, d=6 (b) s=5, d=3 (c) s=3, d=12 (d) s=2, d=24

✅ **(b)** 96 = 32·3 = 2⁵·3. In the other options d is even.

**Q5 (F).** The maximum probability that a composite number passes 5 independent Miller– Rabin rounds is: (a) 1/4 (b) 1/20 (c) 1/1024 (d) 1/625

✅ **(c)** (1/4)^5 = 1/1024 (per-round error ≤ 1/4).

**Q6 (T).** Which statement about 561 is correct? (a) It is prime (b) It is a Carmichael number that fools the Fermat test but is exposed by Miller–Rabin (c) Miller–Rabin declares it prime for every base (d) It is a strong pseudoprime to base 2

✅ **(b)** 561 = 3·11·17. For base 2 the MR chain 263→166→67→1 never hits −1 ⇒ composite, so it is _not_ a strong pseudoprime to base 2.

**Q7 (A).** Solve x ≡ 1 (mod 3), x ≡ 2 (mod 5) using CRT. (a) 4 (b) 11 (c) 7 (d) 13

✅ **(c)** 7: 7 mod 3 = 1, 7 mod 5 = 2; unique mod 15. (11 is 2 mod 3; 13 is 3 mod 5.)

**Q8 (T).** For which set of moduli can the standard CRT _not_ be applied directly? (a) 6, 10, 15 (b) 3, 5, 7 (c) 4, 9, 25 (d) 8, 15, 7

✅ **(a)** Not pairwise coprime (gcd(6,10)=2, gcd(6,15)=3, gcd(10,15)=5). The others are pairwise coprime.

**Q9 (A).** Solve the discrete log 2^x ≡ 9 (mod 11): (a) 3 (b) 4 (c) 5 (d) 6

✅ **(d)** Powers of 2 mod 11: 2,4,8,5,10, **9** → x = 6.

**Q10 (C).** Which attack on the DLP does **not** apply to well-chosen elliptic curves? (a) Babystep giant-step (b) Pollard’s rho (c) Pohlig–Hellman (d) Index-calculus

✅ **(d)** Index-calculus is non-generic (needs integer/field structure of Zp* or GF(2^m)*); that is why ECC can use small keys. The others are generic and work in any group.

**Q11 (A).** In GF(2³), the sum of 101 and 011 is: (a) 110 (b) 000 (c) 112 (d) 100

✅ **(a)** Addition in GF(2^n) is bitwise XOR: 101 ⊕ 011 = 110.

**Q12 (T).** Which of the following is **not** a field? (a) GF(7) (b) Integers mod 256 (c) GF(2⁸) (d) GF(2)

✅ **(b)** Z₂₅₆ has non-invertible elements (all even numbers). GF(2⁸) is built with polynomials mod an irreducible polynomial, not as integers mod 256.

**Q13 (A).** RSA with p = 5, q = 11, e = 3. The private exponent d is: (a) 7 (b) 13 (c) 27 (d) 33

✅ **(c)** φ = 4·10 = 40; 3·27 = 81 = 2·40 + 1 ⇒ d = 27.

**Q14 (A).** RSA with n = 33, e = 3. Ciphertext of M = 4 is: (a) 31 (b) 12 (c) 64 (d) 4

✅ **(a)** 4³ = 64; 64 mod 33 = 31. (64 is not reduced mod n.)

**Q15 (F).** Using CRT in RSA private-key operations speeds them up by roughly: (a) 8× (b) 2× (c) 16× (d) 4×

✅ **(d)** Two exponentiations with half-size modulus & exponent: 2·(1/2)³ = 1/4 the work. **Q16 (C).** The main weakness of textbook RSA that OAEP fixes is: (a) slow decryption (b) deterministic, malleable encryption (c) man-in-the-middle attacks (d) key distribution

✅ **(b)** OAEP adds randomness r and an all-or-nothing transform, giving semantic/CCA security. CRT fixes speed; PKI fixes key authenticity.

**Q17 (F).** In the slide notation of RSA-OAEP, the masked message is: (a) P₁ = M ⊕ G(r) (b) P₂ = M ⊕ H(r) (c) P₁ = r ⊕ H(M) (d) C = M^e mod n

✅ **(a)** P₁ = M ⊕ G(r); P₂ = H(P₁) ⊕ r; ciphertext C = (P₁∥P₂)^e mod n.

**Q18 (A).** In Diffie–Hellman with p = 23, g = 5, Alice sends A = 8, and Bob’s secret is b = 15. Bob’s shared key is: (a) 8 (b) 19 (c) 5 (d) 2

✅ **(d)** K = A^b mod p = 8¹⁵ mod 23 = 2 (Bob’s public value is 5¹⁵ mod 23 = 19).

**Q19 (C).** Basic Diffie–Hellman is vulnerable to a man-in-the-middle attack because: (a) the prime p is too small (b) the exchanged values are not authenticated (c) g is public (d) the DLP is easy

✅ **(b)** An active attacker can substitute its own values; fix with signatures/certificates (authenticated DH).

**Q20 (A).** On y² = x³ + 2x + 2 mod 17, the slope for doubling P = (5,1) is: (a) 2 (b) 5 (c) 13 (d) 9 ✅ **(c)** λ = (3·5² + 2)/(2·1) = 77/2; 77 ≡ 9 and 2⁻¹ = 9 (mod 17) ⇒ 81 mod 17 = 13.

**Q21 (T).** If an ECDSA signer reuses the same nonce k for two different messages, an attacker can: (a) only forge one message (b) cause both signatures to be rejected (c) recover the private key (d) change the public key

✅ **(c)** Two equations s₁, s₂ with the same k let the attacker solve for k and then d.

**Q22 (F).** The RSA modulus size giving about the same security as a 256-bit elliptic curve is: (a) 2048 (b) 3072 (c) 7680 (d) 1024

✅ **(b)** 256-bit ECC ≈ 128-bit security ≈ 3072-bit RSA (2048-bit RSA ≈ 112-bit).

**Q23 (C).** Which service is **not** provided by a digital signature? (a) Authentication (b) Integrity (c) Non-repudiation (d) Confidentiality

✅ **(d)** Signatures do not hide the message; encrypt separately for confidentiality.

**Q24 (C).** A browser verifies the signature on a website’s certificate using: (a) the server’s private key (b) the CA’s public key (c) the CA’s private key (d) the user’s public key

✅ **(b)** The CA signs with its private key; anyone verifies with the CA’s public key (from the trust store / chain).

**Q25 (F).** Generic (birthday) collision-finding effort for SHA-256 is about: (a) 2²⁵⁶ (b) 2¹²⁸ (c) 2⁶⁴ (d) 2³²

✅ **(b)** Collision bound is 2^(n/2); pre-image is 2^n = 2²⁵⁶.

**Q26 (C).** The Merkle–Damgård theorem states: (a) if the compression function is collision-resistant, so is the full hash (b) if the hash is collision-resistant, the compression function is secure against pre-images (c) padding is unnecessary (d) the construction resists length-extension

✅ **(a)** (with length-strengthening padding). MD hashes are actually **vulnerable** to length extension.

**Q27 (A).** How many 512-bit blocks does SHA-256 process for a 1000-bit message? (a) 1 (b) 2 (c) 3 (d) 4

✅ **(c)** ⌈(1000 + 65)/512⌉ = ⌈2.08⌉ = 3 (1 + 64 bits of padding/length are mandatory).

**Q28 (T).** Using MAC = SHA-256(key ∥ message) is insecure mainly because of: (a) birthday attacks (b) length-extension attacks (c) rainbow tables (d) Pohlig–Hellman

✅ **(b)** The digest reveals the internal state; use HMAC instead.

**Q29 (F).** The smallest group size for which two people share a birthday with probability above 50% is: (a) 23 (b) 183 (c) 365 (d) 50

✅ **(a)** ≈ 50.7% for 23 people – the basis of the birthday attack (~√N).

# UNIT 4: APPLIED CRYPTO, ATTACKS & FUTURE TRENDS

## 4.1 Entity Authentication: Passwords, Challenge–Response, ZeroKnowledge Proofs

### 4.1.1 Entity authentication basics

- **Entity (user) authentication** = proving **who you are to a system, in real time** ('is this entity alive and genuine _now_ ?'). **Message authentication** (MAC/signature) = proving **who created a message / that it is unaltered** . ⚠ Different goals.

- **Authentication factors (** 🔑 **): Knowledge** (password, PIN) · **Possession** (token, smart card, phone, TPM) · **Inherence** (biometrics: fingerprint, iris, face, voice) · _(also location/behaviour)_ . **Multi-factor authentication (MFA) = ≥ 2 different categories** (password + password ≠ MFA).

- **5 common types (slide):** password-based, multi-factor, **certificate-based** , **biometric** (face, fingerprint, speaker, iris), **token-based** .

- **Why important:** prevents unauthorised access, protects accounts/data, basis for access control & accountability.

### 4.1.2 Password-based authentication: hashing & salting

**Never store plaintext passwords.** Store a hash; at login hash the input and compare.

|Threat|Description|Defence|
|---|---|---|
|**Brute force**|Try all passwords|Length/complexity, rate limiting,<br>slow hashes|
|**Dictionary**<br>**attack**|Try common words/leaked<br>passwords|Strong passwords, slow hash|
|**Rainbow table**|Pre-computed hash→password<br>chains (time–memory trade-off)|**Salting**|
|**Credential**<br>**stuffing**|Reuse leaked pairs on other sites|MFA, unique passwords|
|**Phishing /**<br>**keylogging**|Steal password directly|MFA, FIDO2, user training|
|**Database**<br>**breach**|Hashes stolen|Salt + slow hash + pepper|


- **Salt (** 🔑 **):** a **random, unique-per-user value (≥ 128 bits)** stored **in the clear** next to the hash: **store (salt, H(salt ∥ password))** .


   - Same password → **different** hashes; **defeats pre-computed rainbow tables** ; forces attacker to crack each user separately.

   - ⚠ A salt is **not secret** and does **not** make a weak password strong.

- **Pepper [+]:** a **secret** value kept outside the database (HSM/config) mixed into hashing.

- **Slow password hashes [+]:** plain SHA-256 is too fast (GPUs test billions/s). Use **bcrypt, scrypt, PBKDF2, Argon2 (PHC winner 2015, memory-hard)** with a tunable **work factor** .

- Login check: compute H(stored_salt ∥ entered_pw) and compare in **constant time** .

### 4.1.3 Challenge–response authentication

**Idea:** the verifier sends a **fresh random challenge (nonce)** ; the prover returns a **response computed from the challenge and a secret** , proving knowledge **without sending the secret** and making old responses useless ( **replay-proof** ).

```
 Verifier                              Prover (knows secret K)
   | ---- challenge: random nonce c ---->  |
   | <--- response: R = f(K, c) ---------  |
   | computes f(K, c), compares             |
```

#### **Implementations:**

|Variant|Response|Notes|
|---|---|---|
|**Symmetric / MAC-**<br>**based**|R = HMAC(K, c) or H(K∥c)|Both hold shared K (CRAM-MD5,<br>NTLM, HOTP/TOTP-style)|
|**Public-key /**<br>**signature-based**|R = Sign(PR, c)|Verifier only needs public key (SSH<br>keys, FIDO2/WebAuthn, TLS client<br>auth)|
|**Time/counter-based**|challenge = time or<br>counter|OTP tokens, TOTP apps|
|**Smart card / token**|computed inside tamper-<br>resistant device|Key never leaves card|


**Types (slides): Static** challenges (do not change over time, e.g., **security questions in 'forgot password'** ) vs **Dynamic** challenges (change every attempt, e.g., random nonce, OTP, CAPTCHA). **Examples (slides):** CAPTCHA (human vs bot), passwords, biometrics, smart-card/OTP challenges. **Uses:** online banking, VPN/Wi-Fi (WPA2 4-way handshake), network logins, API authentication, remote access, ATMs. **Attacks & best practices:** _Replay_ (use fresh nonces, timestamps/counters), _reflection_ (include identities/direction in response), _MITM_ (use TLS, mutual authentication), _offline dictionary attack_ on captured responses when secret is a weak password (use strong secrets/PAKE), predictable challenges (use a CSPRNG), unlimited tries (rate-limit/lockout), combine with MFA.

⚠ **Trap:** challenge-response sends **no password** , but a **weak** shared secret can still be cracked offline from a sniffed (challenge, response) pair. The nonce must be **unpredictable and never reused** .

### 4.1.4 Zero-Knowledge Proofs (ZKP) – basics

**Definition (slide,** 🔑 **):** a method by which a **prover** convinces a **verifier** that a statement is true **without revealing anything except that the statement is true** ('I know X, I won’t tell you X, but I can prove it'). Introduced by **Goldwasser, Micali & Rackoff (1985)** .

#### **Three properties (** 🔑 **learn exact wording):**

|Property|Meaning|
|---|---|
|**Completeness**|If the statement is true, an**honest**prover<br>convinces an**honest**verifier|
|**Soundness**|If the statement is false,**no cheating prover**can<br>convince an honest verifier (except with negligible<br>probability)|
|**Zero-knowledge**|The verifier learns**nothing beyond the truth of the**<br>**statement**(transcript can be simulated without<br>the secret)|


#### **Types (slides):**

||Interactive ZKP|Non-interactive ZKP (NIZK)|
|---|---|---|
|Messages|**Many rounds**(commit–challenge–<br>response)|**Single message**proof|
|Challenge|Random from verifier|Derived by hash (**Fiat–Shamir**<br>**heuristic**) or common reference string|
|Examples|Fiat–Shamir ID protocol, Schnorr<br>identification|**zk-SNARKs, zk-STARKs**, Bulletproofs|


**Analogies (slides):** the **locked-safe** story (verifier puts a secret message in a safe; only a prover with the key can return the message) and **Ali Baba's cave** [+] (prover appears from the side the verifier names, proving knowledge of the secret door word).

**Fiat–Shamir (Feige–Fiat–Shamir) identification protocol (1988) (** 🔑 **steps):**

- **Setup:** trusted party picks **n = p·q** (two large primes, then discards them; slide bank example p=101, q=103, n=10403). Prover picks **secret s** (coprime to n), publishes **v = s² mod n** .

#### **Each round:**

1. **Commitment:** prover picks random **r** , sends **x = r² mod n** .

2. **Challenge:** verifier sends random **bit b ∈ {0,1}** .

3. **Response:** prover sends **y = r·s^b mod n** (b=0 → y=r; b=1 → y=r·s).

4. **Verification:** check **y² ≡ x·v^b (mod n)** (b=0: y²=x; b=1: y² = x·v).

- Repeat **t rounds (usually 20–40)** : a cheater who doesn’t know s passes each round with probability **1/2** , so overall **2^−t** .

- **Toy example:** n=77 (7·11), s=10, v=100 mod 77= **23** . r=5 → x=25. b=0: y=5, 5²=25=x ✓. b=1: y=5·10=50, 50²=2500 mod 77= **36** , and x·v=25·23=575 mod 77 = **36** ✓.

- **Hard problem:** computing square roots mod composite n (equivalent to factoring).

- **Why zero-knowledge:** verifier sees only random-looking (x,y) pairs, which it could have **simulated** itself. **Why r must be fresh:** reusing r with both b=0 and b=1 gives y₁=r and y₂=rs ⇒ s = y₂/y₁ leaks.

- **Soundness trap:** a cheater can prepare for **one** guessed b (50%), not both.

**Is ZK completely anonymous? (slide)** Not necessarily – the verifier learns that the prover _is someone who knows s_ (and public v ties to an identity); linkage/metadata can still leak; proofs hide the secret, not the existence of the interaction. **Pros:** secret never revealed, no password database to steal, resistant to replay (fresh randomness), strong privacy, scalable proofs (SNARKs). **Cons:** computational/communication overhead, interactive rounds, some systems need a **trusted setup** , complexity & implementation bugs, probabilistic soundness. **Applications:** identity/age verification without revealing data, **blockchain privacy (Zcash)** , rollups, voting, secure authentication, verifiable computation.

|Password vs Challenge–<br>response vs ZKP|Password|Challenge–<br>response|ZKP|
|---|---|---|---|
|Secret sent?|**Yes**(or its<br>hash)|No (derived<br>response)|**No, and nothing about**<br>**it leaks**|
|Replay resistant|No|**Yes**|**Yes**|
|Verifier stores|Hash+salt|Shared key /<br>public key|Public value only (v)|
|Rounds|1|1|**Many**(interactive) or 1<br>(NIZK)|


## 4.2 Homomorphic Encryption, HMAC & X.509 Certificates

### 4.2.1 Homomorphic encryption (HE)

**Definition (slide):** a **public-key encryption scheme (Gen, Enc, Dec)** where operations on **ciphertexts** correspond to operations on **plaintexts** : **Dec(c₁ · c₂) = m₁ ⋆ m₂** , where · is the group operation in the ciphertext space and ⋆ in the plaintext space. **Goal:** let a cloud **compute on encrypted data without decrypting** .

|Class|Operations on ciphertexts|Examples|
|---|---|---|
|**Partially HE**<br>**(PHE)**|**Only one**operation type (add**or**<br>multiply), unlimited times|**Paillier = additive**;**RSA**<br>**(textbook) = multiplicative**;<br>**ElGamal = multiplicative**;<br>Goldwasser–Micali (XOR)|
|**Somewhat HE**<br>**(SHE) [+]**|Both, but limited circuit depth<br>(noise grows)|BGN (one multiplication)|
|**Fully HE (FHE)**|**Both addition and multiplication**<br>**→ arbitrary computation**|**Gentry (2009)**, BGV, BFV,**CKKS**,<br>TFHE|


- **Paillier (additive):** public key n=pq (g=n+1); **Enc(m,r) = g^m · r^n mod n²** . **Enc(m₁)·Enc(m₂) mod n² = Enc(m₁+m₂)** → multiplying ciphertexts **adds** plaintexts; also Enc(m)^k = Enc(k·m). Use: **e-voting tallies** , private statistics. (Exponent

- structure: multiplying ciphertext terms multiplies g^m¹ · g^m² = g^(m₁+m₂).)

- **Textbook RSA example:** E(m₁)·E(m₂) = (m₁m₂)^e ⇒ multiplicative (the same property that makes textbook RSA malleable).

- **FHE (Gentry 2009):** built on **lattice-based (ideal lattice)** cryptography; each operation adds **noise** ; **bootstrapping** homomorphically _refreshes_ a noisy ciphertext so computation can continue indefinitely.

- **Advantages:** privacy-preserving cloud computation, ML inference on encrypted data, secure multi-party analytics (healthcare, finance), data stays encrypted at rest, in transit **and in use** .

- **Limitations: very slow** (orders of magnitude), **large ciphertexts/keys** , noise management, complex; **malleable ⇒ no IND-CCA2 security** (by design); needs careful parameter choice.

|PHE vs FHE|PHE|FHE|
|---|---|---|
|Operations|Add**or**multiply|Add**and**multiply (any circuit)|
|Speed|Fast/practical|Slow (improving)|
|Ciphertext size|Small|Large|
|Examples|Paillier, ElGamal, RSA|Gentry, BFV, CKKS, TFHE|
|Quantum-safe?|No (factoring/DLP based)|Lattice-based → believed yes|


⚠ **Traps:** Paillier is **additive** , ElGamal/RSA **multiplicative** . FHE needs **both** operations. Homomorphic ≠ 'can decrypt without key'. Gentry’s FHE is **lattice-based** , introduced **2009** .

### 4.2.2 Message authentication & HMAC

**Message Authentication Code (MAC):** a short **tag = MAC(K, m)** computed with a **shared secret key K** . Receiver recomputes and compares. Provides **integrity + (symmetric) data-origin authentication** . **No confidentiality, no non-repudiation** (both parties know K).

- **Components:** message, secret key, MAC algorithm, tag.

- **Types / approved algorithms: HMAC** (hash-based), **CMAC/OMAC** (block-cipher based, e.g., AES-CMAC), **CBC-MAC** (secure only for fixed-length messages), **GMAC/GCM** & **Poly1305** (universal-hash), **KMAC** (SHA-3).

- **Benefits:** fast, small tag, detects tampering/forgery. **Limitations:** needs **shared key distribution** , **no non-repudiation** , **replay possible** unless nonce/counter/timestamp included, key compromise breaks both sides.

- **MAC vs plain hash (slide 'MAC vs SHA'):** a hash has **no key** – an attacker can modify the message and recompute the hash; a MAC needs **K** .

**HMAC (RFC 2104) (** 🔑 **formula): HMAC(K, m) = H( (K′ ⊕ opad) ∥ H( (K′ ⊕ ipad) ∥ m ) )**

- **K′** = key padded with zeros to block size **B** (if K longer than B, K′ = H(K)).

- **ipad = 0x36** repeated B times; **opad = 0x5C** repeated B times. **B = 64 bytes for MD5, SHA-1, SHA-256; 128 bytes for SHA-384/512.**

- Output length = hash output length (HMAC-SHA256 = 256 bits).

- **Inner hash** over (K⊕ipad)∥m, **outer hash** over (K⊕opad)∥inner → **prevents lengthextension** (unlike naive H(K∥m)).

Works with **any iterated hash** ; security reduces to PRF property of compression function; remains secure even if the hash has some collision weakness (but still prefer SHA-2/3).

**Integrity + confidentiality together (slide 'Simultaneous Integrity and Confidentiality'):** HMAC alone **does not hide** the message (a login request with HMAC is authentic but still readable). Use **separate keys** K₁ (encryption) and SK (HMAC): compute EM = Enc(M, K₁), HM = HMAC(·, SK) and send both. **Order matters [+]: Encrypt-then-MAC** (MAC over ciphertext) is the safest; MAC-then-Encrypt / Encrypt-and-MAC have pitfalls. Modern alternative: **AEAD (AES-GCM, ChaCha20-Poly1305)** .

**Applications (slides):** verification of e-mail/activation links, authenticating form data returned from a browser, **IoT** (cheap), one-time password-reset links without server state, API request signing, **TLS/IPsec record integrity** , **JWT (HS256)** , **HOTP/TOTP** , session cookies. **When to use:** when both ends share a key and need fast integrity/authentication (payments, health data, regulated environments). **Advantages:** fast (hash-based, no public-key maths), widely standardised, suitable for routers/embedded, smaller than signatures. **Disadvantages:** shared secret management, **no non-repudiation** , depends on the strength of the hash and key, key must be ≥ hash output length.

|HMAC vs Digital signature|HMAC|Digital signature|
|---|---|---|
|Key|**Symmetric shared**|**Asymmetric**pair|
|Speed|**Fast**|Slow|
|Non-repudiation|**No**|**Yes**|
|Who can verify|Only key holders|Anyone with public key|
|Tag size|160–512 bits|512–2048+ bits|


⚠ **Traps:** ipad= **0x36** , opad= **0x5C** (don’t swap). HMAC ≠ encryption. HMAC(K,m) ≠ H(K∥m). Compare tags in **constant time** .

### 4.2.3 X.509 certificates

- **X.509** = international ( **ITU-T** ) standard for **public-key certificates** ; v3 profile in **RFC 5280 (PKIX)** . A certificate **binds a public key to an identity (subject)** , **signed by a Certificate Authority** ; core of **PKI** , **TLS/HTTPS** , S/MIME, code-signing, VPN, smartcard login, EAP-TLS, IoT device identity.

**Encoding:** ASN.1 **DER** ; text form **PEM** (Base64 with -----BEGIN CERTIFICATE-----). Files: .crt .cer .pem .der; .p12/.pfx also carry private key.

#### **Structure (** 🔑 **fields in slide order):**

|Field|Content|
|---|---|
|**Version**|v1, v2,**v3**(v3 supports extensions; stored value 2)|
|**Serial number**|Unique per CA; used in revocation|
|**Signature algorithm**|e.g., sha256WithRSAEncryption, ecdsa-with-<br>SHA256|
|**Issuer**|Distinguished name of the**CA**|
|**Validity**|**notBefore**and**notAfter**|
|**Subject**|Owner's distinguished name (CN, O, C…)|
|**Subject Public Key Info**|Algorithm + the**public key**|
|**Extensions (v3)**|**Subject Alternative Name (SAN)**,**Key Usage**,<br>Extended Key Usage,**Basic Constraints (CA:**<br>**true/false, pathLen)**, Authority/Subject Key<br>Identifier,**CRL Distribution Points**,**Authority Info**<br>**Access (OCSP)**, Certificate Policies|
|**CA digital signature**|Signature over all the above (the TBSCertificate)<br>with**CA's private key**|


**Issuance:** (1) applicant generates key pair; (2) creates **CSR** (identity + public key, signed with its private key); (3) **CA validates** identity (DV/OV/EV); (4) CA **signs** and issues the cert; (5) certificate installed/published. **Validation & chain of trust:** (1) verify signature using issuer’s public key; (2) check **validity dates** ; (3) **build chain** leaf → intermediate → **trusted root** ; (4) check **key usage / basic constraints / hostname vs SAN** ; (5) check **revocation** (CRL, OCSP, stapling). **Root** certs are **self-signed** and pre-trusted. **Self-signed** leaf certs give no third-party assurance.

⚠ **Traps:** certificate contains **public** key only; hostname matching uses **SAN** (CN deprecated); the signature is by the **issuer (CA)** , not the subject; Basic Constraints CA:FALSE on leaf; an expired or revoked cert fails validation even with valid signature.

## 4.3 Introduction to Hardware Security: The 'Physical Attacker' Model

**Classical (black-box / network) model:** attacker sees only inputs/outputs or network traffic (Dolev–Yao). **Physical attacker model (slides,** 🔑 **):** the attacker has **physical access** to the device (stolen phone, smart card, IoT sensor in the field, HSM in a hostile site) and can **observe, measure and manipulate** it while it runs cryptographic operations.

- Tools: **oscilloscopes, measurement probes, EM probes, logic analysers, lasers, glitchers** , microscopes/FIB.

- Can **manipulate** supply **voltage, clock, temperature** , inject faults, desolder/read memory, probe buses.

- **Objective:** extract secret keys / firmware or **bypass security checks** . **Key lesson:** a **mathematically secure** algorithm (AES, RSA) can be **broken through its implementation / physical behaviour** .

#### **Types of physical attacks (slides) and classification [+]:**

|Attack|What the attacker does|Class|
|---|---|---|
|**Side-channel attack**|Observes timing, power, EM, sound,<br>cache behaviour|**Passive, non-**<br>**invasive**|
|**Electromagnetic analysis**<br>**(EMA)**|Measures EM emanations of circuits|Passive, non-<br>invasive|
|**Fault injection**|Induces errors (voltage/clock glitch,<br>laser, EM pulse, Rowhammer) to get<br>faulty outputs/skip checks (e.g.,<br>**Bellcore attack on RSA-CRT**, DFA on<br>AES)|**Active**, non-/semi-<br>invasive|
|**Physical tampering**|Modifies/replaces components, re-<br>flashes firmware|Active, invasive|
|**Microprobing**|Probes internal chip wires/buses after<br>decapsulation|**Invasive**|
|**Reverse engineering**|Delayering, FIB, imaging the chip|Invasive|


- **Non-invasive** = no physical damage, device unaware; **semi-invasive** = chip opened but no electrical contact (optical/laser fault); **invasive** = direct access to internal structures.

- **Countermeasure families:** tamper-resistant/tamper-evident packaging, shielding meshes & sensors (light, voltage, temperature), **zeroisation** of keys on tamper detection, **secure elements/smart cards/HSMs/TPMs** , **PUFs** , encrypted memory & secure boot, **constant-time/masked/hidden implementations** , redundancy & fault detection.

⚠ **Traps:** side-channel attacks do **not** break the algorithm’s mathematics; they are **implementation** attacks and are **passive** . Fault injection is **active** .

## 4.4 Side-Channel Attacks: Timing, Power Consumption & Introduction to DPA

**Definition (slides):** attacks that exploit **information leaked unintentionally** during computation (execution time, **power consumption** , electromagnetic emissions, cache behaviour, acoustic signals) instead of attacking the cipher itself.

**Generic workflow (slide):** Cryptographic operation with secret key → **physical leakage** → **measurement** (equipment) → **statistical analysis** of many measurements → **secret key recovered** . More observations improve the attack.

### 4.4.1 Timing attacks

- **Idea:** different inputs/execution paths take **different time** ; if timing depends on a **secret** , measuring time leaks the secret. Repeated measurements + statistics reveal patterns.

- **Classic example (Kocher 1996) – RSA/DH square-and-multiply:** for each exponent bit, **a multiplication occurs only when the bit is 1** → total time correlates with the number/positions of 1-bits → attacker recovers private exponent bit-by-bit from many timings.

- **Other examples [+]: early-exit comparison** of password/MAC bytes ( `memcmp` returns at first mismatch → guess byte-by-byte); **cache-timing attacks** on AES table lookups; remote timing attack on OpenSSL (Brumley–Boneh 2003); microarchitectural leaks (Spectre/Meltdown 2018).

**Countermeasures (slides + [+]): constant-time programming** (no secret-dependent branches or memory addresses), **blinding** (RSA: decrypt c·r^e mod n, then multiply by r⁻¹ so timing is decorrelated from the real ciphertext), **square-and-multiply-always / Montgomery ladder** , **constant-time comparison** , hardware AES instructions (AESNI), random delays (weak).

### 4.4.2 Power analysis

**Principle:** in CMOS circuits the **instantaneous power drawn depends on the data being processed and operations executed** (bit transitions, Hamming weight/distance of values). Measure by inserting a **small shunt resistor (≈10 Ω) in the power/ground line** and recording the voltage with an **oscilloscope** → a **power trace** .

||**SPA – Simple Power Analysis**|**DPA – Differential Power**<br>**Analysis**|
|---|---|---|
|Traces needed|**One or few**|**Hundreds to thousands**<br>(different known inputs)|
|Method|**Visual inspection**of one trace|**Statistical**(difference of<br>means / correlation)|
|Knowledge of<br>device internals|Needs knowledge of<br>implementation|Needs only a**leakage model**<br>(e.g., Hamming weight of S-box<br>output)|
|Example|Distinguish**square vs multiply**in<br>RSA → read exponent bits<br>directly; DES key-schedule<br>rotations|Recover**AES key byte-by-byte**|
|Noise tolerance|Poor|**Good**(averaging cancels noise)|
|Inventors|–|**Kocher, Jaffe & Jun, 1999**|


**DPA procedure (** 🔑 **steps):**

1. Collect **N power traces** while the device encrypts **N known (random) plaintexts** with the same secret key.

2. **Divide and conquer:** attack a **small part of the key** at a time, e.g., **one key byte** of the first AES round → only **256 guesses** .

3. For each guess k and each plaintext, **predict an intermediate value** (e.g., one bit of **Sbox(plaintext ⊕ k)** ).

4. **Partition** the traces into two groups by the predicted bit (0 or 1) and compute the **difference of the group means** at each time point.

5. The **correct key guess shows a clear spike** in the difference trace (prediction correlates with real power); **wrong guesses show ~flat noise** .

6. Repeat for the other bytes: total work ≈ **16 × 256 = 4096 guesses instead of 2¹²⁸** .

**Variants [+]: CPA** (Correlation Power Analysis – Pearson correlation with a Hammingweight model; more efficient), **Template attacks** , **Higher-order DPA** , **EM analysis (EMA)** . **Countermeasures: Masking** (split each secret value into random shares, e.g., x ⊕ m, so power is independent of x), **hiding** (add noise, random delays/clock jitter, **shuffling** , **dualrail / constant-power logic** ), **key rekeying / limiting uses of a key** , shielding, **leakageresilient designs** .

⚠ **Traps:** SPA = **few traces, visual** ; DPA = **many traces, statistics, key byte guesses** . DPA does **not** require knowing exact circuit layout. Side-channel resistance is a **hardware/implementation** property, not a mathematical property of AES.

## 4.5 Hardware Trust: TPMs and PUFs

### 4.5.1 Trusted Platform Module (TPM)

- A **dedicated, tamper-resistant security chip (or firmware/virtual implementation)** acting as the platform's **hardware root of trust** . Standard: **TPM 2.0 by the Trusted Computing Group (TCG)** (ISO/IEC 11889).

- **Functions:** securely **generates and stores keys** , performs crypto on-chip, stores **integrity measurements** , supports **authentication, sealed storage and remote attestation** .

#### **Architecture / components (slides):**

|Component|Role|
|---|---|
|**Cryptographic engine**|RSA/ECC, SHA, HMAC,**random number**<br>**generator (RNG)**|
|**PCRs – Platform Configuration**|Store**hash measurements**of boot components;|
|**Registers**|can only be**extended**, never set directly|


|Component|Role|
|---|---|
|**Non-volatile (NV) storage**|Persistent data, counters, keys across power<br>cycles|
|**Key management / hierarchy**|Generates and protects keys;**Storage Root Key**<br>**(SRK)**wraps other keys|
|**Endorsement Key (EK)**|Unique, manufacturer-installed key giving the<br>TPM a**hardware identity**|
|**Attestation Key (AK)**|Signs statements (**quotes**) about platform state|


**Measured boot & PCRs (** 🔑 **):** during startup each component ( **firmware/UEFI → bootloader → OS kernel → drivers** ) is **hashed** and the hash is **extended** into a PCR: **PCR_new = H(PCR_old ∥ measurement)** . Result depends on **values and order** of everything measured; any change in the boot chain gives a different PCR value. An **event log** records measurements.

**Measured boot** _records_ what booted (does not stop it) vs **Secure boot** _verifies signatures and blocks_ unsigned code. ⚠ Different!

**Sealing / secure key storage:** keys or data can be **sealed to specific PCR values** – the TPM **releases them only when the platform is in the expected state** (and authorisation is given) → e.g., **BitLocker** disk key unlocks only for an untampered boot. **Remote attestation:** TPM **signs (quotes) selected PCR values plus a verifier-supplied nonce with the Attestation Key** ; the remote verifier checks the signature and compares PCRs with known-good values → detects unauthorised modification (nonce prevents replay).

**Working principle (slide):** firmware/OS call TPM commands; keys are generated **inside** the TPM and **never leave in plaintext** ; operations are performed inside; release subject to **authorisation and platform-state policy** . **Applications: full-disk encryption (BitLocker)** , secure & measured boot, device identity/authentication, credential protection (Windows Hello, certificates, VPN keys), remote attestation, enterprise compliance, Windows 11 requirement. **Limitations [+]:** discrete TPM bus can be **sniffed** ; does not protect against a compromised OS _after_ boot; trust in vendor; implementation flaws (e.g., ROCA RSA keygeneration bug).

### 4.5.2 Physically Unclonable Functions (PUFs)

- **Definition (slides):** a hardware primitive that uses **unique, uncontrollable physical variations introduced during chip manufacturing** to produce a **device-specific response** – a **hardware fingerprint** .

- **Working:** apply a **challenge** → physical circuit evaluates it → **response** determined by that chip’s physical characteristics. **(Challenge, Response) pair = CRP.** Same challenge on different chips → different responses; same chip → (nearly) same response.

- **Desired properties: unique** (inter-chip Hamming distance ≈ 50%), **reliable/reproducible** (small intra-chip distance), **unpredictable** , **unclonable** (even the manufacturer can't copy it).

#### **Types (slides):**

|PUF|Physical source|Notes|
|---|---|---|
|**SRAM PUF**|Random**start-up state**of SRAM<br>cells|'Weak' PUF; used for**key**<br>**generation**; no extra hardware|
|**Arbiter PUF**|**Delay differences**in two<br>symmetric signal paths|'Strong' PUF (many CRPs);<br>**vulnerable to ML modelling attacks**|
|**Ring-oscillator**|**Frequency differences**between|Compare counts → bits|
|**PUF**|oscillators||
|**Optical PUF**|**Light-scattering**pattern of a<br>random medium|Hard to model; needs optics|


**PUF-based key generation (slide):** raw response is **noisy** (temperature, voltage, aging) → use **error correction** with a **Fuzzy Extractor + helper data** to reproduce a stable secret → derive cryptographic key (KDF). The key is **not stored in non-volatile memory** → exists only while powered/evaluated. **Benefits:** device-specific identity, **no permanently stored secret key** , tamper evidence (invasive attack alters physical properties), low cost, makes cloning hard. **Challenges:** noise/aging/environmental variation, **ML modelling attacks** on strong PUFs, need secure CRP management, helper-data leakage, enrolment cost. **Applications: IoT device authentication** , **anti-counterfeiting** , secure key generation, semiconductor ID, lightweight embedded security.

|TPM vs|TPM|PUF|
|---|---|---|
|PUF|||
|Nature|**Dedicated chip/firmware module**<br>(stores keys)|**Physical property**of a circuit<br>(derives keys)|
|Keys|Generated &**stored**securely|**Reconstructed**on demand|
|Main uses|Measured boot, sealing, attestation,<br>disk encryption|Device ID, authentication, key<br>generation|
|Standard|TCG TPM 2.0|No single standard|
|Weakness|Bus sniffing, vendor trust|Noise, ML modelling|


## 4.6 Post-Quantum Cryptography (PQC): Why Quantum Threatens RSA/ECC; Lattice-based Crypto (Kyber / ML-KEM)

### 4.6.1 The quantum threat

- **Shor’s algorithm (1994):** on a large fault-tolerant quantum computer, **factors integers and computes discrete logarithms in polynomial time** → **breaks RSA, Diffie–Hellman, DSA, ECDH, ECDSA (all of ECC)** . Bigger keys don't help – the break is polynomial.

- **Grover’s algorithm (1996): quadratic speed-up** for unstructured search → brute-force key search 2^n → **2^(n/2)** . AES-128 → ~64-bit effective; **AES-256 → ~128-bit (still secure)** . Hash pre-image n → n/2. **Fix: double symmetric key/hash sizes (use AES256, SHA-384+).**

- Public-key crypto needs **replacement** ; symmetric crypto needs only **larger keys** .

- **Why act now? (slide 'What’s the rush?')** (1) **Harvest-now-decrypt-later:** adversaries store today's ciphertexts/keys to decrypt later; (2) **rewrite history:** forge signatures for old keys (e.g., blockchains); (3) deploying new crypto at scale takes **10+ years** ( **Y2Q / Q-Day** concept). Estimates for breaking RSA-2048 are of the order of **millions of noisy physical qubits** , so it is not imminent – but migration must start early. Systems need **crypto-agility** .

- **Two responses (slides): Quantum cryptography** (physicists: e.g., **QKD – BB84** : needs special hardware/fibre or line of sight, distance limits, **no authentication by itself** , new infrastructure) vs **Post-quantum (quantum-resistant) cryptography** (mathematicians: **classical algorithms running on ordinary computers** based on

problems believed hard for both classical and quantum computers; different problems from factoring/DLP). ⚠ **PQC ≠ quantum cryptography** : PQC needs **no quantum hardware** .

- **Blockchain link (slides):** blocks are chained by **hashes** (previous-block hash, Merkle root, proof of work); **digital signatures (ECDSA)** authorise transactions → quantum threat to signatures/keys (Shor), whereas hashes are only weakened by Grover.

### 4.6.2 Families of PQC (slides)

|Family|Based on|Examples|
|---|---|---|
|**Lattice-**<br>**based**|Hard lattice problems (SVP,<br>LWE)|**Kyber/ML-KEM, Dilithium/ML-DSA,**<br>**Falcon/FN-DSA**|
|**Code-based**|Decoding random error-<br>correcting codes|Classic McEliece, HQC|
|**Hash-based**|Security of hash functions<br>(Merkle trees)|**SPHINCS+/SLH-DSA**, XMSS, LMS|
|**Isogeny-**<br>**based**|Maps between elliptic curves|SIKE (**broken classically in 2022**)|
|**Multivariate**|Solving systems of multivariate<br>polynomials|Rainbow (**broken 2022**), UOV|


**NIST standards [+, current as of 2024–25]: FIPS 203 = ML-KEM (Kyber) – key encapsulation** ; **FIPS 204 = ML-DSA (Dilithium) – signatures** ; **FIPS 205 = SLH-DSA (SPHINCS+) – hash-based signatures** ; **Falcon → FN-DSA** (FIPS 206 in progress); **HQC** (code-based) chosen in 2025 as backup KEM. (Slide: NIST 2022 announced Kyber for encryption/key establishment; Dilithium, Falcon, SPHINCS+ for signatures.) NIST finalised ML-KEM/ML-DSA/SLH-DSA in **August 2024** . **Challenges (slides):** lack of mature standards/interoperability with existing protocols, **larger keys/ciphertexts/signatures** , performance and bandwidth overhead, implementation/side-channel risks, need for **hybrid** (classical + PQ, e.g., X25519 + ML-KEM in TLS) during migration.

### 4.6.3 Lattice-based cryptography

- **Lattice:** all integer combinations of basis vectors b₁…b_n: **L = { Σ aᵢ bᵢ : aᵢ ∈ ℤ }** – a regular grid of points in n-dim space. Same lattice has 'good' (short, nearly orthogonal) and 'bad' (long, skewed) bases; **finding a good basis in high dimension is**

#### **hard** .

- **Hard problems: SVP** (shortest vector problem), **CVP** (closest vector problem), and **LWE – Learning With Errors (Regev 2005)** : given **(A, b = A·s + e mod q)** with small random **error e** , find secret **s** . Without e it is easy (Gaussian elimination); the **noise** makes it hard. **Ring-LWE / Module-LWE** use polynomial rings/modules → smaller keys and faster operations. **No efficient quantum (or classical) algorithm known** ; security has **worst-case to average-case reductions** .

- **Advantages:** quantum-resistant, **fast** (comparable to or faster than ECC), supports encryption, signatures and **FHE** . **Limitations: large keys/ciphertexts** (KBs vs 32 bytes for X25519), relatively young, parameter/decryption-failure subtleties, side-channel care.

#### **Kyber / ML-KEM (** 🔑 **):**

- A **Key Encapsulation Mechanism (KEM)** built on **Module-LWE** , made **IND-CCA2 secure** with the **Fujisaki–Okamoto transform** . Standardised as **FIPS 203 (ML-KEM)** .

- **Three algorithms: KeyGen → (pk, sk)** ; **Encaps(pk) → (ciphertext c, shared secret K)** ; **Decaps(sk, c) → K** . Both sides then use K (e.g., 32 bytes) with **AES** – a drop-in replacement for **(EC)DH key exchange** .

- **Parameters:** polynomial ring with **n = 256** , modulus **q = 3329** , module rank k = 2/3/4 for **ML-KEM-512 / 768 / 1024** (NIST security **levels 1/3/5 ≈ AES-128/192/256** ).

- **Sizes (ML-KEM-768):** public key **1184 B** , ciphertext **1088 B** , shared secret **32 B** .

- **Idea (sketch):** public key = (A, t = A·s + e); sender picks small r, computes u = Aᵀr + e₁ and v = tᵀr + e₂ + encode(m); receiver computes v − sᵀu = m + (small noise) and **rounds** to recover m. Slightly noisy → tiny decryption-failure probability.

⚠ **Traps:** Kyber/ML-KEM is a **KEM (key establishment)** , **not a signature** (Dilithium/MLDSA is). It is **lattice-based (Module-LWE)** . Shor breaks **RSA/ECC/DH** ; Grover only **halves** symmetric strength. **SIKE (isogeny)** and **Rainbow** were broken – PQ ≠ automatically secure. Quantum cryptography (QKD) ≠ post-quantum cryptography.

## UNIT 4 – DEFINITIONS, FACTS, KEYWORDS & FORMULAE (quick list)

- **Entity authentication** = prove identity live; **factors:** knowledge / possession / inherence; MFA = ≥ 2 _different_ factors.

- **Salt:** random per-user, stored with hash, **not secret** ; defeats rainbow tables. **Pepper:** secret. **Slow hashes:** bcrypt, scrypt, PBKDF2, **Argon2** .

- **Challenge–response:** fresh nonce c → R = f(K, c) (HMAC or signature); **static** vs **dynamic** challenges.

- **ZKP properties: completeness, soundness, zero-knowledge** ; interactive vs noninteractive (zk-SNARK/STARK, Fiat–Shamir heuristic).

- **Fiat–Shamir ID:** n=pq; v=s²; x=r²; b∈{0,1}; y=r·s^b; verify **y² ≡ x·v^b** ; cheat probability **2^−t** .

- **Homomorphic:** Dec(c₁·c₂)=m₁⋆m₂; **Paillier = additive** , **RSA/ElGamal = multiplicative** , **FHE = both (Gentry 2009, lattices, bootstrapping)** .

- **HMAC = H((K⊕opad)∥H((K⊕ipad)∥m))** ; ipad=0x36, opad=0x5C; B=64 (SHA-256); no non-repudiation; stops length-extension.

- **X.509 fields:** Version, Serial, Signature alg, Issuer, Validity, Subject, Public Key Info, Extensions (SAN, Key Usage, Basic Constraints…), CA signature. **CA signs with its private key.**

- **Physical attacker:** has physical access; observes (passive) or manipulates (active/invasive).

- **Side channels:** timing, power, EM, cache, acoustic. **SPA** (few traces, visual) vs **DPA** (many traces, statistics, 256 guesses per key byte). Defences: constant-time, **blinding** , **masking** , **hiding** .

- **TPM:** hardware root of trust; **PCR_new=H(PCR_old∥m)** ; measured boot; sealing; remote attestation (AK-signed quote + nonce); EK identity.

- **PUF:** challenge→response from manufacturing variation; SRAM / Arbiter / Ringoscillator / Optical; **fuzzy extractor + helper data** .

- **Quantum: Shor** (polynomial) breaks RSA, DH, DSA, ECC; **Grover** halves symmetric strength (use AES-256). **PQC ≠ QKD.**

- **Lattice:** SVP, CVP, **LWE** (b=As+e). **ML-KEM (Kyber, FIPS 203)** = Module-LWE KEM; **MLDSA (Dilithium, FIPS 204)** = signature; **SLH-DSA (SPHINCS+, FIPS 205)** = hash-based signature.

## UNIT 4 – FREQUENTLY CONFUSED CONCEPTS

|Pair|Distinction|
|---|---|
|**Entity vs message authentication**|Who is present now vs who created/altered the|
||message|
|**Salt vs pepper**|Public per-user random vs secret global|


|Pair|Distinction|
|---|---|
|**Hash vs MAC vs signature**|No key / shared key / private-public key pair|
|**Challenge–response vs ZKP**|Proves knowledge via keyed response (may leak<br>info) vs proves with**zero leakage**|
|**Interactive vs NIZK**|Multi-round vs single proof message|
|**PHE vs SHE vs FHE**|One operation / limited depth / unlimited both|
|**Paillier vs ElGamal/RSA**|Additive vs multiplicative|
|**Secure boot vs measured boot**|Verify & block vs measure & record|
|**TPM vs PUF**|Secure key store/attestation chip vs physical<br>fingerprint for key derivation|
|**EK vs AK**|Permanent hardware identity vs signing key for<br>attestation|
|**SPA vs DPA**|One trace/visual vs many traces/statistical|
|**Side-channel vs fault injection**|Passive observation vs active disturbance|
|**Masking vs hiding**|Randomise intermediate values vs make leakage<br>noisy/constant|
|**QKD vs PQC**|Quantum hardware key distribution vs classical<br>algorithms resistant to quantum|
|**Shor vs Grover**|Breaks RSA/ECC (poly time) vs quadratic search<br>speed-up|
|**ML-KEM vs ML-DSA vs SLH-DSA**|Key encapsulation (lattice) vs signature (lattice) vs<br>signature (hash-based)|


## UNIT 4 – POTENTIAL MCQ TRAPS

1. MFA = **different categories** ; two passwords is not MFA.

2. **Salt is not secret** ; it does not protect a weak password from a targeted guess; it defeats pre-computation.

3. Fast hashes (SHA-256) are **bad** for password storage; use slow memory-hard KDFs.

4. Challenge nonce must be **fresh/unpredictable** ; reuse enables replay.

5. ZKP: soundness error is **2^−t** , not 1/t; reusing commitment r **leaks s** .

6. ZKP ≠ 'no information about anything at all' – only about the **secret** ; existence/identity linkage may leak.

7. Paillier = **additive** ; RSA/ElGamal = **multiplicative** ; FHE = both and is **lattice-based** .

8. HMAC gives **integrity + authentication, not confidentiality, not non-repudiation** .

9. HMAC pads: **0x36 inner, 0x5C outer** ; block size 64 for SHA-256.

10. X.509: signature is by the **CA's private key** ; leaf has **public** key; hostnames in **SAN** .

11. Side channels are **passive** and exploit **implementation** ; fault injection is **active** .

12. Timing leak in square-and-multiply comes from the **conditional multiply** .

13. DPA needs **many traces and a leakage model** ; it attacks **key bytes separately** .

14. TPM PCRs cannot be written directly, only **extended** ; measured boot does not block.

15. PUF output is **noisy** – needs **fuzzy extractor/ECC** ; keys are not stored in NVM.

16. **Shor** breaks RSA/ECC/DH; **Grover** only halves symmetric security; **AES-256 stays safe** .

17. **Kyber = KEM** (lattice, Module-LWE); not a signature, not code-based.

18. QKD is **not** a post-quantum _algorithm_ ; PQC runs on classical computers.

## UNIT 4 – PRACTICE MCQs (36 questions, answers with explanations)

**Q1 (F).** A fingerprint scan is an example of which authentication factor? (a) Knowledge (b) Possession (c) Inherence (d) Location

✅ **(c)** Biometrics = something you _are_ (inherence).

**Q2 (C).** The main purpose of salting stored passwords is to: (a) encrypt the hash (b) defeat pre-computed rainbow tables and make identical passwords hash differently (c) make weak passwords strong (d) keep the hash secret

✅ **(b)** A unique random salt per user forces per-user cracking; it does not strengthen a weak password.

**Q3 (T).** Which statement about password storage is correct? (a) A salt must be kept secret like a key (b) SHA-256 is ideal because it is fast (c) The salt is stored alongside the hash (d) Salting stops brute-forcing a single user's weak password

✅ **(c)** Salt is public; fast hashes help attackers; salting does not stop targeted guessing.

**Q4 (C).** Challenge–response authentication resists replay attacks mainly because: (a) the password is encrypted (b) the challenge is fresh for every attempt (c) the key is long (d) the hash is slow

✅ **(b)** A captured response is useless for a new random challenge.

**Q5 (F).** Which is a _static_ challenge? (a) Random nonce (b) Time-based OTP (c) Security question in 'forgot password' (d) Signed server nonce

✅ **(c)** Static challenges don't change over time; the others are dynamic.

**Q6 (F).** The _soundness_ property of a ZKP says: (a) an honest prover convinces an honest verifier (b) no cheating prover can convince the verifier of a false statement (except negligibly) (c) the verifier learns nothing (d) the proof is one message

✅ **(b)** (a) is completeness; (c) is zero-knowledge.

**Q7 (A).** In Fiat–Shamir with 1-bit challenges, a cheating prover's success probability after 10 rounds is: (a) 1/10 (b) 1/20 (c) 1/100 (d) 1/1024

✅ **(d)** (1/2)^10 = 1/1024.

**Q8 (A).** Fiat–Shamir with n = 77 and secret s = 10: the public value v = s² mod n is: (a) 10 (b) 100 (c) 23 (d) 77

✅ **(c)** 100 mod 77 = 23.

**Q9 (F).** When the challenge bit is b = 1, the verifier checks: (a) y² ≡ x (b) y² ≡ x·v (mod n) (c) y ≡ x·v (d) y ≡ r

✅ **(b)** y = r·s ⇒ y² = r²s² = x·v.

**Q10 (C).** Which is a non-interactive zero-knowledge proof system? (a) Basic Fiat–Shamir ID protocol (b) zk-SNARK (c) Nonce-based challenge–response (d) Diffie–Hellman

✅ **(b)** SNARKs/STARKs (and the Fiat–Shamir _heuristic_ ) give a single-message proof. **Q11 (F).** Paillier encryption is: (a) multiplicatively homomorphic (b) additively homomorphic (c) fully homomorphic (d) symmetric-key

✅ **(b)** Multiplying Paillier ciphertexts adds plaintexts.

**Q12 (C).** Gentry's 2009 fully homomorphic scheme is based on: (a) factoring and CRT (b) lattices with bootstrapping (c) error-correcting codes (d) hash functions

✅ **(b)** Ideal lattices; bootstrapping refreshes noisy ciphertexts.

**Q13 (T).** Which pairing of scheme and homomorphic property is correct? (a) Paillier – multiplicative (b) ElGamal – additive (c) Textbook RSA – multiplicative (d) Gentry FHE – addition only

✅ **(c)** E(m₁)E(m₂)=E(m₁m₂) in textbook RSA.

**Q14 (F).** In HMAC the inner padding byte ipad equals: (a) 0x5C (b) 0x36 (c) 0xFF (d) 0x00 ✅ **(b)** ipad = 0x36, opad = 0x5C.

**Q15 (C).** MAC = SHA-256(K ∥ m) is weak mainly because of: (a) birthday attacks (b) length-extension attacks (c) side channels (d) slow speed

✅ **(b)** HMAC's nested structure fixes this.

**Q16 (T).** HMAC provides: (a) confidentiality (b) non-repudiation (c) integrity and sharedkey authentication (d) forward secrecy

✅ **(c)** Both parties know K, so no non-repudiation; HMAC doesn't encrypt.

**Q17 (F).** For HMAC-SHA256 the hash block size B and output size are: (a) 32 bytes, 128 bits (b) 64 bytes, 256 bits (c) 128 bytes, 512 bits (d) 64 bytes, 512 bits

✅ **(b)** B = 64 bytes for SHA-256; output 256 bits (SHA-512 would use 128 bytes).

**Q18 (C).** An X.509 certificate's signature is generated with: (a) the subject's private key (b) the subject's public key (c) the CA's private key (d) the CA's public key

✅ **(c)** Verified with the CA's public key.

**Q19 (F).** Which X.509 v3 extension lists the hostnames a TLS certificate is valid for? (a) Serial Number (b) Basic Constraints (c) Subject Alternative Name (d) Validity ✅ **(c)** SAN (CN is deprecated for matching).

**Q20 (C).** A root CA certificate is: (a) signed by an intermediate CA (b) self-signed and trusted because it is pre-installed (c) never expiring (d) the holder of the private key of all leaf certificates

✅ **(b)** Trust anchors are self-signed and distributed in trust stores.

**Q21 (C).** The 'physical attacker' model assumes the attacker: (a) sees only network traffic (b) has physical access and can measure or manipulate the device (c) can only mount man-in-the-middle attacks (d) cannot touch the hardware

✅ **(b)**

**Q22 (T).** Which is an _active_ attack? (a) Timing analysis (b) Simple power analysis (c) Voltage-glitch fault injection (d) Passive EM measurement

✅ **(c)** Fault injection disturbs the device; the others only observe.

**Q23 (C).** The timing attack on square-and-multiply RSA exploits: (a) factoring of n (b) a multiplication performed only for exponent bits equal to 1 (c) small public exponent (d) padding oracle

✅ **(b)**

**Q24 (C).** An effective countermeasure to timing attacks is: (a) longer keys (b) constanttime implementation / blinding (c) larger primes (d) OAEP padding

✅ **(b)**

**Q25 (C).** Compared with SPA, DPA: (a) needs a single trace (b) uses many traces with statistical analysis (c) requires visual inspection only (d) requires destroying the chip

✅ **(b)**

**Q26 (A).** In a DPA on AES-128 attacking one key byte at a time, the guesses per byte and in total are: (a) 16 and 256 (b) 256 and 4096 (c) 128 and 2048 (d) 2¹²⁸ total

✅ **(b)** 256 per byte × 16 bytes = 4096 instead of 2¹²⁸.

**Q27 (F).** The TPM PCR extend operation is: (a) PCR = measurement (b) PCR = H(PCR_old ∥ measurement) (c) PCR = PCR_old ⊕ measurement (d) PCR is overwritten

✅ **(b)**

**Q28 (T).** Which statement is correct? (a) Measured boot blocks unsigned code (b) Secure boot only records hashes (c) Measured boot records measurements in PCRs and does not itself block boot (d) They are the same

✅ **(c)**

**Q29 (C).** An SRAM PUF derives its response from: (a) arbiter delay differences (b) random power-up states of SRAM cells (c) light scattering (d) oscillator frequencies

✅ **(b)**

**Q30 (C).** The role of a fuzzy extractor in PUF key generation is to: (a) encrypt CRPs (b) turn a noisy PUF response into a stable key using helper data and error correction (c) generate challenges (d) speed up the PUF

✅ **(b)**

**Q31 (F).** Shor's algorithm threatens: (a) AES-128 only (b) factoring- and discrete-log-based schemes such as RSA, DH and ECC (c) SHA-256 collisions (d) AES-256 key search

✅ **(b)**

**Q32 (A).** Against Grover's algorithm, AES-128 offers approximately: (a) 128-bit (b) 64-bit (c) 32-bit (d) 256-bit security

✅ **(b)** 2^128 → 2^64 quantum search steps.

**Q33 (C).** CRYSTALS-Kyber (ML-KEM) is: (a) a hash-based signature (b) a lattice-based key encapsulation mechanism (Module-LWE), FIPS 203 (c) a code-based KEM (d) an isogenybased signature

✅ **(b)**

**Q34 (T).** Which is **not** a family of post-quantum algorithms? (a) Lattice-based (b) Codebased (c) Quantum key distribution (d) Hash-based

✅ **(c)** QKD is quantum cryptography needing quantum hardware; PQC algorithms run on classical computers.

**Q35 (C).** 'Harvest now, decrypt later' means: (a) deleting old keys (b) storing today's encrypted traffic to decrypt it once a quantum computer exists (c) harvesting salts (d) side-channel measurement

✅ **(b)** Hence long-lived secrets need PQC now.

**Q36 (F).** ML-DSA (CRYSTALS-Dilithium), standardised as FIPS 204, is a: (a) keyencapsulation mechanism (b) hash function (c) lattice-based digital signature scheme (d) block cipher

✅ **(c)**

# CONCISE REVISION SHEET (Units 3 & 4)

### Numbers & formulae to recall

|Topic|Fact|
|---|---|
|Symmetric keys (n users)|**n(n−1)/2**; PK:**n key pairs**|
|Key equivalence|**128-bit: RSA 3072 = ECC 256 = AES-128**; 112-bit:|
||RSA 2048 = ECC 224; 256-bit: RSA 15360 = ECC 512|


|Topic|Fact|
|---|---|
|Miller–Rabin|n−1=2^s·d; error ≤ (1/4)^k;**561**Carmichael; a∈[2,<br>n−2]|
|CRT|unique mod M=∏mᵢ(pairwise coprime); x=ΣaᵢMᵢyᵢ|
|DLP attacks|BSGS, rho (√n), Pohlig–Hellman,**index-calculus**<br>**(not on ECC)**|
|GF(2^n)|add=XOR; AES poly**x⁸+x⁴+x³+x+1 (0x11B)**|
|RSA|n=pq;φ=(p−1)(q−1);**ed≡1 modφ**; C=M^e; M=C^d;<br>e=65537 (17 squarings); RSA-CRT ≈**4×**|
|OAEP|P₁=M⊕G(r); P₂=H(P₁)⊕r; C=(P₁∥P₂)^e; r=H(P₁)⊕P₂;<br>M=P₁⊕G(r)|
|DH|A=g^a, B=g^b, K=g^(ab) mod p;**MITM**if<br>unauthenticated|
|ECC|y²=x³+ax+b; addλ=(y₂−y₁)/(x₂−x₁); doubleλ=<br>(3x²+a)/2y; Q=dG; ECDH S=d_AQ_B|
|ECDSA|r=x(kG) mod n; s=k⁻¹(h+dr) mod n; verify<br>X=u₁G+u₂Q, x(X)=r;**nonce reuse = key leak**|
|DSA|r=(g^k mod p) mod q; s=k⁻¹(H+xr) mod q; v=<br>(g^u₁y^u₂ mod p) mod q|
|Signature goals|Authentication, integrity, non-repudiation (**not**<br>confidentiality)|
|Hash security|Pre-image 2^n, 2nd pre-image 2^n,**collision**<br>**2^(n/2)**|
|Birthday|23 people → ~50.7%|
|MD|Hᵢ=f(Hᵢ₋₁,Mᵢ); length padding; f CR⇒H CR; length-<br>extension weakness|
|SHA-256|256-bit,**512-bit block, 64 rounds, 8×32-bit regs**,<br>W₀..W₆₃, Ch/Maj/Σ/σ; blocks=⌈(L+65)/512⌉|
|MD5 / SHA-1|128-bit, 64 steps / 160-bit, 80 steps; both broken|
|Salt|random, per user, public, stored with hash|


|Topic|Fact|
|---|---|
|ZKP|completeness, soundness, zero-knowledge; Fiat–<br>Shamir y²=x·v^b; error 2^−t|
|Homomorphic|Paillier**add**; RSA/ElGamal**multiply**; FHE both<br>(Gentry 2009, lattice)|
|HMAC|H((K⊕opad)∥H((K⊕ipad)∥m)); ipad**0x36**, opad<br>**0x5C**; B=64 (SHA-256)|
|X.509|Version, Serial, Sig alg, Issuer, Validity, Subject,<br>PubKey, Extensions, CA signature|
|Side channels|timing (conditional multiply), power: SPA (visual)<br>vs**DPA**(many traces, 256 guesses/byte)|
|TPM|PCR_new=H(PCR_old∥m); measured boot; sealing;<br>attestation (AK quote + nonce)|
|PUF|SRAM/Arbiter/Ring-osc/Optical; fuzzy extractor;<br>ML attacks on arbiter|
|Quantum|**Shor**breaks RSA/DH/ECC;**Grover**halves<br>symmetric (AES-256 OK)|
|PQC|Lattice (**ML-KEM FIPS 203**,**ML-DSA FIPS 204**),<br>hash-based**SLH-DSA FIPS 205**, code-based<br>(McEliece, HQC)|
|ML-KEM|Module-LWE KEM; n=256, q=3329; 512/768/1024;<br>768: pk 1184 B, ct 1088 B, K 32 B|


### One-line memory hooks

- **Encrypt → receiver public; Sign → sender private.**

- **Hash = no key; MAC = shared key; Signature = key pair.**

- **CRT = speed; OAEP = security; PKI = key authenticity; ECC = small keys; PQC = quantum safety.**

- **Passive (side-channel) vs Active (fault injection).**

- **Collision n/2, pre-image n.**

# 40-MARK MCQ MOCK TEST (covers entire syllabus)

_Time: 40 minutes · 40 questions · 1 mark each · no negative marking assumed. Attempt first, then check the answer key at the very end of this document._

### Section A – Unit 3 (Q1–Q22)

**Q1.** Number of symmetric keys needed for 20 users? (a) 20 (b) 40 (c) 190 (d) 380

**Q2.** A digital signature is created using: (a) the receiver’s public key (b) the signer’s private key (c) the signer’s public key (d) the receiver’s private key

**Q3.** Which function is one-way but has **no** trapdoor? (a) SHA-256 (b) RSA encryption function (c) Rabin squaring modulo pq (d) None of these

**Q4.** When Miller–Rabin outputs 'composite': (a) the number is probably composite (b) it is composite with probability 3/4 (c) it may still be prime (d) the number is definitely composite

**Q5.** The smallest Carmichael number is: (a) 341 (b) 561 (c) 1105 (d) 1729

**Q6.** For CRT with moduli 4, 5, 7 the solution is unique modulo: (a) 16 (b) 28 (c) 35 (d) 140 **Q7.** The irreducible polynomial used for AES arithmetic in GF(2⁸) is: (a) x⁸ + x⁴ + x³ + x + 1 (b) x⁸ + x⁴ + x³ + x² + 1 (c) x⁸ + x⁶ + x⁵ + 1 (d) x⁸ + 1

**Q8.** For RSA with p = 7 and q = 13, φ(n) is: (a) 91 (b) 78 (c) 72 (d) 84

**Q9.** The main advantage of choosing e = 65537 in RSA is: (a) faster decryption (b) harder factoring (c) a smaller private exponent (d) fast public-key operations because the exponent has only two 1-bits

**Q10.** OAEP primarily provides: (a) randomised padding giving semantic/CCA security (b) faster decryption (c) authentication of DH values (d) digital signatures

**Q11.** In OAEP decryption the random value r is recovered as: (a) G(P₂) ⊕ P₁ (b) P₁ ⊕ M (c) H(P₁) ⊕ P₂ (d) G(M)

**Q12.** In RSA-CRT, after h = q_inv·(m_p − m_q) mod p, the plaintext is: (a) m_p + h·p (b) m_q + h·q (c) m_p·m_q (d) h·p + q

**Q13.** Diffie–Hellman with p = 11, g = 2, a = 3, b = 4. The shared key is: (a) 4 (b) 8 (c) 5 (d) 3

**Q14.** Man-in-the-middle attacks on Diffie–Hellman are prevented by: (a) a larger generator (b) a larger secret exponent (c) ephemeral keys alone (d) authenticating the exchanged values (signatures/certificates)

**Q15.** The slope used when doubling a point P = (x₁, y₁) on y² = x³ + ax + b is: (a) (y₂ − y₁)/(x₂ − x₁) (b) (3y² + a)/(2x) (c) (3x₁² + a)/(2y₁) (d) (x₁² + b)/y₁

**Q16.** ECC achieves the same security with smaller keys because: (a) it uses larger primes (b) no sub-exponential (index-calculus-type) attack is known for the ECDLP (c) it is quantum-safe (d) it uses symmetric encryption internally

**Q17.** In ECDSA, the signature component s is computed as: (a) k(h + d·r) (b) k⁻¹(h − d·r) (c) d⁻¹(h + k·r) (d) k⁻¹(h + d·r) mod n

**Q18.** Which statement about DSA is correct? (a) It can only sign, not encrypt (b) Its verification is faster than RSA (c) It needs no random nonce (d) It is based on factoring

**Q19.** In PKI, an end-entity certificate is signed by: (a) the subject itself (b) the relying party (c) the issuing CA (d) the browser vendor

**Q20.** Generic collision-finding effort for a 160-bit hash such as SHA-1 is about: (a) 2¹⁶⁰ (b) 2⁸⁰ (c) 2⁴⁰ (d) 2²⁰

**Q21.** SHA-256 uses: (a) 512-bit blocks, 64 rounds, eight 32-bit working variables (b) 1024bit blocks, 80 rounds (c) 512-bit blocks, 80 rounds (d) 256-bit blocks, 64 rounds

**Q22.** The Merkle–Damgård security proof requires: (a) a keyed compression function (b) parallel block processing (c) a random IV per message (d) a collision-resistant compression function plus length padding

### Section B – Unit 4 (Q23–Q40)

**Q23.** Which is an example of 'something you have'? (a) Password (b) Fingerprint (c) Smart card / hardware token (d) PIN

**Q24.** bcrypt and Argon2 are preferred for password hashing because they are: (a) very fast (b) intentionally slow / memory-hard (c) reversible (d) salt-free

**Q25.** In Fiat–Shamir, if a prover reuses the same r for challenges b = 0 and b = 1, the verifier can compute: (a) p (b) q (c) r only (d) the secret s

**Q26.** The ZKP property that guarantees the verifier learns nothing beyond the truth of the statement is: (a) zero-knowledge (b) completeness (c) soundness (d) binding

**Q27.** Which statement is correct? (a) PHE supports both addition and multiplication (b) FHE supports addition and multiplication, enabling arbitrary computation (c) Paillier is fully homomorphic (d) RSA is fully homomorphic

**Q28.** If an HMAC key is longer than the hash block size, it is: (a) truncated (b) rejected (c) hashed first to obtain K′ (d) used unchanged

**Q29.** Which of the following does HMAC **not** provide? (a) Non-repudiation (b) Integrity (c) Data-origin authentication (to key holders) (d) Resistance to length-extension

**Q30.** In a chain of trust, a leaf certificate’s signature is verified with the public key of: (a) the leaf itself (b) the browser vendor (c) always the root CA (d) its issuer (e.g., an intermediate CA)

**Q31.** Which is an **invasive** physical attack? (a) Timing analysis (b) Simple power analysis (c) Microprobing after chip decapsulation (d) Cache-timing measurement

**Q32.** In DPA, the correct key guess typically shows: (a) a flat difference trace (b) a pronounced spike in the difference-of-means trace (c) purely random output (d) only negative values

**Q33.** RSA blinding is a countermeasure against: (a) factoring (b) man-in-the-middle (c) brute force (d) timing / side-channel attacks

**Q34.** A TPM remote-attestation quote consists of: (a) PCR values signed with an attestation key plus the verifier’s nonce (b) the private Endorsement Key (c) the plaintext disk key (d) the BIOS password

**Q35.** Which TPM component stores boot measurements? (a) Endorsement Key (b) PCRs (c) Storage Root Key (d) the RNG

**Q36.** Arbiter PUFs are particularly vulnerable to: (a) Shor’s algorithm (b) timing attacks (c) machine-learning modelling attacks (d) rainbow tables

**Q37.** Shor’s algorithm gives: (a) polynomial-time factoring and discrete logarithms (b) a square-root speed-up for search (c) exponential-time factoring (d) a break of AES-256

**Q38.** The shared secret produced by ML-KEM (Kyber) is: (a) 16 bytes (b) 24 bytes (c) 64 bytes (d) 32 bytes

**Q39.** In the LWE problem, given (A, b = A·s + e mod q), the goal is to find: (a) e only (b) the secret vector s (c) a factor of q (d) a hash pre-image

**Q40.** The recommended migration approach today is: (a) wait until quantum computers exist (b) just double the RSA modulus size (c) hybrid classical + post-quantum with crypto-agility (d) stop using encryption

# MOCK TEST ANSWER KEY (check only after attempting)

|Q|Ans|Why|
|---|---|---|
|1|**c**|20·19/2 = 190|
|2|**b**|Sign with own private key|
|3|**a**|Hashes are one-way, no trapdoor|
|4|**d**|MR 'composite' is certain|
|5|**b**|561 = 3·11·17|
|6|**d**|4·5·7 = 140|
|7|**a**|x⁸+x⁴+x³+x+1 (0x11B); x⁸+1 is reducible|
|8|**c**|6·12 = 72|
|9|**d**|65537 = 2¹⁶+1 → 16 squarings + 1 multiply|
|10|**a**|Randomised padding, CCA security|
|11|**c**|r = H(P₁)⊕P₂|
|12|**b**|M = m_q + h·q|
|13|**a**|A=8, B=5; K=5³ mod 11 = 4 = 8⁴ mod 11|
|14|**d**|Authenticate DH values; ephemeral alone doesn't<br>stop MITM|
|15|**c**|λ= (3x₁²+a)/(2y₁)|
|16|**b**|Only generic √n attacks apply|
|17|**d**|s = k⁻¹(h+dr) mod n|
|18|**a**|DSA = signatures only|


|Q|Ans|Why|
|---|---|---|
|19|**c**|Issuing CA signs|
|20|**b**|Birthday bound 2^(n/2) = 2⁸⁰|
|21|**a**|SHA-256: 512-bit block, 64 rounds, 8 registers|
|22|**d**|f collision-resistant + MD strengthening⇒H<br>collision-resistant|
|23|**c**|Possession factor|
|24|**b**|Slow/memory-hard KDFs resist GPU cracking|
|25|**d**|y₁/y₀ = s|
|26|**a**|Zero-knowledge property|
|27|**b**|FHE = both operations (Gentry 2009)|
|28|**c**|K′ = H(K)|
|29|**a**|Shared key⇒no non-repudiation|
|30|**d**|Verify with issuer's public key up the chain|
|31|**c**|Microprobing is invasive|
|32|**b**|Spike for correct guess|
|33|**d**|Blinding decorrelates timing from the input|
|34|**a**|Signed PCRs + nonce|
|35|**b**|PCRs|
|36|**c**|ML modelling attacks|
|37|**a**|Polynomial-time; breaks RSA/DH/ECC|
|38|**d**|32-byte shared secret|
|39|**b**|Recover secret s|
|40|**c**|Hybrid + crypto-agility|


**Score guide:** 36–40 excellent · 30–35 good (revise traps) · < 30 re-read the sections for questions you missed.

# SYLLABUS COVERAGE CROSS-CHECK

|Syllabus topic|Where covered|
|---|---|
|Public-key paradigm: symmetric vs<br>asymmetric trade-offs|3.1 (incl. hybrid model, key equivalence)|
|Prime generation, Miller–Rabin|3.2.1–3.2.2|
|Chinese Remainder Theorem|3.2.3 and 3.3.6 (RSA-CRT)|
|Discrete Logarithm Problem|3.2.4|
|Finite fields GF(2^n)|3.2.5 (supplemented – thin in slides)|
|RSA key generation, encryption, OAEP|3.3|
|Diffie–Hellman and MITM|3.4|
|ECC: point addition/doubling, small keys,<br>ECDH, ECDSA|3.5|
|Digital signatures (concept, goals), RSA<br>signatures, ECC/DSA signatures, PKI|3.6|
|Hashes: pre-image & collision resistance,<br>Merkle–Damgård, SHA-256|3.7 (birthday attack, ROM, MD5/SHA-1<br>included)|
|Entity authentication: passwords<br>(salting/hashing), challenge–response, ZKP|4.1|
|Homomorphic encryption, HMAC, X.509|4.2|
|Hardware security – physical attacker model|4.3|
|Side channels: timing, power, DPA|4.4|
|TPM and PUF|4.5|
|PQC: quantum threat, lattice crypto,|4.6|
|Kyber/ML-KEM||


**Slide corrections / notes:** (1) Slide on prime generation states 2^111 mod 223 = 222; correct value is 1 (conclusion unchanged). (2) Slides use 'Kyber/Dilithium'; NIST final names are **ML-KEM / ML-DSA / SLH-DSA** (FIPS 203/204/205, Aug 2024). (3) Slides' birthday probability 50.8% vs exact ≈ 50.7%. (4) Slides describe SHA-1 in detail but SHA-

256 only briefly – supplemented in 3.7.8. **Present in your slides but outside the listed syllabus (brief revision only):** key management & KDC (key life-cycle: generation, derivation, establishment, hierarchy, storage, backup, archival, recovery, usage, change, destruction); blockchain use of hashes/signatures; Lamport OTP; CAPTCHA/biometrics. Treat as optional unless your instructor says otherwise.

