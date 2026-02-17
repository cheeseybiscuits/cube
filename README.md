# Spinning Cube ⬜

A cube that spins using rotation matrices, a bit of Linear Algebra, projection equations, and terminal management! Uses key concepts of Linear Algebra such as matrix multiplication to get the rotation matrices of each side of the cube. 

# How it's made 📖

If you're at all familiar with linear algebra, you will know what matrix multiplication means. The way I do this is taking the [rotation matrices](https://en.wikipedia.org/wiki/Rotation_matrix) and applying them over three dimensions X Y and Z.

By using the resulting rotation matrices for X Y and Z, we can then calculate a single point that would be visible. This introduces the problem of two points possibly being viewed which is why I implement an ooz which creates a cutoff for closer looking points to appear than farther points (based on projection equations).

Finally, by repeating this process over the whole cube, adjusting the time it takes between frames and the step from frame to frame, and finally doing some minor terminal management to be compatible with most (if not all) OSes, we get a spinning cube!

All source code is in cube.cpp

# How to run 💻
Here's how to run the code:

1. Clone the repository
2. (If docker is installed) run ```docker build -t cube .```
3. run ```docker run -it --rm --init cube```
4. (If docker is not installed) run ```g++ -o cube cube.cpp``` (or your favorite compiler)
5. run ```./cube``` (must be full screened)
