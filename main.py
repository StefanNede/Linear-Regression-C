import numpy as np
import matplotlib.pyplot as plt

DATAPOINTS_FILE = "data.txt"
PLANE_FILE = "plane.txt"

# TODO: implement this
def read_datapoints():
    """ Take in the datapoints linear regression was run on from data.txt """
    datapoints = []

    with open(DATAPOINTS_FILE, "r") as file:
        for line in file:
            points = [int(i) for i in line.split(",")]
            datapoints.append(points)

    return datapoints

# TODO: implement this
def read_plane():
    """ Take in the plane coefficients from the result of linear regression from plane.txt """
    pass

# TODO: implement this -> currently has example 3d plane
def plot_3d():
    """ Plot the 3d regression plane and the datapoints it was based on """
    datapoints = read_datapoints()
    plane_coefficients = read_plane()

    # The plane equation is Ax + By + Cz = D
    A = 1
    B = 2
    C = 3
    D = 6

    # Create a grid of x and y values
    x = np.linspace(-5, 5, 10)
    y = np.linspace(-5, 5, 10)
    X, Y = np.meshgrid(x, y)

    # Calculate the corresponding z values
    Z = (D - A*X - B*Y) / C

    # Debug log
    print("Plotting the 3d regression plane and datapoints it's based on ...")

    # Create the 3D plot
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    # Plot the surface -> alpha=0.5 to make plane semi transparent to allow us to see the points
    ax.plot_surface(X, Y, Z, alpha=0.5, cmap='viridis')

    # Define and plot the 3D points
    # Example points (x, y, z)
    points = np.array([
        [1, 1, 1],    # A point on the plane (1*1 + 2*1 + 3*1 = 6)
        [3, 0, 0],    # A point off the plane
        [-2, 5, 2]    # Another point off the plane
    ])

    # Plot the points
    ax.scatter(points[:, 0], points[:, 1], points[:, 2], color='red', s=10, label='Data Points')

    # Set labels for the axes
    ax.set_xlabel('X axis')
    ax.set_ylabel('Y axis')
    ax.set_zlabel('Z axis')

    plt.title('3D Plane: x + 2y + 3z = 6')
    plt.show()

if __name__ == "__main__":
    plot_3d()