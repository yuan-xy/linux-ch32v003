# linux-ch32v003
Linux on a $0.15 RISC-V microcontroller

This project enables the CH32V003 microcontroller to run Linux. It achieves this by using an 8 megabyte SPI PSRAM chip and the [mini-rv32ima RISC-V emulator by cnlohr](https://github.com/cnlohr/mini-rv32ima), packaged into the [tiny-rv32ima library](https://github.com/tvlad1234/tiny-rv32ima). Emulating the system is necessary because the PSRAM cannot be mapped into the address space of the microcontroler. Storage for the Linux kernel, device tree binary and rootfs image is provided by an SD card.

This application is enabled by the [ch32fun](https://github.com/cnlohr/ch32fun) SDK.

## How to use

The PSRAM and SD card are connected to the hardware SPI interface of the CH32V003. The chip select pins are defined in the [pins.h](pins.h) file. The system console is exposed over the UART.

The SD card containing the Linux kernel and rootfs image must be formatted as FAT32 or FAT16, with the necessary files placed in the root directory.

A suggested schematic and corresponding single-layer PCB design is included in the [hardware](hardware) folder (it's a KiCad 7 project).

Boot time is around 7 minutes. The provided Linux image includes the coremark benchmark.

## 增加了Flash的支持
你可以用一个W25Qxx系列芯片替换sd card。
可以下下面的命令生成一个文件系统镜像：
```
# 1. 创建8MB空镜像（8*1024*1024=4194304字节，匹配W25Q64容量）
dd if=/dev/zero of=flash_image.bin bs=1M count=8

# 2. 格式化为FAT12（指定扇区大小512字节，兼容多数Flash驱动）
mkfs.vfat -F 12 -S 512 flash_image.bin

# 3. 向镜像添加文件（根目录）
wget https://github.com/tvlad1234/buildroot-tiny-rv32ima/releases/download/v1.0/images.zip
unzip images.zip
mcopy -i flash_image.bin ./Image ::/
mcopy -i flash_image.bin ./dtb ::/DTB
resize2fs ./rootfs 4M
truncate -s 4M ./rootfs
mcopy -i flash_image.bin ./rootfs ::/ROOTFS

由于rootfs是为sdcard准备的，有64M，大于常规的w25qxx系列芯片的flash大小，所以上面把rootfs更改为4M。

# 4. 验证镜像文件
mdir -i flash_image.bin ::/

# 5. 将镜像文件烧录到Flash中


```

    注意：默认make编译的是sdcard版本，要使用flash版本，执行make SPI_FLASH=1


但是首次启动还是报错（可能是rootfs被处理过导致的？对比正常的启动流程，是“devtmpfs: initialized”这一行未正确打印出来。但是在pc上跑模拟器是正常的）。换了一个flash芯片再次测试，结果正常了。看来代码没问题。
```
Starting RISC-V VM


[    0.000000] Linux version 6.6.18tiny-rv32ima (vlad@turboencabulator) (riscv32-buildroot-linux-uclibc-gcc.br_real (Buildroot -gad36342) 13.3.0, GNU ld (GNU Binutils) 2.41) #1 Wed Aug 27 12:46:35 EEST 2025
[    0.000000] Machine model: riscv-minimal-nommu,qemu
[    0.000000] Zone ranges:
[    0.000000]   Normal   [mem 0x0000000080000000-0x00000000807fefff]
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000080000000-0x00000000807fefff]
[    0.000000] Initmem setup node 0 [mem 0x0000000080000000-0x00000000807fefff]
[    0.000000] riscv: base ISA extensions 
[    0.000000] riscv: ELF capabilities 
[    0.000000] Kernel command line: console=hvc0 root=fe00
[    0.000000] Dentry cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.000000] Inode-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.000000] Built 1 zonelists, mobility grouping off.  Total pages: 2031
[    0.000000] mem auto-init: stack:off, heap alloc:off, heap free:off
[    0.000000] Memory: 5880K/8188K available (1721K kernel code, 74K rwdata, 257K rodata, 94K init, 37K bss, 2308K reserved, 0K cma-reserved)
[    0.000000] SLUB: HWalign=64, Order=0-1, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] NR_IRQS: 64, nr_irqs: 64, preallocated irqs: 0
[    0.000000] riscv-intc: 32 local interrupts mapped
[    0.000000] clint: clint@11000000: timer running at 1000000 Hz
[    0.000000] clocksource: clint_clocksource: mask: 0xffffffffffffffff max_cycles: 0x1d854df40, max_idle_ns: 3526361616960 ns
[    0.000000] sched_clock: 64 bits at 1000kHz, resolution 1000ns, wraps every 2199023255500ns
[    0.083100] Console: colour dummy device 80x25
[    0.088950] printk: console [hvc0] enabled
[    0.288638] Calibrating delay loop (skipped), value calculated using timer frequency.. 2.00 BogoMIPS (lpj=10000)
[    0.317963] pid_max: default: 4096 minimum: 301
[    0.367125] Mount-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.390000] Mountpoint-cache hash table entries: 1024 (order: 0, 4096 bytes, linear)
[    0.642963] Oops - store (or AMO) access fault [#1]
[    0.642963] Modules linked in:
[    0.650550] CPU: 0 PID: 2 Comm: kthreadd Not tainted 6.6.18tiny-rv32ima #1
[    0.658238] Hardware name: riscv-minimal-nommu,qemu (DT)
[    0.665588] epc : 0x80039cc0
[    0.665588]  ra : 0x801af5ac
[    0.672875] epc : 80039cc0 ra : 801af5ac sp : 8041fe20
[    0.680138]  gp : 8021a2c0 tp : 804165c0 t0 : 9aca0000
[    0.687075]  t1 : 8000ac38 t2 : 00000000 s0 : 80407a04
[    0.694075]  s1 : 0000ffff a0 : 8021c444 a1 : 80039c7c
[    0.701150]  a2 : 80407a04 a3 : 80407a04 a4 : 00001888
[    0.708150]  a5 : 80211728 a6 : 8021c420 a7 : 98968000
[    0.715213]  s2 : 8020e1d0 s3 : 8021c420 s4 : 8020d000
[    0.722250]  s5 : 0000000a s6 : 00000024 s7 : 00000100
[    0.729325]  s8 : 8021c444 s9 : 00000200 s10: 0000000a
[    0.736825]  s11: 00000024 t3 : ffffffff t4 : 00000000
[    0.744238]  t5 : 00000013 t6 : 0000003b
[    0.751813] status: 00001800 badaddr: 0000003f cause: 00000007
[    0.759388] [<80039cc0>] 0x80039cc0
[    0.766625] [<801af5a8>] 0x801af5a8
[    0.773850] [<8000f178>] 0x8000f178
[    0.780650] [<8000a818>] 0x8000a818
[    0.780650] [<8000f178>] 0x8000f178
[    0.788138] [<801aa98c>] 0x801aa98c
[    0.795250] [<801af2c0>] 0x801af2c0
[    0.802425] [<8000f178>] 0x8000f178
[    0.809738] [<8001dd80>] 0x8001dd80
[    0.816788] [<80084a58>] 0x80084a58
[    0.823475] [<8001dd80>] 0x8001dd80
[    0.830563] [<8001dd38>] 0x8001dd38
[    0.837600] [<801af388>] 0x801af388
[    0.859788] Code: a023 00c7 a023 0006 a603 0087 a683 0047 0463 08d6 (a223) 00ff 
[    0.859788] ---[ end trace 0000000000000000 ]---
[    0.866738] Kernel panic - not syncing: Fatal exception in interrupt
[    1.049463] ---[ end Kernel panic - not syncing: Fatal exception in interrupt ]---
```

### 尝试mini-rv32ima的内核版本

    git clone https://github.com/cnlohr/mini-rv32ima.git
    mcopy -i flash_image.bin mini-rv32ima/windows/Image ::/
    mcopy -i flash_image.bin mini-rv32ima/mini-rv32ima/sixtyfourmb.dtb ::/DTB

运行报错（大概是因为使用ramfs后，8MB的psram内存空间不够）：
```
[  100.089887] Run /init as init process

[  100.543400] init: page allocation failure: order:8, mode:0xcc0(GFP_KERNEL), nodemask=(null)
[  100.580187] CPU: 0 PID: 1 Comm: init Not tainted 6.1.14 #4
[  100.601537] Hardware name: riscv-minimal-nommu,qemu (DT)
[  100.615375] Call Trace:
[  100.631287] [<80003520>] 0x80003520
[  100.645275] [<8016db6c>] 0x8016db6c
[  100.660012] [<8017051c>] 0x8017051c
[  100.680787] [<80170554>] 0x80170554
[  100.695250] [<800778a0>] 0x800778a0
[  100.709625] [<80077cf0>] 0x80077cf0
[  100.730162] [<800781f4>] 0x800781f4
[  100.745075] [<80078550>] 0x80078550
[  100.766175] [<80078a0c>] 0x80078a0c
[  100.780137] [<8007320c>] 0x8007320c
[  100.794887] [<80069f1c>] 0x80069f1c
[  100.815662] [<80069fa4>] 0x80069fa4
[  100.830162] [<800be168>] 0x800be168
[  100.844550] [<8008a5d0>] 0x8008a5d0
[  100.865075] [<8008b064>] 0x8008b064
[  100.879975] [<8016da04>] 0x8016da04
[  100.901062] [<801707fc>] 0x801707fc
[  100.915062] [<80001e4c>] 0x80001e4c
[  100.929787] Mem-Info:
[  101.139150] active_anon:0 inactive_anon:0 isolated_anon:0
[  101.139150]  active_file:0 inactive_file:0 isolated_file:0
[  101.139150]  unevictable:525 dirty:0 writeback:0
[  101.139150]  slab_reclaimable:50 slab_unreclaimable:143
[  101.139150]  mapped:0 shmem:0 pagetables:0
[  101.139150]  sec_pagetables:0 bounce:0
[  101.139150]  kernel_misc_reclaimable:0
[  101.139150]  free:738 free_pcp:0 free_cma:0
[  101.267975] Node 0 active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB isolated(anon):0kB isolated(file):0kB mapped:0kB dirty:0kB writeback:0kB shmem:0kB writeback_tmp:0kB kernel_stack:120kB pagetables:0kB sec_pagetables:0kB all_unreclaimable? no
[  101.369562] Normal free:2952kB boost:0kB min:268kB low:332kB high:396kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB writepending:0kB present:8188kB managed:5992kB mlocked:0kB bounce:0kB free_pcp:0kB local_pcp:0kB free_cma:0kB
[  101.463400] lowmem_reserve[]: 0 0
[  101.527475] Normal: 4*4kB (UM) 5*8kB (UM) 3*16kB (M) 3*32kB (UM) 1*64kB (M) 3*128kB (UM) 3*256kB (UM) 3*512kB (UM) 0*1024kB 0*2048kB 0*4096kB = 2952kB
[  101.798725] 532 total pagecache pages
[  101.813187] 2047 pages RAM
[  101.854100] 0 pages HighMem/MovableOnly
[  101.885587] 549 pages reserved
[  101.901850] nommu: Allocation of length 712704 from process 1 (init) failed
[  101.939875] active_anon:0 inactive_anon:0 isolated_anon:0
[  101.939875]  active_file:0 inactive_file:0 isolated_file:0
[  101.939875]  unevictable:525 dirty:0 writeback:0
[  101.939875]  slab_reclaimable:50 slab_unreclaimable:143
[  101.939875]  mapped:0 shmem:0 pagetables:0
[  101.939875]  sec_pagetables:0 bounce:0
[  101.939875]  kernel_misc_reclaimable:0
[  101.939875]  free:738 free_pcp:0 free_cma:0
[  102.040887] Node 0 active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB isolated(anon):0kB isolated(file):0kB mapped:0kB dirty:0kB writeback:0kB shmem:0kB writeback_tmp:0kB kernel_stack:120kB pagetables:0kB sec_pagetables:0kB all_unreclaimable? no
[  102.139312] Normal free:2952kB boost:0kB min:268kB low:332kB high:396kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB writepending:0kB present:8188kB managed:5992kB mlocked:0kB bounce:0kB free_pcp:0kB local_pcp:0kB free_cma:0kB
[  102.240287] lowmem_reserve[]: 0 0
[  102.296787] Normal: 4*4kB (UM) 5*8kB (UM) 3*16kB (M) 3*32kB (UM) 1*64kB (M) 3*128kB (UM) 3*256kB (UM) 3*512kB (UM) 0*1024kB 0*2048kB 0*4096kB = 2952kB
[  102.565287] 532 total pagecache pages
[  102.597162] binfmt_flat: Unable to allocate RAM for process text/data, errno -12
[  102.633412] Failed to execute /init (error -12)
[  102.673412] Run /sbin/init as init process
[  103.070787] nommu: Allocation of length 712704 from process 1 (init) failed
[  103.099925] active_anon:0 inactive_anon:0 isolated_anon:0
[  103.099925]  active_file:0 inactive_file:0 isolated_file:0
[  103.099925]  unevictable:525 dirty:0 writeback:0
[  103.099925]  slab_reclaimable:50 slab_unreclaimable:143
[  103.099925]  mapped:0 shmem:0 pagetables:0
[  103.099925]  sec_pagetables:0 bounce:0
[  103.099925]  kernel_misc_reclaimable:0
[  103.099925]  free:738 free_pcp:0 free_cma:0
[  103.201587] Node 0 active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB isolated(anon):0kB isolated(file):0kB mapped:0kB dirty:0kB writeback:0kB shmem:0kB writeback_tmp:0kB kernel_stack:120kB pagetables:0kB sec_pagetables:0kB all_unreclaimable? no
[  103.294375] Normal free:2952kB boost:0kB min:268kB low:332kB high:396kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB writepending:0kB present:8188kB managed:5992kB mlocked:0kB bounce:0kB free_pcp:0kB local_pcp:0kB free_cma:0kB
[  103.401375] lowmem_reserve[]: 0 0
[  103.456825] Normal: 4*4kB (UM) 5*8kB (UM) 3*16kB (M) 3*32kB (UM) 1*64kB (M) 3*128kB (UM) 3*256kB (UM) 3*512kB (UM) 0*1024kB 0*2048kB 0*4096kB = 2952kB
[  103.741900] 532 total pagecache pages
[  103.773400] binfmt_flat: Unable to allocate RAM for process text/data, errno -12
[  103.810800] Starting init: /sbin/init exists but couldn't execute it (error -12)
[  103.839425] Run /etc/init as init process
[  103.987825] Run /bin/init as init process
[  104.160925] Run /bin/sh as init process
[  104.533775] nommu: Allocation of length 712704 from process 1 (sh) failed
[  104.564187] active_anon:0 inactive_anon:0 isolated_anon:0
[  104.564187]  active_file:0 inactive_file:0 isolated_file:0
[  104.564187]  unevictable:525 dirty:0 writeback:0
[  104.564187]  slab_reclaimable:50 slab_unreclaimable:143
[  104.564187]  mapped:0 shmem:0 pagetables:0
[  104.564187]  sec_pagetables:0 bounce:0
[  104.564187]  kernel_misc_reclaimable:0
[  104.564187]  free:738 free_pcp:0 free_cma:0
[  104.682900] Node 0 active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB isolated(anon):0kB isolated(file):0kB mapped:0kB dirty:0kB writeback:0kB shmem:0kB writeback_tmp:0kB kernel_stack:120kB pagetables:0kB sec_pagetables:0kB all_unreclaimable? no
[  104.774187] Normal free:2952kB boost:0kB min:268kB low:332kB high:396kB reserved_highatomic:0KB active_anon:0kB inactive_anon:0kB active_file:0kB inactive_file:0kB unevictable:2100kB writepending:0kB present:8188kB managed:5992kB mlocked:0kB bounce:0kB free_pcp:0kB local_pcp:0kB free_cma:0kB
[  104.882575] lowmem_reserve[]: 0 0
[  104.938900] Normal: 4*4kB (UM) 5*8kB (UM) 3*16kB (M) 3*32kB (UM) 1*64kB (M) 3*128kB (UM) 3*256kB (UM) 3*512kB (UM) 0*1024kB 0*2048kB 0*4096kB = 2952kB
[  105.225375] 532 total pagecache pages
[  105.239575] binfmt_flat: Unable to allocate RAM for process text/data, errno -12
[  105.284675] Starting init: /bin/sh exists but couldn't execute it (error -12)
[  105.313612] Kernel panic - not syncing: No working init found.  Try passing init= option to kernel. See Linux Documentation/admin-guide/init.rst for guidance.
[  105.337512] CPU: 0 PID: 1 Comm: sh Not tainted 6.1.14 #4
[  105.360187] Hardware name: riscv-minimal-nommu,qemu (DT)
[  105.374375] Call Trace:
[  105.388650] [<80003520>] 0x80003520
[  105.403425] [<8016db6c>] 0x8016db6c
[  105.424287] [<8017051c>] 0x8017051c
[  105.438925] [<80170554>] 0x80170554
[  105.453262] [<8016dcb0>] 0x8016dcb0
[  105.473962] [<80170888>] 0x80170888
[  105.488912] [<80001e4c>] 0x80001e4c
[  105.510062] ---[ end Kernel panic - not syncing: No working init found.  Try passing init= option to kernel. See Linux Documentation/admin-guide/init.rst for guidance. ]---
```

## Building the Linux image
The Linux distribution meant to be used with tiny-rv32ima is built from [buildroot-tiny-rv32ima](https://github.com/tvlad1234/buildroot-tiny-rv32ima.git). Pre-built images are available in the Releases section of the buildroot-tiny-rv32ima repo.