#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>

// #define pi 3.1415926535
#define pi 3.141592

using namespace std;

typedef struct complex {
    double real;
    double image;
} complex;

void HalfFast_forward(double f[], complex A[], int N);
void HalfFast_backward(double f[], complex A[], int N);


void ComplexSum(complex& z1, complex z2);
void ComplexMulConst(complex& z, double c);
void ComplexMulComplex(complex& z1, complex z2);
void PrintComplex(complex z);
void PrintComplexArray(complex Z[], int n);
void PrintArray(double A[], int n);

void PrintResults(double f[], complex A[], double f_final[], int N);

int main()
{
    cout.precision(7);
    srand(time(NULL));
    int k;
    cout << "n = 2^k" << endl;
    cout << "Input k: ";
    cin >> k;
    int n = pow(2, k);
    double f[n], f_final[n];
    complex A[n];

    for(int i = 0; i < n; i++){
        f[i] = rand() % 100;
    }
    
    HalfFast_forward(f, A, n);
    HalfFast_backward(f_final, A, n);
    PrintResults(f, A, f_final, n);

}

void HalfFast_forward(double f[], complex A[], int N){
    int m_counter = 0;
    for(int k = 0; k < N; k++){
        A[k].real = 0;
        A[k].image = 0;
        for(int j = 0; j < N; j++){
            complex e;
            double angle = (2.0 * pi) * ((double)(k*j)/N);
            e.real = cos(-angle);
            e.image = sin(-angle);
            ComplexMulConst(e, f[j]);
            m_counter++;
            ComplexSum(A[k], e);
        }
        ComplexMulConst(A[k], (1.0/N));
    }
    cout << "Forward mul count = " << m_counter << endl;
}
void HalfFast_backward(double f[], complex A[], int N){
    int m_counter = 0;
    for(int k = 0; k < N; k++){
        complex new_f;
        new_f.real = 0;
        new_f.image = 0;
        for(int j = 0; j < N; j++){
            complex e;
            double angle = (2.0 * pi) * ((double)(k*j)/N);
            e.real = cos(angle);
            e.image = sin(angle);
            ComplexMulComplex(e, A[j]);
            m_counter++;
            ComplexSum(new_f, e);
        }
        f[k] = new_f.real;
    }
    cout << "Backward mul count = " << m_counter << endl;
}

void ComplexSum(complex& z1, complex z2){
    z1.real = z1.real + z2.real;
    z1.image = z1.image + z2.image;
}

void ComplexMulConst(complex& z, double c){
    z.real = z.real * c;
    z.image = z.image * c;
}

void ComplexMulComplex(complex& z1, complex z2){
    complex res; 
    res.real = (z1.real * z2.real) - (z1.image * z2.image);
    res.image = (z1.real * z2.image) + (z2.real * z1.image);
    z1.real = res.real;
    z1.image = res.image;
}


void PrintComplex(complex z){
    cout << z.real << " + (" << z.image << ")*i";
}

void PrintComplexArray(complex Z[], int n){
    for(int i = 0; i < n; i++){
        PrintComplex(Z[i]);
        cout << "   ";
    }
}

void PrintArray(double A[], int n){
    for(int i = 0; i < n; i++){
        cout << A[i] << " ";
    }
}

void PrintResults(double f[], complex A[], double f_final[], int N){
    cout << "|      Default     |       Forward     |       Backward        |" << endl;
    cout << "=======================================================================" << endl;
    for(int i = 0; i < N; i++){
        cout << "      " << f[i] << "         ";
        PrintComplex(A[i]);
        cout << "             " << f_final[i] << "         ";
        cout << endl;
    } 
}