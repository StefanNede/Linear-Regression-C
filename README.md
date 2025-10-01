# Linear Regression Model in C

Currently using least squares and the Gram-Schmidt method for QR factorisation to enable efficient multiple regression (LaTeX file on mathematical background coming soon)...

## Requirements
Simply a C compiler (all implementations of linear algebra operations and graphing have been implemented from scratch)!
If you want 3d plotting then you need python3, numpy, and matplotlib.

## File Structure

* simple.c -> runs simple linear regression on 2 variables
* multi.c -> runs multiple linear regression on >2 variables
* data.txt -> holds the data that we will run linear regression where each new line is a new point in the format of `x,y,z,...` (make sure no new line on the last line) -> the first value in each line is the dependent variable, and the rest are the explanatory ones
* plane.txt -> holds the regression plane generated in the format of coefficients for the plane equation, i.e. if plane Z = C + A*X + B*Y then saved as C,A,B

For plotting 3d points and plane generated, write code to do this in python -> save results to a csv and then have a python program that reads these in and executes generating the 3d plot.

For dimensions higher than 3 do not bother with the plotting.

## How To Use
Write your input points into data.txt in a csv format (the dependent variable is the first entry).

### Simple Regression
Run `make simple_regression` followed by `./simple` in the terminal. The equation for the line of best fit will be output in the terminal, and the graph (graphed in C) will be saved to `simple_regression.png`.

### Multiple Regression
Run `make multi` followed by `./multi` in the terminal. The equation for the plane of best fit will be output in the terminal.
To graph run `python3 main.py` -> graph will be shown and saved to `multiple_regression.png`.

## Examples

### Simple Linear Regression Plots (2D)
Running simple linear regression on the data in `data>data_copy_2.txt` returns the following graph with parameters 1.90 and -16.77 (`y = 1.90x + -16.77`):

<img width="800" height="640" alt="simple_regression" src="https://github.com/user-attachments/assets/14a50265-9769-4362-964d-ad63ff383dea" />

### Multiple Linear Regression Plots (3D)

Running multiple linear regression on the data in `data>data_copy_3.txt` returns the following graph with parameters 1.97, 3.12, and -13.27 (`z = 1.97x + 3.12y - 13.27`):

[![Example 1]](https://github.com/user-attachments/assets/a74e0000-86fb-4be0-b989-c441bb1e889f)


On a less correlated example `data>data_copy_4.txt` we get the parameters 3.59, 4.00, and -261.40 (`z = 3.59x + 4.00y - 261.40`):

[![Example 2]](https://github.com/user-attachments/assets/358c6543-ca2a-4331-ab12-839e40a03858)

Note: we can also display less correlated scenarios although it is hard visually to tell whether the plane is correct (one must trust the mathematics here)


### Multiple Linear Regression Plane Equations (4D+)

On a 4D example of 50 data points generated to have noise around the plane w = 2x + 3y + z (`data>data_copy_5.txt`), we get the parameters 2.79, 2.62, 1.08, and -34.87 (`w = 2.79x + 2.62y + 1.08z - 34.87`). This is indicative of a correct output.

On a 10D example (`data>data_copy_6.txt`) we get the plane equation
`y = -343821340670654.56 + 5.3x_1 + -1.38x_2 + -201.33x_3 + 19154866637184.25x_4 + -161032246183913.375x5 + -52591697583861.63x_6 + 181283712952031.41x_7 + -264.58x_8 + 3.09x_9` (result was instantaneous -> very good performance)

## TODOs
1. Fix file structure 
2. Add LaTeX file explaining mathematical background
3. Find a way to trigger execution of plotting python script upon termination of multiple regression
4. Implement numerically stable method for QR factorisation (modified Gram-Schmidt for example)
