#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void  reverse_string(char *, int, int);
void  print_words(char *, int, int);
int search_and_replace(char *, int, char *, char *);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int i = 0; char previous= ' ';
    while(*user_str != '\0' && i<len){
        if(*user_str!= ' '){
            buff[i++]=*user_str;
            previous=*user_str;
        }
        else if (previous!=' ')
        {
            buff[i++]=' ';
            previous=' ';
            /* code */
        }
        user_str++;
        
    }
    if(*user_str!='\0'){
        return -1;
    }
    if(i>0 && buff[i-1]==' '){
        i--;
    }
    while (i<len)
    {
        buff[i++]= '.';
        /* code */
    }
    return i;
    
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int count =0;
    int i=0;
    while(i< str_len){
        while (i<str_len && (buff[i]==' '||buff[i]=='.'))
        {
            i++;
        }
        if(i < str_len){
            count++;
            while (i < str_len && buff[i] != ' ' && buff[i]!='.')
            {
                i++;/* code */
            }
            
        }
        
    }
    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

void print_words(char *buff, int len, int str_len){
    printf("Word Print\n");
    printf("----------\n");
    int i=0; int word_start=0; int word_len=0; int count =0;
    while(i< str_len){
        while ((i<str_len && (buff[i]==' ' ||buff[i]=='.' )))
        {
            i++;
        }
        word_start=i;
        while (i < str_len && (buff[i] != ' ' && buff[i]!='.'))
        {
                i++;/* code */
        }
        word_len=i-word_start;
        if(word_len>0){
            printf("%d. ",++count);
            for(int j=word_start;j<word_start+word_len;j++ ){
                putchar(buff[j]);
            }
            printf("(%d)\n", word_len);

        }
    }
    printf("\nNumber of words returned: %d\n",count);



}

void reverse_string(char *buff, int len, int str_len){
    while(len<str_len){ 
        while(buff[len]=='.' && len<str_len){
            len++;
        }
        while(buff[str_len]=='.' && str_len>len){
            str_len--;
        }
        
        char temp= buff[len];
        buff[len]=buff[str_len];
        buff[str_len]= temp; 
        len++;
        str_len--;
}
}
int search_and_replace(char *buff, int len, char *find, char *replace){
    int find_length=0;
    int replace_length=0;
    int input_length=0;
    while(find[find_length]!='\0'){ 
        find_length++;
    }
    while(replace[replace_length]!='\0'){ 
        replace_length++;
    }
    while(buff[input_length]!='\0' && buff[input_length]!='.'){
        input_length++;
    }

    int flag=-1;
    for(int i=0; i<=input_length-find_length;i++){
        int j=0;
        while(j<find_length && buff[i+j]==find[j]){
            j++;
        }
        if(j==find_length){
            flag=i;
            break;

        }
    }
    int new_length=input_length-find_length+replace_length;
    if (flag==-1)
    {
        return -1;
    }

    if(new_length>len-1){
        new_length= BUFFER_SZ;
    }
    if(replace_length > find_length){
        for(int i=input_length-1; i>= flag+find_length; i--){
            buff[i+replace_length-find_length]=buff[i];
        }
    }
    else if(replace_length < find_length){
        for(int i=flag+find_length; i<input_length; i++){
            buff[i+replace_length-find_length]=buff[i];
        }
    }
    for(int i=0; i<replace_length;i++){
        buff[flag+i]=replace[i];
    }
    for(int i= new_length; i<len; i++){
        buff[i]='.';
    }
    return 0;

}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      The first part of the argument checks if there is atleast 1 argument present except the program name, if no argument exits checking argv[1] would cause undefined behavior.
    //      The second part of the argument after or ensures the that the command starts withe the hypen otherwise all options are printed.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This is a check if the user provides atleast 2 arguments( the option and the input string) except the program name. Fewer arguments might cause undefined behavior.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff=(char *)malloc(BUFFER_SZ * sizeof(char));
    if(buff==NULL){
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("This is a string that does not work as it is too long");
        exit(3);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff,0, user_str_len-1);  //you need to implement
            break;
        case 'w':
            print_words(buff, BUFFER_SZ, user_str_len);
            break;
        case 'x':
            if(argc<5){
                exit(1);
            }
            rc= search_and_replace(buff, BUFFER_SZ, argv[3], argv[4]);
            if(rc==0){
            break;
            }
            else{
                exit(3);
                break;
            }
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          The program passes the length because then it can be adapted to any size of the buffer. It ensures our code is resuable for other problems.
//          It also prevents the buffer from overflowing to ensure there is no undefined behavior. It's a way of refactoring the code in a way.
