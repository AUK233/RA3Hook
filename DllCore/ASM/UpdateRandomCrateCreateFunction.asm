    // ebx is 0
    //mov [esp+8], ebx
    mov [esp+0xC], ebx
    mov [esp+0x10], ebx
    mov [esp+0x14], ebx
    mov [esp+0x18], ebx
    mov [esp+0x1C], ebx
    mov [esp+0x20], ebx
    mov [esp+0x24], ebx
    mov ebx, eax // save rule pointer
    cmp byte ptr [ebx+0x74], 0
    je powerfulMode
    mov edx, _F_SkirmishSettingsPointerCDBBE0
    mov ecx, [edx]
    mov edx, [ecx+8] // RandomCrateSpawner
    mov eax, _F_GameObjectHashCE6C58
    mov ecx, [eax]
    push edx // hash
    call _F_CallGetPointer7E4230
    test eax, eax
    je powerfulMode
    mov [esp+8], eax
    lea ecx, [esp+8]
    push ecx
    call _F_Call4D8420
    add esp, 4
    //
    xor eax, eax
    //mov [esp+8], eax
    mov [esp+0xC], eax
    mov [esp+0x10], eax
    mov [esp+0x14], eax
    mov [esp+0x18], eax
    mov [esp+0x1C], eax
    mov [esp+0x20], eax
    mov [esp+0x24], eax
powerfulMode:
    cmp byte ptr [ebx+0x74+1], 0
    je ofs216E23
    mov edx, 0x86F89A33
    mov eax, _F_GameObjectHashCE6C58
    mov ecx, [eax]
    push edx // hash
    call _F_CallGetPointer7E4230
    test eax, eax
    je ofs216E23
    mov [esp+8], eax
    lea ecx, [esp+8]
    push ecx
    call _F_Call4D8420
    add esp, 4
ofs216E23:
    test ebp, ebp
    je ofs216E30
    push ebp
    call _F_Call416830
    add esp, 4
ofs216E30:
    pop ebp
    pop ebx
    add esp, 0x20
    ret 