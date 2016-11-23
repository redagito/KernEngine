#ifndef DEF_GLSL_
#define DEF_GLSL_

#version 330 core

//#define VELOCITY_BUFFER
//#define VELOCITY_BUFFER_UINT
#ifdef VELOCITY_BUFFER_UINT
	const float kVelocityPower = 3.0;
#endif

#endif // DEF_GLSL_