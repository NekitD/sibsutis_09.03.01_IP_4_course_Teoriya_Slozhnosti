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

void HalfFast_forward(double f[], complex A2[], int N, int p1, int p2);
void HalfFast_backward(double f[], complex A2[], int N, int p1, int p2);
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
    cout << "CONVOLUTION (HALF-FAST FOURIER)" << endl;
    int ap1, ap2, bp1, bp2, an, bn;
    cout << "Input p1 of a: ";
    cin >> ap1;
    cout << "Input p2 of a: ";
    cin >> ap2;
    cout << "Input p1 of b: ";
    cin >> bp1;
    cout << "Input p2 of b: ";
    cin >> bp2;

    an = ap1 * ap2;
    bn = bp1 * bp2;
    cout << "Size of a: " << an << endl;
    cout << "Size of b: " << bn << endl;
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
    int p1 = 2;
    int p2 = 1;
    while(p1 * p2 != n){
        p2++;
    }
    double* c = new double[n];
    complex ac[n], bc[n];
    complex* cc = new complex[n];
    HalfFast_forward(a, ac, n, p1, p2);
    HalfFast_forward(b, bc, n, p1, p2);
    cc = ArrayComplexMul(ac, bc, n);
    for(int i = 0; i < n; i++){
        ComplexMulConst(cc[i], (double)n);
    }
    HalfFast_backward(c, cc, n, p1, p2);
    return c;
}


void HalfFast_forward(double f[], complex A2[], int N, int p1, int p2){
    int m_counter = 0;
    complex A1[N];
        
    for(int k1 = 0; k1 < p1; k1++){
        for(int j2 = 0; j2 < p2; j2++){
            A1[k1*p2 + j2].real = 0;
            A1[k1*p2 + j2].image = 0;
            for(int j1 = 0; j1 < p1; j1++){
                complex e;
                double angle = (2.0 * pi) * ((double)(j1 * k1)/p1);
                e.real = cos(-angle);
                e.image = sin(-angle);
                ComplexMulConst(e, f[j2 + (p2*j1)]);
                m_counter++;
                ComplexSum(A1[k1*p2 + j2], e);
            }
            ComplexMulConst(A1[k1*p2 + j2], (1.0/p1));      
        } 
    }

    for(int k1 = 0; k1 < p1; k1++){
        for(int k2 = 0; k2 < p2; k2++){
            A2[k1*p2 + k2].real = 0;
            A2[k1*p2 + k2].image = 0;
            for(int j2 = 0; j2 < p2; j2++){
                complex e;
                double angle = (2.0 * pi) * (((double)j2 / (p1 * p2))*(k1 + p1*k2));
                e.real = cos(-angle);
                e.image = sin(-angle);
                ComplexMulComplex(e, A1[k1*p2 + j2]);
                m_counter++;
                ComplexSum(A2[k1*p2 + k2], e);
            }
            ComplexMulConst(A2[k1*p2 + k2], (1.0/p2));     
        } 
    }
    //cout << "Forward mul count = " << m_counter << endl;
}
void HalfFast_backward(double f[], complex A2[], int N, int p1, int p2){
    int m_counter = 0;
    complex A1[N];

    for(int k1 = 0; k1 < p1; k1++){
        for(int j2 = 0; j2 < p2; j2++){
            A1[k1*p2 + j2].real = 0;
            A1[k1*p2 + j2].image = 0;
            for(int k2 = 0; k2 < p2; k2++){
                complex e;
                double angle = (2.0 * pi) * ((double)j2 / (p1 * p2)) * (k1 + p1 * k2);
                e.real = cos(angle);
                e.image = sin(angle);
                ComplexMulComplex(e, A2[k1*p2 + k2]);
                m_counter++;
                ComplexSum(A1[k1*p2 + j2], e);
            }
        }
    }

    for(int j1 = 0; j1 < p1; j1++){
        for(int j2 = 0; j2 < p2; j2++){
            complex new_f;
            new_f.real = 0;
            new_f.image = 0;
            for(int k1 = 0; k1 < p1; k1++){
                complex e;
                double angle = (2.0 * pi) * ((double)(j1 * k1) / p1);
                e.real = cos(angle);
                e.image = sin(angle);
                ComplexMulComplex(e, A1[k1*p2 + j2]);
                m_counter++;
                ComplexSum(new_f, e);
            }
            f[j2 + p2*j1] = new_f.real;                 
        }
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