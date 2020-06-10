#include "tangent.hpp"

using namespace std;

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

        world2Tangent tangent;
        tangent.obj = &objfile;
        tangent.Translate();

        objfile.objSaveFile(argv[2], 6);
        return 0;
}