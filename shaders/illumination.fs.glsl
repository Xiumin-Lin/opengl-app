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

// uniform vec2 u_Dimensions; // dimensions de la fenetre

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

uniform vec3 u_LightAmbientIntensity;  // Intensité lumineuse ambiante
uniform vec3 u_SkyColor;
uniform vec3 u_GroundColor;
uniform vec3 u_SkyDirection;  // Normalement (0, 1, 0)

vec3 calculateAmbient() {
    float NdotSky = max(dot(normalize(v_Normal), u_SkyDirection), 0.0);
    float HemisphereFactor = NdotSky * 0.5 + 0.5;
    return u_LightAmbientIntensity * mix(u_GroundColor, u_SkyColor, HemisphereFactor);
}

// Main ========================================================
void main() {
    // Phong color + ILLUMINATION---------------------------------------------------------
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_Light.direction);
    vec3 V = normalize(u_ViewPosition - gl_FragCoord.xyz);

    // ILLUMINATION Hemisphere
    vec3 ambientColor = calculateAmbient();
    
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
}


