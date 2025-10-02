#include <stdio.h>
#include "linalg.h"

// STRUCTS
struct DataInputs;
typedef struct DataInputs DataInputs;

// Struct for the 2 vector inputs of x and y values
struct DataInputs {
    Matrix x_inputs;
    Vector y_inputs;
};

// FUNCTION DEFINITIONS

// Data handling
void set_lines_dimensions(char *filename);
DataInputs read_data(void);

// Debugging
void print_plane(Vector *coefficients);
void save_plane(Vector *coefficients);
void testing(void);
void test_back_sub(void);

// Orchestration
void multiple_regression(void);
int main(void);