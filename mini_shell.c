#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<signal.h>
#include <fcntl.h>

#define TRUE 1
#define  ERROR -1
#define MAX_CHARS 510
#define  LIMIT_NAME_LENGTH 50

char readSentence(char read[], char * hostname);
int readSecondSentence(char read[]);
int num_of_words(char * str);
char** countWords(char * array);
void print_command_line();
void sig_handler(int signum);
void execWithLine(char **command1, char **command2);
void execOperatorW(char **command1, char **command2);
void execOperatorR(char **command1, char **command2);


int main() {
    signal(SIGTSTP,sig_handler);
    signal(SIGCHLD, sig_handler);
    char is_contains_char;
    pid_t pid;
    int i;
    char hostname[LIMIT_NAME_LENGTH+1], temp[MAX_CHARS+1], temp2[MAX_CHARS+1] = "";
    char** send;
    char** send2;
    gethostname(hostname, LIMIT_NAME_LENGTH);
    while(TRUE) {

        if((is_contains_char = readSentence(temp, hostname)) == '|') {
            if((readSecondSentence(temp2)) == ERROR) {
                printf("data missing\n");
            }
            else {
                send = countWords(temp);
                send2 = countWords(temp2);
                execWithLine(send, send2);
            }
        }
        else if(is_contains_char == '>') {
            if(( readSecondSentence(temp2))== ERROR) {
                printf("have no file\n");
            }
            else {
                send = countWords(temp);
                send2 = countWords(temp2);
                execOperatorW(send, send2);
            }
        }
        else if(is_contains_char == '<') {
            if(( readSecondSentence(temp2)) == ERROR){
                printf("have no file\n");
            }
            else {
                send = countWords(temp);
                send2 = countWords(temp2);
                execOperatorR(send, send2);
            }
        }
        else if(is_contains_char == 'e') {
            readSecondSentence(temp2);
            printf("data missing\n");
        }
        else if(is_contains_char == 'n'){
            send = countWords(temp);
            if (strcmp(send[0], "done") == 0) {
                for (i = 0; (i < 511 )& send[i] != NULL; i++)//release send
                {
                    free(send[i]);
                    send[i] = NULL;
                }
                free(send);
                send = NULL;
                exit(0);
            }
            if (strcmp(send[0], "bg") == 0) {
                kill(-1, SIGCONT);
            } else {
                pid = fork();
                if (pid == ERROR) { // the "fork" failed
                    printf("Can not creat new process");
                    for (i = 0; (i < 511 )& send[i] != NULL; i++)//release send
                    {
                        free(send[i]);
                        send[i] = NULL;
                    }
                    free(send);
                    send = NULL;
                    exit(1);
                } else if (pid == 0) { // if this is a son
                    //signal(SIGTSTP,sig_handler);
                    if (execvp(send[0], send) == ERROR) { // can not execute the command
                        printf("%s: command not found\n", send[0]);
                        for (i = 0; (i < 511 )& send[i] != NULL; i++)//release send
                        {
                            free(send[i]);
                            send[i] = NULL;
                        }
                        free(send);
                        send = NULL;
                        exit(0);
                    }
                } else { // this is a father
                    pause(); // wait to the son to finish execution
                    for (i = 0; (i < 511 )& send[i] != NULL; i++)//release send
                    {
                        free(send[i]);
                        send[i] = NULL;
                    }
                    free(send);
                    send = NULL;
                }
            }
        }
    }
    return 0;
}


int num_of_words(char * str) {
    // the function counts the number of words in a sentence (string)
    int count = 0, i;
    for (i = 0;str[i] != '\0';i++)
    {
        if (str[i] == ' ' && str[i+1] != ' ' && str[i+1] != '\0') // check if a new word begin in the next index
            count++;
    }
    if (str[0] != ' ') { // if there is a word in the beginning of the sentence
        count++;
    }
    return count;
}

 char readSentence(char read[], char * hostname) {
    // the method reads a sentence from the user
    int i=0;
    char ch;
    do {
        i = 0;
        print_command_line(hostname);
        do {
            ch = getchar(); // get the next character from the user
            if (ch == '|' || ch == '<' || ch =='>') {
                read[i] = '\0';
                if(i == 0) {
                    fflush(stdin);
                    return 'e';
                }
                return ch;
            }
            if (ch != '\n') // if the user didn't pressed 'enter'
            {
                read[i] = ch; //put the char into the return string
            }
            if (ch != ' ' || i != 0) { //check if the character at first place is not a 'space' or 'enter'
                i++;
            }
        } while (ch != '\n' && i <= 510); // until the user pressed 'enter'
    }
    while (i <= 1);
        read[i-1]='\0'; //delete the '\n'
    return 'n';
}

int readSecondSentence(char read[]) {
    // the method reads a sentence from the user
    int i=0;
    char ch;
        do {
            ch = getchar(); // get the next character from the user
            if (ch != '\n') // if the user didn't press 'enter'
            {
                read[i] = ch; //put the char into the return string
            }
            if (ch != ' ' || i != 0) { //check if the character at first place is not a 'space' or 'enter'
                i++;
            }
        } while (ch != '\n' && i <= 510); // until the user pressed 'enter'
    if(i<=1)
        return ERROR;
    read[i-1]='\0'; //delete the '\n'
    return TRUE;
}

char** countWords(char * array ){
    //the method gets a string, and return an array of the words from the string
    int  count,  j = 0;
    char* temp;
    char ** newC;
    count = num_of_words(array);
    newC = (char**)malloc(sizeof(char*)*(count+1)); // allocate memory
    if(newC == NULL) {
        printf("could not allocate");
        exit(EXIT_FAILURE);
    }
    temp = strtok (array," "); // take a word from the sentence
    while (temp != NULL)
    {
        newC[j] = (char*)malloc(sizeof(char)*strlen(temp)+1); // allocate memory for the word
        if(newC[j] ==NULL) {
            printf("could not allocate");
            exit(EXIT_FAILURE);
        }
        strcpy(newC[j], temp);
        temp = strtok (NULL, " ");
        j++;
    }
    newC[j] = NULL;
    return newC;
}

void print_command_line(char * hostname) {
     // the function gets the host name, and prints the command line
    printf("%s@%s&: ", getlogin(), hostname);
}

void sig_handler(int signum) {
    if(signum==SIGCHLD)
    {
        waitpid(-1,NULL,WNOHANG);
    }
}

void execWithLine(char **command1, char **command2) {
//void execWithLine(){
    int fd[2];
    int i = 0;
    pid_t childPid, childPid2;
    if (pipe(fd) < 0) {
        perror("can not pipe");
        exit(EXIT_FAILURE);
    }

    if ((childPid = fork()) == ERROR) {
        perror("can not pipe");
        exit(EXIT_FAILURE);
    }

    if (childPid == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        if ((execvp(command1[0], command1)) < 0) {
            fprintf(stderr, "command 1 not found\n");
            exit(1);
        }
    }
    else // if I am the father
    {
        pause();
        if ((childPid2 = fork()) < 0) {
            printf("Could not fork\n");
            exit(EXIT_FAILURE);
        }
        if (childPid2 == 0) {
            close(fd[1]);
            dup2(fd[0], 0);
            close(fd[0]);
            if ((execvp(command2[0], command2))< 0) {
                printf("command 2 not found\n");
                fflush(stdin);
                exit(1);
            }
        }
        else {
            // parent executing, waiting for two children
            close(fd[0]);
            close(fd[1]);
            pause();
            for (i = 0; (i < 511 )& command1[i] != NULL; i++)//release send
            {
                free(command1[i]);
                command1[i] = NULL;
            }
            for (i = 0; i < 511 & command2[i] != NULL ; i++)//release send
            {
                free(command2[i]);
                command2[i] = NULL;
            }
            free(command1);
            command1 = NULL;
            free(command2);
            command2 =NULL;
        }
    }
}

void execOperatorW(char **command1, char **command2) {
    pid_t pid;
    int output_fds, i;
    if((pid = fork()) == ERROR) {
        printf("Could not fork\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        output_fds = open(command2[0] , O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if(output_fds == ERROR) {
            fprintf(stderr, " could not open file");
            exit(1);
        }
        dup2(output_fds, STDOUT_FILENO);
        if ((execvp(command1[0], command1)) < 0) {
            fprintf(stderr, "command 1 not found\n");
            exit(1);
        }
    }
    else {
        pause();
        close(output_fds);
        for (i = 0; (i < 511 )& command1[i] != NULL; i++)//release send
        {
            free(command1[i]);
            command1[i] = NULL;
        }
        for (i = 0; i < 511 & command2[i] != NULL ; i++)//release send
        {
            free(command2[i]);
            command2[i] = NULL;
        }
        free(command1);
        command1 = NULL;
        free(command2);
        command2 =NULL;
    }
}

void execOperatorR(char **command1, char **command2) {
    pid_t pid;
    int output_fds, i;
    if((pid = fork()) == ERROR) {
        printf("Could not fork\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        output_fds = open(command2[0], O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if(output_fds == ERROR) {
            fprintf(stderr, " could not open file");
            exit(1);
        }
        dup2(output_fds, STDIN_FILENO);
        if ((execvp(command1[0], command1)) < 0) {
            printf("command 1 not found\n");
            exit(1);
        }
    }
    else {
        pause();
        for (i = 0; (i < 511 )& command1[i] != NULL; i++)//release send
        {
            free(command1[i]);
            command1[i] = NULL;
        }
        for (i = 0; i < 511 & command2[i] != NULL ; i++)//release send
        {
            free(command2[i]);
            command2[i] = NULL;
        }
        free(command1);
        command1 = NULL;
        free(command2);
        command2 =NULL;
    }
}
