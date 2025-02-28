    ; ebx is 0
    ;mov [esp+8], ebx
    mov [esp+0xC], ebx
    mov [esp+0x10], ebx
    mov [esp+0x14], ebx
    mov [esp+0x18], ebx
    mov [esp+0x1C], ebx
    mov [esp+0x20], ebx
    mov [esp+0x24], ebx
    mov ebx, eax ; save rule pointer
    mov al, [ebx+0x74]
    test al, 1
    jz powerfulMode
    mov edx, _F_SkirmishSettingsPointerCDBBE0
    mov ecx, [edx]
    mov edx, [ecx+8] ; RandomCrateSpawner
    mov eax, _F_GameObjectHashCE6C58
    mov ecx, [eax]
    push edx ; hash
    call _F_CallGetPointer7E4230
    test eax, eax
    je powerfulMode
    mov [esp+8], eax
    lea ecx, [esp+8]
    push ecx
    call _F_Call4D8420
    add esp, 4
    ;
    xor eax, eax
    ;mov [esp+8], eax
    mov [esp+0xC], eax
    mov [esp+0x10], eax
    mov [esp+0x14], eax
    mov [esp+0x18], eax
    mov [esp+0x1C], eax
    mov [esp+0x20], eax
    mov [esp+0x24], eax
powerfulMode:
    mov al, [ebx+0x74]
    test al, 10b
    jz noSuperWeapon
    mov edx, 3804486283 ; GenericPowerfulMode
    mov eax, _F_GameObjectHashCE6C58
    mov ecx, [eax]
    push edx ; hash
    call _F_CallGetPointer7E4230
    test eax, eax
    je noSuperWeapon
    mov [esp+8], eax
    lea ecx, [esp+8]
    push ecx
    call _F_Call4D8420
    add esp, 4
    ;
    xor eax, eax
    ;mov [esp+8], eax
    mov [esp+0xC], eax
    mov [esp+0x10], eax
    mov [esp+0x14], eax
    mov [esp+0x18], eax
    mov [esp+0x1C], eax
    mov [esp+0x20], eax
    mov [esp+0x24], eax
noSuperWeapon:
    mov al, [ebx+0x74+1]
    test al, 10b
    jz ofs216E23
    mov edx, 2717237826 ; GenericNoSuperWeaponMode
    mov eax, _F_GameObjectHashCE6C58
    mov ecx, [eax]
    push edx ; hash
    call _F_CallGetPointer7E4230
    test eax, eax
    je ofs216E23
    mov [esp+8], eax
    lea ecx, [esp+8]
    push ecx
    call _F_Call4D8420
    add esp, 4
    ;
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
