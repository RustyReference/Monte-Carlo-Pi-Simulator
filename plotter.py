import sys
import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Decide which csv file to plot.
# The user enters a command line argument which is the id of the .csv file.
# This id is appended at the end of the name of the .csv file.
if len(sys.argv) > 2 or len(sys.argv) < 2:
    raise ValueError("Usage: python3 plotter.py <id>")

path = "plots/points{id}.csv".format(id = sys.argv[1])

# Check if the .csv file exists
if not os.path.exists(path):
    raise ValueError("File does not exist.");

# Load all the points of the file the user has chosen
df = pd.read_csv(path)

plt.figure(figsize=(3, 3), layout="constrained")
plt.title("All Point Positions")
plt.xlabel("X Position")
plt.ylabel("Y Position")
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.grid(True)
plt.legend()

# Get a radius and a set of points within 0 to pi / 2
r = 1.0
theta = np.linspace(0, np.pi / 2, 100)

# Parametric equations for x and y coords
x = r * np.cos(theta)
y = r * np.sin(theta)

# Draw the unit square
plt.plot([0, 1, 1, 0, 0], [0, 0, 1, 1, 0], "k--", label="Unit Square")

# Draw the quarter circle
plt.plot(x, y, "b-", label="Quarter Circle")

# Get the X- and Y-Coordinates and plot all the points
x = df["x"].to_numpy()
y = df["y"].to_numpy()
plt.scatter(x, y, s=0.1, c="black")

plt.show()