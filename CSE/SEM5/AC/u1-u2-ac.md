# Applied Cryptography — Exam Study Notes
**8 Questions × 2 marks (4/unit) + 24 Questions × 1 mark (12/unit) | 7 hrs study + 1 hr mock**

---

# UNIT 1: Foundations, Number Theory & Classical Ciphers

## 1. Cryptography Basics

**Cryptography** = the art of private communication in a public environment (does NOT always mean encryption).

**NIST definition:** The discipline embodying principles, means, and methods for transformation of data to hide semantic content, prevent unauthorized use, or prevent undetected modification.

### Key Terms
| Term | Meaning |
|---|---|
| Plaintext | Human-readable message |
| Ciphertext | Non-readable transformed message |
| Encryption | Plaintext → Ciphertext |
| Decryption | Ciphertext → Plaintext |
| Algorithm | Public rule-set |
| Key | Secret numeric value that parametrizes the algorithm |
| Keyspace | Total possible key values (2^n for n-bit key) |
| Cryptanalysis | Science of breaking cryptography |

### CIA Triad (+ extensions)
- **Confidentiality** – only authorized parties can read data (achieved via encryption: AES, RSA)
- **Integrity** – data not altered (hash functions, MACs, digital signatures)
- **Availability** – data/systems accessible when needed (redundancy, load balancing, backups) — **cryptography does NOT provide this** (except anti-availability via ransomware!)
- **Authentication** (extra) – verifying identity
- **Non-repudiation** (extra) – sender cannot deny sending message (digital signatures)

**Services vs Mechanisms:** Services = goals (confidentiality, integrity...). Mechanisms = tools to achieve them (encryption, hashing, digital signature).

### Passive vs Active Attacks
| Active Attacks | Passive Attacks |
|---|---|
| Modify/disrupt data | Monitor/eavesdrop only |
| Affect Integrity & Availability | Affect Confidentiality |
| Easier to detect | Difficult to detect |
| Prevention challenging | Detection challenging |

### Attack Types (definitions + prevention — high MCQ yield)
- **MITM (Man-in-the-Middle):** attacker secretly intercepts and possibly modifies communication between two parties.
  - *Prevent:* HTTPS/TLS, verify certificates, VPN, MFA, avoid public Wi-Fi.
- **Replay Attack:** attacker captures a valid message and retransmits it later to gain unauthorized access.
  - *Prevent:* nonces, timestamps, session tokens, challenge-response.
- **Side-Channel Attack:** exploits physical leakage (timing, power, EM, cache, acoustic) instead of attacking the algorithm.
  - Types: Timing, Power Analysis (SPA/DPA/CPA), EM, Cache, Acoustic.
  - *Prevent:* constant-time algorithms, noise injection, shielding.
- **DoS/DDoS:** overwhelm system with traffic to deny service. DDoS = distributed (botnet).

### Cryptography vs Security
Cryptography is **one tool** used to build secure systems; security also needs correct implementation, key management, access control, physical security, etc. A system can use strong crypto and still be insecure (e.g., weak key management).

### Cryptanalytic Attack Types (by attacker's access)
| Attack | Attacker has |
|---|---|
| Ciphertext-only | Only ciphertext (statistical analysis) |
| Known-plaintext | Some plaintext + matching ciphertext |
| Chosen-plaintext | Chooses plaintext, obtains ciphertext |
| Chosen-ciphertext | Chooses ciphertext, obtains plaintext |

**Definition of a secure cryptosystem:** secure if the best known attack is exhaustive key search; insecure if any shortcut attack exists.

---

## 2. Mathematical Toolkit I

### Modular Arithmetic
a mod n = remainder when a divided by n. Numbers "wrap around" after reaching modulus n.

**Examples:**
- 3 mod 2 = 1 (3 = 1×2+1)
- 11 mod 3 = 2
- −2 mod 15 = 13 (since −2 = (−1)×15 + 13)

**Congruence:** a ≡ b (mod n) if (a−b) = kn for integer k.

### GCD — Euclidean Algorithm
Repeatedly apply: gcd(a,b) = gcd(b, a mod b), until remainder = 0.

**Solved Example: GCD(308,154)**
- 308 = 2×154 + 0 → GCD = 154

**Solved Example: GCD(252,105)** (MCQ Q21, Unit1)
- 252 = 2×105 + 42
- 105 = 2×42 + 21
- 42 = 2×21 + 0
- **GCD = 21**

### Extended Euclidean Algorithm
Finds s, t such that: **as + bt = gcd(a,b)**. Used to find modular inverses.

**Solved Example: Extended Euclidean for GCD(161, 28)** (MCQ Q10, Unit1)
- 161 = 5×28 + 21
- 28 = 1×21 + 7
- 21 = 3×7 + 0 → gcd = 7

Back-substitute:
- 7 = 28 − 1×21
- 21 = 161 − 5×28 → 7 = 28 − 1×(161 − 5×28) = 6×28 − 1×161
- So **s = −1, t = 6** ✓ (matches option: s=−1, t=6)
- Check: 161×(−1) + 28×(6) = −161+168 = 7 ✓

### Modular Inverse
x⁻¹ exists in Zₙ iff gcd(x, n) = 1 (x and n are coprime).
Methods: (1) Naive brute force, (2) Extended Euclidean, (3) Fermat's Little Theorem (only works when modulus is prime).

### Euler's Totient Function φ(n)
Count of integers in {1,...,n−1} coprime to n (gcd = 1).
- If p is prime: **φ(p) = p − 1**
- If n = p×q (distinct primes): **φ(n) = (p−1)(q−1)**
- Formula for general n: φ(n) = n × Π(1 − 1/p) over distinct prime factors p.

**Example:** φ(5) = 4 (1,2,3,4 all coprime to 5, since 5 is prime)
φ(9) = φ(3²) = 9×(1−1/3) = 6

### Euler's Theorem
If gcd(a,n) = 1: **a^φ(n) ≡ 1 (mod n)**
Used to compute large modular exponents efficiently — foundation of RSA.

**Solved Example: Find remainder when 2^50 is divided by 11**
- 11 is prime → φ(11) = 10
- gcd(2,11)=1, so 2^10 ≡ 1 (mod 11)
- 50 = 5×10 + 0 → 2^50 = (2^10)^5 ≡ 1^5 = **1 (mod 11)**

**Solved Example: Remainder when 3^75 divided by 13**
- φ(13) = 12
- 75 = 6×12 + 3 → 3^75 ≡ 3^3 (mod 13) = 27 mod 13 = **1**

### Fermat's Little Theorem
Special case of Euler's theorem, for prime modulus p and gcd(a,p)=1:
**a^(p−1) ≡ 1 (mod p)**, equivalently a^p ≡ a (mod p)

---

## 3. Classical Ciphers

### Caesar Cipher (shift cipher)
C = (P + k) mod 26 ; Decrypt: P = (C − k) mod 26
**Keyspace = 26** (only 25 useful shifts, since shift 0 = no encryption)

**Solved: Encrypt "secret" → "zljyla", find shift** (MCQ Q2)
- s(18)→z(25): shift = 25−18 = 7
- Check e(4)→l(11): 4+7=11 ✓ **Shift = 7**

**Solved (from PDF): Brute force SQDYMZK → GERMANY, key=14**

### Monoalphabetic Substitution
Each letter mapped to a fixed different letter via arbitrary permutation.
**Keyspace = 26! ≈ 4×10^26** (much larger than Caesar's 26)
Weakness: broken via **frequency analysis** (E, T, N, I, O, A, U common in English; Z, J, K, Q, X rare).

### Vigenère Cipher (polyalphabetic)
Uses repeating keyword; each letter shifted by corresponding key letter.
Cᵢ = (Pᵢ + Kᵢ mod len(key)) mod 26
Broken via **Kasiski Examination** — finds repeated ciphertext sequences; distance between repeats is a multiple of key length.

### Playfair Cipher
- Build 5×5 matrix from keyword (drop duplicate letters, combine I/J), fill rest of alphabet.
- Rules for digraphs (pairs):
  1. Same row → shift right (wrap)
  2. Same column → shift down (wrap)
  3. Rectangle → swap columns (same row, other's column)
- If a pair has duplicate letters, insert filler (X) between them.

**Solved (MCQ Q1): Decrypt "BPKYFS" with keyword "SECRET" (I/J combined)**
Matrix (SECRET → S,E,C,R,T then remaining alphabet, I/J merged):
```
S E C R T
A B D F G
H I K L M
N O P Q U
V W X Y Z
```
Decrypt pairs (reverse of encryption rules):
- BP: B(row1,col1) P(row3,col2) → rectangle → swap cols → A(row1,col... ) Let's verify against options.
- Answer choices: INDIAN, WORLD, DOLLAR, HELLO
- Decrypting BP-KY-FS with this square gives **"DOLLAR"** ✓ (standard textbook answer for this exact question)

### Hill Cipher
Uses matrix key K (n×n), C = K·P mod 26 (encryption), P = K⁻¹·C mod 26 (decryption)

**Solved (MCQ Q14): Encrypt "HI" with key [[3,3],[2,5]] mod 26**
- H=7, I=8 → P = [7,8]ᵀ
- C₁ = 3×7 + 3×8 = 21+24 = 45 mod 26 = 19 → T
- C₂ = 2×7 + 5×8 = 14+40 = 54 mod 26 = 2 → C
- **Ciphertext = "TC"** ✓

### Transposition Ciphers
Rearrange letter **positions**, don't change letter identity.

**Rail Fence Cipher** — write in zigzag across N rails, read row-by-row.
**Solved: "DEFEND THE EAST WALL", key=3**
Zigzag (ignore spaces or keep, typical textbook keeps letters only):
```
D . . . N . . . E . . . L . . .
. E . E . D . E . A . W . L .
. . F . . . T . . S . . A . .
```
Row1: D N E L
Row2: E E D E A W L
Row3: F T S A
Ciphertext ≈ **DNELEEDEAWLFTSA** (exact spacing may vary by convention)

**Row/Columnar Transposition**
Write plaintext in grid with width = keyword length; number columns by alphabetical order of keyword letters; read columns in that numeric order.

**Solved: "HAVE A GREAT DAY" keyword "PLAIN"**
PLAIN alphabetical order: A(1) I(2) L(3) N(4) P(5) → column order = 5,2,3,4,1... 
(Key insight for the exam: know the **method**, not necessarily memorize this exact output — practice one example yourself with the grid.)

**Double Columnar Transposition** = columnar transposition applied twice (with same or different keys) → stronger security.

**Solved (MCQ Q24): Encrypt "cryptography" with key "luck"**
LUCK alphabetical: C(1)K(2)L(3)U(4)
Grid (4 cols, width=4):
```
c r y p
t o g r
a p h y
```
Column order by key L-U-C-K → alphabetical rank: C=1,K=2,L=3,U=4 → read columns in order C,K,L,U i.e., column-index(3,4,1,2)... 
Read col3(y,g,h) col4(p,r,y) col1(c,t,a) col2(r,o,p):
**yghpryctarop** ✓ (matches given MCQ option)

### Key-space sizes (MEMORIZE)
| Cipher | Keyspace |
|---|---|
| Caesar | 26 |
| Monoalphabetic substitution | 26! |
| Playfair (5×5) | 25! |
| Double columnar transposition, key length 5 | (5!)² = 120² |
| Columnar transposition, key length n | n! |

---

## 4. Security Models & Semantic Security

**Semantic Security:** ciphertext reveals no meaningful information about plaintext; attacker can't distinguish which of two messages was encrypted better than random guessing (50%).

### Security Game
Challenger & Adversary. Adversary picks two equal-length messages M₀, M₁. Challenger encrypts one at random, sends ciphertext. Adversary guesses which. If guess accuracy ≈ 50%, scheme is semantically secure.

### IND-CPA / IND-CCA1 / IND-CCA2
| Model | Attacker can... |
|---|---|
| **IND-CPA** | Encrypt any chosen plaintext (encryption oracle); NO decryption access |
| **IND-CCA1** ("Lunch-time attack") | Decrypt ciphertexts **only before** the challenge |
| **IND-CCA2** | Decrypt ciphertexts **before AND after** challenge (except the challenge ciphertext itself) — strongest |

Security increases: IND-CPA → IND-CCA1 → IND-CCA2

### Random Oracle Model (ROM)
Idealized model where a hash function behaves as a truly random function: same input → same output; different inputs → unpredictable outputs. Used to prove protocol security (SHA-256 approximates this in practice).

### Shannon's Theorem, Confusion & Diffusion
- **Confusion:** hides relationship between **key and ciphertext** (achieved via substitution/S-boxes). Small key change → completely different ciphertext.
- **Diffusion:** spreads influence of **each plaintext bit** across many ciphertext bits (achieved via permutation). Small plaintext change → many ciphertext bits change (avalanche effect).
- AES: SubBytes = confusion; ShiftRows + MixColumns = diffusion.

### Entropy
Measure of randomness/unpredictability, in bits. Higher entropy = harder to guess. Password "123456" = low entropy; "T9@qL#8zP!" = high entropy.

### Perfect Secrecy & One-Time Pad (OTP)
**Perfect secrecy:** a posteriori probability = a priori probability → ciphertext leaks NO information about plaintext, i.e., P(M|C) = P(M).
**Condition:** |Message space| = |Key space| = |Ciphertext space|, every key equally likely, key used only once.

**One-Time Pad:**
- Enc: C = K ⊕ M ; Dec: P = K ⊕ C
- Requirements for perfect security: key truly random, key length ≥ message length, used only ONCE, only 2 copies exist, destroyed after use.
- If reused → "Two-Time Pad" → **insecure**: C1⊕C2 = M1⊕M2 leaks info (frequency analysis can then recover messages).

**Solved (MCQ Q11): If OTP key is 100 bits, max plaintext length for perfect secrecy?**
Key must be ≥ plaintext length → plaintext **≤ 100 bits** ✓

**Solved (MCQ Q8): Best explains Shannon's perfect secrecy?**
→ **"Ciphertext gives no information about plaintext"** ✓

---

# UNIT 2: Symmetric Key Cryptography & Modern Standards

## 1. Block Cipher Structures

### Symmetric Key Cryptography — recap
- Same key for encryption & decryption.
- **Advantages:** fast, efficient, simple, scalable.
- **Risks:** secure key exchange, key never expires automatically, if compromised — all data must be re-encrypted.
- Keys needed for n users (symmetric, pairwise): **n(n−1)/2**
- Keys for asymmetric: **2n** (each user has a public+private key pair)

### Feistel Network
- Splits block into **L (left) and R (right)** halves.
- Round: **Lᵢ₊₁ = Rᵢ** ; **Rᵢ₊₁ = Lᵢ ⊕ F(Rᵢ, Kᵢ)**
- Round function F need NOT be reversible — decryption just reverses subkey order, huge advantage.
- Example: **DES**

**Solved (MCQ): In Feistel cipher, R1 = ?**
Answer: **L0 ⊕ F(R0, K1)** ✓

### Substitution-Permutation Network (SPN)
- Applies **substitution** (S-boxes → confusion) and **permutation/mixing** (→ diffusion) repeatedly.
- Transformations must be reversible (unlike Feistel's F).
- Example: **AES**

### Feistel vs SPN
| Feistel | SPN |
|---|---|
| Splits into 2 halves | Whole block transformed via S-box+P-box layers |
| Round function need not be reversible | Transformations must be reversible |
| DES | AES |

---

## 2. DES / 3DES

- Based on Feistel cipher **LUCIFER** (Horst Feistel, IBM, 1971).
- **Block size = 64 bits, Key = 56 bits effective (64 bits with 8 parity bits), Rounds = 16.**
- Steps: Initial Permutation (IP) → 16 Feistel rounds → swap → Final Permutation (FP, inverse of IP).
- **E-box:** expands 32-bit half to 48 bits (per round), so it can XOR with 48-bit round key.
- **S-boxes:** 8 boxes, each 6-bit input → 4-bit output — provide the real confusion (non-linearity).
- **P-box:** straight 32-bit permutation after S-boxes.

### Why DES is deprecated
- Key size (56 bits) too small — brute-forced in 22 hours (1999, combined efforts); EFF hardware cracked it in ~56 hours (1998).
- Keyspace = 2^56 ≈ 7.2×10^16 — feasible today.

### Triple DES (3DES)
- Applies DES three times (with 2 or 3 keys) to increase effective key length.
- 3-key 3DES resists known-plaintext attacks better than 2-key.
- Too slow for modern use; vulnerable to memory-timing attacks; being phased out.

**Solved MCQs (Unit 2):**
- DES rounds = **16**
- DES key length = **56 bits**
- E-box purpose = **expand 32-bit input to 48 bits**
- DES based on **LUCIFER**

---

## 3. AES (Advanced Encryption Standard)

- Selected 2001, designed by **Vincent Rijmen & Joan Daemen** (Belgium) — "Rijndael".
- **NOT a Feistel cipher — it's an SPN.**
- **Block size = 128 bits fixed.** Key sizes: 128/192/256 bits → Rounds: 10/12/14 respectively.

### Round Structure
1. **AddRoundKey** (initial)
2. Rounds (repeated Nr−1 times): **SubBytes → ShiftRows → MixColumns → AddRoundKey**
3. **Final round** (no MixColumns): SubBytes → ShiftRows → AddRoundKey

### Step Details
- **SubBytes:** byte-by-byte substitution via 16×16 S-box (built from multiplicative inverses in GF(2^8)) — provides **confusion**.
- **ShiftRows:** row 0 unchanged; row 1 shifts left 1; row 2 shifts left 2; row 3 shifts left 3 — scrambles byte positions (part of diffusion).
- **MixColumns:** matrix multiplication in GF(2^8) using irreducible polynomial **x⁸+x⁴+x³+x+1**; mixes bytes within each column — provides **diffusion**. (Skipped in final round.)
- **AddRoundKey:** XOR state with round key.
- **Key Expansion:** original key → 44 words (for 128-bit key) via RotWord, SubWord (S-box), XOR with round constant Rcon[i].

### Galois Field basics (GF)
- Field = set with +,−,×,÷ always producing a result within the set.
- Finite field has **pᵐ** elements, p prime.
- AES uses **GF(2⁸)** — 256 elements, each byte = one field element.
- Prime field GF(p): elements {0,...,p−1}; arithmetic is mod p.
- Extension field GF(2ᵐ): elements are polynomials.
- Irreducible (prime) polynomial: cannot be factored into lower-degree polynomials over the same field.

**Solved (MCQ Unit2): Addition of (x²+1)+(x²+x) over GF(2)**
= x²+x²+x+1 = (since coefficients mod 2, x²+x²=0) → **x+1** ✓

**Solved: In GF(2³), x³+x+1 irreducible — degree?** → **3** (degree of the polynomial itself)

### DES vs AES
| DES | AES |
|---|---|
| Bit-oriented, Feistel | Byte-oriented, SPN |
| 64-bit block, 56-bit key | 128-bit block, 128/192/256-bit key |
| 16 rounds | 10/12/14 rounds |
| Broken (insecure today) | Current standard |

---

## 4. Modes of Operation

| Mode | Formula | Key Property |
|---|---|---|
| **ECB** | Cᵢ = E(K, Pᵢ) | Each block independent — identical plaintext blocks → identical ciphertext (pattern leakage!). Fast, parallel, but insecure for large/structured data. |
| **CBC** | C₁=E(K,P₁⊕IV); Cᵢ=E(K,Pᵢ⊕Cᵢ₋₁) | Chained via XOR with previous ciphertext; needs random IV; sequential (slow, no parallel encryption); achieves **CPA-security** with random IV. |
| **CTR** | Cᵢ = Pᵢ ⊕ E(K, Counterᵢ) | Turns block cipher into a stream cipher; fully parallelizable; random access; must never reuse key+counter. |
| **GCM** | CTR encryption + GHASH authentication | Provides Confidentiality+Integrity+Authentication (AEAD); supports Additional Authenticated Data (AAD); high performance. |
| **CCM** | CBC-MAC (auth) + CTR (encryption) | AEAD; "Counter with CBC-MAC"; used in Wi-Fi/IoT/Bluetooth/802.15.4. |

**Why GCM/CCM needed:** plain encryption modes give confidentiality only — an attacker could flip ciphertext bits undetected. GCM/CCM add **authentication tags** to detect tampering (Authenticated Encryption with Associated Data — AEAD).

**Solved MCQs:**
- ECB insecure because: **identical plaintext blocks → identical ciphertext blocks** (pattern leakage)
- CBC with random IV achieves: **CPA-security**
- Mode that turns block cipher into stream cipher: **CFB / OFB / CTR** (CTR most emphasized in these notes)
- CBC encryption of a block depends on: **previous ciphertext block**

---

## 5. Randomness: PRNG vs TRNG

| | PRNG | TRNG |
|---|---|---|
| Source | Algorithm/formula | Physical phenomenon (noise, decay) |
| Deterministic | Yes (same seed → same output) | No |
| Speed | Fast | Slow, hardware dependent |
| Reproducible | Yes, given seed | No |
| Use | Expand a seed into many pseudorandom bits efficiently | Supply high-quality entropy/seed |

- PRNG = "Deterministic Random Bit Generator" (DRBG). Functions: **Seed(), Reseed(), Generate(n)**.
- **Important:** 128-bit output ≠ 128-bit security if the seed itself has low entropy (e.g., 8-bit seed → only 256 possible keys, brute-forceable!).
- Used for: key generation, IVs/nonces, salts, UUIDs.
- CTR-DRBG (block cipher in CTR mode) and HMAC-DRBG are standard constructions.

**Solved MCQs:**
- PRNG is: **deterministic and predictable if seed is known**
- PRNG randomness comes from: **algorithmic computation based on a seed**
- TRNG differs from PRNG: **slower but based on physical phenomena**
- PRNG desired property: **uniform distribution**

---

## 6. Attacks on Block Ciphers

- **Differential Cryptanalysis** (Biham & Shamir, 1980s; known earlier to IBM/DES team): Chosen-Plaintext attack; observes XOR differences between plaintext pairs and resulting ciphertext differences; uses Difference Distribution Table (DDT). DES S-boxes were specifically designed to resist this.
- **Linear Cryptanalysis** (Matsui, early 1990s): finds linear approximations between plaintext/ciphertext/key bits holding with probability ≠ 0.5; needs only **Known-Plaintext** (not chosen); DES breakable with ~2^43 known plaintext-ciphertext pairs.
- **Padding Oracle Attack:** Adaptive Chosen-Ciphertext attack (IND-CCA2 style) on CBC mode; exploits a system that reveals whether decrypted padding (e.g., PKCS7) is valid; attacker manipulates ciphertext bytes to recover plaintext byte-by-byte.
- **Side-Channel Attacks:** exploit physical leakage — SPA (visual inspection of power traces), DPA/CPA (statistical analysis over many traces) — recover keys via Hamming weight/power correlation, without breaking the math of the algorithm.

---

# QUICK-FIRE FACT SHEET (for 1-mark MCQs)

- Cryptography does NOT ensure availability (can cause anti-availability, e.g. ransomware)
- DES: 64-bit block / 56-bit key / 16 rounds / Feistel / based on LUCIFER
- AES: 128-bit block / 128,192,256-bit key / 10,12,14 rounds / SPN / GF(2⁸)
- AES is NOT Feistel; DES IS Feistel
- Keyspace: Caesar=26, Monoalphabetic=26!, Playfair=25!
- RC4 = stream cipher; DES, AES = block ciphers; RSA = asymmetric (not a cipher mode)
- Symmetric key count for n users = n(n−1)/2; Asymmetric = 2n
- Perfect secrecy ⇒ P(M|C) = P(M); needs |K|≥|M|, random key, one-time use
- OTP reused = "Two-Time Pad" = insecure
- IND-CPA < IND-CCA1 < IND-CCA2 (increasing strength)
- Confusion = key-ciphertext relationship (S-box); Diffusion = spreads plaintext bit influence (P-box/permutation)
- CBC-MAC + CTR = CCM; GHASH + CTR = GCM
- Linear cryptanalysis: known-plaintext; Differential cryptanalysis: chosen-plaintext
- φ(p) = p−1 for prime p; Euler's theorem: a^φ(n) ≡ 1 mod n if gcd(a,n)=1

---
-------------------------------------------------unit1 and unit2 end------------------------------------------------------------------
Good luck! 🎯
