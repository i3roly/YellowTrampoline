//
//  injectCheckPre108.c
//  injectCheckPre108
//
//  Created by Gagan on 2024-09-08.
//  Copyright © 2024 gagz. All rights reserved.
//

#include <mach/mach_types.h>

kern_return_t injectCheckPre108_start(kmod_info_t * ki, void *d);
kern_return_t injectCheckPre108_stop(kmod_info_t *ki, void *d);

kern_return_t injectCheckPre108_start(kmod_info_t * ki, void *d)
{
    return KERN_SUCCESS;
}

kern_return_t injectCheckPre108_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
