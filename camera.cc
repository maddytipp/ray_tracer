//
//  camera.cc
//  raytra_1
//

#include <vector>  // std::vector - template for array-like structures.
#include <cmath>
#include <limits>

#include "camera.h"
#include "surface.h"
#include "ray.h"
#include "lights.h"
#include "material.h"


#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

using namespace Imf;



//
// Initialize a camera, given its paraemters.
// Called from the parser, after it has read in the description from
// the scene file.
//
void Camera::init (
    mypoint &pos,
    myvector &dir,
    double d,
    double iw,
    double ih,
    int pw,
    int ph)
{
    eye_ = pos;
    w_ = dir.invert ();
    w_.normalize ();
    
    focal_length_ = d;
    
    image_width_ = iw;
    image_height_ = ih;
    
    pixels_width_ = pw;
    pixels_height_ = ph;
    
    // make an orthonormal frame for the camera:
    if (1.0 - fabs (w_[1]) < .0001) {
        // camera is pointing up or down - make an arbitrary
        // right-handed coordinate system:
        u_.set (1.0, 0.0, 0.0);
        v_.crossProduct (w_, u_);
    }
    else {
        myvector up (0., 1., 0.);
        u_.crossProduct (up, w_);
        v_.crossProduct (w_, u_);
    }

    u_.normalize ();
    v_.normalize ();
    
    // set up the image:
    // OpenEXR uses exceptions - catch and print on error

    try
    {
    pixelBuffer_.resizeErase(pixels_height_, pixels_width_);    
    }
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }
   
    
}


//
// Generate a ray for a specific pixel:
//
Ray Camera::generateRay (
    const int i, // i is in the u direction (similar to X in cartesian)
    const int j) // j is in the v direction (similar to Y in cartesian)
{
    
    
    // get the u and v values that correspond to i and j:
    
    // we are assuming a centered image here - but maybe that
    // will change in the future, so let's calculate u,v from
    // the more general image plane description:
    double r = image_width_ / 2.0;
    double l = -1. * r;
    double t = image_height_ / 2.0;
    double b = -1. * t;

    double u = l + (r - l) * (i + .5) / pixels_width_;
    double v = t + (b - t) * (j + .5) / pixels_height_;
    
    
    // compute the direction vector d based on this:
    myvector d = w_ * -1.0 * focal_length_;
    d += u_ * u;
    d += v_ * v;
        
    d.normalize ();
        
    return Ray (eye_, d);;
    
}


//
// Render the scene:
//
void Camera::renderScene (
    std::vector< Surface * > &surfaces,
    std::vector< Light * > &lights,
    std::vector< Material * > &materials)
{
    
    std::cout << "rendering" << std::endl;
    
    int printProgress = pixels_height_ * pixels_width_ / 10;
        
    // for each of the pixels, generate a ray, then test against the 
    // scene for intersection:
    
    for (int yy = 0; yy < pixels_height_; ++yy) {
        for (int xx = 0; xx < pixels_width_; ++xx) {
            
            // print one of these for every 1/10th of the image:
            if (printProgress && 
                (yy * pixels_width_ + xx) % printProgress == 0)
                std::cout << "." << std::flush;
            
            Ray r = generateRay (xx, yy);
                                
            myvector ret_rgb = Material::reflection(r, .0001, INFINITY, 6, 000, lights, materials, surfaces);
                
            setPixel (xx, yy, ret_rgb[0], ret_rgb[1], ret_rgb[2]);
                
            
        }
    }
    
    std::cout << "done!" << std::endl;
    
}

//
// Save out the final image:
//
void Camera::writeImage (
    const char *sceneFile)
{
    
    // write the image data as an openEXR file
    // OpenEXR uses exceptions - catch and print on error
    try
    {
    RgbaOutputFile file (sceneFile, pixels_width_, pixels_height_, WRITE_RGBA);
    file.setFrameBuffer (&pixelBuffer_[0][0], 1, pixels_width_);
    file.writePixels (pixels_height_);
    }
    
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }

}


