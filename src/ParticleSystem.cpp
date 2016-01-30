#include <iostream>
#include <stdlib.h>
#include "ParticleSystem.h"

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

float move_rightArm = 0.0;
float translate_bear = 10.0;
bool bear_direction_RIGHT = true;
GLuint	loop;
float	slowdown=2.0f;
float	xspeed;
float	yspeed;
float particle_size = 0.2f;
particles particle[MAX_PARTICLES];

void InitParticles() {
	for (loop=0;loop<MAX_PARTICLES;loop++)				
	{
		particle[loop].active=true;								
		particle[loop].life=1.0f;								
		particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	
		particle[loop].r=1.0f;	
		particle[loop].g=1.0f;	
		particle[loop].b=1.0f;	
		particle[loop].yi=float((rand()%50)-25.0f)*10.0f;		
		particle[loop].zi=float((rand()%50)-25.0f)*10.0f;		
		particle[loop].xg=0.0f;									
		particle[loop].yg=-4.0f;								
		particle[loop].zg=0.0f;									
	}
}

int DrawParticles(float pos_x, float pos_y, float pos_z) {
	for (loop=0;loop<MAX_PARTICLES;loop++)					
	{
		if (particle[loop].active)							
		{
			float x=particle[loop].x;						
			float y=particle[loop].y;						
			float z=particle[loop].z+pos_z;					

			
			glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);

			glBegin(GL_TRIANGLE_STRIP);						
			    glTexCoord2d(1,1); glVertex3f(x+(particle_size/2),y+(particle_size/2),z); 
				glTexCoord2d(0,1); glVertex3f(x-(particle_size/2),y+(particle_size/2),z); 
				glTexCoord2d(1,0); glVertex3f(x+(particle_size/2),y-(particle_size/2),z);
				glTexCoord2d(0,0); glVertex3f(x-(particle_size/2),y-(particle_size/2),z);
			glEnd();									

			particle[loop].x+=particle[loop].xi/(slowdown*1000);
			particle[loop].y+=particle[loop].yi/(slowdown*1000);
			particle[loop].z+=particle[loop].zi/(slowdown*1000);

			
			if (particle[loop].y <= -4.0f) { 
				particle[loop].yi = -particle[loop].yi;
			}

			particle[loop].xi+=particle[loop].xg;			
			particle[loop].yi+=particle[loop].yg;			
			particle[loop].zi+=particle[loop].zg;			
			particle[loop].life-=particle[loop].fade;		

			if (particle[loop].life<0.0f)					
			{
				particle[loop].life=1.0f;					
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	
				particle[loop].x=pos_x;						
				particle[loop].y=pos_y;						
				particle[loop].z=pos_z;						
				particle[loop].xi=xspeed+float((rand()%60)-32.0f);	
				particle[loop].yi=yspeed+float((rand()%60)-30.0f);	
				particle[loop].zi=float((rand()%60)-30.0f);	
				particle[loop].r=1.0f;			
				particle[loop].g=1.0f;			
				particle[loop].b=1.0f;			
			}
		}
    }
	return true;											
}

void moveBear() {
	if (bear_direction_RIGHT) {
		translate_bear = translate_bear+0.1;
	}
	else {
		translate_bear = translate_bear-0.1;
	}
	if (translate_bear>50.0f) {
		bear_direction_RIGHT = false;
	}
	if (translate_bear<10.0f) {
		bear_direction_RIGHT = true;
	}
}