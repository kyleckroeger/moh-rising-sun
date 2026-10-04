// Reconstructed from the pinned target; see docs/Matrix.md.
#include "game/CMatrix.h"

void CMatrix::GetSlot(unsigned int slot) {
    if (slot == 0) {
        row[0].x = 1.0f;
        row[0].y = 0.0f;
        row[0].z = 0.0f;
        row[0].w = 0.0f;
        row[1].x = 0.0f;
        row[1].y = 1.0f;
        row[1].z = 0.0f;
        row[1].w = 0.0f;
        row[2].x = 0.0f;
        row[2].y = 0.0f;
        row[2].z = 1.0f;
        row[2].w = 0.0f;
        row[3].x = 0.0f;
        row[3].y = 0.0f;
        row[3].z = 0.0f;
        row[3].w = 1.0f;
    }
}
