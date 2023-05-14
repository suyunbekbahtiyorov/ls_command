#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int main() {
    DIR* dir;
    dir = opendir("ex00");
    struct dirent* entry;
    while((entry = readdir(dir))){
        printf("%s\n",entry->d_name);
    }
    closedir(dir);
}

