 B E G I N N I N G   O F   P R O C E D U R E ================       B E G I N N I N G   O F   P R O C E D U R E ================



     _fdexec:                                                           _fdexec:
         push       rbp                                                     push       rbp                                        
         mov        rbp, rsp                                                mov        rbp, rsp
         push       r15                                                     push       r15
         push       r14                                                     push       r14
         push       r13                                                     push       r13
         push       r12                                                     push       r12
         push       rbx                                                     push       rbx
         sub        rsp, 0x28                                               sub        rsp, 0x28
         mov        dword [ss:rbp+var_44], esi                              mov        dword [ss:rbp+var_44], esi
         mov        qword [ss:rbp+var_40], rdi                              mov        qword [ss:rbp+var_40], rdi
         mov        rbx, qword [ds:rdi+0xd8]                                mov        rbx, qword [ds:rdi+0xd8]
         mov        qword [ss:rbp+var_38], rbx                              mov        qword [ss:rbp+var_38], rbx
         lea        rax, qword [ds:rdi+0xc0]                                lea        rax, qword [ds:rdi+0xc0]
         mov        qword [ss:rbp+var_30], rax                              mov        qword [ss:rbp+var_30], rax
         mov        rdi, rax                                                mov        rdi, rax                                   
         call       _lck_mtx_lock                                           call       _lck_mtx_lock
         mov        ebx, dword [ds:rbx+0x24]                                mov        ebx, dword [ds:rbx+0x24]
         test       ebx, ebx                                                test       ebx, ebx
         js         0xffffff800053690d                          |           js         0xffffff8000536b1d

         mov        rax, qword [ss:rbp+var_40]                              mov        rax, qword [ss:rbp+var_40]
         lea        rcx, qword [ds:rax+0xd8]                                lea        rcx, qword [ds:rax+0xd8]
         mov        qword [ss:rbp+var_50], rcx                              mov        qword [ss:rbp+var_50], rcx
         and        dword [ss:rbp+var_44], 0x4000                           and        dword [ss:rbp+var_44], 0x4000
         lea        r14, qword [ds:rax+0x228]                               lea        r14, qword [ds:rax+0x228]
         movsxd     r15, ebx                                                movsxd     r15, ebx
         nop        qword [ds:rax+0x0]                                      nop        qword [ds:rax+0x0]
         nop        qword [ds:rax+rax+0x0]                                  nop        qword [ds:rax+rax+0x0]

         mov        rax, qword [ss:rbp+var_38]                              mov        rax, qword [ss:rbp+var_38]                 
         mov        rcx, qword [ds:rax]                                     mov        rcx, qword [ds:rax]
         mov        rax, qword [ds:rax+0x8]                                 mov        rax, qword [ds:rax+0x8]
         mov        r12, qword [ds:rcx+r15*8]                               mov        r12, qword [ds:rcx+r15*8]
         test       r12, r12                                    <
         mov        cl, byte [ds:rax+r15]                                   mov        cl, byte [ds:rax+r15]
         je         0xffffff8000536807                          <
                                                                <
         cmp        dword [ss:rbp+var_44], 0x0                              cmp        dword [ss:rbp+var_44], 0x0
         je         0xffffff8000536807                          |           je         0xffffff8000536a12

         movzx      edx, cl                                                 movzx      edx, cl
         and        edx, 0x21                                               and        edx, 0x21
         cmp        edx, 0x20                                               cmp        edx, 0x20
         je         0xffffff8000536800                          |           je         0xffffff8000536a0b

         or         cl, 0x1                                                 or         cl, 0x1
         mov        byte [ds:rax+r15], cl                                   mov        byte [ds:rax+r15], cl

         and        cl, 0xdf                                                and        cl, 0xdf                                   
         mov        byte [ds:rax+r15], cl                                   mov        byte [ds:rax+r15], cl

         movzx      eax, cl                                                 movzx      eax, cl                                    
         and        eax, 0x5                                                and        eax, 0x5
         cmp        eax, 0x1                                                cmp        eax, 0x1
         je         0xffffff800053683c                          |           je         0xffffff8000536a47

         test       r12, r12                                                test       r12, r12
         je         0xffffff80005368fd                          |           je         0xffffff8000536b0d

         mov        r13, qword [ds:r12+0x8]                                 mov        r13, qword [ds:r12+0x8]
         mov        rdi, qword [ss:rbp+var_40]                              mov        rdi, qword [ss:rbp+var_40]                 
         call       _proc_ucred                                             call       _proc_ucred
         mov        rdi, rax                                                mov        rdi, rax                                   
         mov        rsi, r13                                                mov        rsi, r13                                   
         call       _mac_file_check_inherit                                 call       _mac_file_check_inherit
         test       eax, eax                                                test       eax, eax
         je         0xffffff80005368fd                          |           je         0xffffff8000536b0d

         mov        rax, qword [ss:rbp+var_38]                              mov        rax, qword [ss:rbp+var_38]                 
         cmp        dword [ds:rax+0x34], ebx                                cmp        dword [ds:rax+0x34], ebx
         jle        0xffffff8000536850                          |           jle        0xffffff8000536a5b

         mov        esi, ebx                                                mov        esi, ebx                                   
         mov        rdi, qword [ss:rbp+var_40]                              mov        rdi, qword [ss:rbp+var_40]                 
         call       _knote_fdclose                                          call       _knote_fdclose

         mov        rdi, qword [ss:rbp+var_50]                              mov        rdi, qword [ss:rbp+var_50]                 
         mov        rax, qword [ds:rdi]                                     mov        rax, qword [ds:rdi]
         mov        rcx, qword [ds:rax]                                     mov        rcx, qword [ds:rax]
         mov        rax, qword [ds:rax+0x8]                                 mov        rax, qword [ds:rax+0x8]
         movzx      eax, byte [ds:rax+r15]                                  movzx      eax, byte [ds:rax+r15]
         mov        qword [ds:rcx+r15*8], 0x0                               mov        qword [ds:rcx+r15*8], 0x0
         mov        rcx, qword [ds:rdi]                                     mov        rcx, qword [ds:rdi]
         mov        rcx, qword [ds:rcx+0x8]                                 mov        rcx, qword [ds:rcx+0x8]
         mov        byte [ds:rcx+r15], 0x0                                  mov        byte [ds:rcx+r15], 0x0
         test       al, 0x10                                                test       al, 0x10
         je         0xffffff8000536880                          |           je         0xffffff8000536a8b

         call       _wakeup                                                 call       _wakeup

         mov        rax, qword [ss:rbp+var_38]                              mov        rax, qword [ss:rbp+var_38]                 
         mov        eax, dword [ds:rax+0x24]                                mov        eax, dword [ds:rax+0x24]
         cmp        ebx, eax                                                cmp        ebx, eax
         jne        0xffffff8000536898                          |           jne        0xffffff8000536aa3

         test       ebx, ebx                                                test       ebx, ebx
         jle        0xffffff8000536898                          |           jle        0xffffff8000536aa3

         dec        eax                                                     dec        eax
         mov        rcx, qword [ss:rbp+var_38]                              mov        rcx, qword [ss:rbp+var_38]
         mov        dword [ds:rcx+0x24], eax                                mov        dword [ds:rcx+0x24], eax

         mov        rax, qword [ss:rbp+var_38]                              mov        rax, qword [ss:rbp+var_38]                 
         cmp        dword [ds:rax+0x28], ebx                                cmp        dword [ds:rax+0x28], ebx
         jle        0xffffff80005368d2                          |           jle        0xffffff8000536ae2

         mov        dword [ds:rax+0x28], ebx                                mov        dword [ds:rax+0x28], ebx
         jmp        0xffffff80005368d2                          |           jmp        0xffffff8000536ae2
         nop        qword [cs:rax+rax+0x0]                      |           nop        qword [ds:rax+0x0]
                                                                >           nop        qword [ds:rax+rax+0x0]

         mov        dword [ds:r14], 0x1                                     mov        dword [ds:r14], 0x1                        
         xor        r8d, r8d                                                xor        r8d, r8d                                   
         mov        rdi, r14                                                mov        rdi, r14                                   
         mov        rsi, qword [ss:rbp+var_30]                              mov        rsi, qword [ss:rbp+var_30]                 
         mov        edx, 0x10                                               mov        edx, 0x10                                  
         lea        rcx, qword [ds:0xffffff80006e0ee1]          |           lea        rcx, qword [ds:0xffffff80006da808]         
         call       _msleep                                                 call       _msleep

         cmp        dword [ds:r12+0x4], 0x0                                 cmp        dword [ds:r12+0x4], 0x0                    
         jg         0xffffff80005368b0                          |           jg         0xffffff8000536ac0

         mov        rsi, qword [ds:r12+0x8]                                 mov        rsi, qword [ds:r12+0x8]                    
         mov        rdi, r12                                                mov        rdi, r12                                   
         mov        rdx, qword [ss:rbp+var_40]                              mov        rdx, qword [ss:rbp+var_40]                 
         call       _closef_locked                                          call       _closef_locked
         mov        rdi, r12                                                mov        rdi, r12                                   
         mov        esi, 0x18                                               mov        esi, 0x18                                  
         mov        edx, 0x26                                               mov        edx, 0x26                                  
         call       __FREE_ZONE                                             call       __FREE_ZONE

         dec        r15                                                     dec        r15                                        
         test       rbx, rbx                                                test       rbx, rbx
         lea        rbx, qword [ds:rbx+0xffffffffffffffff]                  lea        rbx, qword [ds:rbx+0xffffffffffffffff]
         jne        0xffffff80005367d0                          |           jne        0xffffff80005369e0

         mov        rdi, qword [ss:rbp+var_30]                              mov        rdi, qword [ss:rbp+var_30]                 
         add        rsp, 0x28                                               add        rsp, 0x28
         pop        rbx                                                     pop        rbx
         pop        r12                                                     pop        r12
         pop        r13                                                     pop        r13
         pop        r14                                                     pop        r14
         pop        r15                                                     pop        r15
         pop        rbp                                                     pop        rbp
         jmp        _lck_mtx_unlock                                         jmp        _lck_mtx_unlock
        ; endp                                                             ; endp
         nop        qword [ds:rax+rax+0x0]                                  nop        qword [ds:rax+rax+0x0]
         nop        qword [ds:rax+rax+0x0]                                  nop        qword [ds:rax+rax+0x0]
