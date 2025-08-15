#include <stdio.h>

// STRUCTS
struct Vector;
typedef struct Vector Vector;

struct Matrix;
typedef struct Matrix Matrix;

struct DataInputs;
typedef struct DataInputs DataInputs;

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
    struct Vector x_inputs;
    struct Vector y_inputs;
};

// FUNCTION DECLARATIONS
int count_lines(char *filename);
double *get_padded_points(double *points, double min, double max, int length, double pad_amount);

DataInputs read_data(void);

Matrix gen_X(Vector x_values);
Matrix transpose_matrix(Matrix X);
Matrix invert_matrix_2by2(Matrix X);
Matrix multiply_matrix_matrix(Matrix X, Matrix Y);

Vector multiply_matrix_vector(Matrix X, Vector y);
Vector get_min_max(double *data_values, int length);

void print_matrix(Matrix X);
void plot_results(DataInputs data_inputs, Vector c_m);
void simple_regression(void);