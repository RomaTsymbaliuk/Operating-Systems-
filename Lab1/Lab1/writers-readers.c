#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
using namespace std;
int const N=20;
int const M=3;
int readers_counter=0;
sem_t door_mutex;
sem_t block_mutex;
sem_t print_mutex;
sem_t counter_mutex;
void *readers(void* value){
int *count = static_cast<int*>(value); 
sem_wait(&door_mutex);
if (readers_counter==0){
sem_wait(&block_mutex);
}
readers_counter++;
sem_post(&door_mutex);
sem_wait(&print_mutex);
cout<<"\n "<<readers_counter<<" readers here \n";
sem_post(&print_mutex);
int a=rand()%3;
sleep(a);
sem_wait(&door_mutex);
readers_counter--;
if (readers_counter==0){
sem_post(&block_mutex);
}
sem_post(&door_mutex);
return NULL;
}
void *writers(void *value){
int *count= static_cast<int*>(value);
sem_wait(&block_mutex);
sem_wait(&print_mutex);
cout<<"\n Writer "<<*count<<"  working \n";
sem_post(&print_mutex);
int a=rand()%5;
sleep(a);
sem_post(&block_mutex);
return NULL;
}                             
int main(){
while (true){
pthread_t tid_r[N], tid_w[N]; 
sem_init(&door_mutex, 0, 1);
sem_init(&block_mutex, 0, 1);
sem_init(&print_mutex, 0, 1);
sem_init(&counter_mutex, 0, 1);
int *p_readers=new int [N];
int *p_writers=new int [M];
for (int i=0; i<M; i++){
p_writers[i]=i;
pthread_create(&tid_w[i], NULL, writers, &p_writers[i]);
}
for (int i=0; i<N; i++){
p_readers[i]=i;
pthread_create(&tid_r[i], NULL, readers, &p_readers[i]);
}
for (int i=0; i<N; i++){
pthread_join(tid_r[i], NULL);
}
for (int i=0; i<M; i++){
pthread_join(tid_w[i], NULL);
}
sem_destroy(&door_mutex);
sem_destroy(&block_mutex);
sem_destroy(&print_mutex);
}
return 0;
}
