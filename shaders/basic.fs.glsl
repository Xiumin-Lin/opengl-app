#version 120

uniform float u_Time;

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

// Variables reçues du vertex shader.
varying vec3 v_Normal;
varying vec2 v_TexCoords;

void main() {
    // Créez une couleur simple basée sur la valeur absolue des normales.
    vec3 color = abs(v_Normal);

    // Utilisez les normales comme couleur de sortie pour visualiser les orientations des surfaces.
    gl_FragColor = vec4(color, 1.0);
}