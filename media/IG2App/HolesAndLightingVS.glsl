#version 330 core

in vec4 vertex;                     // atributos de los vertices a procesar
in vec3 normal;
in vec2 uv0;

uniform mat4 modelViewProjMat;      // matriz de transformacion
uniform mat4 normalMat;             // matriz inversa (normales)
uniform mat4 modelViewMat;      // matriz de transformacion

out vec2 vUv0;                 // out del vertex shader
out vec3 vGlobalNormal;
out vec4 vGlobalVertex; 

void main() {
    vUv0 = uv0;
    vGlobalNormal = normalize(mat3(normalMat) * normal);
    vGlobalVertex = vec4(modelViewMat * vertex);

    gl_Position = modelViewProjMat * vertex;
}