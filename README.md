# an injector for firefox on lion and lower

people:
using johnny's (@wowfunhappy) gist: https://gist.github.com/Wowfunhappy/8212f5bea4c601ac9a6297789f232321 and also 

also got a lot of help from krack krack krack (@krackers) wherever he at.

time to start pestering "themasta" aka the @mstange

proof:

https://github.com/user-attachments/assets/bf5f03df-6c25-4daa-81dc-f82b919d832a

purpose:

for some reason or another, firefox crashes on 10.7 but not 10.8 and above.

after enduring a lot of problems with the 10.7 KernelDebugKit in VMWare (which doesn't work in VMWare fusion > 6.x),
ol' limpy (my now-retired 2012 MacPro) once-again proved why there is no substitute for a wiley veteran with its 10.7 support.

i booted into a native 10.7 environment, used vmware fusion 6.x, loaded the kernel debug kit (still mostly broken--panic dump/log server doesn't work)
and managed to get what i needed via kdp-remote, though i would have much preferred the core dumps via the server mechanism.

i found it was crashing somewhere in fdexec, and it the register was holding null:
![debugginandshit](https://github.com/user-attachments/assets/294c9537-01cc-4539-ab95-947bc4bd4927)

so i did a bit of bisect-ual healing (bisect-ual) between xnu-1699.32.7~1 (10.7.5) and 2050.79 (10.8.0) and found that fdexec now had a 
null check for the file pointer:
- https://github.com/apple/darwin-xnu/blame/8e385d87fd58065e798611d8362b83ce695b78dc/bsd/kern/kern_descrip.c#L4126

so johnny (@wowfunhappy) pointed me to a few links that made it easy for me to build and test a kernel with this "small" modification.
        -the kernel no longer panicked with this "small" change
        
then @krackers helped me figure the rest out, using johnny's kext that patches (a few) kernels for chromium for a very different issue.
- https://github.com/blueboxd/chromium-legacy/issues/44
- https://gist.github.com/Wowfunhappy/8212f5bea4c601ac9a6297789f232321

this kext is a product of all of that, i guess.

now we have a nice/clean injector, i have to figure out how to package it tidily and only activate it on sub 10.8 boxes :>

