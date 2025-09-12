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
    struct Matrix x_inputs;
    struct Vector y_inputs;
};

// FUNCTION DECLARATIONS
void set_lines_dimensions(char *filename);
DataInputs read_data(void);
Vector get_column(Matrix X, int i);
void multiple_regression(void);