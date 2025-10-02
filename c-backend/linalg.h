#include <stdio.h>

// STRUCTS
struct Vector;
typedef struct Vector Vector;

struct Matrix;
typedef struct Matrix Matrix;

struct QR;
typedef struct QR QR;

// Struct for a size x 1 vector
struct Vector {
    double* data;
    int size; 
};

// Struct for an nxm matrix
// could also implement as a list of pointers 
struct Matrix {
    int n, m; 
    double* data; // data[i][j] = data[i*n + m]
};

// Struct for the 2 matrix output of QR factorisation of matrix X
struct QR {
    Matrix Q;
    Matrix R;
};

// FUNCTION DEFINITIONS
Matrix transpose_matrix(Matrix X);
Matrix invert_matrix_2by2(Matrix X);

double get_magnitude(Vector x);
Vector get_column(Matrix X, int i);
void copy_column_to_matrix_inplace(Vector col, Matrix *X, int col_idx);

Matrix multiply_matrix_matrix(Matrix X, Matrix Y);
Vector multiply_matrix_vector(Matrix X, Vector y);
double multiply_vector_vector(Vector x, Vector y);
void subtract_vector_vector_inplace(Vector *x, Vector y);
void multiply_scalar_vector_inplace(double scalar, Vector *x);
Vector solve_back_sub(Matrix UT, Vector y);

// Matrix factorisations
QR QR_factorise(Matrix X);

void print_matrix(Matrix X);
void print_vector(Vector x);