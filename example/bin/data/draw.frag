#extension GL_ARB_texture_rectangle : enable

uniform vec2 resolution;
uniform sampler2DRect gpuCompute0;
uniform sampler2DRect tex;

void main(void) {
	
	vec4 color; 

	if ( (gl_FragCoord.x > 0.0) && (gl_FragCoord.x < resolution.x - 1.0) &&
		(gl_FragCoord.y > 0.0) && (gl_FragCoord.y < resolution.y - 1.0) )
  	{
		vec2 hm = resolution/2.0; //la mitad de la pantalla
		float d = texture2DRect(gpuCompute0,gl_FragCoord.xy).x;
		float data = 1024.0 - d;
		//data = 0.0;
		float xx = ((gl_FragCoord.x-hm.x)*data/1024.0)+hm.x;
		float yy = ((gl_FragCoord.y-hm.y)*data/1024.0)+hm.y;
		vec2 v;
		v.x = xx; 
		v.y = yy;
		//v.x = gl_FragCoord.x;
		//v.y = gl_FragCoord.y;
		//bounds check
		if (v.x>=resolution.x-1.0) v.x=resolution.x-1.0;
		if (v.x<=0.0) v.x=0.0;
		if (v.y>=resolution.y-1.0) v.y=resolution.y-1.0;
		if (v.y<=0.0) v.y=0.0;

		//color = texture2DRect(tex,gl_FragCoord.xy);
		color = texture2DRect(tex,v);
		
		if (abs(data) < 1000.0)
		{
			float alpha = (1024.0 / data);
			if (alpha < 0.0) alpha = alpha * -1.0;
			float e = ((color.x + color.y + color.z) / 3.0) * 0.1;
			alpha = alpha * e;
			color.x = color.x + alpha;
			color.y = color.y + alpha;
			color.z = color.z + alpha;
			if (color.x > 1.0) color.x = 1.0;
			if (color.y > 1.0) color.y = 1.0;
			if (color.z > 1.0) color.z = 1.0;
		}
		
	}
	else
	{
		color = texture2DRect(tex,gl_FragCoord.xy);
	}
	
	
	//color = vec4(1);
	//color = texture2DRect(tex, gl_FragCoord.xy);
	//color.x = gl_FragCoord.x / resolution.x;
	//color.w = 1;
	gl_FragColor = color;

}
