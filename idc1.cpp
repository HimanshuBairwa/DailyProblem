#include <bits/stdc++.h>
using namespace std;
using u32 = uint32_t;
using u64 = uint64_t;

static inline u32 leftrotate(u32 x, unsigned c) {
    return (x << c) | (x >> (32 - c));
}

string md5_hex(const vector<uint8_t>& msg) {
    
    const uint32_t s[64] = {
         7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
         5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
         4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
         6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
    };

    
    uint32_t K[64];
    for (int i = 0; i < 64; ++i) {
        K[i] = (uint32_t) (uint64_t(floor(pow(2.0,32) * fabs(sin((double)(i+1))))) ) ;
    }

    
    u32 A = 0x67452301;
    u32 B = 0xefcdab89;
    u32 C = 0x98badcfe;
    u32 D = 0x10325476;

    
    vector<uint8_t> data = msg;
    u64 orig_bits = (u64)msg.size() * 8ULL;
    data.push_back(0x80);
    while ((data.size() * 8) % 512 != 448) data.push_back(0x00);

    
    for (int i = 0; i < 8; ++i) data.push_back((uint8_t)((orig_bits >> (8 * i)) & 0xFF));

    for (size_t chunk = 0; chunk < data.size(); chunk += 64) {
        
        u32 M[16];
        for (int i = 0; i < 16; ++i) {
            size_t off = chunk + i*4;
            M[i] = (u32)data[off] | ((u32)data[off+1] << 8) | ((u32)data[off+2] << 16) | ((u32)data[off+3] << 24);
        }

        u32 a = A, b = B, c = C, d = D;

        for (int i = 0; i < 64; ++i) {
            u32 F;
            int g;
            if (i <= 15) {
                F = (b & c) | ((~b) & d);
                g = i;
            } else if (i <= 31) {
                F = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i <= 47) {
                F = b ^ c ^ d;
                g = (3*i + 5) % 16;
            } else {
                F = c ^ (b | (~d));
                g = (7*i) % 16;
            }
            F = F + a + K[i] + M[g];
            a = d;
            d = c;
            c = b;
            b = b + leftrotate(F, s[i]);
        }

        A += a; B += b; C += c; D += d;
    }

    
    auto to_hex8 = [](u32 x) {
        std::ostringstream oss;
        oss << hex << setw(8) << setfill('0') << (uint32_t)x;
        return oss.str();
    };

    
    stringstream out;
    out << setw(8) << setfill('0') << hex << std::nouppercase;
    
    auto put_word_le = [&](u32 w){
        for (int i=0;i<4;i++){
            uint8_t byte = (w >> (8*i)) & 0xFF;
            out << setw(2) << (int)byte;
        }
    };
    put_word_le(A); put_word_le(B); put_word_le(C); put_word_le(D);
    return out.str();
}

int main(){
    vector<uint8_t> empty;
    cout << "MD5(\"\") = " << md5_hex(empty) << "\n";
    string s = "The quick brown fox jumps over the lazy dog";
    vector<uint8_t> v(s.begin(), s.end());
    cout << "MD5(\"" << s << "\") = " << md5_hex(v) << "\n";
    // Known vectors:
    // MD5("") = d41d8cd98f00b204e9800998ecf8427e
    // MD5("The quick ...") = 9e107d9d372bb6826bd81d3542a419d6
    return 0;
}
