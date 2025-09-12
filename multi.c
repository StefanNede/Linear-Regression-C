#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void multiple_regression(void) {
    printf("Running Multiple Linear Regression on Input from `data.txt`\n");

    // Loading in data 
    set_lines_dimensions("data.txt");
    printf("Number of datapoints: %d\n", n);
    printf("Number of dimensions: %d\n", p);

    // Matrix X = data_inputs.x_inputs
    DataInputs data_inputs = read_data();
    // print_matrix(data_inputs.x_inputs);
    
    // TODO: PERFORM QR FACTORISATION OF X ===========

    // TODO: PERFORM MULTIPLE LINEAR REGRESSION ===========

    // Free used memory
    free(data_inputs.x_inputs.data);
    free(data_inputs.y_inputs.data);
}

int main(void) {
    multiple_regression();
    return 0;
}