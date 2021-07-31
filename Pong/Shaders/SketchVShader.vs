#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normals_;
layout (location = 2) in vec2 TexCoords_;

    out vec2 TexCoords;
out vec3 Normals;
out vec3 fragPos;

  //  uniform float z_offset[2500];
    
    uniform mat4 modelMat;

layout(std140) uniform ViewProj
{
    mat4 viewProjMat;

};
    
    void main()
    {
       // gl_Position =  projMat * viewMat * modelMat*vec4(size*aPos, 1.0);
        gl_Position =  viewProjMat * modelMat * vec4(aPos, 1.0);
        fragPos = vec3(modelMat*vec4(aPos, 1.0));
        TexCoords = TexCoords_;
        Normals = mat3(transpose(inverse(modelMat))) * Normals_;
    }



