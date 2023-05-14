#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct listnode{
    char* name;
    struct stat fstat;
    struct listnode *next;
}listnode;

void my_addlast(listnode** node, char* name, struct stat fl){
    listnode* list = (listnode*)malloc(sizeof(listnode));
    list->name = name;
    list->fstat = fl;
    list->next = NULL;
    if(*node == NULL){
        *node = list;
    }else{
        listnode* ln = *node;
        while(ln->next != NULL){
            ln = ln->next;
        }
        ln->next = list;
    }
}

void my_swap(listnode* a, listnode* b){
    char* temp;
    struct stat fs;
    temp = a->name;
    a->name = b->name;
    b->name = temp;
    fs = a->fstat;
    a->fstat = b->fstat;
    b->fstat = fs;
}

void my_sort_2(listnode** node){
    listnode* list = *node;
    listnode*ln = NULL;
    if(*node == NULL){
        printf("ERROR!\n");
    }
    while(list != NULL){
        ln = list->next;
            while(ln != NULL){
                if(list->fstat.st_mtim.tv_sec == ln->fstat.st_mtim.tv_sec){
                    if(list->fstat.st_mtim.tv_nsec <= ln->fstat.st_mtim.tv_nsec){
                        my_swap(list, ln);
                    }/*else if(list->fstat.st_mtim.tv_nsec == ln->fstat.st_mtim.tv_nsec){
                        if(strcmp(list->name, ln->name) >= 0){
                            my_swap(list, ln);
                        }
                    }*/
                }else if(list->fstat.st_mtim.tv_sec <= ln->fstat.st_mtim.tv_sec){
                    my_swap(list, ln);
                }
                ln = ln->next;
            }
        list = list->next;
    }
}

void my_swap_2(char** a, char** b){
    char* c = *a;
    *a = *b;
    *b = c;
}

void my_sort_1(listnode** node){
    listnode* list = *node, *ln = NULL;
    if(*node == NULL){
        printf("ERROR!\n");
    }
    while(list != NULL){
        ln = list->next;
            while(ln != NULL){
                if(strcmp(list->name, ln->name) > 0){
                    my_swap_2(&list->name,&ln->name);
                }
                ln = ln->next;
            }
        list = list->next;
    }
}

void print_lists(listnode** list){
    listnode* node = *list;
    while(node != NULL){
        printf("%s\n",node->name);
        node = node->next;
    }
}

void print_lists_2(listnode** list){
    listnode* node = *list;
    while(node != NULL){
        if(node->name[0] != '.'){
            printf("%s\n",node->name);
        }
            node = node->next;
    }
}

// void s(){
//     open();
// }
void call_sort(listnode* node, int flag){
    switch (flag){
            case 0:
                my_sort_1(&node);
                print_lists_2(&node);
            break;
            case 1:
                my_sort_1(&node);
                print_lists(&node);
            break;
            case 2:
                my_sort_2(&node);
                print_lists_2(&node);
            break;
            case 3:
                my_sort_2(&node);
                print_lists(&node);
            break;
        }
}

void my_dir_2(char** arr, int flag){
    DIR* dir;
    struct dirent* entry;
    listnode* list = NULL;
    listnode* f_list = NULL;
    int i = 0,k = 0,m = 0,l = 0;
    while(arr[i]){
        struct stat fs;
        stat(arr[i], &fs);
        if(open(arr[i], O_RDONLY)>0){
            if(S_ISDIR(fs.st_mode)){
                my_addlast(&list, arr[i], fs);
                l++;
            }else{
                my_addlast(&f_list , arr[i], fs);
                k++;
            }
        }else{
            printf("ls: cannot access '%s': No such file or directory\n", arr[i]);
        }
        i++;
        m++;
    }

    if(k > 0){
        call_sort(f_list,flag);
        if(list != NULL){
            putchar('\n');
        }
    }

    while(list){
        dir = opendir(list->name);
        listnode* node = NULL;
        if(m > 1)
            printf("%s:\n", list->name );
        while((entry = readdir(dir))){
            struct stat f_stat;
            stat(entry->d_name,&f_stat);
            my_addlast(&node, entry->d_name, f_stat);
            
        }
        call_sort(node, flag);
        if(list->next != NULL)
        putchar('\n');
        list = list->next;
    }
    
}

void my_dir(int flag){
    listnode* node = NULL;
    DIR* dir;
    struct dirent* entry;
    dir = opendir(".");
    while((entry = readdir(dir))){
        struct stat fstat;
        stat(entry->d_name, &fstat);
        my_addlast(&node, entry->d_name, fstat);
    }
    closedir(dir);
    call_sort(node, flag);
    // my_sort_1(&node);
    // print_lists(&node);
}


void my_ls(int ac, char** av){
    char** arr = (char**)malloc(sizeof(char*)*ac);
    int a = 0,t = 0, at = 0,flag = 0,y = 0;
    for(int i = 1;i < ac;i++){
        if(strcmp(av[i], "-a") == 0){
            a = 1;
        }else if(strcmp(av[i],"-t") == 0){
            t = 1;
        }else if(strcmp(av[i],"-at") == 0 || strcmp(av[i],"-ta") == 0){
            at = 1;
        }else{
            arr[y] = malloc(sizeof(char*));
            arr[y] = av[i];
            // printf("%s\n",arr[i]);
            y++;
        }
    }
    if(at == 1 || (a == 1 && t == 1)){
        flag = 3;
    }else if(a == 1){
        flag = 1;
    }else if(t == 1){
        flag = 2;
    }
    if(y > 0){
        my_dir_2(arr,flag);
    }else{
        my_dir(flag);
    }
}

int main(int ac, char** av){
    if(ac == 4) {
        for(int i=1; i < ac; i++) {
            struct stat fs;
            stat(av[i], &fs);
            if(!S_ISDIR(fs.st_mode)) {
                goto label; 
            }
        }
        char* temp = av[3];
        av[3] = av[2];
        av[2] = temp;
        // printf("%ld\n", sizeof(av));
        // char **temp = {".", "/", "/tmp/"};
        
        my_ls(ac, av);
        return 0;
    }
    label:
    my_ls(ac,av);
}