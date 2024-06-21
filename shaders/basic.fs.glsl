#version 120

uniform vec2 u_Dimensions; // dimensions de la fenetre

// un FS doit toujours ecrire dans 
// gl_FragColor qui est un vec4

// Variables reçues du vertex shader.
varying vec3 v_Normal;
varying vec2 v_TexCoords;

uniform sampler2D u_Texture;
// uniform sampler2D u_Texture1;

uniform vec3 u_LightColor;      // Couleur de la lumiere
uniform vec3 u_LightDirection;  // Direction de la lumiere
uniform vec3 u_AmbientColor;    // Lumiere ambiante
uniform vec3 u_DiffuseMaterial; // Couleur de diffision du material

uniform vec3 u_ViewPosition;      // Position de la caméra dans l'espace
uniform float u_Shininess;      // Brillance du matériau

// Calculer la lumiere diffuse
// N = normale en un point (vertex ou fragment) dans l’espace
// L = vecteur directeur normalisé allant d’un point VERS une lumiere
vec3 diffuse(vec3 N, vec3 L, vec3 lightColor) {
    // loi de lambert = max(N.L, 0) = cos(angle entre N et L)
    float NdotL = max(dot(normalize(N), normalize(L)), 0.0);
    return NdotL * lightColor;
}

/**
* Calcule la lumiere speculaire.
* 
* @param N normale du point
* @param L vecteur directeur normalisé allant d'un point VERS une lumiere
* @param V vecteur directeur normalisé allant d’un point VERS la caméra
* @param lightColor couleur de la lumiere.
* @param shininess brillance du materiau.
* @return
*/
vec3 specular(vec3 N, vec3 L, vec3 V, vec3 lightColor, float shininess) {
    vec3 R = reflect(-L, N);  // L doit être inversé car reflect s'attend à ce que I pointe vers la surface
    float spec = pow(max(dot(R, V), 0.0), shininess);
    return spec * lightColor;
}

void main() {
    // Basic color ---------------------------------------------------------
    // vec4 normalColor = vec4(abs(v_Normal), 1.0);
    // gl_FragColor = vec4(abs(v_Normal), 1.0);

    // Texture -------------------------------------------------------------
    // vec4 normalColor = vec4(abs(v_Normal), 1.0);
    // vec4 texColor = texture2D(u_Texture, v_TexCoords);
    // vec4 texColor1 = texture2D(u_Texture1, v_TexCoords);
    // gl_FragColor = mix(texColor, texColor1, 0.5) * vec4(finalColor, 1.0);

    // Ambiant + Diffuse color -------------------------------------------------------
    // vec3 normalizedNormal = normalize(v_Normal);
    // vec3 normalizedLightDir = normalize(u_LightDirection);
    // vec3 diffuseLight = diffuse(normalizedNormal, normalizedLightDir, u_LightColor);
    // vec3 finalColor = diffuseLight + u_AmbientColor;
    // gl_FragColor = vec4(finalColor, 1.0);

    // Ambiant + Diffuse + Texture -----------------------------------------------------
    // vec4 texColor = texture2D(u_Texture, v_TexCoords);
    // vec3 normalizedNormal = normalize(v_Normal);
    // vec3 normalizedLightDir = normalize(u_LightDirection);
    // vec3 diffuseLight = diffuse(normalizedNormal, normalizedLightDir, u_LightColor);
    // vec3 finalColor = (diffuseLight * u_DiffuseMaterial * texColor.rgb) + u_AmbientColor;
    // gl_FragColor = vec4(finalColor, texColor.a);

    // Specular color ------------------------------------------------------
    // Simule les reflets de la lumiere sur la surface de l'objet
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_LightDirection);
    vec3 V = normalize(u_ViewPosition - gl_FragCoord.xyz); // Calculer le vecteur vue
    vec3 specularColor = specular(N, L, V, u_LightColor, u_Shininess);
    gl_FragColor = vec4(specularColor, 1.0);

    // Gradient color -------------------------------------------------------
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