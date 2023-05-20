/* Einfuegen benoetigter Headerdateien in Quelltext */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

/* Deklaration von Funktionskoepfen */
/* Deklaration von globalen Variablen */

/*====================================================================
 * 
 * mmap example class for reading process
 * 
 * ==================================================================*/

int main(int argc, char *argv[]) 
{
	char *quelldatei = argv[1];
	int fd = open(quelldatei, O_RDWR , S_IRUSR | S_IWUSR);
	struct stat sb;
	
	
	if(stat(quelldatei, &sb) == -1) 
	{
		perror("Could not get file size.\n");
		exit(EXIT_FAILURE);
	}
	
	//get file size
	printf("File Size is %ld\n", sb.st_size);
	
	char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ | 
		PROT_WRITE, MAP_SHARED, fd, 0);
	
	printf("Printing file, as an array of characters...\n");
	for(int i = 0; i < sb.st_size; i++) 
	{
		//capitalize letters
		if((i % 2) == 0) {
			file_in_memory[i] = toupper(file_in_memory[i]);
		}
		printf("%c", file_in_memory[i]);
	}
	
	printf("\n");
	
	munmap(file_in_memory, sb.st_size);
	
	close(fd);
	
	return EXIT_SUCCESS;

}
