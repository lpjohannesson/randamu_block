#ifndef __RANDOM_H__
#define __RANDOM_H__


#include <cstdint>
#include "Json.h"
#include "new_rng/pcg_basic.h"

uint32_t msvc_rand(uint32_t boundary);
void msvc_srand(uint32_t seed);
long msvc_rand2(void);
void msvc_srand2(unsigned long seed);

// seeded RNG states

extern pcg32_random_t seed_primary, seed_chests, seed_npcs;
extern pcg32_random_t seed_primary_clone, seed_chests_clone, seed_npcs_clone; //those are used for saving

void to_json(json& j, const pcg32_random_t& i);
void from_json(const json& j, pcg32_random_t& i);

#endif // __RANDOM_H__