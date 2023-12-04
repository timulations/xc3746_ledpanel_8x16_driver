#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


static void set_screen_loop(int fd, unsigned char *buf, size_t bufsize) {
    while (1) {
        write(fd, buf, bufsize);
    }
}

int main(int argc, char *argv[]) {

    int ledpanel;
    unsigned char buf[4][16] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x42, 0x84, 0x42, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x26, 0x41, 0x86, 0x80, 0x80, 0x80, 0x86, 0x41, 0x26, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x20, 0x44, 0x42, 0x84, 0x80, 0x84, 0x42, 0x44, 0x20, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0xC0, 0x40, 0xF8, 0xD8, 0x7E, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    unsigned char allzero[16] = {0};

    ledpanel = open("/dev/ledpanel", O_WRONLY);
    if (ledpanel < 0)
        return 1;

    if (argc <= 1) {
        set_screen_loop(ledpanel, buf[0], 16);
    } else {
        switch (argv[1][0]) {
            case '0':
                set_screen_loop(ledpanel, buf[0], 16);
                break;
            case '1':
                set_screen_loop(ledpanel, buf[1], 16);
                break;
            case '2':
                set_screen_loop(ledpanel, buf[2], 16);
                break;
            case '3':
                set_screen_loop(ledpanel, buf[3], 16);
                break;
            default:
                set_screen_loop(ledpanel, allzero, 16);
        }
    }
    
    close(ledpanel);
    return 0;
}
