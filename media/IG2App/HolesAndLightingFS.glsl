#version 330 core

uniform sampler2D textura0; // ‐> unidades de textura (int)
uniform sampler2D textura1; // tipo sampler2D para texturas 2D
uniform sampler2D textura2; // ‐> unidades de textura (int)

uniform vec3 lightAmbient; // intensidades de la luz
uniform vec3 lightDiffuse;
uniform vec4 lightPosition;

uniform vec4 materialDiffuse;

uniform vec3 ocre;
uniform vec3 azul;

in vec2 vUv0; // out del vertex shader
in vec3 vGlobalNormal; // coordenadas de la normal en camera space
in vec4 vGlobalVertex; // coordenadas del vértice en camera space

out vec4 fFragColor; // out del fragment shader

float diff(vec3 vVertex, vec3 vNormal) {
    vec3 lightDir = lightPosition.xyz; // directional light ?
    if(lightPosition.w == 1) // positional light ?
        lightDir = lightPosition.xyz - vVertex;
        
    return max(dot(vNormal, normalize(lightDir)), 0.0);
}

void main() {
    vec3 color2 = vec3(texture(textura2, vUv0)); // configuración!

    if(color2.r > 0.6)
        discard;

    vec3 ambient = lightAmbient * materialDiffuse.rgb;

    if(gl_FrontFacing) {
        vec3 color0 = vec3(texture(textura0, vUv0)); // configuración!
        color0 *= (ocre + ambient);

        float diff = diff(vec3(vGlobalVertex), vGlobalNormal); // positive normal
        vec3 diffuse = vec3(lightDiffuse) * diff * materialDiffuse.rgb;

        fFragColor = vec4(color0 * diffuse, 1.0);
    } else {
        vec3 color1 = vec3(texture(textura1, vUv0)); // acceso a téxel
        color1 *= (azul  + ambient);

        float diff = diff(vec3(vGlobalVertex), -vGlobalNormal); // negative normal
        vec3 diffuse = vec3(lightDiffuse) * diff * materialDiffuse.rgb;

        fFragColor = vec4(color1 * diffuse, 1.0);
    }
}