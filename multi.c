#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multi.h"

/* VARIABLES with types
    p - int - number of dimensions we are running regression on
    n - int - number of data point tuples -> 1 dependent variable and p explanatory ones
    b - (p+1)x1 vector - (b_0, b_1, ... , b_p)^T -> the coefficients for the regression plane
    y - nx1 vector - dependent variable inputs (y_1, y_2, ..., y_n)^T
    X - nx(p+1) matrix - each row consists of 1 followed by the p explanatory variable values 
*/

/* QR FACTORISATION of X => X = QR
    Q - nx(p+1) matrix - orthonormal columns
    R - (p+1)x(p+1) matrix - upper triangular and square
*/

/* NORMAL EQUATIONS (solve for b), applying QR factorisation to X
    X_T * X * b = X_T * y 
 => R * b = Q_T * y (solve via back sub)
*/

// GLOBALS -------------------------------
volatile int n, p;

// FUNCTIONS -------------------------------

// Count the number of lines in an input file
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
    int onx = 1, i = 0; // flag: 1 - on x field of input, 0 - on y field of input
    double x, y;

    data_inputs.x_inputs.n= n;
    data_inputs.y_inputs.size = n;
    data_inputs.x_inputs.data = (double*)malloc(n*sizeof(double));
    data_inputs.y_inputs.data = (double*)malloc(n*sizeof(double));

    fptr = fopen("data.txt", "r");

    if (fptr != NULL) {
        while (fgets(line, sizeof(line), fptr)) {
            if (sscanf(line, "%lf,%lf", &x, &y) == 2) {
                data_inputs.x_inputs.data[i] = x;
                data_inputs.y_inputs.data[i] = y;
                i++;
            } else {
                fprintf(stderr, "Invalid line format: %s", line);
            }
        }
    } else {
        printf("Not able to open the file: `data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
}

void multiple_regression(void) {
    printf("Running Multiple Linear Regression on Input from `data.txt`\n");

    // Loading in data 
    set_lines_dimensions("data.txt");
    printf("Number of datapoints: %d\n", n);
    printf("Number of dimensions: %d\n", p);

    // DataInputs data_inputs = read_data();


    // PERFORM SIMPLE LINEAR REGRESSION ===========

    // Free used memory
    // free(data_inputs.x_inputs.data);
    // free(data_inputs.y_inputs.data);
}

int main(void) {
    multiple_regression();
    return 0;
}