#include "pch.h"
#include <format>

void __fastcall SweepingLaserStateCPP1(int ptr)
{
	// Target Position
	float opx = *(float*)(ptr + 0x3C);
	float opy = *(float*)(ptr + 0x40);
	float opz = *(float*)(ptr + 0x44);
	// The difference between the target and its own position.
	float spx = opx - *(float*)(ptr + 0x30);
	float spy = opy - *(float*)(ptr + 0x34);
	// Read EndOffset's z as radian.
	int posOffsetPtr = *(int*)(*(int*)(ptr + 4) + 0x24);
	if (posOffsetPtr)
	{
		float ofsRadian = *(float*)(posOffsetPtr + 8);
		if (ofsRadian) {
			// rotate coordinates
			float dx = cos(ofsRadian) * spx - sin(ofsRadian) * spy;
			float dy = cos(ofsRadian) * spy + sin(ofsRadian) * spx;
			spx = dx;
			spy = dy;
			// restore height
			//opz -= ofsRadian;
			//opz += 0.01f;
			//*(float*)(ptr + 0x44) = opz;
		}
	}

	float spz = opz - *(float*)(ptr + 0x38);

	float fr = (spz * spz) + (spy * spy) + (spx * spx);
	float DeltaX;
	float DeltaY;
	float DeltaZ;
	if (fr <= 0.0001f)
	{
		DeltaX = 1.0f;
		DeltaY = 0.0f;
		DeltaZ = 0.0f;
	}
	else
	{
		float scale;
		if (fr == 0.0f)
		{
			scale = 0.0f;
		}
		else
		{
			scale = 1.0f / sqrt(fr);
		}
		DeltaX = spx * scale;
		DeltaY = spy * scale;
		DeltaZ = spz * scale;
	}

	float Radius = *(float*)(*(int*)(ptr + 4) + 0x2C);
	float ofsx;
	float ofsy;
	float ofsz;
	if (Radius < -0.1f) {
		// vertical sweep, one way
		*(char*)(ptr + 0xBC) = 0;
		ofsx = DeltaX * Radius;
		ofsy = DeltaY * Radius;
		ofsz = 0.0f;
	}
	else {
		//float ofsz = ((DeltaX * 0.0f) - (DeltaY * 0.0f)) * Radius;
		// horizontal sweep, both directions
		ofsx = DeltaY * Radius;
		ofsy = -DeltaX * Radius;
		ofsz = 0.0f;
	}

	// no, that's not what uprising looks like
	/*
	UINT32 moduleTag = *(UINT32*)(*(int*)(ptr + 4) + 4);
	switch (moduleTag)
	{
	case 1529473134U:
	case 3164596609U: {
		// JapanFortressShipAirborneSeigeCannonWeapon_SweepingWeapon
		// JapanFortressShipAirborneSeigeCannonWeapon_SweepingWeapon_Veteran

		// sweep end position
		*(float*)(ptr + 0x98) = opx;
		*(float*)(ptr + 0x9C) = opy;
		*(float*)(ptr + 0xA0) = opz;
		// sweep start position
		*(float*)(ptr + 0xA4) = opx + ofsx + ofsx;
		*(float*)(ptr + 0xA8) = opy + ofsy + ofsy;
		*(float*)(ptr + 0xAC) = opz;
		break;
	}
	default: {
		// sweep end position
		*(float*)(ptr + 0x98) = opx - ofsx;
		*(float*)(ptr + 0x9C) = opy - ofsy;
		*(float*)(ptr + 0xA0) = opz - ofsz;
		// sweep start position
		*(float*)(ptr + 0xA4) = opx + ofsx;
		*(float*)(ptr + 0xA8) = opy + ofsy;
		*(float*)(ptr + 0xAC) = opz + ofsz;
		break;
	}
	}
	*/

	// sweep end position
	*(float*)(ptr + 0x98) = opx - ofsx;
	*(float*)(ptr + 0x9C) = opy - ofsy;
	*(float*)(ptr + 0xA0) = opz - ofsz;
	// sweep start position
	*(float*)(ptr + 0xA4) = opx + ofsx;
	*(float*)(ptr + 0xA8) = opy + ofsy;
	*(float*)(ptr + 0xAC) = opz + ofsz;
}

__declspec(naked) void __fastcall SweepingLaserStateASM1(int ptr)
{
	__asm {
		mov ecx, esi
		call SweepingLaserStateCPP1
		pop edi
		pop esi
		add esp, 0x18
		ret 0x8
	}
}
