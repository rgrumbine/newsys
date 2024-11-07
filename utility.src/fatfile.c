#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  long int gb = 1024*1024*1024;
  int n;
  char *x;
  FILE *fout;
  size_t len;

  fout = fopen("fat","w");
  len = gb;
  if (argc > 1) {
    n = atoi(argv[1]);
  }
  else {
    n = 4;
  }

  printf("%ld %ld  n = %d\n",sizeof(size_t), len, n);

  x = malloc(n*gb*sizeof(char));
  fwrite(x, n*gb, 1, fout);

  fclose(fout);

  return(0);
}
