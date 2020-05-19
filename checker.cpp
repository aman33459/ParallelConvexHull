// generating a random sequence of distinct elements
#include <bits/stdc++.h>
using namespace std;

long long rand(long long a, long long b) {
    return a + rand() % (b - a + 1);
}
int main(int argc, char* argv[]) {
    srand(atoi(argv[1]));
    int t= rand(1,1000000);
    cout << t << "\n";
    while(t--){
        int x = rand(-100,100);
        int y = rand(-100,100);
        cout << x  << " " << y << "\n";   
        }
      	//cout << "\n";
}

/*

9 6 2 9 5 8 6 8 3 10
2 3 5 6 8 
*/