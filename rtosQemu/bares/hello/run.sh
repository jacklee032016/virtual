# /bin/bash

# -kernel: start VM at address of 0x10000
/root/qemu/build/arm-softmmu/qemu-system-arm -machine versatileab -nographic -kernel init.bin

