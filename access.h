//Ben Sherington
//CS483-02: Project1
#ifndef ACCESS_H
#define ACCESS_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>

int check_access(char* file,int readAccess,int writeAccess);
int check_owner(char* file, uid_t owner_id);
void create_file_put(char* source, char* destination);
void create_file_get(char* source, char* destination);
void access_parser(char* file, uid_t real_id, uid_t owner_id);
char* get_username(uid_t real_id);


#endif
