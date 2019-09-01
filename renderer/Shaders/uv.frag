#version 430

layout (location = 0) uniform sampler2D albedo;
layout (location = 1) uniform sampler2D tex2;
layout (location = 2) uniform sampler2D tex3;
uniform float val;

in vec2 vUV;

out vec4 vertColor;

void main() 
{ 
	vertColor = texture(albedo, vUV); 
}