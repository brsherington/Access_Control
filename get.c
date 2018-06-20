//Ben Sherington
//CS483-02: Project1
#include "access.h"

static uid_t real_id; //ruid
static uid_t owner_id; //euid


void get_file(char* source_file, char* dest_file){
  //checks if the the source file can be accessed and has read access
  if (check_access(source_file,1,0) == 0) {
    //now to check for the .access file
    char* access_file = ".access";
    char* access = (char*)malloc(strlen(source_file)+strlen(access_file)+1);
    strcpy(access, source_file);
    strcat(access, access_file);
    if(check_owner(access,owner_id) == 0){
      // now we are parsing the access file
      access_parser(access,real_id,owner_id);
      create_file_get(source_file, dest_file);
    }
  }
  free(access);
}

int main(int argc, char* argv[]){
  if(argc < 3){
    fprintf(stderr, "%s: Not enough arguments provided\n",argv[0]);
    exit(1);
  }
  if(argv[1] == NULL){
    fprintf(stderr, "%s: Source file not provided\n",argv[0]);
    exit(1);
  }
  if(argv[2] == NULL){
    fprintf(stderr, "%s: Destination file not provided\n",argv[0]);
    exit(1);
  }

  char* source = argv[1];
  char* dest = argv[2];
  real_id = getuid();
  owner_id = geteuid();
  seteuid(real_id);
  get_file(source, dest);

  return 0;
}
