
// #define LOAD_SNAPSHOT 1
#define SNAPSHOT_FILENAME "SNAP"

#define KERNEL_FILENAME "IMAGE"
#define DTB_FILENAME "DTB"
#define BLK_FILENAME "ROOTFS"

#define DTB_SIZE 2048

// RAM size in megabytes
#define EMULATOR_RAM_MB 8

// Kernel command line
#define KERNEL_CMDLINE "console=hvc0 root=fe00"

// Time divisor
#define EMULATOR_TIME_DIV 8

// Tie microsecond clock to instruction count
#define EMULATOR_FIXED_UPDATE 0

// Cache configuration
#define CACHE_LINE_SIZE 16
#define OFFSET_BITS 4 // log2(CACHE_LINE_SIZE)
#define CACHE_SET_SIZE 16
#define INDEX_BITS 4 // log2(CACHE_SET_SIZE)

// Keyboard queue length
#define KEY_QUEUE_LEN 8
