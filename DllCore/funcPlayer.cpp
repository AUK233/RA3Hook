#include "pch.h"
#include <format>

#include "funcPlayer.h"

extern uintptr_t pPlayerTechStore[];

uintptr_t __fastcall LoadPlayerTechStoreCPP(uintptr_t ptr, uintptr_t pptr)
{
	uintptr_t pTemplate = *(uintptr_t*)ptr;
	UINT32 TemplateID = *(UINT32*)(pTemplate + 4);

	UINT32 sideIndex;
	switch (TemplateID)
	{
	case 3267362061U:
		// PlayerTechStore_Allied1
		sideIndex = 4;
		break;
	case 2350054027U:
		// PlayerTechStore_Allied2
		sideIndex = 5;
		break;
	case 1022500346U:
		// PlayerTechStore_Allied3
		sideIndex = 6;
		break;
	case 1606882289U:
		// PlayerTechStore_Soviet1
		sideIndex = 7;
		break;
	case 55352523U:
		// PlayerTechStore_Soviet2
		sideIndex = 8;
		break;
	case 3838998804U:
		// PlayerTechStore_Soviet3
		sideIndex = 9;
		break;
	case 3587428384U:
		// PlayerTechStore_Japan1
		sideIndex = 10;
		break;
	case 3494380189U:
		// PlayerTechStore_Japan2
		sideIndex = 11;
		break;
	case 1268198533U:
		// PlayerTechStore_Japan3
		sideIndex = 12;
		break;
	default:
		sideIndex = *(UINT32*)(ptr + 4);
		if (sideIndex > 3)
		{
			return pptr;
		}
		break;
	}
	// Write Pointer
	pPlayerTechStore[sideIndex] = pptr;
	return pptr;
}

extern uintptr_t _Ret_PlayerTechStoreL;

__declspec(naked) void __fastcall LoadPlayerTechStoreASM()
{
	__asm {
		mov edx, eax
		call LoadPlayerTechStoreCPP
		mov ecx, [esp + 4]
		mov [eax], ecx
		mov ecx, [ecx + 4]
		jmp _Ret_PlayerTechStoreL
	}
}

UINT32 __fastcall ReadPlayerTechStoreCPP(uintptr_t ptr)
{
	UINT32 sideID = *(UINT32*)(ptr + 8);

	switch (sideID)
	{
	case 2293878703U:
		// 0Allies1
		return 4;
	case 2382707485U:
		// 2Allies9
		return 5;
	case 2389174180U:
		// 3Allies
		return 6;
	case 3111316061U:
		// 1Soviet4
		return 7;
	case 3154887896U:
		// 0Soviet5
		return 8;
	case 3237873227U:
		// 3Soviet7
		return 9;
	case 1758239350U:
		// 3Japan2
		return 10;
	case 1895103435U:
		// 1Japan2
		return 11;
	case 1959656780U:
		// Japan3
		return 12;
	default:
		uintptr_t pSide = *(uintptr_t*)ptr;
		UINT32 sideIndex = *(UINT32*)(pSide + 0x30);
		return sideIndex;
	}
}
