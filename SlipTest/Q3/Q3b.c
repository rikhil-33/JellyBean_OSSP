/*
CASE STUDY – 3: Executing a Linux Command
A user enters the following command in a Linux terminal:

</> Bash
ls -l /home/student
The shell accepts the command and displays the contents of the directory.

3A. 5 Marks
Arrange the major components involved in executing the command in the order through which control passes, starting from the user and shell and ending with the hardware resources. Relate each component to its position in the Linux software stack.

3B. 7 Marks
Construct an investigation plan to study the execution of the ls command. Your plan should identify:
the user-space program involved, 
the system calls used to access directory information, 
the transition between User Mode and Kernel Mode, 
the relevant Linux kernel services, 
and the role of the file-system layer in producing the output. 

3C. 3 Marks
A user tries to execute ls on a directory for which they have no execute/search permission. Analyze how Linux access-control mechanisms affect the result.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

int main() {
    const char *dir_path = "/home/student";

    // 1. System Call: openat()
    int fd = open(dir_path, O_RDONLY | O_DIRECTORY);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }

    // 2. System Call: getdents64() wrapped via opendir/readdir
    DIR *dir = fdopendir(fd);
    struct dirent *entry;

    printf("Reading directory contents...\n");
    while ((entry = readdir(dir)) != NULL) {
        struct stat file_stat;
        
        // 3. System Call: fstatat() to fetch metadata for -l flag
        if (fstatat(fd, entry->d_name, &file_stat, AT_SYMLINK_NOFOLLOW) == 0) {
            printf("Inode: %ld | Mode: %o | Size: %ld bytes | Name: %s\n",
                   entry->d_ino, file_stat.st_mode, file_stat.st_size, entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}