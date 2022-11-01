#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {

	// Initalizae semaphores
	int sh = shm_open("table", O_RDWR, 0666);
	int* table = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, sh, 0);
	sem_t* fill = sem_open("fill", O_CREAT, 0666, 0);
	sem_t* available = sem_open("available", O_CREAT, 0666, 2);
	sem_t* mutex = sem_open("mutex", O_CREAT, 0666, 1);

	// loop for 10 items
	for (int i = 1; i < 10; i++) {

		// waits between 1 and 5 seconds
		sem_wait(available);
		int randomNumber = rand() % 5 + 1;

		sleep(randomNumber);

		*table = randomNumber;
		sem_wait(mutex);

		++(*table);

		sem_post(mutex);

		sem_post(fill);
	}

	// close table
	munmap(table, sizeof(int));
	close(*table);
	shm_unlink("table");

	return 0;
}