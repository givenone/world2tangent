# world2tangent

convert world space normal to tangent space normal in obj file

by [서준원](givenone1@gmail.com)

reference :: 
- http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/#preparing-our-vbo
- https://www.cs.utexas.edu/~fussell/courses/cs384g-spring2016/lectures/normal_mapping_tangent.pdf

## Specification

- "n" vector는 face에 수직인 벡터로, p1-p0와 p2-p0의 cross (외적)임.
- tangent, bitangent vector를 계산한 후, Gram-Schmidt Orthogonalization, normalization 해서 orthonormal하도록 해줌.
- t,b,n의 방향 (오른손 좌표계)를 유지하기 위해, 오른손 좌표계의 방향이 아닐 경우 t의  방향을 reverse해줌.

## 사용 방법

```
$ make
$ ./tangent [INFILE.obj] [OUTFILE.obj]

ex) ./tangent 3DFace.obj 3DFace_test.obj
```

## 파일 구성

- ``obj.cpp & obj.hpp`` : wavefront obj file loader & writer. mtl file을 참조하거나 여러 개의 mtl file을 reference하는 경우 정상 작동 안할 수 있음.

- ```tangent.cpp & tangent.hpp``` : obj 파일을 읽은 후 world 좌표계의 normal을 tangent space의 normal로 바꿔주는 클래스. 
  - ```TangentFace``` 함수 : Tangent, Bitangent, Normal (u,v,n) base vector를 계산하고, transformation matrix를 생성. (world to tangent.) u v n base 벡터는 orthonormal이며, M^-1 = M^T를 만족.
  
  - ```Translate()``` 함수 : 모든 face에 대해 TangentFace함수를 실행. Transformation matrix를 triangle mesh의 세 point에 대해 dot product 실행. 이는 기저 변환으로 그 결과는 tangent space에서의 normal 벡터가 됨. 한 point가 여러 face를 이루는 경우 평균을 계산함. 