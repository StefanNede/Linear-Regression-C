# Linear Regression Model in C

Currently using least squares and the Gram-Schmidt method for QR factorisation to enable efficient multiple regression (LaTeX file on mathematical background coming soon)...

## Requirements
Simply a C compiler (all implementations of linear algebra operations and graphing have been implemented from scratch)!

## File Structure

* simple.c -> runs simple linear regression on 2 variables
* multi.c -> runs multiple linear regression on >2 variables
* data.txt -> holds the data that we will run linear regression where each new line is a new point in the format of `x,y,z,...` (make sure no new line on the last line) -> the first value in each line is the dependent variable, and the rest are the explanatory ones
* plane.txt -> holds the regression plane generated in the format of coefficients for the plane equation, i.e. if plane Z = C + A*X + B*Y then saved as C,A,B

For plotting 3d points and plane generated, write code to do this in python -> save results to a csv and then have a python program that reads these in and executes generating the 3d plot.

For dimensions higher than 3 do not bother with the plotting.

## How To Use
Write your input points into data.txt in a csv format.

### Simple Regression
Run `make simple_regression` followed by `./simple` in the terminal. The equation for the line of best fit will be output in the terminal, and the graph (graphed in C) will be saved to `simple_regression.png`.

### Multiple Regression
Run `make multi` followed by `./multi` in the terminal. The equation for the plane of best fit will be output in the terminal - graphing coming soon.

## Examples

### Simple Linear Regression Plots (2D)
Running simple linear regression on the data in `data>data_copy_2.txt` returns the following graph with parameters 1.90 and -16.77 (y = 1.90x + -16.77):

<img width="1000" height="800" alt="simple_regression" src="https://github.com/user-attachments/assets/14a50265-9769-4362-964d-ad63ff383dea" />

### Multiple Linear Regression Plots (3D)

Running multiple linear regression on the data in `data>data_copy_3.txt` returns the following graph with parameters 1.97, 3.12, and -13.27 (z = 1.97x + 3.12y - 13.27):

[![Example 1]](https://github.com/user-attachments/assets/a74e0000-86fb-4be0-b989-c441bb1e889f)



(Insert less correlated example here)

### Multiple Linear Regression Plane Equations (4D+)

(Insert numerical examples here)

## TODOs
1. Complete README with examples of use
2. Add LaTeX file explaining mathematical background
3. Find a way to trigger execution of plotting python script upon termination of multiple regression
4. Implement numerically stable method for QR factorisation (modified Gram-Schmidt for example)
