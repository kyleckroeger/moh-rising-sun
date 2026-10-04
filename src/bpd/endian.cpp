#include "BPD.h"
#include "Endian.h"

void EndianSwap(BPDHeader &value) {
    ChangeEndian(value.field00);
    ChangeEndian(value.field04);
    ChangeEndian(value.field08);
    ChangeEndian(value.lightPatternCount);
    ChangeEndian(value.pointCount);
    ChangeEndian(value.objectCount);
    ChangeEndian(value.classLayoutCount);
    ChangeEndian(value.lightVolumeCount);
    ChangeEndian(value.areaCount);
    ChangeEndian(value.bspCount);
    ChangeEndian(value.nodeCount);
    ChangeEndian(value.pathCount);
    ChangeEndian(value.animatedLightCount);
    ChangeEndian(value.stringCount);
    // All three calls occur in the original routine.
    ChangeEndian(value.field24);
    ChangeEndian(value.field24);
    ChangeEndian(value.field24);
    ChangeEndian(value.points);
    ChangeEndian(value.objects);
    ChangeEndian(value.classLayouts);
    ChangeEndian(value.lightVolumes);
    ChangeEndian(value.areas);
    ChangeEndian(value.bsp);
    ChangeEndian(value.nodes);
    ChangeEndian(value.paths);
    ChangeEndian(value.pointerTable);
    ChangeEndian(value.strings);
    ChangeEndian(value.lightPatterns);
    ChangeEndian(value.animatedLights);
}

void EndianSwap(xyzProperty_Struct &value) {
    ChangeEndian(value.field00);
    ChangeEndian(value.field04);
    ChangeEndian(value.field08);
    ChangeEndian(value.field0c);
    ChangeEndian(value.field10);
    ChangeEndian(value.field14);
    ChangeEndian(value.field18);
    ChangeEndian(value.field1c);
    ChangeEndian(value.field20);
    ChangeEndian(value.field24);
    ChangeEndian(value.field28);
}

void EndianSwap(MOH_core_Struct &value) {
    ChangeEndian(value.field2c);
    ChangeEndian(value.field2e);
    ChangeEndian(value.field30);
    ChangeEndian(value.field32);
    ChangeEndian(value.field36);
    ChangeEndian(value.field38);
    ChangeEndian(value.field3a);
    ChangeEndian(value.field3c);
    ChangeEndian(value.field40);
    ChangeEndian(value.field44);
    ChangeEndian(value.field48);
    ChangeEndian(value.field4c);
    ChangeEndian(value.field50);
    ChangeEndian(value.field54);
    ChangeEndian(value.field58);
    ChangeEndian(value.field5c);
    ChangeEndian(value.field60);
    ChangeEndian(value.field64);
    ChangeEndian(value.field68);
}

void EndianSwap(MOH_mechanic_Struct &value) {
    EndianSwap(value.core);
    ChangeEndian(value.field74);
}

void EndianSwap(MOH_enemy_Struct &value) {
    EndianSwap(value.core);
    ChangeEndian(value.field7e);
    ChangeEndian(value.field80);
    ChangeEndian(value.field82);
    ChangeEndian(value.field84);
    ChangeEndian(value.field86);
    ChangeEndian(value.field88);
    ChangeEndian(value.field8a);
    ChangeEndian(value.field8c);
    ChangeEndian(value.field8e);
    ChangeEndian(value.field90);
    ChangeEndian(value.field94);
    ChangeEndian(value.field98);
    ChangeEndian(value.field9c);
    ChangeEndian(value.fielda0);
    ChangeEndian(value.fielda4);
    ChangeEndian(value.fielda8);
    ChangeEndian(value.fieldac);
}

void EndianSwap(MOH_mechanicEnvMod_Struct &value) {
    EndianSwap(value.core);
    ChangeEndian(value.field74);
    ChangeEndian(value.field78);
}

void EndianSwap(MOH_animatedLight_Struct &value) {
    EndianSwap(value.core);
    ChangeEndian(value.field72);
    ChangeEndian(value.colours);
    ChangeEndian(value.times);
}

void EndianSwap(BPDLightVolume &value) {
    ChangeEndian(value.planeCount);
    ChangeEndian(value.lightCount);
    ChangeEndian(value.priority);
    ChangeEndian(value.planes);
    ChangeEndian(value.lights);
    ChangeEndian(value.maximum[0]);
    ChangeEndian(value.maximum[1]);
    ChangeEndian(value.maximum[2]);
    ChangeEndian(value.minimum[0]);
    ChangeEndian(value.minimum[1]);
    ChangeEndian(value.minimum[2]);
}

void EndianSwapList(unsigned long *values) {
    ChangeEndian(values[0]);
    for (int i = 1; i <= values[0]; ++i)
        ChangeEndian(values[i]);
}
