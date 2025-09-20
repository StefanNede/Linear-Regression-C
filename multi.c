#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "multi.h"

/* VARIABLES with types
    p - int - number of dimensions we are running regression on
    n - int - number of data point tuples -> 1 dependent variable and p-1 explanatory ones
    b - px1 vector - (b_0, b_1, ... , b_p-1)^T -> the coefficients for the regression plane
    y - nx1 vector - dependent variable inputs (y_1, y_2, ..., y_n)^T
    X - nxp matrix - each row consists of 1 followed by the p-1 explanatory variable values 
*/

/* QR FACTORISATION of X => X = QR
    Q - nxp matrix - orthonormal columns
    R - pxp matrix - upper triangular and square
*/

/* NORMAL EQUATIONS (solve for b), applying QR factorisation to X
    X_T * X * b = X_T * y 
 => R * b = Q_T * y (solve via back sub)
*/

// GLOBALS -------------------------------
volatile int n, p;

// FUNCTIONS -------------------------------

// Count the number of lines in an input file 
// and the number of dimensions we are working with
void set_lines_dimensions(char *filename) {
    FILE *fptr; 
    char c;
    int count = 0; 
    int dimensions = 1;

    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Failed at counting lines for file: `%s`\n", filename);
        return;
    }

    for (c = getc(fptr); c != EOF; c = getc(fptr)) {
        if (c == ',') {
            dimensions++;
        }

        if (c == '\n') { 
            count++; 
            dimensions = 1;
        }
    }

    fclose(fptr);
    n = count+1;
    p = dimensions;
}

// Read the data input from the csv file
DataInputs read_data(void) {
    FILE *fptr;  
    DataInputs data_inputs;
    char c, line[1000];
    int line_index = 0;
    double x, y;

    data_inputs.x_inputs.n= n;
    data_inputs.x_inputs.m= p;
    data_inputs.y_inputs.size = n;
    data_inputs.x_inputs.data = (double*)malloc(n*p*sizeof(double));
    data_inputs.y_inputs.data = (double*)malloc(n*sizeof(double));

    fptr = fopen("data.txt", "r");

    if (fptr != NULL) {

        while (fgets(line, sizeof(line), fptr)) {
            char *line_ptr = line; // pointer to line we are currently on 
            int scanned_count = 0; // the number of characters successfully scanned in the line - how far we've advanced in the string
            double value; // last value scanned 

            // Loop p times to extract p values
            // first value is the dependent y variable, and the rest are the explanatory variables
            for (int i = 0; i < p; i++) {
                if (sscanf(line_ptr, "%lf%*[, \t]%n", &value, &scanned_count) == 1) { // scan the next double and consume a comma, storing number of chars successfully scanned so far into scanned count
                    if (i == 0) {
                        // First value is the dependent variable and each row of matrix X starts with a 1 
                        data_inputs.y_inputs.data[line_index] = value;
                        data_inputs.x_inputs.data[line_index*p] = 1.0f;
                    } else {
                        // Store values into the matrix X 
                        data_inputs.x_inputs.data[line_index*p + i] = value;
                    }

                    // Advance the pointer for the next scan
                    line_ptr += scanned_count;
                } else {
                    fprintf(stderr, "Invalid line format at line %d: %s\n", line_index+1, line);
                    break;
                }
            }
            line_index++;
        }

    } else {
        printf("Not able to open the file: `data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
}

// Print out a matrix for debugging purposes
void print_matrix(Matrix X) {
    int i, j;
    printf("PRINTING MATRIX X:\n");
    for (i = 0; i < X.n; i++) {
        for (j = 0; j < X.m; j++) {
            printf("%lf ", X.data[i * X.m + j]);
        }
        printf("\n");
    }
}

void print_vector(Vector x) {
    int i;
    printf("PRINTING VECTOR x:\n");
    for (i = 0; i < x.size; i++) {
        printf("%lf ", x.data[i]);
    }
    printf("\n");
}

// Return column i of matrix X
Vector get_column(Matrix X, int i) {
    Vector res;
    int j;
    res.size = X.n;
    res.data = (double*)malloc(res.size * sizeof(double));
    
    for (j = 0; j < X.n; j++) {
        res.data[j] = X.data[j * X.m + i];
    }

    return res;
}

// Return the magnitude of vector x
double get_magnitude(Vector x) {
    int i;
    double mag = 0.0f;

    for (i = 0; i < x.size; i++) {
        mag += x.data[i] * x.data[i];
    }
    mag = pow(mag, 1.0 / x.size);

    return mag;
}

// res = x_T * y
double multiply_vector_vector(Vector x, Vector y) {
    double res = 0.0f;
    int i;

    if (x.size != y.size) {
        printf("ERROR in dot product of 2 vectors. Dimensions of vector x is %dx1 and of vector y is %dx1\n", x.size, y.size);
        return 0.0f;
    }

    for (i = 0; i < x.size; i++) {
        res += x.data[i] * y.data[i];
    }

    return res;
}

// I will only use these functionalities in place and so the following sometimes take pass by reference (pointer) fields

// x = x - y
void subtract_vector_vector_inplace(Vector *x, Vector y) {
    int i;

    if (x->size != y.size) {
        printf("ERROR in subtracting 2 vectors. Dimensions of vector x is %dx1 and of vector y is %dx1\n", x->size, y.size);
        return;
    }

    for (i = 0; i < x->size; i++) {
        x->data[i] -= y.data[i];
    }

    return;
}

// x = r * x
void multiply_scalar_vector_inplace(double scalar, Vector *x) {
    int i;

    for (i = 0; i < x->size; i++) {
        x->data[i] *= scalar;
    }

    return;
}

// TODO: TEST THIS
// X_i = col where i = col_idx
void copy_column_to_matrix_inplace(Vector col, Matrix *X, int col_idx) {
    int i;

    if (col.size != X->n) {
        printf("ERROR in inserting column into matrix. Dimensions of vector is %dx1 and of destination matrix is %dx%d\n", col.size, X->n, X->m);
        return;
    }

    for (i = 0; i < col.size; i++) {
        X->data[i * X->m + col_idx] = col.data[i];
    }

    return;
}

// QR factorisation via Classical Gram-Schmidt
QR QR_factorise(Matrix X) {
    QR res;
    double magnitude;

    res.Q.n = X.n;
    res.Q.m = X.m;
    res.R.n = X.m;
    res.R.m = X.m;
    res.Q.data = (double*)malloc(res.Q.n*res.Q.m*sizeof(double));
    res.R.data = (double*)malloc(res.R.n*res.R.m*sizeof(double));

    // printf("%d x %d, %d x %d", res.Q.n, res.Q.m, res.R.n, res.R.m);

    int i, j;

    // loop over the columns of X
    for (i = 0; i < X.m; i++) {
        // generate corresponding orthonormal column of Q and necessary entries in R
        Vector X_i = get_column(X, i);
        Vector Q_i = get_column(X, i); // Q_i = X_i

        for (j = 0; j < i; j++) {
            Vector Q_j = get_column(res.Q, j);
            // r_ji = Q_j • X_i
            double r_ji = multiply_vector_vector(Q_j, X_i);
            // TODO: save r_ji to matrix R

            // Q_i = Q_i - r_ji * Q_j
            multiply_scalar_vector_inplace(r_ji, &Q_j);
            subtract_vector_vector_inplace(&Q_i, Q_j);
        }

        // Q_i = Q_i / |Q_i|
        // r_ii = |Q_i|
        double r_ii = get_magnitude(Q_i);
        // TODO: save r_ii to matrix R

        multiply_scalar_vector_inplace(1/r_ii, &Q_i);
        // Move Q_i back into the corresponding column of the Q matrix 
        copy_column_to_matrix_inplace(Q_i, &res.Q, i);
    }

    return res;
}

/* TODO: put this comment somewhere as a 'for further work' comment
   Note: we are using classical Gram Schmidt here which is potentially numerically unstable 
    -> if encounter issues switch to the modified gram schmidt method for better stability  :) 
*/

// Testing various functions briefly
void testing(void) {
    Vector x; Matrix y;
    x.size = 2; y.n = y.m = 2;
    x.data = (double*)malloc(sizeof(double)*2);
    x.data[0] = 1.0f; x.data[1] = 2.0f;
    y.data = (double*)malloc(sizeof(double)*4);
    y.data[0] = y.data[1] = y.data[2] = y.data[3] = 0.0;
    
    // Dot product function
    // double res = multiply_vector_vector(x,x);
    // printf("%lf\n", res);

    // subtract_vector_vector_inplace(&x, x);

    // multiply_scalar_vector_inplace(2.0f, &x);
    // print_vector(x);

    copy_column_to_matrix_inplace(x, &y, 0);
    print_matrix(y);
    copy_column_to_matrix_inplace(x, &y, 1);
    print_matrix(y);
    free(x.data);
    free(y.data);
}

void multiple_regression(void) {
    printf("Running Multiple Linear Regression on Input from `data.txt`\n");
    // testing();

    // Loading in data 
    set_lines_dimensions("data.txt");
    printf("Number of datapoints: %d\n", n);
    printf("Number of dimensions: %d\n", p);

    // Matrix X = data_inputs.x_inputs
    DataInputs data_inputs = read_data();
    print_matrix(data_inputs.x_inputs);
    
    // PERFORM QR FACTORISATION OF X ===========
    QR qr = QR_factorise(data_inputs.x_inputs);

    // TODO: PERFORM MULTIPLE LINEAR REGRESSION ===========

    // Free used memory
    free(data_inputs.x_inputs.data);
    free(data_inputs.y_inputs.data);
}



int main(void) {
    multiple_regression();
    return 0;
}