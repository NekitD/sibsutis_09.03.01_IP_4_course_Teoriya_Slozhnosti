#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

void SelectSort(int A[], int n);

void PrintArray(int A[], int n){
    for(int i = 0; i < n; i++){
        cout << A[i] << " ";
    }
}

int main()
{
    srand(time(NULL));
    int k = 1;
    cout << "n = 2^k" << endl;
    cout << "Input k: " << endl;
    cin >> k;
    cout << endl;
    int n = pow(2, k);
    int A[n];
    for(int i = 0; i < n; i++){
        A[i] = rand() % n;
    }
    cout << "Default Array: " << endl;
    PrintArray(A, n);
    cout << endl;
    SelectSort(A, n);
    cout << endl;
    cout << "Sorted Array: " << endl;
    PrintArray(A, n);
}

void SelectSort(int A[], int n){
    for(int i = 0; i < n - 1; i++){
        int j_min = i;
        for(int j = i + 1; j < n; j++){
            if(A[j] < A[j_min]){
                j_min = j;
            }
        }
        int temp = A[i];
        A[i] = A[j_min];
        A[j_min] = temp;
    }
}