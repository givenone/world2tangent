
#include <obj.hpp>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <math.h>
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
