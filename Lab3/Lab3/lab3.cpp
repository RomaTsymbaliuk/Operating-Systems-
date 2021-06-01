#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <iterator>
#define MAX_PATH 1000
const char *path = "/media/roma/01D5F93CEC804180/TESTDIR";
//char *path = "/media/roma/01D5F93CEC804180/Skyrim - Legendary Edition";
//string path = "/media/roma/01D5F93CEC804180/TESTDIR";
//int interval = 2048;
int index_value = 5;
int file_counter = 0;
std::map <int, int> files_and_sizes;
int process_directory(const char *this_dir, int interval){
	DIR *dir = NULL;
	struct dirent entry;
	struct dirent *entryPtr = NULL;
	int retval = 0;
	unsigned count = 0;
	char Allpath[MAX_PATH + 1];
	dir = opendir(this_dir);
	if (dir == NULL)
		printf("\nError opening : %s\n", strerror(errno));
	entryPtr = readdir( dir );
	while (entryPtr !=NULL){
		struct stat entryInfo;
	if ((strncmp(entryPtr->d_name, ".", MAX_PATH) == 0) ||  (strncmp(entryPtr->d_name, "..", MAX_PATH) == 0) ){
		     entryPtr = readdir(dir);
		     continue;
		    }

	(void)strncpy(Allpath, this_dir, MAX_PATH);
	(void)strncat(Allpath, "/", MAX_PATH);
	(void)strncat(Allpath, entryPtr->d_name, MAX_PATH);
	entryPtr = readdir(dir);
	if (lstat(Allpath, &entryInfo) == 0){
		if (S_ISDIR(entryInfo.st_mode)){	
			process_directory(Allpath, interval);
		}
		else if (S_ISREG(entryInfo.st_mode)){
			long int size = entryInfo.st_size;
			int count = size / interval;
			//Not found key
			if (files_and_sizes.find(count) == files_and_sizes.end()){
				files_and_sizes.insert(std::pair<int, int>(count, 1));
			}
			//Found key
			else {
				files_and_sizes[count] = files_and_sizes[count] + 1;
			}
			file_counter++;
		}
	}
}
	return 0;
}
int find_max_in_map(){
	std::map<int, int>::iterator it = files_and_sizes.begin();
	int max = 0;
	while (it != files_and_sizes.end()){
		if (it->second > max){
			max = it->second;
		}
		it++;
	}
//	std::cout<<"The max value is : "<<max<<std::endl;
	return max;
}
void draw_bargraph(int interval){
	std::map<int, int>::iterator it = files_and_sizes.begin();
	int max = find_max_in_map();
	int scale_value = max / index_value;
	char graph_charachter = '*';
	int num = 0;
	while (it != files_and_sizes.end()){
		printf("\nInterval : (%7d %5c %7d) ", (it->first)*interval, '-', ((it->first)+1)*interval);
		int number_graph_charachter = (it->second)/scale_value + 1;
		for (int j = 0; j<number_graph_charachter; j++)
			printf("%c", graph_charachter);
		it++;
	}
}
int main(void){
	printf("Enter interval: \n");
	int interval;
	scanf("%d", &interval);
	process_directory(path, interval);
	draw_bargraph(interval);
	printf("\nFound %d files", file_counter); 
	return 0;
}
