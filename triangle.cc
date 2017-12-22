//
//  triangle.cc
//  raytra_1
//


#include "triangle.h"



Triangle::Triangle (
    mypoint &pa, 
    mypoint &pb, 
    mypoint &pc)
{
    
    verts_[0] = pa;
    verts_[1] = pb;
    verts_[2] = pc;
    
    
    // the normal will get used a lot - just precompute it here for the
    // slight memory cost, so we never have to recompute it again:
    
    myvector v1 = pb - pa;
    myvector v2 = pc - pa;
    
    assert (v1.length () != 0.);
    assert (v2.length () != 0.);
   
    N_.crossProduct (v1, v2);
    
    assert (N_.length () != 0.0);
    
    N_.normalize();
    
}


bool Triangle::intersect (
    const Ray &r,
    double &t, 
    mypoint &ipoint, 
    myvector &normal
) const
{
    
    // set up as in Shirley et al p 78:
    // this is an OK way to do it, but definitely not the fastest. the
    // derivation is nice and clear, though.
    
    const mypoint &A = verts_[0];
    const mypoint &B = verts_[1];
    const mypoint &C = verts_[2];
    
    const myvector D = r.getDir();
    const mypoint E = r.getOrigin();
    
    double a = A[0] - B[0];
    double b = A[1] - B[1];
    double c = A[2] - B[2];
    
    double d = A[0] - C[0];
    double e = A[1] - C[1];
    double f = A[2] - C[2];
    
    double g = D[0];
    double h = D[1];
    double i = D[2];
    
    double j = A[0] - E[0];
    double k = A[1] - E[1];
    double l = A[2] - E[2];
    
    // reused composite values:
    double ei_hf = e*i-h*f;
    double gf_di = g*f-d*i;
    double dh_eg = d*h-e*g;
    double ak_jb = a*k-j*b;
    double jc_al = j*c-a*l;
    double bl_kc = b*l-k*c;
    
    // compute M:
    
    double M = a*ei_hf + b*gf_di + c*dh_eg;
    
    // compute t:
    t = -1. * (f*ak_jb+e*jc_al+d*bl_kc) / M;
    
    if (t <= 0.)
        return false;
    
    double alpha = (i*ak_jb+h*jc_al+g*bl_kc) / M;
    
    if ((alpha < 0.) or (alpha > 1.))
        return false;
    
    double beta = (j*ei_hf+k*gf_di+l*dh_eg) / M;
    
    if ((beta < 0.) or (beta > (1.-alpha)))
        return false;
    
    ipoint = E + D*t;
    
    normal = N_;
    return true;
}

