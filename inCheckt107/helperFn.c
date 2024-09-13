static boolean_t write_protection_is_enabled() {
        return (get_cr0() & CR0_WP) != 0;
}

static IOReturn disableInterruptsAndProtection(boolean_t interrupts_were_enabled, boolean_t write_protection_was_enabled) {
        IOReturn retVal;
        if (interrupts_were_enabled) {
                ml_set_interrupts_enabled(false);
                
                if (! ml_get_interrupts_enabled()) {
                        printf("YellowTrampoline::%s: Disabled interrupts\n", __func__);
                        retVal =  KERN_SUCCESS;
                } else {
                        printf("YellowTrampoline::%s: Failed to disable interrupts\n", __func__);
                        retVal = KERN_FAILURE;
                }
        }
        
        if (write_protection_was_enabled) {
                set_cr0(get_cr0() & ~CR0_WP); // disable write protection
                
                if (!write_protection_is_enabled()) {
                        printf("YellowTrampoline::%s: Disabled write protection\n", __func__);
                        retVal = KERN_SUCCESS;
                } else {
                        printf("YellowTrampoline::%s: Failed to disable write protection\n", __func__);
                        
                        //Re-enable interrupts before exiting.
                        if (interrupts_were_enabled && !ml_get_interrupts_enabled()) {
                                ml_set_interrupts_enabled(true);
                                
                                if (ml_get_interrupts_enabled()) {
                                        printf("YellowTrampoline::%s: Re-enabled interrupts after failing to disable write protection.\n", __func__);
                                } else {
                                        panic("YellowTrampoline::%s: Failed to re-enable interrupts after failing to disable write protection!\n", __func__);
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
                        printf("YellowTrampoline::%s: Re-enabled write protection\n", __func__);
                } else {
                        panic("YellowTrampoline::%s: failed to re-enable write protection!\n", __func__);
                }
        }
        
        if (interrupts_were_enabled && !ml_get_interrupts_enabled()) {
                ml_set_interrupts_enabled(true);
                
                if (ml_get_interrupts_enabled()) {
                        printf("YellowTrampoline::%s: Re-enabled interrupts\n", __func__);
                } else {
                        panic("YellowTrampoline::%s: Failed to re-enable interrupts!\n", __func__);
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
#ifdef DEBUG
                                IOLog("YellowTrampoline::%s: found kernel mach-o header address at %lx\n", __func__, tmp);
#endif
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

static void injectInstructions(void);
static void TheLadyIsAVamp (long long VampsHomePhone, char *where) {
        //The TrampsHomePhone is the starting address of the Tramp
        //just an easy way to print the contents
        long long byteCount = 0;
        uint8_t *matchOpCodeBytes =  (uint8_t*) &injectInstructions;
        for (int k=0; k<64;k++) {
                IOLog("YellowTrampoline::%s:: %s %llx %02x\n", __func__, where, VampsHomePhone + byteCount, matchOpCodeBytes[k]);
                byteCount += 1;
                
        }
}
