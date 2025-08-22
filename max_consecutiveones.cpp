#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> nums = {1, 1, 0, 1, 1, 1};  // example input

    int count = 0;
    int max_count = 0;

    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == 1) {
            count++;
            max_count = max(max_count, count); // cleaner update
        } else {
            count = 0; // reset
        }
    }

    cout << "Maximum consecutive 1's: " << max_count << endl;
    return 0;
}
