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


        vertexArray = (vector3*) malloc(sizeof(vector3) * (nVertex + 1));
        normalArray = (vector3*) malloc(sizeof(vector3) * (nNormal + 1));
        textureArray = (vector2*) malloc(sizeof(vector2) * (nTexture + 1));
        faceArray = (face*) malloc(sizeof(face) * nFace);

        p = m;
        int nV = 1, nT = 1, nN = 1, nF = 0;
        while (p != e) {
                if (memcmp(p, "v ", 2) == 0) {
                        sscanf(p, "v %lf %lf %lf", &vertexArray[nV].x, &vertexArray[nV].y, &vertexArray[nV].z);
                        nV++;
                } else if (memcmp(p, "vt", 2) == 0) {
                        sscanf(p, "vt %lf %lf", &textureArray[nT].u, &textureArray[nT].v);
                        nT++;
                } else if (memcmp(p, "vn", 2) == 0) {
                        sscanf(p, "vn %lf %lf %lf", &normalArray[nN].x, &normalArray[nN].y, &normalArray[nN].z);
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
                        sscanf(p, "%s", tex);
                }
                while (*p++ != (char) 0x0A);
        }
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
            printf("1\n");
            fprintf(f, "v %lf %lf %lf\n",vertexArray[v].x, vertexArray[v].y, vertexArray[v].z );
//                fprintf(f, formv, vertexArray[v].x, vertexArray[v].y, vertexArray[v].z);
                                
        }
        for(int v=1; v<=nTexture; v++)
        {
                                            printf(formv,vertexArray[v].x, vertexArray[v].y, vertexArray[v].z);
                fprintf(f, formt, textureArray[v].u, textureArray[v].v);
        }        
        for(int v=1; v<=nNormal; v++)
        {
                fprintf(f, formv, normalArray[v].x, normalArray[v].y, normalArray[v].z);
        }

        // usemtl if mtl file exist.
        if(tex[0] != '\0') fprintf(f, "%s\n", tex);

        for(int nF=0; nF<nFace; nF++)
        {
                fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                &faceArray[nF].v[0].v, &faceArray[nF].v[0].vt, &faceArray[nF].v[0].vn,
                &faceArray[nF].v[1].v, &faceArray[nF].v[1].vt, &faceArray[nF].v[1].vn,
                &faceArray[nF].v[2].v, &faceArray[nF].v[2].vt, &faceArray[nF].v[2].vn);   
        } 

        fclose(f);

}