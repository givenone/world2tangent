#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include "test.hpp"

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
                        //printf("%d %d",file, end);
                        fseek(file, 0, SEEK_SET);

                        m = (char*) malloc(end);
                        bytes = fread(m, sizeof(char), end, file);

                        fclose(file);
                }
                size = bytes;
        }
}

void objReader::objLoadModel(char* filename)
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

        printf("done?\n");

        vertexArray = (vector3*) malloc(sizeof(vector3) * nVertex);
        normalArray = (vector3*) malloc(sizeof(vector3) * nNormal);
        textureArray = (vector2*) malloc(sizeof(vector2) * nTexture);
        faceArray = (face*) malloc(sizeof(face) * nFace);

        free(m);
        int nV = 0, nF = 0, nT = 0, nN = 0;
        printf("?\n");

        FILE* file = fopen(filename, "r");

        while (p != e) {
                char lineHeader[128];
                int res = fscanf(file, "%s", lineHeader);
                if (res == EOF)
			break; // EOF = End Of File. Quit the loop.


                if (strcmp("v", lineHeader) == 0) {
                        fscanf(file, "%lf %lf %lf", &vertexArray[nV].x, &vertexArray[nV].y, &vertexArray[nV].z);
                        nV++;
                } else if (strcmp("vt", lineHeader) == 0) {
                        fscanf(file, "%lf %lf", &textureArray[nT].u, &textureArray[nT].v);
                        nT++;
                } else if (strcmp("vn", lineHeader) == 0) {
                        fscanf(file, "%lf %lf %lf", &normalArray[nN].x, &normalArray[nN].y, &normalArray[nN].z);
                        nN++;
                } else if (strcmp("f", lineHeader) == 0) {
                        fscanf(file, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                        &faceArray[nF].v[0].v, &faceArray[nF].v[0].vt, &faceArray[nF].v[0].vn,
                        &faceArray[nF].v[1].v, &faceArray[nF].v[1].vt, &faceArray[nF].v[1].vn,
                        &faceArray[nF].v[2].v, &faceArray[nF].v[2].vt, &faceArray[nF].v[2].vn);
                        //faceArray[nF].texture = texture;
                        nF++;
                } else if (strcmp(lineHeader, "usemtl") == 0) {
                        texture = (mtl*) malloc(sizeof(mtl));
                        fscanf(file, "%s", (texture->material));
                }
                while (*p++ != (char) 0x0A);
        }
        printf("?\n");
}


int main(int argc, char** argv)
{
        objReader objfile;
        objfile.objLoadFile(argv[1]);
        objfile.objLoadModel(argv[1]);

        cout<<"No. of vertices: "<<objfile.nVertex<<endl;
        cout<<"No. of faces: "<<objfile.nFace<<endl;
        cout<<"No. of Texture: "<<objfile.nTexture<<endl;
        cout<<"No. of Normal: "<<objfile.nNormal<<endl;

        return 0;
} 