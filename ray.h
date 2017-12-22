//
//  ray.h
//  raytra_1
//

#ifndef RAY_H_
#define RAY_H_

#include "basicmath.h"


class Ray {
    
public:
    
    Ray () {}
    Ray (const mypoint &p, const myvector &d) {
        p_ = p;
        d_ = d;
    }
    
    //
    // Get and set functions, for the direction and origin:
    void setDir (const myvector dir)   { d_ = dir; }
    void setOrigin (const mypoint pnt) { p_ = pnt; }
    
    myvector getDir () const { return d_; }
    mypoint  getOrigin () const { return p_; }
    
    void printRay () {
        std::cout << "point is " << p_[0] << " " << p_[1] << " "
        << p_[2] << "   dir is " << d_[0] << " " << d_[1] << " " << d_[2] 
        << std::endl;
    }
    
private:
    
    myvector    d_; // direction of ray
    mypoint     p_; // origin of ray
    
};

#endif
