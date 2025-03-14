#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
    if (cmd_line == NULL || strlen(cmd_line) == 0){
        return WARN_NO_CMDS;
    }

    clear_cmd_buff(cmd_buff);
    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX - 1);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

    char *ptr = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*ptr != '\0' && argc < CMD_ARGV_MAX - 1){
        while (isspace(*ptr)) ptr++;
        if (*ptr == '\0') break;

        if (*ptr == '"'){
            ptr++;
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && *ptr != '"') ptr++;
            if (*ptr == '"') *ptr = '\0';
            ptr++;
        } else if (*ptr == '>' || *ptr == '<'){
            if (*ptr == '>' && *(ptr + 1) == '>'){
                cmd_buff->argv[argc++] = ">>";
                ptr += 2;
            } else {
                cmd_buff->argv[argc++] = (*ptr == '>') ? ">" : "<";
                ptr++;
            }
        } else {
            cmd_buff->argv[argc++] = ptr;
            while (*ptr && !isspace(*ptr) && *ptr != '>' && *ptr != '<') ptr++;
        }

        if (*ptr) {
            *ptr = '\0';
            ptr++;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    if (argc == 0){
        return WARN_NO_CMDS;
    }

    return OK;
}



int build_cmd_list(char *cmd_line, command_list_t *clist){
     if (cmd_line == NULL || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }
    memset(clist, 0, sizeof(command_list_t));
    char cmd_copy[SH_CMD_MAX];
    strncpy(cmd_copy, cmd_line, SH_CMD_MAX-1);
    cmd_copy[SH_CMD_MAX - 1]='\0';
    char *pipe_save;
    char *cmd_tokens=strtok_r(cmd_copy, "|",&pipe_save);
    while (cmd_tokens != NULL)
    {
        if(clist->num >= CMD_MAX){
            return ERR_TOO_MANY_COMMANDS;
        }
        while (*cmd_tokens == SPACE_CHAR) 
        {cmd_tokens++;
        }
        char *end = cmd_tokens + strlen(cmd_tokens) - 1;
        while (end > cmd_tokens && *end == SPACE_CHAR){
            *end = '\0';
            end--;

        } 
        if(strlen(cmd_tokens)==0){
            cmd_tokens = strtok_r(NULL, "|", &pipe_save);
            continue;   
        }
        cmd_buff_t *cmd=&clist->commands[clist->num];
        if(alloc_cmd_buff(cmd)!= OK){
            return ERR_MEMORY;
        }
        if(build_cmd_buff(cmd_tokens,cmd)==OK){
            clist->num++;

        }
        cmd_tokens=strtok_r(NULL,"|", &pipe_save);
    }
    if(clist->num == 0){
        return WARN_NO_CMDS;
    }
    return OK;

}



int exec_local_cmd_loop()
{
    char cmd_buff[SH_CMD_MAX];
    command_list_t clist;

    while(1){
         printf("%s", SH_PROMPT);
         if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
         }
       cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
       if(strcmp(cmd_buff, EXIT_CMD)==0){
        break;
       }
       int parse_status=build_cmd_list(cmd_buff, &clist);
       if(parse_status!=OK){

        if (parse_status == WARN_NO_CMDS){
                printf(CMD_WARN_NO_CMD);
            } 
            else if (parse_status == ERR_TOO_MANY_COMMANDS){
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            }
            continue;
       }
       if (clist.num==1) {
        cmd_buff_t *cmd=&clist.commands[0];
        Built_In_Cmds bicmd= match_command(cmd->argv[0]);
       if (bicmd != BI_NOT_BI) {
        exec_built_in_cmd(cmd);
        }
        else{
        /*
        pid_t pid = fork();
        if (pid == 0) {
            execvp(cmd->argv[0], cmd->argv);
            perror("execute command error");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0);
        } else {
            perror("fork failed");
            continue;
        }

    }*/
        execute_pipeline(&clist);}}
    else{
        execute_pipeline(&clist);
    }
    free_cmd_list(&clist);
    }
    return OK;

}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd){
    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(cmd->argv[0], "cd") == 0){
        if (cmd->argc == 2){
            if (chdir(cmd->argv[1]) != 0){
                perror("cd failed");
            }
        }
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}

int execute_pipeline(command_list_t *clist){
    int num_cmds = clist->num;
    int pipes[CMD_MAX - 1][2];
    pid_t pids[CMD_MAX];

    for (int i = 0; i < num_cmds - 1; i++){
        if (pipe(pipes[i]) == -1){
            perror("pipe");
            return ERR_MEMORY;
        }
    }

    for (int i = 0; i < num_cmds; i++){
        pids[i] = fork();
        if (pids[i] < 0){
            perror("fork");
            return ERR_MEMORY;
        }

        if (pids[i] == 0){
            int fd_in = -1, fd_out = -1;
            for (int j = 0; j < clist->commands[i].argc; j++){
                if (strcmp(clist->commands[i].argv[j], ">") == 0){
                    fd_out = open(clist->commands[i].argv[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd_out < 0) {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                    clist->commands[i].argv[j] = NULL;
                    break;
                } else if (strcmp(clist->commands[i].argv[j], ">>") == 0){
                    fd_out = open(clist->commands[i].argv[j + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (fd_out < 0) {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                    clist->commands[i].argv[j] = NULL;
                    break;
                } else if (strcmp(clist->commands[i].argv[j], "<") == 0){
                    fd_in = open(clist->commands[i].argv[j + 1], O_RDONLY);
                    if (fd_in < 0) {
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                    clist->commands[i].argv[j] = NULL;
                    break;
                }
            }

            if (i > 0) dup2(pipes[i - 1][0], STDIN_FILENO);
            if (i < num_cmds - 1) dup2(pipes[i][1], STDOUT_FILENO);

            for (int j = 0; j < num_cmds - 1; j++){
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_cmds - 1; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_cmds; i++){
        waitpid(pids[i], NULL, 0);
    }

    return OK;
}


int free_cmd_list(command_list_t *cmd_lst){
    if (!cmd_lst) return OK;
    for (int i = 0; i < cmd_lst->num; i++){
        free_cmd_buff(&cmd_lst->commands[i]);
    }
    cmd_lst->num = 0;
    return OK;
}
