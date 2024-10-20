			// check CirclesForAttack is 0x11
			mov edx, [esi]
			mov ecx, esi
			call dword ptr[edx + 0x250]
			cmp al, 0x11
			je getNewPos
		ofs387119:
			// if x^2 + y^2 <= 2500
			movaps xmm2, xmm1
			mulss xmm2, xmm1
			movaps xmm1, xmm0
			mulss xmm1, xmm0
			addss xmm2, xmm1
			comiss xmm2, _float2500
			// that is distance is less than 50
			jbe ofs387231
			jmp _ret_CirclesForAttack787138
		ofs387231:
            fld dword ptr [ebp+0x44]
            fld st(0)
            fcos 
            mov eax, [esp+0x3C]
            mov [esp+0x4C-4], eax
            fld dword ptr [esp+0x80-4]
            fmul st(1), st(0)
            fxch st(1)
            fst dword ptr [esp+0x80-4]
            fadd dword ptr [esp+0x38-4]
            fstp dword ptr [esp+0x44-4]
            fxch st(1)
            fsin 
            fmulp st(1), st(0)
            fst dword ptr [esp+0x20-4]
            fadd dword ptr [esp+0x3C-4]
            fstp dword ptr [esp+0x48-4]
        callVFunc28:
            mov edx, _F_ofs8E8F60
            lea eax, [esp+0x60]
            push eax // get arg
            mov ecx, [edx]
            mov eax, _ret_CirclesForAttack787282
            mov edx, [ecx]
            push eax // push return address
            jmp dword ptr [edx+0x28]
		getNewPos:
			lea edx, [esp + 0x34]
            lea eax, callVFunc28
			mov ecx, esi
            push eax // push return address
			jmp JetAIUpdateCirclesForAttackNewPosition
            
