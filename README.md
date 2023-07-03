# Interned Logs
This demo shows how to intern logs into unique IDs at link time by turning the log messages into
byte-addressible symbol names overlayed at address 0x0000. Since the ELF header bits to the special
`.logs` section are set to NOALLOC (adding the "(NOTES)" section attribute), the symbols are not
loaded.

## Demo
The first column is the symbol name (the log message) and the second is its address, the ID.

```sh
❯ make run
arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o
arm-none-eabi-ld -T linker.ld main.o startup.o -o main.elf
arm-none-eabi-ld: warning: main.elf has a LOAD segment with RWX permissions
arm-none-eabi-objcopy -O binary main.elf main.bin
arm-none-eabi-nm -f sysv main.elf | rg '.logs$' | rg -v '__log_\d+_label'
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Nunc non blandit massa enim nec. Vel orci porta non pulvinar neque laoreet suspendisse interdum. Integer quis auctor elit sed vulputate mi sit amet. Sed vulputate mi sit amet mauris commodo quis imperdiet massa. Nisl pretium fusce id velit ut tortor pretium viverra suspendisse. Faucibus turpis in eu mi bibendum neque. Semper risus in hendrerit gravida rutrum. Velit laoreet id donec ultrices tincidunt. Diam volutpat commodo sed egestas egestas fringilla. Lectus sit amet est placerat in egestas erat. Tincidunt lobortis feugiat vivamus at augue. Pellentesque habitant morbi tristique senectus et netus et. Id neque aliquam vestibulum morbi. Sit amet dictum sit amet justo donec enim diam vulputate. Volutpat lacus laoreet non curabitur gravida arcu ac tortor. Semper auctor neque vitae tempus quam. Ullamcorper eget nulla facilisi etiam dignissim diam quis enim. Purus sit amet volutpat consequat. Vulputate enim nulla aliquet porttitor lacus luctus. Commodo elit at imperdiet dui accumsan sit amet. Enim nulla aliquet porttitor lacus. Est velit egestas dui id ornare arcu. Netus et malesuada fames ac turpis. Vel orci porta non pulvinar neque. Aliquet eget sit amet tellus cras adipiscing enim eu. Phasellus vestibulum lorem sed risus ultricies tristique. In fermentum posuere urna nec tincidunt praesent. Amet nulla facilisi morbi tempus iaculis urna id volutpat lacus. Aenean et tortor at risus. Ac turpis egestas maecenas pharetra convallis posuere. Nulla aliquet porttitor lacus luctus accumsan tortor posuere. Varius morbi enim nunc faucibus a pellentesque sit amet. Lectus mauris ultrices eros in. Tellus mauris a diam maecenas sed enim ut sem. Dui id ornare arcu odio ut sem nulla pharetra diam. Nisi vitae suscipit tellus mauris a diam maecenas. Imperdiet proin fermentum leo vel orci porta non. Metus vulputate eu scelerisque felis imperdiet proin. Mi tempus imperdiet nulla malesuada pellentesque elit eget. Sit amet nisl purus in. Lectus quam id leo in vitae. Nullam eget felis eget nunc lobortis mattis. Malesuada nunc vel risus commodo viverra. Auctor augue mauris augue neque gravida in fermentum et sollicitudin. Aliquet risus feugiat in ante metus dictum at tempor. Quam quisque id diam vel quam elementum pulvinar etiam non. Commodo sed egestas egestas fringilla phasellus faucibus. Habitant morbi tristique senectus et netus et. Sollicitudin tempor id eu nisl. Augue eget arcu dictum varius. Ornare quam viverra orci sagittis eu volutpat odio. Nisi quis eleifend quam adipiscing vitae. Vulputate mi sit amet mauris commodo quis imperdiet. Sed arcu non odio euismod lacinia. Dignissim enim sit amet venenatis urna cursus eget nunc. Dui vivamus arcu felis bibendum ut tristique et. Aliquet porttitor lacus luctus accumsan tortor posuere ac. Nunc sed id semper risus in.
                    |00000003|   n  |            NOTYPE|00000001|     |.logs
foo bar             |00000001|   n  |            NOTYPE|00000001|     |.logs
hello world         |00000000|   n  |            NOTYPE|00000001|     |.logs
test                |00000002|   n  |            NOTYPE|00000001|     |.logs
qemu-system-arm -M versatilepb -m 128M -nographic -semihosting  -kernel main.bin
Hello world!
0x123456
log_id=0x0
log_id=0x1
log_id=0x2
log_id=0x3
<c-a>x
QEMU: Terminated
```

## Reference
1. [Hello world for bare metal ARM using QEMU - Jason's Blog](https://jasonblog.github.io/note/arm_emulation/hello_world_for_bare_metal_arm_using_qemu.html)
2. Better explanation: [Logging with Symbols - Rust Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html)
