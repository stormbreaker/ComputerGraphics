#include "mandelbrot.cpp"
#include "common.cpp"
#include "julia.cpp"

using namespace std;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// GLdisplay callback
void displayFcn(void)
{
	GLint nx = 1000, ny = 1000, maxIter = 1000;

	glClear(GL_COLOR_BUFFER_BIT);

	complex<double> c(.353,.288);

	mandelbrot(nx, ny, maxIter);

	// julia(nx, ny, maxIter, c);

	glFlush();
}

// GLwindow reshape callback
void winReshapeFcn (GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newHeight, newHeight);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluOrtho2D(xComplexMin, xComplexMax, yComplexMin, yComplexMax);

	glClear(GL_COLOR_BUFFER_BIT);
}

void keyboard(unsigned char key, int x, int y)
{
    cout << "KEY HIT" << endl;

    // process keypresses
    switch(key)
    {
        // Escape key quits program
        case ESC_KEY:
            exit(0);
            break;
        // Plus key increases Velocity
        case PLUS_KEY:
            ZoomMagnification += 1;

            cout << "PLUS HIT" << endl;

            glutPostRedisplay();
            break;
        // minus key decreses Velocity
        case MINUS_KEY:
            ZoomMagnification -= 1;

            cout << "MINUS HIT" << endl;

            glutPostRedisplay();
            break;

        // anything else redraws window
        default:
            glutPostRedisplay();
            break;
    }
}

void mouse(int button, int state, int x, int y)
{
    double xOffset;
    double yOffset;

    if (state == 0)
    {
        double width = abs(xComplexMin) + abs(xComplexMax);
        double amount = width / winWidth;
        double yAxis = abs(xComplexMin) / amount;

        double height = abs(yComplexMin) + abs(yComplexMax);
        double amount2 = height / winHeight;
        double xAxis = abs(yComplexMin) / amount2;

        xOffset = yAxis - (winWidth / 2);
        yOffset = xAxis - (winHeight / 2);

        mouseX = (x - xOffset - (winWidth / 2)) * amount;
        mouseY = ((winHeight / 2) + yOffset - y) * amount2;

        ZoomMagnification += 1;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("MandelBrot Set");
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

	init();
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();

	return 0;
}
