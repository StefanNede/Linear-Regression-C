#include <stdio.h>

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

int main(void) {
    printf("Running Multiple Linear Regression on Input from `data.txt`\n");
    return 0;
}