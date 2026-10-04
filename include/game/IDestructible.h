// AI-assisted scoped reconstruction; see docs/Observers.md.
#ifndef MOH_IDESTRUCTIBLE_H
#define MOH_IDESTRUCTIBLE_H
#pragma interface
class IDestructible {
  public:
    virtual void MarkForDestruction(int);
    virtual ~IDestructible() {}
    virtual void Destroy();
};

#endif
