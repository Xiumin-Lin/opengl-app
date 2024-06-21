#version 120
// un VS doit toujours ecrire dans
// gl_Position qui est un vec4

// attribute fait le lien avec
// glVertexAttrib ou glVertexAttribPointer

// Attributs d'entrée du vertex shader.
attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords; // coords de texture

// varying fait le lien avec la variable
// du meme nom et type dans le fragment shader
// Variables à passer au fragment shader.
varying vec3 v_Normal;
varying vec2 v_TexCoords;

// valeur constante durant le rendu de la primitive
// Matrices de transformation.
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform mat4 u_NormalMatrix;

void main() {
    // Transformez la position du vertex.
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    // Passez les normales et les coordonnées de texture au fragment shader.
    v_Normal = mat3(u_NormalMatrix) * a_Normal;
    v_TexCoords = a_TexCoords;
}