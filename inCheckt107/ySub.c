//
//  KQueueScanContinuePatch.c
//  KQueueScanContinuePatch
//
//  Created by Wowfunhappy with assistance from krackers.
//

#include "ySub.h"
#include "helperFn.c"

static void injectInstructions() {
        // Add a bunch of nops so there is enough dead space in your func
#ifdef __LP64__
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        //IOLog("YellowTrampoline::%s: Success\n", __func__);
#ifdef INSERT_TRAP
        __builtin_trap();
#endif
        
        __asm__(".intel_syntax \t\n"
                "test       r12, r12"); //new
        __asm__(".intel_syntax \t\n"
                "mov        cl, byte ptr [rax+r15]"); //original
        __asm__("je0:");
        __asm__(".intel_syntax \t\n"
                "je         [0x90]"); //original
        
        __asm__(".intel_syntax\t\n"
                "cmp      dword ptr [rbp-0x44], 0x0"); // new
        __asm__("je1:");
        __asm__(".intel_syntax \t\n"
                "je         [0x90]"); //new, but simply a jump after check for the fp variable.
        
        //jump back if these checks are both false.
        __asm__("jmp0:");
        __asm__(".intel_syntax noprefix\t\n"
                "jmp        [0xffffff90909090]");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
#else
        // Add a bunch of nops so there is enough dead space in your func
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        
        __asm__(".intel_syntax \t\n"
                "test       ebx, ebx"); //new
        __asm__(".intel_syntax \t\n"
                "mov        cl, byte ptr ds:[eax+esi]"); //original
        __asm__("je0:");
        __asm__(".intel_syntax \t\n"
                "je         [0x90]"); //original
        __asm__(".intel_syntax \t\n"
                "cmp        dword ptr [ebp-0x20], 0x0"); // new
        __asm__("je1:");
        __asm__(".intel_syntax \t\n"
                "je         [0x90]"); //new, but simply a jump after check for the fp variable.
        
        //jump back if these checks are both false.
        __asm__("jmp0:");
        __asm__(".intel_syntax noprefix \t\n"
                "jmp        [0x90]");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
#endif
        
}

static void computeRelativeAddressesAndOverwrite() {

#ifdef __LP64__
        int64_t je_abs=0, jmp_abs=0;
#else
        int32_t je_abs=0, jmp_abs=0;
#endif
        uint8_t *scanStart = (uint8_t*) originAddress;
        int32_t je0_rel=0, je1_rel=0, jmp0_rel=0;
         for (int k=0; k<64;k++) {
                 if(scanStart[k] == 0x74) {
                         je_abs = (&scanStart[k+2] + scanStart[k+1]);
                         jmp_abs = &scanStart[k+2];
                         break;
                 }
         }

        /* the point of this function is to calculate the relative address from the
         * memory locations given by the labels je0, je1, jmp0, which the compiler will
         * insert through the clever stackexchange post found by @krackers.
         * we now overwrite the compiled trampoline jmp/je dummy addresses with the real ones
         * that we find using inline asm.
         * addresses are correct now.
         */
#ifdef __LP64__
        int64_t absAddr = 0;
#else
        int32_t absAddr = 0;
#endif
        
#ifdef DEBUG
        //ensure that the label of je0 printed here is in agreement with the
        //debug print loop that prints 128 bytes of memory starting at the trampoline function.
        asm ("movabs $je0, %0 \n\t"
             : "=r" (absAddr)
             );
        IOLog("YellowTrampoline::%s: je0 absolute: %llx\n", __func__, absAddr);
        absAddr = 0;
#endif
        
        asm ("movabs $je0, %0 \n\t"
             : "=r" (absAddr)
             );
        je0_rel = (int32_t) (je_abs - (absAddr + 5));
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: je0 absolute: %llx, 32bit: %x\n", __func__, absAddr, je0_rel);
#endif
        
        absAddr = 0;
        asm ("movabs $je1, %0 \n\t"
             : "=r" (absAddr)
             );
        je1_rel = (int32_t) (je_abs - (absAddr + 5));
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: je1 absolute: %llx, 32bit: %x\n", __func__, absAddr, je1_rel);
#endif
        
        absAddr = 0;
        asm ("movabs $jmp0, %0 \n\t"
             : "=r" (absAddr)
             );
        jmp0_rel = (int32_t) (jmp_abs - (absAddr + 5));
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: jmp0 absolute: %llx, 32bit: %x\n", __func__, absAddr, jmp0_rel);
#endif
        
        //commence overwriting
        
        int count = 0;
        uint8_t *matchOpCodeBytes =  (uint8_t*) &injectInstructions;
        for (int k=0; k<64;k++) {
                //  IOLog("YellowTrampoline::%s:: %llx %02x\n", __func__, funcAddr + byteCount, matchOpCodeBytes[k]);
                if(matchOpCodeBytes[k] == 0x0f && matchOpCodeBytes[k+1] == 0x84 && !count) {
                        memcpy(&matchOpCodeBytes[k]+2, &je0_rel, sizeof(je0_rel));
                        count++;
                }
                else if(matchOpCodeBytes[k] == 0x0f && matchOpCodeBytes[k+1] == 0x84 && count == 1){
                        memcpy(&matchOpCodeBytes[k]+2, &je1_rel, sizeof(je1_rel));
                        count++;
                }
                else if(matchOpCodeBytes[k] == 0xe9 && count == 2) {
                        memcpy(&matchOpCodeBytes[k]+1, &jmp0_rel, sizeof(jmp0_rel));
                        count++;
                } else if(count > 2)
                        break; // we done
                
        }
        
}

kern_return_t YellowTrampoline_start(kmod_info_t * ki, void *d)
{
        if(checkKernelVersion() != KERN_SUCCESS)
                return KERN_FAILURE;
        
        vm_offset_t kernel_base = get_kernel_base();
        char search_bytes[4];
        
        
        for (int i = 0; i < LENGTH(possible_kqueue_scan_continue_panic_start_locations); i++) {
                bzero(search_bytes, 4);
                originAddress = kernel_base + possible_kqueue_scan_continue_panic_start_locations[i];
                memcpy(search_bytes, possible_search_bytes[i], sizeof(search_bytes));
                
                if (memcmp((void*) originAddress, search_bytes, sizeof(search_bytes)) == 0) {
                        break;
                }
                if (i == LENGTH(possible_kqueue_scan_continue_panic_start_locations) - 1) {
                        IOLog("YellowTrampoline::%s: Memory region not found. You are probably using an unsupported kernel, or your kernel has already been patched.\n", __func__);
                        return KERN_FAILURE;
                }
        }
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: Pre-Patch: Bytes at kqueue_scan_continue panic location: ", __func__);
        uint8_t *kscpb = (uint8_t*) originAddress;
        for (int k=0; k < 39; k ++)
                IOLog(" %02x", kscpb[k]);
        IOLog(" %02x\n", kscpb[39]);
#endif
        
        
        interrupt_status = ml_get_interrupts_enabled();
        write_protection_status = write_protection_is_enabled();
        
        if(disableInterruptsAndProtection(interrupt_status, write_protection_status) == KERN_FAILURE)
                return KERN_FAILURE;
        
        long long funcAddr = (long long) &injectInstructions;
        uint8_t *matchOpCodeBytes = (uint8_t*) &injectInstructions;
        long long byteCount = 0;
        
        /*The DEBUG macro inserts an invalid opcode trap
         * to verify we have jumped to the right function
         */
#ifdef INSERT_TRAP
        while (true) {
                
                IOLog("YellowTrampoline::%s:: %llx %02x\n", __func__, funcAddr + byteCount, *matchOpCodeBytes);
                if (*matchOpCodeBytes == 0x0F && matchOpCodeBytes[1] == 0x0B) // 0x0F 0x0B is a trap code
                        break;
                matchOpCodeBytes += 1;
                byteCount += 1;
        }
        IOLog("YellowTrampoline::%s: Trap at %llx\n", __func__, (long long) funcAddr + byteCount);
        byteCount = 0;
#endif
        
#ifdef DEBUG
        TheLadyIsAVamp(funcAddr, "BEFORE");
#endif
        
        computeRelativeAddressesAndOverwrite();
        
#ifdef DEBUG
        TheLadyIsAVamp(funcAddr, "AFTER");
#endif
        //commence memory rewriting
        matchOpCodeBytes = (uint8_t*)  &injectInstructions;
        byteCount = 0;
        while (true) {
#ifdef DEBUG
                IOLog("YellowTrampoline::%s:: %llx %02x\n", __func__, funcAddr + byteCount, *matchOpCodeBytes);
#endif
                if (*matchOpCodeBytes == (unsigned char) 0x90 && matchOpCodeBytes[1] == (unsigned char) 0x90) // 2 nops in a row, we're probably after the prologue
                        break;
                matchOpCodeBytes += 1;
                byteCount += 1;
        }
        uint32_t pcDelta = (funcAddr + byteCount) - originAddress;
        
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: funcAddr: %llx, real start %llx\n", __func__, funcAddr, funcAddr + byteCount);
        IOLog("YellowTrampoline::%s: current %llx\n", __func__, originAddress);
        IOLog("YellowTrampoline::%s: Offset is %x, full %llx\n", __func__, pcDelta, (funcAddr + byteCount) - originAddress);
#endif
        
        /* presumably have to subtract 5 bytes to save/offset something in the counter,
         * since https://defuse.ca/online-x86-assembler.htm decodes to an address that
         * seems to add 5 bytes to the address
         */
        pcDelta -= 5;
        //save original bytes first
        memcpy(&original_bytes[0], (void *)originAddress, sizeof(original_bytes));
        
        //create new jmp asm instruction.
        memset(&replacement_bytes[0], 0x90, sizeof(replacement_bytes));
        memset(&replacement_bytes[0], 0xE9, 1);
        memcpy(&replacement_bytes[1], &pcDelta, sizeof(pcDelta));
        
        /*
         * effectively wiping out the 3 lines with this jump (since replacement_bytes is 10 bytes):
         *
         *  ffffff80005369ef         mov        cl, byte [ds:rax+r15]
         *  ffffff80005369f3         cmp        dword [ss:rbp+var_44], 0x0
         *  ffffff80005369f7         je         0xffffff8000536a12
         */
        memcpy((void *)originAddress, replacement_bytes, sizeof(replacement_bytes));
        
        //conclude memory rewriting
        enableInterruptsAndProtection(interrupt_status, write_protection_status);
        
#ifdef DEBUG
        IOLog("YellowTrampoline::%s: Post-Patch: Bytes at kqueue_scan_continue panic location: ", __func__);
        for (int k=0; k < 39; k ++)
                IOLog(" %02x", kscpb[k]);
        IOLog(" %02x\n", kscpb[39]);
#endif
        
        return KERN_SUCCESS;
}

kern_return_t YellowTrampoline_stop(kmod_info_t *ki, void *d)
{
        //should write back the old shit.
        disableInterruptsAndProtection(interrupt_status, write_protection_status);
        memcpy((void *)originAddress, &original_bytes, sizeof(original_bytes));
        enableInterruptsAndProtection(interrupt_status, write_protection_status);
        IOLog("YellowTrampoline::%s: Unloading\n", __func__);

        return KERN_SUCCESS;
}
