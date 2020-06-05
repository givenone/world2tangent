#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include "tangent.hpp"

using namespace std;

objReader::objReader()
{
        m = NULL;
        nVertex = 0;
        nFace = 0;
        nNormal = 0;
        nTexture = 0;
        size = 0;
}

void objReader::objLoadFile(char* filename)
{
        {
                size_t bytes = 0;
                FILE* file = fopen(filename, "rt");

                if (file != NULL) {
                        fseek(file, 0, SEEK_END);
                        size_t end = ftell(file);
                        fseek(file, 0, SEEK_SET);

                        m = (char*) malloc(end);
                        bytes = fread(m, sizeof(char), end, file);

                        fclose(file);
                }

                size = bytes;
        }
}

void objReader::objLoadModel()
{
        char* p = NULL, * e = NULL;
        bool start = true;
        mtl *texture = 0;
        p = m;
        e = m + size;

        // First, count numbers of v, vt, vn, f.
        while (p != e) {
                if (memcmp(p, "f",  1) == 0) nFace++;
                else if (memcmp(p, "vt",  2) == 0) nTexture++;
                else if (memcmp(p, "vn",  2) == 0) nNormal++;
                else if (memcmp(p, "v",  1) == 0) nVertex++;
                while (*p++ != (char) 0x0A);
        }

        vertexArray = (vector3*) malloc(sizeof(vector3) * nVertex);
        normalArray = (vector3*) malloc(sizeof(vector3) * nNormal);
        textureArray = (vector2*) malloc(sizeof(vector2) * nTexture);
        faceArray = (face*) malloc(sizeof(face) * nFace);

        p = m;
        int nV = 0, nF = 0, nT = 0, nN = 0;

        while (p != e) {
                if (memcmp(p, "v", 1) == 0) {
                        sscanf(p, "v %lf %lf %lf", &vertexArray[nV].x, &vertexArray[nV].y, &vertexArray[nV].z);
                        nV++;
                } else if (memcmp(p, "vt", 2) == 0) {
                        sscanf(p, "vt %lf %lf", &textureArray[nT].u, &textureArray[nT].v);
                        nT++;
                } else if (memcmp(p, "vn", 2) == 0) {
                        sscanf(p, "vn %lf %lf %lf", &normalArray[nN].x, &normalArray[nT].y, &normalArray[nN].z);
                        nN++;
                } else if (memcmp(p, "f", 1) == 0) {
                        sscanf(p, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                        &faceArray[nF].v[0].v, &faceArray[nF].v[0].vt, &faceArray[nF].v[0].vn,
                        &faceArray[nF].v[1].v, &faceArray[nF].v[1].vt, &faceArray[nF].v[1].vn,
                        &faceArray[nF].v[2].v, &faceArray[nF].v[2].vt, &faceArray[nF].v[2].vn);
                        faceArray[nF].texture = texture;
                        nF++;
                } else if (memcmp(p, "usemtl", 6) == 0) {
                        texture = (mtl*) malloc(sizeof(mtl));
                        sscanf(p, "usemtl %s", &(texture->material));
                }
                while (*p++ != (char) 0x0A);
        }
}


int main(int argc, char** argv)
{
        objReader objfile;
        objfile.objLoadFile(argv[1]);
        objfile.objLoadModel();

        cout<<"No. of vertices: "<<objfile.nVertex<<endl;
        cout<<"No. of faces: "<<objfile.nFace<<endl;
        cout<<"No. of Texture: "<<objfile.nTexture<<endl;
        cout<<"No. of Normal: "<<objfile.nNormal<<endl;

        return 0;
}