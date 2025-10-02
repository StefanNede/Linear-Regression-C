#include <stdio.h>

// STRUCTS
struct Vector;
typedef struct Vector Vector;

struct Matrix;
typedef struct Matrix Matrix;

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

// FUNCTION DEFINITIONS
Matrix transpose_matrix(Matrix X);
Matrix invert_matrix_2by2(Matrix X);

Matrix multiply_matrix_matrix(Matrix X, Matrix Y);
Vector multiply_matrix_vector(Matrix X, Vector y);

void print_matrix(Matrix X);