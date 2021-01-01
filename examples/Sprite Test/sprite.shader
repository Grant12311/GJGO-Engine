#shader vertex
#version 300 es

precision highp float;
layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0, 1.0);
}

#shader fragment
#version 300 es

precision highp float;
layout (location = 0) out vec3 color;

void main()
{
    color = vec3(1.0, 1.0, 1.0);
}
