#include "pch.h"
#include <format>

#include "utiliy.h"
#include "utiliy_game.h"
#include "funcMLaser.h"

int __fastcall SweepingLaserStateGetID(uintptr_t ptr)
{
	int laserID = *(int*)(*(uintptr_t*)(ptr + 4) + 8);
	
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
			case 0:
				return SweepLaserT_toTarget0dot1;
			case 1:
				return SweepLaserT_baseRotation;
			case 100:
				return SweepLaserT_formSourceToTarget;
			case 101:
				return SweepLaserT_formSourceToDistance;
			default:
				return SweepLaserT_Enhanced;
			}
		}
		else {
			return SweepLaserT_Enhanced;
		}
	}
	else {
		return SweepLaserT_Original;
	}
}

void __fastcall SweepingLaserStateCPP1(uintptr_t ptr, int cfg)
{
	// ptr+0xB0 is laser time (int)
	//atan2(y, x);
	switch (cfg)
	{
	case SweepLaserT_baseRotation: {
		*(char*)(ptr + 0xBD) = 1;
		*(int*)(ptr + 0xA0) = 0;
		// sweep start position
		*(float*)(ptr + 0xA4) = *(float*)(ptr + 0x3C);
		*(float*)(ptr + 0xA8) = *(float*)(ptr + 0x40);
		*(float*)(ptr + 0xAC) = *(float*)(ptr + 0x44);

		// sweep rotation angle
		float Radius = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
		float Orientation;
		if (Radius < -0.1f) {
			Orientation = getRadomFloatValue(0.0f, 6.28f);
			Radius *= -1.0f;
		} else {
			Orientation = *(float*)(*(uintptr_t*)(ptr + 8) + 0x44);
			if (rand() % 2) {
				Radius *= -1.0f;
			}
		}
		// Read EndOffset's z as angle.
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
		if (posOffsetPtr)
		{
			Orientation += *(float*)(posOffsetPtr + 8);
		}

		float ofsx = cos(Orientation) * Radius;
		float ofsy = sin(Orientation) * Radius;
		*(float*)(ptr + 0x98) = ofsx;
		*(float*)(ptr + 0x9C) = ofsy;
		break;
	}
	case SweepLaserT_toTarget0dot1: {
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
		float ofsx = DeltaX * Radius;
		float ofsy = DeltaY * Radius;
		float ofsz = 0.0f;
		*(char*)(ptr + 0xBC) = 0;

		// sweep start position
		*(float*)(ptr + 0xA4) = opx - ofsx;
		*(float*)(ptr + 0xA8) = opy - ofsy;
		*(float*)(ptr + 0xAC) = opz - ofsz;
		// sweep end 
		ofsx *= 0.1f;
		ofsy *= 0.1f;
		*(float*)(ptr + 0x98) = opx + ofsx;
		*(float*)(ptr + 0x9C) = opy + ofsy;
		*(float*)(ptr + 0xA0) = opz + ofsz;
		break;
	}
	case SweepLaserT_formSourceToTarget: {
		//ep1 + 3A7F86
		*(char*)(ptr + 0xBC) = 0;
		// Self Position
		float sfx = *(float*)(ptr + 0x30);
		float sfy = *(float*)(ptr + 0x34);
		float sfz = *(float*)(ptr + 0x38);
		// Target Position
		//float opx = *(float*)(ptr + 0x3C);
		//float opy = *(float*)(ptr + 0x40);
		//float opz = *(float*)(ptr + 0x44);
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
	case SweepLaserT_formSourceToDistance: {
		*(char*)(ptr + 0xBC) = 0;
		// Self Position
		float sfx = *(float*)(ptr + 0x30);
		float sfy = *(float*)(ptr + 0x34);
		float sfz = *(float*)(ptr + 0x38);
		// Target Position
		float opx = *(float*)(ptr + 0x3C);
		float opy = *(float*)(ptr + 0x40);
		float opz = *(float*)(ptr + 0x44);
		// Read EndOffset's z as minimum distance.
		float minDistance = 0.0f;
		uintptr_t posOffsetPtr = *(uintptr_t*)(*(uintptr_t*)(ptr + 4) + 0x24);
		if (posOffsetPtr) {
			minDistance = *(float*)(posOffsetPtr + 8);
		}
		// Read target distance
		float Length = *(float*)(*(uintptr_t*)(ptr + 4) + 0x2C);
		// The difference between the target and its own position.
		float spx = opx - sfx;
		float spy = opy - sfy;
		float ofsL = (sqrtf((spx * spx) + (spy * spy))) - Length;
		if (ofsL > 0.0f) {
			minDistance += ofsL;
			Length += ofsL;
		}
		//
		float Orientation = atan2f(spy, spx);
		float cosAngle = cos(Orientation);
		float sinAngle = sin(Orientation);
		// sweep end position
		*(float*)(ptr + 0x98) = sfx + (cosAngle * Length);
		*(float*)(ptr + 0x9C) = sfy + (sinAngle * Length);
		*(float*)(ptr + 0xA0) = opz;
		// sweep start position
		*(float*)(ptr + 0xA4) = sfx + (cosAngle * minDistance);
		*(float*)(ptr + 0xA8) = sfy + (sinAngle * minDistance);
		*(float*)(ptr + 0xAC) = sfz;
		break;
	}
	case SweepLaserT_Enhanced: {
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




void __fastcall SweepingLaserActivateCPP(uintptr_t ptr, SweepingLaserPos* inPtr, float inXMM3)
{
	// 006E3743
	float sweepAngle = inXMM3 * 6.28f;
	float cosAngle = cos(sweepAngle);
	float sinAngle = sin(sweepAngle);
	
	float sfx = *(float*)(ptr + 0xA4);
	float sfy = *(float*)(ptr + 0xA8);
	float ofsx = *(float*)(ptr + 0x98);
	float ofsy = *(float*)(ptr + 0x9C);

	inPtr->x = sfx + (cosAngle * ofsx) - (sinAngle * ofsy);
	inPtr->y = sfy + (cosAngle * ofsy) + (sinAngle * ofsx);
	inPtr->z = *(float*)(ptr + 0xAC);
}

uintptr_t _Ret_SweepLaserActivate;

__declspec(naked) void __fastcall SweepingLaserActivateASM(uintptr_t ptr)
{
	__asm {
		cmp byte ptr[ecx + 0xBD], 1
		je NewFuncBlock
		subss xmm3, [esp]
		jmp _Ret_SweepLaserActivate
	NewFuncBlock:
		mov eax, [esp]
		mov edx, [esp+8]
		push eax
		call SweepingLaserActivateCPP
		pop ecx
		ret 4
	}
}
