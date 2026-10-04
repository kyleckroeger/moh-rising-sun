// AI-assisted reconstruction; see docs/Animation.md and NOTICE.
#include "EAGLAnim/Skeleton.h"
namespace EAGLAnim {

static inline void SwapMirroredPose(float *a, float *b) {
    float temp;
    temp = b[4];
    b[4] = -a[4];
    a[4] = -temp;
    temp = b[5];
    b[5] = -a[5];
    a[5] = -temp;
    temp = b[6];
    b[6] = a[6];
    a[6] = temp;
    temp = b[7];
    b[7] = a[7];
    a[7] = temp;
    temp = b[8];
    b[8] = a[8];
    a[8] = temp;
    temp = b[9];
    b[9] = a[9];
    a[9] = temp;
    temp = b[10];
    b[10] = -a[10];
    a[10] = -temp;
}
static inline void MirrorSingle(float *p) {
    p[4] = -p[4];
    p[5] = -p[5];
    p[10] = -p[10];
}
static inline void CopyMirroredPose(float *a, float *b) {
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
    b[4] = -a[4];
    b[5] = -a[5];
    b[6] = a[6];
    b[7] = a[7];
    b[8] = a[8];
    b[9] = a[9];
    b[10] = -a[10];
}
void Skeleton::MirrorPose(float *pose, float *output, bool local, const BoneMask *mask) {
    int count = GetNumBones();
    int i;
    float *mirrored;
    if (mask) {
        if (pose == output) {
            for (i = 0; i < count; i++)
                if (mask->GetBone(i)) {
                    int other = GetBoneData()[i].mLeftRightIdx;
                    if (other > i) {
                        mirrored = &output[other * 12];
                        SwapMirroredPose(&output[i * 12], mirrored);
                    } else if (other == i) {
                        mirrored = &output[i * 12];
                        MirrorSingle(mirrored);
                    }
                }
        } else {
            for (i = 0; i < count; i++, pose += 12) {
                if (mask->GetBone(i)) {
                    mirrored = &output[GetBoneData()[i].mLeftRightIdx * 12];
                    CopyMirroredPose(pose, mirrored);
                }
            }
        }
    } else {
        if (pose == output) {
            for (i = 0; i < count; i++) {
                int other = GetBoneData()[i].mLeftRightIdx;
                if (other > i) {
                    mirrored = &output[other * 12];
                    SwapMirroredPose(&output[i * 12], mirrored);
                } else if (other == i) {
                    mirrored = &output[i * 12];
                    MirrorSingle(mirrored);
                }
            }
        } else {
            for (i = 0; i < count; i++) {
                mirrored = &output[GetBoneData()[i].mLeftRightIdx * 12];
                CopyMirroredPose(pose, mirrored);
                pose += 12;
            }
        }
    }
    if (!local) {
        mirrored = output;
        COORD4 q = *reinterpret_cast<COORD4 *>(mirrored + 4);
        mirrored[4] = q.z;
        mirrored[5] = q.w;
        mirrored[6] = -q.x;
        mirrored[7] = -q.y;
        mirrored[8] = -mirrored[8];
        mirrored[10] = -mirrored[10];
    }
}

static inline void BuildPose(float *pose, EAGL::Transform &output) {
    output.BuildSQT(pose[0], pose[1], pose[2], pose[4], pose[5], pose[6], pose[7], pose[8], pose[9],
                    pose[10]);
    output.m[0][0] *= pose[3];
    output.m[1][0] *= pose[3];
    output.m[2][0] *= pose[3];
}
void Skeleton::PoseSQTToLocal(float *pose, EAGL::Transform *output, BoneMask *mask) {
    int i;
    if (mask) {
        int count = GetNumBones();
        for (i = 0; i < count; i++) {
            if (mask->GetBone(i))
                BuildPose(pose, output[i]);
            pose += 12;
        }
    } else {
        int count = GetNumBones();
        for (i = 0; i < count; i++) {
            BuildPose(pose, output[i]);
            pose += 12;
        }
    }
}

void Skeleton::PoseLocalToGlobal(EAGL::Transform *local, EAGL::Transform *output, BoneMask *mask) {
    int count = GetNumBones();
    const BoneData *bones = GetBoneData();
    if (mask) {
        if (local == output) {
            for (int i = 0; i < count; i++)
                if (mask->GetBone(i)) {
                    int parent = bones[i].mParentIdx;
                    if (parent >= 0)
                        MatrixMultiply(&output[i], &output[parent], &local[i]);
                    else
                        output[i] = local[i];
                }
        } else {
            for (int i = 0; i < count; i++)
                if (mask->GetBone(i)) {
                    int parent = bones[i].mParentIdx;
                    if (parent >= 0)
                        MatrixMultiply(&output[i], &output[parent], &local[i]);
                    else
                        output[i] = local[i];
                }
        }
    } else {
        if (local == output) {
            for (int i = 0; i < count; i++) {
                int parent = bones[i].mParentIdx;
                if (parent >= 0)
                    MatrixMultiply(&output[i], &output[parent], &local[i]);
                else
                    output[i] = local[i];
            }
        } else {
            for (int i = 0; i < count; i++) {
                int parent = bones[i].mParentIdx;
                if (parent >= 0)
                    MatrixMultiply(&output[i], &output[parent], &local[i]);
                else
                    output[i] = local[i];
            }
        }
    }
}

void Skeleton::PoseSQTToGlobal(float *inputPose, EAGL::Transform *output, BoneMask *mask) {
    int i;
    if (mask) {
        int count = GetNumBones();
        float *pose = inputPose;
        for (i = 0; i < count; i++) {
            if (mask->GetBone(i)) {
                BuildPose(pose, output[i]);
                int parent = GetBoneData()[i].mParentIdx;
                if (parent >= 0)
                    MatrixMultiply(&output[i], &output[parent], &output[i]);
            }
            pose += 12;
        }
    } else {
        int count = GetNumBones();
        float *pose = inputPose;
        for (i = 0; i < count; i++) {
            BuildPose(pose, output[i]);
            int parent = GetBoneData()[i].mParentIdx;
            if (parent >= 0)
                MatrixMultiply(&output[i], &output[parent], &output[i]);
            pose += 12;
        }
    }
}

void Skeleton::GetStillPose(float *pose, const BoneMask *mask) const {
    int count = GetNumBones();
    int i;
    if (mask) {
        if (GetInvBoneScales()) {
            for (i = 0; i < count; i++) {
                if (mask->GetBone(i)) {
                    pose[0] = GetBoneData()[i].mS.x;
                    pose[1] = GetBoneData()[i].mS.y;
                    pose[2] = GetBoneData()[i].mS.z;
                    pose[3] = GetInvBoneScales()[i];
                    pose[4] = GetBoneData()[i].mQ.x;
                    pose[5] = GetBoneData()[i].mQ.y;
                    pose[6] = GetBoneData()[i].mQ.z;
                    pose[7] = GetBoneData()[i].mQ.w;
                    pose[8] = GetBoneData()[i].mT.x;
                    pose[9] = GetBoneData()[i].mT.y;
                    pose[10] = GetBoneData()[i].mT.z;
                    pose[11] = 1.0f;
                }
                pose += 12;
            }
        } else {
            for (i = 0; i < count; i++) {
                if (mask->GetBone(i)) {
                    pose[0] = GetBoneData()[i].mS.x;
                    pose[1] = GetBoneData()[i].mS.y;
                    pose[2] = GetBoneData()[i].mS.z;
                    pose[3] = 1.0f;
                    pose[4] = GetBoneData()[i].mQ.x;
                    pose[5] = GetBoneData()[i].mQ.y;
                    pose[6] = GetBoneData()[i].mQ.z;
                    pose[7] = GetBoneData()[i].mQ.w;
                    pose[8] = GetBoneData()[i].mT.x;
                    pose[9] = GetBoneData()[i].mT.y;
                    pose[10] = GetBoneData()[i].mT.z;
                    pose[11] = 1.0f;
                }
                pose += 12;
            }
        }
    } else {
        if (GetInvBoneScales()) {
            for (i = 0; i < count; i++) {
                pose[0] = GetBoneData()[i].mS.x;
                pose[1] = GetBoneData()[i].mS.y;
                pose[2] = GetBoneData()[i].mS.z;
                pose[3] = GetInvBoneScales()[i];
                pose[4] = GetBoneData()[i].mQ.x;
                pose[5] = GetBoneData()[i].mQ.y;
                pose[6] = GetBoneData()[i].mQ.z;
                pose[7] = GetBoneData()[i].mQ.w;
                pose[8] = GetBoneData()[i].mT.x;
                pose[9] = GetBoneData()[i].mT.y;
                pose[10] = GetBoneData()[i].mT.z;
                pose[11] = 1.0f;
                pose += 12;
            }
        } else {
            for (i = 0; i < count; i++) {
                pose[0] = GetBoneData()[i].mS.x;
                pose[1] = GetBoneData()[i].mS.y;
                pose[2] = GetBoneData()[i].mS.z;
                pose[3] = 1.0f;
                pose[4] = GetBoneData()[i].mQ.x;
                pose[5] = GetBoneData()[i].mQ.y;
                pose[6] = GetBoneData()[i].mQ.z;
                pose[7] = GetBoneData()[i].mQ.w;
                pose[8] = GetBoneData()[i].mT.x;
                pose[9] = GetBoneData()[i].mT.y;
                pose[10] = GetBoneData()[i].mT.z;
                pose[11] = 1.0f;
                pose += 12;
            }
        }
    }
}

void Skeleton::PoseGlobalToSkin(EAGL::Transform *pose, EAGL::Transform *output, BoneMask *mask) {
    int count = GetNumBones();
    if (mask) {
        for (int i = 0; i < count; i++) {
            if (mask->GetBone(i)) {
                MatrixMultiply(&output[i], &pose[i], &GetBoneData()[i].mInvBaseMatrix);
                output[i].Transpose();
            }
        }
    } else {
        for (int i = 0; i < count; i++) {
            MatrixMultiply(&output[i], &pose[i], &GetBoneData()[i].mInvBaseMatrix);
            output[i].Transpose();
        }
    }
}
} // namespace EAGLAnim
