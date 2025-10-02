from ctypes import *
from utils import read_datapoints, read_plane
from plot3d import plot_3d
import os

# Globals + Constants
DATAPOINTS_FILE = "data.txt"
PLANE_FILE = "plane.txt"
CUR_DIR = os.getcwd()
SIMPLE_SO_FILE = f"{CUR_DIR}/simple_export.so"
MULTIPLE_SO_FILE = f"{CUR_DIR}/multi_export.so"
is_3d = False # flag to indicate whether regression is in 3D

# Use the efficient C implementations via the shared libraries created
def run_regression():
    global is_3d

    simple_regression_functions = CDLL(SIMPLE_SO_FILE)
    multiple_regression_functions = CDLL(MULTIPLE_SO_FILE)

    datapoints = read_datapoints(DATAPOINTS_FILE)
    
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
        plot_3d(DATAPOINTS_FILE, PLANE_FILE)