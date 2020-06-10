#ifndef OBJ_READER_H
#define OBJ_READER_H

#include <stdlib.h>

typedef struct {
        double x, y, z;
} vector3;

typedef struct {
        double u, v;
} vector2;

typedef struct {
        int v, vt, vn;
} vertex;

typedef struct {
        char material[50];
} mtl;

typedef struct {
        vertex v[3];
        mtl* texture;
} face;


class objReader
{
public:
        objReader();
        void objLoadModel(char* filename);
        void objLoadFile(char* filename);
        char* m;

        size_t size;
        vector3* vertexArray;
        vector2* textureArray;
        vector3* normalArray;
        face* faceArray;

        int nVertex, nTexture, nNormal, nFace;
};

#endif 