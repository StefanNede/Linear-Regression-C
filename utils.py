# Utility functions used in multiple python files

def read_datapoints(datapoints_file):
    """ Take in the datapoints linear regression was run on from data.txt """
    datapoints = []

    with open(datapoints_file, "r") as file:
        for line in file:
            line = line.strip()
            points = [float(i) for i in line.split(",")]
            # move dependent variable to the end
            points = points[1:] + [points[0]]
            datapoints.append(points)

    return datapoints

def read_plane(plane_file):
    """ Take in the plane coefficients from the result of linear regression from plane.txt

    Returns
    -------
    plane_coefficients : list[float]
        Returns [scalar_value, coefficient_1, coefficient_2, ...]
    """
    plane_coefficients = []

    with open(plane_file, "r") as file:
        for line in file:
            line = line.strip()
            plane_coefficients = [float(i) for i in line.split(",")]

    return plane_coefficients