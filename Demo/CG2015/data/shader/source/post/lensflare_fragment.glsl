#version 330 core

uniform sampler2D scene_texture;
uniform float screen_width;
uniform float screen_height;

uniform vec4 uScale = vec4(1.0);
uniform vec4 uBias = vec4(-9.0);

layout (location = 0) out vec4 color;

//Pseudo Lens Flare by john-chapman-graphics
//http://john-chapman-graphics.blogspot.co.at/2013/02/pseudo-lens-flare.html
void main() {

	// Calculate screen position of the fragment [0-1]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	color = max(vec4(0.0), texture(scene_texture, screen_coords) + uBias) * uScale;
}