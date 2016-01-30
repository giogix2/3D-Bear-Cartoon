#ifndef _TreeNode_H_
#define _TreeNode_H_

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include "Shapes.h"
#include "objloader.h"

class TreeNode {
private:
	GLfloat m[16];
	objloader object;
	TreeNode *sibling;
	TreeNode *child;
public:
	void setObject(objloader obj);
	void setObject(objloader obj);
	
};

#endif