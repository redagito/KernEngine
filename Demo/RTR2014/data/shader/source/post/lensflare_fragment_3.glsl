#version 330 core

uniform sampler2D scene_texture;
uniform sampler2D lens_texture;
uniform float screen_width;
uniform float screen_height;

layout (location = 0) out vec3 color;

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

void main()
{
	// Calculate screen position of the fragment [0.0 - 1.0]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	
	// Retrieve scene texel and lens texel
	vec3 sceneTexel = vec3(texture(scene_texture, screen_coords));
	vec3 lensTexel = vec3(texture2D(lens_texture, screen_coords));

	//lensTexel = brightpass(lensTexel, 1.4) * lensTexel;
	
	// Add bloom to scene
	color = sceneTexel + lensTexel * 0.4f;
}

