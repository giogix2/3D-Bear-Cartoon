#ifndef _Water_H_
#define _Water_H_
#include "vec3f.h"

class Water {
	private:
		int w; 
		int l;
		float** hs;
		Vec3f** normals;
		bool computedNormals;
	public:
		Water(int w2, int l2);
		~Water();
		int width();
		int length();
		void setHeight(int x, int z, float y);
		float getHeight(int x, int z);
		void computeNormals();
		Vec3f getNormal(int x, int z);
		void draw();
};



#endif