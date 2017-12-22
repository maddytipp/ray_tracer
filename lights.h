//
//  lights.h
//  raytra_1
//

#ifndef LIGHTS_H_
#define LIGHTS_H_


#include "basicmath.h"

class Light {
    
public:
    
    Light () { spd_.set (0., 0., 0.); }
    virtual ~Light () {};
        
    virtual myvector spd () {return spd_; }  // return the spectral power distribution
    // i.e. the "brightness" for each color channel
    
    virtual mypoint position () const { return mypoint (0., 0., 0.);}

protected:
    
    myvector spd_;

    
};


class PointLight : public Light {
    
public:
    
    PointLight () {}
    PointLight (const mypoint &pos, const myvector &spd) {
        position_ = pos;
        spd_ = spd;
    }
    
    mypoint position () const { return position_; }

private:
    
    mypoint position_;

};


#endif
