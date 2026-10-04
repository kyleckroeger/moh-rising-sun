#ifndef GAME_CMATRIX_H
#define GAME_CMATRIX_H

// Only the first three float components are established for CVector3.
// Its complete layout and size remain unknown; see docs/Matrix.md.
class CVector3;

// Descriptive storage names, not recovered historical member/type names.
struct CMatrixRow {
    float x, y, z, w;
};

class CMatrix {
public:
    CMatrixRow row[4];

    static int s_ClassInit;
    static void InitClass();

    CMatrix& operator=(const CMatrix& other);
    void GetSlot(unsigned int slot);
    void BuildRot(const CVector3& axis, float angle);
    void Perspective(float x, float y, float near, float far);
    void Orthographic(float x, float y, float near, float far);
    void Inverse(const CMatrix& in);
    void Multiply(const CMatrix& a, const CMatrix& b);
    void BuildRotX(float angle);
    void BuildRotY(float angle);
    void BuildRotZ(float angle);
    void BuildScale(float scale);
    void BuildScale(const CVector3& scale);
    void BuildTrans(const CVector3& position);
    void SetRight(const CVector3& right);
    void SetFront(const CVector3& front);
    void SetUp(const CVector3& up);
    void SetPos(const CVector3& position);
    void PreTranslate(const CVector3& offset);
    void Translate(const CVector3& offset);
};

typedef char CMatrixStorageSizeCheck[sizeof(CMatrix) == 64 ? 1 : -1];

// A view of the observed float prefix, without defining the rest of CVector3.
inline const float* Vector3Components(const CVector3& vector) {
    return reinterpret_cast<const float*>(&vector);
}

#endif
