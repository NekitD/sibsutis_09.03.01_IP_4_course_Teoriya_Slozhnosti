#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>

// #define pi 3.1415926535
#define pi 3.141592

using namespace std;

typedef struct complex {
    double real;
    double image;
} complex;

void HalfFast_forward(double f[], complex A2[], int N, int p1, int p2);
void HalfFast_backward(double f[], complex A2[], int N, int p1, int p2);


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
    int p1, p2;
    cout << "HALF-FAST FOURIER" << endl;
    cout << "Input p1: ";
    cin >> p1;
    cout << "Input p2: ";
    cin >> p2;
    int n = p1 * p2;
    cout << "N = " << n << endl;
    double f[n], f_final[n];
    complex A[n];

    for(int i = 0; i < n; i++){
        f[i] = rand() % 100;
    }

    HalfFast_forward(f, A, n, p1, p2);
    HalfFast_backward(f_final, A, n, p1, p2);
    PrintResults(f, A, f_final, n);
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
            //m_counter++;     
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
            //m_counter++;     
        } 
    }
    cout << "Forward mul count = " << m_counter << endl;
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