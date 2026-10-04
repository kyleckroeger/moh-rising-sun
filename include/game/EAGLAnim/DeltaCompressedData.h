// Adapted from the CC0 NFS Most Wanted reconstruction; see docs/Animation.md.
#ifndef MOH_EAGLANIM_DELTACOMPRESSEDDATA_H
#define MOH_EAGLANIM_DELTACOMPRESSEDDATA_H

namespace EAGLAnim {

// total size: 0x4
class DeltaCompressedData {
  public:
    // total size: 0xC
    struct DofInfo {
        float mQuantMin;   // offset 0x0, size 0x4
        float mQuantRange; // offset 0x4, size 0x4
        float mStartValue; // offset 0x8, size 0x4
    };

    unsigned short GetNumDofs() const { return mNumDofs; }

    void SetNumDofs(unsigned short numDofs) { mNumDofs = numDofs; }

    unsigned char GetNumQuantBits() const { return mNumQuantBits; }

    void SetNumQuantBits(unsigned char numBits) { mNumQuantBits = numBits; }

    const unsigned char *GetDataBlock() const { return reinterpret_cast<const unsigned char *>(&this[1]); }

    unsigned char *GetDataBlock() { return reinterpret_cast<unsigned char *>(&this[1]); }

    int GetDeltaDataOffset() const { return mNumDofs * sizeof(DofInfo); }

    void DecompressValues(int startDof, int numDofs, int prevFrame, int currFrame, const float *prevValues,
                          float *currValues) const;

    void DecompressValuesIndexed(int startDof, int numDofs, int prevFrame, int currFrame,
                                 const float *prevValues, float *currValues, int valuesPerIndex,
                                 unsigned short *indices, float scale) const;

    void DecompressValues(int valuesPerIndex, int prevFrame, int currFrame, const float *prevValues,
                          float *currValues, int numDofMask, const unsigned short *dofMask) const;

    void DecompressValuesIndexed(int prevFrame, int currFrame, const float *prevValues, float *currValues,
                                 int valuesPerIndex, unsigned short *indices, float scale, int numDofMask,
                                 const unsigned short *dofMask) const;

  private:
    unsigned short mNumDofs;     // offset 0x0, size 0x2
    unsigned char mNumQuantBits; // offset 0x2, size 0x1
    unsigned char unknown_3[1];  // offset 0x3, size 0x1
};

} // namespace EAGLAnim

#endif
