#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */




int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}
int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0)
        return BI_CMD_EXIT;
    else if (strcmp(input, "cd") == 0)
        return BI_CMD_CD;
    else if (strcmp(input, "dragon") == 0)
        return BI_CMD_DRAGON;
    return BI_NOT_BI;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
    if (cmd_line == NULL || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    clear_cmd_buff(cmd_buff);

    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX - 1);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

    char *ptr = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*ptr != '\0' && argc < CMD_ARGV_MAX - 1) {
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;

        if (*ptr == '"') {
            ptr++;
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != '"') ptr++;
            if (*ptr == '"') {
                *ptr = '\0';
                ptr++;
            }
        } else {
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && !isspace(*ptr)) ptr++;
            if (*ptr) {
                *ptr = '\0';
                ptr++;
            }
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    if (argc == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}



int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX];
    cmd_buff_t cmd;

    if (alloc_cmd_buff(&cmd) != OK){
        return ERR_MEMORY;

    }
    while(1){
         printf("%s", SH_PROMPT);
         if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
         }
      //remove the trailing \n from cmd_buff
       cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
       int rc= build_cmd_buff(cmd_buff, &cmd);


        
       if(rc==WARN_NO_CMDS || cmd.argv[0] == NULL){
        printf(CMD_WARN_NO_CMD);
        continue;
       }

       Built_In_Cmds bicmd= match_command(cmd.argv[0]);
       if (bicmd != BI_NOT_BI) {
            bicmd = exec_built_in_cmd(&cmd);
            if (bicmd == BI_CMD_EXIT) {
                break;
            }
            continue;
        }
     
    
        pid_t pid = fork();
        if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            perror("execute command error");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            perror("fork failed");
        }
    }
    free_cmd_buff(&cmd);
    return OK;

}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc == 2) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd failed");
            }
        }
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

