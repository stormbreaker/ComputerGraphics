#ifndef TDBK_MANDELBROT
#define TDBK_MANDELBROT

#include "mandelbrot.h"

using namespace std;

int mandelSqTransf(complex<double> z0, GLint maxIter)
{

	//book assigns z0 here but the assignment description says z0 should be 0
	complex<double> z = 0;
	int counter = 0;

	while ((z.real() * z.real() + z.imag() * z.imag() <= 4.0) && (counter < maxIter))
	{
		z = complexSquare(z);
		z += z0;
		counter++;
	}
	return counter;
}

//plots the points out
void mandelbrot (int nx, int ny, int maxIter)
{
	complex<double> z, zIncr;
	color ptColor;

	int iterCount;

	zIncr = complex<double>(complexWidth/GLdouble(nx), complexHeight/GLdouble(ny));

	double realIterator, imaginaryIterator;

	for (realIterator = xComplexMin; realIterator < xComplexMax; realIterator += zIncr.real())
    {
        for (imaginaryIterator = yComplexMin; imaginaryIterator < yComplexMax; imaginaryIterator += zIncr.imag())
    	{
			z = complex<double>(realIterator, imaginaryIterator);

    		iterCount = mandelSqTransf(z, maxIter);
    		if (iterCount >= maxIter)
    		{
    			ptColor.r = ptColor.g = ptColor.b = 0.0;
    		}
    		else if (iterCount > (maxIter / 8))
    		{
    			ptColor.r = 1.0;
    			ptColor.g = .5;
    			ptColor.b = 0;
    		}
    		else if (iterCount > (maxIter / 10))
    		{
    			ptColor.r = 1.0;
    			ptColor.g = ptColor.b = 0;
    		}
    		else if (iterCount > (maxIter/20))
    		{
    			ptColor.b = .5;
    			ptColor.r = ptColor.g = 0;
    		}
    		else if (iterCount > (maxIter/40))
    		{
    			ptColor.r = ptColor.g = 1.0;
    			ptColor.b = 0;
    		}
    		else if (iterCount > (maxIter/100))
    		{
    			ptColor.r = ptColor.b = 0;
    			ptColor.g = .3;
    		}
    		else
    		{
    			ptColor.r = 0;
    			ptColor.g = ptColor.b = 1;
    		}
    		glColor3d(ptColor.r, ptColor.g, ptColor.b);
    		plotPoint(z);
    	}
    }
}

#endif
