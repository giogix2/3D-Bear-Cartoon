#ifndef _ParticleSystem_H_
#define _ParticleSystem_H_

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include <GL/glew.h>

#define	MAX_PARTICLES	1000
extern float move_rightArm;
extern float translate_bear;
extern bool bear_direction_RIGHT;

// ----------------------------------Animation and particle system variables-------------------------------
extern GLuint	loop;
extern float	zoom;
extern float	slowdown;
extern float	xspeed;
extern float	yspeed;
extern float particle_size;

typedef struct {
	bool	active;					
	float	life;					
	float	fade;					
	float	r;						
	float	g;						
	float	b;						
	float	x;						
	float	y;						
	float	z;						
	float	xi;						
	float	yi;						
	float	zi;						
	float	xg;						
	float	yg;						
	float	zg;						
}
particles;

extern particles particle[MAX_PARTICLES];

int DrawParticles(float pos_x, float pos_y, float pos_z);
void InitParticles();
void moveBear();
#endif