//
//  main.cc
//  raytra_1
//
//

#include <iostream>
#include <vector>

#include "surface.h"
#include "material.h"
#include "lights.h"
#include "camera.h"
#include "parser.h"


int main (int argc, const char * argv[])
{
    
    if (argc != 3) {
        // error condition:
        std::cout << "usage: raytra scenefile outputimage" << std::endl;
        return -1;
    }
    
    std::vector< Surface * > surfaces;
    std::vector< Material * > materials;
    std::vector< Light * > lights;
    
    // make a default material, which objects get if they are defined
    // before any material is. the default material is material 0.
    Material *default_material = new Material;
    materials.push_back (default_material);
    
    Camera cam;
    
    Parser parser;
    
    parser.parse (argv[1], surfaces, lights, materials, cam);
    
    assert (surfaces.size () != 0); // mae sure there are some surfaces
    
    std:: cout << "read in: " << surfaces.size () << " surfaces, " <<
    lights.size () << " lights, " << materials.size () - 1 << " materials."
    << std::endl;
    
    // if there is no light defined in the scene file, just make one
    // colocated with the camera, and white:
    if (lights.size () == 0) {
        myvector lightcolor (1., 1., 1.);
        PointLight *defaultlight = new PointLight (cam.position (), lightcolor);
        lights.push_back(defaultlight);
        std::cout << "note: no lights! using default light." << std::endl;
    }
    
    cam.renderScene (surfaces, lights, materials);
    cam.writeImage (argv[2]);
    
    for (Surface * s : surfaces)
        delete s;
    
    for (Light * l : lights)
        delete l;
    
    for (Material * m : materials)
        delete m;
    
}









