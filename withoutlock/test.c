#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock1, lock2;

/**
 * console.log we add here flush for std out
 * @param str - some string for output
 */
void consoleLog(const char * str) {
  printf("%s", str);
  fflush(stdout);
}

void *thread1(void *arg) {
  consoleLog("T1: Started\n");

  // pthread_mutex_lock(&lock1);
  // consoleLog("T1: Job started in resource1..\n");

  usleep(2000);

  consoleLog("T1: Trying to get resource2\n");

  pthread_mutex_lock(&lock2);
  consoleLog("T1: Aquired resource2\n");

  pthread_mutex_unlock(&lock2);
  consoleLog("T1: Job finished in resource2..\n");

  // pthread_mutex_unlock(&lock1);
  // consoleLog("T1: Job finished in resource1..\n");

  consoleLog("T1: Finished\n");
  pthread_exit(NULL);
}

void *thread2(void *arg) {
  consoleLog("T2: Started\n");

  // pthread_mutex_lock(&lock2);
  // consoleLog("T2: Job started in resource2..\n");

  usleep(2000);

  consoleLog("T2: Trying to get resource1\n");

  pthread_mutex_lock(&lock1);
  consoleLog("T2: Aquired resource1\n");

  pthread_mutex_unlock(&lock1);
  consoleLog("T2: Job finished in resource1..\n");

  // pthread_mutex_unlock(&lock2);
  // consoleLog("T2: Job finished in resource2..\n");

  consoleLog("T2: Finished\n");
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  consoleLog("Main: Started\n");
  pthread_mutex_init(&lock1, NULL);
  pthread_mutex_init(&lock2, NULL);

  pthread_t t1, t2;

  pthread_create(&t1, NULL, thread1, NULL);
  pthread_create(&t2, NULL, thread2, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  consoleLog("Main: Finished\n");
  return 0;
}
