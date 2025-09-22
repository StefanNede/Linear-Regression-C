# Linear Regression Model in C

## Requirements
Simply a C compiler (all implementations of linear algebra operations and graphing have been implemented from scratch)!

## File Structure

* simple.c -> runs simple linear regression on 2 variables
* multi.c -> runs multiple linear regression on >2 variables
* data.txt -> holds the data that we will run linear regression where each new line is a new point in the format of `x,y,z,...` (make sure no new line on the last line) -> the first value in each line is the dependent variable, and the rest are the explanatory ones

For plotting 3d points and plane generated, write code to do this in python -> save results to a csv and then have a python program that reads these in and executes generating the 3d plot.

For dimensions higher than 3 do not bother with the plotting.

## How To Use
Write your input points into data.txt in a csv format.

### Simple Regression
Run `make simple_regression` followed by `./simple` in the terminal. The equation for the line of best fit will be output in the terminal, and the graph (graphed in C) will be saved to `simple_regression.png`.

### Multiple Regression
Run `make multi` followed by `./multi` in the terminal. The equation for the plane of best fit will be output in the terminal - graphing coming soon.

## Examples

Running simple linear regression on the data in `data>data_copy_2.txt` returns the following graph with parameters 1.90 and -16.77 (y = 1.90x + -16.77):

<img width="1000" height="800" alt="simple_regression" src="https://github.com/user-attachments/assets/14a50265-9769-4362-964d-ad63ff383dea" />

(Multiple regression examples coming soon).

## TODOs
1. Solve upper triangular system in multiple regression
2. Write python script for multi-dimensional plotting (save results of regression plane to a csv)
3. Complete README with examples of use
4. Add LaTeX file explaining mathematical background
5. Find a way to trigger execution of plotting python script upon termination of multiple regression
6. Implement numerically stable method for QR factorisation (modified Gram-Schmidt for example)
