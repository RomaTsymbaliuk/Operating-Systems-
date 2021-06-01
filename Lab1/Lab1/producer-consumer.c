#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
using namespace std;
int const N=7;
int const M=9; //C++ 2011
int const buffer_length=4;
struct queue* head=NULL;
int queue_counter=0;
pthread_mutex_t prod_mutex, cons_mutex, count_mutex, one_mutex, print_mutex;
sem_t free_places, busy_places;
struct queue{
  struct queue* next;
  int test;
};
int push(int num){
  if (!head){
    head=new struct queue;
    head->test=num;
    head->next=NULL;
    return 0;
  }
  struct queue* p=new struct queue;
  struct queue* t=head;
  while (t->next!=NULL){
    t=t->next;
  }
  t->next=p;
  p->next=NULL;
  p->test=num;
  return 1;
}
int remove(){
  struct queue* t=head;
  head=head->next;
  t->next=NULL;
  delete t;
  return 1;
}
void *productive(void* numb){
while (true){
  pthread_mutex_lock(&prod_mutex);
  int *num = static_cast<int*>(numb);
  sem_wait(&free_places);
  pthread_mutex_lock(&one_mutex);
  if (queue_counter==1)
  {
    push(*num);
    pthread_mutex_lock(&count_mutex);
    queue_counter++;
    cout<<"\nProductive added\n";
    cout<<"\nElemets here "<<queue_counter<<"\n";
    int r=rand()%2;
    sleep(r);
    pthread_mutex_unlock(&count_mutex);
    pthread_mutex_unlock(&one_mutex);
   }
   else
   {
     pthread_mutex_unlock(&one_mutex);
     push(*num);
     pthread_mutex_lock(&count_mutex);
     queue_counter++;
     cout<<"\nProductive added\n";
     cout<<"\nElemets here "<<queue_counter<<"\n";
     int r=rand()%2;
     sleep(r);
     pthread_mutex_unlock(&count_mutex);
   }
   sem_post(&busy_places); //+1 
   pthread_mutex_unlock(&prod_mutex);
 }
}
void *consumer(void *num){
while (true){
      pthread_mutex_lock(&cons_mutex);
      //if (queue_counter>0){
      sem_wait(&busy_places); //
      pthread_mutex_lock(&one_mutex);
      if (queue_counter==1)
      {
        remove();
        pthread_mutex_lock(&count_mutex);
        queue_counter--;
        cout<<"\nConsumer removed\n";
        cout<<"\nElemets here "<<queue_counter<<"\n";
        int r=rand()%2;
        sleep(r);
        pthread_mutex_unlock(&count_mutex);
        pthread_mutex_unlock(&one_mutex);
      }
      else {
        pthread_mutex_unlock(&one_mutex);
        remove();
        pthread_mutex_lock(&count_mutex);
        queue_counter--;
        cout<<"\nConsumer removed\n";
        cout<<"\nElemets here "<<queue_counter<<"\n";
        int r=rand()%2; 
        sleep(r);
        pthread_mutex_unlock(&count_mutex);
       }
      sem_post(&free_places);    
  //}
  pthread_mutex_unlock(&cons_mutex);   
  }
}
int main(){
pthread_t tid_consumer[N], tid_productive[M]; 
sem_init(&free_places, 0, buffer_length);
sem_init(&busy_places, 0, 0);
pthread_mutex_init(&prod_mutex, NULL);
pthread_mutex_init(&cons_mutex, NULL);
pthread_mutex_init(&count_mutex, NULL);
pthread_mutex_init(&one_mutex, NULL);
pthread_mutex_init(&print_mutex, NULL);
int *p_consumer= new int [N];
for (int i=0; i<N; i++){
p_consumer[i]=i;
pthread_create(&tid_consumer[i], NULL, consumer, &p_consumer[i]);
}
int *p_productive = new int [M];
for (int i=0; i<M; i++){
  p_productive[i]=i;
  pthread_create(&tid_productive[i], NULL, productive, &p_productive[i]);
}
for (int i=0; i<M; i++){
  pthread_join(tid_productive[i], NULL);
}
for (int i=0; i<N; i++){
  pthread_join(tid_consumer[i], NULL);
}
sem_destroy(&free_places);
sem_destroy(&busy_places);
pthread_mutex_destroy(&prod_mutex);
pthread_mutex_destroy(&cons_mutex);
pthread_mutex_destroy(&count_mutex);
pthread_mutex_destroy(&one_mutex);
pthread_mutex_destroy(&print_mutex);
return 0;
}
