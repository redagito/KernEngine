#include "graphics/renderer/shader/ShaderForwardRenderer.h"

// Vertex shader for the forward renderer
static const std::string vertexShader = R"shadersource(

#version 330 core

// Vertex data streams
layout (location = 0) in vec3 vertexPositionModelSpace;
layout (location = 1) in vec3 vertexNormalModelSpace;
layout (location = 2) in vec2 vertexUV;

// Transformation matrices
uniform mat4 model;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 scale;

// View and projection matrices
uniform mat4 view;
uniform mat4 projection;

// Texture coordinate
out vec2 uv;
// Nornal vector
smooth out vec3 normalVectorCameraSpace;


void main(void)
{
	// Calculate vertex position in camera space
	gl_Position = projection * view * model * vec4(vertexPositionModelSpace, 1.f);
	// Forward texture coordinates
	uv = vertexUV;
	// Calculate transformed normal vector, assumes uniform scale
	normalVectorCameraSpace = (view * translation * rotation * vec4(vertexNormalModelSpace, 0.f)).xyz;
}

)shadersource";


static const std::string fragmentShader = R"shadersource(

#version 330 core

in vec2 uv;
smooth in vec3 normalVectorCameraSpace;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;
uniform sampler2D glow_texture;
uniform sampler2D alpha_texture;

out vec3 fragmentColor;

void main(void)
{
	vec3 diffuseColor = texture(diffuse_texture, uv).rgb;
	vec3 textureNormal = texture(normal_texture, uv).rgb;
	
	float specular = texture(specular_texture, uv).r;
	float glow = texture(glow_texture, uv).r;
	float alpha = texture(alpha_texture, uv).r;
	
	float ambient = 0.3f;
	
	fragmentColor = diffuseColor * ambient + diffuseColor * glow + diffuseColor * specular;
	
}

)shadersource";

const std::string& getForwardRendererVertexShader()
{
	return vertexShader;
}

const std::string& getForwardRendererFragmentShader()
{
	return fragmentShader;
}