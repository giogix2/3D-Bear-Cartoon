#include <iostream>
#include <stdlib.h> //Needed for "exit" function

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "objloader.h"

#include "Shapes.h"
#include <vector>
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include "ParticleSystem.h"
#include <math.h>
#include "vec3f.h"
#include "Water.h"


using namespace std;

// ---------------------------------------Variables for camera moving----------------------------------
float _angle = 0; // Rotation angle
float translationX = 0.0; // Variable used to translate on the x axis with keyboard's commands
float translationY = 0.0; // Variable used to translate on the y axis with keyboard's commands
float translationZ = 40.0; // Variable used to translate on the z axis with keyboard's commands
float cameraAngle = 45.0; // View angle in the gluPerspective function (in handleResize)

// ----------------------------------------Variables for object picking---------------------------
float mouseX;
float mouseY;
bool selectMode;
const int BUFSIZE = 64;
GLuint buff[64];
const bool SELECT = true;
const bool RENDER = false;
GLint hits;
GLuint detectedObject;
bool waiting_for_another_click;

// -----------------------------------Variables containing .obj objects---------------------------
objloader obj_bear_torso;
objloader obj_bear_head;
objloader obj_bear_head2;
objloader obj_monkey_head;
objloader obj_bear_leftHarm;
objloader obj_bear_rightHarm;
objloader obj_bear_leftHand;
objloader obj_bear_rightHand;
objloader obj_bear_leftLeg;
objloader obj_roof;
objloader obj_chair_lift;
objloader obj_cylinder1;
objloader obj_cylinder2;
objloader obj_pool;

//----------------------------------- Water variables------------------------------
Water* water;
float time = 0;
int waveWidth = 20000;

// Function which initialize the hightmap
Water* loadWater(float height, int w, int z) {
	Water* t = new Water(w, z);
	for(int y = 0; y < z; y++) {
		for(int x = 0; x < w; x++) {
			float h = height;
			t->setHeight(x, y, h);
		}
	}
	t->computeNormals();
	return t;
}

//Funcion which increase the angle of rotation of the camera
void rotation() {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}
	glutPostRedisplay();
}

void moveCamera(float x, float y, float z, float angle) {
	translationX = translationX + x;
	translationY = translationY + y;
	translationZ = translationZ + z;
	cameraAngle = cameraAngle + angle;
	glutPostRedisplay();
}

void waiting_another_click(int a) {
	waiting_for_another_click = false;
}

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {    
	switch (key) {
		case 27: //Escape key
			exit(0); //Exit the program
		case 97: //a key
			rotation();
			break;
		case 101: //e key (move on z axis)
			moveCamera(0.0, 0.0, -1.0, 0.0);
			break;
		case 115: //s key (move on x axis to the left)
			moveCamera(-1.0, 0.0, 0.0, 0.0);
			break;
		case 102: //f key (move on x axis to the right)
			moveCamera(1.0, 0.0, 0.0, 0.0);
			break;
		case 100: //d key (move on z axis)
			moveCamera(0.0, 0.0, 1.0, 0.0);
			break;
		case 114: //r key (move up on z axis)
			moveCamera(0.0, 1.0, 0.0, 0.0);
			break;
		case 103: //f key (move down on z axis)
			moveCamera(0.0, -1.0, 0.0, 0.0);
			break;
		case 113: //q key (increase the view angle in the gluPerspective function)
			moveCamera(0.0, 0.0, 0.0, -1.0);
			break;
		case 122: //z key (decrease the view angle in the gluPerspective function)
			moveCamera(0.0, 0.0, 0.0, 1.0);
			break;
		case 'l': //l key
			move_rightArm++;
			glutPostRedisplay();
			break;
			obj_bear_torso.setChild(&obj_bear_head);
		case 'o':
			// Change the head of the bear
			obj_bear_torso.setChild(&obj_bear_head2);
			obj_bear_head2.setSibling(&obj_bear_leftHarm); // left arm sibling of head
			obj_bear_leftHarm.setSibling(&obj_bear_rightHarm); // right arm sibling of left arm
			obj_bear_rightHarm.setChild(&obj_bear_rightHand); // right hand child of right arm
			obj_bear_leftHarm.setChild(&obj_bear_leftHand); // left hand child of left arm
			break;
		case 'p':
			// Change the head of the bear
			obj_bear_torso.setChild(&obj_monkey_head);
			obj_monkey_head.setSibling(&obj_bear_leftHarm); // left arm sibling of head
			obj_bear_leftHarm.setSibling(&obj_bear_rightHarm); // right arm sibling of left arm
			obj_bear_rightHarm.setChild(&obj_bear_rightHand); // right hand child of right arm
			obj_bear_leftHarm.setChild(&obj_bear_leftHand); // left hand child of left arm
			break;
		case 'i':
			// Change the head of the bear
			obj_bear_torso.setChild(&obj_bear_head);
			obj_bear_head.setSibling(&obj_bear_leftHarm); // left arm sibling of head
			obj_bear_leftHarm.setSibling(&obj_bear_rightHarm); // right arm sibling of left arm
			obj_bear_rightHarm.setChild(&obj_bear_rightHand); // right hand child of right arm
			obj_bear_leftHarm.setChild(&obj_bear_leftHand); // left hand child of left arm
			break;
		
	}
}

//Ray picking using gluUnProject (not yet used in this program)
void selectObject(int x, int y){
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		
}

//Initializes 3D rendering
void initRendering() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size
	
	//Create the window
	glutCreateWindow("Basic Shapes - videotutorialsrock.com");

	// -------------------------Load all the .obj files-----------------------------
	obj_roof.loadShapeToVector("Roof.obj");
	obj_bear_torso.loadShapeToVector("Bear_Torso.obj");
	obj_bear_head.loadShapeToVector("Bear_Head.obj");
	obj_bear_head2.loadShapeToVector("Bear_Head2.obj");
	obj_monkey_head.loadShapeToVector("Monkey_Head2.obj");
	obj_bear_leftHarm.loadShapeToVector("Bear_LeftArm.obj");
	obj_bear_rightHarm.loadShapeToVector("Bear_RightArm.obj");
	obj_bear_leftHand.loadShapeToVector("Bear_LeftHand.obj");
	obj_bear_rightHand.loadShapeToVector("Bear_RightHand.obj");
	obj_chair_lift.loadShapeToVector("chairLift.obj");
	obj_cylinder1.loadShapeToVector("vertical_tube.obj");
	obj_cylinder2.loadShapeToVector("vertical_tube.obj");
	obj_pool.loadShapeToVector("pool.obj");

	// ----------------------Set the transformation for the parts of the bear-------------
	obj_bear_leftHarm.setTranslation(-0.6, 1.1, 0.0);
	obj_bear_rightHarm.setTranslation(0.8, 1.1, 0.0);
	obj_bear_rightHand.setTranslation(2.1, 0.0, 0.0);
	obj_bear_leftHand.setTranslation(-2.1, 0.0, 0.0);

	// ------------------------Compose the bear----------------------------------
	obj_bear_torso.setChild(&obj_bear_head); // head child of torso
	obj_bear_head.setSibling(&obj_bear_leftHarm); // left arm sibling of head
	obj_bear_leftHarm.setSibling(&obj_bear_rightHarm); // right arm sibling of left arm
	obj_bear_rightHarm.setChild(&obj_bear_rightHand); // right hand child of right arm
	obj_bear_leftHarm.setChild(&obj_bear_leftHand); // left hand child of left arm

	selectMode = false;
	detectedObject = 0;
	waiting_for_another_click = false;


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				
	InitParticles();
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   200.0);                //The far z clipping coordinate
}

//Draws the 3D scene
void drawScene() {
	glClearColor (0.0, 0.0, 0.0, 0.0);
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//id's stack initialize
	glInitNames();
	glPushName(~0);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(cameraAngle, 1, 1.0, 200.0); 

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	glTranslatef(-10.0f, 0.0f, 0.0f);
	gluLookAt(translationX, translationY, translationZ, 0.5f, -0.5f, -10.0f, 0, 1, 0);
	//Add ambient light

	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; //Color(0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//Add specular light
	GLfloat lightColor2[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Color (0.5, 0.2, 0.2)
	GLfloat lightPos2[] = {-1.0f, 10.0f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	
	glRotatef(-_angle, 1.0f, 1.0f, 0.0f);

	//Preparing all the object relative of the shapes
	Triangle triangle1 = Triangle(0.5f, -0.5f, -10.0f, 0.5f, -1.5f, -10.0f, -0.5f, -1.0f, -10.0f); //right triangle of the road sign
	Triangle triangle2 = Triangle(1.0f, -0.5f, -10.0f, 1.0f, -1.5f, -10.0f, 2.0f, -1.0f, -10.0f); //left triangle of the road sign
	Quad quad1 = Quad(0.5f, -0.5f, -9.8f, 1.0f, -0.5f, -9.8f, 1.0f, -4.0f, -9.8f, 0.5f, -4.0f, -9.8f); // front face of the cube
	Quad quad2 = Quad(1.0f, -0.5f, -9.8f, 1.0f, -0.5f, -10.2f, 1.0f, -4.0f, -10.2f, 1.0f, -4.0f, -9.8f); // right face of the cube
	Quad quad3 = Quad(0.5f, -0.5f, -9.8f, 0.5f, -0.5f, -10.2f, 0.5f, -4.0f, -10.2f, 0.5f, -4.0f, -9.8f); // left face of the cube
	Quad quad4 = Quad(0.5f, -0.5f, -10.2f, 1.0f, -0.5f, -10.2f,1.0f, -4.0f, -10.2f, 0.5f, -4.0f, -10.2f); // back face of the cube
	Quad quad5 = Quad(0.5f, -0.5f, -9.8f, 1.0f, -0.5f, -9.8f, 1.0f, -0.5f, -10.2f, 0.5f, -0.5f, -10.2f); // top face of the cube
	Quad quad6 = Quad(0.5f, -4.0f, -9.8f, 1.0f, -4.0f, -9.8f, 1.0f, -4.0f, -10.2f, 0.5f, -4.0f, -10.2f); // bottom face of the cube
	Line line1 = Line(7.0f, 2.0f, -10.0f, 7.0f, -4.0f, -10.0f); // trunk of the right tree
	Line line2 = Line(-7.0f, 2.0f, -10.0f, -7.0f, -4.0f, -10.0f); // trunk of the left tree
	Triangle triangle3 = Triangle(5.0f, 2.0f, -10.0f, 9.0f, 2.0f, -10.0f, 7.0f, 6.0f, -10.0f); // crown of the right tree
	Triangle triangle4 = Triangle(-9.0f, 2.0f, -10.0f, -5.0f, 2.0f, -10.0f, -7.0f, 6.0f, -10.0f); // crown of the left tree
	Quad quad7 = Quad(-100.0f, -4.0f, 20.0f, 100.0f, -4.0f, 20.0f, 100.0f, -4.0f, -40.0f, -100.0, -4.0f, -40.0f); //ground
	Quad quad8 = Quad(100.0f, -4.0f, -40.0f, -100.0, -4.0f, -40.0f, -100.0, 100.0f, -40.0f, 100.0, 100.0f, -40.0f); //sky
	Quad quad9 = Quad(30.0f, -4.0f, -38.0f, -30.0, -4.0f, -38.0f, -10.0, 20.0f, -38.0f, 10.0f, 20.0f, -38.0f); //montain 

	//drawParticles();
	//displayParticles();
	DrawParticles(translate_bear, -3.0, 2.0);

	//Draw bear with traverse algorithm
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	traverse(&obj_bear_torso);
	moveBear();
	obj_bear_torso.setTranslation(translate_bear, 0.0, 5.0);
	obj_bear_rightHarm.setRotation(move_rightArm, 0.0, 0.0);
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glFlush();

	//Draw Chair Lift
	glPushMatrix();
	glTranslatef(13.0f, 2.0f, -4.0f);
	obj_chair_lift.draw();
	glPopMatrix();

	//Draw Tube 1
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glTranslatef(22.0f, 0.0f, -8.0f);
	obj_cylinder1.draw();
	glDisable(GL_CULL_FACE);
	glPopMatrix();

	//Draw Tube 2
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glRotatef(-30, 0.0f, 0.0f, 1.0f);
	glTranslatef(27.0f, 14.5f, 0.0f);
	obj_cylinder2.draw();
	glDisable(GL_CULL_FACE);
	glPopMatrix();

	//Draw Roof
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -50.0f);
	obj_roof.draw();
	glDisable(GL_CULL_FACE);
	glPopMatrix();

	//TRIANGLE OF THE ROAD SIGN
	glPushName(1);

	if (detectedObject == 1) 
		glColor3f(0.2f, 0.2f, 0.2f);
	else 
		glColor3f(0.5f, 0.35f, 0.05f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	triangle1.draw();
	glNormal3f(0.0f, 0.0f, 1.0f);
	triangle2.draw();

	glPopName();

	//CUBE OF THE ROAD SIGN
	glPushName(2);

	if (detectedObject == 2) 
		glColor3f(0.7f, 0.7f, 0.7f);
	else 
		glColor3f(0.7f, 0.35f, 0.15f);
	
	// front
	glNormal3f(0.0f, 0.0f, 1.0f);
	quad1.draw();
 
	// right
	glNormal3f(1.0f, 0.0f, 0.0f);
	quad2.draw();

	//// left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	quad3.draw();

	//// back
	glNormal3f(0.0f, 0.0f, -1.0f);
	quad4.draw();

	//// top
	glNormal3f(0.0f, 1.0f, 0.0f);
	quad5.draw();

	//// bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	quad6.draw();
 
	glPopName();

	//TREE 1
	if (detectedObject == 3) 
		glColor3f(0.7f, 0.7f, 0.7f);
	else 
		glColor3f(0.64f, 0.16f, 0.16f);

	glLineWidth(GLfloat(7.0));
	glPushName(3);

	line1.draw();

	if (detectedObject == 3) 
		glColor3f(0.7f, 0.7f, 0.7f);
	else 
		glColor3f(0.3f, 0.8f, 0.1f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	triangle3.draw();

	glPopName();

	//TREE 2
	glLineWidth(GLfloat(7.0));
	glPushName(4);

	if (detectedObject == 4) 
		glColor3f(0.7f, 0.7f, 0.7f);
	else 
		glColor3f(0.64f, 0.16f, 0.16f);

	line2.draw();

	if (detectedObject == 4) 
		glColor3f(0.7f, 0.7f, 0.7f);
	else 
		glColor3f(0.3f, 0.8f, 0.1f);

	glNormal3f(0.0f, 0.0f, -1.0f);
	triangle4.draw();
	glPopName();

	//GROUND
	glPushName(5);
	if (detectedObject == 5) 
		glColor3f(0.4f, 0.4f, 0.4f);
	else 
		glColor3f(1.0f, 1.0f, 1.0f);

	glNormal3f(0.0f, 1.0f, 0.0f);
	quad7.draw();
 
	glPopName();

	//SKY
	glPushName(6);
	if (detectedObject == 6) 
		glColor3f(0.8f, 0.8f, 0.8f);
	else 
		glColor3f(0.1f, 0.1f, 0.8f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	quad8.draw();
 
	glPopName();

	//MONTAIN
	glPushName(7);
	if (detectedObject == 7) 
		glColor3f(0.2f, 0.2f, 0.2f);
	else 
		glColor3f(1.0f, 1.0f, 1.0f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	quad9.draw();
 
	glPopName();	

	//Draw Pool
	glPushMatrix();
	glTranslatef(0.0f, -3.0f, 0.0f);
	glScalef(6.5f, 1.0f, 4.5f);
	obj_pool.draw();
	glPopMatrix();

	// Water
	time+=0.02;
	for(int z = 0; z < water->length(); z++) {
		for(int x = 0; x < water->width(); x++) {
			float y = std::sin(waveWidth*(float)x+time)*std::cos(waveWidth*(float)z+time);
			water->setHeight(x, z, y);
		}
	}
	water->draw();

	glutPostRedisplay();
	glutSwapBuffers(); //Send the 3D scene to the screen
	
}

void listHits (GLint hits, GLuint *names){
	if (hits>0) {
		int i;
		GLuint name, minZ;
		names = names+4;
		minZ = *(names-2);
		name = *names;

		for (i = 1; i < hits; i++) {
			names = names+5;
			if (*(names-2) < minZ) {
				minZ = *(names-2);
				name = *names;
			}
		}

		detectedObject = name;
	}
}

//Function called after a double left click of the mouse, used to pick objects
void picking(void){ 
	glClearColor (0.0, 0.0, 0.0, 0.0); 
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if (selectMode == SELECT) {
		GLint viewport[4]; 
		float ratio; //Get the viewport 
		glGetIntegerv(GL_VIEWPORT,viewport); 
		//Space for selection buffer 
		glSelectBuffer(BUFSIZE,buff); //Change render mode 
		glRenderMode(GL_SELECT); //Switch to projection and save the matrix 
		glMatrixMode(GL_PROJECTION); 
		glPushMatrix(); 
		glLoadIdentity(); /*Establish new clipping volume to be unit cube around mouse cursor point (xPos, yPos) and extending # pixels in the vertical and horizontal direction*/ 
		gluPickMatrix(mouseX,viewport[3]-mouseY, 1, 1, viewport); 
		ratio = (viewport[2]+0.0) / viewport[3]; 
		gluPerspective(45, ratio, 1.0, 1000.0); //Go back to modelview for normal rendering 
		glMatrixMode(GL_MODELVIEW); //Initialize the names stack 
		glInitNames(); 
		glPushName(0);
	}

	drawScene(); 

	if (selectMode == SELECT) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glFlush(); // returning to normal rendering mode
		hits = glRenderMode(GL_RENDER);
		selectMode = RENDER;
		if (hits != 0){
			glutSwapBuffers();
			listHits(hits, buff);
		} 
	}
	else 
		glutSwapBuffers(); 

	drawScene();
} 

//Function which manage the double click of the mouse
void handleMouseInteraction(int button, int state, int x, int y){
	if ((state == GLUT_DOWN)) {
		mouseX = x;
		mouseY = y;
		glutTimerFunc(450, waiting_another_click, 1);
		if (waiting_for_another_click == true) {
			selectMode = SELECT;
			picking();
		}
		waiting_for_another_click = true;

	}
}

int main(int argc, char** argv) {

	//Initialize GLUT	
	glutInit(&argc, argv);
	initRendering(); //Initialize rendering

	water = loadWater(30, 30, 20);
	
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMouseInteraction);
	//glutIdleFunc(idle);
	
	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}









