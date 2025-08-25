//Bruteforce approach to find the largest element in an array
// #include <iostream>
// #include <algorithm>
// #include <vector>
// using namespace std;

// int main() {
//     vector<int> v = {4, 2, 9, 1, 5};

//     sort(v.begin(), v.end());  // Sort in ascending order

//     cout<<"Largest element in the array is:"<<v.back();
//     return 0;
// }



//Optimal Solution to find the largest element in an array
#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int>arr={1,5,21,4,1,24,3,2,6,7,8};
    int largest=arr[0];
    for(int x:arr){
        if(x>largest){
            largest =x;
        }   
    }
    cout<<"Largest element in the array is: "<<largest;
    return 0;}