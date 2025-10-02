#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "multi.h"
#include "linalg.h"

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

    fptr = fopen("../data/data.txt", "r");

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
        printf("Not able to open the file: `../data/data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
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
    fptr = fopen("../data/plane.txt", "w");
    for (i = 0; i < coefficients->size-1;i++) {
        fprintf(fptr, "%lf,", coefficients->data[i]);
    }
    fprintf(fptr, "%lf", coefficients->data[coefficients->size-1]);
    fclose(fptr);
}

void multiple_regression(void) {
    printf("Running Multiple Linear Regression on Input from `../data/data.txt`\n");
    // testing();

    // Loading in data 
    set_lines_dimensions("../data/data.txt");
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