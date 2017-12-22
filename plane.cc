//
//  plane.cc
//  raytra_1
//
//  Created by michael on 10/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "plane.h"


Plane::Plane (
    const myvector &normal, 
    const double d)
{
    
    N_ = normal;
    d_ = d;
    
}


bool Plane::intersect (
    const Ray &r,
    double &t, 
    mypoint &ipoint, 
    myvector &normal
    ) const
{
    
 
    // t = - (ray.p0 dot N - d) / ray.dir dot N
    
    double denom = r.getDir().dotProduct(N_);
    
    if (denom == 0.) {
        return false;
    }
    
    const mypoint r_orig = r.getOrigin();
    
    myvector my_orig_vec (r_orig[0], r_orig[1], r_orig[2]);
    double ray_point_dot_n = my_orig_vec.dotProduct(N_);
    
    t = -1. * (ray_point_dot_n - d_) / denom;
    
    if (t <= 0) return false;
    
    ipoint = r_orig + r.getDir()*t;
    
    normal = N_;
    
    return true;
    
}
