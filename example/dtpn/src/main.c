#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>

#define DOT_SYMBOL "."
#define GIT_REPOSITORY_SYMBOL ".git"

#ifdef _WIN32
#define SLASH_CHARACTOR '\\'
#define FULLPATH_SIZE MAX_PATH
#elif __APPLE__
#define SLASH_CHARACTOR '/'
#define FULLPATH_SIZE PATH_MAX
#endif

unsigned int get_charactor_count(
    char* string,
    unsigned int length,
    char searchCharactor
) {
    unsigned int count = 0;
    for(int index = 0; index < length; index++) {
        if(string[index] == searchCharactor) count++;
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

char get_fullpath(const char* relative_path, char* absolute_path) {
#ifdef _WIN32
    _fullpath(absolute_path, relative_path, FULLPATH_SIZE);
#elif __APPLE__
    realpath(relative_path, absolute_path);
#endif
    return 1;
}

#define SUCCESS_FOUND_PROJECT   0
#define ERR_CANT_FIND_DIR       1
#define ERR_NO_STDOUT           2
#define ERR_READ_DIR            3

/**
  * @param argv Exclude path.
  * @return
  *     0: Success! Found the project.
  *     1: Can't find the project directory.
  *     2: Found the project, but no stdout.
  *     3: Read directory error.
  */
int main( int argc, char *argv[] ) {
    char current_path[FULLPATH_SIZE] = { 0 };
    getcwd(current_path, FULLPATH_SIZE);
    unsigned int count = get_charactor_count(current_path, FULLPATH_SIZE, SLASH_CHARACTOR);

    char relative_path[FULLPATH_SIZE] = DOT_SYMBOL;
    sprintf(relative_path, "%s%c", relative_path, SLASH_CHARACTOR);
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
            closedir(dir);

            // Check the exclude path.
            char absolute_path[FULLPATH_SIZE+1];
            get_fullpath(relative_path, absolute_path);
            if (argc == 2 && equal_string(argv[1], absolute_path)) return ERR_NO_STDOUT;

            // Stdout absolute path.
            printf("%s\n", absolute_path);

            return SUCCESS_FOUND_PROJECT;
        }
        if (dir != NULL) closedir(dir);

        // Update path string.
        sprintf(relative_path, "%s%s%s%c", relative_path, DOT_SYMBOL, DOT_SYMBOL, SLASH_CHARACTOR);
    }
    return ERR_CANT_FIND_DIR;
}
