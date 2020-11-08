#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

#define DATASIZE 100 * 1024 * 1024
#define NT	4
long value;
unsigned char *data;
pthread_mutex_t lock;
int tid[NT];

/**
 * Compile with gcc -o h4 h4.c -lm
 */

int compute_value(int start, int chunk) {
  int v = 0;
  for(int i = start; i < chunk; i++)
    v += (long)cos(sqrt((double)data[i]));
  return v;
}

void *run(int *tid)
{
  int v = 0;
  int chunk = DATASIZE/NT;

  if (*tid == 0)
    v = compute_value(0, chunk);
  else if (*tid == 1)
    v = compute_value(chunk, chunk*2);
  else if (*tid == 2)
    v = compute_value(chunk*2, chunk*3);
  else if (*tid == 3) 
    v = compute_value(chunk*3, DATASIZE);
 
  pthread_mutex_lock(&lock);
  value += v;
  pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[])
{
  /* Leave the following unmodified: */
  if (argc > 1) srand(atoi(argv[1]));
  value = 0;
  data = malloc(sizeof(unsigned char) * DATASIZE);

  printf("Generating data...\n");
  for(int i=0; i < DATASIZE; i++) {
    data[i] = (unsigned char)(rand() % 100);
  }

  /* Add thread support from here: */

  pthread_t th[NT];
  pthread_mutex_init(&lock, NULL);

  printf("Computing...\n");
  for (int i = 0; i < NT; i++) {
    tid[i] = i;
    pthread_create(&th[i], NULL, (void *)run, &tid[i]);
  }
  for (int i = 0; i < NT; i++)
    pthread_join(th[i], NULL);

  printf("%ld\n", value);
  return 0;
}
