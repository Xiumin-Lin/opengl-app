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

// GAMMA
varying vec3 v_Color;
const float gamma = 2.2;

vec3 sRGBToLinear(vec3 color) {
    return pow(color, vec3(gamma));
}

vec3 linearTosRGB(vec3 color) {
    return pow(color, vec3(1.0 / gamma));
}

// Main ========================================================
void main() {
    // GAMMA color -------------------------------------------------------
    vec3 linearColor = sRGBToLinear(v_Color); // Convertir de sRGB à linéaire
    vec3 outputColor = linearTosRGB(linearColor); // Convertir de linéaire à sRGB pour l'affichage
    gl_FragColor = vec4(outputColor, 1.0);
}