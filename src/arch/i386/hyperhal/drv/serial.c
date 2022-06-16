#include <arch/i386/hyperhal/asm/io.h>
#include <arch/i386/hyperhal/drv/serial.h>

bool i386_init_serial() {
    write_port(COM1_SePORT + 1, 0x00);    // Disable all interrupts
    write_port(COM1_SePORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    write_port(COM1_SePORT + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    write_port(COM1_SePORT + 1, 0x00);    //                  (hi byte)
    write_port(COM1_SePORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    write_port(COM1_SePORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    write_port(COM1_SePORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    write_port(COM1_SePORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    write_port(COM1_SePORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (read_port(COM1_SePORT + 0) != 0xAE) {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    write_port(COM1_SePORT + 4, 0x0F);
    return true;
}

int i386_serial_received() {
    return read_port(COM1_SePORT + 5) & 1;
}

char i386_read_serial() {
    while (i386_serial_received() == 0);
    return read_port(COM1_SePORT);
}

int i386_is_transmit_empty() {
    return read_port(COM1_SePORT + 5) & 0x20;
}

void i386_write_serial(char a) {
    while (i386_is_transmit_empty() == 0);
    write_port(COM1_SePORT, a);
}