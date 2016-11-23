#version 330 core

uniform sampler2D scene_texture;
uniform float screen_width;
uniform float screen_height;

layout (location = 0) out vec4 color;

float toonify(in float intensity) {
    if (intensity > 0.8)
        return 1.0;
    else if (intensity > 0.5)
        return 0.8;
    else if (intensity > 0.25)
        return 0.3;
    else
        return 0.1;
}
 
void main(){

	// Calculate screen position of the fragment [0-1]
	vec2 screen_coords = vec2(gl_FragCoord.x / screen_width, gl_FragCoord.y / screen_height);

    vec4 help_color = texture2D(scene_texture, screen_coords);
    float factor = toonify(max(help_color.r, max(help_color.g, help_color.b)));
    color = vec4(factor*help_color.rgb, help_color.a);
}