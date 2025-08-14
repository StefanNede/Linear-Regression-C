// Simple linear regression -- 2 variables

#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h> 

/* VARIABLES with types
    n - int - number of data point pairs to run regression on
    b - 2x1 vector - (c, m)T
    y - nx1 vector - inputs (y_1, y_2, ..., y_n)T
    X - nx2 matrix - 1st column is all 1s and 2nd column is nx1 vector of inputs (x_1, x_2, ..., x_n)T
*/

/* NORMAL EQUATIONS solution (solve for b)
    b = (XT*X)^(-1) * XT * y
*/

// GLOBALS -------------------------------
volatile int n;

// STRUCTS -------------------------------

// Struct for a size x 1 vector
struct Vector {
    float* data;
    int size; 
};

// Struct for an nxm matrix
// could also implement as a list of pointers 
struct Matrix {
    int n, m; 
    float* data; // data[i][j] = data[i*n + m]
};

// Struct for the 2 vector inputs of x and y values
struct DataInputs {
    struct Vector x_inputs;
    struct Vector y_inputs;
};

// FUNCTIONS -------------------------------

// Count the number of lines in an input file
int count_lines(char *filename) {
    FILE *fptr; 
    char c;
    int count = 0; 

    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Failed at counting lines for file: `%s`\n", filename);
        return -1;
    }

    for (c = getc(fptr); c != EOF; c = getc(fptr)) {
        if (c == '\n') { 
            count++; 
        }
    }

    fclose(fptr);
    return count;
}

// Read the data input from the csv file
struct DataInputs read_data(void) {
    FILE *fptr;  
    struct DataInputs data_inputs;
    char c;
    int onx = 1, i = 0; // flag: 1 - on x field of input, 0 - on y field of input

    data_inputs.x_inputs.size = n;
    data_inputs.y_inputs.size = n;
    data_inputs.x_inputs.data = (float*)malloc(n*sizeof(float));
    data_inputs.y_inputs.data = (float*)malloc(n*sizeof(float));

    fptr = fopen("data.txt", "r");

    if (fptr != NULL) {
        for (c = getc(fptr); c != EOF; c = getc(fptr)) {
            if (c == '\n') {
                i++;
                onx = 1;
            } else if (c == ',') {
                onx = 0;
            } else {
                if (onx) {
                    (data_inputs.x_inputs.data)[i] = atof(&c);
                    // printf("%f,", (data_inputs.x_inputs.data)[i]);
                } else {
                    (data_inputs.y_inputs.data)[i] = atof(&c);
                    // printf("%f\n", (data_inputs.y_inputs.data)[i]);
                }
            }
        }
    } else {
        printf("Not able to open the file: `data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
}

// Generate the nx2 X matrix 
struct Matrix gen_X(struct Vector x_values)  {
    struct Matrix X; int i;

    X.n = x_values.size;
    X.m = 2;
    X.data = (float*)malloc(X.n * X.m * sizeof(float));

    for (i = 0; i < x_values.size; i++) {
        X.data[2*i] = 1.0;
        X.data[2*i + 1] = x_values.data[i];
    }

    return X;
}

// Transpose a matrix 
struct Matrix transpose_matrix(struct Matrix X) {
    struct Matrix X_T; int i, j;

    X_T.n = X.m; X_T.m = X.n;
    X_T.data = (float*)malloc(X_T.n * X_T.m * sizeof(float));

    // data[i][j] = data[j][i]
    for (i = 0; i < X.n; i++) {
        for (j = 0; j < X.m; j++) {
            X_T.data[j * X_T.n + i] = X.data[i * X.n + j];
        }
    }

    return X_T;
}

// Invert the 2x2 matrix provided
struct Matrix invert_matrix_2by2(struct Matrix X) {
    struct Matrix X_inverse;
    float a, b, c, d;
    X_inverse.n = 2;
    X_inverse.m = 2;
    X_inverse.data = (float*)malloc(4 * sizeof(float));
    a = X.data[0];
    b = X.data[1];
    c = X.data[2];
    d = X.data[3];

    // Using formula for inverse
    float factor = (float) 1 / (a*d - b*c);

    X_inverse.data[0] = factor * d;
    X_inverse.data[3] = factor * a;
    X_inverse.data[1] = -1 * factor * b;
    X_inverse.data[2] = -1 * factor * c;

    return X_inverse;
}

// Calculate X*Y = Z
struct Matrix multiply_matrix_matrix(struct Matrix X, struct Matrix Y) {
    struct Matrix Z; int i, j, k; float res;
    Z.n = X.n; Z.m = Y.m;
    Z.data = (float*)malloc(Z.n * Z.m * sizeof(float));

    if (X.m != Y.n) {
        printf("ERROR in matrix-matrix multiplication: Dimensions do not match. Trying to multiply matrix X of dimensions %d x %d, with matrix Y of dimensions %d x %d \n", X.n, X.m, Y.n, Y.m);
        return Z;
    }

    for (i = 0; i < Z.n; i++) {
        for (j = 0; j < Z.m; j++) {
            // multiply row i of X by column j of Y

            res = 0.0f;
            for (k = 0; k < Z.m; k++) {
                res += X.data[i * X.n + k] * Y.data[k * Y.n + j];
            }

            Z.data[i * Z.n + j] = res;
        }
    }
    
    return Z;
}

// Calculate X*y = z
struct Vector multiply_matrix_vector(struct Matrix X, struct Vector y) {
    struct Vector z;

    return z;
}

// Multiply transpose of a matrix by a vector
void transpose_vector_multiply(void) {};

int main(void) {
    printf("Running Simple Linear Regression on Input from `data.txt`\n");

    // Loading in data 
    n = count_lines("data.txt");
    struct DataInputs data_inputs = read_data();


    // Perform linear regression
    struct Matrix X = gen_X(data_inputs.x_inputs);
    struct Matrix X_T = transpose_matrix(X);
    struct Matrix X_TX = multiply_matrix_matrix(X_T, X);

    // Free used memory
    free(data_inputs.x_inputs.data);
    free(data_inputs.y_inputs.data);
    free(X.data);
    free(X_T.data);
    free(X_TX.data);

    return 0;
}