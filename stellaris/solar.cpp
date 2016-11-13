#include "solar.h"

using namespace std;

bool isWireFrame = true;
bool isSmoothShading = false;
bool isLighted = true;
bool isTextured = false;

int main(int argc, char **argv)
{
    // Need to double buffer for animation
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Create and position the graphics window
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 720, 480 );
    glutCreateWindow( "Solar System Demo" );

    // Initialize OpenGL.
    OpenGLInit();

    glutKeyboardFunc(keyboard);

    // Callback for graphics image redrawing
    glutDisplayFunc(Animate);
    glutReshapeFunc(ResizeWindow);

    AllPlanets = CollectPlanetData();

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0;
}

// Initialize OpenGL's rendering modes
void OpenGLInit()
{
    glShadeModel( GL_FLAT );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
}

void ResizeWindow(int w, int h)
{
    float aspectRatio;
    h = ( h == 0 ) ? 1 : h;
    w = ( w == 0 ) ? 1 : w;
    glViewport( 0, 0, w, h );	// View port uses whole window
    aspectRatio = ( float ) w / ( float ) h;

    // Set up the projection view matrix (not very well!)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectRatio, 1.0, 1000.0 );

    // Select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
}

// Animate() handles the animation and the redrawing of the graphics window contents.
void Animate()
{
    // Clear the rendering window
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Clear the current matrix (Modelview)
    glLoadIdentity();

    // Back off eight units to be able to view from the origin.
    glTranslatef ( 0.0, 0.0, -100.0 );

    // Rotate the plane of the elliptic
    // (rotate the model's plane about the x axis by fifteen degrees)
    glRotatef( 15.0, 1.0, 0.0, 0.0 );

    DrawPlanets();

    // Flush the pipeline, and swap the buffers
    glFlush();
    glutSwapBuffers();

    if (singleStep)
    {
        spinMode = GL_FALSE;
    }

    glutPostRedisplay();		// Request a re-draw for animation purposes
}

void keyboard(unsigned char key, int x, int y)
{
    // process keypresses
    switch(key)
    {
        // Escape key quits program
        case ESC_KEY:
            exit(0);
            break;
        // Change the shading model
        case 'S':
        case 's':
            // Toggle if smooth shading is enabled
            isSmoothShading = !isSmoothShading;

            // Change shading model
            if (isSmoothShading == true)
            {
                glShadeModel( GL_SMOOTH );
            }
            else
            {
                glShadeModel( GL_FLAT );
            }

            glutPostRedisplay();
            break;
        // Change if planets are wireframe
        case 'W':
        case 'w':
            // Toggle wireframe display
            isWireFrame = !isWireFrame;

            glutPostRedisplay();
            break;
        // Plus key zooms in
        case '+':
            AnimateIncrement += 5;

            glutPostRedisplay();
            break;
        case '-':
            AnimateIncrement -= 5;

            glutPostRedisplay();
            break;
		case 'L':
		case 'l':
			isLighted = !isLighted;
			if (isLighted == true)
			{
    		    glEnable( GL_LIGHTING );
    			glEnable( GL_LIGHT0 );
			}
            else
            {
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
            }
		case 'T':
		case 't':
			isTextured = !isTextured;
			if (isLighted == true)
			{
				
			}
			else
			{
				
			}
        // anything else redraws window
        default:
            break;
    }
}

void DrawPlanets()
{
    for (Planet &planet : AllPlanets)
    {
        if (planet.getName() == "Sun")
        {
            DrawSun(planet);
        }
        else
        {
            if (spinMode)
            {
                planet.incrememtOrbitValues();
            }

            if (planet.getName() == "Earth")
            {
                glPushMatrix();
                DrawEarth(planet);
            }
            else if (planet.getName() == "Moon")
            {
                DrawMoon(planet);
                glPopMatrix();
            }
            else if (planet.getName() == "Saturn")
            {
                DrawPlanet(planet, true);
            }
            else
            {
                DrawPlanet(planet, false);
            }
        }
    }
}

void DrawPlanet(Planet planet, bool drawRings)
{
    Color planetColor = planet.getPlanetColor();

    glPushMatrix();

    glRotatef(360.0 * planet.getDayOfYear() / planet.getYear(), 0.0, 1.0, 0.0);

    glTranslatef(planet.getDistance(), 0.0, 0.0);

    glRotatef(360.0 * planet.getHourOfDay() / planet.getDay(), 0.0, 1.0, 0.0);
    glEnable(GL_COLOR_MATERIAL);
    float planetEmission[] = {0, 0, 0, 0};
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, planetEmission );
    glColor3ub(planetColor.red, planetColor.green, planetColor.blue);
    glDisable(GL_COLOR_MATERIAL);

    DrawSphere(planet);

    if (drawRings == true)
    {
        GLUquadric* diskObject = gluNewQuadric();

        glRotatef(90.0, 1.0, 0.0, 0.0);

        gluCylinder(diskObject, planet.getRadius() + 10, planet.getRadius() + 2, 0, 20, 20);
    }

    glPopMatrix();
}

void DrawSun(Planet planet)
{
    Color sunColor = planet.getPlanetColor();

    float light_position[] = { 0, 0, 0, 1.0 };
    float light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };       // ambient light
    float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };       // diffuse light
    float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };      // highlights

    glEnable(GL_COLOR_MATERIAL);
    float sunEmisison[] = { 1.0, 1.0, 0.0, 1.0 };
    // glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, sunEmisison );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );
    glColor3ub(sunColor.red, sunColor.green, sunColor.blue);

    // glDisable(GL_COLOR_MATERIAL);
    // enable one light source
    glDisable(GL_COLOR_MATERIAL);

    

    DrawSphere(planet);
}

void DrawEarth(Planet planet)
{
    Color earthColor = planet.getPlanetColor();

    glRotatef( 360.0 * planet.getDayOfYear() / planet.getYear(), 0.0, 1.0, 0.0 );
    glTranslatef( planet.getDistance(), 0.0, 0.0 );
    glPushMatrix();
    // Second, rotate the earth on its axis. Use HourOfDay to determine its rotation.
    glRotatef( 360.0 * planet.getHourOfDay() / planet.getDay(), 0.0, 1.0, 0.0 );
    // Third, draw the earth as a wireframe sphere.
    glEnable(GL_COLOR_MATERIAL);
    glColor3ub( earthColor.red, earthColor.green, earthColor.blue );
    glDisable(GL_COLOR_MATERIAL);

    DrawSphere(planet);

    glPopMatrix();
}

void DrawMoon(Planet planet)
{
    Color moonColor = planet.getPlanetColor();

    glRotatef( 360.0 * 12.0 * planet.getDayOfYear() / planet.getYear(), 0.0, 1.0, 0.0 );
    glTranslatef( planet.getDistance(), 0.0, 0.0 );
    glEnable(GL_COLOR_MATERIAL);
    glColor3ub( moonColor.red, moonColor.green, moonColor.blue );
    glDisable(GL_COLOR_MATERIAL);

    DrawSphere(planet);
}

void DrawSphere(Planet planet)
{
    GLUquadric* ball = gluNewQuadric();
	//cout << isTextured << endl;
	if (isTextured == true)
	{
		glEnable(GL_TEXTURE_2D);
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );     // or GL_CLAMP
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		gluQuadricNormals(ball, GLU_SMOOTH);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, planet.getTexture().width, planet.getTexture().height, 0, GL_RGB, GL_UNSIGNED_BYTE, planet.getTexture().imageDataArray);
		gluQuadricTexture(ball, GL_TRUE);
		//glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
    if (isWireFrame == true)
    {
        glutWireSphere(planet.getRadius(), 15, 15);
    }
    else
    {
	   gluSphere(ball, planet.getRadius(), 15, 15); 
       //glutSolidSphere(planet.getRadius(), 15, 15);
    }
}

vector<Planet> CollectPlanetData()
{
    string planetLine;

    string name;
    double radius;
    double distance;
    double year;
    double day;
    string imageFile;
    Color planetColor;

    vector<Planet> planets;

    ifstream fin("planetdata.info");

    if (fin)
    {
        while (fin >> name >> radius >> distance >> year >> day >> imageFile >> planetColor.red >> planetColor.green >> planetColor.blue)
        {
            Planet planet;
			Texture texture;

            int rows;
            int cols;
			unsigned char* data;

			imageFile = "texture/" + imageFile;

            planet.setName(name);

            planet.setRadius(radius);
            planet.setDistance(distance);
            planet.setYear(year);
            planet.setDay(day);

            planet.setPlanetColor(planetColor);			

            LoadBmpFile(imageFile.c_str(), rows, cols, data);

			texture.path = imageFile;
			texture.height = rows;
			texture.width = cols;
			texture.imageDataArray = data;

			planet.setTexture(texture);

            planets.push_back(planet);
        }
    }
    else
    {
        cout << "Unable to open planetary information.  Exiting." << endl;
        exit(1);
    }

    fin.close();
    return planets;
}
