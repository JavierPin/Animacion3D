#version 400

in vec3 position;
in vec3 normal;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 LightPosition;
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float Shininess;

layout (location=0) out vec4 fColor;

void main()
{

	vec3 l = normalize( LightPosition - position );
	vec3 v = normalize( -position );
        vec3 r = reflect( -l, normalize(normal) );
	fColor = vec4(
		(Ia * Ka) +
		(Id * Kd * max( dot(l, normal), 0.0 )) +
		(Is * Ks * pow( max( dot(r, v), 0.0 ), Shininess )), 1.0);
}
