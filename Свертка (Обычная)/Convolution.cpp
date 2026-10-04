#include <iostream>

using namespace std;

void PrintArray(int* A, int n){
    for(int i = 0; i < n; i++){
        cout << A[i] << " ";
    }
    cout << endl;
}

int* Conv(int*, int*, int);
void Fill(int*&, int*&, int, int);

int main()
{
    int *a, *b, *c;
    int an, bn;
    cout << "Convolution (SIMPLE)" << endl;
    cout << "Input size of a: ";
    cin >> an;
    cout << "Input size of b: ";
    cin >> bn;
    if(an > bn){
        Fill(a, b, an, bn);
        cout << "a: ";
        PrintArray(a, an);
        cout << "b: ";
        PrintArray(b, an);
        c = Conv(a, b, an);
        cout << "c: ";
        PrintArray(c, an);
    } else {
        Fill(b, a, bn, an);
        cout << "a: ";
        PrintArray(a, bn);
        cout << "b: ";
        PrintArray(b, bn);
        c = Conv(a, b, bn);
        cout << "c: ";
        PrintArray(c, bn);
    }   
}

int* Conv(int* a, int* b, int n){
    int* c = new int[n];
    for(int i = 0; i < n; i++){
        c[i] = 0;
        for(int k = 0; k <= i; k++){
            for(int l = 0; l <= i; l++){
                if(k + l == i){
                    c[i] += a[k] * b[l];
                }
            }
        }
    }
    return c;
}


void Fill(int*& big, int*& small, int size_b, int size_s){
    big = new int[size_b];
    small = new int[size_b];
    for(int i = 0; i < size_s; i++){
        big[i] = i + 1;
        small[i] = i + 2;
    }
    for(int i = size_s; i < size_b; i++){
        big[i] = i + 1;
        small[i] = 0;
    }
}