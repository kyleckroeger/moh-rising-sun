#include "BPD.h"
#include "CMatrix.h"
int IsPointInLightVolume(const CVector3 &point, const BPDLightVolume *volume) {
    const float *position = Vector3Components(point);
    const PropPlane4 *plane = volume->planes;
    for (int i = 0; i < volume->planeCount; ++i, ++plane) {
        float distance = plane->x * position[0] + plane->y * position[1]
                       + plane->z * position[2] + plane->d;
        if (!(distance >= 0.0f))
            return 0;
    }
    return 1;
}
