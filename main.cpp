

#include <iostream>
#include <string>
using namespace std;


bool IsKPeriodic(string str, int k) {
    if (k == 0) return false;
    string pat = str.substr(0,k);
    int n = str.length();
    int i = k, j = 0;
    while (i < n) {
        j = 0;
        while (j < k) {
            if (str[i] != pat[j]) return false; else {
                j++, i++;
            }
        }

    }  return true;
}

int main()
{
    if (IsKPeriodic("asasasas", 4)) cout << " kratno"; else cout << " ne kratno";
}

