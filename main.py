import numpy as np
import matplotlib.pyplot as plt

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