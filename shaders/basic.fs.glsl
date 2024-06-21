#version 120

uniform vec2 u_Dimensions; // dimensions de la fenêtre

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

// Variables reçues du vertex shader.
varying vec3 v_Normal;
varying vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture1;

uniform vec3 u_LightColor;  // Couleur de la lumière
uniform vec3 u_LightDirection;  // Direction de la lumière
uniform vec3 u_AmbientColor;  // Lumière ambiante

// Calculer la lumière diffuse
// N = normale en un point (vertex ou fragment) dans l’espace
// L = vecteur directeur normalisé allant d’un point VERS une lumière
vec3 diffuse(vec3 N, vec3 L, vec3 lightColor) {
    // loi de lambert = max(N.L, 0) = cos(angle entre N et L)
    float NdotL = max(dot(normalize(N), normalize(L)), 0.0);
    return NdotL * lightColor;
}

void main() {
    // Diffuse color -------------------------------------------------------
    vec3 normalizedNormal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightDirection);
    vec3 diffuseColor = diffuse(normalizedNormal, lightDir, u_LightColor);
    vec3 finalColor = diffuseColor + u_AmbientColor;
    gl_FragColor = vec4(finalColor, 1.0);

    // Basic color ---------------------------------------------------------
    // vec4 normalColor = vec4(abs(v_Normal), 1.0);
    // gl_FragColor = vec4(abs(v_Normal), 1.0);

    // TEXTURE -------------------------------------------------------------
    // vec4 normalColor = vec4(abs(v_Normal), 1.0);
    // vec4 texColor = texture2D(u_Texture, v_TexCoords);
    // vec4 texColor1 = texture2D(u_Texture1, v_TexCoords);
    // gl_FragColor = mix(texColor, texColor1, 0.5) * vec4(finalColor, 1.0);

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