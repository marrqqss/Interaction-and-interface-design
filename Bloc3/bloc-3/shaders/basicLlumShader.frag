#version 330 core

in vec3 fcolor;
out vec4 FragColor;

uniform vec4 posFocusCam;
uniform vec4 posFocusEscena;
uniform vec3 colorFocus;
uniform vec3 llumAmbient;

in vec4 vertexSCO;
in vec3 normalSCO;

in vec3 matambFS;
in vec3 matdiffFS;
in vec3 matspecFS;
in float matshinFS;
in mat4 viewFS;

vec3 Ambient() {
    return llumAmbient * matambFS;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiffFS * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshinFS == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinFS);
    return (matspecFS * colFocus * shine); 
}

void main()
{	
  vec4 posFocusEscena2 = viewFS * posFocusEscena;
  vec3 color = Ambient();
	vec3 LSCOCam = normalize(posFocusCam.xyz - vertexSCO.xyz);
  vec3 LSCOEscena = normalize(posFocusEscena2.xyz - vertexSCO.xyz);
  color += Difus(normalSCO, LSCOCam, colorFocus) + Especular(normalSCO, LSCOCam, vertexSCO, colorFocus);
  color += Difus(normalSCO, LSCOEscena, colorFocus) + Especular(normalSCO, LSCOEscena, vertexSCO, colorFocus);
	FragColor = vec4(color, 1);
}
