#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--) {
    ;
  }
  p++;
  return p;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


void find(char *path, char* filename) {

  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("find: path too long\n");
    return;
  }

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';


  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    switch (st.type)
    {
    case T_FILE:
      if (strcmp(fmtname(buf), filename) == 0) {
        printf("%s\n", buf);
        break;
      }
      break;
    
    case T_DIR:
      if (strcmp(fmtname(buf), ".") != 0 && strcmp(fmtname(buf), "..") != 0) {
        find(buf, filename);
      }
      break;
    
    default:
      break;
    }
  }
  close(fd);
}


int main(int argc, char *argv[]) {

  if (argc < 2 || argc > 3) {
    printf("wrong usage\n");
    exit(1);
  }

  else if (argc == 2) {
    find(".", argv[1]);
    exit(0);
  }

  else {
    find(argv[1], argv[2]);
    exit(0);
  }
}
