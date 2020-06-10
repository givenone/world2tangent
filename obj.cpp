#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "obj.hpp"

objReader::objReader()
{
        m = NULL;
        nVertex = 0;
        nFace = 0;
        nNormal = 0;
        nTexture = 0;
        size = 0;
        *tex = '\0';
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
        int nV = 1, nT = 1, nN = 1, nF = 0;
        printf("?\n");

        FILE* file = fopen(filename, "r");

        while (1) {
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
                        fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", 
                        &faceArray[nF].v[0].v, &faceArray[nF].v[0].vt, &faceArray[nF].v[0].vn,
                        &faceArray[nF].v[1].v, &faceArray[nF].v[1].vt, &faceArray[nF].v[1].vn,
                        &faceArray[nF].v[2].v, &faceArray[nF].v[2].vt, &faceArray[nF].v[2].vn);
                        //faceArray[nF].texture = texture;
                        nF++;
                } else if (strcmp(lineHeader, "usemtl") == 0) {
                        texture = (mtl*) malloc(sizeof(mtl));
                        fscanf(file, "%s", (texture->material));
                }
        }
        printf("%d %d %d %d\n", nV, nT, nN, nF);
}

void objReader::objSaveFile(char* filename, int precision)
{

        FILE *f = fopen(filename, "wt");
        char formv[256];
        char formt[256];
        char formn[256];
        sprintf(formv, "v %%.%df %%.%df %%.%df\n",  precision, precision, precision);
        sprintf(formt, "vt %%.%df %%.%df\n",        precision, precision);
        sprintf(formn, "vn %%.%df %%.%df %%.%df\n", precision, precision, precision);
        printf("%d %d %d %d\n", nVertex, nTexture, nNormal, nFace);
        
        if (f == NULL) 
        { 
            printf("Could not open file"); 
            return; 
        } 
  
        for(int v=1; v<=nVertex; v++)
        {   
                fprintf(f, formv, vertexArray[v].x, vertexArray[v].y, vertexArray[v].z);
                                
        }
        for(int v=1; v<=nTexture; v++)
        {
                fprintf(f, formt, textureArray[v].u, textureArray[v].v);
        }        
        for(int v=1; v<=nNormal; v++)
        {
                fprintf(f, formn, normalArray[v].x, normalArray[v].y, normalArray[v].z);
        }

        // usemtl if mtl file exist.
        if(tex[0] != '\0') fprintf(f, "%s\n", tex);

        for(int nF=0; nF<nFace; nF++)
        {
                fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                faceArray[nF].v[0].v, faceArray[nF].v[0].vt, faceArray[nF].v[0].vn,
                faceArray[nF].v[1].v, faceArray[nF].v[1].vt, faceArray[nF].v[1].vn,
                faceArray[nF].v[2].v, faceArray[nF].v[2].vt, faceArray[nF].v[2].vn);   
        } 

        fclose(f);

}