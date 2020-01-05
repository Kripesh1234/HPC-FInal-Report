#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Data07 CrackAZ99-With-Data07.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Data07 > CrackAZ99-with-Data07.c -lcrypt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$Iacidq803bZ2itSyHdJEMh.rsvvVlWI7FQ2Frb6lmNdnSwpSRbABWQJ1nlDpWF.0Vbe8F8osCyeNXuZFXzVj10",
  "$6$KB$BT.iImwz5zUcP.el4Jkl17oZTbs49n.ckZNFILIeQsX3/eo2OcX0qA5NPjwDjYqFFoxJTVbhF0e3axS/TqxvG/",
  "$6$KB$rgvmQ1v3UaTITh1Ng/GELNWZh0.uWTOjAevitxjVPHtbGNBOt2R.dwhvhydJtDqDfoeRUTZyPkq/xVCqq0y3X/",
  "$6$KB$aUgmUgRmTT88HkmpHEj6YNirT15cRVbVoHk0dZBJRXFl6BiBhv5MG71Epn2gIBFcBXoffBch3tqGMN77suHZZ0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int m, n, o, p;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(m='A'; m<='Z'; m++){
    for(n='A'; n<='Z'; n++){
      for(o='A'; o<='Z'; o++){
      for(p=0; p<=99; p++){
        sprintf(plain, "%c%c%c%02d", m, n, o, p); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}
  printf("%d solutions explored\n", count);
}
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(){
  int i;
struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }
clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
