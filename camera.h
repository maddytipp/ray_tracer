//
//  camera.h
//  raytra_1
//

#ifndef CAMERA_H_
#define CAMERA_H_

#include "basicmath.h"
#include "ray.h"

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

using namespace Imf;


class Surface;
class Light;
class Material;

//
//
//
class Camera {
    
public:
    
    void    init (
                  mypoint &pos,
                  myvector &dir,
                  double d,
                  double iw,
                  double ih,
                  int pw,
                  int ph);

    Ray     generateRay (const int i, const int j);
    void    renderScene (std::vector< Surface * > &surfaces,
                         std::vector< Light * > &lights,
                         std::vector< Material * > &materials);
    
    void    writeImage (const char *sceneFile);
    
    void    setPixel (int pix_x, int pix_y, float r, float g, float b) {
        Rgba &px = pixelBuffer_[pix_y][pix_x];
        px.r = r;
        px.g = g;
        px.b = b;
        px.a = 1.0;
    }
    
    mypoint position () const { return eye_; }
    
private:
    
    // right-handed coordinate system:
    myvector   u_;  // u goes to the "right" with respect to the image plane
    myvector   v_;  // v goes "up" with respect to the image plane
    myvector   w_;  // remember: points in OPPOSITE the viewing direction!
    
    mypoint eye_;   // position of camera, & center of projection 
    
    double   image_width_;
    double   image_height_;
    
    double   focal_length_;
    
    int     pixels_width_;
    int     pixels_height_;
    
    Array2D<Rgba>    pixelBuffer_;  // the image we are rendering
    
};

#endif
