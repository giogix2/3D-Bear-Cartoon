#include <iostream>
#include <stdlib.h>
#include "Water.h"
#include "vec3f.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;

Water::Water(int w2, int l2) {
	w = w2;
	l = l2;
			
	hs = new float*[l];
	for(int i = 0; i < l; i++) {
		hs[i] = new float[w];
	}
			
	normals = new Vec3f*[l];
	for(int i = 0; i < l; i++) {
		normals[i] = new Vec3f[w];
	}
			
	computedNormals = false;
}
		
Water::~Water() {
	for(int i = 0; i < l; i++) {
		delete[] hs[i];
	}
	delete[] hs;
			
	for(int i = 0; i < l; i++) {
		delete[] normals[i];
	}
	delete[] normals;
}
		
int Water::width() {
	return w;
}
		
int Water::length() {
	return l;
}
		
//Sets the height at (x, z) to y
void Water::setHeight(int x, int z, float y) {
	hs[z][x] = y;
	computedNormals = false;
}
		
//Returns the height at (x, z)
float Water::getHeight(int x, int z) {
	return hs[z][x];
}
		
//Computes the normals, if they haven't been computed yet
void Water::computeNormals() {
	if (computedNormals) {
		return;
	}
			
	//Compute the rough version of the normals
	Vec3f** normals2 = new Vec3f*[l];
	for(int i = 0; i < l; i++) {
		normals2[i] = new Vec3f[w];
	}
			
	for(int z = 0; z < l; z++) {
		for(int x = 0; x < w; x++) {
			Vec3f sum(0.0f, 0.0f, 0.0f);
					
			Vec3f out;
			if (z > 0) {
				out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
			}
			Vec3f in;
			if (z < l - 1) {
				in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
			}
			Vec3f left;
			if (x > 0) {
				left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
			}
			Vec3f right;
			if (x < w - 1) {
				right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
			}
					
			if (x > 0 && z > 0) {
				sum += out.cross(left).normalize();
			}
			if (x > 0 && z < l - 1) {
				sum += left.cross(in).normalize();
			}
			if (x < w - 1 && z < l - 1) {
				sum += in.cross(right).normalize();
			}
			if (x < w - 1 && z > 0) {
				sum += right.cross(out).normalize();
			}
					
			normals2[z][x] = sum;
		}
	}
			
	//Smooth out the normals
	const float FALLOUT_RATIO = 0.5f;
	for(int z = 0; z < l; z++) {
		for(int x = 0; x < w; x++) {
			Vec3f sum = normals2[z][x];
					
			if (x > 0) {
				sum += normals2[z][x - 1] * FALLOUT_RATIO;
			}
			if (x < w - 1) {
				sum += normals2[z][x + 1] * FALLOUT_RATIO;
			}
			if (z > 0) {
				sum += normals2[z - 1][x] * FALLOUT_RATIO;
			}
			if (z < l - 1) {
				sum += normals2[z + 1][x] * FALLOUT_RATIO;
			}
					
			if (sum.magnitude() == 0) {
				sum = Vec3f(0.0f, 1.0f, 0.0f);
			}
			normals[z][x] = sum;
		}
	}
			
	for(int i = 0; i < l; i++) {
		delete[] normals2[i];
	}
	delete[] normals2;
			
	computedNormals = true;
}
		
//Returns the normal at (x, z)
Vec3f Water::getNormal(int x, int z) {
	if (!computedNormals) {
		computeNormals();
	}
	return normals[z][x];
}

void Water::draw() {
	float scale = 10.0f / max(width() - 1, length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(width() - 1) / 2, -6.2f, -(float)(length() - 1) / 2);
	
	glColor3f(0.0f, 1.0f, 1.0f);
	float ambient[4] = {0.25, 0.25, 0.25};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	float diffuse[4] = {0.4, 0.4, 0.4};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	float specular[4] = {0.774597, 0.774597, 0.774597};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	float shininess[1] = {0.6*128.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	for(int z = 0; z < length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < width(); x++) {
			Vec3f normal = getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, getHeight(x, z), z);
			normal = getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
}