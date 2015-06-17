//#extension GL_ARB_texture_rectangle : enable
//#extension GL_EXT_gpu_shader4 : enable

#version 120
#extension GL_ARB_texture_rectangle : enable

// ping pong inputs
uniform sampler2DRect gpuCompute0; //data 
uniform sampler2DRect gpuCompute1; //add
uniform sampler2DRect old; 
uniform float damping;
uniform vec2 resolution;

float fromImageOld(vec2 v)  
{
	//vec4 d = texture2DRect(gpuCompute0, v);
	//float n = (d.x * 255.0) + (d.y * 255.0 * 256.0);

	//return( n - 10000.0);
	//return (d.x);

	return(texture2DRect(gpuCompute0, v).x);

}

   

void main()
{
    //vec3 data = texture2DRect(gpuCompute0, gl_TexCoord[0].st).xyz;
    //vec3 vel = texture2DRect(gpuCompute1, gl_TexCoord[0].st).xyz;
    vec4 resultado = vec4(0);
	if ( (gl_FragCoord.x > 0.0) && (gl_FragCoord.x < resolution.x ) &&
		(gl_FragCoord.y > 0.0) && (gl_FragCoord.y < resolution.y ) )
	{
		vec2 v = gl_FragCoord.xy;
		float data = fromImageOld( vec2(v.x,v.y+1.0) ) +
			fromImageOld( vec2(v.x,v.y-1.0) ) +
			fromImageOld( vec2(v.x+1.0,v.y) ) +
			fromImageOld( vec2(v.x-1.0,v.y) );
		data = data / 2.0;
		//data = data / 2.0;
		//data = data - fromImageNew(v);
		data = data - texture2DRect(old, v).x;
		data = data + texture2DRect(gpuCompute1, v).x;
		data = data * damping;
		//data = data * .70;
		// if (data < 10.0) data = 0.0;
		// if (data > 1024) data = 1024;
		resultado = vec4(data,0,0,0);
	} 
    gl_FragData[0] = resultado;
    gl_FragData[1] = vec4(0);
    //gl_FragData[1] = vec4(vel, 0.0);
}

