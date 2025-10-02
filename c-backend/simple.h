#include <stdio.h>
#include "linalg.h"

// STRUCTS
struct DataInputs;
typedef struct DataInputs DataInputs;

// Struct for the 2 vector inputs of x and y values
struct DataInputs {
    Vector x_inputs;
    Vector y_inputs;
};

// FUNCTION DEFINITIONS
int count_lines(char *filename);
double *get_padded_points(double *points, double min, double max, int length, double pad_amount);

DataInputs read_data(void);

Matrix gen_X(Vector x_values);
Vector get_min_max(double *data_values, int length);

void plot_results(DataInputs data_inputs, Vector c_m);
void simple_regression(void);