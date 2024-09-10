================ B E G I N N I N G   O F   P R O C E D U R E ================



                     _fdexec:
ffffff8000536970         push       rbp                                         ; XREF=sub_ffffff800053f1e0+2935
ffffff8000536971         mov        rbp, rsp
ffffff8000536974         push       r15
ffffff8000536976         push       r14
ffffff8000536978         push       r13
ffffff800053697a         push       r12
ffffff800053697c         push       rbx
ffffff800053697d         sub        rsp, 0x28
ffffff8000536981         mov        dword [ss:rbp+var_44], esi
ffffff8000536984         mov        qword [ss:rbp+var_40], rdi
ffffff8000536988         mov        rbx, qword [ds:rdi+0xd8]
ffffff800053698f         mov        qword [ss:rbp+var_38], rbx
ffffff8000536993         lea        rax, qword [ds:rdi+0xc0]
ffffff800053699a         mov        qword [ss:rbp+var_30], rax
ffffff800053699e         mov        rdi, rax                                    ; argument #1 for method _lck_mtx_lock
ffffff80005369a1         call       _lck_mtx_lock
ffffff80005369a6         mov        ebx, dword [ds:rbx+0x24]
ffffff80005369a9         test       ebx, ebx
ffffff80005369ab         js         0xffffff8000536b1d

ffffff80005369b1         mov        rax, qword [ss:rbp+var_40]
ffffff80005369b5         lea        rcx, qword [ds:rax+0xd8]
ffffff80005369bc         mov        qword [ss:rbp+var_50], rcx
ffffff80005369c0         and        dword [ss:rbp+var_44], 0x4000
ffffff80005369c7         lea        r14, qword [ds:rax+0x228]
ffffff80005369ce         movsxd     r15, ebx
ffffff80005369d1         nop        qword [ds:rax+0x0]
ffffff80005369d8         nop        qword [ds:rax+rax+0x0]

ffffff80005369e0         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+423
ffffff80005369e4         mov        rcx, qword [ds:rax]
ffffff80005369e7         mov        rax, qword [ds:rax+0x8]
ffffff80005369eb         mov        r12, qword [ds:rcx+r15*8]
ffffff80005369ef         mov        cl, byte [ds:rax+r15]
ffffff80005369f3         cmp        dword [ss:rbp+var_44], 0x0
ffffff80005369f7         je         0xffffff8000536a12

ffffff80005369f9         movzx      edx, cl
ffffff80005369fc         and        edx, 0x21
ffffff80005369ff         cmp        edx, 0x20
ffffff8000536a02         je         0xffffff8000536a0b

ffffff8000536a04         or         cl, 0x1
ffffff8000536a07         mov        byte [ds:rax+r15], cl

ffffff8000536a0b         and        cl, 0xdf                                    ; XREF=_fdexec+146
ffffff8000536a0e         mov        byte [ds:rax+r15], cl

ffffff8000536a12         movzx      eax, cl                                     ; XREF=_fdexec+135
ffffff8000536a15         and        eax, 0x5
ffffff8000536a18         cmp        eax, 0x1
ffffff8000536a1b         je         0xffffff8000536a47

ffffff8000536a1d         test       r12, r12
ffffff8000536a20         je         0xffffff8000536b0d

ffffff8000536a26         mov        r13, qword [ds:r12+0x8]
ffffff8000536a2b         mov        rdi, qword [ss:rbp+var_40]                  ; argument #1 for method _proc_ucred
ffffff8000536a2f         call       _proc_ucred
ffffff8000536a34         mov        rdi, rax                                    ; argument #1 for method _mac_file_check_inherit
ffffff8000536a37         mov        rsi, r13                                    ; argument #2 for method _mac_file_check_inherit
ffffff8000536a3a         call       _mac_file_check_inherit
ffffff8000536a3f         test       eax, eax
ffffff8000536a41         je         0xffffff8000536b0d

ffffff8000536a47         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+171
ffffff8000536a4b         cmp        dword [ds:rax+0x34], ebx
ffffff8000536a4e         jle        0xffffff8000536a5b

ffffff8000536a50         mov        esi, ebx                                    ; argument #2 for method _knote_fdclose
ffffff8000536a52         mov        rdi, qword [ss:rbp+var_40]                  ; argument #1 for method _knote_fdclose
ffffff8000536a56         call       _knote_fdclose

ffffff8000536a5b         mov        rdi, qword [ss:rbp+var_50]                  ; XREF=_fdexec+222
ffffff8000536a5f         mov        rax, qword [ds:rdi]
ffffff8000536a62         mov        rcx, qword [ds:rax]
ffffff8000536a65         mov        rax, qword [ds:rax+0x8]
ffffff8000536a69         movzx      eax, byte [ds:rax+r15]
ffffff8000536a6e         mov        qword [ds:rcx+r15*8], 0x0
ffffff8000536a76         mov        rcx, qword [ds:rdi]
ffffff8000536a79         mov        rcx, qword [ds:rcx+0x8]
ffffff8000536a7d         mov        byte [ds:rcx+r15], 0x0
ffffff8000536a82         test       al, 0x10
ffffff8000536a84         je         0xffffff8000536a8b

ffffff8000536a86         call       _wakeup

ffffff8000536a8b         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+276
ffffff8000536a8f         mov        eax, dword [ds:rax+0x24]
ffffff8000536a92         cmp        ebx, eax
ffffff8000536a94         jne        0xffffff8000536aa3

ffffff8000536a96         test       ebx, ebx
ffffff8000536a98         jle        0xffffff8000536aa3

ffffff8000536a9a         dec        eax
ffffff8000536a9c         mov        rcx, qword [ss:rbp+var_38]
ffffff8000536aa0         mov        dword [ds:rcx+0x24], eax

ffffff8000536aa3         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+292, _fdexec+296
ffffff8000536aa7         cmp        dword [ds:rax+0x28], ebx
ffffff8000536aaa         jle        0xffffff8000536ae2

ffffff8000536aac         mov        dword [ds:rax+0x28], ebx
ffffff8000536aaf         jmp        0xffffff8000536ae2
ffffff8000536ab1         nop        qword [ds:rax+0x0]
ffffff8000536ab8         nop        qword [ds:rax+rax+0x0]

ffffff8000536ac0         mov        dword [ds:r14], 0x1                         ; XREF=_fdexec+376
ffffff8000536ac7         xor        r8d, r8d                                    ; argument #5 for method _msleep
ffffff8000536aca         mov        rdi, r14                                    ; argument #1 for method _msleep
ffffff8000536acd         mov        rsi, qword [ss:rbp+var_30]                  ; argument #2 for method _msleep
ffffff8000536ad1         mov        edx, 0x10                                   ; argument #3 for method _msleep
ffffff8000536ad6         lea        rcx, qword [ds:0xffffff80006da808]          ; "fpdrain", argument #4 for method _msleep
ffffff8000536add         call       _msleep

ffffff8000536ae2         cmp        dword [ds:r12+0x4], 0x0                     ; XREF=_fdexec+314, _fdexec+319
ffffff8000536ae8         jg         0xffffff8000536ac0

ffffff8000536aea         mov        rsi, qword [ds:r12+0x8]                     ; argument #2 for method _closef_locked
ffffff8000536aef         mov        rdi, r12                                    ; argument #1 for method _closef_locked
ffffff8000536af2         mov        rdx, qword [ss:rbp+var_40]                  ; argument #3 for method _closef_locked
ffffff8000536af6         call       _closef_locked
ffffff8000536afb         mov        rdi, r12                                    ; argument #1 for method __FREE_ZONE
ffffff8000536afe         mov        esi, 0x18                                   ; argument #2 for method __FREE_ZONE
ffffff8000536b03         mov        edx, 0x26                                   ; argument #3 for method __FREE_ZONE
ffffff8000536b08         call       __FREE_ZONE

ffffff8000536b0d         dec        r15                                         ; XREF=_fdexec+176, _fdexec+209
ffffff8000536b10         test       rbx, rbx
ffffff8000536b13         lea        rbx, qword [ds:rbx+0xffffffffffffffff]
ffffff8000536b17         jne        0xffffff80005369e0

ffffff8000536b1d         mov        rdi, qword [ss:rbp+var_30]                  ; argument #1 for method _lck_mtx_unlock, XREF=_fdexec+59
ffffff8000536b21         add        rsp, 0x28
ffffff8000536b25         pop        rbx
ffffff8000536b26         pop        r12
ffffff8000536b28         pop        r13
ffffff8000536b2a         pop        r14
ffffff8000536b2c         pop        r15
ffffff8000536b2e         pop        rbp
ffffff8000536b2f         jmp        _lck_mtx_unlock
                        ; endp
ffffff8000536b34         nop        qword [ds:rax+rax+0x0]
ffffff8000536b3a         nop        qword [ds:rax+rax+0x0]
