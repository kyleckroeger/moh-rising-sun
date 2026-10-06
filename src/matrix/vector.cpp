// AI-assisted reconstruction; see docs/Matrix.md, "CVector3 layout".
#include <math.h>
#include "CVector3.h"

// Turn toward target until the cosine of the angle between them reaches cosLimit.
// Nearly opposite vectors take the target directly. target is scaled in place.
void CVector3::Constrain(CVector3 &target, float cosLimit) {
    const float epsilon = 0.0001f;
    float cosAngle = x * target.x + y * target.y + z * target.z;
    if (!(cosAngle + 1.0f >= epsilon)) {
        *this = target;
    } else if (!(cosAngle >= cosLimit)) {
        float limit = acosf(cosLimit);
        float angle = acosf(cosAngle);
        float sinAngle = sinf(angle);
        float targetWeight = sinf(angle - limit) / sinAngle;
        float selfWeight = sinf(limit) / sinAngle;
        *this *= selfWeight;
        target *= targetWeight;
        *this += target;
    }
}

void CVector3::RotateAboutX(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    float ny = y * c - z * s;
    z = y * s + z * c;
    y = ny;
}

void CVector3::RotateAboutZ(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    float nx = x * c - y * s;
    y = x * s + y * c;
    x = nx;
}

float CVector3::Distance(const CVector3 &o) const {
    CVector3 d(x - o.x, y - o.y, z - o.z);
    return sqrtf(d.LengthSquared());
}

float CVector3::DistanceXY(const CVector3 &o) const {
    CVector3 d;
    d.Set(x - o.x, y - o.y, 0.0f);
    return sqrtf(d.LengthSquaredXY());
}

float CVector3::DistanceSquared(const CVector3 &o) const {
    CVector3 d(x - o.x, y - o.y, z - o.z);
    return d.LengthSquared();
}

float CVector3::DistanceSquaredXY(const CVector3 &o) const {
    CVector3 d;
    d.Set(x - o.x, y - o.y, 0.0f);
    return d.LengthSquaredXY();
}
