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


#include "TreeNode.h"


void TreeNode::setObject(objloader obj) {
		object = obj;
};