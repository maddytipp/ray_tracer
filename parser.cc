
#include <iostream>
#include <fstream>

#include "parser.h"

#include "material.h"
#include "basicmath.h"
#include "surface.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "lights.h"
#include "obj.h"

using namespace std;


// In this code, you can see how Sphere and Camera are set up - you
// will need to do similar for the other classes.
//
// You will also need to keep track of the last material defined, so
// that you can connect it to any surfaces that are subsequently defined.
//
void Parser::parse (
    const char *file,
    std::vector< Surface * > &surfaceVector,
    std::vector< Light * > &lightVector,
    std::vector< Material * > &materialVector,
    Camera &cam)
{
    
    ifstream in(file);
    char buffer[1025];
    string cmd;
    
    int num_cams = 0; // keep track of how many cameras we read in
    
    // this is the index of the current material - zero is the
    // default material.
    int currentMaterial = 0;
    
    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;
        
        Surface *thisSurface = 0;
        
        cmd="";
        
        istringstream iss(buffer);
        
        iss >> cmd;
        
        if (cmd[0]=='/' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        }
        else if (cmd=="w") {
            std::vector<float> verts;
            std::vector<int> tris;
            string file;
            
            iss >> file;
            
            read_wavefront_file(file.c_str(), tris, verts);
            
            for (int i = 0; i < tris.size () / 3; ++i) {
                mypoint pa(verts[3*tris[3*i]], verts[3*tris[3*i]+1], verts[3*tris[3*i]+2]);
                mypoint pb(verts[3*tris[3*i+1]], verts[3*tris[3*i+1]+1], verts[3*tris[3*i+1]+2]);
                mypoint pc(verts[3*tris[3*i+2]], verts[3*tris[3*i+2]+1], verts[3*tris[3*i+2]+2]);
                
                thisSurface = new Triangle(pa,pb,pc);
                surfaceVector.push_back (thisSurface);
                thisSurface->setMaterialIndex (currentMaterial);
            }
        }
        else if (cmd=="s") {
            // Sphere:
            
            // read in the parameters:
            mypoint pos; 
            double r;
            iss >> pos >> r;
            
            // construct it:
            // at the end of this loop, we put any newly-constructed surfaces
            // into the surfaceVector.
            thisSurface = new Sphere(pos, r);
        } 
        else if (cmd=="t") {
            // Triangle
            
            // read in the parameters:
            mypoint pa,pb,pc;
            iss >> pa >> pb >> pc;
            
            // construct it:
            thisSurface = new Triangle(pa,pb,pc);
        } 
        else if (cmd=="p") {
            // Plane
            
            // read in the parameters:
            myvector n; 
            double d;
            iss >> n >> d;
            
            // construct it:
            thisSurface = new Plane(n,d);
        } 
        else if (cmd=="c") {
            // Camera:
            ++num_cams; // keep track of how many we read in
 
            
            // read in the parameters:
            mypoint pos; myvector dir; 
            double d,iw,ih; 
            int pw,ph;
            iss >> pos >> dir >> d >> iw >> ih >> pw >> ph;
            
            // construct it:
            cam.init (pos,dir,d,iw,ih,pw,ph);
        } 
        else if (cmd=="l") {
            iss >> cmd;
            if (cmd=="p") {
                mypoint pos;
                myvector rgb;
                iss >> pos >> rgb;
                Light *plight = new PointLight (pos,rgb);
                lightVector.push_back (plight);
            } 
            else if (cmd=="d") {
                myvector dir,rgb;
                iss >> dir >> rgb;
            }
            else if (cmd=="a") {
                myvector rgb;
                iss >> rgb;
            }
            else {
                cout << "Parser error: invalid light at line " << line << endl;
            }
        } 
        else if (cmd=="m") {
            // Material:
            
            // read in the parameters:
            myvector diff,spec,refl; 
            double r;
            iss >> diff >> spec >> r >> refl;
            
            // construct it, and put it in the materials list:
            Material *thismat = new Material (diff, spec, refl, r);
            materialVector.push_back (thismat);
            
            // geometry defined after this has to get the correct index
            // into the material list:
            ++currentMaterial;
        } 
        else {
            std::cerr << "Parser error: invalid command at line " << line << endl;
        }
        
        // if we found a surface, add it to the list of surfaces:
        if (thisSurface && cmd!="w") {
            surfaceVector.push_back (thisSurface);
            thisSurface->setMaterialIndex (currentMaterial);
        }
    }
    
    in.close();
    
    // make sure we read in 1 camera, no more no less 8).
    if (num_cams != 1) {
        std::cerr << "scene file error: exactly ONE camera must be defined." << endl;
    }
}

