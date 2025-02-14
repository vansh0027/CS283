#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if(cmd_line==NULL || strlen(cmd_line)==0){
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
        char *space;
        char *curr_token=strtok_r(cmd_tokens, " ", &space);
        if (curr_token!= NULL)
        {
            
        
        strncpy(clist->commands[clist->num].exe, curr_token, EXE_MAX-1);
        clist->commands[clist->num].exe[EXE_MAX - 1]= '\0';

        char *arg_token=strtok_r(NULL, "", &space);
        if (arg_token != NULL)
        {
            while (*arg_token==' ') arg_token++;
            strncpy(clist->commands[clist->num].args, arg_token, ARG_MAX - 1);
            clist->commands[clist->num].args[ARG_MAX - 1] = '\0';
        }
        else
        {
            clist->commands[clist->num].args[0] = '\0';
        }
            
        
        
        clist->num++;
        }
        cmd_tokens = strtok_r(NULL,"|", &pipe_save);
    
    


    }
    if(clist->num == 0) {
        return WARN_NO_CMDS;
    }
    return(OK);

}