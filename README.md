# Monte-Carlo-Pi-Simulator
A program that allows the user to run Multiple Monte Carlo simulations to estimate the value of Pi. Each stimulation generates a number of points inside a 1x1 square, then takes the ratio between the number of points that landed inside a quarter circle that sweeps from the bottom side to the left side and the total number of points in the square. 

The formula works as follows:

<code>Number of Points in Quarter Circle: πr<sup>2</sup>/4</code>
<code>Number of Total Points in Square: r<sup>2</sup></code>

Therefore, <code>π = (πr<sup>2</sup>/4)/(r<sup>2</sup>)</code>

# Features
1. Robust error handling via `errno.h` and conditionals. 
2. Test functions to verify functionality of core functions
3. Python sub-program to display the generated points and then quarter circle, allowing the user to visualize the simulations.

# Requirements to Run Program
Be sure to have a `make` tool. In Unix-like systems, this is usually built-in or easily installable, but on Windows, one would need to install a compatible tool, such as nmake from Visual Studio Code, or MinGW.

# How to Run
1. First compile the program using `make clean` and then `make`. Then, run `./main`.
2. You will enter a prompt loop that requests you to enter a number that represents an option you want to select. Enter one of the numbers displayed and press `enter`.

# How to Test
1. Compile the test program using `make clean` and then `make test`. Then, run `./test`.