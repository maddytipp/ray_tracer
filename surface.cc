//
//  surface.cc
//  raytra_1
//


#include "surface.h"

bool Surface::intersect (
                         const Ray &, 
                         double &, 
                         mypoint &, 
                         myvector & 
    ) const
{
    std::cout << "error: surface::intersect should not be called - it's virtual!"
    << std::endl;
    
    return false;
}
