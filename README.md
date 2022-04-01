# Mini-shell

==Description==
The program simulates a linux shell.

functions:
There are two help methods:
char readSentence(char read[], char * hostname)
the function reads a sentence from the user until enter or |,<,> entered
int readSecondSentence(char read[])
if "readSentence" identified a special character, this function will be called to read the rest of the sentence
int num_of_words(char * str)
the method counts how many words are in a sentence
char** countWords(char * array)
the function gets a string and split it to words array
void print_command_line()
the function prints the command line according to the host name
void sig_handler(int signum)
the method hendel some signals
void execWithLine(char **command1, char **command2)
this function will do the execution if pipe is needed
void execOperatorW(char **command1, char **command2)
this function will do the execution if the character '>' has entered
void execOperatorR(char **command1, char **command2)
this function will do the execution if the character '<' has entered
==Program Files==
main.c

==How to compile?==
compile: gcc mini_shell.c -o main
run: ./main

==Input:==
You can enter any command  you want as follows:
regular, such as: echo hello world
with pipe like: ls -l | wc -l
or using a file, for example: ls -l> out
wc -l<out
note: file can be only one word
at the end of typing the sentence press Enter to receive the output.

==Output:==
if the input was: echo hello world"
the output will be:
hello world
ls -l | wc -l- prints the sum of lines ls -l have( num of folders)
