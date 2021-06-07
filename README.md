Here some system information is get with C/C++ and Linux.
In first lab some efficient concurrent algorithms 
are implemented, such as the eating philosophers problem,
the sleeping barber problem, the productors and clients
problem and the readers and writers problem
```
:~$g++ -o barber barber.c -lpthread 
:~$./barber
```
```
:~$g++ -o writers-readers writers-readers.c -lpthread 
:~$./writers-readers
```
etc

In second lab a simple task manager with /proc directory is
implemented, also CPU usage by processes is computed.
```
:~$gcc -o lab2 lab2.c 
:~$./lab2
```

In third lab hard disk information is get and displayed using
a bargraph. Enter an interval (in bytes) to distinguish files for size 
```
:~$g++ -o lab3 lab3.cpp 
:~$./lab3
```
