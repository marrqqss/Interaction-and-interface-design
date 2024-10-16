#version 330 core

in vec3 vertex;

//input color del vbo
in vec3 color;

//output color + alpha para el fragment shader
out vec4 outputColor;

void main()  {
    gl_Position = vec4 (vertex*0.5, 1.0);
    outputColor = vec4 (color, 1.0);
}
