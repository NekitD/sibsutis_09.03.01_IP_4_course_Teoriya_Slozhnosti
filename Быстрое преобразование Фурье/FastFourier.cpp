#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>

// #define pi 3.1415926535
// #define pi 3.141592
#define pi 3.14

using namespace std;

typedef struct complex {
    double real;
    double image;
} complex;

void Fast_forward(double f[], complex A_final[], int N, int N_real, int r);
void FFT_forward_rec(complex in[], complex out[], int N, int& m_counter);

void Fast_backward(double f[], complex A_final[], int N, int N_real, int r);
void FFT_backward_rec(complex in[], complex out[], int N, int& m_counter);

void ComplexSum(complex& z1, complex z2);
void ComplexSub(complex& z1, complex z2);
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
    int n;
    cout << "FAST FOURIER" << endl;
    cout << "Input n: ";
    cin >> n;

    int r = -1;
    for(int i = 0; i < 100; i++){
        if(pow(2, i) >= n){
            r = i;
            break;
        }
    }
    if(r < 0){
        cout << "Требуется r > 100" << endl;
        return 1;
    }
    int n_real = pow(2, r);
    cout << "n = " << n << endl;
    cout << "n_real = " << n_real << endl;
    double f[n_real], f_final[n_real];
    complex A[n_real];

    for(int i = 0; i < n; i++){
        f[i] = rand() % 100;
    }

    for(int i = n; i < n_real; i++){
        f[i] = 0;
    }

    Fast_forward(f, A, n, n_real, r);
    Fast_backward(f_final, A, n, n_real, r);
    PrintResults(f, A, f_final, n);
}

//================================================================================

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

    cout << "Forward mul count = " << m_counter << endl;
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


//================================================================================

void Fast_backward(double f_final[], complex A_final[], int N, int N_real, int r){
    int m_counter = 0;

    complex* out = new complex[N_real];
    FFT_backward_rec(A_final, out, N_real, m_counter);

    for(int i = 0; i < N_real; i++){
        f_final[i] = out[i].real;
    }

    delete[] out;

    cout << "Backward mul count = " << m_counter << endl;
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

//================================================================================

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
}

void PrintResults(double f[], complex A[], double f_final[], int N){
    cout << "\n";
    cout << "+" << string(6, '-') << "+"
         << string(22, '-') << "+"
         << string(28, '-') << "+"
         << string(22, '-') << "+" << endl;

    cout << "|" << setw(6) << "  i  " << "|"
         << setw(22) << "Default" << "|"
         << setw(28) << "Forward" << "|"
         << setw(22) << "Backward" << "|" << endl;

    cout << "+" << string(6, '-') << "+"
         << string(22, '-') << "+"
         << string(28, '-') << "+"
         << string(22, '-') << "+" << endl;


    for(int i = 0; i < N; i++){
        cout << "|" << setw(6) << i << "|";
        cout << setw(22) << fixed << f[i] << "|";

        ostringstream oss;
        oss.precision(cout.precision());
        oss << fixed
            << A[i].real << (A[i].image >= 0 ? " + " : "")
            << A[i].image << "*i";
        cout << setw(28) << oss.str() << "|";

        cout << setw(22) << fixed << f_final[i] << "|";

        cout << endl;

        if(i < N - 1){
            cout << "+" << string(6, '-') << "+"
                 << string(22, '-') << "+"
                 << string(28, '-') << "+"
                 << string(22, '-') << "+" << endl;
        }
    }
    cout << "+" << string(6, '-') << "+"
         << string(22, '-') << "+"
         << string(28, '-') << "+"
         << string(22, '-') << "+" << endl;
    cout << endl;
}