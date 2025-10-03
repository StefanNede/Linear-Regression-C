from ctypes import *
from utils import read_datapoints, read_plane
from plot3d import plot_3d
import os

# Globals + Constants
# NOTE: if running these files whilst in the root folder then change them 
DATAPOINTS_FILE = "../data/data.txt"
PLANE_FILE = "../data/plane.txt"
CUR_DIR = os.getcwd()
SIMPLE_SO_FILE = f"{CUR_DIR}/../c-backend/simple_export.so"
MULTIPLE_SO_FILE = f"{CUR_DIR}/../c-backend/multi_export.so"

# Use the efficient C implementations via the shared libraries created
def run_regression():
    is_3d = False # flag to indicate whether regression is in 3D

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

    return is_3d

if __name__ == "__main__":
    is_3d = run_regression()
    if is_3d:
        plot_3d(DATAPOINTS_FILE, PLANE_FILE)