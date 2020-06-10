#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>

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
                        sscanf(p, "usemtl %s", texture->material);
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
                faceArray[nF].v[0].v, faceArray[nF].v[0].vt, faceArray[nF].v[0].vn,
                faceArray[nF].v[1].v, faceArray[nF].v[1].vt, faceArray[nF].v[1].vn,
                faceArray[nF].v[2].v, faceArray[nF].v[2].vt, faceArray[nF].v[2].vn);   
        } 

        fclose(f);

}

class world2Tangent
{
public:
        world2Tangent();
        void Translate(); // main initial function
        void Cross(const vector3, const vector3, vector3&);
        void Normalize(vector3&);
        void Transpose(vector3[]); // Inverse in orthonormal matrix.
        void TangentFace(face&);

        objReader *obj;
};

world2Tangent::world2Tangent(){};

void world2Tangent::Translate()
{
    vector3 *tangentNormalArray = (vector3*) calloc(sizeof(vector3), obj->nNormal);
    int *normalCount = (int*) calloc(sizeof(int), obj->nNormal);

    for(int i=0; i < obj->nFace; i++)
    {
        face f = obj->faceArray[i];
        TangentFace(f); // M is calculated
        for(int ver = 0; ver < 3; ver ++)
        {
            int normal_idx = f.v[ver].vn;
            vector3 nxyz = obj->normalArray[normal_idx];

            // [nx, ny, nz] dot M( ([T, B, N]^-1)^T)
            tangentNormalArray[normal_idx].x = nxyz.x * f.M[0].x + nxyz.y * f.M[1].x + nxyz.z * f.M[2].x;
            tangentNormalArray[normal_idx].y = nxyz.x * f.M[0].y + nxyz.y * f.M[1].y + nxyz.z * f.M[2].y;
            tangentNormalArray[normal_idx].z = nxyz.x * f.M[0].z + nxyz.y * f.M[1].z + nxyz.z * f.M[2].z;
            normalCount[normal_idx]++;
        }
    }

    for(int i=0; i< obj->nNormal; i++)
    {
        Normalize(tangentNormalArray[i]);
        obj->normalArray[i] = tangentNormalArray[i];
    }
}

void world2Tangent::Cross(const vector3 v1, const vector3 v2, vector3& result)
{
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v2.x * v1.z - v1.x * v2.z;
    result.z = v1.x * v2.y - v2.x * v1.y;
}

void world2Tangent::Normalize(vector3& v)
{
    double norm = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    v.x /= norm;
    v.y /= norm;
    v.z /= norm;

}

// in place transpose of matrix
void world2Tangent::Transpose(vector3* M)
{
    double M01 = M[1].x, M02 = M[2].x, M12 = M[2].y;
    M[1].x = M[0].y;
    M[2].x = M[0].z;
    M[2].y = M[1].z;
    M[0].y = M01;
    M[0].z = M02;
    M[1].z = M12;
}

void world2Tangent::TangentFace(face& f)
{
    // f.vertex[0] :: p0, f.vertex[1] :: p1, f.vertex[2] :: p2
    // idx :: f.v[i].v / vt / vn
    // p0 = vertexArray[idx_t], t0 = textureArray[idx_t]
    // T, B, N -> f.M
    // point vector :: x, y, z
    vector3 p0 = obj->vertexArray[f.v[0].v];
    vector3 p1 = obj->vertexArray[f.v[1].v];
    vector3 p2 = obj->vertexArray[f.v[2].v];

    // texture coordinate :: u, v
    vector2 t0 = obj->textureArray[f.v[0].vt];
    vector2 t1 = obj->textureArray[f.v[1].vt];
    vector2 t2 = obj->textureArray[f.v[2].vt];

    double v1x = p1.x - p0.x;
    double v1y = p1.y - p0.y;
    double v1z = p1.z - p0.z;
 
    double v2x = p2.x - p0.x;
    double v2y = p2.y - p0.y;
    double v2z = p2.z - p0.z;
 
    double u1x = t1.u - t0.u;
    double u1y = t1.v - t0.v;
 
    double u2x = t2.u - t0.u;
    double u2y = t2.v - t0.v;
 
    double det = u1x * u2y - u2x * u1y;
 
    // M :: t (M[0]), b (M[1]), n (M[2]) of surface (3 row vector)

    // Tangent :: (v1 * uv2y - v2 * uv1y) / det
    // Bitangent :: (v2 * uv1x - v1 * uv2x) / det

    // Tangent and Bitangent is not perpendicular !!
    f.M[0].x = (v1x * u2y - v2x * u1y) / det;
    f.M[0].y = (v1y * u2y - v2y * u1y) / det;
    f.M[0].z = (v1z * u2y - v2z * u1y) / det;

    f.M[1].x = (v2x * u1x - v1x * u2x) / det;
    f.M[1].y = (v2y * u1x - v1y * u2x) / det;
    f.M[1].z = (v2z * u1x - v1z * u2x) / det;
 
    // n : t cross b
    Cross(f.M[0], f.M[1], f.M[2]);

    // Normalize
    // normalize each vector
    Normalize(f.M[0]);
    Normalize(f.M[1]);
    Normalize(f.M[2]);

    // make b perpendicular.
    double dot = f.M[0].x * f.M[1].x + f.M[0].y * f.M[1].y + f.M[0].z * f.M[1].z; // t, b
    // b - t * dot (t, b)
    f.M[1].x -= dot * f.M[0].x;
    f.M[1].y -= dot * f.M[0].y;
    f.M[1].z -= dot * f.M[0].z;
    Normalize(f.M[1]);


    // Headness of n (cross(n,t) dot b >= 0)
    vector3 n_t;
    Cross(f.M[2], f.M[0], n_t);
    double dot_n_t = n_t.x * f.M[1].x + n_t.y * f.M[1].y + n_t.z * f.M[1].z;
    if(dot_n_t < 0.0f)
    {
            f.M[0].x = -f.M[0].x; f.M[0].y = -f.M[0].y; f.M[0].z = -f.M[0].z;
    }

    Transpose(f.M);

}

int main(int argc, char** argv)
{
        objReader objfile;
        objfile.objLoadFile(argv[1]);
        objfile.objLoadModel();
        printf("ok\n");
        world2Tangent tangent;
        tangent.obj = &objfile;
        tangent.Translate();

        objfile.objSaveFile(argv[2], 6);
        return 0;
}