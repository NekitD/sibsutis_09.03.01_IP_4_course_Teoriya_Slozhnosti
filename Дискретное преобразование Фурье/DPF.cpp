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

void DPF_forward(double f[], complex A[], int N);
void DPF_backward(double f[], complex A[], int N);
void PrintResults(double f[], complex A[], double f_final[], int N);


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

int main()
{
    cout.precision(7);
    srand(time(NULL));
    int n;
    cout << "Input n: ";
    cin >> n;
    double f[n], f_final[n];
    complex A[n];

    for(int i = 0; i < n; i++){
        f[i] = rand() % 100;
    }

    DPF_forward(f, A, n);
    DPF_backward(f_final, A, n);
    PrintResults(f, A, f_final, n);
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
    cout << "Forward mul count = " << m_counter << endl;
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
    cout << "Backward mul count = " << m_counter << endl;
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