#version 330 core

in vec3 fragColor;
in vec3 fragPos;
in vec4 fragPosLightSpace;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    
    // Check if outside shadow map
    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || 
       projCoords.y < 0.0 || projCoords.y > 1.0) {
        return 0.0;
    }
    
    // PCF for soft shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            float currentDepth = projCoords.z;
            float bias = 0.005;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    return shadow;
}

void main() {
    // Calculate normal based on position
    vec3 normal;
    vec3 absPos = abs(fragPos);
    
    if(absPos.x > absPos.y && absPos.x > absPos.z) {
        normal = vec3(sign(fragPos.x), 0.0, 0.0);
    }
    else if(absPos.y > absPos.x && absPos.y > absPos.z) {
        normal = vec3(0.0, sign(fragPos.y), 0.0);
    }
    else {
        normal = vec3(0.0, 0.0, sign(fragPos.z));
    }
    
    normal = normalize(normal);
    
    // Light parameters
    float ambientStrength = 0.3;
    float specularStrength = 0.5;
    
    // Ambient
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Shadow
    float shadow = ShadowCalculation(fragPosLightSpace);
    
    // Final result
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * fragColor;
    
    color = vec4(result, 1.0);
}