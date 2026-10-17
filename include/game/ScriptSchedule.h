// AI-assisted scoped reconstruction from GR8E69; see docs/Script.md.
#ifndef GAME_SCRIPT_SCHEDULE_H
#define GAME_SCRIPT_SCHEDULE_H
struct BSObject;
struct BSScheduleRegistrationRecord_struct;

// Method-only view of the script scheduler; storage and size remain original.
class BSSchedule {
  public:
    void Unregister(BSScheduleRegistrationRecord_struct *);
    void Unregister(BSObject *);
};

// Original schedulers; these declarations do not allocate them.
extern BSSchedule g_TargetSearchSchedule;
extern BSSchedule g_TargetCheckSchedule;
extern BSSchedule g_DistanceToTargetSchedule;
extern BSSchedule g_CorpseSearchSchedule;

#endif
