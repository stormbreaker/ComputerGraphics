#include "calculations.h"
#include "common.h"

/*
	CSC 433 - Computer Graphics
	Dr. John Weiss
	Programming Assignment #2 - Mandelbrot
	Benjamin Kaiser and Taylor Doell
	Due 10-21-16

	Description:

*/

using namespace std;

/*
	Name: displayFcn
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function is the main display function for GLUT. It defines
    nx, ny and maxIter to help in calculations in each set. After the type of
    set is chosen, parallel or serial is chosen for the way the set is
    calculated.
*/
void displayFcn(void)
{
	GLint nx = 1024, ny = 1024, maxIter = 1024;

	glClear(GL_COLOR_BUFFER_BIT);

    if (isMandelbrotSet == true)
    {
        if (isParallel == true)
        {
            calculateSetParallel(nx, ny, maxIter, true);
        }
        else
        {
            calculateSetSerial(nx, ny, maxIter, true);
        }
    }
    else
    {
        if (isParallel == true)
        {
            calculateSetParallel(nx, ny, maxIter, false, mouseCoords);
        }
        else
        {
            calculateSetSerial(nx, ny, maxIter, false, mouseCoords);
        }
    }

	glFlush();
}

/*
	Name: animation
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function animates the colors by choosing the next color
    set and then displaying and calling this function again. If the isAnimating
    becomes false, the function stops and stops the color animation.
*/
void animation(int value)
{
    if (isAnimating == true)
    {
        NextColorSet();
	    glutPostRedisplay();
	    glutTimerFunc(250, animation, 1);
	}
}

/*
	Name: winReshapeFcn
	Author: Benjamin Kaiser and Taylor Doell
	Description: This is the reshape function for GLUT. It handles the change in
    size of the window. Nothing special is taken into account when the size changes
    so the window just shows a square for the view.
*/
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newHeight, newHeight);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

/*
	Name: specialInput
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function handles the special keys for the keyboard. It is
    used for catching the arrow keys for panning the set.
*/
void specialInput(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			pan(0, -PIXEL_PAN);
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			pan(0, PIXEL_PAN);
		    glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			pan(PIXEL_PAN, 0);
            glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			pan(-PIXEL_PAN, 0);
            glutPostRedisplay();
			break;
		default:
			break;
	}
}

/*
	Name: keyboard
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function handles the regular keys from the keyboard. The
    + and - key zoom in and out respectively. A animates the color, C changes the
    color set used for displaying the sets, R chooses and random color set, J
    toggles between the mandlebrot and julia set and V is used for toggling
    between serial and parallel calculations.
*/
void keyboard(unsigned char key, int x, int y)
{
    // process keypresses
    switch(key)
    {
        // Escape key quits program
        case ESC_KEY:
            exit(0);
            break;
        // Plus key zooms in
        case PLUS_KEY:
            mouseCoords = getViewCoordinates(x, y);
            zoom(ZOOM_FACTOR);

            IsZooming = true;

            glutPostRedisplay();
            break;
        // minus key zooms out
        case MINUS_KEY:
            mouseCoords = getViewCoordinates(x, y);
            zoom(-ZOOM_FACTOR);

            IsZooming = true;

            glutPostRedisplay();
            break;
        case LOWERCASE_A_KEY:
        case A_KEY:
            isAnimating = !isAnimating;
            IsZooming = false;

            if (isAnimating == true)
            {
                glutTimerFunc(100, animation, 1);
            }
            break;
        case LOWERCASE_C_KEY:
        case C_KEY:
            UseRandomColorSet = false;
            IsZooming = false;

            NextColorSet();
            glutPostRedisplay();
            break;
        case LOWERCASE_R_KEY:
        case R_KEY:
            UseRandomColorSet = true;
            IsZooming = false;

            glutPostRedisplay();
            break;
        case LOWERCASE_J_KEY:
        case J_KEY:
            isMandelbrotSet = !isMandelbrotSet;

            mouseCoords = getViewCoordinates(x, y);

            glutPostRedisplay();
            break;
        case LOWERCASE_V_KEY:
        case V_KEY:
            isParallel = !isParallel;

            glutPostRedisplay();
            break;
        // anything else redraws window
        default:
            break;
    }
}

/*
	Name: mouse
	Author: Benjamin Kaiser and Taylor Doell
	Description: Handling the scroll wheel is done by checking the buttons 3 and
    4. This functions handles the zooming for the set by using the scroll wheel.
    This also gets the first coordinate for the panning of the mouse.
*/
void mouse(int button, int state, int x, int y)
{
    if (state == 0)
	{
        mouseCoords = getViewCoordinates(x, y);

        if (button == GLUT_LEFT_BUTTON)
        {
            
        }
		else if (button == 3)
		{
			zoom(ZOOM_FACTOR);

			IsZooming = true;

			glutPostRedisplay();
		}
		else if (button == 4)
		{
			zoom(-ZOOM_FACTOR);

			IsZooming = true;

			glutPostRedisplay();
		}
	}
}

/*
	Name: currentMousePosition
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function gets the second coordinate for the mouse panning.
    That coordinate is then taken and used for calculating the distance to pan.
*/
void currentMousePosition(int x, int y)
{
    if (isMouseClicked == true)
    {

    }
}

/*
	Name: main
	Author: Benjamin Kaiser and Taylor Doell
	Description: This function is just the main function that the C++ language
	requires to compile and run.  It calls glutMainLoop which is really our main
	function/loop.
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("MandelBrot Set");
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(currentMousePosition);
	glutSpecialFunc(specialInput);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

    GenerateColorSets();

	glutMainLoop();

	return 0;
}
