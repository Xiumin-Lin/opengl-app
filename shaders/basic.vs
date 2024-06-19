#version 120

// un VS doit toujours ecrire dans
// gl_Position qui est un vec4

// valeur constante durant le rendu de la primitive
uniform float u_Time;

// attribute fait le lien avec
// glVertexAttrib ou glVertexAttribPointer
attribute vec2 a_Position;
attribute vec3 a_Color;
// varying fait le lien avec la variable
// du meme nom et type dans le fragment shader
varying vec4 v_Color;

void main(void) {
	gl_Position = vec4(a_Position, 0.0, 1.0);
	v_Color = vec4(a_Color, 1.0);
}
