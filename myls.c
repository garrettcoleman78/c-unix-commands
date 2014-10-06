// Preprocessor instructions
#include <stdio.h> // include standard I/O functions
#include <dirent.h> // include standard utilities (exit)
#include <stdlib.h> // include functions for number conversion and storage allocation
#include <string.h> // include string handling functions
#include <unistd.h> // include POSIX API (getopt)


#define _GNU_SOURCE // as per assignment instruction

/* 
Main declaration gives program the facility to use/manipulate command line 
arguments.
argc parameter is the count of total command line arguments passed (including 
name of executable as first argument). 
argv parameter is the array of character string of each command line argument 
passed. 
*/
int main(int argc, char* argv[]){

    int opt; // temporary variable for return code of getopt
    int longlist = 0; // variable to store if -l option entered
    const char* dirtodisplay = NULL; // char pointer that will store directory to display

    /* 
    getopt() function parses the command-line arguments and gets the next 
    option argument from the argument list specified by the argv and argc arguments.
    An element of argv that starts with '-' is an option element. If getopt() is 
    called repeatedly, it returns successively each of the option characters 
    from each of the option elements.
    We use getopt to search for the -l option, including a default error message
    if another option is entered. When there are no more option characters in the
    parsed command line arguments, getopt() returns -1, terminating the while loop.
    */
    while ((opt = getopt(argc, argv, "l")) != -1){
        switch(opt){ // checks the code returned from getopt()
            case 'l':
                longlist = 1; // if -l option was entered then longlist is true
                break;
            default: // if wrong option, error message displayed and program terminates
                fprintf(stderr, "Correct usage: %s -l\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    /* 
    optind is the index of the next element to be processed in argv.
    If getopt() has returned -1 above, then there are no more option 
    characters, and then optind is the index in argv of the first argv-element 
    that is not an option.
    if optind is less than argc then there are arguments left after parsing
    and the first argument that is not an option (i.e. the directory name 
    specified by input argument) is stored in dirtodisplay, otherwise dirtodisplay 
    is assigned the current working directory (i.e. if no argument has been passed 
    for directory name)
    */
    if (optind < argc){ 
        dirtodisplay = argv[optind];
    }
    else dirtodisplay = ".";
   
    
    DIR* dp; // Create a directory pointer dp
    struct dirent*  ep; // Create a pointer ep to the current directory

    /* 
    open a directory stream for directory stored in dirtodisplay and assign
    the stream to the dp pointer
    */
    dp = opendir(dirtodisplay);
    if (dp != NULL) { // if a directory exists within dirtodisplay variable and is readable
        
        /* 
        readdir function retrieves the directory entries from the stream, and 
        while loop iterates until stream has been read 
        */
        while ((ep = readdir(dp)) != NULL){
            if (longlist){ // if -l option was entered
                char type; // variable to store directory entry type

                /* 
                switch condition is each directory type within the directory,
                which are accessed by the d_type fields within the dirent 
                structure using -> 
                */
                switch (ep->d_type){ // d_type is type of entry

                    case DT_DIR: type = 'd'; break; // for directories type is assigned 'd'

                    case DT_REG: type = 'f'; break; // for files type is assigned 'f'

                    case DT_LNK: type = 'l'; break; // for links type is assigned 'l'

                    default: type = 'o'; break; // for other types type is assigned 'o'

                }

                /* 
                we do not display the current or parent directory which are included
                in all directories as . and .. directories respectively, but are hidden
                by the ls command. We use the strcmp function to compare each entry name
                (stored in the d_name field within the dirent structure with the . and 
                .. strings), and if the comparison does not return a 0 value then the d_name)
                does not relate to the current or parent directory.
                */
                if (strcmp(ep->d_name, ".")!=0 && strcmp(ep->d_name, "..")!=0){
                    // display the type and name of all other entries in the directory stream 
                    printf("%c %s\n", type, ep->d_name);
                }
            }

            else{ // if -l option was not entered we just display the entry name

                if (strcmp(ep->d_name, ".")!=0 && strcmp(ep->d_name, "..")!=0){
                    printf("%s\n", ep->d_name);
                }
            }
        }
        closedir(dp); // close the directory stream
    }
    return 0; // close the program
}