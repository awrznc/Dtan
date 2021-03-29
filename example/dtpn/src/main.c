#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#define SLASH_CHARACTOR '/'
#define GIT_REPOSITORY_SYMBOL ".git"

#define ERR_CANT_FIND_DIR   1
#define ERR_NO_STDOUT       2
#define ERR_READ_DIR        3

unsigned int get_charactor_count(
    char* directoryPath,
    unsigned int length,
    char searchCharactor
) {
    unsigned int count = 0;
    for(int index = 0; index < length; index++) {
        if(directoryPath[index] == searchCharactor) count++;
    }
    return count;
}

char equal_string(char* a, char* b) {
    while (*a != '\0' || *b != '\0' ) {
        if (*a != *b || *a == '\0' || *b == '\0') return 0;
        a++; b++;
    }
    return 1;
}

/**
  * @param argv Exclude path.
  * @return
  *     0: Success! Found the project.
  *     1: Can't find the project directory.
  *     2: Found the project, but no stdout.
  *     3: Read directory error.
  */
int main( int argc, char *argv[] ) {
    char current_path[PATH_MAX] = { 0 };
    getcwd(current_path, PATH_MAX);
    unsigned int count = get_charactor_count(current_path, PATH_MAX, SLASH_CHARACTOR);

    char relative_path[PATH_MAX] = "./";
    for (int index = 0; count > index; index++) {
        DIR *dir = opendir(relative_path);
        if (dir == NULL) return ERR_READ_DIR;

        for(
            struct dirent *dir_entries = readdir(dir);
            dir_entries != NULL;
            dir_entries = readdir(dir)
        ) {
            // Check the existence of the directory.
            if (!equal_string(dir_entries->d_name, GIT_REPOSITORY_SYMBOL)) continue;

            // Check the exclude path.
            char absolute_path[PATH_MAX+1];
            realpath(relative_path, absolute_path);
            if (argc == 2 && equal_string(argv[1], absolute_path)) return ERR_NO_STDOUT;

            // Stdout absolute path.
            printf("%s\n", absolute_path);
            if (dir != NULL) closedir(dir);

            return 0;
        }
        if (dir != NULL) closedir(dir);

        // Update path string.
        sprintf(relative_path, "%s../", relative_path);
    }
    return ERR_CANT_FIND_DIR;
}
