#include<stdlib.h>

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
        vector3 M[3];
} face;
class objReader
{
public:
        objReader();
        void objLoadModel();
        void objLoadFile(char* filename);
        void objSaveFile(char* filename, int precision);
        char* m;

        size_t size;
        vector3* vertexArray;
        vector2* textureArray;
        vector3* normalArray;
        face* faceArray;
        char tex[50];

        int nVertex, nTexture, nNormal, nFace;
};