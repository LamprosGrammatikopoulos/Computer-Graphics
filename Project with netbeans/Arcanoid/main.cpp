#include <stdio.h>     // - Just for some ASCII messages
#include <GL/glut.h>   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions


/////////////// Main Program ///////////////////////////
int main(int argc, char* argv[])
{
    // initialize GLUT library state
    glutInit(&argc, argv);

    // Set up the display using the GLUT functions to
    // get rid of the window setup details:
    // - Use true RGB colour mode ( and transparency )
    // - Enable double buffering for faster window update
    // - Allocate a Depth-Buffer in the system memory or
    //   in the video memory if 3D acceleration available
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);


    // Define the main window size and initial position
    // ( upper left corner, boundaries included )
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(50, 50);

    // Create and label the main window
    glutCreateWindow("Arcanoid");

    
    // Configure various properties of the OpenGL rendering context
    Setup();

    // Callbacks for the GL and GLUT events:
    glutDisplayFunc(Render);
    glutReshapeFunc(Resize);
    glutIdleFunc(Idle);

    //Keyboard controller
    glutKeyboardFunc(KeyboardControl);

    //Enter main event handling loop
    glutMainLoop();
    
    return 0;
}
