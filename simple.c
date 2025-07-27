// Simple linear regression -- 2 variables

#include <stdlib.h>
#include <stdio.h>

/* VARIABLES with types
    n - int - number of data point pairs to run regression on
    b - 2x1 vector - (c, m)T
    y - nx1 vector - inputs (y_1, y_2, ..., y_n)T
    X - nx2 matrix - 1st column is all 1s and 2nd column is nx1 vector of inputs (x_1, x_2, ..., x_n)T
*/

/* NORMAL EQUATIONS solution (solve for b)
    b = (XT*X)^(-1) * XT * y
*/

// GLOBAL variables
volatile int n;

// structs
struct DataInputs {
    float* x_inputs;
    float* y_inputs;
};

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

    data_inputs.x_inputs = (float*)malloc(n*sizeof(float));
    data_inputs.y_inputs = (float*)malloc(n*sizeof(float));

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
                    (data_inputs.x_inputs)[i] = atof(&c);
                } else {
                    (data_inputs.y_inputs)[i] = atof(&c);
                }
            }
        }
    } else {
        printf("Not able to open the file: `data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
}

// Invert the 2x2 matrix provided
void invert_matrix_2by2(void) {};

// Transpose a matrix
void tranpose_matrix(void) {};

// Calculate XT*X 
void tranpose_square(void) {};

// Multiply transpose of a matrix by a vector
void transpose_vector_multiply(void) {};

int main(void) {
    printf("Running Simple Linear Regression on Input from `data.txt`\n");

    // Loading in data 
    n = count_lines("data.txt");
    struct DataInputs data_inputs = read_data();

    // Free used memory
    free(data_inputs.x_inputs);
    free(data_inputs.y_inputs);

    return 0;
}