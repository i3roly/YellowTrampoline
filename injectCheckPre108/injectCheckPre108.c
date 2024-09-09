//
//  KQueueScanContinuePatch.c
//  KQueueScanContinuePatch
//
//  Created by Wowfunhappy with assistance from krackers.
//

#include "injectCheckPre108.h"
#include "helperFn.c"

void dummyFuncLion() {
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
        IOLog("%s: Success\n", __func__);
        //        __asm__("test       %r12, %r12"); //new
        //        __asm__("mov        cl, byte [ds:rax+r15]"); //original
        //        __asm__("je         0xffffff8000536a12"); //original
        //        __asm__("cmp        dword [ss:rbp+var_44], 0x0"); // new
        //        __asm__("je         0xffffff8000536a12"); //new, but simply a jump after check for the fp variable.
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
        __asm__("test       ebx, ebx"); //new
        __asm__("mov        cl, byte [ds:eax+esi]"); //original
        __asm__("je         0x55671b"); //original
        __asm__("cmp        dword [ss:ebp+var_20], 0x0"); // new
        __asm__("je         0x55671b"); //new, but simply a jump after check for the fp variable.
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
#endif
}

kern_return_t injectCheckPre108_start(kmod_info_t * ki, void *d)
{
        IOLog("KQueueScanContinuePatch START\n");
        kernel_base = get_kernel_base();
        char search_bytes[sizeof(possible_search_bytes[0])];
        
        
        for (int i = 0; i < LENGTH(possible_kqueue_scan_continue_panic_start_locations); i++) {
                kqueue_scan_continue_panic_start_location = kernel_base + possible_kqueue_scan_continue_panic_start_locations[i];
                kqueue_scan_continue_panic_end_location = kernel_base + possible_kqueue_scan_continue_panic_end_locations[i];
                memcpy(search_bytes, possible_search_bytes[i], sizeof(search_bytes));
                //memcpy(replacement_bytes, possible_replacement_bytes[i], sizeof(replacement_bytes));
                
                kscpb = (uint8_t*) kqueue_scan_continue_panic_start_location;
                if (memcmp(kscpb, search_bytes, sizeof(search_bytes)) == 0) {
                        break;
                }
                if (i == LENGTH(possible_kqueue_scan_continue_panic_start_locations) - 1) {
                        IOLog("%s: Memory region not found. You are probably using an unsupported kernel, or your kernel has already been patched.\n", __func__);
                        return KERN_FAILURE;
                }
        }
        
        IOLog("%s: Pre-Patch: Bytes at kqueue_scan_continue panic location: ", __func__);
        for (int k=0; k < 39; k ++)
                IOLog(" %02x", kscpb[k]);
        IOLog(" %02x\n", kscpb[39]);
        
        unsigned long extra_space_to_fill = kqueue_scan_continue_panic_end_location - kqueue_scan_continue_panic_start_location - sizeof(replacement_bytes);
        
        if (kqueue_scan_continue_panic_start_location + sizeof(replacement_bytes) + extra_space_to_fill != kqueue_scan_continue_panic_end_location) {
                IOLog("kqueue_scan_continue_panic_start_location + sizeof(replacement_bytes) + extra_space_to_fill != kqueue_scan_continue_panic_end_location\n");
                return KERN_FAILURE;
        }
        
        interrupt_status = ml_get_interrupts_enabled();
        write_protection_status = write_protection_is_enabled();
        
        if(disableInterruptsAndProtection(interrupt_status, write_protection_status) == KERN_FAILURE)
                return KERN_FAILURE;
        
        
        //commence memory rewriting
        IOLog("%s: Jumping to Dummy function\n", __func__);
        long long funcAddr = (long long) &dummyFuncLion;
        IOLog("%s: funcAddr: %llx\n", __func__, funcAddr);
        IOLog("%s: current %llx\n", __func__, 0xffffff80005369ef);
        long long pcDelta = funcAddr - 0xffffff80005369ef;
        IOLog("%s: Value is %llx\n", __func__, pcDelta);
        // presumably have to subtract 5 bytes to save/offset something in the counter,
        // since https://defuse.ca/online-x86-assembler.htm decodes to an address that
        // seems to add 5 bytes to the address
        pcDelta -= 5;
        //save original bytes first
        memcpy(&original_bytes[0], (void *)kqueue_scan_continue_panic_start_location, sizeof(original_bytes));
        
        //create new jmp asm instruction.
        memset(&replacement_bytes[0], 0xE9, 1);
        memcpy(&replacement_bytes[1], &pcDelta, sizeof(pcDelta));
        //effectively wiping out the 3 lines with this jump (since replacement_bytes is 10 bytes):
        //ffffff80005369ef         mov        cl, byte [ds:rax+r15]
        //ffffff80005369f3         cmp        dword [ss:rbp+var_44], 0x0
        //ffffff80005369f7         je         0xffffff8000536a12
        memcpy((void *)kqueue_scan_continue_panic_start_location, replacement_bytes, sizeof(replacement_bytes));
        // memset((void *)kqueue_scan_continue_panic_start_location + sizeof(replacement_bytes), 0x90 /*nop*/, extra_space_to_fill);
        
        //conclude memory rewriting
        enableInterruptsAndProtection(interrupt_status, write_protection_status);
        
        
        IOLog("%s: Post-Patch: Bytes at kqueue_scan_continue panic location: ", __func__);
        for (int k=0; k < 39; k ++)
                IOLog(" %02x", kscpb[k]);
        IOLog(" %02x\n", kscpb[39]);
        
        return KERN_SUCCESS;
}

kern_return_t injectCheckPre108_stop(kmod_info_t *ki, void *d)
{
        IOLog("KQueueScanContinuePatch STOP\n");
        //should write back the old shit.
        disableInterruptsAndProtection(interrupt_status, write_protection_status);
        memcpy((void *)kqueue_scan_continue_panic_start_location, &original_bytes, sizeof(original_bytes));
        enableInterruptsAndProtection(interrupt_status, write_protection_status);
        IOLog("%s: UNLOAD: Bytes at kqueue_scan_continue panic location: ", __func__);
        for (int k=0; k < 39; k ++)
                IOLog(" %02x", kscpb[k]);
        IOLog(" %02x\n", kscpb[39]);
        
        //__asm__("jmp         0xffffff8000536a12");
        return KERN_SUCCESS;
}
