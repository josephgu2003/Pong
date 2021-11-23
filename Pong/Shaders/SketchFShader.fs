#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
    in vec2 TexCoords;



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

layout (std140) uniform StopWatch
{
    float time;
};

    uniform sampler2D diffuse;

uniform float alpha;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    float alpha_ = texture(diffuse, TexCoords).a;
    if(alpha_ < 0.5)
        discard;

    vec3 fragColor = vec3(0.0);
    
    fragColor += vec3(texture(diffuse, TexCoords));
    
    fragColor *= 2.0;
    
    //Yirou these next 3 lines can provide fish vfx, my version makes colors flash based on how much red they have, i couldn't figure out anything cooler yet
    FragColor = vec4(fragColor, alpha);
    
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
       if(brightness > 0.0)
           BrightColor = vec4(FragColor.rgb, 1.0)*alpha;
       else
          BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
