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

// ADVANCED TECHNIQUES ------

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

// DEBUGGING -----

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

// Print out a vector for debugging purposes
void print_vector(Vector x) {
    int i;
    printf("PRINTING VECTOR x:\n");
    for (i = 0; i < x.size; i++) {
        printf("%lf ", x.data[i]);
    }
    printf("\n");
}