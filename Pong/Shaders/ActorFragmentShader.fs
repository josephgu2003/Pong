#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec3 Normals;


in VS_OUT {
    vec3 fragPos;
    vec2 TexCoords;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Light {
vec3 pos;

vec3 ambient;
vec3 diffuse;
vec3 specular;
    
};

struct DirLight {
vec3 dir;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

layout (std140) uniform Lights
{
   Light light;

    DirLight dirLight;
    
    vec3 viewPos;
};

    uniform sampler2D diffuse;
uniform sampler2D specular;
    uniform sampler2D normMap;
uniform float brightness;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = vec3(fs_in.TangentViewPos-fs_in.TangentFragPos);
    float fogFactor = exp(-pow(length(viewDir)*0.03,2));
    vec3 norm = normalize(texture(normMap, fs_in.TexCoords).rgb * 2.0 - 1.0);
    
    vec3 fragColor = vec3(0.0);
    
    fragColor += CalcDirLight(dirLight, norm, viewDir);

    FragColor = vec4(fragColor, texture(diffuse, fs_in.TexCoords).a);
    
    float brightnessLvl = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightnessLvl > 1.0)
           BrightColor = vec4(0.3*FragColor.rgb, 1.0);
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

vec3 CalcDirLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
    
    vec3 dirlightDir = normalize(-fs_in.TangentLightDir);
    
    float diff = 0;
    float spec = 0;

    viewDir = normalize(viewDir);
        
    diff = max(dot(norm,dirlightDir),0.0);
        
        vec3 reflectDir = reflect(-dirlightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 ambient = (vec3(brightness, brightness, brightness)+dirLight.ambient)*vec3(texture(diffuse, fs_in.TexCoords));

    
    vec3 diffuse = dirLight.diffuse*diff*vec3(texture(diffuse, fs_in.TexCoords));
    
//    vec3 specular = dirLight.specular * spec * vec3(texture(specular, TexCoords));

    return (ambient + diffuse);
}
