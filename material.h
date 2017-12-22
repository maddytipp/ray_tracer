//
//  material.h
//  raytra_1
//

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector> 
#include "basicmath.h"
#include "ray.h"
#include "lights.h"
#include "material.h"
#include "surface.h"

class Material {
    
public:
    
    Material () {   // default plastic material
        k_diffuse_.set (.2, .3, .8);
        k_specular_.set (.9, .9, .9);
        k_reflective_.set (0., 0., 0.);
        p_ = .05; 
    }
    
    Material (myvector &k_d, myvector &k_s, myvector &k_r, double p) {
        k_diffuse_ = k_d; k_specular_ = k_s; k_reflective_ = k_r; p_ =p;
    }
    
    const myvector get_reflective() const {
        return k_reflective_;
    }
    
    virtual ~Material () {};
        
    static myvector reflection(Ray ray,
                        double min_t,
                        double max_t,
                        int recurse_limit,
                        int ray_type,
                        std::vector<Light *> &lights,
                        std::vector<Material *> &materials,
                        std::vector<Surface *> &surfaces);
                         
                         
    // compute phong shading
    myvector backside_phongShading (const myvector &ray, const myvector &N) const;
    myvector phongShading (
                const myvector &L,    // unit vector to the light
                const myvector &e,    // unit vector to the viewer
                const myvector &N,    // unit surface normal
                const myvector &L_e) const; //
    
    bool isReflective() {
        const double *r = k_reflective_.get_ijk();
        if(r[0] == 0. && r[1] == 0. && r[2] == 0.) {
            return false;
        }
        return true;
    }
    
    
private:
    
    myvector    k_diffuse_;     // diffuse coefficient
    myvector    k_specular_;     // specular coefficient
    myvector    k_reflective_;  // total reflection coefficient
    
    double      p_;             // phong exponent
    
};

#endif
