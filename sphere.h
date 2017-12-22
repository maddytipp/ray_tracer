//
//  sphere.h
//  raytra_1
//

#ifndef SPHERE_H_
#define SPHERE_H_

#include <iostream>

#include "ray.h"
#include "surface.h"


//
//
//
class Sphere : public Surface {


public:
    Sphere (mypoint &pos, double r) { center_ = pos; radius_ = r; }
    virtual ~Sphere () {}
    
    virtual bool intersect (const Ray &r,
                            double &t, 
                            mypoint &ipoint, 
                            myvector &normal) const;
    
private:
    
    mypoint center_;
    double  radius_;
};

#endif
