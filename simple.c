// Simple linear regression -- 2 variables

#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h> 
#include "pbPlots.h"
#include "supportLib.h"

/* VARIABLES with types
    n - int - number of data point pairs to run regression on
    b - 2x1 vector - (c, m)T
    y - nx1 vector - inputs (y_1, y_2, ..., y_n)T
    X - nx2 matrix - 1st column is all 1s and 2nd column is nx1 vector of inputs (x_1, x_2, ..., x_n)T
*/

/* NORMAL EQUATIONS solution (solve for b)
    b = (X_T*X)^(-1) * X_T * y
*/

// GLOBALS -------------------------------
volatile int n;

// STRUCTS -------------------------------

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
    return count+1;
}

// Read the data input from the csv file
struct DataInputs read_data(void) {
    FILE *fptr;  
    struct DataInputs data_inputs;
    char c, line[100];
    int onx = 1, i = 0; // flag: 1 - on x field of input, 0 - on y field of input
    double x, y;

    data_inputs.x_inputs.size = n;
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

// Generate the nx2 X matrix 
struct Matrix gen_X(struct Vector x_values)  {
    struct Matrix X; int i;

    X.n = x_values.size;
    X.m = 2;
    X.data = (double*)malloc(X.n * X.m * sizeof(double));

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
    X_T.data = (double*)malloc(X_T.n * X_T.m * sizeof(double));

    // data[i][j] = data[j][i]
    for (i = 0; i < X.n; i++) {
        for (j = 0; j < X.m; j++) {
            X_T.data[j * X_T.m + i] = X.data[i * X.m + j];
        }
    }

    return X_T;
}

// Invert the 2x2 matrix provided
struct Matrix invert_matrix_2by2(struct Matrix X) {
    struct Matrix X_inverse;
    double a, b, c, d, determinant;
    X_inverse.n = 2;
    X_inverse.m = 2;
    X_inverse.data = (double*)malloc(4 * sizeof(double));

    if (X.n != 2 || X.m != 2) {
        printf("ERROR in inverting 2x2 matrix. Dimensions of matrix X to invert are not 2x2 but are %dx%d\n", X.n, X.m);
        return X_inverse;
    }

    a = X.data[0];
    b = X.data[1];
    c = X.data[2];
    d = X.data[3];
    determinant = a*d - b*c;

    if (determinant == 0.0f) {
        printf("ERROR in inverting 2x2 matrix. Matrix is not invertible - has determinant of 0\n");
        return X_inverse;
    }

    // Using formula for inverse
    double factor = (double) (1 / determinant);

    X_inverse.data[0] = factor * d;
    X_inverse.data[3] = factor * a;
    X_inverse.data[1] = -1 * factor * b;
    X_inverse.data[2] = -1 * factor * c;

    return X_inverse;
}

// Calculate X*Y = Z
struct Matrix multiply_matrix_matrix(struct Matrix X, struct Matrix Y) {
    struct Matrix Z; int i, j, k; double res;
    Z.n = X.n; Z.m = Y.m;
    Z.data = (double*)malloc(Z.n * Z.m * sizeof(double));

    if (X.m != Y.n) {
        printf("ERROR in matrix-matrix multiplication: Dimensions do not match. Trying to multiply matrix X of dimensions %dx%d, with matrix Y of dimensions %dx%d\n", X.n, X.m, Y.n, Y.m);
        return Z;
    }

    for (i = 0; i < Z.n; i++) {
        for (j = 0; j < Z.m; j++) {
            // multiply row i of X by column j of Y

            res = 0;
            for (k = 0; k < X.m; k++) {
                // printf("%lf*%lf = %lf\n", X.data[i * X.m + k], Y.data[k * Y.m + j], X.data[i * X.m + k] * Y.data[k * Y.m + j]);
                res += X.data[i * X.m + k] * Y.data[k * Y.m + j];
            }
            // printf("(%d,%d): %lf\n\n", i, j, res);
            Z.data[i * Z.m + j] = res;
        }
    }
    
    return Z;
}

// Calculate X*y = z
struct Vector multiply_matrix_vector(struct Matrix X, struct Vector y) {
    struct Vector z; int i, j; double res;
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

void print_matrix(struct Matrix X) {
    int i, j;
    printf("PRINTING MATRIX X:\n");
    for (i = 0; i < X.n; i++) {
        for (j = 0; j < X.m; j++) {
            printf("%lf ", X.data[i * X.m + j]);
        }
        printf("\n");
    }
}

// TODO: PLOTTING THE DATA POINTS AND LINEAR REGRESSION LINE GENERATED
void plot_results(struct DataInputs data_inputs, struct Vector c_m) {
    // RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    // Draw Input Points
	// DrawScatterPlot(canvasReference, 600, 400, data_inputs.x_inputs.data, data_inputs.x_inputs.size, data_inputs.y_inputs.data, data_inputs.y_inputs.size);
    // Draw regression line
   
    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = data_inputs.x_inputs.data;
	series->xsLength = data_inputs.x_inputs.size;
	series->ys = data_inputs.y_inputs.data;
	series->ysLength = data_inputs.y_inputs.size;
	series->linearInterpolation = false;
    series->pointType = L"circles";
	series->pointTypeLength = wcslen(series->pointType);
	// series->lineType = L"dashed";
	// series->lineTypeLength = wcslen(series->lineType);
	// series->lineThickness = 2;
	// series->color = GetGray(0.3);

	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    settings->title = L"Linear Regression";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"X axis";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"Y axis";
    settings->yLabelLength = wcslen(settings->yLabel);

	ScatterPlotSeries *s [] = {series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 1;

    RGBABitmapImageReference *canvasReference= CreateRGBABitmapImageReference();
    DrawScatterPlotFromSettings(canvasReference, settings);


    // Convert to PNG file and save it 
	size_t length;
	double *pngdata = ConvertToPNG(&length, canvasReference->image);
	WriteToFile(pngdata, length, "simple_regression.png");
	DeleteImage(canvasReference->image);
}

int main(void) {
    printf("Running Simple Linear Regression on Input from `data.txt`\n");

    // Loading in data 
    n = count_lines("data.txt");
    struct DataInputs data_inputs = read_data();


    // PERFORM SIMPLE LINEAR REGRESSION ===========
    struct Matrix X = gen_X(data_inputs.x_inputs);
    struct Matrix X_T = transpose_matrix(X);
    struct Matrix X_TX = multiply_matrix_matrix(X_T, X);
    struct Matrix inverse_X_TX = invert_matrix_2by2(X_TX);
    struct Matrix final_matrix = multiply_matrix_matrix(inverse_X_TX, X_T);
    // Final step is multiply final_matrix by y vector
    struct Vector res = multiply_matrix_vector(final_matrix, data_inputs.y_inputs);

    // OUTPUT RESULTS ===========
    // Printing in y = mx + c format, rounding coefficients to 2dp
    printf("y = %.2fx + %.2f\n", res.data[1], res.data[0]);
    plot_results(data_inputs, res);

    // Free used memory
    free(data_inputs.x_inputs.data);
    free(data_inputs.y_inputs.data);
    free(X.data);
    free(X_T.data);
    free(X_TX.data);
    free(inverse_X_TX.data);
    free(final_matrix.data);
    free(res.data);

    return 0;
}