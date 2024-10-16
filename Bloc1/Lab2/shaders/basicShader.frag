#version 330 core

//variable de ejemplo para recibir desde el vertex shader de entrada
//in float pepe;

//variable de salida del vertex shader para colorear los fragmentos
out vec4 FragColor;

//input de color que viene del output del vertex shader
in vec4 outputColor;

void main() {
    
    //gl_FragCoord es una variable que no hace definir y hace referencia a la posicion del fragmento
    //en la pantalla, no en el cubo de opengl
    //if (gl_FragCoord.x < 354.)
    //FragColor = vec4(1.,0.,0.,1);

    //Exercici 1
    //if (gl_FragCoord.x < 713/2 - 1 && gl_FragCoord.y > 708/2) FragColor = vec4(1,0,0,1);
    //else if (gl_FragCoord.x > 713/2 - 1 && gl_FragCoord.y > 708/2) FragColor = vec4(0,0,1,1);
    //else if (gl_FragCoord.x < 713/2 - 1 && gl_FragCoord.y < 708/2) FragColor = vec4(1,1,0,1);
    //else FragColor = vec4(0,1,0,1); //default green

    //Exercici 2
    //if (int(gl_FragCoord.y)%10 > 5 && int(gl_FragCoord.y)%10 <= 10) discard;

    //Exercici 3 multiplicar vertex * 0.5 en el vertex shader

    //Exercici 4 crear un vbo nuevo en myglwidget que represente los colores de los vertices y
    // pasar el vbo del vertex shader al fragment shader
    FragColor = outputColor;
}

