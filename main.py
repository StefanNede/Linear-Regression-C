import numpy as np
import matplotlib.pyplot as plt

DATAPOINTS_FILE = "data.txt"
PLANE_FILE = "plane.txt"

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

# TODO: implement this -> currently has example 3d plane
def plot_3d():
    """ Plot the 3d regression plane and the datapoints it was based on """
    datapoints = read_datapoints()
    plane_coefficients = read_plane()

    # plotting 3d data check
    assert len(datapoints[0]) == 3 
    assert len(plane_coefficients) == 3

    # The plane equation is Cz = D + Ax + By where C = 1
    A = plane_coefficients[1]
    B = plane_coefficients[2]
    C = 1
    D = plane_coefficients[0]

    # Create a grid of x and y values
    x = np.linspace(-5, 5, 10)
    y = np.linspace(-5, 5, 10)
    X, Y = np.meshgrid(x, y)

    # Calculate the corresponding z values
    Z = (D + A*X + B*Y) / C

    # Debug log
    print("Plotting the 3d regression plane and datapoints it's based on ...")

    # Create the 3D plot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Plot the surface -> alpha=0.5 to make plane semi transparent to allow us to see the points
    ax.plot_surface(X, Y, Z, alpha=0.5, cmap='viridis')

    # Define and plot the 3D points
    # Example points (x, y, z)
    points = np.array(datapoints)

    # TODO: generalise to more than 3 points
    # Plot the points
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], color='red', s=10, label='Data Points')

    # Set labels for the axes
    ax.set_xlabel('X axis')
    ax.set_ylabel('Y axis')
    ax.set_zlabel('Z axis')

    # TODO: fix this title
    plt.title('3D Plane: x + 2y + 3z = 6')
    plt.show()

if __name__ == "__main__":
    plot_3d()