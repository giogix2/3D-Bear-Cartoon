#ifndef _Shapes_H_
#define _Shapes_H_

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

class Triangle {
	public:
	Triangle();
	Triangle(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2, GLdouble px3, GLdouble py3, GLdouble pz3);
	GLdouble getX1();
	GLdouble getX2();
	GLdouble getX3();
	GLdouble getY1();
	GLdouble getY2();
	GLdouble getY3();
	GLdouble getZ1();
	GLdouble getZ2();
	GLdouble getZ3();
	void setNormal(GLdouble x, GLdouble y, GLdouble z);
	void setDiffuse(GLdouble d1, GLdouble d2, GLdouble d3);
	void setAmbient(GLdouble a1, GLdouble a2, GLdouble a3);
	void setSpecular(GLdouble s1, GLdouble s2, GLdouble s3);
	void setShininess(GLdouble ns);
	void setColor(GLdouble c1, GLdouble c2, GLdouble c3);
	bool isShininess();
	bool isAmbient();
	bool isSpecular();
	bool isDiffuse();
	void draw();
	private:
		GLdouble x1;
		GLdouble x2;
		GLdouble x3;
		GLdouble y1;
		GLdouble y2;
		GLdouble y3;
		GLdouble z1;
		GLdouble z2;
		GLdouble z3;
		glm::vec3 norm;
		//glm::vec3 diffuse;
		//glm::vec3 specular;
		//glm::vec3 ambient;

		float diffuse[4];
		float ambient[4];
		float specular[4];
		float color[3];
		float shininess; //ns
		bool isDif;
		bool isAmb;
		bool isSpec;
		bool isShin;
		bool isColored;

		//float amb[3],dif[3],spec[3],alpha,ns,ni;

};
class Quad{
	public:
		Quad();
		Quad(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2, GLdouble px3, GLdouble py3, GLdouble pz3, GLdouble px4, GLdouble py4, GLdouble pz4);
		void draw();
	private:
		GLdouble x1;
		GLdouble x2;
		GLdouble x3;
		GLdouble y1;
		GLdouble y2;
		GLdouble y3;
		GLdouble z1;
		GLdouble z2;
		GLdouble z3;
		GLdouble x4;
		GLdouble y4;
		GLdouble z4;
};

class Line{
	public:
		Line();
		Line(GLdouble px1, GLdouble py1, GLdouble pz1, GLdouble px2, GLdouble py2, GLdouble pz2);
		void draw();
	private:
		GLdouble x1;
		GLdouble x2;
		GLdouble y1;
		GLdouble y2;
		GLdouble z1;
		GLdouble z2;
};

#endif