#version 330 core

uniform sampler2D scene_texture;
uniform float screen_width;
uniform float screen_height;

uniform float uDispersal = 0.80;
uniform int uGhosts = 4;
uniform float uHaloWidth = 2.0;
uniform float uDistortion = 10.0;

layout (location = 0) out vec4 color;

vec4 textureDistorted(
	in sampler2D tex, 
	in vec2 texcoord, 
	in vec2 direction,
	in vec3 distortion 
) {
	return vec4(
		texture(tex, texcoord + direction * distortion.r).r,
		texture(tex, texcoord + direction * distortion.g).g,
		texture(tex, texcoord + direction * distortion.b).b,
		1.0
	);
}

//Pseudo Lens Flare by john-chapman-graphics
//http://john-chapman-graphics.blogspot.co.at/2013/02/pseudo-lens-flare.html
void main() {

	// Calculate screen position of the fragment [0-1]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	vec2 inv_coords = -screen_coords + vec2(1.0);
	vec2 texelSize = 1.0 / vec2(textureSize(scene_texture, 0));

	// ghost vector to image centre:
    vec2 ghostVec = (vec2(0.5) - inv_coords) * uDispersal;
	vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

	vec4 result = vec4(0.0);
	for (int i = 0; i < uGhosts; ++i) { 
		vec2 offset = fract(inv_coords + ghostVec * float(i));
		float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = pow(1.0 - weight, 2.0);

		result += textureDistorted(
			scene_texture,
			offset,
			normalize(ghostVec),
			distortion
			) * weight;
    }

	vec2 haloVec = normalize(ghostVec) * uHaloWidth;
	float weight = length(vec2(0.5) - fract(inv_coords + haloVec)) / length(vec2(0.5));
	weight = pow(1.0 - weight, 2.0);
	result += textureDistorted(
		scene_texture,
		fract(inv_coords + haloVec),
		normalize(ghostVec),
		distortion
	) * weight;

	color = result;
}