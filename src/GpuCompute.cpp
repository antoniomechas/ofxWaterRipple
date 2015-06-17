//
// created by antoniomechas, 2015/06/17
// 
// some code stolen and modified from Neil Mendoza's gpuParticleSystem
// https://github.com/neilmendoza/ofxGpuParticles
//
//

#include "GpuCompute.h"

const string GpuCompute::UNIFORM_PREFIX = "gpuCompute";

//-------------------------------------------------------
//

GpuCompute::GpuCompute() : currentReadFbo(0)
{
}
    
//-------------------------------------------------------
//
void GpuCompute::init(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;
    numFloats = width * height * FLOATS_PER_TEXEL;
        
    // fbos
    ofFbo::Settings s;
    s.internalformat = GL_RGBA32F_ARB;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.minFilter = GL_NEAREST;
    s.maxFilter = GL_NEAREST;
    s.wrapModeHorizontal = GL_CLAMP;
    s.wrapModeVertical = GL_CLAMP;
    s.width = width;
    s.height = height;
    s.numColorbuffers = 2;
    for (unsigned i = 0; i < 2; ++i)
    {
        fbos[i].allocate(s);
		fbos[i].begin();
			ofClear(0);
		fbos[i].end();
    }
    drawFbo.allocate(width,height);
    updateShader.load("update");
    drawShader.load("draw");
		
	imagen.loadImage("images/fondo.jpg");
	imagen.resize(width,height);
	imagen.update();
	tex.loadData(imagen.getPixelsRef());
}
    

//-------------------------------------------------------
//
void GpuCompute::update()
{
    fbos[1 - currentReadFbo].begin(false);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_BLEND);
    ofSetColor(255, 255, 255);
    fbos[1 - currentReadFbo].activateAllDrawBuffers();

    updateShader.begin();
		setUniforms(updateShader);
		int texIndex = fbos[currentReadFbo].getNumTextures();
        updateShader.setUniformTexture("old", fbos[1 - currentReadFbo].getTextureReference(0), texIndex);

		ofVec2f r(width, height);
		updateShader.setUniform2fv("resolution", r.getPtr());
		updateShader.setUniform1f("damping", paramDamping);
		texturedQuad();
    updateShader.end();

	glPopAttrib();
        
    fbos[1 - currentReadFbo].end();
        
    currentReadFbo = 1 - currentReadFbo;
}
    
//-------------------------------------------------------
//
void GpuCompute::draw()
{
	drawFbo.begin();
		drawShader.begin();
		setUniforms(drawShader);
		
		ofVec2f r(width, height);
		drawShader.setUniform2fv("resolution", r.getPtr());
		drawShader.setUniformTexture("tex", tex,2);
		drawShader.setUniform1f("damping",20);

		texturedQuad();
		drawShader.end();
	drawFbo.end();

	drawFbo.draw(0,0,width,height);
}
    
//-------------------------------------------------------
//
void GpuCompute::setUniforms(ofShader& shader)
{
    for (unsigned i = 0; i < fbos[currentReadFbo].getNumTextures(); ++i)
    {
		ostringstream oss;
        oss << UNIFORM_PREFIX << ofToString(i);
        //cout << "setUniforms: " << oss.str() << endl;
        shader.setUniformTexture(oss.str().c_str(), fbos[currentReadFbo].getTextureReference(i), i);
    }
}
    
//-------------------------------------------------------
//
void GpuCompute::loadDataTexture(unsigned idx, float* data,
                                    unsigned x, unsigned y, unsigned width, unsigned height)
{
    if (idx < fbos[currentReadFbo].getNumTextures())
    {
        if (!width) width = this->width;
        if (!height) height = this->height;
        fbos[currentReadFbo].getTextureReference(idx).bind();
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, x, y, width, height, GL_RGBA, GL_FLOAT, data);
        fbos[currentReadFbo].getTextureReference(idx).unbind();
    }
    else ofLogError() << "Trying to load data from array into non-existent buffer.";
}
    
//-------------------------------------------------------
//
void GpuCompute::zeroDataTexture(unsigned idx,
                                    unsigned x, unsigned y, unsigned width, unsigned height)
{
    if (!width) width = this->width;
    if (!height) height = this->height;
    float* zeroes = new float[width * height * FLOATS_PER_TEXEL];
    memset(zeroes, 0, sizeof(float) * width * height * FLOATS_PER_TEXEL);
    loadDataTexture(idx, zeroes, x, y, width, height);
    delete[] zeroes;
}
    
//-------------------------------------------------------
//
void GpuCompute::texturedQuad()
{
    // TODO: change to triangle fan/strip
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
        
    glTexCoord2f(1, 0);
    glVertex3f(width, 0, 0);
        
    glTexCoord2f(1, 1);
    glVertex3f(width, height, 0);
        
    glTexCoord2f(0, 1);
    glVertex3f(0, height, 0);
    glEnd();
}
    
