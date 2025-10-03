# Linear Regression Model in C

A linear regression model that uses OLS (ordinary least squares), the normal equations, and the Gram-Schmidt method for QR factorisation to enable efficient multiple regression (LaTeX file on mathematical background coming soon).

___

## Features
* **C Library** for both simple and multiple linear regression that you can port into any applications you might have
* **Python UI** and **3D Interactive Plotting** for a better user experience

---

## Requirements

* **Base** - simply a **C compiler** (all implementations of linear algebra operations and graphing have been implemented from scratch)!
* **Advanced Plotting / Interactive UI** - If you want 3d plotting then you need **python3**, with packages numpy, matplotlib, and customtkinter:
  ```bash
  pip install -r requirements.txt
  ```

---

## File Structure

```code
├── 📁 data/
│   ├── 📝 data.txt          # Holds the data regression will be run on in CSV format (dependent variable first).
│   └── 📝 plane.txt         # Holds the regression plane coefficients.
│
├── 📁 app/
│   ├── 🐍 main.py           # For interfacing with the regression model through Python in the terminal.
│   ├── 🐍 plot3d.py         # Handles 3D plotting for multiple regression.
│   ├── 🐍 ui.py             # For interfacing with the regression model through an interactive app.
│   └── 🐍 utils.py          # Utility functions for the Python app.
│
└── 📁 c-backend/
    ├── Makefile            # Build script for compiling the C backend.
    ├── C linalg.c          # C implementation of linear algebra functions.
    ├── H linalg.h          # Header for linear algebra functions.
    ├── C main.c            # Main entry point for the C backend logic.
    ├── C multi.c           # Functions for multiple linear regression.
    ├── H multi.h           # Header for multiple linear regression.
    ├── C pbPlots.c         # Plotting functions for a 2D plotting library.
    ├── H pbPlots.h         # Header for plotting functions.
    ├── C simple.c          # Functions for simple linear regression.
    ├── H simple.h          # Header for simple linear regression.
    ├── C supportLib.c      # Supporting library functions for plotting library.
    └── H supportLib.h      # Header for the support library.
```

--- 
## Mathematical Background 

I will develop a LaTeX file going over the in-depth mathematics for the project soon, but in the meantime...

<br/>  

Here are some resources for further reading:
- Ordinary Least Squares:
  - [Wikipedia: Ordinary Least Squares](https://en.wikipedia.org/wiki/Ordinary_least_squares)
  - [Built In: OLS Regression](https://builtin.com/data-science/ols-regression)

- QR Factorisation:
  - [Wikipedia: QR Decomposition](https://en.wikipedia.org/wiki/QR_decomposition)
  - [DataCamp: QR Decomposition in Machine Learning](https://www.datacamp.com/tutorial/qr-decomposition)

- Gram-Schmidt Process:
  - [Wikipedia: Gram-Schmidt process](https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process)
  - [RPubs: QR Decomposition with Gram-Schmidt](https://rpubs.com/aaronsc32/qr-decomposition-gram-schmidt)

---

## How To Use
There are 3 different ways you can use the repository. You can either...
* Interface directly with the C files in the terminal or plug them into your own code (shared libraries generated)
* Use a simple Python orchestration file, still staying in the terminal but getting 3D plots
* Use a comprehensive Python app with a proper UI


### UI 
1. Run the following in the terminal to compile the C backend shared libraries:
   ```bash
   cd c-backend
   make all
   cd ..
   ```
2. Run the following in the terminal to start the Python app
   ```bash
   cd app
   python3 ui.py
   ```
3. Interface with the easy-to-use UI (check demo video further down for example)

### Python Orchestration
1. Write your input points into `data/data.txt` in a csv format (the dependent variable is the first entry)
2. Run the following in the terminal to compile the C backend shared libraries:
   ```bash
   cd c-backend
   make all
   cd ..
   ```
3. Run the following in the terminal to run linear regression on your data:
   ```bash
   cd app
   python3 main.py
   ```
   3D plots will be immediately shown and saved in `data/multiple_regression.png`; 2D plots will be saved in `data/simple_regression.png`

### Simple Regression
1. Write your input points into `data/data.txt` in a csv format (the dependent variable is the first entry)
2. Run the following in the terminal:
   ```bash
   cd c-backend
   make simple
   ./simple
   ```
   The equation for the line of best fit will be output in the terminal, and the graph (graphed in C) will be saved to `simple_regression.png`.

### Multiple Regression
1. Write your input points into `data/data.txt` in a csv format (the dependent variable is the first entry)
2. Run the following in the terminal:
   ```bash
   cd c-backend
   make multi
   ./multi
   ```
3. The equation for the plane of best fit will be output in the terminal.
4. To graph run the following in the terminal:
   ```bash
   cd ../app
   python3 plot3d.py
   ```
   The graph will be shown and saved to `data/multiple_regression.png`.

---

## Examples 

### UI
This is what the base UI looks like:
<img width="800" height="640" alt="Screenshot 2025-10-03 at 17 54 58" src="https://github.com/user-attachments/assets/c241c43e-3b31-4b74-8cfe-cc0f92adc017" />
<img width="800" height="640" alt="Screenshot 2025-10-03 at 17 55 34" src="https://github.com/user-attachments/assets/fb66c04b-b9f6-4b7f-99a1-7cde37d3e2ff" />

Check the video in `demos>app_demo.mov` for a demonstration of using the app.

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

---

## Coming Soon...
1. LaTeX file explaining mathematical background
2. A more numerically stable method for QR factorisation (modified Gram-Schmidt for example)
