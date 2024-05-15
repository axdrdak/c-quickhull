#include <stdio.h>
#include <math.h>

#define EPS 0.00001

/**
 * Allocates and returns far-left point.
 * @param points
 * @param pointsCount
 * @return Pointer to allocated memory of extreme point.
 */
double* leftCornerPoint(double** points, int pointsCount) {
    double* thePoint = (double*)calloc(2, sizeof(double));
    thePoint[0] = points[0][0];
    thePoint[1] = points[0][1];

    for(int i = 0; i < pointsCount; i++) {
        if(points[i][0] < thePoint[0]) {
            thePoint[0] = points[i][0];
            thePoint[1] = points[i][1];
        } else if(points[i][0] == thePoint[0]) {
            if(points[i][1] < thePoint[1]) {
                thePoint[0] = points[i][0];
                thePoint[1] = points[i][1];
            }
        }
    }

    return thePoint;
}

/**
 * Allocates and returns far-right point.
 * @param points
 * @param pointsCount
 * @return Pointer to allocated memory of extreme point.
 */
double* rightCornerPoint(double** points, int pointsCount) {
    double* thePoint = (double*)calloc(2, sizeof(double));
    thePoint[0] = points[0][0];
    thePoint[1] = points[0][1];

    for(int i = 0; i < pointsCount; i++) {
        if(points[i][0] > thePoint[0]) {
            thePoint[0] = points[i][0];
            thePoint[1] = points[i][1];
        } else if(points[i][0] == thePoint[0]) {
            if(points[i][1] > thePoint[1]) {
                thePoint[0] = points[i][0];
                thePoint[1] = points[i][1];
            }
        }
    }

    return thePoint;
}

void computeLineParams(double* lineParams, const double* pointA, const double* pointB) {
    lineParams[0] = pointA[1]-pointB[1];
    lineParams[1] = -(pointA[0]-pointB[0]);
    lineParams[2] = pointA[0]*pointB[1] - pointA[1]*pointB[0];
}

inline double halfplaneTest(const double* lineParams, const double* point) {
    return (point[0]*lineParams[0] + point[1]*lineParams[1] + 1*lineParams[2]);
}
int getLSet(int* pointSetIndexes, double* lineParams, const int* pointSet, int pointSetSize, double** allPoints) {
    int setSize = 0;

    for(int i = 0 ; i < pointSetSize; i++) {
        if(halfplaneTest(lineParams, allPoints[pointSet[i]]) > EPS) {
            pointSetIndexes[setSize++] = pointSet[i];
        }
    }

    return setSize;
}
int getLSetFromAllPoints(int* pointSetIndexes, double* lineParams, int pointSetSize, double** allPoints) {
    int setSize = 0;

    for(int i = 0 ; i < pointSetSize; i++) {
        if(halfplaneTest(lineParams, allPoints[i]) > EPS) {
            pointSetIndexes[setSize++] = i;
        }
    }

    return setSize;
}

int getRSetFromAllPoints(int* pointSetIndexes, double* lineParams, int pointSetSize, double** allPoints) {
    int setSize = 0;

    for(int i = 0 ; i < pointSetSize; i++) {
        if(halfplaneTest(lineParams, allPoints[i]) < 0) {
            pointSetIndexes[setSize++] = i;
        }
    }

    return setSize;
}

/**
 * Returns distance (oriented!) of the point from the given line.
 * @param lineParams
 * @param point
 * @return
 */
double distFromLine(double* lineParams, double* point) {
    return lineParams[0]*point[0] + lineParams[1]*point[1] + lineParams[2];
}

/**
 *
 * @param lineParams
 * @param pointsSet
 * @param allPoints
 * @param pointSize
 * @return index of the furthest point from the line from given PointSet.
 */
int furthestPointFromLine(double* lineParams, int* pointsSet, double** allPoints, int pointSize) {
    int resultPointIndex = pointsSet[0];
    double maxDistance = distFromLine(lineParams, allPoints[resultPointIndex]);
    double d;

    for(int i = 1; i < pointSize; i++) {
        d = distFromLine(lineParams, allPoints[pointsSet[i]]);
        if(d > maxDistance) {
            maxDistance = d;
            resultPointIndex = pointsSet[i];
        }
    }

    return resultPointIndex;
}

void quickHullRec(double** allPoints, int* pointSet, int pointSetSize, double** hullPoints, int& hullSize, double* lineParams, double* pointA, double* pointB) {
    if(pointSetSize < 1) {
        return;
    }

    double* pointD = (double*)calloc(2, sizeof(double));
    pointD[0] = allPoints[furthestPointFromLine(lineParams, pointSet, allPoints, pointSetSize)][0];
    pointD[1] = allPoints[furthestPointFromLine(lineParams, pointSet, allPoints, pointSetSize)][1];

    double* lLineParams = (double*)calloc(3, sizeof(double));
    computeLineParams(lLineParams, pointA, pointD);
    double* rLineParams = (double*)calloc(3, sizeof(double));
    computeLineParams(rLineParams, pointD, pointB);

    int* lSetTemp = (int*)calloc(pointSetSize, sizeof(int*));
    int* rSetTemp = (int*)calloc(pointSetSize, sizeof(int*));
    int lSetSize = getLSet(lSetTemp, lLineParams, pointSet, pointSetSize, allPoints);
    int rSetSize = getLSet(rSetTemp, rLineParams, pointSet, pointSetSize, allPoints);

    quickHullRec(allPoints, lSetTemp, lSetSize, hullPoints, hullSize, lLineParams, pointA, pointD);
    hullPoints[hullSize++] = pointD;
    quickHullRec(allPoints, rSetTemp, rSetSize, hullPoints, hullSize, rLineParams, pointD, pointB);

    free(lSetTemp);
    free(rSetTemp);
    free(lLineParams);
    free(rLineParams);
}

double** quickHull(double** points, int n, int& hullSize) {
    if (n <= 1)
        return NULL;

    double** hull = (double**)calloc(n, sizeof(double*));
    hullSize = 0;

    double* pointA = leftCornerPoint(points, n);
    hull[hullSize++] = pointA;
    double* pointB = rightCornerPoint(points, n);

    double* lineParams = (double*)calloc(3, sizeof(double));
    computeLineParams(lineParams, pointA, pointB);

    int* lSet = (int*)calloc(n, sizeof(int*));
    int* rSet = (int*)calloc(n, sizeof(int*));
    int lSetSize = getLSetFromAllPoints(lSet, lineParams, n, points);
    int rSetSize = getRSetFromAllPoints(rSet, lineParams, n, points);

    quickHullRec(points, lSet, lSetSize, hull, hullSize, lineParams, pointA, pointB);
    hull[hullSize++] = pointB;
    computeLineParams(lineParams, pointB, pointA);
    quickHullRec(points, rSet, rSetSize, hull, hullSize, lineParams, pointB, pointA);

    free(lSet);
    free(rSet);
    free(lineParams);

    hull = (double**)realloc(hull, (hullSize)*(sizeof(double*)));
    return hull;
}
