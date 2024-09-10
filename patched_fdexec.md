================ B E G I N N I N G   O F   P R O C E D U R E ================



                     _fdexec:
ffffff8000536760         push       rbp                                         ; XREF=sub_ffffff800053efd0+2935
ffffff8000536761         mov        rbp, rsp
ffffff8000536764         push       r15
ffffff8000536766         push       r14
ffffff8000536768         push       r13
ffffff800053676a         push       r12
ffffff800053676c         push       rbx
ffffff800053676d         sub        rsp, 0x28
ffffff8000536771         mov        dword [ss:rbp+var_44], esi
ffffff8000536774         mov        qword [ss:rbp+var_40], rdi
ffffff8000536778         mov        rbx, qword [ds:rdi+0xd8]
ffffff800053677f         mov        qword [ss:rbp+var_38], rbx
ffffff8000536783         lea        rax, qword [ds:rdi+0xc0]
ffffff800053678a         mov        qword [ss:rbp+var_30], rax
ffffff800053678e         mov        rdi, rax                                    ; argument #1 for method _lck_mtx_lock
ffffff8000536791         call       _lck_mtx_lock
ffffff8000536796         mov        ebx, dword [ds:rbx+0x24]
ffffff8000536799         test       ebx, ebx
ffffff800053679b         js         0xffffff800053690d

ffffff80005367a1         mov        rax, qword [ss:rbp+var_40]
ffffff80005367a5         lea        rcx, qword [ds:rax+0xd8]
ffffff80005367ac         mov        qword [ss:rbp+var_50], rcx
ffffff80005367b0         and        dword [ss:rbp+var_44], 0x4000
ffffff80005367b7         lea        r14, qword [ds:rax+0x228]
ffffff80005367be         movsxd     r15, ebx
ffffff80005367c1         nop        qword [ds:rax+0x0]
ffffff80005367c8         nop        qword [ds:rax+rax+0x0]

ffffff80005367d0         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+423
ffffff80005367d4         mov        rcx, qword [ds:rax]
ffffff80005367d7         mov        rax, qword [ds:rax+0x8]
ffffff80005367db         mov        r12, qword [ds:rcx+r15*8]
ffffff80005367df         test       r12, r12
ffffff80005367e2         mov        cl, byte [ds:rax+r15]
ffffff80005367e6         je         0xffffff8000536807

ffffff80005367e8         cmp        dword [ss:rbp+var_44], 0x0
ffffff80005367ec         je         0xffffff8000536807

ffffff80005367ee         movzx      edx, cl
ffffff80005367f1         and        edx, 0x21
ffffff80005367f4         cmp        edx, 0x20
ffffff80005367f7         je         0xffffff8000536800

ffffff80005367f9         or         cl, 0x1
ffffff80005367fc         mov        byte [ds:rax+r15], cl

ffffff8000536800         and        cl, 0xdf                                    ; XREF=_fdexec+151
ffffff8000536803         mov        byte [ds:rax+r15], cl

ffffff8000536807         movzx      eax, cl                                     ; XREF=_fdexec+134, _fdexec+140
ffffff800053680a         and        eax, 0x5
ffffff800053680d         cmp        eax, 0x1
ffffff8000536810         je         0xffffff800053683c

ffffff8000536812         test       r12, r12
ffffff8000536815         je         0xffffff80005368fd

ffffff800053681b         mov        r13, qword [ds:r12+0x8]
ffffff8000536820         mov        rdi, qword [ss:rbp+var_40]                  ; argument #1 for method _proc_ucred
ffffff8000536824         call       _proc_ucred
ffffff8000536829         mov        rdi, rax                                    ; argument #1 for method _mac_file_check_inherit
ffffff800053682c         mov        rsi, r13                                    ; argument #2 for method _mac_file_check_inherit
ffffff800053682f         call       _mac_file_check_inherit
ffffff8000536834         test       eax, eax
ffffff8000536836         je         0xffffff80005368fd

ffffff800053683c         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+176
ffffff8000536840         cmp        dword [ds:rax+0x34], ebx
ffffff8000536843         jle        0xffffff8000536850

ffffff8000536845         mov        esi, ebx                                    ; argument #2 for method _knote_fdclose
ffffff8000536847         mov        rdi, qword [ss:rbp+var_40]                  ; argument #1 for method _knote_fdclose
ffffff800053684b         call       _knote_fdclose

ffffff8000536850         mov        rdi, qword [ss:rbp+var_50]                  ; XREF=_fdexec+227
ffffff8000536854         mov        rax, qword [ds:rdi]
ffffff8000536857         mov        rcx, qword [ds:rax]
ffffff800053685a         mov        rax, qword [ds:rax+0x8]
ffffff800053685e         movzx      eax, byte [ds:rax+r15]
ffffff8000536863         mov        qword [ds:rcx+r15*8], 0x0
ffffff800053686b         mov        rcx, qword [ds:rdi]
ffffff800053686e         mov        rcx, qword [ds:rcx+0x8]
ffffff8000536872         mov        byte [ds:rcx+r15], 0x0
ffffff8000536877         test       al, 0x10
ffffff8000536879         je         0xffffff8000536880

ffffff800053687b         call       _wakeup

ffffff8000536880         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+281
ffffff8000536884         mov        eax, dword [ds:rax+0x24]
ffffff8000536887         cmp        ebx, eax
ffffff8000536889         jne        0xffffff8000536898

ffffff800053688b         test       ebx, ebx
ffffff800053688d         jle        0xffffff8000536898

ffffff800053688f         dec        eax
ffffff8000536891         mov        rcx, qword [ss:rbp+var_38]
ffffff8000536895         mov        dword [ds:rcx+0x24], eax

ffffff8000536898         mov        rax, qword [ss:rbp+var_38]                  ; XREF=_fdexec+297, _fdexec+301
ffffff800053689c         cmp        dword [ds:rax+0x28], ebx
ffffff800053689f         jle        0xffffff80005368d2

ffffff80005368a1         mov        dword [ds:rax+0x28], ebx
ffffff80005368a4         jmp        0xffffff80005368d2
ffffff80005368a6         nop        qword [cs:rax+rax+0x0]

ffffff80005368b0         mov        dword [ds:r14], 0x1                         ; XREF=_fdexec+376
ffffff80005368b7         xor        r8d, r8d                                    ; argument #5 for method _msleep
ffffff80005368ba         mov        rdi, r14                                    ; argument #1 for method _msleep
ffffff80005368bd         mov        rsi, qword [ss:rbp+var_30]                  ; argument #2 for method _msleep
ffffff80005368c1         mov        edx, 0x10                                   ; argument #3 for method _msleep
ffffff80005368c6         lea        rcx, qword [ds:0xffffff80006e0ee1]          ; "fpdrain", argument #4 for method _msleep
ffffff80005368cd         call       _msleep

ffffff80005368d2         cmp        dword [ds:r12+0x4], 0x0                     ; XREF=_fdexec+319, _fdexec+324
ffffff80005368d8         jg         0xffffff80005368b0

ffffff80005368da         mov        rsi, qword [ds:r12+0x8]                     ; argument #2 for method _closef_locked
ffffff80005368df         mov        rdi, r12                                    ; argument #1 for method _closef_locked
ffffff80005368e2         mov        rdx, qword [ss:rbp+var_40]                  ; argument #3 for method _closef_locked
ffffff80005368e6         call       _closef_locked
ffffff80005368eb         mov        rdi, r12                                    ; argument #1 for method __FREE_ZONE
ffffff80005368ee         mov        esi, 0x18                                   ; argument #2 for method __FREE_ZONE
ffffff80005368f3         mov        edx, 0x26                                   ; argument #3 for method __FREE_ZONE
ffffff80005368f8         call       __FREE_ZONE

ffffff80005368fd         dec        r15                                         ; XREF=_fdexec+181, _fdexec+214
ffffff8000536900         test       rbx, rbx
ffffff8000536903         lea        rbx, qword [ds:rbx+0xffffffffffffffff]
ffffff8000536907         jne        0xffffff80005367d0

ffffff800053690d         mov        rdi, qword [ss:rbp+var_30]                  ; argument #1 for method _lck_mtx_unlock, XREF=_fdexec+59
ffffff8000536911         add        rsp, 0x28
ffffff8000536915         pop        rbx
ffffff8000536916         pop        r12
ffffff8000536918         pop        r13
ffffff800053691a         pop        r14
ffffff800053691c         pop        r15
ffffff800053691e         pop        rbp
ffffff800053691f         jmp        _lck_mtx_unlock
                        ; endp
ffffff8000536924         nop        qword [ds:rax+rax+0x0]
ffffff800053692a         nop        qword [ds:rax+rax+0x0]
