#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "linalg.h"

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

// Invert the 2x2 matrix provided
Matrix invert_matrix_2by2(Matrix X) {
    Matrix X_inverse;
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
Matrix multiply_matrix_matrix(Matrix X, Matrix Y) {
    Matrix Z; int i, j, k; double res;
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