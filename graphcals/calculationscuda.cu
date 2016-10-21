#ifndef TDBK_MANDELBROT
#define TDBK_MANDELBROT

#include "calculations.h"
#include <stdio.h>

using namespace std;

/*
	Name: complexSquare
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function squares the complex number. This function is tagged
    with __device__ to put the function on the card to allow the cuda card to
    easily call it.
*/
__device__ Complex complexSquare(Complex z)
{
	Complex zSquare;

	zSquare.x = (z.x * z.x) - (z.y * z.y);
	zSquare.y = 2 * z.x * z.y;

	return zSquare;
}

/*
	Name: mandelSqTransform
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function performs the calculations finding how fast the
    point diverges for the mandelbrot set. This function has the __global__ tag
    in order for the cuda card to be able to call this function.
*/
__global__ void mandelSqTransf(int maxIter, WindowInfo windowInfo,  Complex *points, int *iterations)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;

    if (index < windowInfo.arrayLength)
    {
	    int counter = 0;
        Complex z0;
	    Complex z;

	    z.x = 0;
	    z.y = 0;

        z0 = points[index];

	    while ((z.x * z.x + z.y * z.y <= 4.0) && (counter < maxIter))
	    {
		    z = complexSquare(z);
		    z.x += z0.x;
		    z.y += z0.y;
		    counter++;
	    }

	    iterations[index] = counter;
    }
}

/*
	Name: juliaSqTransform
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function uses the point from the mandelbrot set to help
    calculate the julia set and uses that to see how fast the point diverges.
    This function has the __global__ tag in order for the cuda card to be able
    to call this function.
*/
__global__ void juliaSqTransform(int maxIter, WindowInfo windowInfo, Complex c, Complex *points, int *iterations)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;

    if (index < windowInfo.arrayLength)
    {
        int counter = 0;
	    Complex z;

	    z = points[index];

	    while ((z.x * z.x + z.y * z.y <= 4.0) && (counter < maxIter))
	    {
		    z = complexSquare(z);
		    z.x += c.x;
		    z.y += c.y;
		    counter++;
	    }

	    iterations[index] = counter;
    }
}

/*
	Name: calculateSetParallel
	Author: Benjamin Kaiser and Taylor Doell
	Description: This is the main function that handles both the mandelbrot and
    the julia set in parallel. A bool flag tells the function which set to
    calculate. There is an optional parameter that can be used if the julia set
    is the set to calculate. Once the number (iterCount) is found, it does some
    calculations to divide the colors that were provided by the color set. It
    then plots the point.
*/
void calculateSetParallel(int nx, int ny, int maxIter, bool isMandelBrot, Complex c)
{
	Complex z, zIncr;
    int counter = 0;
    int arraySize = nx * ny;
    double realIterator = 0.0;
    double imaginaryIterator = 0.0;

    WindowInfo winInfo;
    winInfo.arrayLength = arraySize;
    winInfo.xComplexMin = xComplexMin;
    winInfo.xComplexMax = xComplexMax;
    winInfo.yComplexMin = yComplexMin;
    winInfo.yComplexMax = yComplexMax;

    int size = arraySize * sizeof(Complex);
    Complex *points = (Complex *)malloc(size);
    Complex *dev_points;
    cudaMalloc((void **) &dev_points, size);

    int it_size = arraySize * sizeof(int);
    int *iterations = (int *)malloc(it_size);
    int *dev_iterations;
    cudaMalloc((void **) &dev_iterations, it_size);

    int nThreads = 1024;
    int nBlocks = (arraySize + nThreads - 1) / nThreads;

    zIncr.x = complexWidth / double(nx);
    zIncr.y = complexHeight / double(ny);

    for (realIterator = xComplexMin; realIterator < xComplexMax - zIncr.x; realIterator += zIncr.x)
    {
        for (imaginaryIterator = yComplexMin; imaginaryIterator < yComplexMax - zIncr.y; imaginaryIterator += zIncr.y)
    	{
    	    z.x = realIterator;
    	    z.y = imaginaryIterator;

    	    points[counter] = z;

    	    counter++;
    	}
    }
    cudaMemcpy(dev_points, points, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_iterations, iterations, it_size, cudaMemcpyHostToDevice);

    if (isMandelBrot == true)
    {
        mandelSqTransf<<< nBlocks, nThreads >>>(maxIter, winInfo, dev_points, dev_iterations);
    }
    else
    {
        juliaSqTransform<<< nBlocks, nThreads >>>(maxIter, winInfo, c, dev_points, dev_iterations);
    }

    cudaMemcpy(points, dev_points, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(iterations, dev_iterations, it_size, cudaMemcpyDeviceToHost);

    vector<Color> colorSet = GetCurrentColorSet();

    glBegin(GL_POINTS);
        for (int i = 0; i < arraySize; i++)
        {
            Complex point = points[i];
            int iterCount = iterations[i];

            SetColorAndPlot(maxIter, iterCount, point, colorSet);
        }
    glEnd();

    cudaFree(dev_points);
    cudaFree(dev_iterations);
    free(points);
    free(iterations);
}

#endif
