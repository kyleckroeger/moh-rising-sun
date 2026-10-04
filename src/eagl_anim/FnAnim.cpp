// AI-assisted reconstruction from pinned GR8E69; see docs/Animation.md and NOTICE.
#include "EAGLAnim/AnimCore.h"
namespace EAGLAnim {
unsigned short FnAnim::GetTargetCheckSum() const { return 0; }
void FnAnim::UseFPS(bool) {}
void FnAnim::Eval(float, float, float *) {}
bool FnAnim::GetLength(float &) const { return false; }
bool FnAnim::FindMatchTime(const MatchPhaseInput &, float &) const { return false; }
bool FnAnim::EvalSQT(float, float *, const BoneMask *) { return false; }
bool FnAnim::EvalPhase(float, PhaseValue &) { return false; }
bool FnAnim::EvalVel2D(float, float *) { return false; }
bool FnAnim::EvalEvent(float, float, EventHandler **, void *) { return false; }
bool FnAnim::EvalWeights(float, float *) { return false; }
bool FnAnim::EvalState(float, State *) { return false; }
bool FnAnim::EvalPose(float, const PosePaletteBank *, float *) { return false; }
bool FnAnim::FindTime(const StateTest &, float, float &) { return false; }
const PhaseChan *FnAnim::GetPhaseChan() { return 0; }
bool FnAnim::GetAnimatedBones(BoneMask *, int &, int &, int &) { return false; }
bool FnAnim::GetAnimatedBonesAux(BoneMask *, int &, int &, int &) { return false; }
const AttributeBlock *FnAnim::GetAttributes() const { return 0; }
} // namespace EAGLAnim
