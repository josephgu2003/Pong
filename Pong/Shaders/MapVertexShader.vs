#version 410 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 TexCoords_;
    
    out vec2 TexCoords;

    uniform mat4 modelMat;

    uniform mat4 viewMat;

    uniform mat4 projMat;

    void main()
    {
        gl_Position =  projMat * viewMat * modelMat * vec4(aPos.x , aPos.y , aPos.z, 1.0);
        TexCoords = TexCoords_;
    }
