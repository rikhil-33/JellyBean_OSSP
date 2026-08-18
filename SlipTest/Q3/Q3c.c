#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main() {
    const char *restricted_dir = "./restricted_folder";

    // Create folder with Read-only permission (0400: no execute/search permission)
    mkdir(restricted_dir, 0700);
    chmod(restricted_dir, 0400); 

    DIR *dir = opendir(restricted_dir);
    if (dir == NULL) {
        // VFS blocks access and sets errno
        perror("opendir failed");
    } else {
        struct dirent *entry = readdir(dir);
        if (entry == NULL && errno == EACCES) {
            perror("readdir failed");
        }
        closedir(dir);
    }

    // Cleanup
    chmod(restricted_dir, 0700);
    rmdir(restricted_dir);
    return 0;
}