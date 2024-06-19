#version 120

uniform float u_Time;

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

varying vec4 v_Color;

void main(void) {
    gl_FragColor = v_Color;
}
