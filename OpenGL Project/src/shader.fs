//#version 330 core
#version 120

//varying vec2 texCoord0;
varying vec3 normal0;

//uniform sampler2D sampler;
uniform vec3 lightDirection;

void main()
{
	//gl_FragColor = texture2D(sampler, texCoord0) * 
	//	clamp(dot(-lightDirection, normal0), 0.0, 1.0);
    //gl_FragColor = vec4(0,0.5,0.5,1) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
    gl_FragColor = vec4(0,0.5,0.5,1) * clamp(dot(-lightDirection, normal0)*.16+.2, 0.02, 1.0);

}



////version 330 core
//
//void main (void)  
//{
//                  //   R    G    B    A
//   gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);  
//}