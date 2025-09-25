#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "multi.h"

/* TODO: 'for further work' comment
   Note: we are using classical Gram Schmidt here which is potentially numerically unstable 
    -> if encounter issues switch to the modified gram schmidt method for better stability  :) 
*/

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
    mag = pow(mag, 1.0 / 2.0);

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

// Transpose a matrix 
Matrix transpose_matrix(Matrix X) {
    Matrix X_T; int i, j;

    X_T.n = X.m; X_T.m = X.n;
    X_T.data = (double*)malloc(X_T.n * X_T.m * sizeof(double));

    // data[i][j] = data[j][i]
    for (i = 0; i < X.n; i++) {
        for (j = 0; j < X.m; j++) {
            X_T.data[j * X_T.m + i] = X.data[i * X.m + j];
        }
    }

    return X_T;
}

// Calculate X*y = z
Vector multiply_matrix_vector(Matrix X, Vector y) {
    Vector z; int i, j; double res;
    z.size = X.n;
    z.data = (double*)malloc(sizeof(double) * X.n);

    if (X.m != y.size) {
        printf("ERROR in matrix vector multiplication. Dimensions do not match. Trying to multiply %dx%d matrix X with %dx1 vector y\n", X.n, X.m, y.size);
    }

    for (i = 0; i < z.size; i++) {
        // multiply row i of X by y

        res = 0;
        for (j = 0; j < y.size; j++) {
            // printf("%lf, %lf\n", X.data[i*X.m + j], y.data[j]);
            res += X.data[i * X.m + j] * y.data[j];
        }

        z.data[i] = res;
    }

    return z;
}

// Returns whether the matrix X is upper triangular (1) or not (0)
int is_upper_triangular(Matrix *X) {
    int i, j;
    for (i = 0; i < X->n; i++) {
        for (j = 0; j < i; j++) {
            if (X->data[i*X->m + j] != 0.0f) {
                return 0;
            }
        }
    }
    return 1;
}

// Solve upper triangular system via back substitution: UT * x = y
Vector solve_back_sub(Matrix UT, Vector y)  {
    Vector x; int i,j;
    x.size = UT.m;
    x.data = (double*)malloc(sizeof(double)*x.size);

    // Input validation
    if (UT.n != UT.m) {
        printf("ERROR in solving upper-triangular system UT*x = y. Dimensions of matrix UT is %dx%d, and it should be square for a consistent system.\n", UT.n, UT.m);
        return x;
    }

    if (UT.n != y.size) {
        printf("ERROR in solving upper-triangular system UT*x = y. Dimensions of matrix UT is %dx%d and of vector y is %dx1\n", UT.n, UT.m, y.size);
        return x;
    }

    if (is_upper_triangular(&UT) == 0) {
        printf("ERROR in solving upper-triangular system UT*x = y. Matrix UT is not upper triangular.\n");
        return x;
    }

    // Back substitution
    for (i = x.size - 1; i >= 0; i--) {
        double coeff = UT.data[i * UT.m + i];
        double res = y.data[i];

        // Avoid division by 0 error
        if (coeff == 0.0f) {
            printf("BEWARE: in solving upper-triangular system UT*x = y. There exists a 0 on the diagonal of matrix UT, making the system have infinite solutions.\n");
            continue;
        }
        
        // Substitute discovered values
        for (j = UT.m-1; j > i; j--) {
            res -= UT.data[i * UT.m + j] * x.data[j];
        }
        res /= coeff;
        x.data[i] = res;
    }

    return x;
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
            // save r_ji to matrix R
            res.R.data[j*res.R.m + i] = r_ji;

            // Q_i = Q_i - r_ji * Q_j
            multiply_scalar_vector_inplace(r_ji, &Q_j);
            subtract_vector_vector_inplace(&Q_i, Q_j);
        }

        // Q_i = Q_i / |Q_i|
        // r_ii = |Q_i|
        double r_ii = get_magnitude(Q_i);
        // save r_ii to matrix R
        res.R.data[i*res.R.m + i] = r_ii;

        multiply_scalar_vector_inplace(1/r_ii, &Q_i);
        // Move Q_i back into the corresponding column of the Q matrix 
        copy_column_to_matrix_inplace(Q_i, &res.Q, i);
    }

    return res;
}

// Testing the function that solves a consistent square upper triangular system via back substitution
void test_back_sub(void) {
    Matrix UT; Vector y;
    UT.n = UT.m = 3; y.size = 3;
    UT.data = (double*)malloc(sizeof(double)*UT.n*UT.m); y.data = (double*)malloc(sizeof(double)*y.size);

    UT.data[0] = 2;
    UT.data[1] = 1;
    UT.data[2] = -1;
    UT.data[3] = 0;
    UT.data[4] = 3;
    UT.data[5] = 2;
    UT.data[6] = 0;
    UT.data[7] = 0;
    UT.data[8] = 4;

    y.data[0] = 5;
    y.data[1] = 4;
    y.data[2] = 8;

    Vector x = solve_back_sub(UT, y);
    print_vector(x);
    if (x.data[0] == 3.5f && x.data[1] == 0.0f && x.data[2] == 2.0f){
        printf("Back substitution test passed :)\n");
    } else{
        printf("ERROR: Back substitution test failed\n");
    }

    free(UT.data);
    free(y.data);
}

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

    test_back_sub();

    copy_column_to_matrix_inplace(x, &y, 0);
    print_matrix(y);
    copy_column_to_matrix_inplace(x, &y, 1);
    print_matrix(y);
    free(x.data);
    free(y.data);
}

// Print a plane given the coefficients necessary
void print_plane(Vector *coefficients) {
    int i;
    printf("Y = ");
    printf("%lf", coefficients->data[0]);
    for (i = 1; i < coefficients->size; i++) {
        printf(" + %lf*X_%d", coefficients->data[i], i);
    }
    printf("\n");
}

// Save the plane coefficients to a csv file for 3d plotting in python
void save_plane(Vector *coefficients) {
    int i;
    FILE *fptr;
    fptr = fopen("plane.txt", "w");
    for (i = 0; i < coefficients->size-1;i++) {
        fprintf(fptr, "%lf,", coefficients->data[i]);
    }
    fprintf(fptr, "%lf", coefficients->data[coefficients->size-1]);
    fclose(fptr);
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
    // print_matrix(qr.Q);
    // print_matrix(qr.R);

    // PERFORM MULTIPLE LINEAR REGRESSION ===========
    // R*b = Q_T * y
    Matrix Q_T = transpose_matrix(qr.Q);
    Vector z = multiply_matrix_vector(Q_T, data_inputs.y_inputs);
    Vector b = solve_back_sub(qr.R, z);
    printf("Your regression plane equation is:\n");
    print_plane(&b);
    save_plane(&b);

    // Free used memory
    free(data_inputs.x_inputs.data);
    free(data_inputs.y_inputs.data);
    free(qr.Q.data);
    free(qr.R.data);
    free(Q_T.data);
    free(z.data);
    free(b.data);
}

int main(void) {
    printf("DISCLAIMER: MULTIPLE LINEAR REGRESSION IS CURRENTLY STILL PRONE TO INSTABILITY ISSUES.\n");
    multiple_regression();
    return 0;
}