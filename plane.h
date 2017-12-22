//
//  plane.h
//  raytra_1
//


#ifndef PLANE_H_
#define PLANE_H_

#include <iostream>

#include "ray.h"
#include "surface.h"


//
//
//
class Plane : public Surface {
    
    
public:
    Plane (const myvector &normal, const double d);
    virtual ~Plane () {}
    
    virtual bool intersect (const Ray &r,
                            double &t, 
                            mypoint &ipoint, 
                            myvector &normal) const;
    
private:
    
    myvector    N_;
    double      d_;

};


#endif
