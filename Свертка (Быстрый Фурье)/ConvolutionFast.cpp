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

void Fast_forward(double f[], complex A_final[], int N, int N_real, int r);
void FFT_forward_rec(complex in[], complex out[], int N, int& m_counter);
void Fast_backward(double f[], complex A_final[], int N, int N_real, int r);
void FFT_backward_rec(complex in[], complex out[], int N, int& m_counter);

int Fill(double*& big, double*& small, int size_b, int size_s);
double* Conv(double* a, double* b, int n);

void ComplexSum(complex& z1, complex z2){
    z1.real = z1.real + z2.real;
    z1.image = z1.image + z2.image;
}
void ComplexSub(complex& z1, complex z2){
    z1.real = z1.real - z2.real;
    z1.image = z1.image - z2.image;
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
    int r = sqrt(n);
    Fast_forward(a, ac, n, n, r);
    Fast_forward(b, bc, n, n, r);
    cc = ArrayComplexMul(ac, bc, n);
    for(int i = 0; i < n; i++){
        ComplexMulConst(cc[i], (double)n);
    }
    Fast_backward(c, cc, n, n, r);
    return c;
}


void Fast_forward(double f[], complex A_final[], int N, int N_real, int r){
    int m_counter = 0;

    complex* in = new complex[N_real];
    for(int i = 0; i < N_real; i++){
        in[i].real = f[i];  
        in[i].image = 0;
    }

    FFT_forward_rec(in, A_final, N_real, m_counter);

    for(int i = 0; i < N_real; i++){
        ComplexMulConst(A_final[i], 1.0 / N_real);
    }

    delete[] in;

    //cout << "Forward mul count = " << m_counter << endl;
}
void FFT_forward_rec(complex in[], complex out[], int N, int& m_counter){
    if(N == 1){
        out[0] = in[0];
        return;
    }
    int half = N / 2;

    complex* even_cur = new complex[half];
    complex* uneven_cur = new complex[half];
    for(int i = 0; i < half; i++){
        even_cur[i] = in[2*i];
        uneven_cur[i]  = in[2*i + 1];
    }

    complex* even_next = new complex[half];
    complex* uneven_next = new complex[half];
    FFT_forward_rec(even_cur, even_next, half, m_counter);
    FFT_forward_rec(uneven_cur,  uneven_next, half, m_counter);

    for(int k = 0; k < half; k++){
        double angle = (2.0 * pi) * ((double)k / N);
        complex e;
        e.real = cos(-angle);
        e.image = sin(-angle);

        complex u = uneven_next[k];
        ComplexMulComplex(u, e);  
        m_counter++;              

        out[k] = even_next[k];
        out[k + half] = even_next[k];
        ComplexSum(out[k], u);
        ComplexSub(out[k + half], u);
    }

    delete[] even_cur; 
    delete[] uneven_cur;
    delete[] even_next;    
    delete[] uneven_next;
}

void Fast_backward(double f_final[], complex A_final[], int N, int N_real, int r){
    int m_counter = 0;

    complex* out = new complex[N_real];
    FFT_backward_rec(A_final, out, N_real, m_counter);

    for(int i = 0; i < N_real; i++){
        f_final[i] = out[i].real;
    }

    delete[] out;

    //cout << "Backward mul count = " << m_counter << endl;
}
void FFT_backward_rec(complex in[], complex out[], int N, int& m_counter){
    if(N == 1){
        out[0] = in[0];
        return;
    }
    int half = N / 2;

    complex* even_cur = new complex[half];
    complex* uneven_cur = new complex[half];
    for(int i = 0; i < half; i++){
        even_cur[i] = in[2*i];
        uneven_cur[i] = in[2*i + 1];
    }

    complex* even_next = new complex[half];
    complex* uneven_next = new complex[half];
    FFT_backward_rec(even_cur, even_next, half, m_counter);
    FFT_backward_rec(uneven_cur, uneven_next, half, m_counter);

    for(int k = 0; k < half; k++){
        double angle = (2.0 * pi) * ((double)k / N);
        complex e;
        e.real = cos(angle); 
        e.image = sin(angle);

        complex u = uneven_next[k];
        ComplexMulComplex(u, e);
        m_counter++;

        out[k] = even_next[k];
        out[k + half] = even_next[k];
        ComplexSum(out[k], u);
        ComplexSub(out[k + half], u);
    }
    delete[] even_cur; 
    delete[] uneven_cur;
    delete[] even_next;    
    delete[] uneven_next;
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