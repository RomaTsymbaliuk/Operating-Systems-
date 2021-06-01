#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int const N=15;
pthread_mutex_t mutex;
pthread_mutex_t forks[N];
void *philosoph_func(void *param){
int *p = static_cast<int*>(param);
int left=*p;
int right=(*p + 1) % N;
// TryEnterCriticalSection - mutex - semaphores
if (!pthread_mutex_trylock(&forks[left])){
if (!pthread_mutex_trylock(&forks[right])){
pthread_mutex_lock(&mutex);
cout<<"\nPhilosopher "<<*p<<" start eating \n";
pthread_mutex_unlock(&mutex);
sleep(5);
pthread_mutex_lock(&mutex);
cout<<"\nPhilosopher "<<*p<<" ended eating \n";
pthread_mutex_unlock(&mutex);
pthread_mutex_unlock(&forks[right]);
}
pthread_mutex_unlock(&forks[left]);
//ExitCriticalSection
}
return NULL;
}
int main(){
while(true){
cout<<"\n--------------------------------------\n";
pthread_t tid[N];
pthread_attr_t attr[N];
pthread_mutex_init(&mutex, NULL);
int general=0;
for (int i=0; i<N; i++){
pthread_mutex_init(&forks[i], NULL);
}
int *p=new int [N];
for (int i=0; i<N; i++){
p[i]=i;
pthread_attr_init(&attr[i]);
pthread_create(&tid[i], &attr[i], philosoph_func, &p[i]);
}
for (int i=0; i<N; i++){
pthread_join(tid[i], NULL);
}
delete [] p;
pthread_mutex_destroy(&mutex);
for (int i=0; i<N; i++){
pthread_mutex_destroy(&forks[i]);
}
cout<<"\n----------------------------------\n";
}
return 0;
}

