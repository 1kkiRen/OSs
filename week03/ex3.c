#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
#define MAX_DIR 64
#define MAX_PATH 2048
#define MAX_DATA 1024

struct Directory;
struct File;

typedef struct File
{
    short int id;
    char file_name[MAX_NAME];
    int size;
    char *data;
    struct Directory *directory;
} File;

typedef struct Directory
{
    char name[MAX_NAME];
    File files[256];
    struct Directory *directories[MAX_DIR];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];

} Directory;

void add_dir(Directory *parent, const char* name){
    Directory *child = (Directory *) malloc(sizeof(Directory));
    if (child == NULL){
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(child->name, name);
    strcpy(child->path, parent->path);
    strcat(child->path, name);
    strcat(child->path, "/");
    child->nf = 0;
    child->nd = 0;

    parent->directories[parent->nd] = child;
    parent->nd++;
}

void add_file(File* file, Directory* directory){
    file->directory = directory;

    directory->files[directory->nf] = *file;
    directory->nf++;
}

File* initialise_file(const char* name){
    File *file = (File *) malloc(sizeof(File));
    if (file == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    strcpy(file->file_name, name);
    file->size = 0;
    file->data = (char *) malloc(MAX_DATA * sizeof(char));
    if (file->data == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    return file;
}

void overwrite_to_file(File* file, const char* str){
    strcpy(file->data, str);
    file->size = strlen(str);
}

void append_to_file(File* file, const char* str){
    strcat(file->data, str);
    file->size += strlen(str);
}

void printp_file(File* file){
    printf("%s%s\n", file->directory->path, file->file_name);
}

void print_file(File* file){
    printf("%s\n", file->data);
}

int main(void){
    Directory *root = (Directory *) malloc(sizeof(Directory));
    if (root == NULL){
        printf("Memory allocation failed\n");
        return 1;
    }

    strcpy(root->name, "/");
    strcpy(root->path, "/");
    root->nf = 0;
    root->nd = 0;

    add_dir(root, "home");
    add_dir(root, "bin");

    add_file(initialise_file("bash"), root->directories[1]);
    add_file(initialise_file("ex3_1.c"), root->directories[0]);
    add_file(initialise_file("ex3_2.c"), root->directories[0]);

    overwrite_to_file(&root->directories[0]->files[0], "int printf(const char * format, ...);");
    overwrite_to_file(&root->directories[0]->files[1], "//This is a comment in C language");

    overwrite_to_file(&root->directories[1]->files[0], "Bourne Again Shell!!");

    append_to_file(&root->directories[0]->files[0], "int main(){printf(”Hello World!”)}");

    printp_file(&root->directories[0]->files[0]);
    printp_file(&root->directories[0]->files[1]);
    printp_file(&root->directories[1]->files[0]);

    free(root);

    return 0;
}