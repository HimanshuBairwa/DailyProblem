#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

// modular operations
long long modinv(long long a, long long p) {
    // extended euclid
    long long b = p, u = 1, v = 0;
    while (b) {
        long long t = a / b;
        a -= t*b; swap(a,b);
        u -= t*v; swap(u,v);
    }
    if (u < 0) u += p;
    return u;
}
long long modpow(long long a, long long e, long long p) {
    long long r = 1 % p;
    a %= p;
    while (e) {
        if (e & 1) r = (r * a) % p;
        a = (a * a) % p;
        e >>= 1;
    }
    return r;
}

bool is_on_curve(long long x, long long y, long long a, long long b, long long p) {
    long long lhs = (y*y) % p;
    long long rhs = ( ( (x*x)%p * x ) % p + (a * x) % p + b ) % p;
    if (lhs < 0) lhs += p;
    if (rhs < 0) rhs += p;
    return lhs == rhs;
}

// enumerate all points (excluding point at infinity)
vector<pair<int,int>> enum_points(int a, int b, int p) {
    vector<pair<int,int>> pts;
    for (int x = 0; x < p; ++x) {
        int rhs = ( ( (1LL*x*x)%p * x ) % p + (1LL*a*x)%p + b ) % p;
        if (rhs < 0) rhs += p;
        // we need y such that y^2 = rhs mod p
        for (int y = 0; y < p; ++y) {
            if ((1LL*y*y) % p == rhs) pts.emplace_back(x,y);
        }
    }
    return pts;
}

// basic affine point addition (not optimized); returns pair<inf_flag, pair(x,y)>
// inf_flag=true means point at infinity
struct Point { bool inf; long long x,y; };

Point point_add(const Point& P, const Point& Q, long long a, long long p) {
    if (P.inf) return Q;
    if (Q.inf) return P;
    if (P.x == Q.x && (P.y != Q.y || P.y == 0)) { // P + (-P) = O
        return {true,0,0};
    }
    long long lambda;
    if (P.x == Q.x && P.y == Q.y) {
        // doubling: lambda = (3*x^2 + a) / (2*y)
        long long num = (3 * ((P.x*P.x)%p) + a) % p;
        if (num < 0) num += p;
        long long den = (2 * P.y) % p;
        if (den < 0) den += p;
        long long inv = modinv(den, p);
        lambda = (num * inv) % p;
    } else {
        // addition: lambda = (y2 - y1) / (x2 - x1)
        long long num = (Q.y - P.y) % p; if (num < 0) num += p;
        long long den = (Q.x - P.x) % p; if (den < 0) den += p;
        long long inv = modinv(den, p);
        lambda = (num * inv) % p;
    }
    long long xr = ( (lambda*lambda - P.x - Q.x) % p + p) % p;
    long long yr = ( (lambda*(P.x - xr) - P.y) % p + p) % p;
    return {false, xr, yr};
}

int main(){
    // Example: curve y^2 = x^3 + ax + b mod p
    int a = 2, b = 3;
    int p = 97; // prime modulus (change to any small prime)
    cout << "Curve: y^2 = x^3 + " << a << "*x + " << b << " over F_" << p << "\n";
    auto pts = enum_points(a,b,p);
    cout << "Number of (affine) points (excluding infinity): " << pts.size() << "\n";
    // Print first 30 points
    for (size_t i=0;i<pts.size() && i<30;i++){
        cout << "(" << pts[i].first << "," << pts[i].second << ") ";
        if ((i+1)%6==0) cout << "\n";
    }
    cout << "\n";

    // Example checks
    vector<pair<int,int>> test = {{3,6}, {5,7}, {10,20}};
    for (auto &pr : test) {
        cout << "Point (" << pr.first << "," << pr.second << ") on curve? "
             << (is_on_curve(pr.first, pr.second, a, b, p) ? "Yes" : "No") << "\n";
    }

    // Example point addition (take first two points if available)
    if (pts.size() >= 2) {
        Point P{false, pts[0].first, pts[0].second};
        Point Q{false, pts[1].first, pts[1].second};
        auto R = point_add(P,Q,a,p);
        if (R.inf) cout << "P + Q = O (point at infinity)\n";
        else cout << "P + Q = (" << R.x << "," << R.y << ")\n";
    }

    // Optionally export all points to CSV for plotting externally
    {
        ofstream fout("ecc_points.csv");
        fout << "x,y\n";
        for (auto &pr : pts) fout << pr.first << "," << pr.second << "\n";
        fout.close();
        cout << "All points exported to ecc_points.csv (you can plot them in Python/Excel).\n";
    }

    return 0;
}
