//
// created by antoniomechas
//
// some code stolen and modified from Neil Mendoza's gpuParticleSystem
// https://github.com/neilmendoza/ofxGpuParticles
//
//

#pragma once

#include "ofMain.h"

class GpuCompute
{
public:
	static const string UNIFORM_PREFIX;
    static const unsigned FLOATS_PER_TEXEL = 4;
        
    enum DataTextureIndex
    {
        DATA,
        ADD
    };
        
    GpuCompute();
        
    void init(unsigned width, unsigned height);
    void update();
    void draw();
        
    void loadDataTexture(unsigned idx, float* data,
                            unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
    void zeroDataTexture(unsigned idx,
                            unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0);
        
    unsigned getWidth() const { return width; }
    unsigned getHeight() const { return height; }
    unsigned getNumFloats() const { return numFloats; }
        
	ofImage imagen;
	ofTexture tex;

	float paramDamping;

private:
    void texturedQuad();
    void setUniforms(ofShader& shader);
        
    ofFbo fbos[2];
    ofVboMesh mesh;
    ofShader updateShader, drawShader, addShader;
    unsigned currentReadFbo;
    unsigned width, height, numFloats;

	ofFbo drawFbo;

};
