#include "mem.h"

void report_error(const char *message) {
    fprintf(stderr, ERROR"%s\n", message);
}

// Define an enum for the commands
enum Command {
    CMD_HELP,
    CMD_EXIT,
    CMD_PID,
    CMD_READLINES,
    CMD_READLINESCHAR,
    CMD_SEARCH,
    CMD_SEARCHONE,
    CMD_SEARCHSTR,
    CMD_SEARCHSTRONE,
    CMD_SEARCHRANGE,
    CMD_WRITE,
    CMD_WRITESTR,
    CMD_PAUSE,
    CMD_RESUME,
    CMD_RESULTS,
    CMD_UNKNOWN
};

// Function to get the command based on the first argument
enum Command getCommand(char *firstArg) {
    if (strcmp(firstArg, "help\n") == 0) {
        return CMD_HELP;
    } else if (strcmp(firstArg, "exit\n") == 0 || strcmp(firstArg, "e\n") == 0) {
        return CMD_EXIT;
    } else if (strcmp(firstArg, "pid\n") == 0) {
        return CMD_PID;
    } else if (strcmp(firstArg, "readlines") == 0 || strcmp(firstArg, "rl") == 0) {
        return CMD_READLINES;
    } else if (strcmp(firstArg, "readlineschar") == 0 || strcmp(firstArg, "rlc") == 0) {
        return CMD_READLINESCHAR;
    } else if (strcmp(firstArg, "search") == 0 || strcmp(firstArg, "s") == 0) {
        return CMD_SEARCH;
    } else if (strcmp(firstArg, "searchone") == 0 || strcmp(firstArg, "so") == 0) {
        return CMD_SEARCHONE;
    } else if (strcmp(firstArg, "searchstr") == 0 || strcmp(firstArg, "ss") == 0) {
        return CMD_SEARCHSTR;
    } else if (strcmp(firstArg, "searchstrone") == 0 || strcmp(firstArg, "sso") == 0) {
        return CMD_SEARCHSTRONE;
    } else if (strcmp(firstArg, "searchrange") == 0 || strcmp(firstArg, "sr") == 0) {
        return CMD_SEARCHRANGE;
    } else if (strcmp(firstArg, "write") == 0 || strcmp(firstArg, "w") == 0) {
        return CMD_WRITE;
    } else if (strcmp(firstArg, "writestr") == 0 || strcmp(firstArg, "ws") == 0) {
        return CMD_WRITESTR;
    } else if (strcmp(firstArg, "pause\n") == 0 || strcmp(firstArg, "p\n") == 0) {
        return CMD_PAUSE;
    } else if (strcmp(firstArg, "resume\n") == 0 || strcmp(firstArg, "r\n") == 0) {
        return CMD_RESUME;
    } else if (strcmp(firstArg, "results\n") == 0 || strcmp(firstArg, "res\n") == 0) {
        return CMD_RESULTS;
    } else {
        return CMD_UNKNOWN;
    }
}

void interact(pid_t pid, mach_port_t task) {
    char input[128];
    char args[10][30];

    vm_address_t base;
    vm_address_t end;
    vm_address_t out[SEARCH_MAX];
    result_t resultnum = 0;
    base = end = 0;

    printf(GOOD"To list iFindMemory commands, type 'help'\n");

    while (1) {
        memset(args, 0, sizeof(args[0][0])*10*30);

        printf(CYAN ">> " WHITE);
        fgets(input, 128, stdin);

        int charIndex = 0;
        int argIndex = 0;
        int argCharIndex = 0;
        bool isInQuote = false;

        input[strlen(input)] = ' ';

        for (int i = 0; i < strlen(input); i++) {
            if (input[i] == '"') isInQuote =!isInQuote;
            else if (input[i] == ' ' &&!isInQuote) {
                argCharIndex = 0;
                for (; charIndex < i; charIndex++) {
                    if (input[charIndex] == '"') continue;
                    args[argIndex][argCharIndex] = input[charIndex];
                    argCharIndex++;
                }
                argIndex++;
                charIndex++;
            }
        }
        // Get the command enum value
        enum Command cmd = getCommand(args[0]);

        // Use the switch statement to handle commands
        switch (cmd) {



        case CMD_HELP:
                printf(GOOD"List of commands:\n"
           MAGENTA "search (s) " GREEN "[bytes] " WHITE "- search " GREEN"[bytes] " WHITE "(ex: s 434231)\n"
           MAGENTA "searchstr (ss) " GREEN "[string] " WHITE "- search " GREEN"[string] " WHITE "(ex: ss CBA)\n"
           MAGENTA "searchone (so) " GREEN "[bytes] " WHITE "- search " GREEN"[bytes] " WHITE "for one result(ex: s 434231)\n"
           MAGENTA "searchstrone (sso) " GREEN "[string] " WHITE "- search " GREEN"[string] " WHITE "for one result (ex: ss CBA)\n"
           MAGENTA "searchrange (sr) " GREEN "[string] [string]" WHITE " - search from " GREEN " [string]" WHITE " to another " GREEN "[string] " WHITE "(ex: sr CBA ABC)\n" 
           MAGENTA "write (w) " GREEN "[bytes] " WHITE "- write " GREEN"[bytes] " WHITE "to scanned address (ex: w 434231)\n"
           MAGENTA "writestr (ws) " GREEN "[string] " WHITE "- write " GREEN"[string] " WHITE "to scanned address (ex: ws CBA)\n"
           MAGENTA "readlines (rl) " GREEN "0x[address] [lines] " WHITE "- read " GREEN "[lines] " WHITE "of " GREEN "[address]"WHITE"\n"
           MAGENTA "readlineschar (rlc) " GREEN "0x[address] [lines]" WHITE "- read " GREEN "[lines] " WHITE "of " GREEN "[address] "WHITE"as chars\n"
           MAGENTA "pid " GREEN "[pid] " WHITE "- changes pid to new [pid]\n"
           MAGENTA "pause (p) " WHITE "- pauses task\n"
           MAGENTA "resume (re) " WHITE "- resumes task\n"
           MAGENTA "results (res) " WHITE "- prints search results\n"
           MAGENTA "quit (q) " WHITE "- exits iFindMemory\n");

            break;


        case CMD_EXIT:
            printf(GOOD"Exiting iFindMemory....\n");
            exit(0);
            break;
                
        case CMD_PID:
            if (args[1][0]!= '\0') {
                pid = (int) strtol(args[1], NULL, 0);
                kern_return_t changetfp = task_for_pid(mach_task_self(), pid, &task);
                if (changetfp == KERN_SUCCESS) {
                    printf(GOOD"Changed pid to %d\n", pid);
                } else {
                    report_error("Could not change PID\n");
                }
            } else {
                report_error("Not enough arguments for pid!\n");
            }
            break;
      
        case CMD_READLINES:
            if (args[1][0]!= '\0' && args[2][0]!= '\0') {
                read_lines(task, (vm_address_t) strtol(args[1], NULL, 0), (int) strtol(args[2], NULL, 0), false);
            } else {
                report_error("Not enough arguments for readlines!\n");
            }
            break;
  
        case CMD_READLINESCHAR:
            if (args[1][0]!= '\0' && args[2][0]!= '\0') {
                read_lines(task, (vm_address_t) strtol(args[1], NULL, 0), (int) strtol(args[2], NULL, 0), true);
            } else {
                report_error("Not enough arguments for readlineschar!\n");
            }
            break;

        case CMD_SEARCHSTR:
            if (args[1][0]!= '\0') {
                get_region_size(task, &base, &end);
                printf(GOOD"Finding string - %s\n", args[1]);
                search_data(task, true, false, base, end, (vm_address_t **)&out, &resultnum, args[1]);
            } else {
                report_error("Not enough arguments for searchstr!\n");
            }
            break;

        case CMD_SEARCHSTRONE:
            if (args[1][0]!= '\0') {
                get_region_size(task, &base, &end);
                printf(GOOD"Finding string - %s\n", args[1]);
                search_data(task, true, true, base, end, (vm_address_t **)&out, &resultnum, args[1]);
            } else {
                report_error("Not enough arguments for searchstrone!\n");
            }
            break;

        case CMD_SEARCHRANGE:
            if (args[1][0]!= '\0' && args[2][0]!= '\0') {
                get_region_size(task, &base, &end);
                printf(GOOD"Finding string range - %s to %s\n", args[1], args[2]);
                search_with_range(task, true, base, end, (vm_address_t **)&out, &resultnum, args[1], args[2]);
            } else {
                report_error("Not enough arguments for searchrange!\n");
            }
            break;

        case CMD_WRITE:
            if (args[1][0]!= '\0') {
                write_data(task, false, out, args[1]);
            } else {
                report_error("Not enough arguments for write!\n");
            }
            break;

        case CMD_WRITESTR:
            if (args[1][0]!= '\0') {
                write_data(task, true, out, args[1]);
            } else {
                report_error("Not enough arguments for writestr!\n");
            }
            break;

        case CMD_RESULTS:
            if (resultnum == 0) {
                report_error("No results available to print!");
            } else {
                printf(GOOD"Printing %d result(s) from last scan...\n", resultnum);
                for (int i=0; i<resultnum; i++) {
                    printf("0x%lx\n", out[i]);
                }
            }
            break;

        default:
            printf(ERROR"Unknown command\n");
            break;
        }
    }
} 



int main(int argc, char *argv[]) {
    pid_t pid = 0;
    mach_port_t task;

    printf(YELLOW" # Welcome to iFindMemory!\n");

    if (geteuid() && getuid()) {
        report_error("Run iFindMemory as root.\n");
        EXIT
    }

    if (argc == 1) {
        printf(GOOD"PID to attach: ");
        scanf("%d", &pid);
        getchar();
    } else if (argc == 2) {
        pid = strtol(argv[1], NULL, 0);
    } else if (argc > 2) {
        report_error("Too many CLI arguments!\n");
        EXIT
    }

    kern_return_t kret = task_for_pid(mach_task_self(), pid, &task);
    if (kret!= KERN_SUCCESS) {
        printf(ERROR"Couldn't obtain task_for_pid(%d)\n", pid);
        report_error("Do you have proper entitlements?\n");
        EXIT
    } else {
         printf(GOOD"Obtained task_for_pid(%d)\n", pid);
    }

    interact(pid, task);
    return 0;
}