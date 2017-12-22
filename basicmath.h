//
//  basicmath.h
//  raytra_1
//

#ifndef BASICMATH_H_
#define BASICMATH_H_

#include <cassert>
#include <iostream>
#include <cmath>

using std::istream;
using std::ostream;


//
//
//
class myvector {
    
public:
    
    myvector () {}
        
    myvector (double i, double j, double k) {
        ijk_[0] = i; ijk_[1] = j; ijk_[2] = k;
    }
    
    myvector (const myvector &v) {
        ijk_[0] = v[0]; ijk_[1] = v[1]; ijk_[2] = v[2];
    }
    
    void set (double i, double j, double k) {
        ijk_[0] = i; ijk_[1] = j; ijk_[2] = k;
    }

    double operator[] (const int i) const;
    
    void normalize ();
    
    const myvector multiply(const myvector v) const {
        myvector vec;
        vec.ijk_[0] = ijk_[0] * v.ijk_[0];
        vec.ijk_[1] = ijk_[1] * v.ijk_[1];
        vec.ijk_[2] = ijk_[2] * v.ijk_[2];
        
        return myvector(vec.ijk_[0], vec.ijk_[1], vec.ijk_[2]);
    }
    
    // return scaled version of this vector:
    myvector operator* (const double scale) const {
        return myvector (scale * ijk_[0],scale * ijk_[1],scale * ijk_[2]); 
    }
    
    myvector invert () const { 
        // call the above scaling operator to invert:
        return operator* (-1.0); 
    }
    
    // allow vector addition via +=
    myvector &operator+= (const myvector &v) {
        ijk_[0] = ijk_[0] + v.ijk_[0];
        ijk_[1] = ijk_[1] + v.ijk_[1];
        ijk_[2] = ijk_[2] + v.ijk_[2];
        
        return *this;
    }
   
    // returns the scalar value that is the dotproduct
    // between this and v:
    double dotProduct (const myvector &v) const {
        return (ijk_[0] * v.ijk_[0] + ijk_[1] * v.ijk_[1] + ijk_[2] * v.ijk_[2]);
    }
    

    double length () const {
        return sqrt (ijk_[0] * ijk_[0] + ijk_[1] * ijk_[1] + ijk_[2] * ijk_[2]);
    }
    

    // sets this to be the cross product of vectors va and vb:
    // i.e. va X vb
    void crossProduct (const myvector &va, const myvector &vb) {
        ijk_[0] = va.ijk_[1] * vb.ijk_[2] - va.ijk_[2] * vb.ijk_[1];
        ijk_[1] = va.ijk_[2] * vb.ijk_[0] - va.ijk_[0] * vb.ijk_[2];
        ijk_[2] = va.ijk_[0] * vb.ijk_[1] - va.ijk_[1] * vb.ijk_[0];
    }
    
    // these two functions are used to allow us to read in
    // and print out (or save to a file). they are used by
    // the parser code.
    friend istream &operator>>(istream &is, myvector &v) {
        return is>>v.ijk_[0]>>v.ijk_[1]>>v.ijk_[2];
    }
    
    friend ostream &operator<<(ostream &os, myvector &v) {
        return os<<"<"<<v.ijk_[0]<<","<<v.ijk_[1]<<","<<v.ijk_[2]<<">";
    }
    
    myvector &operator- (const myvector &v) {
        ijk_[0] = ijk_[0] - v.ijk_[0];
        ijk_[1] = ijk_[1] - v.ijk_[1];
        ijk_[2] = ijk_[2] - v.ijk_[2];
        return *this;
    }
    
    const double *get_ijk() {
        return ijk_;
    }
    
    
private:
    
    double ijk_[3];
};




//
//
//
class mypoint {
    
public:
    
    mypoint () {} // default constructor does nothing, not even inits.
    mypoint (double x, double y, double z) {
        xyz_[0] = x; xyz_[1] = y; xyz_[2] = z;
    }
    mypoint (const mypoint &p) {
        xyz_[0] = p.xyz_[0]; xyz_[1] = p.xyz_[1]; xyz_[2] = p.xyz_[2];
    }
    
    void set (double x, double y, double z) {
        xyz_[0] = x; xyz_[1] = y; xyz_[2] = z;
    }

    double operator[] (const int i) const;
    
    // subtracting one point from another returns a vector
    myvector operator- (const mypoint &p) const {
        return myvector (
                         xyz_[0] - p.xyz_[0], 
                         xyz_[1] - p.xyz_[1], 
                         xyz_[2] - p.xyz_[2]);
    }
    
    mypoint operator+ (const myvector &v) const {
        return mypoint (xyz_[0] + v[0], xyz_[1] + v[1], xyz_[2] + v[2]); 
    }
    
    // these two functions are used to allow us to read in
    // and print out (or save to a file). they are used by
    // the parser code.
    friend  istream &operator>>(istream &is, mypoint &p) {
        return is >> p.xyz_[0] >> p.xyz_[1] >> p.xyz_[2];
    }
    
    friend ostream &operator<<(ostream &os, mypoint &p) {
        return os<<"<"<<p.xyz_[0]<<","<<p.xyz_[1]<<","<<p.xyz_[2]<<">";
    }
    
private:
    
    double xyz_[3];
    
};



inline double myvector::operator[]
  (const int i) const
{
    assert(i >= 0 && i < 3);
    return ijk_[i];
}


inline void myvector::normalize ()
{

    assert (! (ijk_[0] == 0.0 && ijk_[1] == 0.0 && ijk_[2] == 0.0));
        
    double len = length ();
    
    ijk_[0] /= len;
    ijk_[1] /= len;
    ijk_[2] /= len;
    
}



inline double mypoint::operator[]
(const int i) const
{
    assert(i >= 0 && i < 3);
    return xyz_[i];
}

#endif
