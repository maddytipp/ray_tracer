//
//  surface.h
//  raytra_1
//

#ifndef SURFACE_H_
#define SURFACE_H_


#include <iostream>

#include "ray.h"


//
// Abstract surface class: all geometry should derive from this
// class so that we can put them in lists, etc.
//
class Surface {
    
public:
    
    // default constructor give it the default material
    Surface () { materialIndex_ = 0; }
    virtual ~Surface() {}

    virtual bool intersect (const Ray &, double &, mypoint &, myvector &) const;
    
    void setMaterialIndex (int indx) { materialIndex_ = indx; }
    int materialIndex () { return materialIndex_; }
    
protected:
    
    // note: protected class members are visible to the derived
    // classes. i.e. Sphere, etc. can access the materialIndex_.
    
    // all surfaces have a material:
    int materialIndex_;

};



#endif
