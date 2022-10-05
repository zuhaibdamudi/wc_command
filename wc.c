// characters (-c)
// lines (-l)
// words (-w)
// special characters (-sc)
// comment lines (-cl)
// paragraphs (-p)
// bytes (-b)
// occurrences of particular word (-wrd)
// longest line (-ll)

// argv[1] = sample.txt
// argv[2] = "-l"
// argv[3] = word to count occurrence of

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#define BUFFER_SIZE 1000

int main(int argc, char*argv[])
{
    if(argc < 3)
    {
        printf("Not enough arguments\n");
        exit(0);
    }

    int fd;
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        char buf[] = "Cannot Open File!\n";
        write(1, buf, sizeof(buf));
        exit(0);
    }

    FILE *fptr;
    fptr = fopen(argv[1], "r");
    if (!fptr)
    {
        char buf[] = "Cannot Open File!\n";
        write(1, buf, sizeof(buf));
        exit(0);
    }

    if(strcmp(argv[2], "-c") == 0)      //number of characters
    {
    	int count = 0;
        char c;
    	while((read(fd, &c, sizeof(c))) > 0)
        {
            count++;
        }
    	printf("Number of Characters: %d\n", count);
    }
    else if(strcmp(argv[2], "-sc") == 0)        //number of special characters
    {
        int count = 0;
        char c;

        while((read(fd, &c, sizeof(c))) > 0)
        {
            if(isspace(c) == 0 && isalnum(c) == 0)
                count++;
        }
        printf("Number of special characters: %d\n",count);
    }
    else if(strcmp(argv[2], "-b") == 0)     //number of bytes
    {
        fseek(fptr, 0L, SEEK_END); 
        long int count = ftell(fptr);
        
        printf("Number of Bytes: %ld \n",count);
    }
    else if(strcmp(argv[2], "-w") == 0)     //number of words
    {
        int count = 0;
        char ca;

        while((read(fd, &ca, sizeof(ca))) > 0)
        {
            if(ca == ' ' || ca == '\n')
            {
                char cb = read(fd, &cb, sizeof(cb));
                if((ca == ' ' && cb == ' ') || (ca == '\n' && cb == '\n'))
                {
                    read(fd, &ca, sizeof(ca));
                    count++;
                }
                count++;
            }
        }
        printf("Number of Words: %d\n", count);
    }
    else if(strcmp(argv[2], "-p") == 0)     //number of paragraphs
    {
        int count = 1;
        char ca, cb;

        //empty file
        if(fgetc(fptr) == EOF)
        {
            printf("Number of Paragraphs: 0\n");
            exit(0);
        }
        
        while((read(fd, &ca, sizeof(ca))) > 0)
        {
            if(ca == '\n')
            {
                read(fd, &cb, sizeof(cb));
                if(cb == '\n')
                    count++;
                read(fd, &ca, sizeof(ca));
            }
        }
        printf("Number of Paragraphs: %d\n", count);
    }
    else if(strcmp(argv[2], "-ll") == 0)        //print longest line
    {
        char line[200], max[200]=" ";
        
        while(fgets(line, sizeof(line), fptr)!=NULL)
        {
            if(strlen(max) < strlen(line))
                strcpy(max, line);
        }
        printf("Longest line in the file is: \"%s\"\n", max);
    }
    else if(strcmp(argv[2], "-wrd") == 0)       //occurrence of a particular word
    {
        if(argc == 2)
        {
            printf("Not enough arguments\n");
            exit(0);
        }

        char str[BUFFER_SIZE];
        char *pos;
        int index;
        int count = 0;

        while ((fgets(str, BUFFER_SIZE, fptr)) != NULL)
        {
            index = 0;
            while ((pos = strstr(str + index, argv[3])) != NULL)
            {
                index = (pos - str) + 1;
                count++;
            }
        }
        printf("The word '%s' is found %d times in the file\n", argv[3], count);
    }
    else if(strcmp(argv[2], "-l") == 0)     //number of lines
    {
        char c;
        int count = 0;

        while((read(fd, &c, sizeof(c))) > 0)
        {
            if(c == '\n' || c== '\0')
                count++;
        }
        printf("Number of Lines: %d\n",count);
    }
    else if(strcmp(argv[2], "-cl") == 0)        //number of comment lines
    {
        char ca, cb;
        int count=0;

        while((read(fd, &ca, sizeof(ca))) > 0)
        {
            if(ca == '/')
            {
                read(fd, &cb, sizeof(cb));
                if(cb == '/')
                {
                    while(ca!= '\n')
                        read(fd, &ca, sizeof(ca));
                    count++;
                }
                else if(cb == '*')
                {
                    do
                    {
                        while(ca!='*')
                        {
                            read(fd, &ca, sizeof(ca));
                        }
                        read(fd, &ca, sizeof(ca));
                    }
                    while(ca!= '/');
                    count++;
                }
            }
        }
        printf("Number of Comments: %d\n", count);
    }
    else
    {
        printf("%s is an invalid argument\n", argv[2]);
        exit(0);
    }
    fclose(fptr);
    close(fd);
}