#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
using namespace std;
int const N=5;
int const M=20;
mutex print_mutex;
mutex enter;
mutex sleep_b;
mutex start_cutting[M];
mutex stop_cutting[M];
mutex queue_mutex_barber, queue_mutex_client; //C++ 2011
int clients_counter=0;
int flag=0;
int flag_order=0;
struct clients_queue* head=NULL;
struct clients_queue{
struct clients_queue* next;
int order;
};
struct clients_queue* push(int cl_num){
if (!clients_counter)
{
head=new struct clients_queue;
head->order=cl_num;
head->next=NULL;
return head;
}
struct clients_queue* client=new struct clients_queue;
client->order=cl_num;
struct clients_queue* temp=head;
while (temp->next!=NULL){
temp=temp->next;
}
temp->next=client;
client->next=NULL;
return head;
}
struct clients_queue* remove(){
struct clients_queue* temp=NULL;
temp=head;
head=head->next;
temp->next=NULL;
delete temp;
return head;
}
void barber_func(int brb_param){
while (true){
if (clients_counter==0){
sleep_b.lock();
continue;
}
start_cutting[head->order].unlock();
print_mutex.lock();
cout<<"\nCutting client "<<head->order<<"\n";
int r=rand()%7;
sleep(r);
print_mutex.unlock();
stop_cutting[head->order].unlock();
enter.lock();
clients_counter--;
head=remove();
enter.unlock();
}
}
void client_func(int cl_num){
if (clients_counter<=N){
enter.lock();
print_mutex.lock();
cout<<"\n Added client "<<cl_num<<" to the queue \n";
int r=rand()%2;
sleep(r);
print_mutex.unlock();
head=push(cl_num);

if (clients_counter==0){
sleep_b.unlock();
}
clients_counter++;
enter.unlock();
start_cutting[head->order].lock();
start_cutting[head->order].lock();
stop_cutting[head->order].lock();
stop_cutting[head->order].lock();
}

else {
print_mutex.lock();
cout<<"\nQueue limit("<<clients_counter<<") thread "<<cl_num<<" not added\n";
print_mutex.unlock();
}
}
int main(){
thread barber(barber_func, 10);
thread clients[M];
for (int i=0; i<M; i++){
clients[i]=thread(client_func, i);
int r=rand()%5;
sleep(r);
}
barber.join();
return 0;
}
