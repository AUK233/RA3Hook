#include "pch.h"
#include <format>
#include "funcMLaser.h"

int __fastcall SweepingLaserStateGetID(uintptr_t ptr)
{
	int laserID = *(int*)(*(int*)(ptr + 4) + 8);
	
	if (laserID > 499)
	{
		if (laserID > 999) {
			/*
			UINT32 moduleTag = *(UINT32*)(*(uintptr_t*)(ptr + 4) + 4);
			switch (moduleTag)
			{
			case 1783858841U:
			case 2674295398U:
				// ModuleTag_SweepingLaserState1FormSource
				// ModuleTag_SweepingLaserState2FormSource
				return 2;
			default:
				return 1;
			}*/
			int laserIndex = (laserID - 1000) / 20;
			switch (laserIndex)
			{
			case 100:
				return 2;
			default:
				return 1;
			}
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}

	// no, that's not what uprising looks like
	/*
	UINT32 moduleTag = *(UINT32*)(*(uintptr_t*)(ptr + 4) + 4);
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
}

void __fastcall SweepingLaserStateCPP1(uintptr_t ptr, int cfg)
{
	//atan2(y, x);
	switch (cfg)
	{
	case 2: {
		//ep1 + 3A7F86
		*(char*)(ptr + 0xBC) = 0;
		// Self Position
		float sfx = *(float*)(ptr + 0x30);
		float sfy = *(float*)(ptr + 0x34);
		float sfz = *(float*)(ptr + 0x38);
		// Target Position
		float opx = *(float*)(ptr + 0x3C);
		float opy = *(float*)(ptr + 0x40);
		float opz = *(float*)(ptr + 0x44);
		//
		float Orientation = *(float*)(*(uintptr_t*)(ptr + 8) + 0x44);
		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
		float ofsx = cos(Orientation) * Radius;
		float ofsy = sin(Orientation) * Radius;
		float ofsz = 0.0f;
		// Read EndOffset's z as radian.
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
		float sweepAngle = *(float*)(posOffsetPtr + 8);
		float cosAngle = cos(sweepAngle);
		float sinAngle = sin(sweepAngle);
		// sweep end position
		*(float*)(ptr + 0x98) = sfx + (cosAngle * ofsx) - (sinAngle * ofsy);
		*(float*)(ptr + 0x9C) = sfy + (cosAngle * ofsy) + (sinAngle * ofsx);
		*(float*)(ptr + 0xA0) = sfz + ofsz;
		// sweep start position
		*(float*)(ptr + 0xA4) = sfx;
		*(float*)(ptr + 0xA8) = sfy;
		*(float*)(ptr + 0xAC) = sfz;
		break;
	}
	case 1: {
		// Target Position
		float opx = *(float*)(ptr + 0x3C);
		float opy = *(float*)(ptr + 0x40);
		float opz = *(float*)(ptr + 0x44);
		// The difference between the target and its own position.
		float spx = opx - *(float*)(ptr + 0x30);
		float spy = opy - *(float*)(ptr + 0x34);
		// Read EndOffset's z as radian.
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
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

		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
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
	default: {
		// Target Position
		float opx = *(float*)(ptr + 0x3C);
		float opy = *(float*)(ptr + 0x40);
		float opz = *(float*)(ptr + 0x44);
		// The difference between the target and its own position.
		float spx = opx - *(float*)(ptr + 0x30);
		float spy = opy - *(float*)(ptr + 0x34);
		float spz = opz - *(float*)(ptr + 0x38);
		// *(float*)(ptr + 0x48) self position x?

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

		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
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
}

__declspec(naked) void __fastcall SweepingLaserStateASM1(uintptr_t ptr)
{
	__asm {
		mov ecx, esi
		call SweepingLaserStateGetID
		mov edx, eax
		mov ecx, esi
		call SweepingLaserStateCPP1
		pop edi
		pop esi
		add esp, 0x18
		ret 0x8
	}
}

extern uintptr_t _Ret_ActivateLaser;

__declspec(naked) void __fastcall ActivateLaserNuggetASM(uintptr_t ptr)
{
	__asm {
			je retFunc
			mov eax, [ebp + 0x10]
			// check laser id
			cmp dword ptr [eax+0x2C], 3000
			jl Original
			// check laser activation
			cmp byte ptr [esi+0x94], 0
			jne retFunc
		Original:
			jmp _Ret_ActivateLaser
		retFunc:
			pop edi
			pop esi
			pop ebp
			pop ebx
			add esp, 0x10
			ret 0x10
	}
}
