// Simple linear regression -- 2 variables

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "simple.h"
#include "pbPlots.h"
#include "supportLib.h"
#include "linalg.h"

#define PLOT_PAD_AMOUNT 2.0

/* VARIABLES with types
    n - int - number of data point pairs to run regression on
    b - 2x1 vector - (c, m)^T -> the coefficients for the regression line
    y - nx1 vector - dependent inputs (y_1, y_2, ..., y_n)^T
    X - nx2 matrix - 1st column is all 1s and 2nd column is nx1 vector of inputs (x_1, x_2, ..., x_n)^T
*/

/* NORMAL EQUATIONS solution (solve for b)
    b = (X_T*X)^(-1) * X_T * y
*/

// GLOBALS -------------------------------
volatile int n;

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
DataInputs read_data(void) {
    FILE *fptr;  
    DataInputs data_inputs;
    char c, line[100];
    int onx = 1, i = 0; // flag: 1 - on x field of input, 0 - on y field of input
    double x, y;

    data_inputs.x_inputs.size = n;
    data_inputs.y_inputs.size = n;
    data_inputs.x_inputs.data = (double*)malloc(n*sizeof(double));
    data_inputs.y_inputs.data = (double*)malloc(n*sizeof(double));

    fptr = fopen("../data/data.txt", "r");

    if (fptr != NULL) {
        while (fgets(line, sizeof(line), fptr)) {
            // Input is in format 'y,x' as y is dependent and x explanatory
            if (sscanf(line, "%lf,%lf", &y, &x) == 2) {
                data_inputs.x_inputs.data[i] = x;
                data_inputs.y_inputs.data[i] = y;
                i++;
            } else {
                fprintf(stderr, "Invalid line format: %s", line);
            }
        }
    } else {
        printf("Not able to open the file: `../data/data.txt`\n");
    }

    fclose(fptr);

    return data_inputs;
}

// Generate the nx2 X matrix 
Matrix gen_X(Vector x_values)  {
    Matrix X; int i;

    X.n = x_values.size;
    X.m = 2;
    X.data = (double*)malloc(X.n * X.m * sizeof(double));

    for (i = 0; i < x_values.size; i++) {
        X.data[2*i] = 1.0;
        X.data[2*i + 1] = x_values.data[i];
    }

    return X;
}

// Returns (min, max) of data_values list
Vector get_min_max(double *data_values, int length) {
    Vector min_max;
    double min, max;
    int i;

    min_max.size = 2;
    min_max.data = (double*)malloc(sizeof(double) * 2);
    min = max = data_values[0];
    
    for (i = 0; i < length; i++) {
        if (data_values[i] < min) {
            min = data_values[i];
        }

        if (data_values[i] > max)  {
            max = data_values[i];
        }
    }

    min_max.data[0] = min;
    min_max.data[1] = max;

    return min_max;
}

// Pad an array by +- a PLOT_PAD_AMOUNT of the max and min on either side
double *get_padded_points(double *points, double min, double max, int length, double pad_amount) {
    double *padded_points;
    int i;

    padded_points = (double*)malloc(sizeof(double) * (length + 2));
    padded_points[0] = min - pad_amount;

    for (i = 0; i < length; i++) {
        padded_points[i+1] = points[i];
    }

    padded_points[i+1] = max + pad_amount;

    return padded_points;
}

// Plot the data points and linear regression line generated
void plot_results(DataInputs data_inputs, Vector c_m) {
    double c = c_m.data[0];
    double m = c_m.data[1];

    Vector min_max_x = get_min_max(data_inputs.x_inputs.data, data_inputs.x_inputs.size);
    Vector min_max_y = get_min_max(data_inputs.y_inputs.data, data_inputs.y_inputs.size);
    double min_x_dataset = min_max_x.data[0];
    double max_x_dataset = min_max_x.data[1];
    double min_y_dataset = min_max_y.data[0];
    double max_y_dataset = min_max_y.data[1];
    free(min_max_x.data);
    free(min_max_y.data);

    // Generate regression line inputs -> apply some padding to look nicer
    double min_x_regression = min_x_dataset - PLOT_PAD_AMOUNT/2;
    double max_x_regression = max_x_dataset + PLOT_PAD_AMOUNT/2;
    double xs [] = {min_x_regression, max_x_regression};
	double ys [] = {min_x_regression * m + c, max_x_regression * m + c};

    // Pad the data inputs to make sure they show up on the plot by adding artificial points
    double *padded_x_points = get_padded_points(data_inputs.x_inputs.data, min_x_dataset, max_x_dataset, data_inputs.x_inputs.size, PLOT_PAD_AMOUNT);
    double *padded_y_points = get_padded_points(data_inputs.y_inputs.data, min_y_dataset, max_y_dataset, data_inputs.y_inputs.size, PLOT_PAD_AMOUNT);

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    ScatterPlotSeries *regression_series= GetDefaultScatterPlotSeriesSettings();

    // Plot the input data
    series->xs = padded_x_points;
	series->xsLength = data_inputs.x_inputs.size + 2;
	series->ys = padded_y_points;
	series->ysLength = data_inputs.y_inputs.size + 2;
	series->linearInterpolation = false;
    series->pointType = L"circles";
	series->pointTypeLength = wcslen(series->pointType);
	series->color = GetBlack();

    // Plot the regression line
    regression_series->xs = xs;
	regression_series->xsLength = 2;
	regression_series->ys = ys;
	regression_series->ysLength = 2;
	regression_series->linearInterpolation = true;
	regression_series->lineType = L"dashed";
	regression_series->lineTypeLength = wcslen(regression_series->lineType);
	regression_series->lineThickness = 2;
	regression_series->color = GetGray(0.5);

    // Set axes data
	ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 1000;
	settings->height = 800;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
    settings->title = L"Linear Regression";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"Y-axis"; // for some reason these are the wrong way around
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"X-axis";
    settings->yLabelLength = wcslen(settings->yLabel);

    // Actually generate plot
	ScatterPlotSeries *s [] = {series, regression_series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 2;

    RGBABitmapImageReference *canvasReference= CreateRGBABitmapImageReference();
    DrawScatterPlotFromSettings(canvasReference, settings);

    // Convert to PNG file and save it 
	size_t length;
	double *pngdata = ConvertToPNG(&length, canvasReference->image);
	WriteToFile(pngdata, length, "../output/simple_regression.png");
    printf("Plot saved to `../output/simple_regression.png`\n");
	DeleteImage(canvasReference->image);

    free(padded_x_points);
    free(padded_y_points);
}

void simple_regression(void) {
    printf("Running Simple Linear Regression on Input from `../data/data.txt`\n");

    // Loading in data 
    n = count_lines("../data/data.txt");
    DataInputs data_inputs = read_data();


    // PERFORM SIMPLE LINEAR REGRESSION ===========
    Matrix X = gen_X(data_inputs.x_inputs);
    Matrix X_T = transpose_matrix(X);
    Matrix X_TX = multiply_matrix_matrix(X_T, X);
    Matrix inverse_X_TX = invert_matrix_2by2(X_TX);
    Matrix final_matrix = multiply_matrix_matrix(inverse_X_TX, X_T);
    // Final step is multiply final_matrix by y vector
    Vector res = multiply_matrix_vector(final_matrix, data_inputs.y_inputs);

    // OUTPUT RESULTS ===========
    // Printing in y = mx + c format, rounding coefficients to 2dp
    printf("Your regression line equation is:\n");
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
}

int main(void) {
    simple_regression();
    return 0;
}