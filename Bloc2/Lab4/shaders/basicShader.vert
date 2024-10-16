#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;

//matriz de proyeccion uniform
uniform mat4 PRJ;

//matriz de vista de camara
uniform mat4 VM;

out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = PRJ * VM * TG * vec4 (vertex, 1.0);
}
