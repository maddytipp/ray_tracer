//
//  material.cc
//  raytra_1
//

#include "material.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

myvector Material::backside_phongShading (const myvector &ray,    // unit vector to the light
                                          const myvector &N) const    // unit surface normal

{
    
    
    myvector R (0., 0., 0.); // the amount of light (color) returned along the ray
    
    myvector norm_copy = N;
    norm_copy.invert();
    
    
    double n_dot_l = ray.dotProduct (norm_copy);
    //double n_dot_e = norm_copy.dotProduct(e);;
    
    if (n_dot_l > 0.) {
        myvector yellow (1., 1., 0.);
        R += yellow;
    }
    else {
        return R;
    }

    return R;
}

myvector Material::phongShading (
    const myvector &L,    // unit vector to the light
    const myvector &e,    // unit vector to the viewer
    const myvector &N,    // unit surface normal
    const myvector &L_e) const
{
    

    myvector R (0., 0., 0.); // the amount of light (color) returned along the ray
    
    double n_dot_l = N.dotProduct (L);
    double n_dot_e = N.dotProduct(e);;
    
    if (n_dot_l > 0.) {
        myvector diff (k_diffuse_[0] * L_e[0] * n_dot_l,
                       k_diffuse_[1] * L_e[1] * n_dot_l,
                       k_diffuse_[2] * L_e[2] * n_dot_l);
        R += diff;
    }
    else {
        return R;
    }
    
    // compute the specular component:
    myvector half_vec = e;
    half_vec += L;
    
    // check to make sure the vector towards the light, and the 
    // vector towards the viewer, are not opposite (if they are
    // normalize() will fail)
    if (half_vec.length () > 0.) {
        
        half_vec.normalize ();
    
        double n_dot_h = N.dotProduct (half_vec);
        
        if (n_dot_h > 0.) {
            n_dot_h = pow (n_dot_h, p_);
            myvector spec (k_specular_[0] * L_e[0] * n_dot_h,
                           k_specular_[1] * L_e[1] * n_dot_h,
                           k_specular_[2] * L_e[2] * n_dot_h);
        
            R += spec;
        }
    } 

    
    return R;
    
}

myvector Material::reflection (Ray ray,
                               double min_t,
                               double max_t,
                               int recurse_limit,
                               int ray_type,
                               std::vector<Light *> &lights,
                               std::vector<Material *> &materials,
                               std::vector<Surface *> &surfaces)
{
    myvector tmp_rgb; // just added
    
    myvector ret_rgb (0., 0., 0.);
    
    if (recurse_limit == 0) {
        return myvector(0., 0., 0.);
    }
    
    // largest possible double
//    double best_t = std::numeric_limits<double>::max();
    double best_t = INFINITY;
    
    if (ray_type == 666) {
        mypoint ipoint;     // intersction point on surface
        myvector normal;    // normal at intersection point
        for (int whichSurf = 0; whichSurf < surfaces.size (); ++whichSurf) {
            
            Surface *s = surfaces[whichSurf];
            
            double t;           // ray parameterization of intersection point
            mypoint temp_point;     // intersction point on surface
            myvector temp_normal;    // normal at intersection point
            
            if (s->intersect (ray, t, temp_point, temp_normal) && min_t <= t && t <= max_t) {
                return myvector(0., 0., 0.);
            }
        }
            Light light = *lights.back();
            return light.spd();
        
    }

    
    // get closest intersection w scene, along ray
    
    mypoint ipoint;     // intersction point on surface
    myvector normal;    // normal at intersection point
    int materialId = 0; // which material to use
    
    bool foundIntersection = false;
    
    // select closest object to shade
    for (int whichSurf = 0; whichSurf < surfaces.size (); ++whichSurf) {
        
        Surface *s = surfaces[whichSurf];
        
        double t;           // ray parameterization of intersection point
        mypoint temp_point;     // intersction point on surface
        myvector temp_normal;    // normal at intersection point
        
        if (s->intersect (ray, t, temp_point, temp_normal) &&
            t < best_t && t > .0000001) {
            foundIntersection = true;
            ipoint = temp_point;
            normal = temp_normal;
            best_t = t;
            materialId = s->materialIndex (); // save for later!
            

            //for (int whichLight = 0; whichLight < lights.size (); ++whichLight){
            if(ray_type != 666) { //&& temp_normal.dotProduct(ray.getDir()) > 0){
                    // compute the direction to the light (assuming 1 light):
//                    Light *lgt = lights[whichLight];
//                    myvector L_e = lgt->spd ();
//                    myvector L = lgt->position () - ipoint;
//                    L.normalize ();
//
//                    myvector e = ray.getDir ();
//                    //e = e.invert ();    // already normalized
                ret_rgb = materials[materialId]->backside_phongShading (ray.getDir(), normal);
//                return ret_rgb;
            }
            //}
        }
    }
    
    // if no intersection, return [0 0 0]
    if (foundIntersection == false) {
        return myvector(0., 0., 0.);
    }
    
    for (int whichLight = 0; whichLight < lights.size (); ++whichLight){
        
        // compute the direction to the light (assuming 1 light):
        Light *lgt = lights[whichLight];
        myvector L_e = lgt->spd ();
        myvector L = lgt->position () - ipoint;
        
        // don't look beyond this for a showing object
        double shadowT = L.length();
        double lightdistsq = shadowT * shadowT;
        
        L.normalize ();
        
        myvector e = ray.getDir ();
        e = e.invert ();    // already normalized
        
        // compute s_ray
        Ray s_ray (ipoint, L);
        
        std::vector<Light *> the_light;
        the_light.push_back(lgt);
       
        myvector L_rgb = reflection(s_ray, 0.0001, max_t, 1, 666, the_light, materials, surfaces);
        
        if (L_rgb.length() > 0) {
            // set up shading calulation
//            myvector tmp_rgb;
            
            tmp_rgb = materials[materialId]->phongShading (L, e, normal, L_e);
            
            // scale by 1 over distance squared:
            ret_rgb += tmp_rgb * (1.0 / lightdistsq);
        }
    }
    
    //if material is reflective, compute reflected ray ref_ray
    Material m = *materials[materialId];
    if (m.isReflective()) {
        
        myvector direction = ray.getDir() - ( normal * (2.0 * ray.getDir().dotProduct(normal)) );
        direction.normalize();
        Ray ref_ray(ipoint, direction);
        
        
        return (ret_rgb += m.get_reflective().multiply(reflection(ref_ray, .0001, INFINITY, recurse_limit-1, 000, lights, materials, surfaces)));
    }
    
    return ret_rgb;
}

