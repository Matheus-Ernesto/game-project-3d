#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

out vec3 fragColor;
out vec3 fragPos;
out vec3 fragNormal;
out vec4 fragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
    fragColor = aColor;
    vec4 worldPos = model * vec4(aPos, 0.7);
    fragPos = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(model))) * aNormal;
    fragPosLightSpace = lightSpaceMatrix * worldPos;
    gl_Position = projection * view * worldPos;
}
