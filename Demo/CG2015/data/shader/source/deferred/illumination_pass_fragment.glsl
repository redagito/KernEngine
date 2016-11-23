#version 330 core

// G-buffer textures
uniform sampler2D diffuse_glow_texture;
uniform sampler2D depth_texture;

// L-buffer texture
uniform sampler2D light_texture;

// Screen size
uniform float screen_width;
uniform float screen_height;

// Lit fragment
layout(location = 0) out vec3 fragmentColor;

void main(void)
{
	// Get screen position for uv texture lookup
	vec2 normalized_screen_coordinates = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);

	// Retrieve gbuffer data
	vec4 temp = texture(diffuse_glow_texture, normalized_screen_coordinates);

	// Extract diffuse base color
	vec3 diffuseColor = temp.rgb;

	// Extract glow, should be used for blur pass
	vec3 glow = vec3(temp.a);

	// Retrieve light data
	vec3 light = texture(light_texture, normalized_screen_coordinates).rgb;

	// Calculate fragment color based on incomming light or glow
	fragmentColor = diffuseColor * max(light, glow * 4.f);
}