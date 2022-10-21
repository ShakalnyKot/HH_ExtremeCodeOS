#include <hyperhal/arch_hal/hal_bvideo.h>
#include <hyperhal/arch_hal/hal_keyboard.h>
#include <hyperhal/basefnc.h>
#include <hyperhal/string.h>
#include <ver.h>
#include <types.h>
#include <printf.h>
#include <msg.h>

#define BUFSIZE 1024
#define COMMANDS_COUNT  7
#define WELCOME_MSG \
" _____     _                           _____           _       _____ _____ \n"\
"|  ___|   | |                         /  __ \\         | |     |  _  /  ___|\n"\
"| |____  _| |_ _ __ ___ _ __ ___   ___| /  \\/ ___   __| | ___ | | | \\ `--. \n"\
"|  __\\ \\/ / __| '__/ _ \\ '_ ` _ \\ / _ \\ |    / _ \\ / _` |/ _ \\| | | |`--. \\\n"\
"| |___>  <| |_| | |  __/ | | | | |  __/ \\__/\\ (_) | (_| |  __/\\ \\_/ /\\__/ /\n"\
"\\____/_/\\_\\___|_|  \\___|_| |_| |_|\\___|\\____/\\___/ \\__,_|\\___| \\___/\\____/ \n"\
"                                terminal v0.0.1\n"\                             
/*

                              New gen.
                              --------------
          ..                  OS: ExtremeCodeOS v1.0.0 Yobkiy
       .+#%%*=                Kernel: ExtremeCodeOS kernel
      .#@@@@@%^     ^*=       Uptime: 15 mins
      *@@@##@@%-   -%@%+      Packages: 0 (dpkg) 1 (flatpack)
     :%@@*. :@@^   ^@@@%-     Shell: bash
     =@@@^@= #@^ .=%@@@@#.    Resolution: 1920x1080
    .+@@@^--:%@^+#%@@@@@@^    DE: GNOME
  .=+=#@@@=-%@@%@@@@@@@@@%:   Theme: Yobkiy
  ==+++@@@@@@@@@@@@@@@@@@@=   Icons: Yobkiy
  .+=+=*@@@@@@@@@@@@@@@@@@^   Terminal: ExtremeCodeOS terminal v0.0.1
   .=+++%@@@@@@@@@@@@@@@@@*   CPU: AMD Bulldozer (0.3 x 0.002 Ghz)
       -%@@@@@@@@@@@@@@@@@*   RAM: 5Kb
       #@@@@@@@@@@@@@@@@@@+   GPU: Nvidia Geforce RTX 3080
      +@@@@@@@@@@@@@@@@@#.
      -%@@@@@@@@@@@@@@@#:
       ^@@@@@@@@@@@@@%^.
       .#@@@@@@@@@@%*-
        :*@@@@@@@%*-
          =^##*^=:
                              KONEC EPTA!

*/
#ifdef HEADLESS
#define NEOFETCH_OUTPUT \
"                              New gen.\n" \
"                              --------------\n" \
"          ..                  OS: ExtremeCodeOS/HyperHAL v1.0.0 Yobkiy\n" \
"       .+#%%%%*=                Kernel: HyperHAL\n" \
"      .#@@@@@%%^     ^*=       Uptime: null seconds\n" \
"      *@@@##@@%%-   -%%@%%+      Packages: null\n" \
"     :%%@@*. :@@^   ^@@@%%-     Shell: ExtremeCodeOS Kernel\n" \
"     =@@@^@= #@^ .=%%@@@@#.    Resolution: undefined\n" \
"    .+@@@^--:%%@^+#%%@@@@@@^    DE: undefined\n" \
"  .=+=#@@@=-%%@@%%@@@@@@@@@%%:   Theme: HyperHAL blue/white\n" \
"  ==+++@@@@@@@@@@@@@@@@@@@=   Icons: undefined\n" \
"  .+=+=*@@@@@@@@@@@@@@@@@@^   Terminal: Legacy x86 COM1 serial port\n" \
"   .=+++%%@@@@@@@@@@@@@@@@@*   CPU: Unknown x86\n" \
"       -%%@@@@@@@@@@@@@@@@@*   RAM: undefined\n" \
"       #@@@@@@@@@@@@@@@@@@+   GPU: undefined\n" \
"      +@@@@@@@@@@@@@@@@@#.\n" \
"      -%%@@@@@@@@@@@@@@@#:\n" \
"       ^@@@@@@@@@@@@@%%^.\n" \
"       .#@@@@@@@@@@%%*-\n" \
"        :*@@@@@@@%%*-\n" \
"          =^##*^=:\n" \
"                              KONEC EPTA!\n"
#else
#define NEOFETCH_OUTPUT \
"                              New gen.\n" \
"                              --------------\n" \
"          ..                  OS: ExtremeCodeOS/HyperHAL v1.0.0 Yobkiy\n" \
"       .+#%%%%*=                Kernel: HyperHAL\n" \
"      .#@@@@@%%^     ^*=       Uptime: null seconds\n" \
"      *@@@##@@%%-   -%%@%%+      Packages: null\n" \
"     :%%@@*. :@@^   ^@@@%%-     Shell: ExtremeCodeOS Kernel\n" \
"     =@@@^@= #@^ .=%%@@@@#.    Resolution: Text 80x25\n" \
"    .+@@@^--:%%@^+#%%@@@@@@^    DE: undefined\n" \
"  .=+=#@@@=-%%@@%%@@@@@@@@@%%:   Theme: HyperHAL blue/white\n" \
"  ==+++@@@@@@@@@@@@@@@@@@@=   Icons: undefined\n" \
"  .+=+=*@@@@@@@@@@@@@@@@@@^   Terminal: Legacy x86 VGA mode 80x25\n" \
"   .=+++%%@@@@@@@@@@@@@@@@@*   CPU: Unknown x86\n" \
"       -%%@@@@@@@@@@@@@@@@@*   RAM: undefined\n" \
"       #@@@@@@@@@@@@@@@@@@+   GPU: undefined\n" \
"      +@@@@@@@@@@@@@@@@@#.\n" \
"      -%%@@@@@@@@@@@@@@@#:\n" \
"       ^@@@@@@@@@@@@@%%^.\n" \
"       .#@@@@@@@@@@%%*-\n" \
"        :*@@@@@@@%%*-\n" \
"          =^##*^=:\n" \
"                              KONEC EPTA!\n"
#endif

static const char* commands_arr[COMMANDS_COUNT] = {
    "exit",
    "ls",
    "echo",
    "clear",
    "touch",
    "rm",
    "neofetch",
};

typedef enum {
    E_SUCCESS,
    E_UNKNOWN,
    E_EMPTY
} req_exit_code_t;

typedef enum {
    C_EXIT,
    C_LS,
    C_ECHO,
    C_CLEAR,
    C_TOUCH,
    C_RM,
    C_NEOFETCH,
} command_t;

typedef struct {
    size_t size;
    char   input[BUFSIZE];
    command_t cm;
} terminal_req_t;

static terminal_req_t get_user_input (void){
    terminal_req_t __req = {BUFSIZE, "", -1};
    char* tmp_input = (char*)hal_kbd_getstring();
    memcpy(&__req.input, tmp_input, strlen(tmp_input));

    for (size_t i = 0; i < COMMANDS_COUNT; i++)
        if (!memcmp(commands_arr[i], __req.input, strlen(commands_arr[i]))){
            __req.cm = i;
            break;
        }
    
    return __req;
}

static req_exit_code_t terminal(terminal_req_t __req){
    char prom[BUFSIZE];
    switch (__req.cm){
    case C_EXIT:
        do_halt();
    case C_LS:
        printf("FS not implemented.\n");
        /* printf("--r--w--r admin admin 3.6GB porn\n");
        printf("--r--w--r admin admin 53KB kernel.c\n");
        printf("--r--w--r admin admin 53KB GTAV.exe\n"); */
        break;
    case C_ECHO:
        __req.input[__req.size] = '\0';
        printf("%s\n", __req.input + strlen(commands_arr[C_ECHO]) + 1);
        break;
    case C_CLEAR:
        bvideo_clearscr();
        break;
    case C_TOUCH:
        printf("FS not implemented.\n");
        break;
    case C_RM:
        printf("FS not implemented.\n");
        break;
    case C_NEOFETCH:
        printf(NEOFETCH_OUTPUT);
	printf("\n");
        break;
    default:
        return (*__req.input == '\0') ? E_EMPTY : E_UNKNOWN;
    }
    return E_SUCCESS;
}

void ecos_entry(){
   printf(WELCOME_MSG);
   terminal_req_t rq = {0};

    for (;;) {
        printf ("area/: ~$ ");
        
        rq = get_user_input();
        switch (terminal(rq)) {
        case E_EMPTY:   ; break;
        case E_SUCCESS: ; break;
        case E_UNKNOWN: 
            printf("terminal error: unknown command: \"%s\"\n", rq.input);
            break;
        }
    } 

    return;
}
