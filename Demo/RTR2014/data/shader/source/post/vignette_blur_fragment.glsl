#version 400

// Noise based distortion shader
// Applies vignette with blur/distortion effect
// Radial calculation and fast noise blur based on work by Francois Tarlier and Martins Upitis
// Vignette by Thibaut Despoulain

uniform sampler2D scene_texture;
uniform float screen_width;
uniform float screen_height;

layout (location = 0) out vec3 color;

// Blur intensity
// TODO Should be blur_strength
float blurIntensity = 2.0;

// Randomization function for distortion blur
// Needed for fast noise based blurring
// Range [0.0 - 1.0]
// TODO Noise function not properly implemented in glsl?
vec2 rand(vec2 co)
{
	float noise1 = fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
	float noise2 = fract(sin(dot(co, vec2(12.9898, 78.233) * 2.0)) * 43758.5453);
	return clamp(vec2(noise1, noise2), 0.0, 1.0);
}

// Blur calculations
// TODO Rename?
vec3 blur(vec2 coords, float blurAmount)
{
	// Calculate fast noise
	vec2 noise = rand(coords);
	float tolerance = 0.2; // From uniform?
	float vignette_size = 0.5; // From uniform?
	
	// Range [0.0 - 0.5], pow2 -> range [0.0 - 0.25]
	// TODO Could be done faster, abs not needed?
	vec2 powers = pow(abs(vec2(coords.x - 0.5, coords.y - 0.5)), vec2(2.0));
	
	float radiusSqrd = vignette_size * vignette_size;
	// Smoothstep max to 0.5f?
	float gradient = smoothstep(radiusSqrd - tolerance, radiusSqrd + tolerance, powers.x + powers.y);

	// Exit on gradient threshold
	// TODO Min gradient from uniform
	if (gradient < 0.07) 
	{
		// Use original texture
		return texture(scene_texture, coords).rgb;
	}
	// Output color
	vec4 color = vec4(0.0);

	// Calculate coordinates for texture access based in blur strength, noise and gradient
	vec2 k1 = noise * blurAmount * 0.004 * gradient;
	vec2 v1 = coords + k1;
	vec2 v2 = coords - k1;

	// Inverse coordinates for texture access
	vec2 k2 = (vec2(1.0, 1.0) - noise) * blurAmount * 0.004 * gradient * 0.5;
	vec2 inv1 = coords + k2;
	vec2 inv2 = coords - k2;
	
	// Calculate final color
	color += texture(scene_texture, v1) * 0.1;
	color += texture(scene_texture, v2) * 0.1;
	color += texture(scene_texture, vec2(v1.x, v2.y)) * 0.1;
	color += texture(scene_texture, vec2(v2.x, v1.y)) * 0.1;

	color += texture(scene_texture, inv1) * 0.15;
	color += texture(scene_texture, inv2) * 0.15;
	color += texture(scene_texture, vec2(inv1.x, inv2.y)) * 0.15;
	color += texture(scene_texture, vec2(inv2.x, inv1.y)) * 0.15;

	return color.rgb;
}

void main(void)
{
	// Calculate screen position of the fragment [0-1]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);
	// Calculate distortion
	color = blur(screen_coords, blurIntensity);
}
