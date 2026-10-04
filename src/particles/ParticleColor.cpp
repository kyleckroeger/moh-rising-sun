#include "ParticleRecipe.h"
// Scoped packing view, not a complete historical CColor definition.
union ColorPrefix {
    unsigned int word;
    struct {
        unsigned char r, g, b;
        // -1 supplies the observed all-ones alpha byte.
        signed char a;
    } rgba;
    ColorPrefix(unsigned char r, unsigned char g, unsigned char b) {
        rgba.r = r;
        rgba.g = g;
        rgba.b = b;
        rgba.a = -1;
    }
};
void CParticleSystem::GetParticleColor(CColor &a, CColor &b, CColor &c, CColor &d) const {
    *reinterpret_cast<unsigned int *>(&a) = ColorPrefix(definition->contents->entries[22]->value.integer,
                                                        definition->contents->entries[23]->value.integer,
                                                        definition->contents->entries[24]->value.integer)
                                                .word;
    *reinterpret_cast<unsigned int *>(&b) = ColorPrefix(definition->contents->entries[39]->value.integer,
                                                        definition->contents->entries[40]->value.integer,
                                                        definition->contents->entries[41]->value.integer)
                                                .word;
    *reinterpret_cast<unsigned int *>(&c) = ColorPrefix(definition->contents->entries[26]->value.integer,
                                                        definition->contents->entries[27]->value.integer,
                                                        definition->contents->entries[28]->value.integer)
                                                .word;
    *reinterpret_cast<unsigned int *>(&d) = ColorPrefix(definition->contents->entries[42]->value.integer,
                                                        definition->contents->entries[43]->value.integer,
                                                        definition->contents->entries[44]->value.integer)
                                                .word;
}
