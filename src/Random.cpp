#include "Random.h"
#include "Json.h"
#include <cstring>
#define PHI 0x9e3779b9

// A replication of MSVC's rand algorithm
static unsigned long next2 = 1;

pcg32_random_t seed_primary, seed_chests, seed_npcs;
pcg32_random_t seed_primary_clone, seed_chests_clone, seed_npcs_clone;

void to_json(json& j, const pcg32_random_t& i){
	j = json{ {"increment", i.inc}, {"state", i.state}};
}

void from_json(const json& j, pcg32_random_t& i) {
    j.at("increment").get_to(i.inc);

	j.at("state").get_to(i.state);
}

void msvc_srand(uint32_t seed)
{
	pcg32_srandom(seed ^ (intptr_t)&strcat, 22);
}

uint32_t msvc_rand(uint32_t boundary)
{
	return pcg32_boundedrand(boundary);
}

uint32_t msvc_rand(uint64_t boundary)
{
	return pcg32_boundedrand64(boundary);
}

void msvc_srand2(unsigned long seed)
{
    long safe2 = 1;
    seed %= 0x7FFFFFFF;
    if (seed)
        safe2 = seed;
    next2= safe2;
}

long msvc_rand2()
{
    long result = 48271 * next2 % 0x7FFFFFFF;
    next2 = result;
    return result;
}
