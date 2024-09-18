#include "pch.h"
#include <format>

#include "funcOther.h"
#include "commonStruct.h"

extern inputSettingINFO inputSetting;
extern uintptr_t _Ret_BloomOpen;
extern uintptr_t _F_CallKillGameObject;
extern uintptr_t _Ret_BehaviorUpdate_TiberiumCrystal;

__declspec(naked) void __fastcall SetNoBloomASM()
{
	__asm {
			test eax, eax
			je ofs5FB9E8
			cmp inputSetting.CheckBloom, 0
			jne ofs5FB9EC
			cmp dword ptr[eax + 0x2EC], 1
			jle ofs5FB9EC
		ofs5FB9E8:
			mov bl, 1
			jmp _Ret_BloomOpen
		ofs5FB9EC:
			xor bl, bl
			jmp _Ret_BloomOpen
	}
}

__declspec(naked) void __fastcall KillTibCrystalWhenEmptyASM1()
{
	__asm {
		// check if the mine is empty
		cmp dword ptr[edi + 0x30], 0
		jg ofs422717
		push 0
		push 8 // SUICIDED
		push 6
		mov ecx, [edi + 8]
		call _F_CallKillGameObject
		add esp, 4
		pop edi
		pop esi
		pop ebp
		mov eax, -2
		pop ebx
		add esp, 0x18
		ret
	ofs422717:
		mov edx, [esi + 0x28]
		fild dword ptr[esi + 0x28]
		ret
	}
}

__declspec(naked) void __fastcall KillTibCrystalWhenEmptyASM2()
{
	__asm {
		// check if the mine is empty
		cmp dword ptr[edi + 0x30], 0
		jne ofs42278D
		push 0
		push 8 // SUICIDED
		push 6
		mov ecx, [edi+8]
		call _F_CallKillGameObject
	ofs42278D:
		add [esi + 0x2C], ebp
		test ebp, ebp
		ret
	}
}

__declspec(naked) void __fastcall BehaviorUpdate_TiberiumCrystal()
{
	__asm {
		// check if the mine is empty
		// here we need to reduce by 16 bytes
		cmp dword ptr[esi + 0x30 - 0x10], 0
		jne addr705A26
		push 0
		push 8 // SUICIDED
		push 6
		mov ecx, ebp // ebp is GameObject pointer in 00705A17
		call _F_CallKillGameObject
		// then return instead of jumping back to original
		pop esi
		mov eax, 1
		pop ebp
		add esp, 0x10
		ret
	addr705A26:
		cmp byte ptr[esi + 0x3C], 0
		push ebx
		jmp _Ret_BehaviorUpdate_TiberiumCrystal
	}
}

__declspec(naked) int32_t __fastcall BuildList_GetNewBuildCountASM(BuildList_Producer* pBuilder, int32_t shiftOn)
{
	/*
	__asm {
		mov eax, [esp]
		mov [esp], esi
		push eax
		jmp BuildList_GetNewBuildCountCPP
	}*/
	__asm {
		// check if is JapanWallPiece
		cmp dword ptr[esi+8], 4173660217
		je JapanWallCount
		mov eax, edx
		shl eax, 5 // to 32
		inc eax
		ret
	JapanWallCount:
		xor eax, eax
		// increase production only if the list is empty.
		cmp [ecx+0x10], eax
		je AddJapanWallCount
		ret
	AddJapanWallCount:
		inc eax
		ret
	}
}

int32_t __fastcall BuildList_GetNewBuildCountCPP(BuildList_Producer* pBuilder, int32_t shiftOn, uintptr_t* pObjectHash)
{
	return shiftOn*32 + 1;
}

/*
uintptr_t ofs982FE0 = 0x982FE0;

__declspec(naked) void __fastcall sub_982FE0(void* ecx, void* edx, void* a2, void* a3) {
    __asm {
        jmp ofs982FE0
    }
}

uintptr_t ofs416720 = 0x416720;

__declspec(naked) uintptr_t* __cdecl sub_4169E0(size_t a1) {
    __asm {
        jmp ofs416720
    }
}*/

struct FXParticleSystemTemplate
{
	BYTE pad1[0x74];
	void* shader;
	int shaderTypeForComparison;
};
static_assert(offsetof(FXParticleSystemTemplate, shader) == 0x74);
static_assert(offsetof(FXParticleSystemTemplate, shaderTypeForComparison) == 0x78);


struct ValueNode
{
	ValueNode* nextNode;
	ValueNode* previousNode;
	void* item;
};

struct FXParticleSystemManager {
	BYTE pad1[0x38];
	ValueNode node;
};
static_assert(offsetof(FXParticleSystemManager, node) == 0x38);

void __fastcall sub_006CAA60(FXParticleSystemManager* ecx, void* /* edx */, uintptr_t particleSystemRef) {
	uintptr_t ofs2CAA61 = *(uintptr_t*)particleSystemRef;
	uintptr_t ofs2CAA65 = *(uintptr_t*)ofs2CAA61;
	uintptr_t ofs2CAA67 = *(uintptr_t*)ofs2CAA65;
	// ebp, ecx
	ValueNode* nextNode = ecx->node.nextNode;
	// esi
	ValueNode** nextNodeAddr = &ecx->node.nextNode;
	FXParticleSystemTemplate* particleSystem = (FXParticleSystemTemplate*)(ofs2CAA67 + 8);
	if (particleSystem->shader != 0) {
		if ((uintptr_t)nextNode != (uintptr_t)nextNodeAddr) {
			// 006CAA8E
		}
	}
}
