#ifndef GAME_EAGL_TRANSFORM_H
#define GAME_EAGL_TRANSFORM_H
// Names are from symbols; member names and header organization are descriptive.
// Matrix and coordinate storage is supported by whole-object copies and point transforms.
struct COORD3 { float x, y, z; };
struct COORD4 { float x, y, z, w; };
struct MATRIX4 { float m[4][4]; };
void MultMatrix(const MATRIX4*, const MATRIX4*, MATRIX4*);
extern "C" void MEM_copy(void*, const void*, unsigned int);
namespace EAGL {
class Transform {
public:
    float m[4][4];
    void TransformPoint(const COORD3&, COORD3&) const;
    void TransformPoint(const COORD4&, COORD4&) const;
    void Transpose();
    void BuildIdentity();
    void BuildQT(float, float, float, float, float, float, float);
    void BuildSQT(float, float, float, float, float, float, float, float, float, float);
    void BuildQuatTrans(const COORD4*, const COORD4*);
    void PrependQuatTrans(const COORD4*, const COORD4*);
    void BuildScale(float, float, float, float);
    void AppendTranslate(float, float, float);
    void PostMult(const Transform&);
    void PreMult(const Transform&);
    void AppendMatrix(const MATRIX4*);
    void PrependMatrix(const MATRIX4*);
    void BuildMatrix(const MATRIX4*);
    void OrthoInverse();
    static void Transpose(const Transform&, Transform&);
};
}
#endif
