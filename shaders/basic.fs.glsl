#version 120

uniform vec2 u_Dimensions; // dimensions de la fenêtre

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

// Variables reçues du vertex shader.
varying vec3 v_Normal;
varying vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main() {
    // Basic color ---------------------------------------------------------
    vec4 normalColor = vec4(abs(v_Normal), 1.0);
    // gl_FragColor = vec4(abs(v_Normal), 1.0);

    // TEXTURE -------------------------------------------------------------
    vec4 texColor = texture2D(u_Texture, v_TexCoords) * normalColor;
    gl_FragColor = texColor;

    // GRADIENT COLOR -------------------------------------------------------
    // Calculez une couleur de base à partir des valeurs absolues des normales.
    // vec3 baseColor = abs(v_Normal);

    // Calcul des coordonnées normalisées sur l'écran.
    // vec2 normalizedCoords = gl_FragCoord.xy / u_Dimensions;

    // // Créez un gradient qui s'assombrit en fonction des positions x et y.
    // float gradient = 0.7 + 0.3 * (1.0 - normalizedCoords.x) * (1.0 - normalizedCoords.y);

    // // Appliquez le gradient à la couleur de base.
    // vec3 gradientColor = baseColor * gradient;

    // Définissez la couleur de sortie.
    // gl_FragColor = vec4(gradientColor, 1.0);
    
}