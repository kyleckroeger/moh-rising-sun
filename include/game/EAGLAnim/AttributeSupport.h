// Attributed reference names; target CompoundChannel::UseFPS establishes the
// two-byte ID and by-value copies, and the returned pointer's high-byte access.
// See docs/Animation.md and src/eagl_anim/NOTICE.
#ifndef MOH_ATTRIBUTE_SUPPORT_H
#define MOH_ATTRIBUTE_SUPPORT_H
namespace EAGLAnim {
class AttributeId {
  public:
    enum ID { ID_FPS = 1 };
    AttributeId(ID id) : mId(id) {}
    AttributeId(const AttributeId &other) : mId(other.mId) {}
    ~AttributeId() {}

  private:
    unsigned short mId;
};
class AttributeBlock {
  public:
    bool GetAttribute(AttributeId, void *&) const;
    bool GetAttribute(AttributeId id, unsigned char &result) const {
        void *r;
        if (GetAttribute(id, r)) {
            result = static_cast<unsigned char>((reinterpret_cast<unsigned int>(r) & 0xFF000000) >> 24);
            return true;
        }
        return false;
    }
};
} // namespace EAGLAnim
#endif
