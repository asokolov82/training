

#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <winsock2.h> 

using namespace std;
void sum(int arr[], int _m, int pos, int res[], int respos) {
    int summ = 0;
    for (int i = 0; i < _m; i++) {
    //    cout << "pos" << pos << " " << arr[pos] << ", " ;
        summ += arr[pos]; 
        pos++;
        
    } 
    res[respos] = summ;
  //  Sleep(50 * respos);
  //  cout << endl << " potok-" << respos << " " << summ << endl;
}
int main()
{
    vector<thread> threads;
    const int   n = 100000000;
    const int del = 20000000;
    int m = n / del;
    int *array=new int[n];
    int *res = new int[del];
    for (int i = 0; i < n; i++) {
        array[i] = 1 + rand() % 10;
    }
    for (int i = 0; i < m; i++)
    {
        threads.push_back(thread(sum, ref(array), del, i * del, ref(res), i));
    }
    for_each(threads.begin(), threads.end(), [](std::thread& t)
        { t.join(); });
    int ressum = 0;
    for (int i = 0; i < m; i++) {
        ressum += res[i];
    }
    cout << "potokov - " << m << endl;
    cout << " KONETZ potokov" << ressum << endl;
    int ressumm = 0;
    for (int i = 0; i < n; i++) {
        ressumm += array[i];
    }
    cout << " KONETZ main " << ressumm << endl;
    delete[] array;
    return 0;
}
