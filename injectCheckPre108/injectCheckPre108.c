//
//  KQueueScanContinuePatch.c
//  KQueueScanContinuePatch
//
//  Created by Wowfunhappy with assistance from krackers.
//

#include <IOKit/IOLib.h>
#include <mach-o/loader.h>
#include <i386/proc_reg.h>
#include <string.h>

#define LENGTH(arr) sizeof(arr) / sizeof(*arr)

// Copied from github.com/leiless/ksymresolver/blob/master/ksymresolver/ksymresolver.h
#define KERN_TEXT_BASE ((vm_offset_t) 0xffffff8000200000ULL)

#define        CR0_WP 0x00010000

static size_t KASLRAlignment = 0x100000;
vm_offset_t kernel_base;
vm_offset_t kqueue_scan_continue_panic_start_location = 0;
vm_offset_t kqueue_scan_continue_panic_end_location = 0;
char replacement_bytes[10];
char original_bytes[10];
boolean_t interrupt_status = 0;
boolean_t write_protection_status = 0;
uint8_t *kscpb = NULL;

#if __LP64__
#define NUM_SUPPORTED_KERNELS 1
#else
#define NUM_SUPPORTED_KERNELS 1
#endif

// Taken from Hopper
static vm_offset_t possible_kqueue_scan_continue_panic_start_locations[NUM_SUPPORTED_KERNELS] = {
#if __LP64__
        0xffffff80005369ef, //10.7.5 11G63, xnu-1699.32.7
#else
        0x005566fb,         //10.7.5 11G63, xnu-1699.32.7 (32 bit)
#endif
};
static vm_offset_t possible_kqueue_scan_continue_panic_end_locations[NUM_SUPPORTED_KERNELS] = {
#if __LP64__
        0xffffff80005369f9, //10.7.5 11G63, xnu-1699.32.7
#else
        0x00556704,         //10.7.5 11G63, xnu-1699.32.7 (32 bit)
#endif
};

static char possible_search_bytes[NUM_SUPPORTED_KERNELS][4] = {
#if __LP64__
        //10.7.5 11G63, xnu-1699.32.7
        {
                0x42, 0x8A, 0x0C, 0x38, //mov   cl, byte [ds:rax+r15]
        },
#else
        //10.7.5 11G63, xnu-1699.32.7 (32 bit)
        {
                0x89, 0x4c, 0x24, 0x04, //mov   cl, byte [ds:eax+esi]
        },
#endif
};

//static char possible_replacement_bytes[NUM_SUPPORTED_KERNELS][4] = {
//        {0x00, 0x00, 0x00, 0x00},                        // jmp [abs] (gotta figure out address of deadspace
//};
//
boolean_t write_protection_is_enabled() {
        return (get_cr0() & CR0_WP) != 0;
}

static IOReturn disableInterruptsAndProtection(boolean_t interrupts_were_enabled, boolean_t write_protection_was_enabled) {
        IOReturn retVal;
        if (interrupts_were_enabled) {
                ml_set_interrupts_enabled(false);
                
                if (! ml_get_interrupts_enabled()) {
                        printf("KQueueScanContinuePatch: Disabled interrupts\n");
                        retVal =  KERN_SUCCESS;
                } else {
                        printf("KQueueScanContinuePatch: Failed to disable interrupts\n");
                        retVal = KERN_FAILURE;
                }
        }
        
        if (write_protection_was_enabled) {
                set_cr0(get_cr0() & ~CR0_WP); // disable write protection
                
                if (!write_protection_is_enabled()) {
                        printf("KQueueScanContinuePatch: Disabled write protection\n");
                        retVal = KERN_SUCCESS;
                } else {
                        printf("KQueueScanContinuePatch: Failed to disable write protection\n");
                        
                        //Re-enable interrupts before exiting.
                        if (interrupts_were_enabled && !ml_get_interrupts_enabled()) {
                                ml_set_interrupts_enabled(true);
                                
                                if (ml_get_interrupts_enabled()) {
                                        printf("KQueueScanContinuePatch: Re-enabled interrupts after failing to disable write protection.\n");
                                } else {
                                        panic("KQueueScanContinuePatch: Failed to re-enable interrupts after failing to disable write protection!\n");
                                }
                        }
                        
                        retVal = KERN_FAILURE;
                }
        }
        return retVal;
}

static void enableInterruptsAndProtection(boolean_t interrupts_were_enabled, boolean_t write_protection_was_enabled) {
        
        if (write_protection_was_enabled && !write_protection_is_enabled()) {
                set_cr0(get_cr0() | CR0_WP); // re-enable write protection
                
                if (write_protection_is_enabled()) {
                        printf("KQueueScanContinuePatch: Re-enabled write protection\n");
                } else {
                        panic("KQueueScanContinuePatch: failed to re-enable write protection!\n");
                }
        }
        
        if (interrupts_were_enabled && !ml_get_interrupts_enabled()) {
                ml_set_interrupts_enabled(true);
                
                if (ml_get_interrupts_enabled()) {
                        printf("KQueueScanContinuePatch: Re-enabled interrupts\n");
                } else {
                        panic("KQueueScanContinuePatch: Failed to re-enable interrupts!\n");
                }
        }
}

// Adapted from github.com/acidanthera/Lilu/blob/137b4d9deb7022bb97fa9899303934534ff20ec7/Lilu/Sources/kern_mach.cpp
static vm_offset_t get_kernel_base() {
#if __LP64__
        // The function choice is mostly arbitrary, but IOLog frequently has a low address.
        vm_offset_t tmp = (vm_offset_t)(IOLog);
        
        // Align the address
        tmp &= ~(KASLRAlignment - 1);
        
        // Search backwards for the kernel base address (mach-o header)
        while (tmp >= KASLRAlignment) {
                if (*(uint32_t *)(tmp) == MH_MAGIC_64) {
                        // make sure it's the header and not some reference to the MAGIC number
                        struct segment_command_64 segmentCommand = *(struct segment_command_64 *)(tmp + sizeof(struct mach_header_64));
                        if (!strncmp(segmentCommand.segname, "__TEXT", sizeof(segmentCommand.segname))) {
                                IOLog("KQueueScanContinuePatch: found kernel mach-o header address at %lx\n", tmp);
                                break;
                        }
                }
                tmp -= KASLRAlignment;
        }
        return tmp - KERN_TEXT_BASE;
#else
        // No 32-bit XNU kernels have KASLR
        return 0;
#endif
}


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
        IOLog("Success\n");
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
                        IOLog("KQueueScanContinuePatch: Memory region not found. You are probably using an unsupported kernel, or your kernel has already been patched.\n");
                        return KERN_FAILURE;
                }
        }
        
        IOLog("KQueueScanContinuePatch: Pre-Patch: Bytes at kqueue_scan_continue panic location: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", kscpb[0], kscpb[1], kscpb[2], kscpb[3], kscpb[4], kscpb[5], kscpb[6], kscpb[7], kscpb[8], kscpb[9], kscpb[10], kscpb[11], kscpb[12], kscpb[13], kscpb[14], kscpb[15], kscpb[16], kscpb[17], kscpb[18], kscpb[19], kscpb[20], kscpb[21], kscpb[22], kscpb[23], kscpb[24], kscpb[25], kscpb[26], kscpb[27], kscpb[28], kscpb[29], kscpb[30], kscpb[31], kscpb[32], kscpb[33], kscpb[34], kscpb[35], kscpb[36], kscpb[37], kscpb[38], kscpb[39]);
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
        IOLog("KQueueScanContinuePatch: Jumping to Dummy function\n");
        long long funcAddr = (long long) &dummyFuncLion;
        IOLog("KQueueScanContinuePatch: funcAddr: %llx\n", funcAddr);
        IOLog("KQueueScanContinuePatch: current %llx\n", 0xffffff80005369ef);
        long long pcDelta = 0xffffff80005369ef - funcAddr;
        IOLog("Value is %llx\n", pcDelta);
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
        
        
        IOLog("KQueueScanContinuePatch: Post-Patch: Bytes at kqueue_scan_continue panic location: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", kscpb[0], kscpb[1], kscpb[2], kscpb[3], kscpb[4], kscpb[5], kscpb[6], kscpb[7], kscpb[8], kscpb[9], kscpb[10], kscpb[11], kscpb[12], kscpb[13], kscpb[14], kscpb[15], kscpb[16], kscpb[17], kscpb[18], kscpb[19], kscpb[20], kscpb[21], kscpb[22], kscpb[23], kscpb[24], kscpb[25], kscpb[26], kscpb[27], kscpb[28], kscpb[29], kscpb[30], kscpb[31], kscpb[32], kscpb[33], kscpb[34], kscpb[35], kscpb[36], kscpb[37], kscpb[38], kscpb[39]);
        
        return KERN_SUCCESS;
}

kern_return_t injectCheckPre108_stop(kmod_info_t *ki, void *d)
{
        IOLog("KQueueScanContinuePatch STOP\n");
        //should write back the old shit.
        disableInterruptsAndProtection(interrupt_status, write_protection_status);
        memcpy((void *)kqueue_scan_continue_panic_start_location, &original_bytes, sizeof(original_bytes));
        enableInterruptsAndProtection(interrupt_status, write_protection_status);
        IOLog("KQueueScanContinuePatch: UNLOAD: Bytes at kqueue_scan_continue panic location: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", kscpb[0], kscpb[1], kscpb[2], kscpb[3], kscpb[4], kscpb[5], kscpb[6], kscpb[7], kscpb[8], kscpb[9], kscpb[10], kscpb[11], kscpb[12], kscpb[13], kscpb[14], kscpb[15], kscpb[16], kscpb[17], kscpb[18], kscpb[19], kscpb[20], kscpb[21], kscpb[22], kscpb[23], kscpb[24], kscpb[25], kscpb[26], kscpb[27], kscpb[28], kscpb[29], kscpb[30], kscpb[31], kscpb[32], kscpb[33], kscpb[34], kscpb[35], kscpb[36], kscpb[37], kscpb[38], kscpb[39]);
        
        //__asm__("jmp         0xffffff8000536a12");
        return KERN_SUCCESS;
}
