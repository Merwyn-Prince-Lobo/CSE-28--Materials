# Applied Crypto: All 20 Numerical Types, Solved in Depth (Units 3 & 4)

Legend: 🔑 = formula to memorise | ⚠ = trap | ✅ = check step

---

## 1. Key count (symmetric vs public-key)

🔑 Symmetric keys for n users: **n(n-1)/2**. Public-key: **n key pairs** (2n keys).

**Example:** 50 users.
- 50 x 49 / 2 = 2450 / 2 = **1225**
- PK: 50 pairs

| n | symmetric | PK pairs |
|---|---|---|
| 10 | 45 | 10 |
| 100 | 4,950 | 100 |
| 1000 | 499,500 | 1000 |

⚠ n(n-1) without halving = wrong. Symmetric is quadratic, PK is linear.

---

## 2. Miller-Rabin

🔑 Write n-1 = 2^s * d, d odd. Pick a in [2, n-2]. x = a^d mod n.
- Pass if x = 1 or x = n-1.
- Else square up to s-1 times; pass if x ever becomes n-1.
- If never: **composite for sure**.
- Error after k rounds: **(1/4)^k**. k=5 -> 1/1024.

**Step 1: find s and d.** n = 97: n-1 = 96 = 2 x 48 = 4 x 24 = 8 x 12 = 16 x 6 = 32 x 3. So **s = 5, d = 3**.

**Step 2: run a = 2 on 97.**
- x0 = 2^3 = 8
- x1 = 8^2 = 64
- x2 = 64^2 = 4096 mod 97 = 4096 - 4074 = 22
- x3 = 22^2 = 484 mod 97 = 484 - 388 = **96 = n-1** -> passes -> probably prime (it is prime).

**Example: n = 29, a = 2.** 28 = 2^2 x 7 (s=2, d=7). 2^7 = 128 mod 29 = 12. 12^2 = 144 mod 29 = **28 = n-1** -> passes.

**Example: n = 561 (Carmichael), a = 2.** 560 = 2^4 x 35 (s=4, d=35).
- 2^35 mod 561 = 263
- 263^2 = 69169 mod 561 = 166
- 166^2 = 27556 mod 561 = 67
- 67^2 = 4489 mod 561 = **1**
- Never hit 560 -> **composite**. (561 = 3 x 11 x 17, fools Fermat, not Miller-Rabin.)

⚠ "Composite" is certain. "Prime" is probable. Error bound is (1/4)^k, not (1/2)^k.

---

## 3. Chinese Remainder Theorem

🔑 Moduli must be **pairwise coprime**. Solution unique mod M = m1 x m2 x ...
x = sum of a_i * M_i * y_i, where M_i = M/m_i and y_i = M_i^-1 mod m_i.

**Example A:** x = 2 (mod 3), x = 3 (mod 5), x = 2 (mod 7).
- M = 105. M1 = 35, M2 = 21, M3 = 15.
- y1 = 35^-1 mod 3 = 2^-1 mod 3 = 2
- y2 = 21^-1 mod 5 = 1^-1 = 1
- y3 = 15^-1 mod 7 = 1^-1 = 1
- x = 2*35*2 + 3*21*1 + 2*15*1 = 140 + 63 + 30 = 233
- 233 mod 105 = **23**
- ✅ 23 mod 3 = 2, 23 mod 5 = 3, 23 mod 7 = 2

**Example B:** x = 1 (mod 3), x = 2 (mod 5).
- M = 15, M1 = 5, M2 = 3. y1 = 5^-1 mod 3 = 2. y2 = 3^-1 mod 5 = 2.
- x = 1*5*2 + 2*3*2 = 10 + 12 = 22 mod 15 = **7**
- ✅ 7 mod 3 = 1, 7 mod 5 = 2

**MCQ shortcut:** plug each option into EVERY congruence. Only one survives.

⚠ Moduli 6, 10, 15 are NOT pairwise coprime (gcd(6,10)=2), so basic CRT fails. 4, 5, 7 -> unique mod 140.

---

## 4. Discrete log (small brute force)

🔑 Given g, p, y = g^x mod p, find x.

**Example A:** 2^x = 9 (mod 11). Powers of 2: 2, 4, 8, 16->5, 10, 20->9. That's x = 1..6, so **x = 6**.

**Example B:** 3^x = 13 (mod 17). 3, 9, 27->10, 30->13. So **x = 4**.

Generic attacks (any group): brute force, BSGS, Pollard rho (about sqrt of group order), Pohlig-Hellman (reduces to largest prime factor of the order).
**Index-calculus**: NOT generic, works in Zp* and GF(2^m)*, **fails on elliptic curves**. That's why ECC keys are small.

---

## 5. GF(2^n) arithmetic

🔑 Addition = subtraction = **XOR**. Multiplication = polynomial multiply, then reduce mod irreducible polynomial. AES: **x^8 + x^4 + x^3 + x + 1 (0x11B)**.

**Addition:** 101 xor 011 = **110**. (x^2+1) + (x+1) = x^2 + x.

**Multiplication in GF(2^3), P(x) = x^3 + x + 1:** (x^2+x+1)(x+1)
- Expand: x^3 + x^2 + x + x^2 + x + 1 = x^3 + 1 (the x^2 and x terms cancel in pairs)
- x^3 = x + 1, so x^3 + 1 = (x+1) + 1 = **x** = 010

**xtime (multiply by 02) in AES:** shift left 1. If the bit shifted out was 1, XOR 0x1B.
- 0x57 -> 0xAE (top bit was 0, no XOR)
- 0xAE -> shift = 0x15C -> keep 8 bits 0x5C -> XOR 0x1B = **0x47**
- FIPS-197: {57} x {83} = {C1}

⚠ Z_256 is NOT a field. In GF(2^n), x + x = 0.

---

## 6. RSA keygen, encrypt, decrypt, sign

🔑 n = pq. phi = (p-1)(q-1). gcd(e, phi) = 1. **d = e^-1 mod phi**. C = M^e mod n. M = C^d mod n.

**Full example: p = 11, q = 17, e = 7.**
- n = 187, phi = 10 x 16 = 160
- d: need 7d = 1 mod 160. 7 x 23 = 161 = 160 + 1. So **d = 23**
- Public (187, 7). Private 23.

**Encrypt M = 88 (square-and-multiply, 7 = 111 in binary):**
- r = 88
- bit 1: square 88^2 = 7744 mod 187 = 77; multiply 77 x 88 = 6776 mod 187 = 44
- bit 1: square 44^2 = 1936 mod 187 = 66; multiply 66 x 88 = 5808 mod 187 = **11**
- C = 11

**Decrypt 11^23 mod 187:** 23 = 16 + 4 + 2 + 1.
- 11^2 = 121, 11^4 = 121^2 = 14641 mod 187 = 55, 11^8 = 55^2 = 3025 mod 187 = 33, 11^16 = 33^2 = 1089 mod 187 = 154
- 154 x 55 = 8470 mod 187 = 55; 55 x 121 = 6655 mod 187 = 110; 110 x 11 = 1210 mod 187 = **88** ✅

**Quick d-checks:**
- p=5, q=11, e=3: phi = 40. 3 x 27 = 81 = 2x40 + 1, so **d = 27**
- p=7, q=13: phi = 6 x 12 = **72** (not 91, not 84)

**Small encryption:** n = 33, e = 3, M = 4. 4^3 = 64. 64 mod 33 = **31**. (Don't forget to reduce.)

**Signature:** n=187, e=7, d=23, H(m)=10. s = 10^23 mod 187: 10^2=100, 10^4=89, 10^8=67, 10^16=1. 1 x 89 x 100 x 10 = 89000 mod 187 = **175**. Verify: 175^7 mod 187 = 10 ✅

**Cost of e = 65537 = 2^16 + 1:** binary has two 1-bits -> **16 squarings + 1 multiply**. That's why encrypt/verify is fast and decrypt/sign (big d) is slow.

⚠ d is inverse mod **phi(n)**, not mod n. gcd(e, phi) = 1, not gcd(e, n).

---

## 7. RSA-CRT

🔑 Precompute dp = d mod (p-1), dq = d mod (q-1), qinv = q^-1 mod p.
mp = C^dp mod p, mq = C^dq mod q, h = qinv(mp - mq) mod p, **M = mq + h*q**.

**Example: n=187, p=17, q=11, d=23, C=11.**
- dp = 23 mod 16 = **7**
- dq = 23 mod 10 = **3**
- qinv = 11^-1 mod 17: 11 x 14 = 154 = 9x17 + 1 -> **14**
- mp = 11^7 mod 17: 11^2 = 121 mod 17 = 2; 11^4 = 4; 11^7 = 4 x 2 x 11 = 88 mod 17 = **3**
- mq = 11^3 mod 11 = **0**
- h = 14 x (3 - 0) = 42 mod 17 = **8**
- M = 0 + 8 x 11 = **88** ✅ (same as direct)

**Speedup:** two exponentiations with half-size modulus and exponent. Cost scales roughly with size^3, so 2 x (1/2)^3 = 1/4 -> **about 4x faster**.

⚠ Only private-key ops (decrypt/sign) speed up. Public op with small e gains nothing. A fault in one half can leak a factor (Bellcore attack).

---

## 8. OAEP

🔑 Encrypt: P1 = M xor G(r), P2 = H(P1) xor r, C = (P1 || P2)^e mod n.
Decrypt: P = C^d, **r = H(P1) xor P2**, **M = P1 xor G(r)**.

**Toy bit example (G and H are made up for illustration):** M = 11001100, r = 1010, G(r) = r repeated = 10101010, H(P1) = xor of the two nibbles of P1.
- P1 = 11001100 xor 10101010 = 01100110
- H(P1) = 0110 xor 0110 = 0000
- P2 = 0000 xor 1010 = 1010
- Decrypt: r = H(P1) xor P2 = 0000 xor 1010 = 1010 ✅. M = 01100110 xor 10101010 = 11001100 ✅

**Max message length:** k - 2*hLen - 2 bytes.
2048-bit RSA (k = 256 B) with SHA-256 (hLen = 32): 256 - 64 - 2 = **190 bytes**.

⚠ OAEP is for **encryption** (randomised, CCA-secure). Signatures use PSS. OAEP = security, CRT = speed.

---

## 9. Diffie-Hellman (and MITM)

🔑 A = g^a, B = g^b, **K = g^(ab) mod p**. Alice: K = B^a. Bob: K = A^b.

**Example 1: p=11, g=2, a=3, b=4.** (the one you missed)
- A = 2^3 = 8. B = 2^4 = 16 mod 11 = 5.
- Alice: B^a = 5^3 = 125 mod 11 = **4**
- Bob: A^b = 8^4 = 4096 mod 11 = **4** ✅
- ⚠ 8 is Alice's PUBLIC value, not the key.

**Example 2: p=23, g=5, a=6, b=15.**
- A = 5^6 mod 23 = 8. B = 5^15 mod 23 = 19.
- Alice: 19^6 mod 23. 19 = -4, (-4)^6 = 4096 mod 23 = **2**
- Bob: 8^15 mod 23 = **2** ✅

**MITM (p=11, g=7):** Alice a=3 -> A=2. Bob b=6 -> B=4. Mallory m=4 -> M=7^4 mod 11 = 3.
- Mallory sends M=3 to both sides.
- Alice-Mallory key: 3^3 mod 11 = **5** (= 2^4 mod 11 = 5 ✅)
- Bob-Mallory key: 3^6 mod 11 = **3** (= 4^4 mod 11 = 3 ✅)
- Two different keys, Mallory sits in the middle. Fix: sign the DH values / certificates.

⚠ K = g^(ab), not g^(a+b). a and b are never sent. DH has no built-in authentication.

---

## 10. Elliptic curve point arithmetic

🔑 Curve y^2 = x^3 + ax + b mod p.
- Addition (P != Q): **lambda = (y2-y1)/(x2-x1)**
- Doubling: **lambda = (3x1^2 + a)/(2y1)** (only "a" appears, never b)
- x3 = lambda^2 - x1 - x2. y3 = lambda(x1 - x3) - y1.
- Division = multiply by modular inverse. P + (-P) = O, where -P = (x, -y).

**Curve y^2 = x^3 + 2x + 2 mod 17, G = (5,1).**

**2G (doubling):**
- lambda = (3*25 + 2)/(2*1) = 77/2. 77 mod 17 = 9. 2^-1 mod 17 = 9. lambda = 9 x 9 = 81 mod 17 = **13**
- x3 = 13^2 - 5 - 5 = 169 - 10 = 159 mod 17 = **6**
- y3 = 13(5 - 6) - 1 = -14 mod 17 = **3**
- **2G = (6,3)**

**3G = G + 2G (addition):**
- lambda = (3-1)/(6-5) = **2**
- x3 = 4 - 5 - 6 = -7 mod 17 = **10**
- y3 = 2(5 - 10) - 1 = -11 mod 17 = **6**
- **3G = (10,6)**

✅ Check (10,6) is on the curve: 10^3 + 20 + 2 = 1022 mod 17 = 2. And 6^2 = 36 mod 17 = 2. ✅

**Double-and-add for 13G:** 13 = 1101 in binary.
- start G -> bit 1: double = 2G, add G = 3G
- bit 0: double = 6G
- bit 1: double = 12G, add G = **13G = (16,4)**

**ECDH on this curve:** dA = 2, dB = 3. QA = 2G = (6,3), QB = 3G = (10,6). Shared S = dA*QB = dB*QA = 6G = **(16,13)**. Use x-coordinate through a KDF.

⚠ Doubling doubles the tangent slope with "a" only. Point addition is not adding coordinates.

---

## 11. ECDSA / DSA sign, verify, nonce reuse

🔑 **ECDSA:** r = x(kG) mod n. **s = k^-1 (h + d*r) mod n**. Verify: w = s^-1, u1 = h*w, u2 = r*w, X = u1*G + u2*Q, valid iff x(X) = r mod n.
**DSA:** r = (g^k mod p) mod q. s = k^-1 (H + x*r) mod q. Verify: w = s^-1 mod q, u1 = H*w, u2 = r*w, v = ((g^u1 * y^u2) mod p) mod q, valid iff v = r.

**Toy DSA: p=23, q=11, g=4, private x=3, public y = 4^3 mod 23 = 18, nonce k=7, H=5.**
- 4^7 mod 23: 4^2=16, 4^4=3, 4^7 = 3 x 16 x 4 = 192 mod 23 = 8. **r = 8 mod 11 = 8**
- k^-1 mod 11: 7 x 8 = 56 = 1 mod 11, so 8
- s = 8 x (5 + 3x8) = 8 x 29 = 232 mod 11 = **1**
- Signature (8, 1)

**Verify:** w = 1^-1 = 1. u1 = 5, u2 = 8.
- g^5 = 4^5 mod 23 = 12. y^8 = 18^8 mod 23 = 16 (18 = -5, 5^8 = 2^4 = 16)
- 12 x 16 = 192 mod 23 = 8. v = 8 mod 11 = **8 = r** ✅

**Nonce reuse attack (same k, two messages), using the same toy numbers.** Second message H2 = 7: s2 = 8 x (7 + 24) = 248 mod 11 = 6. So (r=8, s1=1, H1=5) and (r=8, s2=6, H2=7).
- Subtract: s1 - s2 = k^-1 (H1 - H2). So **k = (H1 - H2)/(s1 - s2)** = (-2)/(-5) = 2 x 5^-1 = 2 x 9 = 18 mod 11 = **7** ✅
- Then **x = (s1*k - H1)/r** = (7 - 5)/8 = 2 x 8^-1 = 2 x 7 = 14 mod 11 = **3** ✅ private key recovered.

⚠ Nonce k must be secret, random, and unique. Reuse = private key leak (Sony PS3). Fix: RFC 6979 deterministic nonce / EdDSA.

---

## 12. Hash numericals (collision bound, birthday, block count)

🔑 For an n-bit hash: pre-image **2^n**, 2nd pre-image **2^n**, collision **2^(n/2)**.

| Hash | bits | collision (generic) |
|---|---|---|
| 64-bit toy | 64 | 2^32 |
| MD5 | 128 | 2^64 |
| SHA-1 | 160 | **2^80** |
| SHA-256 | 256 | **2^128** |

**Birthday:** P(no match among k people) = product of (1 - i/365), i = 1..k-1. For k = 23 that's about 0.4927, so P(match) = **about 50.7%**. Rough rule: collisions at about 1.18 x sqrt(N).

**SHA-256 block count:** 🔑 **blocks = ceil((L + 65) / 512)**. (1 bit "1" + 64-bit length are mandatory.)
- L = 1000: (1000 + 65)/512 = 2.08 -> **3 blocks**
- L = 447: 512/512 = 1 -> **1 block**
- L = 448: 513/512 = 1.002 -> **2 blocks**
- L = 0 (empty): 65/512 -> 1 block

SHA-256: 512-bit block, 64 rounds, 8 x 32-bit registers (a-h). SHA-512: 1024-bit block, 80 rounds. MD5: 64 steps. SHA-1: 80 steps.

---

## 13. Key-size equivalence

🔑 Memorise this table:

| Security | Symmetric | RSA/DH | ECC |
|---|---|---|---|
| 80 | - | 1024 | 160 |
| 112 | 3DES | 2048 | 224 |
| **128** | AES-128 | **3072** | **256** |
| 192 | AES-192 | 7680 | 384 |
| 256 | AES-256 | 15360 | 512 |

Ratio at 128-bit: 3072 / 256 = **12x** smaller for ECC.
⚠ 256-bit ECC ~ AES-128 (NOT AES-256). 2048-bit RSA ~ 112-bit.

---

## 14. Fiat-Shamir identification

🔑 Setup: n = pq, secret s, public **v = s^2 mod n**.
Round: prover sends x = r^2. Verifier sends bit b. Prover sends y = r * s^b. Check **y^2 = x * v^b (mod n)**.

**n = 77, s = 10:** v = 100 mod 77 = **23**.
- Pick r = 5, x = 25.
- **b = 0:** y = 5. Check 5^2 = 25 = x ✅
- **b = 1:** y = 5 x 10 = 50. y^2 = 2500 mod 77 = 36 (77 x 32 = 2464). x*v = 25 x 23 = 575 mod 77 = 36 (77 x 7 = 539). ✅ 36 = 36

**Cheating probability:** a cheater without s guesses b correctly with prob 1/2 each round. After t rounds: **2^-t**. t = 10 -> 1/1024. t = 20 -> about 1 in a million.

**Reusing r leaks s:** if r is reused for b=0 and b=1, y0 = r = 5 and y1 = rs = 50. So **s = y1 / y0 = 10** 💀

---

## 15. Homomorphic encryption

🔑 Dec(c1 * c2) = m1 (op) m2.
- **Textbook RSA = multiplicative**
- **ElGamal = multiplicative**
- **Paillier = additive**
- **FHE = both** (Gentry 2009, lattice, bootstrapping)

**RSA, n = 33, e = 3:**
- E(2) = 2^3 = 8
- E(4) = 4^3 = 64 mod 33 = 31
- E(2) x E(4) = 8 x 31 = 248 mod 33 = **17** (33 x 7 = 231)
- E(2 x 4) = E(8) = 8^3 = 512 mod 33 = **17** (33 x 15 = 495) ✅ equal -> multiplicative

**Paillier toy: n = 15 (p=3, q=5), g = n+1 = 16, n^2 = 225.** Enc(m, r) = g^m * r^n mod n^2.
- Enc(2, r=2) = 16^2 x 2^15 mod 225 = 31 x 143 mod 225 = **158**
- Enc(3, r=2) = 16^3 x 2^15 mod 225 = 46 x 143 mod 225 = **53**
- Product: 158 x 53 = 8374 mod 225 = **49**
- Enc(5, r=4) = 16^5 x 4^15 mod 225 = 76 x 199 mod 225 = **49** ✅
- Multiplying ciphertexts added plaintexts (2 + 3 = 5). Additive!

⚠ Paillier = **P**lus. RSA/ElGamal = times.

---

## 16. HMAC

🔑 **HMAC(K, m) = H( (K' xor opad) || H( (K' xor ipad) || m ) )**
- **ipad = 0x36, opad = 0x5C** (repeated B times)
- Block size **B = 64 bytes** for MD5/SHA-1/SHA-256. **128 bytes** for SHA-384/512.
- Output length = hash output: HMAC-SHA256 = **256 bits (32 bytes)**.

**Key handling for HMAC-SHA256 (B = 64):**
- Key 20 bytes: zero-pad to 64 bytes -> K'.
- Key 100 bytes (> B): K' = H(K) = 32 bytes, zero-pad to 64.
- Key exactly 64 bytes: used as is.

**Two hash calls:** inner = H((K' xor ipad) || m), outer = H((K' xor opad) || inner). Nested structure defeats **length extension**, unlike H(K || m).

⚠ ipad 0x36 (inner), opad 0x5C (outer). No non-repudiation (both sides have K). No confidentiality.

---

## 17. Power analysis (DPA / SPA count)

🔑 DPA attacks **one key byte at a time**. 1 byte = 8 bits = **256** guesses.

- AES-128 key = **16 bytes**
- Total = 16 x 256 = **4096** guesses (vs 2^128 brute force)
- AES-256 key = 32 bytes -> 32 x 256 = 8192

🗣️ Say it: "sixteen bytes, two-fifty-six each, forty-ninety-six."

**Procedure:** collect N traces with known plaintexts -> for each key-byte guess, predict one bit of Sbox(plaintext xor guess) -> split traces by the predicted bit -> difference of means -> **correct guess = spike**, wrong = flat.

| | SPA | DPA |
|---|---|---|
| Traces | 1 or few | hundreds-thousands |
| Method | visual | statistical |

Related: **passive = watch** (timing, power, EM) vs **active = poke** (fault injection, glitching). **Blinding** (decrypt c*r^e, then times r^-1) defends against **timing/side-channels**.

---

## 18. Quantum (Shor vs Grover)

🔑 **Grover:** 2^n -> **2^(n/2)** (halves the bits). **Shor:** polynomial time, breaks RSA/DH/DSA/ECC. Bigger keys don't save public-key.

| Cipher | classical | vs Grover |
|---|---|---|
| AES-128 | 128 | **64** |
| AES-192 | 192 | 96 |
| AES-256 | 256 | **128 (still safe)** |
| SHA-256 pre-image | 2^256 | 2^128 |

Fix: symmetric -> double key size (AES-256). Public-key -> **replace** with PQC (ML-KEM, ML-DSA, SLH-DSA).
**Harvest-now-decrypt-later:** store ciphertext today, decrypt after quantum arrives.

---

## 19. ML-KEM (Kyber) sizes and parameters

🔑 Module-LWE KEM, FIPS 203. n = 256, q = 3329.

| Variant | module rank k | security level | ~AES | public key | ciphertext |
|---|---|---|---|---|---|
| ML-KEM-512 | 2 | 1 | 128 | 800 B | 768 B |
| **ML-KEM-768** | 3 | 3 | 192 | **1184 B** | **1088 B** |
| ML-KEM-1024 | 4 | 5 | 256 | 1568 B | 1568 B |

- **Shared secret = 32 bytes** for all variants.
- Compare X25519: public key 32 B. ML-KEM-768 pk 1184 B -> about **37x bigger**.
- Three algorithms: KeyGen -> (pk, sk). Encaps(pk) -> (c, K). Decaps(sk, c) -> K.

**LWE tiny idea:** b = A*s + e mod q. Without e: Gaussian elimination solves s. With small hidden noise e: hard.

⚠ ML-KEM is a **KEM** (key establishment), not a signature. ML-DSA (FIPS 204) = lattice signature. SLH-DSA (FIPS 205) = hash-based signature.

---

## 20. TPM PCR extend

🔑 **PCR_new = H(PCR_old || measurement)**. PCRs can't be written directly, only extended. Order matters.

**Toy hash (non-commutative, illustration only): H(old || m) = (3*old + m) mod 100.** PCR starts at 0.
- Order A then B (A=10, B=20): extend A -> 3x0 + 10 = **10**. extend B -> 3x10 + 20 = **50**.
- Order B then A: extend B -> 3x0 + 20 = **20**. extend A -> 3x20 + 10 = **70**.
- 50 != 70. Different boot order = different PCR. ✅

**Why hash, not XOR or overwrite:** XOR is order-independent and reversible, so an attacker could cancel a bad measurement. A hash chain can't be undone.

**Related traps:**
- **Measured boot** = records into PCRs, doesn't block (notes).
- **Secure boot** = verifies signatures, blocks unsigned code (bouncer).
- **Sealing:** key released only if PCRs match.
- **Attestation:** AK signs (PCRs + verifier nonce). Nonce stops replay.
- EK = permanent hardware identity. AK = signs quotes.

---

## Last-minute drill (your repeat misses)
1. DPA = 16 x 256 = **4096**
2. DH key = **other side's public ^ your secret**
3. Grover halves bits: AES-128 -> **64**
4. Collision = **2^(n/2)**. Blocks = **ceil((L+65)/512)**
5. RSA d = e^-1 mod **phi**. CRT answers must satisfy **every** congruence.
6. Doubling slope uses **a**, never b.
7. Passive = watch, active = poke. Blinding = timing defence.
8. Measured boot records, secure boot blocks. PCR extend = hash chain.
9. CA signs with **private** key, verifier uses CA **public** key.
10. SAN = hostnames, Validity = dates.

Good luck! 🍀
