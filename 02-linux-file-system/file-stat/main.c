#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#define PATH_NAME "hello.txt"


int main()
{
    
    char* pn = PATH_NAME;
    struct stat sb;
    
    if (stat(pn, &sb) == 0)
    {
        printf("\nFile Name: %s\n", pn);
        printf("\nFile Type: ");
        switch (sb.st_mode & S_IFMT) {
            case S_IFBLK:   printf("block device\n");       break;
            case S_IFCHR:   printf("character device\n");   break;
            case S_IFDIR:   printf("directory\n");          break;
            case S_IFIFO:   printf("FIFO/pipe\n");          break;
            case S_IFLNK:   printf("symlink\n");            break;
            case S_IFREG:   printf("regular file\n");       break;
            case S_IFSOCK:  printf("socket\n");             break;
            default:        printf("unknown?\n");
        }

        printf("\nFile size: %jd bytes\n\n", (intmax_t)sb.st_size);

        printf("Last status change:     %s\n", ctime(&sb.st_ctim.tv_sec));
        printf("Last file access:       %s\n", ctime(&sb.st_atim.tv_sec));
        printf("Last file modification: %s\n", ctime(&sb.st_mtim.tv_sec));
    }
    else
    {
        printf("Failed to get an information from file\n");
    }

    return 0;
}