#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#define pi 3.1415926535
//#define pi 3.141592

using namespace std;

typedef struct complex {
    double real;
    double image;
} complex;

void DPF_forward(double f[], complex A[], int N);
void DPF_backward(double f[], complex A[], int N);
int Fill(double*& big, double*& small, int size_b, int size_s);
double* Conv(double* a, double* b, int n);

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
    cout << endl;
}


complex* ArrayComplexMul(complex A[], complex B[], int n){
    complex* C = new complex[n];
    for(int i = 0; i < n; i++){
        C[i] = A[i];
        ComplexMulComplex(C[i], B[i]);
    }
    return C;
}

int main()
{
    cout.precision(2);
    srand(time(NULL));
    cout << "CONVOLUTION (FOURIER)" << endl;
    int an, bn;
    cout << "Input size of a: ";
    cin >> an;
    cout << "Input size of b: ";
    cin >> bn;
    double *a, *b, *c;
    int size = 0;
    if(an > bn){
        size = Fill(a, b, an, bn);
        cout << "a: ";
        PrintArray(a, an);
        cout << "b: ";
        PrintArray(b, an);
        c = Conv(a, b, size);
        cout << "c: ";
        PrintArray(c, an);
    } else {
        size = Fill(b, a, bn, an);
        cout << "a: ";
        PrintArray(a, bn);
        cout << "b: ";
        PrintArray(b, bn);
        c = Conv(a, b, size);
        cout << "c: ";
        PrintArray(c, bn);
    }
}


double* Conv(double* a, double* b, int n){
    double* c = new double[n];
    complex ac[n], bc[n];
    complex* cc = new complex[n];
    DPF_forward(a, ac, n);
    DPF_forward(b, bc, n);
    cc = ArrayComplexMul(ac, bc, n);
    for(int i = 0; i < n; i++){
        ComplexMulConst(cc[i], (double)n);
    }
    DPF_backward(c, cc, n);
    return c;
}


void DPF_forward(double f[], complex A[], int N){
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
    //cout << "Forward mul count = " << m_counter << endl;
}
void DPF_backward(double f[], complex A[], int N){
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
    //cout << "Backward mul count = " << m_counter << endl;
}


int Fill(double*& big, double*& small, int size_b, int size_s){
    int r = 0;
    while(pow(2, r) < 2*size_b - 1) r++;
    int res_size = pow(2, r);
    big = new double [res_size];
    small = new double [res_size];
    for(int i = 0; i < size_s; i++){
        big[i] = (double)i + 1;
        small[i] = (double)i + 2;
    }
    for(int i = size_s; i < size_b; i++){
        big[i] = (double)i + 1;
        small[i] = 0;
    }
    for(int i = size_b; i < res_size; i++){
        big[i] = 0;
        small[i] = 0;
    }
    return res_size;
}