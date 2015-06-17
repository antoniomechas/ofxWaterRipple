//
// created by antoniomechas, 2015/06/17
// 
//
// Ported to shaders from http://www.neilwallis.com/projects/java/water/index.php
//


#include "ofxWaterRipple.h"
	
#define FACTOR 1
#define MAX_TEX 1024.0f

ofxWaterRipple::~ofxWaterRipple()
{
		
};

void ofxWaterRipple::setup(int w, int h)
{
	
	_height=h/FACTOR;
	_width=w/FACTOR;
	
	ofEnableArbTex();

	pDamping = 0.99f;		//Factor de damping

	setupGPU();

}


void ofxWaterRipple::setupGPU()
{

	//ofSetVerticalSync(true);

	unsigned w = _width;
    unsigned h = _height;

    gpuCompute.init(_width, _height);
    
    // initial data
	data = new float[w * h * 4];
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            unsigned idx = y * w + x;
			data[idx * 4] = 0; // x
            data[idx * 4 + 1] = 0; // y
            data[idx * 4 + 2] = 0.f; // z
            data[idx * 4 + 3] = 0.f; // dummy
        }
    }
    gpuCompute.loadDataTexture(GpuCompute::DATA, data);
    
}

void ofxWaterRipple::reset ()
{

}


void ofxWaterRipple::tocaPunto(int x, int y, int profundidad)
{
	int fw = _width;
	int fh = _height;

	//CRect bounds(1,1,fw-1,fh-1);
	ofRectangle bounds(1,1,fw-1,fh-1);

	if ( bounds.inside(ofPoint(x,y)) )
		data[y * (_width * 4) + (x * 4)] += profundidad;

}


void ofxWaterRipple::disturb(float x, float y, int radio, int profundidad)
{
	int fw = _width;
	int fh = _height;

	//int dx = x*fw;
	//int dy = y*fh;
	int dx = x;
	int dy = y;
	
	int radio_2 = radio * radio;
	int i,j,dist,finaldepth;
	ofRectangle bounds(1,1,fw-1,fh-1);
	for (j = dy - radio; j < dy + radio; j++)
	{
		for (i = dx - radio; i < dx + radio; i++)
		{
			if (bounds.inside(ofPoint(i,j)))
			{
				dist = SquaredDist(dx,dy,i,j);
				if (dist < radio_2)
				{
					finaldepth = (int)((float)profundidad/2.0f * ((float)(radio - sqrt((float)dist))/(float)radio));
					if (finaldepth > MAX_TEX) finaldepth = MAX_TEX;
					if (finaldepth < 0) finaldepth = 0;
					data[j * (_width * 4) + (i * 4)] = finaldepth;
				}
			}
		}
	}
}


void ofxWaterRipple::gotaPerimetro(float x, float y, int radio)
{
	int fw = _width;
	int fh = _height;

	int dx = x;
	int dy = y;
	
	int radio_2 = radio * radio;
	int i,j,dist,finaldepth;
	ofRectangle bounds(1,1,fw-1,fh-1);
	for (j = dy - radio; j < dy + radio; j++)
	{
		for (i = dx - radio; i < dx + radio; i++)
		{
			if (bounds.inside(ofPoint(i,j)))
			{
				dist = SquaredDist(dx,dy,i,j);
				if (dist > radio_2-2 && dist < radio_2+2)
				{
					finaldepth = (int)((float)(MAX_TEX/2.0f) * ((float)(radio - sqrt((float)dist))/(float)radio));
					data[j * (_width * 4) + (i * 4)] += MAX_TEX/2;
				}
			}
		}
	}
}


void ofxWaterRipple::draw(bool bUseShader)
{

	gpuCompute.draw();

}

int inline ofxWaterRipple::SquaredDist(int sx, int sy, int dx, int dy)
{
	return ((dx - sx) * (dx - sx)) + ((dy - sy) * (dy - sy));
}


void ofxWaterRipple::update()
{
    gpuCompute.paramDamping = pDamping;
	gpuCompute.loadDataTexture(GpuCompute::ADD, data);
	gpuCompute.update();
	memset(data,0,_width*_height*4*sizeof(float));
}


