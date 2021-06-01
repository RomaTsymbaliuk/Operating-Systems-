#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#define STATE_BUF 50
//30
#define PROCESS_NAME 500
char *choice(char state){
  char *string = (char*)malloc(STATE_BUF*sizeof(char));
  switch(state){
    case 'R': string = "RUNNING"; break;
    case 'S': string = "SLEEPING"; break;
    case 'D': string = "WAITING DISK SLEEP"; break;
    case 'Z': string = "ZOMBIE"; break;
    case 'T': string = "STOPPED"; break;
    case 't': string = "TRACING STOP"; break;
    case 'X': string = "DEAD"; break;
    case 'x': string = "DEAD"; break;
    case 'K': string = "WAKEKILL"; break;
    case 'W': string = "WAKING"; break;
    case 'P': string = "PARKED"; break;
    case 'I': string = "IDLE"; break;
    case 'C': string = "NOTHING EXECUTING"; break;
    default : *string = state; break;
  }
  return string;
}
long int Total_time_calculation(){
  int length = 10;
  long int *array = (long int*)malloc(length*sizeof(long int));
  char *time_file = "/proc/stat";
  FILE *cpu_time = fopen(time_file, "r");
  char ch;
  int counter = 0;
  while ((ch = fgetc(cpu_time))!= '\n' )
  counter++ ;
  char *row = (char*)malloc(counter*sizeof(char));
  fseek(cpu_time, SEEK_SET, 0);
  int i=0;
  int point_counter=0;
  while ((ch = fgetc(cpu_time))!= '\n' ){
  	row[i++]=ch;
  } 
  char *token = strtok(row, " ");
  long int sum=0;
  while (token != NULL){
  	if (token[0]<'A'){
  		int number = atoi(token);
  		sum += number;
  	}
  	token = strtok(NULL, " ");
  }
  fclose(cpu_time);
  return sum;
}
int Get_Parameters(unsigned long int *stime, unsigned long int *utime, char *filename){
	FILE *stat = fopen(filename, "r");
	fscanf(stat, "%*d %*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %lu", utime, stime);
	fclose(stat);
	return 1;
}
int Count_CPU_time(long int time_after, long int time_before, unsigned long int stime_before, unsigned long int stime_after, unsigned long int utime_before, unsigned long int utime_after, double *utime_percentage, double *stime_percentage){
	double stime_diff = stime_after - stime_before;
	double utime_diff = utime_after - utime_before;
	double total_time_diff = time_after - time_before;
	*utime_percentage = 100*(utime_diff / total_time_diff);
	*stime_percentage = 100*(stime_diff / total_time_diff);
	return 1;
}
int Search_Availlable(char* token, char **Good_Results, const int LENGTH){
	char tk_char ; char gr_char;
	for (int j = 0; j<LENGTH; j++){
		int flag = 0;
		for (int i = 0; i<strlen(Good_Results[j]); i++){
			gr_char = *(Good_Results[j]+i);
			if (i < strlen(token))
				tk_char = token[i];
			else { break; }
			if ( gr_char == tk_char )
				flag = 1;
			else { flag = 0; break; }
		}
		if (flag)
			return 1;
	}
	return 0;
}
int Get_Memory_Info(){
	const int LENGTH = 5;
	char **Good_Results = (char**)malloc(LENGTH*sizeof(char*));
	char *memtotal = "MemTotal";
	char *memfree = "MemFree";
	char *memavailable = "MemAvailable";
	char *swapTotal = "SwapTotal";
	char *swapFree = "SwapFree";
	Good_Results[0] = memtotal; Good_Results[1]=memfree; 
	Good_Results[2]=memavailable; Good_Results[3]=swapTotal;
	Good_Results[4]=swapFree;
	char *mem_path = "/proc/meminfo";
  	FILE *mem_info = fopen(mem_path, "r");
  	char ch;
  	int counter = 0;
  	while ((ch = fgetc(mem_info))!= '\n' ){
  	counter++;
  	}
  	char *row = (char*)malloc(sizeof(char)*counter);
  	int i = 0;
  	char general;
  	fseek(mem_info, SEEK_SET, 0);
  	while ((general = fgetc(mem_info))!=EOF){
  		if (general!= '\n'){
  			row[i++] = general;
  			//printf("\nrow: %s\n", row);
  		}
  		else {
  			char *token = strtok(row, " ");
  			while (token != NULL){
  				int result = Search_Availlable(token, Good_Results, LENGTH);
  				if (result) {
  					printf("\n%s", token);
  					token = strtok(NULL, " ");
  					printf("\t%s", token);
  					token = strtok(NULL, " ");
  					printf("\t%s", token);
  				}
  				token = strtok(NULL, " ");
 		 	}
 		 	i = 0;
		 	}
  	}
  	fclose(mem_info);
  	return 1;
}
int Get_Virtual_Memory_Info(){
	const int LENGTH = 2;
	char *kernel_page_size="KernelPageSize:";
	char *mmu_page_size="MMUPageSize:";
	char **Good_results = (char**)malloc(LENGTH*sizeof(char*));
	Good_results[0]=kernel_page_size;
	Good_results[1]=mmu_page_size;
	char *virtual_path = "/proc/1/smaps";
	FILE *virtual_mem_info = fopen( virtual_path , "r");
	if (virtual_mem_info == NULL){
		printf("Error detected");
		return -1;
	}
	char ch;
	int counter = 0;
  	while ((ch = fgetc(virtual_mem_info))!= '\n' ){
  	counter++;
  	}
  	char *row = (char*)malloc(sizeof(char)*counter);
  	int i = 0;
  	int only_two_flag = 0;
  	fseek(virtual_mem_info, SEEK_SET, 0);
  	while ((ch = fgetc(virtual_mem_info))!=EOF && (only_two_flag!=2)){
  		if (ch!= '\n'){
  			row[i++] = ch;
  			//printf("\nrow: %s\n", row);
  		}
  		else {
  			char *token = strtok(row, " ");
  			while (token != NULL){
  				int result = Search_Availlable(token, Good_results, LENGTH);
  				if (result) {
  					printf("\n%s", token);
  					token = strtok(NULL, " ");
  					printf("\t%s", token);
  					token = strtok(NULL, " ");
  					printf("\t%s", token);
  					only_two_flag++;
  				}
  				token = strtok(NULL, " ");
 		 	}
 		 	i = 0;
		 	}
  	}
  	
	
}
int main(int argc, char **argv){
  int pid;
  char *read;
  struct dirent *de;
  DIR *dr = opendir("/proc");
  if (!dr)
  {
    printf("Could not open directory");
    return -1;
  }
  
  char *directory;
  Get_Memory_Info();
  Get_Virtual_Memory_Info();
  printf("\n---------------------------------------------------------------------");
  printf("\n|PID\t \tFileName\t\tState\t\tParent PID\t\tUser Time percentage\t\tSystem Time Percentage|\n");
  while ((de = readdir(dr)) != NULL){
  	if (de->d_name[0] < 'A' && (de->d_name[0]!='.')){
  		directory = de->d_name;
  		char filename[PROCESS_NAME];
  		sprintf(filename, "/proc/%s/stat", directory);
  		FILE *stat = fopen(filename, "r");
  		int id;
  		char proc_name[PROCESS_NAME];
  		char state;
  		int ppid;
  		fseek(stat, SEEK_SET, 0);
		fscanf(stat, "%d %s %c %d", &id, proc_name, &state, &ppid);
		char *name = choice(state);
		long int total_time_before;
		unsigned long int utime_before;
		unsigned long int stime_before;
		total_time_before=Total_time_calculation();
		Get_Parameters(&stime_before, &utime_before, filename);
		sleep(2);
		unsigned long int utime_after;
		unsigned long int stime_after;
		long int total_time_after;
		Get_Parameters(&stime_after, &utime_after, filename);
		total_time_after=Total_time_calculation();
		double utime_percentage;
		double stime_percentage;
		Count_CPU_time(total_time_after, total_time_before, stime_before, stime_after, utime_before, utime_after ,&utime_percentage, &stime_percentage);
		printf("|%-10d %-30s %-40s %-20d %-30f %-30f|\n", id, proc_name, name, ppid, utime_percentage, stime_percentage);
		
  	}
  }
  return 1;
}
