//Ben Sherington
//CS483-02: Project1
#include "access.h"

uid_t real_id;
uid_t owner_id;

int check_access(char* file,int read_access,int write_access){
  struct stat* source_stat = NULL;
  if(lstat(file, source_stat) != 0){
    return -1;
  }

  if(source_stat->st_uid != owner_id){
    return -1;
  }

  if(read_access == 1){
    if((source_stat->st_mode & (S_IFMT | S_IRUSR)) == (S_IFREG | S_IRUSR)){
      return 0;
    }
    else{
      return -1;
    }
  }

  if(write_access == 1){
    if((source_stat->st_mode & (S_IFMT | S_IRUSR)) == (S_IFREG | S_IWUSR)){
      return 0;
    }
    else{
      return -1;
    }
  }
  return -1;
}
int check_owner(char* file, uid_t owner_id){
  struct stat* access_stat = NULL;
  if(lstat(file, access_stat) !=0){
    return -1;
  }
  if(access_stat->st_uid != owner_id){
    return -1;
  }
  if(access_stat->st_mode & (S_IRWXG | S_IRWXO)){
    return -1;
  }
  return 0;
}
void access_parser(char* file, uid_t real_id, uid_t owner_id){
  //this is what parses the file to check for the user name and permissions
  int buffer_size = 256;
  char string_buffer[buffer_size];
  char file_user[64];
  char* user_name = get_username(real_id);
  char permissions = "r" || "w" || "b";
  int line_length;
  seteuid(owner_id);
  FILE* access_file_pointer = fopen(file,"r");
  seteuid(real_id);
    while(fgets(string_buffer, buffer_size, access_file_pointer) != NULL){
      if(string_buffer[0] == '#'){
        continue;
      }
      line_length = sscanf(string_buffer,"%s %c [^\n]",file_user, &permissions);
      if(line_length !=2){
        exit(-1);
      }else{
        if(strcmp(user_name, file_user)== 0){
          if((strcmp(&permissions, "r") == 0) || (strcmp(&permissions, "w") == 0) || (strcmp(&permissions, "b") == 0)){
              fclose(access_file_pointer);
          }
        }
      }
    }

}

char* get_username(uid_t real_id){
  struct passwd* access_user = getpwuid(real_id);

  return access_user->pw_name;
}

void create_file_get(char* source, char* destination){
  char* prompt_choice = NULL;
  struct stat* create_stat = NULL;
  if(lstat(destination, create_stat) == 0){
    if(S_ISREG(create_stat->st_mode)){
      if(create_stat->st_uid == real_id){
        printf("Would you like to overwrite the file (y/n)?: \n");
        fgets(prompt_choice, 5, stdin);
        if(strcmp(prompt_choice, "n")== 0){
          exit(-1);
        }
        else if(strcmp(prompt_choice, "y") == 0){
          seteuid(owner_id);
          int source_path = open(source, O_RDONLY);
          seteuid(real_id);
          int dest_path = open(destination, O_WRONLY);
          sendfile(dest_path,source_path, NULL, sizeof(source));
          close(source_path);
          close(dest_path);
        }
      }
    }
    else{
      seteuid(owner_id);
      int source_path = open(source, O_RDONLY);
      seteuid(real_id);
      int dest_path = open(destination, O_CREAT);
      sendfile(dest_path,source_path, NULL, sizeof(source));
      close(source_path);
      close(dest_path);
    }

  }
}

void create_file_put(char* source, char* destination){
  char* prompt_choice = NULL;
  struct stat* create_stat = NULL;
  if(lstat(destination, create_stat) == 0){
    if(S_ISREG(create_stat->st_mode)){
      if(create_stat->st_uid == owner_id){
        printf("Would you like to overwrite the file (y/n)?: \n");
        fgets(prompt_choice, 5, stdin);
        if(strcmp(prompt_choice, "n")== 0){
          exit(-1);
        }
        else if(strcmp(prompt_choice, "y") == 0){
          seteuid(real_id);
          int source_path = open(source, O_RDONLY);
          seteuid(owner_id);
          int dest_path = open(destination, O_WRONLY);
          sendfile(dest_path,source_path, NULL, sizeof(source));
          close(source_path);
          close(dest_path);
        }
      }
    }
    else{
      seteuid(real_id);
      int source_path = open(source, O_RDONLY);
      seteuid(owner_id);
      int dest_path = open(destination, O_CREAT);
      sendfile(dest_path,source_path, NULL, sizeof(source));
      close(source_path);
      close(dest_path);
    }

  }
}
