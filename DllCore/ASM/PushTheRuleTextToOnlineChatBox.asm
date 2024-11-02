    sub esp, 8+4
    mov [esp], ecx
    mov dword ptr [esp+4], 0
    mov [esp+8], ecx
    //
    mov eax, _F_GameTextPointerCDB754
    mov ecx, [eax]
    mov eax, [ecx]
    push 0
    push edx // pText
    call dword ptr [eax+0x40]
    //
    push eax
    lea ecx, [esp+4+4]
    call _F_CallGetEAString4CB750
    //
    mov ecx, [esp]
    mov eax, [ecx]
    lea edx, [esp+4]
    push edx
    push 0
    call dword ptr [eax+4]
    //
    mov eax, _F_CallFreeEAString4CB6D0
    lea ecx, dword ptr [esp+4]
    call eax
    //
    mov eax, [esp]
    cmp [esp+8], eax
    je safeStack
    int 3
safeStack:
    add esp, 8+4
    ret
