#version 150
in vec3 vertex;
in vec3 normal;
in mat4 modelMatrix; // instance data
out vec3 vert;
out vec3 vertNormal;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;


void main()
{
   vert = (vertex).xyz;
   vertNormal = (transpose(inverse(modelMatrix)) * vec4(normal,1.0)).xyz;
   gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertex,1.0);
}
