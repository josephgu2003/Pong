#version 410 core
layout (location = 0) in vec4 vertex;

    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(vertex.x, vertex.y, 0, 1.0);
        TexCoords = vec2(vertex.z, vertex.w);
    }
