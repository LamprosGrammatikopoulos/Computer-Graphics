//|---------------------AUTHORS----------------------------|
//|                                                        |
//|          Name1: Grammatikopoulos Lampros               |
//|          AM: 2022201800038                             |
//|                                                        |
//|          Name2: Kolotouros Konstantinos                |
//|          AM: 2022201800090                             |
//|                                                        |
//|--------------------------------------------------------|

#include <stdio.h>     // - Just for some ASCII messages
#include <GL/glut.h>   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions
#include <math.h>
#include <time.h>


//Screen size
int width = 900;
int height = 600;

//Position of light
float lightPosX = -10.0f, lightPosY = -30.0f, lightPosZ = -30.0f;

//Starting position of the game rectangle
float gameContentsX = 15.0f, gameContentsY = -5.0f;

//Racket
float racketX = 1.0f, racketY = 5.5f, racketZ = -7.85f;
float racketscaleX = 3.5f, racketscaleY = 3.5f, racketscaleZ = 0.3f;

//Game rectangle
float rectX = -2.0f, rectY = 2.5f, rectZ = -20.0f;
float rectscaleX = 6.0f, rectscaleY = 6.0f, rectscaleZ = 12.0f;

//Ball
float ballX = -1.0f, ballY = 2.5f, ballZ = -17.0f;
float VballX = 0.4f, VballY = 0.4f, VballZ = 0.4f;
double sphereRadius = 0.7;

//Bricks
float bricksSizeX = 2.3, bricksSizeY = 2.3, bricksSizeZ = 0.7;
float BrickRotation = 0.0f;


bool SphereBrickCollision = false;              //Only one collision at a time
bool ManualMode = false;                    //Movement from user
bool showMenu = true;                       //Show/Hide menu

//Camera rotation and zoom variables
const float PI = 3.14159f;          //π
float camera_theta = 3.66519142f;   //θ
float camera_phi = 0.0f;            //φ
float camera_r = 15.0f;             //r

//score
int TotalScore=0;

//Bricks
#define NumberOfBricks 48                   //give a number that is multiple of eight
double bricks[NumberOfBricks][4] = {0};     //Double because coordinates can be double
int index=0;


//================================================================================================
//================================================================================================


void DrawBricks(void)
{
    srand(0x98765432); // this is the SEED
    glPushMatrix();
        glTranslatef(1.5f, 1.5f, -9.5f);                        //moving all bricks at a time
        
        for(int k=0; k<NumberOfBricks/8; k=k+2) {
            for(int i=0; i<NumberOfBricks/4; i=i+3) {
                for(int j=0; j<NumberOfBricks/4; j=j+3) {
                    glColor3ub(rand()%255, rand()%255, rand()%255);
                    glPushMatrix();
                        // State 0 : Brick has NOT been hit
                        if(bricks[index][3] == 0) {
                            glTranslatef(1.0f-j, 5.5f-i, -20.0f+k);             //moving one brick at a time
                            glScalef(bricksSizeX,bricksSizeY,bricksSizeZ);
                            bricks[index][0]=1.5f+1.0f-j;
                            bricks[index][1]=1.5f+5.5f-i;                //Calculation of ball position: translate1 + translate2 for all 3 axes
                            bricks[index][2]=-9.5f-20.0f+k;
                            glutSolidCube(1);
                        }
                        // State -1 : Brick has been hit but is in animation
                        else if (bricks[index][3] == -1) {
                            glTranslatef(1.0f-j, bricks[index][1], -20.0f+k);                  //moving one brick at a time
                            glRotatef(BrickRotation,1,0,0);
                            glScalef(bricksSizeX, bricksSizeY, bricksSizeZ);
                            glutSolidCube(1);
                        }
                        // State 1 or anything else : Brick animation has ended -> don't print the brick anymore
                        index++;
                    glPopMatrix();
                }
            }
        }
    glPopMatrix();
    index=0;
}

void DrawSphere(void)
{
    glPushMatrix();
        glColor3f((rectscaleY-ballY)*0.1f, 0, 1.0f);
        glTranslatef(ballX, ballY, ballZ);  
        glutSolidSphere(sphereRadius,100,100);      //radius,slices,stacks
    glPopMatrix();
}

void DrawRacket(void)
{
    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(racketX, racketY, racketZ);
        glScalef(racketscaleX, racketscaleY, racketscaleZ);
        glutSolidCube(1);
    glPopMatrix();
}

void DrawRectangle(void)
{
    glColor4f(0.3f, 0.8f, 0.5f, 0.50);       //Dark green color
    
    glPushMatrix();

        glTranslatef(rectX, rectY, rectZ);
        glScalef(rectscaleX,rectscaleY,rectscaleZ);

        glNormal3f(0, 0, 1);
        
        glDisable(GL_CULL_FACE);                //Disabling Cull_Face to see inside the box
        
        glBegin(GL_TRIANGLES);                   // Start drawing the rectangle from triangles
        
            // Front face  (z = 1.0f)
            //glNormal3f(0.0f, 0.0f, 1.0f);
            //---------------------------
            //glVertex3f(-1.0f, 1.0f, 1.0f);
            //glVertex3f(-1.0f,-1.0f, 1.0f);
            //glVertex3f(1.0f,-1.0f, 1.0f);
            //---------------------------
            //glVertex3f(1.0f, 1.0f, 1.0f);
            //glVertex3f(-1.0f, 1.0f, 1.0f);
            //glVertex3f(1.0f,-1.0f, 1.0f);


            // Back face (z = -1.0f)
            glNormal3f(0.0f, 0.0f, -1.0f);
            //---------------------------
            glVertex3f(1.0f, 1.0f,-1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            //---------------------------
            glVertex3f(1.0f, 1.0f,-1.0f);
            glVertex3f(1.0f,-1.0f,-1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);


            // Top face (y = 1.0f)
            glNormal3f(0.0f, 1.0f, 0.0f);
            //---------------------------
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f,-1.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            //---------------------------
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);


            // Bottom face (y = -1.0f)
            glNormal3f(0.0f, -1.0f, 0.0f);
            //---------------------------
            glVertex3f(1.0f,-1.0f, 1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glVertex3f(1.0f,-1.0f,-1.0f);
            //---------------------------
            glVertex3f(1.0f,-1.0f, 1.0f);
            glVertex3f(-1.0f,-1.0f, 1.0f);
            glVertex3f(-1.0f,-1.0f,-1.0f);


            // Right face (x = 1.0f)
            glNormal3f(1.0f, 0.0f, 0.0f);
            //---------------------------
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(1.0f,-1.0f,-1.0f);
            glVertex3f(1.0f, 1.0f,-1.0f);
            //---------------------------
            glVertex3f(1.0f,-1.0f,-1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(1.0f,-1.0f, 1.0f);


            // Left face (x = -1.0f)
            glNormal3f(-1.0f, 0.0f, 0.0f);
            //---------------------------
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glVertex3f(-1.0f,-1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            //---------------------------
            glVertex3f(-1.0f,-1.0f,-1.0f);
            glVertex3f(-1.0f, 1.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f,-1.0f);
            
        // End drawing the rectangle
        glEnd();
        
    glPopMatrix();
}

void BricksCollisionDetection() {
    for(int i=0; i<NumberOfBricks; i++) {           //For each group of coordinates of every brick
        if (bricks[i][3] == 0) {                        //If the brick is NOT broken
            if ( abs(bricks[i][0]-ballX)<((bricksSizeX/2)+sphereRadius) && abs(bricks[i][1]-ballY)<((bricksSizeY/2)+sphereRadius) && abs(bricks[i][2]-ballZ)<((bricksSizeZ/2)+sphereRadius) ) {     //Check every boundary of a brick
                ballZ = ballZ-VballZ;   //Bounce the ball off the brick
                VballZ = -VballZ;       //Change the ball direction towards the racket (to negative Z)
                
                TotalScore=TotalScore+10;   //Increase the score
                
                bricks[i][3] = -1;          //If the brisk has been hit
                
                SphereBrickCollision = true;    //A collision has been detected
            }
        }
    }
}

void _2DScoreAndToggleMenu() {  
    
    //Print Score
    glMatrixMode(GL_PROJECTION);            //ProjectionView to represent camera's lens
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, width+4000, 0.0, height+4000);      //Change the Width and Height of letters
        glMatrixMode(GL_MODELVIEW);         //ModelView to represent camera
        glPushMatrix();
            glLoadIdentity();
            glColor3f(0.0, 0.0, 0.0);
            glTranslatef(50,height+3800,0);         //Position of line
            char snum[50];
            char *str;
            itoa(TotalScore, snum, 10);
            char snum2[50]="Total Score: ";
            strcat(snum2, snum);
            for (int i = 0; i < strlen(snum2); i++) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, snum2[i]);       //Print each character
            }
            glMatrixMode(GL_MODELVIEW);     //Enable again ModelView
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);            //Enable again ProjectionView
    glPopMatrix();
    
    
    //Print "Toggle menu: m"
    glMatrixMode(GL_PROJECTION);            //ProjectionView to represent camera's lens
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, width+4000, 0.0, height+4000);      //Change the Width and Height of letters
        glMatrixMode(GL_MODELVIEW);         //ModelView to represent camera
        glPushMatrix();
            glLoadIdentity();
            glColor3f(0.0, 0.0, 0.0);
            glTranslatef(3800,height+3800,0);       //Position of line
            char string[] = "Toggle menu: m";
            for (int i = 0; i < strlen(string); i++) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);      //Print each character
            }
            glMatrixMode(GL_MODELVIEW);     //Enable again ModelView
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);            //Enable again ProjectionView
    glPopMatrix();
}

void _2DMenu(char menu[], float y) {
    //Print the Menu
    glMatrixMode(GL_PROJECTION);        //ProjectionView to represent camera's lens
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, width+4000, 0.0, height+4000);      //Change the Width and Height of letters
        glMatrixMode(GL_MODELVIEW);         //ModelView to represent camera
        glPushMatrix();
            glLoadIdentity();
            glColor3f(0.0, 0.0, 0.0);  
            glTranslatef(50,height+3000+y,0);       //Position of line

            for (int i = 0; i < strlen(menu); i++) {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, menu[i]);      //Print each character
            }
            glMatrixMode(GL_MODELVIEW);         //Enable again ModelView
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);        //Enable again ProjectionView
    glPopMatrix();
}

void SphereMovement() {
    
    float dt = 0.01;        //Time period that passes after the ball changes location 
    
    //right, left
    if (ballX+sphereRadius>=(rectX+rectscaleX) || ballX-sphereRadius<=(rectX-rectscaleX)) {             //If the ball(center+radius) hits the Right OR the Left wall 
        VballX=-VballX;
    }
    //up, down
    if (ballY+sphereRadius>=(rectY+rectscaleY) || ballY-sphereRadius<=(rectY-rectscaleY)) {             //If the ball(center+radius) hits the Top OR the Bottom wall 
        VballY=-VballY;
    }
    //close, far
    if (ballZ+sphereRadius>=(rectZ+rectscaleZ) || ballZ-sphereRadius<=(rectZ-rectscaleZ)) {             //If the ball(center+radius) hits the Back wall OR the Front wall/Racket 

        SphereBrickCollision = false;         //if sphere collides with racket then enable again collisions
        
        //Randomness when ball hits the racket in order to break all bricks
//        if (rand()% 2 == 1) {
//            VballX = VballX;
//        }
//        else {
//            VballX = -VballX;
//        }
//
//        if (rand()% 2 == 1) {
//            VballY = VballY;
//        }
//        else {
//            VballY = -VballY;
//        }
                        
        if(ballZ+sphereRadius>(rectZ+rectscaleZ)) {             //If ball hasn't hit the Front wall/racket END the game
            if((ballX+sphereRadius>racketX+racketscaleX || ballX-sphereRadius<racketX-racketscaleX) || (ballY+sphereRadius>racketY+racketscaleY || ballY-sphereRadius<racketY-racketscaleY)) {   //If the ball(center+radius) hits the racket  
                exit(0);
            }
        }
        VballZ=-VballZ;             //Change the ball direction in the Z axis
    }
    ballX = ballX + VballX*dt;      //New position of ball on X axis
    ballY = ballY + VballY*dt;      //New position of ball on Y axis
    ballZ = ballZ + VballZ*dt;      //New position of ball on Z axis
}

void AutomaticRacketMovement() {        //Make Racket follow the Ball 
    if (ManualMode == false) {          //When manual mode is disabled
        if (ballX>=(rectX+rectscaleX-racketscaleX/2)) {     //If the Ball is inside the Racket's zone
            racketX = rectX+rectscaleX-racketscaleX/2;          //Make center of racket be on top of the border in +X
        }
        else if (ballX<=(rectX-rectscaleX+racketscaleX/2)) {    //If the Ball is inside the Racket's zone
            racketX = rectX-rectscaleX+racketscaleX/2;          //Make center of racket be on top of the border in -X
        }
        else {
            racketX = ballX;        //Follow ball on X axis
        }
        if (ballY>=(rectY+rectscaleY-racketscaleX/2)) {     //If the Ball is inside the Racket's zone
            racketY = rectY+rectscaleY-racketscaleY/2;      //Make center of racket be on top of the border in +Y
        }
        else if (ballY<=(rectY-rectscaleY+racketscaleX/2)) {    //If the Ball is inside the Racket's zone
            racketY = rectY-rectscaleY+racketscaleY/2;          //Make center of racket be on top of the border in -Y
        }
        else {
            racketY = ballY;    //Follow ball on Y axis
        }
    }
}

void BricksCollisionAnimation() {
    int HeightBorder = 40;                       //Maximum height that the ball can reach before disappearing
    for (int i=0; i<NumberOfBricks; i++) {
        if (bricks[i][3] == -1) {                   //Animation state
            if (bricks[i][1] < HeightBorder) {          //If Maximum height has NOT been reached
                bricks[i][1] += 0.01f;                //Move the brick upwards
                BrickRotation += 0.5f;              //Rotate the brick
            }
            else {                     //If Maximum height has been reached
                bricks[i][3] = 1;      //Remove the brick from scene    
            }
        }
    }
}

void GameOver() {
    int counter = 0;
    for (int i=0; i<NumberOfBricks; i++) {
        if (bricks[i][3] == 1) {
            counter++;      //Count broken bricks
        }
    }
    if (counter == NumberOfBricks) {    //If all bricks are broken exit the game
        exit(0);
    }
}



//===========================================Main-Functions=======================================
//================================================================================================


void Render()
{
    //CLEARS FRAME BUFFER i.e COLOR BUFFER& DEPTH BUFFER (1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean up the colour of the window
    // and the depth buffer
    glMatrixMode(GL_MODELVIEW);
   
    glLoadIdentity();

    // spherical coordinate camera transform, +Z is "up"
    glTranslatef(gameContentsX, gameContentsY, -camera_r);
    
    //Calculations for camera rotation
    glRotatef( (camera_theta - PI) * (180.0f/PI), 1,0,0 );  //x
    glRotatef(45.0f, 0, 1, 0);                              //y
    glRotatef( -camera_phi * (180.0f/PI), 0,0,1 );          //z
    
    glPushMatrix();     
        glEnable(GL_CULL_FACE);     //Enable Cull_Face for the bricks, racket and ball that are inside the box and have nothing in them
        glFrontFace(GL_CCW);        //Specifies the orientation of front-facing Polygons in Counter-Clock-Wise
        DrawBricks();
        DrawSphere();
        DrawRacket();
        glDisable(GL_CULL_FACE);    //Disable Cull_Face so that all game contents can be seen inside the box
        DrawRectangle();
    glPopMatrix();
    
    if (showMenu == true) {     //Menu
        _2DMenu("Menu:", 0);
        _2DMenu("Exit game: esc", -200);
        _2DMenu("Zoom in: q", -400);
        _2DMenu("Zoom out: e", -550);
        _2DMenu("Rotate left: s", -700);
        _2DMenu("Rotate right: w", -850);
        _2DMenu("Rotate up: a", -1000);
        _2DMenu("Rotate down: d", -1150);
        _2DMenu("Manual mode: g", -1350);
        _2DMenu("Racket upwards: i", -1500);
        _2DMenu("Racket downwards: k", -1650);
        _2DMenu("Racket right: l", -1800);
        _2DMenu("Racket left: j", -1950);
        _2DMenu("Easy: 1", -2150);
        _2DMenu("Medium: 2", -2300);
        _2DMenu("Hard: 3", -2450);
    }
    
    _2DScoreAndToggleMenu();    //Showing score and menu

    glutSwapBuffers(); // All drawing commands applied to the
                       // hidden buffer, so now, bring forward
                       // the hidden buffer and hide the visible one
}

void Idle()
{   
    if(SphereBrickCollision == false) {     //One collision at a time
        BricksCollisionDetection();
    }
    
    SphereMovement();
    
    AutomaticRacketMovement();
    
    BricksCollisionAnimation();

    GameOver();
    
    glutPostRedisplay();    //Mark the normal plane of current window as needing to be redisplayed.
                            //The next iteration through glutMainLoop, the window's display callback will be called to redisplay the window's normal plane.
}

void Resize(int w, int h)
{
    // define the visible area of the window ( in pixels )
    if (h==0) h=1;
    glViewport(0,0,w,h);
    
    // Setup viewing volume
    glMatrixMode(GL_PROJECTION);    //Specifies which matrix stack is the target for subsequent matrix operations.
                                    //GL_PROJECTION: Applies subsequent matrix operations to the projection matrix stack
    glLoadIdentity();
    
    float aspect = (float)w/(float)h;             // aspect ratio
    gluPerspective(60.0, aspect, 1.0, 500.0); 
    
    
//    gluLookAt(-18.0,12.5,-1.0,  //< eye position
//              10.0,-10.0,-30.0,  //< aim position
//              0.0,1.0,0.0); //< up direction
}

void Setup()
{
    //all the faces that are not front-faces are discarded
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    
    //Parameter handling
    glShadeModel(GL_SMOOTH);

    
    //Elegxos tou vathous me ton algorithmo tou ZBuffer (xwris afthn thn entolh pio mprosta emfanizetai to antikeimeno pou sxediasthke teleutaio)
    glEnable(GL_DEPTH_TEST);
    //Poia antikhmena tha pernane apo ton elegxo tou vathous (me LEQUAL osa exoun mikrotero h iso vathos pernane) 
    glDepthFunc(GL_LEQUAL); //renders a fragment if its z value is less or equal of the stored value
    //Elegxei to megisto vathos apokophs
    glClearDepth(1.0);

    
    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //Set up light source
    //Ftiaxnw thn thesh tou fwtismou
    glRotatef(-90.0f, 0, 1, 0);
    glRotatef(60.0f, 1, 0, 0);
    GLfloat light_position[] = {lightPosX, lightPosY, lightPosZ, 0.5}; //(h fwtinh phgh einai sto apeiro kai h teleftaia parametros einai 0 gia na mas dinei thn katefthinsh fwtismou)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat ambientLight[] = {0.3, 0.3, 0.3, 1.0};  //fws tou perivalontos
    GLfloat diffuseLight[] = {0.8, 0.8, 0.8, 1.0};  //diaxuth anaklash

    //parametroi fwtismou
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight); //upeftino gia tis vasikes fwtoskiaseis

    // polygon rendering mode and material properties
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    
    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL); //Leei oti gia to uliko tou antikeimenou xrhsimopoihse to xrwma
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); //Leei oti gia ta mprostina antikeimena xrhsimopoihse kai thn ambient alla kai thn diffuse sunhstwsa

    
    //Energophei fwtismo
    glEnable(GL_LIGHTING);
    //Energophei fwtinh phgh
    glEnable(GL_LIGHT0);

    
    //BLENDING
    glEnable(GL_BLEND);
    //sundyasmos duo xrwmatwn
    // ((1-0.25(alpha)) * (xrwma upovathrou)) + (src_alpha*(xrwma kainourgiou))
    //          source-incoming  destination-stored
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Green background
    glClearColor(0.2f, 0.7f, 0.65f, 1.0f);
}

void KeyboardControl(unsigned char key, int x, int y)
{
    if (key == 27) {    //escape key closes the game
        exit(0);
    }
    
    switch (key) {
        case 'm':
            if (showMenu == false) {
                showMenu = true;
            }
            else {
                showMenu = false;
            }
            break;
        case '1':       //difficulty easy
            if (VballX>0) {
                VballX = 0.3;
            }
            else {
                VballX = -0.3;
            }
            if (VballY>0) {
                VballY = 0.3;
            }
            else {
                VballY = -0.3;
            }
            if (VballZ>0) {
                VballZ = 0.3;
            }
            else {
                VballZ = -0.3;
            }
            break;
        case '2':       //difficulty medium
            if (VballX>0) {
                VballX = 0.4;
            }
            else {
                VballX = -0.4;
            }
            if (VballY>0) {
                VballY = 0.4;
            }
            else {
                VballY = -0.4;
            }
            if (VballZ>0) {
                VballZ = 0.4;
            }
            else {
                VballZ = -0.4;
            }
            break;
        case '3':       //difficulty hard
            if (VballX>0) {
                VballX = 0.5;
            }
            else {
                VballX = -0.5;
            }
            if (VballY>0) {
                VballY = 0.5;
            }
            else {
                VballY = -0.5;
            }
            if (VballZ>0) {
                VballZ = 0.5;
            }
            else {
                VballZ = -0.5;
            }
            break;
            
            
            
        case 'q':       //increase radius
            camera_r+=0.5;
            break;
        case 'e':       //decrease radius
            camera_r-=0.5;
            break;
        case 's':       //increase theta angle
            camera_theta+=PI/20;
            break;
        case 'w':       //decrease theta angle
            camera_theta-=PI/20;
            break;
        case 'a':       //decrease phi angle
            camera_phi-=PI/20;
            break;
        case 'd':       //increase phi angle
            camera_phi+=PI/20;
            break;
            
            
            
        case 'g':
            if (ManualMode == false) {
                ManualMode = true;      //Manual mode ON
            }
            else {
                ManualMode = false;     //Manual mode OFF
            }
            break;
        case 'i':       //move racket upwards
            if (ManualMode == true) {
                if (racketY+racketscaleY<=(rectY+rectscaleY)+1.0) { 
                    racketY=racketY+1;
                }
            }
            break;
        case 'k':       //move racket downwards
            if (ManualMode == true) {
                if (racketY-racketscaleY>=(rectY-rectscaleY)-1.0) {
                    racketY=racketY-1;
                }
            }
            break;
        case 'l':       //move racket right
            if (ManualMode == true) {
                if (racketX+racketscaleX<=(rectX+rectscaleX)+1.0) {
                    racketX=racketX+1;
                }
            }
            break;
        case 'j':       //move racket left
            if (ManualMode == true) {
                if (racketX-racketscaleX>=(rectX-rectscaleX)-1.0) {
                    racketX=racketX-1;
                }
            }
            break;    
        default:
            break;
    }
    
    //θ
    if (camera_theta < 0) camera_theta = 0;         //Αν θ<0 -> θ=0
    //Kleidwnei to θ sto π me to pou patithei to 's'.
    //Vlepoume to sxhma na allazei akariaia giati to arxiko camera_theta ksepernaei to π gia na vlepume to sxhma apo panw
    if (key == 's')
    {
        if (camera_theta > PI) camera_theta = PI;       //Αν θ>π -> θ=π
    }
    //φ
    if (camera_phi < 0) camera_phi = 0;             //Αν φ<0 -> φ=0
    if (camera_phi > 2*PI) camera_phi = 2*PI;       //Αν φ>2π -> φ=2π
    
    glutPostRedisplay();
}