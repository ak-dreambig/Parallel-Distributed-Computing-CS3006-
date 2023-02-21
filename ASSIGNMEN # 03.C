#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem1, sem2, sem3,sem4;

void *T1(void *arg) {
  sem_wait(&sem1);
  printf("T1 executed by T2,T3 and T4\n");
  sem_post(&sem2);
  sem_post(&sem3);
  sem_post(&sem4);
  return NULL;
}

void *T2(void *arg) {
  sem_wait(&sem2);
  printf("T2 executed by T1,T3 and T4\n");
  sem_post(&sem1);
  sem_post(&sem3);
  sem_post(&sem4);
  return NULL;
}

void *T3(void *arg) {
  sem_wait(&sem3);
  printf("T3 executed by T1 ,T2 and T4\n");
  sem_post(&sem1);
  sem_post(&sem2);
  sem_post(&sem4);
  return NULL;
}

void *T4(void *arg) {
  sem_wait(&sem4);
  printf("T4 executed by T1 , T2 and T3\n");
  sem_post(&sem1);
  sem_post(&sem2);
  sem_post(&sem3);

  return NULL;
}

int main() {
  pthread_t t1, t2, t3,t4;

  sem_init(&sem1, 0, 0);
  sem_init(&sem2, 0, 0);
  sem_init(&sem3, 0, 1);
  sem_init(&sem4, 0, 1);

  pthread_create(&t1, NULL, T1, NULL);
  pthread_create(&t2, NULL, T2, NULL);
  pthread_create(&t3, NULL, T3, NULL);
  pthread_create(&t4, NULL, T4, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  sem_destroy(&sem1);
  sem_destroy(&sem2);
  sem_destroy(&sem3);
  sem_destroy(&sem4);

  return 0;
}