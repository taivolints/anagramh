#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

int main(int numberofarguments, char *arguments[]){
  struct timespec startingtime;
  clock_gettime(CLOCK_MONOTONIC, &startingtime);

  int numberofresults = 0;
  if(numberofarguments < 3) goto finish;

  int givenwordlength = 0;
  int currentargumentlength;
  int lengthcorrection = 0;
  unsigned char givenwordfrequencytable[256] = {0};
  char previouschar = 0;
  char multibytestarted = 0;
  for(int argumentindex = 2; argumentindex < numberofarguments; argumentindex++){
    if(argumentindex > 2){
      givenwordfrequencytable[32] += 1;
      givenwordlength++;}
    for(currentargumentlength = 0; arguments[argumentindex][currentargumentlength] != 0; currentargumentlength++){
      char currentchar = arguments[argumentindex][currentargumentlength];
      if(multibytestarted){
        lengthcorrection++; 
        switch(previouschar){
          case -61:
            switch(currentchar){
              case -68: currentchar = -4; break;
              case -100: currentchar = -4; break;
              case -75: currentchar = -11; break;
              case -107: currentchar = -11; break;
              case -92: currentchar = -28; break;
              case -124: currentchar = -28; break;
              case -74: currentchar = -10; break;
              case -106: currentchar = -10; break;
              case -87: currentchar = -23; break;
              default: goto finish;}
              break;
          case -59:
            switch(currentchar){
              case -66: currentchar = -2; break;
              case -67: currentchar = -2; break;
              case -95: currentchar = -16; break;
              case -96: currentchar = -16; break;
              default: goto finish;}
            break;
          default: goto finish;}
        givenwordfrequencytable[(unsigned char)currentchar] += 1;
        multibytestarted = 0;}
      else
        if(currentchar > 0){
          if(currentchar >= 65 && currentchar <= 90) currentchar += 32;
          givenwordfrequencytable[(unsigned char)currentchar] += 1;}
        else multibytestarted = 1;
      previouschar = currentchar;}
    givenwordlength += currentargumentlength;}
  givenwordlength -= lengthcorrection;
  if(givenwordlength < 1 || givenwordlength > 31) goto finish;

  int filedescriptor = open(arguments[1], O_RDONLY);
  if(filedescriptor < 0){
    printf("\nERROR: Could not open dictionary file.\n\n");
    goto finish;}
  struct stat filestat;
  fstat(filedescriptor, &filestat);
  char *dictionary = mmap(NULL, filestat.st_size, PROT_READ, MAP_SHARED, filedescriptor, 0);

  char *resultwordstarts[500];
  int resultwordlengths[500];
  char *currentwordstart = dictionary;
  int currentwordlength = 0;
  for(int i = 0; i < filestat.st_size; i++){
    if(dictionary[i] == 13) continue;
    if(dictionary[i] == 10){
      if(currentwordlength == givenwordlength){
        unsigned char currentwordfrequencytable[256] = {0};
        for(int j = 0; j < givenwordlength; j++){
          if((unsigned char)*(currentwordstart + j) >= 65 && (unsigned char)*(currentwordstart + j) <= 90)
            currentwordfrequencytable[(unsigned char)*(currentwordstart + j) + 32] += 1;
          else
            currentwordfrequencytable[(unsigned char)*(currentwordstart + j)] += 1;}
        if(!memcmp(currentwordfrequencytable, givenwordfrequencytable, 256)){
          resultwordstarts[numberofresults] = currentwordstart;
          resultwordlengths[numberofresults] = currentwordlength;
          numberofresults++;}}
      currentwordstart += currentwordlength + 2;
      currentwordlength = 0;
      continue;}
    currentwordlength++;}

  finish:;
  struct timespec finishingtime;
  clock_gettime(CLOCK_MONOTONIC, &finishingtime);
  printf("%f", 1e6 * (finishingtime.tv_sec - startingtime.tv_sec) + 1e-3 * (finishingtime.tv_nsec - startingtime.tv_nsec));
  for(int i = 0; i < numberofresults; i++){
    printf(",");
    for(int j = 0; j < resultwordlengths[i]; j++)
      switch(*(resultwordstarts[i] + j)){
        case -11: printf("õ"); break;
        case -43: printf("Õ"); break;
        case -28: printf("ä"); break;
        case -60: printf("Ä"); break;
        case -10: printf("ö"); break;
        case -42: printf("Ö"); break;
        case -4: printf("ü"); break;
        case -36: printf("Ü"); break;
        case -2: printf("ž"); break;
        case -34: printf("Ž"); break;
        case -16: printf("š"); break;
        case -48: printf("Š"); break;
        case -23: printf("é"); break;
        default: printf("%c", *(resultwordstarts[i] + j));}}
  printf("\n");
  return 0;}
