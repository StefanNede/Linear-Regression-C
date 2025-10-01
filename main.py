from ctypes import *
import os
import numpy as np
import matplotlib.pyplot as plt

# GLOBALS + CONSTANTS
DATAPOINTS_FILE = "data.txt"
PLANE_FILE = "plane.txt"
CUR_DIR = os.getcwd()
SIMPLE_SO_FILE = f"{CUR_DIR}/simple_export.so"
MULTIPLE_SO_FILE = f"{CUR_DIR}/multi_export.so"
is_3d = False # flag to indicate whether regression is in 3D

def read_datapoints():
    """ Take in the datapoints linear regression was run on from data.txt """
    datapoints = []

    with open(DATAPOINTS_FILE, "r") as file:
        for line in file:
            line = line.strip()
            points = [float(i) for i in line.split(",")]
            # move dependent variable to the end
            points = points[1:] + [points[0]]
            datapoints.append(points)

    return datapoints

def read_plane():
    """ Take in the plane coefficients from the result of linear regression from plane.txt

    Returns
    -------
    plane_coefficients : list[float]
        Returns [scalar_value, coefficient_1, coefficient_2, ...]
    """
    plane_coefficients = []

    with open(PLANE_FILE, "r") as file:
        for line in file:
            line = line.strip()
            plane_coefficients = [float(i) for i in line.split(",")]

    return plane_coefficients

def plot_3d():
    """ Plot the 3d regression plane and the datapoints it was based on """
    datapoints = read_datapoints()
    plane_coefficients = read_plane()

    # Plotting 3d data check
    assert len(datapoints[0]) == 3 
    assert len(plane_coefficients) == 3

    # Define the 3D points
    # Example points (x, y, z)
    points = np.array(datapoints)

    x_points = points[:, 0]
    y_points = points[:, 1]
    z_points = points[:, 2]

    # The plane equation is Cz = D + Ax + By where C = 1
    A = plane_coefficients[1]
    B = plane_coefficients[2]
    C = 1
    D = plane_coefficients[0]

    # Create a grid of x and y values
    min_x, max_x = min(x_points), max(x_points)
    deviation_x = abs(max_x - min_x)
    min_y, max_y = min(y_points), max(y_points)
    deviation_y = abs(max_y - min_y) # use this to give boundary around points for nicer plotting

    x = np.linspace(min_x - deviation_x/2, max_x + deviation_x/2)
    y = np.linspace(min_y - deviation_y/2, max_y + deviation_y/2)
    X, Y = np.meshgrid(x, y)

    # Calculate the corresponding z values
    Z = (D + A*X + B*Y) / C

    # Debug log
    print("Plotting the 3d regression plane and datapoints it's based on ...")

    # Create the 3D plot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")

    # Plot the surface -> alpha=0.5 to make plane semi transparent to allow us to see the points
    ax.plot_surface(X, Y, Z, alpha=0.5, cmap="viridis")

    # Plot the 3D points
    ax.scatter(x_points, y_points, z_points, color="red", s=10, label="Data Points")

    # Set labels for the axes
    ax.set_xlabel("X axis")
    ax.set_ylabel("Y axis")
    ax.set_zlabel("Z axis")

    # TODO: make this title nicer
    plt.title(f"3D Plane: {A}x + {B}y - {C}z = -{D}")
    plt.savefig("multiple_regression.png")
    plt.show()

# Use the efficient C implementations via the shared libraries created
def run_regression():
    global is_3d

    simple_regression_functions = CDLL(SIMPLE_SO_FILE)
    multiple_regression_functions = CDLL(MULTIPLE_SO_FILE)

    datapoints = read_datapoints()
    
    if len(datapoints[0]) == 2:
        simple_regression_functions.simple_regression()
        return
    elif len(datapoints[0]) == 3:
        is_3d = True
    else:
        is_3d = False

    multiple_regression_functions.multiple_regression()

if __name__ == "__main__":
    run_regression()
    if is_3d:
        plot_3d()