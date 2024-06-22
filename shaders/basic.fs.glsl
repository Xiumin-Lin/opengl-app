#version 120
// un FS doit toujours ecrire dans gl_FragColor qui est un vec4

// Variables reçues du vertex shader ============================
varying vec3 v_Normal;
varying vec2 v_TexCoords;

// Variables uniformes ==========================================
uniform vec3 u_ViewPosition; // Position de la caméra
uniform sampler2D u_Texture;
uniform sampler2D u_TextureSpecular;

struct Light {
    vec3 direction;     // I
    vec3 ambientColor;  // ia pour la lumière ambiante
    vec3 diffuseColor;  // id pour la lumière diffuse
    vec3 specularColor; // is pour la lumière spéculaire
};

struct Material {
    vec3 ambientColor;   // ka pour la couleur du mat ambiante
    vec3 diffuseColor;   // kd pour la couleur du mat diffuse
    vec3 specularColor;  // ks pour la couleur du mat spéculaire
    float shininess;     // Facteur de brillance

    int hasTexture;
    int hasTextureSpecular;
};

uniform Light u_Light;
uniform Material u_Material;

uniform vec2 u_Dimensions; // dimensions de la fenetre

// Fonctions ===================================================
vec3 ambient() {
    return u_Material.ambientColor * u_Light.ambientColor;
}

/**
* Calculer la lumiere diffuse
* N = normale en un point (vertex ou fragment) dans l’espace
* L = vecteur directeur normalisé allant d’un point VERS une lumiere
*/
vec3 diffuse(vec3 N, vec3 L) {
    // loi de lambert = max(N.L, 0) = cos(angle entre N et L)
    float NdotL = max(dot(N, L), 0.0);
    return NdotL * u_Material.diffuseColor * u_Light.diffuseColor;
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
vec3 specular(vec3 N, vec3 L, vec3 V) {
    // Phong model
    // vec3 R = reflect(-L, N); // L doit être inversé car reflect s'attend à ce que N pointe vers la surface
    // float spec = pow(max(dot(R, V), 0.0), u_Material.shininess);
    // return spec * u_Light.specularColor * u_Material.specularColor;

    // Blinn-Phong model
    vec3 H = normalize(L + V);
    float NdotH = max(dot(N, H), 0.0);
    float spec = pow(NdotH, u_Material.shininess);

    if (u_Material.hasTextureSpecular == 1) {
        return spec * u_Light.specularColor * texture2D(u_TextureSpecular, v_TexCoords).r;
    }
    
    return spec * u_Light.specularColor * u_Material.specularColor;
}

// Main ========================================================
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
    // vec3 N = normalize(v_Normal);
    // vec3 L = normalize(u_Light.direction);
    // vec3 diffuseColor = diffuse(N, L);
    // vec3 finalColor = diffuseColor + ambient();
    // gl_FragColor = vec4(finalColor, 1.0);

    // Ambiant + Diffuse + Texture -----------------------------------------------------
    // vec4 texColor = texture2D(u_Texture, v_TexCoords);
    // vec3 N = normalize(v_Normal);
    // vec3 L = normalize(u_Light.direction);
    // vec3 diffuseColor = diffuse(N, L);
    // vec3 finalColor = (diffuseColor + ambient()) * texColor.rgb;
    // gl_FragColor = vec4(finalColor, texColor.a);

    // Specular color ------------------------------------------------------
    // Simule les reflets de la lumiere sur la surface de l'objet
    // vec3 N = normalize(v_Normal);
    // vec3 L = normalize(u_Light.direction);
    // vec3 V = normalize(u_ViewPosition - gl_FragCoord.xyz); // Calculer le vecteur vue
    // vec3 specularColor = specular(N, L, V);
    // gl_FragColor = vec4(specularColor, 1.0);

    // Phong color ---------------------------------------------------------
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_Light.direction);
    vec3 V = normalize(u_ViewPosition - gl_FragCoord.xyz);

    vec3 ambientColor = ambient();
    vec3 diffuseColor = diffuse(N, L);
    vec3 specularColor = specular(N, L, V);

    if (u_Material.hasTexture == 1) {
        vec4 texColor = texture2D(u_Texture, v_TexCoords);
        vec3 result = texColor.rgb * (ambientColor + diffuseColor) + specularColor;
        gl_FragColor = vec4(result, texColor.a);
    } else {
        vec3 result = ambientColor + diffuseColor + specularColor;
        gl_FragColor = vec4(result, 1.0);
    }
    

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