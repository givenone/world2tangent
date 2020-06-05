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
        double M[3][3];
} matrix3;

typedef struct {
        int v, vt, vn;
} vertex;

typedef struct {
        char material[50];
} mtl;

typedef struct {
        vertex v[3];
        mtl* texture;
        matrix3 T;
} face;


class objReader
{
public:
        objReader();
        void objLoadModel();
        void objLoadFile(char* filename);
        char* m;

        size_t size;
        vector3* vertexArray;
        vector2* textureArray;
        vector3* normalArray;
        face* faceArray;

        int nVertex, nTexture, nNormal, nFace;
};

class world2Tangent
{
public:
        world2Tangent();
        void Translate(); // main initial function
        void Cross(const vector3, const vector3, vector3&);
        void Normalize(vector3&);
        void Transpose(matrix3&); // maybe not needed in translation
        void TangentFace(const face, matrix3&);
        void objLoadFile(char* filename);

        objReader *obj;
};

#endif