#version 400

uniform sampler2D scene_texture;
uniform float screen_width;
uniform float screen_height;

layout (location = 0) out vec3 color;

// Bright pass filter for bloom

float brightpass(vec3 texel, float threshold)
{
	vec3 avgLuminance = vec3(0.2126f, 0.7152f, 0.0722f);
	float luminance = dot(texel, avgLuminance);
	// Can be done faster?
	if (luminance < threshold)
	{
		luminance = 0.f;
	}
	return luminance;
}

// Writes bloom texture
void main()
{
	// Calculate screen position of the fragment [0-1]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	
	// Get scene texel
	vec3 texel = texture(scene_texture, screen_coords).rgb;

	// Should be uniform
	float threshold = 1.4f;
	
	// Perform brightpass filter, write result to texture
	color = brightpass(texel, threshold) * texel;
}
