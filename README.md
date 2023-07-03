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

The strings nor byte variables are included in the final binary.

```
00010180 <main>:

#define LOG(msg) \
    __LOG(msg, CONCAT(__log_, __COUNTER__))


int main() {
   10180:	e92d4800 	push	{fp, lr}
   10184:	e28db004 	add	fp, sp, #4
    print_uart_str("Hello world!\n");
   10188:	e59f0128 	ldr	r0, [pc, #296]	@ 102b8 <main+0x138>
   1018c:	ebffffe5 	bl	10128 <print_uart_str>
    print_uart_uint(0x123456);
   10190:	e59f0124 	ldr	r0, [pc, #292]	@ 102bc <main+0x13c>
   10194:	ebffff9d 	bl	10010 <print_uart_uint>
    LOG("hello world");
   10198:	e59f3120 	ldr	r3, [pc, #288]	@ 102c0 <main+0x140>
   1019c:	e5933000 	ldr	r3, [r3]
   101a0:	e59f211c 	ldr	r2, [pc, #284]	@ 102c4 <main+0x144>
   101a4:	e5922000 	ldr	r2, [r2]
   101a8:	e59f1118 	ldr	r1, [pc, #280]	@ 102c8 <main+0x148>
   101ac:	e7812103 	str	r2, [r1, r3, lsl #2]
   101b0:	e59f3108 	ldr	r3, [pc, #264]	@ 102c0 <main+0x140>
   101b4:	e5933000 	ldr	r3, [r3]
   101b8:	e2833001 	add	r3, r3, #1
   101bc:	e59f20fc 	ldr	r2, [pc, #252]	@ 102c0 <main+0x140>
   101c0:	e5823000 	str	r3, [r2]
   101c4:	e59f0100 	ldr	r0, [pc, #256]	@ 102cc <main+0x14c>
   101c8:	ebffffd6 	bl	10128 <print_uart_str>
   101cc:	e59f30f0 	ldr	r3, [pc, #240]	@ 102c4 <main+0x144>
   101d0:	e5933000 	ldr	r3, [r3]
   101d4:	e1a00003 	mov	r0, r3
   101d8:	ebffff8c 	bl	10010 <print_uart_uint>
    LOG("foo bar");
   101dc:	e59f30dc 	ldr	r3, [pc, #220]	@ 102c0 <main+0x140>
   101e0:	e5933000 	ldr	r3, [r3]
   101e4:	e59f20e4 	ldr	r2, [pc, #228]	@ 102d0 <main+0x150>
   101e8:	e5922000 	ldr	r2, [r2]
   101ec:	e59f10d4 	ldr	r1, [pc, #212]	@ 102c8 <main+0x148>
   101f0:	e7812103 	str	r2, [r1, r3, lsl #2]
   101f4:	e59f30c4 	ldr	r3, [pc, #196]	@ 102c0 <main+0x140>
   101f8:	e5933000 	ldr	r3, [r3]
   101fc:	e2833001 	add	r3, r3, #1
   10200:	e59f20b8 	ldr	r2, [pc, #184]	@ 102c0 <main+0x140>
   10204:	e5823000 	str	r3, [r2]
   10208:	e59f00bc 	ldr	r0, [pc, #188]	@ 102cc <main+0x14c>
   1020c:	ebffffc5 	bl	10128 <print_uart_str>
   10210:	e59f30b8 	ldr	r3, [pc, #184]	@ 102d0 <main+0x150>
   10214:	e5933000 	ldr	r3, [r3]
   10218:	e1a00003 	mov	r0, r3
   1021c:	ebffff7b 	bl	10010 <print_uart_uint>
    LOG("test");
   10220:	e59f3098 	ldr	r3, [pc, #152]	@ 102c0 <main+0x140>
   10224:	e5933000 	ldr	r3, [r3]
   10228:	e59f20a4 	ldr	r2, [pc, #164]	@ 102d4 <main+0x154>
   1022c:	e5922000 	ldr	r2, [r2]
   10230:	e59f1090 	ldr	r1, [pc, #144]	@ 102c8 <main+0x148>
   10234:	e7812103 	str	r2, [r1, r3, lsl #2]
   10238:	e59f3080 	ldr	r3, [pc, #128]	@ 102c0 <main+0x140>
   1023c:	e5933000 	ldr	r3, [r3]
   10240:	e2833001 	add	r3, r3, #1
   10244:	e59f2074 	ldr	r2, [pc, #116]	@ 102c0 <main+0x140>
   10248:	e5823000 	str	r3, [r2]
   1024c:	e59f0078 	ldr	r0, [pc, #120]	@ 102cc <main+0x14c>
   10250:	ebffffb4 	bl	10128 <print_uart_str>
   10254:	e59f3078 	ldr	r3, [pc, #120]	@ 102d4 <main+0x154>
   10258:	e5933000 	ldr	r3, [r3]
   1025c:	e1a00003 	mov	r0, r3
   10260:	ebffff6a 	bl	10010 <print_uart_uint>
    LOG("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Nunc non blandit massa enim nec. Vel orci porta non pulvinar neque laoreet suspendisse interdum. Integer quis auctor elit sed vulputate mi sit amet. Sed vulputate mi sit amet mauris commodo quis imperdiet massa. Nisl pretium fusce id velit ut tortor pretium viverra suspendisse. Faucibus turpis in eu mi bibendum neque. Semper risus in hendrerit gravida rutrum. Velit laoreet id donec ultrices tincidunt. Diam volutpat commodo sed egestas egestas fringilla. Lectus sit amet est placerat in egestas erat. Tincidunt lobortis feugiat vivamus at augue. Pellentesque habitant morbi tristique senectus et netus et. Id neque aliquam vestibulum morbi. Sit amet dictum sit amet justo donec enim diam vulputate. Volutpat lacus laoreet non curabitur gravida arcu ac tortor. Semper auctor neque vitae tempus quam. Ullamcorper eget nulla facilisi etiam dignissim diam quis enim. Purus sit amet volutpat consequat. Vulputate enim nulla aliquet porttitor lacus luctus. Commodo elit at imperdiet dui accumsan sit amet. Enim nulla aliquet porttitor lacus. Est velit egestas dui id ornare arcu. Netus et malesuada fames ac turpis. Vel orci porta non pulvinar neque. Aliquet eget sit amet tellus cras adipiscing enim eu. Phasellus vestibulum lorem sed risus ultricies tristique. In fermentum posuere urna nec tincidunt praesent. Amet nulla facilisi morbi tempus iaculis urna id volutpat lacus. Aenean et tortor at risus. Ac turpis egestas maecenas pharetra convallis posuere. Nulla aliquet porttitor lacus luctus accumsan tortor posuere. Varius morbi enim nunc faucibus a pellentesque sit amet. Lectus mauris ultrices eros in. Tellus mauris a diam maecenas sed enim ut sem. Dui id ornare arcu odio ut sem nulla pharetra diam. Nisi vitae suscipit tellus mauris a diam maecenas. Imperdiet proin fermentum leo vel orci porta non. Metus vulputate eu scelerisque felis imperdiet proin. Mi tempus imperdiet nulla malesuada pellentesque elit eget. Sit amet nisl purus in. Lectus quam id leo in vitae. Nullam eget felis eget nunc lobortis mattis. Malesuada nunc vel risus commodo viverra. Auctor augue mauris augue neque gravida in fermentum et sollicitudin. Aliquet risus feugiat in ante metus dictum at tempor. Quam quisque id diam vel quam elementum pulvinar etiam non. Commodo sed egestas egestas fringilla phasellus faucibus. Habitant morbi tristique senectus et netus et. Sollicitudin tempor id eu nisl. Augue eget arcu dictum varius. Ornare quam viverra orci sagittis eu volutpat odio. Nisi quis eleifend quam adipiscing vitae. Vulputate mi sit amet mauris commodo quis imperdiet. Sed arcu non odio euismod lacinia. Dignissim enim sit amet venenatis urna cursus eget nunc. Dui vivamus arcu felis bibendum ut tristique et. Aliquet porttitor lacus luctus accumsan tortor posuere ac. Nunc sed id semper risus in.");
   10264:	e59f3054 	ldr	r3, [pc, #84]	@ 102c0 <main+0x140>
   10268:	e5933000 	ldr	r3, [r3]
   1026c:	e59f2064 	ldr	r2, [pc, #100]	@ 102d8 <main+0x158>
   10270:	e5922000 	ldr	r2, [r2]
   10274:	e59f104c 	ldr	r1, [pc, #76]	@ 102c8 <main+0x148>
   10278:	e7812103 	str	r2, [r1, r3, lsl #2]
   1027c:	e59f303c 	ldr	r3, [pc, #60]	@ 102c0 <main+0x140>
   10280:	e5933000 	ldr	r3, [r3]
   10284:	e2833001 	add	r3, r3, #1
   10288:	e59f2030 	ldr	r2, [pc, #48]	@ 102c0 <main+0x140>
   1028c:	e5823000 	str	r3, [r2]
   10290:	e59f0034 	ldr	r0, [pc, #52]	@ 102cc <main+0x14c>
   10294:	ebffffa3 	bl	10128 <print_uart_str>
   10298:	e59f3038 	ldr	r3, [pc, #56]	@ 102d8 <main+0x158>
   1029c:	e5933000 	ldr	r3, [r3]
   102a0:	e1a00003 	mov	r0, r3
   102a4:	ebffff59 	bl	10010 <print_uart_uint>
    return (int)(logs[2]);
   102a8:	e59f3018 	ldr	r3, [pc, #24]	@ 102c8 <main+0x148>
   102ac:	e5933008 	ldr	r3, [r3, #8]
}
   102b0:	e1a00003 	mov	r0, r3
   102b4:	e8bd8800 	pop	{fp, pc}
   102b8:	000102e0 	andeq	r0, r1, r0, ror #5
   102bc:	00123456 	andseq	r3, r2, r6, asr r4
   102c0:	00010348 	andeq	r0, r1, r8, asr #6
   102c4:	000102f8 	strdeq	r0, [r1], -r8
   102c8:	00010308 	andeq	r0, r1, r8, lsl #6
   102cc:	000102f0 	strdeq	r0, [r1], -r0	@ <UNPREDICTABLE>
   102d0:	000102fc 	strdeq	r0, [r1], -ip
   102d4:	00010300 	andeq	r0, r1, r0, lsl #6
   102d8:	00010304 	andeq	r0, r1, r4, lsl #6
```
```sh
xxd main.bin
00000000: 04d0 9fe5 5d00 00eb feff ffea 5013 0100  ....].......P...
00000010: 04b0 2de5 00b0 8de2 1cd0 4de2 1800 0be5  ..-.......M.....
00000020: 0130 a0e3 0830 0be5 1830 1be5 2332 a0e1  .0...0...0..#2..
00000030: 0c30 0be5 0500 00ea 0830 1be5 0130 83e2  .0.......0...0..
00000040: 0830 0be5 0c30 1be5 2332 a0e1 0c30 0be5  .0...0..#2...0..
00000050: 0c30 1be5 0000 53e3 f6ff ff1a c030 9fe5  .0....S......0..
00000060: 3020 a0e3 0020 83e5 b430 9fe5 7820 a0e3  0 ... ...0..x ..
00000070: 0020 83e5 1300 00ea 0830 1be5 0130 43e2  . .......0...0C.
00000080: 0331 a0e1 1820 1be5 3233 a0e1 0f30 03e2  .1... ..23...0..
00000090: 1430 0be5 1430 1be5 0900 53e3 0100 008a  .0...0....S.....
000000a0: 3020 a0e3 0000 00ea 5720 a0e3 7010 9fe5  0 ......W ..p...
000000b0: 1430 1be5 0330 82e0 0030 81e5 0830 1be5  .0...0...0...0..
000000c0: 0130 43e2 0830 0be5 0830 1be5 0100 53e3  .0C..0...0....S.
000000d0: e8ff ffca 1830 1be5 0f30 03e2 1030 0be5  .....0...0...0..
000000e0: 1030 1be5 0900 53e3 0100 008a 3020 a0e3  .0....S.....0 ..
000000f0: 0000 00ea 5720 a0e3 2410 9fe5 1030 1be5  ....W ..$....0..
00000100: 0330 82e0 0030 81e5 1430 9fe5 0a20 a0e3  .0...0...0... ..
00000110: 0020 83e5 0000 a0e1 00d0 8be2 04b0 9de4  . ..............
00000120: 1eff 2fe1 0010 1f10 04b0 2de5 00b0 8de2  ../.......-.....
00000130: 0cd0 4de2 0800 0be5 0600 00ea 0830 1be5  ..M..........0..
00000140: 0020 d3e5 3030 9fe5 0020 83e5 0830 1be5  . ..00... ...0..
00000150: 0130 83e2 0830 0be5 0830 1be5 0030 d3e5  .0...0...0...0..
00000160: 0000 53e3 f4ff ff1a 0000 a0e1 0000 a0e1  ..S.............
00000170: 00d0 8be2 04b0 9de4 1eff 2fe1 0010 1f10  ........../.....
00000180: 0048 2de9 04b0 8de2 2801 9fe5 e5ff ffeb  .H-.....(.......
00000190: 2401 9fe5 9dff ffeb 2031 9fe5 0030 93e5  $....... 1...0..
000001a0: 1c21 9fe5 0020 92e5 1811 9fe5 0321 81e7  .!... .......!..
000001b0: 0831 9fe5 0030 93e5 0130 83e2 fc20 9fe5  .1...0...0... ..
000001c0: 0030 82e5 0001 9fe5 d6ff ffeb f030 9fe5  .0...........0..
000001d0: 0030 93e5 0300 a0e1 8cff ffeb dc30 9fe5  .0...........0..
000001e0: 0030 93e5 e420 9fe5 0020 92e5 d410 9fe5  .0... ... ......
000001f0: 0321 81e7 c430 9fe5 0030 93e5 0130 83e2  .!...0...0...0..
00000200: b820 9fe5 0030 82e5 bc00 9fe5 c5ff ffeb  . ...0..........
00000210: b830 9fe5 0030 93e5 0300 a0e1 7bff ffeb  .0...0......{...
00000220: 9830 9fe5 0030 93e5 a420 9fe5 0020 92e5  .0...0... ... ..
00000230: 9010 9fe5 0321 81e7 8030 9fe5 0030 93e5  .....!...0...0..
00000240: 0130 83e2 7420 9fe5 0030 82e5 7800 9fe5  .0..t ...0..x...
00000250: b4ff ffeb 7830 9fe5 0030 93e5 0300 a0e1  ....x0...0......
00000260: 6aff ffeb 5430 9fe5 0030 93e5 6420 9fe5  j...T0...0..d ..
00000270: 0020 92e5 4c10 9fe5 0321 81e7 3c30 9fe5  . ..L....!..<0..
00000280: 0030 93e5 0130 83e2 3020 9fe5 0030 82e5  .0...0..0 ...0..
00000290: 3400 9fe5 a3ff ffeb 3830 9fe5 0030 93e5  4.......80...0..
000002a0: 0300 a0e1 59ff ffeb 1830 9fe5 0830 93e5  ....Y....0...0..
000002b0: 0300 a0e1 0088 bde8 e002 0100 5634 1200  ............V4..
000002c0: 4803 0100 f802 0100 0803 0100 f002 0100  H...............
000002d0: fc02 0100 0003 0100 0403 0100 0010 1f10  ................
000002e0: 4865 6c6c 6f20 776f 726c 6421 0a00 0000  Hello world!....  < from test print_uart_str, not LOG()
000002f0: 6c6f 675f 6964 3d00 0000 0000 0100 0000  log_id=.........  < from test print_uart_str, not LOG()
00000300: 0200 0000 0300 0000                      ........
```

## Reference
1. [Hello world for bare metal ARM using QEMU - Jason's Blog](https://jasonblog.github.io/note/arm_emulation/hello_world_for_bare_metal_arm_using_qemu.html)
2. Better explanation: [Logging with Symbols - Rust Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html)
