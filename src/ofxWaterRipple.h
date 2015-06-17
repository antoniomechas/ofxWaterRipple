//
// created by antoniomechas, 2015/06/17
// 
//
// Ported to shaders from http://www.neilwallis.com/projects/java/water/index.php
//

#pragma once

#include "ofMain.h"
#include "GpuCompute.h"

class ofxWaterRipple {

	public:
		~ofxWaterRipple();


		void	setup(int w, int h);
		void	setupGPU();
		void	reset ();
		void	update();
		void	loadImage(const char* archivo);
		void	loadImage(ofPixelsRef img);
		void	draw(bool bUseShader);
		void	disturb(float x, float y, int radio, int profundidad);
		void	gotaPerimetro(float x, float y, int radio);
		void	tocaPunto(int x, int y, int profundidad);
		void	generaLluvia();
		void	siguienteFoto();										//Pasa a la siguiente foto de la lista

		float	pDamping;			//Factor de damping


protected:

		int SquaredDist(int sx, int sy, int dx, int dy);

		GpuCompute gpuCompute;

		int _height;
		int _width;
		
		float* data;

};