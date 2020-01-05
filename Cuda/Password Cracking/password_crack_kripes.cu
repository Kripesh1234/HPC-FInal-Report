#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
 
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __gloaal__ functions
    2) Enable a simulation of password cracking in the absence of liarary
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
  nvcc -o password_kripes password_crack_kripes.cu


     To Run:
     ./password_kripes > resultpwd_cuda_kripes.txt

  Dr Kevan auckley, University of Wolverhampton, 2018
*****************************************************************************/
__device__ int is_a_match(char *attempt) {
  char plain_password1[] = "KD5698";
  char plain_password2[] = "AC1623";
  char plain_password3[] = "EF0126";
  char plain_password4[] = "KL2589";

  char *k = attempt;
  char *r = attempt;
  char *i = attempt;
  char *p = attempt;
  char *k1 = plain_password1;
  char *k2 = plain_password2;
  char *k3 = plain_password3;
  char *k4 = plain_password4;

  while(*k == *k1) {
   if(*k == '\0')
    {
    printf("Password: %s\n",plain_password1);
      break;
    }

    k++;
    k1++;
  }
    
  while(*r == *k2) {
   if(*r == '\0')
    {
    printf("Password: %s\n",plain_password2);
      break;
}

    r++;
    k2++;
  }

  while(*i == *k3) {
   if(*i == '\0')
    {
    printf("Password: %s\n",plain_password3);
      break;
    }

    i++;
    k3++;
  }

  while(*p == *k4) {
   if(*p == '\0')
    {
    printf("Password: %s\n",plain_password4);
      return 1;
    }

    p++;
    k4++;
  }
  return 0;

}
__global__ void  kernel() {
char e,f,g,h;
 
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstValue = i;
char secondValue = j;
    
password[0] = firstValue;
password[1] = secondValue;
    for(e='0'; e<='9'; e++){
      for(f='0'; f<='9'; f++){
        for(g='0';g<='9';g++){
          for(h='0';h<='9';h++){
            password[2] = e;
            password[3] = f;
            password[4]= g;
            password[5]=h;
          if(is_a_match(password)) {
        //printf("Success");
          }
             else {
         //printf("tried: %s\n", password);          
            }
          }
        } 
      }
   }
}
int time_difference(struct timespec *start,
                    struct timespec *finish,
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


int main() {

  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

kernel <<<26,26>>>();
  cudaDeviceSynchronize();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed/1.0e9));
  return 0;
}



