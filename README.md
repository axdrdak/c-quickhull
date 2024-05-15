# C QUICKHULL
This is a fast, pure, header-only C Quickhull implementation. It is possible to modify this code easily to work with *P2* geometry space thanks to the nataure of the `halfplaneTest` function.

The Quickhull algorithm has an average complexity of *O(N log N)*.

This implementation was created to cure the need for the as minimal as possible, low-level, simple, efficient and fast Quickhull algorithm, without any redundant structures. These features make it suitable for comparative benchmarking of other CH construction algorithms, and this was in fact primary reason of this implementation creation.
 
This implementation returns exclusively critical points of minimal CH, ignoring any points lying on lines of CH. The output points are not referenced to input points, so the input points can be easily freed after use. 

This implementation is Valgrind tested and is memory-safe.

keywords: _quickhull, convexhull computation, half-plane test, distance from line, line equation, line parameters, projective space, log n, recursive, header-only_ 

---

## Basic usage
The main function is the ``double** quickHull(double** points, int n, int& hullSize);`` where `points` is an allocated array of input points, `n` is the number of these input points and `hullSize` is the integer reference that will represent the size of the returned _convex hull_ points. The result of this function is independently allocated array of sub-arrays (points) of size *hullSize* - don't forget to free this memory after use. 

Here is the code snippet. (`loadPointsFromFile()` is a function from "fileops.h" which is not included. It simply returns allocated array of arrays - points)
```c
#include <stdio.h>
#include "dQuickhull.h"
#include "fileops.h"

void printHull(double** hull, int size) {
    printf("Convex Hull: \n");
    for (int i = 0; i < size; i++) {
        printf("%f %f\n", hull[i][0], hull[i][1]);
    }
    printf("\n");
}

int main() {
    int n = 40;
    double** points = loadPointsFromFile("ch1", n);

    int hullSize = 0;
    double** hull = quickHull(points, n, hullSize);
    printHull(hull, hullSize);

    for(int i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);
    free(hull);

    return 0;
}
```
Format of input points is `double* point`, where each element is in the format of `(double x, double y)`.

## EPS value
The default eps value is `0.00001`. In case of incorrect results, please, scale this parameter adequately.

---

_PS: If you happen to be developing in CLion, please be aware that with default build settings you are far from getting the best performance. Look for some custom compiler configurations on the internet, it makes a day and night difference!_ 
