#version 400

uniform sampler2D scene_texture;
uniform sampler2D bloom_texture;
uniform float screen_width;
uniform float screen_height;

layout (location = 0) out vec3 color;

void main()
{
	// Calculate screen position of the fragment [0.0 - 1.0]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	
	// Retrieve scene texel and bloom texel
	vec3 sceneTexel = vec3(texture(scene_texture, screen_coords));
	vec3 bloomTexel = vec3(texture2D(bloom_texture, screen_coords));

	// Add bloom to scene
	color = sceneTexel + bloomTexel * 0.4f;
}
