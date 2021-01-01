#shader vertex
#version 300 es

precision highp float;
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 ourTexCoord;

uniform mat4 transformer;
uniform mat4 orthoMatrix;

void main()
{
    ourTexCoord = aTexCoord;

    gl_Position = orthoMatrix * transformer * vec4(aPos, 1.0, 1.0);
}

#shader fragment
#version 300 es

precision highp float;
layout (location = 0) out vec4 color;
in vec2 ourTexCoord;

uniform vec4 quadColor;
uniform sampler2D texture1;
uniform bool useTexture;

void main()
{
    if (useTexture)
        //color = texture(texture1, ourTexCoord) * quadColor;
        color = texture(texture1, ourTexCoord) * quadColor;
    else
        color = quadColor;
}
