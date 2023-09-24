#version 330 core

uniform sampler2D textura0;
uniform sampler2D textura1;
uniform sampler2D textura2;
in vec2 vUv0;
out vec4 fFragColor;

void main(){
    vec3 color0 = vec3(texture(textura0,vUv0));
    vec3 color1 = vec3(texture(textura1,vUv0));
    vec3 color2 = vec3(texture(textura2,vUv0));
    if(color0.r > 0.6){
        if(gl_FrontFacing){
            fFragColor = vec4(color1,1.0);
        }
        else{
            fFragColor = vec4(color2,1.0);
        }
    }
    else{
        discard;
    }
}