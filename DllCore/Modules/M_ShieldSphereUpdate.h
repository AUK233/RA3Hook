#pragma once
#include "../Base/CommonStructure.hpp"

typedef struct M_ShieldSphereUpdate_t : baseCommonModule_t {
	char pad[0x24];
	float ShieldCurrentDamage, ShieldMaxDamage;
	float ShieldDeltaRadius; // is RadiusMax - RadiusMin
	float ShieldDamageChange; // increased damage value per logic
	int ShieldCurrentRandom, ShieldMaxRandom; // maybe? see 0078A1F4
	char padEnd[0xC]; // pad size to 0x6C
}*pM_ShieldSphereUpdate;

