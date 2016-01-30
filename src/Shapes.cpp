#include <iostream>
#include <stdlib.h>
 
#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "Shapes.h"



Triangle::Triangle(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2, GLdouble px3, GLdouble py3, GLdouble pz3){
	x1 = px1;
	x2 = px2;
	x3 = px3;
	y1 = py1;
	y2 = py2;
	y3 = py3;
	z1 = pz1;
	z2 = pz2;
	z3 = pz3;
	isDif = false;
	isAmb = false;
	isSpec = false;
	isShin = false;
	isColored = false;
}
GLdouble Triangle::getX1() {return x1;}
GLdouble Triangle::getX2() {return x2;}
GLdouble Triangle::getX3() {return x3;}
GLdouble Triangle::getY1() {return y1;}
GLdouble Triangle::getY2() {return y2;}
GLdouble Triangle::getY3() {return y3;}
GLdouble Triangle::getZ1() {return z1;}
GLdouble Triangle::getZ2() {return z2;}
GLdouble Triangle::getZ3() {return z3;}
void Triangle::setNormal(GLdouble x, GLdouble y, GLdouble z) {norm = glm::vec3(x, y, z);}
void Triangle::setDiffuse(GLdouble d1, GLdouble d2, GLdouble d3) {diffuse[0] = d1;  diffuse[1] = d2; diffuse[2] = d3; isDif = true;}
void Triangle::setSpecular(GLdouble s1, GLdouble s2, GLdouble s3) {specular[0] = s1; specular[1] = s2; specular[2] = s3; isSpec = true;}
void Triangle::setAmbient(GLdouble a1, GLdouble a2, GLdouble a3) {ambient[0]  = a1; ambient[1]  = a2; ambient[2]  = a3; isAmb = true;}
void Triangle::setShininess(GLdouble ns) {shininess = ns; isShin = true;}
void Triangle::setColor(GLdouble c1, GLdouble c2, GLdouble c3) {color[0]  = c1; color[1]  = c2; color[2]  = c3; isColored = true;}
bool Triangle::isShininess() {return isShin;}
bool Triangle::isAmbient() {return isAmb;}
bool Triangle::isSpecular() {return isSpec;}
bool Triangle::isDiffuse() {return isDif;}

void Triangle::draw() {
	//glPushMatrix();
	if (isDif)
		glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	if (isAmb)
		glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	//if (isSpec)
	//	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	if (isShin)
		glMaterialf(GL_FRONT,GL_SHININESS, shininess);
	if (isColored)
		glColor3f(color[0], color[1], color[2]);
	glBegin(GL_TRIANGLES);
	glNormal3f(norm.x, norm.y, norm.z);	//use them
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x3, y3, z3);
	glEnd();
	//glPopMatrix();
}

Quad::Quad(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2, GLdouble px3, GLdouble py3, GLdouble pz3, GLdouble px4, GLdouble py4, GLdouble pz4) {
	x1 = px1;
	x2 = px2;
	x3 = px3;
	x4 = px4;
	y1 = py1;
	y2 = py2;
	y3 = py3;
	y4 = py4;
	z1 = pz1;
	z2 = pz2;
	z3 = pz3;
	z4 = pz4;
}

void Quad::draw() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x3, y3, z3);
	glVertex3f(x4, y4, z4);
	glEnd();
	glPopMatrix();
}

Line::Line(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2) {
	x1 = px1;
	x2 = px2;
	y1 = py1;
	y2 = py2;
	z1 = pz1;
	z2 = pz2;
}

void Line::draw() {
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
}

