#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

void BubbleSort(int A[], int n);

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
    BubbleSort(A, n);
    cout << endl;
    cout << "Sorted Array: " << endl;
    PrintArray(A, n);
}

void BubbleSort(int A[], int n){
    for(int i = 0; i < n - 1; i++){
        bool changed = false;
        for(int j = 0; j < n - 1; j++){
            if(A[j] > A[j + 1]){
                changed = true;
                int temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
        if (!changed){
            break;
        }
    }
}