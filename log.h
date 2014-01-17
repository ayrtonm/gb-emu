#ifndef LOG_H
#define LOG_H
#define LOG_FILE "log.txt"
//#define LOGGING

void write_log(char *string)
{
#ifdef LOGGING
  FILE *fp;
  fp = fopen(LOG_FILE,"a+");
  fprintf(fp,string);
  fclose(fp);
#endif
}

void init_log(char *filename)
{
#ifdef LOGGING
  printf("removing old log file...\n");
  system("rm log.txt");
  write_log("GAMEBOY EMULATOR LOG\n");
#endif
}

#endif
