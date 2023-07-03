#define CONCAT_INTERNAL(x, y) x ## y
#define CONCAT(x, y) CONCAT_INTERNAL(x, y)
#define STR(x) #x

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

static unsigned long int logs[16];
static int num_logs = 0;

void print_uart_uint(unsigned int i) {
    int nibbles = 1;
    unsigned int temp = i >> 4;
    while (temp > 0) {
        nibbles++;
        temp >>= 4;
    }

    *UART0DR = '0';
    *UART0DR = 'x';
    for (; nibbles > 1; nibbles--) {
        unsigned int nibble = 0xf & (i >> ((nibbles - 1) << 2));
        *UART0DR = nibble + ((nibble < 0xa) ? 48 : 87);
    }
    unsigned int nibble = 0xf & i;
    *UART0DR = nibble + ((nibble < 0xa) ? 48 : 87);
    *UART0DR = '\n';
}

void print_uart_str(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

#define ___LOG(msg, id, label) \
    __asm(".pushsection .logs"); \
    __asm(".size \"" msg "\", 4"); \
    __asm(STR(label) ":"); \
    __asm("\"" msg "\":"); \
    __asm(".word 0"); \
    __asm(".popsection"); \
    extern const int label; \
    static unsigned long int id = (unsigned long int)(&label); \
    logs[num_logs] = id; \
    num_logs++; \
    print_uart_str("log_id="); \
    print_uart_uint(id)


#define __LOG(msg, id) \
    ___LOG(msg, id, CONCAT(id, _label))

#define LOG(msg) \
    __LOG(msg, CONCAT(__log_, __COUNTER__))


int main() {
    print_uart_str("Hello world!\n");
    print_uart_uint(0x123456);
    LOG("hello world");
    LOG("foo bar");
    LOG("test");
    LOG("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Nunc non blandit massa enim nec. Vel orci porta non pulvinar neque laoreet suspendisse interdum. Integer quis auctor elit sed vulputate mi sit amet. Sed vulputate mi sit amet mauris commodo quis imperdiet massa. Nisl pretium fusce id velit ut tortor pretium viverra suspendisse. Faucibus turpis in eu mi bibendum neque. Semper risus in hendrerit gravida rutrum. Velit laoreet id donec ultrices tincidunt. Diam volutpat commodo sed egestas egestas fringilla. Lectus sit amet est placerat in egestas erat. Tincidunt lobortis feugiat vivamus at augue. Pellentesque habitant morbi tristique senectus et netus et. Id neque aliquam vestibulum morbi. Sit amet dictum sit amet justo donec enim diam vulputate. Volutpat lacus laoreet non curabitur gravida arcu ac tortor. Semper auctor neque vitae tempus quam. Ullamcorper eget nulla facilisi etiam dignissim diam quis enim. Purus sit amet volutpat consequat. Vulputate enim nulla aliquet porttitor lacus luctus. Commodo elit at imperdiet dui accumsan sit amet. Enim nulla aliquet porttitor lacus. Est velit egestas dui id ornare arcu. Netus et malesuada fames ac turpis. Vel orci porta non pulvinar neque. Aliquet eget sit amet tellus cras adipiscing enim eu. Phasellus vestibulum lorem sed risus ultricies tristique. In fermentum posuere urna nec tincidunt praesent. Amet nulla facilisi morbi tempus iaculis urna id volutpat lacus. Aenean et tortor at risus. Ac turpis egestas maecenas pharetra convallis posuere. Nulla aliquet porttitor lacus luctus accumsan tortor posuere. Varius morbi enim nunc faucibus a pellentesque sit amet. Lectus mauris ultrices eros in. Tellus mauris a diam maecenas sed enim ut sem. Dui id ornare arcu odio ut sem nulla pharetra diam. Nisi vitae suscipit tellus mauris a diam maecenas. Imperdiet proin fermentum leo vel orci porta non. Metus vulputate eu scelerisque felis imperdiet proin. Mi tempus imperdiet nulla malesuada pellentesque elit eget. Sit amet nisl purus in. Lectus quam id leo in vitae. Nullam eget felis eget nunc lobortis mattis. Malesuada nunc vel risus commodo viverra. Auctor augue mauris augue neque gravida in fermentum et sollicitudin. Aliquet risus feugiat in ante metus dictum at tempor. Quam quisque id diam vel quam elementum pulvinar etiam non. Commodo sed egestas egestas fringilla phasellus faucibus. Habitant morbi tristique senectus et netus et. Sollicitudin tempor id eu nisl. Augue eget arcu dictum varius. Ornare quam viverra orci sagittis eu volutpat odio. Nisi quis eleifend quam adipiscing vitae. Vulputate mi sit amet mauris commodo quis imperdiet. Sed arcu non odio euismod lacinia. Dignissim enim sit amet venenatis urna cursus eget nunc. Dui vivamus arcu felis bibendum ut tristique et. Aliquet porttitor lacus luctus accumsan tortor posuere ac. Nunc sed id semper risus in.");
    return (int)(logs[2]);
}
