#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <vector>

#include "camera.h"

using namespace std;

class Surface;
class Material;
class Light;
class AmbientLight;

//
// Basic parser for the scene files:
//
class Parser {
public:
    virtual void parse (
            const char *file,
            std::vector< Surface * > &surfaceVector,
            std::vector< Light * > &lightVector,
            std::vector< Material * > &materialVector,
            Camera &cam);
    
    void read_wavefront_file (
                              const char *file,
                              std::vector< int > &tris,
                              std::vector< float > &verts)
    {
        
        // clear out the tris and verts vectors:
        tris.clear ();
        verts.clear ();
        
        ifstream in(file);
        char buffer[1025];
        string cmd;
        
        
        for (int line=1; in.good(); line++) {
            in.getline(buffer,1024);
            buffer[in.gcount()]=0;
            
            cmd="";
            
            istringstream iss (buffer);
            
            iss >> cmd;
            
            if (cmd[0]=='#' or cmd.empty()) {
                // ignore comments or blank lines
                continue;
            }
            else if (cmd=="v") {
                // got a vertex:
                
                // read in the parameters:
                double pa, pb, pc;
                iss >> pa >> pb >> pc;
                
                verts.push_back (pa);
                verts.push_back (pb);
                verts.push_back (pc);
            }
            else if (cmd=="f") {
                // got a face (triangle)
                
                // read in the parameters:
                int i, j, k;
                iss >> i >> j >> k;
                
                // vertex numbers in OBJ files start with 1, but in C++ array
                // indices start with 0, so we're shifting everything down by
                // 1
                tris.push_back (i-1);
                tris.push_back (j-1);
                tris.push_back (k-1);
            }
            else {
                std::cerr << "Parser error: invalid command at line " << line << std::endl;
            }
            
        }
        
        in.close();
        
        //   std::cout << "found this many tris, verts: " << tris.size () / 3.0 << "  " << verts.size () / 3.0 << std::endl;
    }
    
};

#endif
