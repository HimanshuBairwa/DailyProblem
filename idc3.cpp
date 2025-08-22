#include <bits/stdc++.h>
using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;

static inline u32 rotr(u32 x, unsigned n) { return (x >> n) | (x << (32 - n)); }

const u32 K[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

string sha256_hex(const vector<uint8_t>& msg) {
    // initial hash values
    u32 H[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };

    // padding
    vector<uint8_t> data = msg;
    u64 bitlen = (u64)msg.size() * 8ULL;
    data.push_back(0x80);
    while ((data.size() * 8) % 512 != 448) data.push_back(0x00);
    // append length as big-endian 64-bit
    for (int i = 7; i >= 0; --i) data.push_back((bitlen >> (8 * i)) & 0xFF);

    // process each 512-bit chunk
    for (size_t chunk=0; chunk < data.size(); chunk += 64) {
        u32 W[64];
        // first 16 words big-endian
        for (int t = 0; t < 16; ++t) {
            size_t off = chunk + t*4;
            W[t] = (data[off] << 24) | (data[off+1] << 16) | (data[off+2] << 8) | (data[off+3]);
        }
        for (int t = 16; t < 64; ++t) {
            u32 s0 = rotr(W[t-15], 7) ^ rotr(W[t-15], 18) ^ (W[t-15] >> 3);
            u32 s1 = rotr(W[t-2], 17) ^ rotr(W[t-2], 19) ^ (W[t-2] >> 10);
            W[t] = (u32)((uint64_t)W[t-16] + s0 + W[t-7] + s1);
        }

        u32 a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
        for (int t = 0; t < 64; ++t) {
            u32 S1 = rotr(e,6) ^ rotr(e,11) ^ rotr(e,25);
            u32 ch = (e & f) ^ ((~e) & g);
            u32 temp1 = (u32)((uint64_t)h + S1 + ch + K[t] + W[t]);
            u32 S0 = rotr(a,2) ^ rotr(a,13) ^ rotr(a,22);
            u32 maj = (a & b) ^ (a & c) ^ (b & c);
            u32 temp2 = S0 + maj;
            h = g; g = f; f = e;
            e = d + temp1;
            d = c; c = b; b = a;
            a = temp1 + temp2;
        }
        H[0] += a; H[1] += b; H[2] += c; H[3] += d;
        H[4] += e; H[5] += f; H[6] += g; H[7] += h;
    }

    // produce hex digest (big-endian H0..H7)
    std::ostringstream oss;
    oss << hex << setw(8) << setfill('0') << nouppercase;
    for (int i=0;i<8;i++){
        oss << setw(8) << H[i];
    }
    return oss.str();
}

int main(){
    vector<uint8_t> empty;
    cout << "SHA256(\"\") = " << sha256_hex(empty) << "\n";
    string s = "abc";
    vector<uint8_t> v(s.begin(), s.end());
    cout << "SHA256(\"abc\") = " << sha256_hex(v) << "\n";
    // Known:
    // SHA256("") = e3b0c44298fc1c149afbf4c8996fb924...
    // SHA256("abc") = ba7816bf8f01cfea414140de5dae2223...
    return 0;
}
