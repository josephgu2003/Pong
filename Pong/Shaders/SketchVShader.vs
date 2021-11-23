#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoords_;
layout (location = 2) in float arrayTexId_;

    out vec2 TexCoords;

  //  uniform float z_offset[2500];
    
    uniform mat4 modelMat;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;

};
    
    void main()
    {
        gl_Position =  viewProjMat * modelMat * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        TexCoords = TexCoords_;
    }



 
