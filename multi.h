#include <stdio.h>

// STRUCTS
struct Vector;
typedef struct Vector Vector;

struct Matrix;
typedef struct Matrix Matrix;

struct DataInputs;
typedef struct DataInputs DataInputs;

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

// Struct for the 2 vector inputs of x and y values
struct DataInputs {
    struct Matrix x_inputs;
    struct Vector y_inputs;
};

// Struct for the 2 matrix output of QR factorisation of matrix X
struct QR {
    struct Matrix Q;
    struct Matrix R;
};

// FUNCTION DEFINITIONS

// Data handling
void set_lines_dimensions(char *filename);
DataInputs read_data(void);

// Debugging
void print_matrix(Matrix X);
void print_vector(Vector x);
void print_plane(Vector *coefficients);
void testing(void);
void test_back_sub(void);

// Basic LA data movement
double get_magnitude(Vector x);
Vector get_column(Matrix X, int i);
void copy_column_to_matrix_inplace(Vector col, Matrix *X, int col_idx);
int is_upper_triangular(Matrix *X);

// Vector/Matrix operations
double multiply_vector_vector(Vector x, Vector y);
void subtract_vector_vector_inplace(Vector *x, Vector y);
void multiply_scalar_vector_inplace(double scalar, Vector *x);
Matrix transpose_matrix(Matrix X);
Vector multiply_matrix_vector(Matrix X, Vector y);
Vector solve_back_sub(Matrix UT, Vector y);

// Matrix factorisations
QR QR_factorise(Matrix X);

// Orchestration
void multiple_regression(void);
int main(void);