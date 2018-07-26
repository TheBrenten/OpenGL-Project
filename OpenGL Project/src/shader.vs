#version 120

attribute vec3 position;
//attribute vec2 texCoord;
attribute vec3 normal;

//varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	//texCoord0 = texCoord;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}



//#version 330 core
//
//layout (location = 0) in vec3 aPos;
//
//// Input vertex data, different for all executions of this shader.
//layout(location = 0) in vec3 vertexPosition_modelspace;
//
//// Values that stay constant for the whole mesh.
//uniform mat4 MVP;
//
//void main()
//{
//    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
//    //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//}