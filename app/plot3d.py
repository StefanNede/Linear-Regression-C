# 3D plotting code as last step in multiple linear regression

from utils import read_datapoints, read_plane
import numpy as np
import matplotlib.pyplot as plt

# Globals + Constants
DEFAULT_DATAPOINTS_FILE = "../data/data.txt"
DEFAULT_PLANE_FILE = "../data/plane.txt"

def plot_3d(datapoints_file, plane_file):
    """ Plot the 3d regression plane and the datapoints it was based on """
    datapoints = read_datapoints(datapoints_file)
    plane_coefficients = read_plane(plane_file)

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
    plt.savefig("../output/multiple_regression.png")

    return fig

if __name__ == "__main__":
    fig = plot_3d(DEFAULT_DATAPOINTS_FILE, DEFAULT_PLANE_FILE)
    plt.show()