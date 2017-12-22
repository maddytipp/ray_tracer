//
//  triangle.h
//  raytra_1
//

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <iostream>

#include "ray.h"
#include "surface.h"


//
//
//
class Triangle : public Surface {
    
    
public:
    Triangle (mypoint &pa, mypoint &pb, mypoint &pc);
    virtual ~Triangle () {}
    
    virtual bool intersect (const Ray &r,
                            double &t, 
                            mypoint &ipoint, 
                            myvector &normal) const;
    
private:
    
    mypoint     verts_[3];
    myvector    N_;
};


#endif
