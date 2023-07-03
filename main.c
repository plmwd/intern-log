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
    return (int)(logs[2]);
}
