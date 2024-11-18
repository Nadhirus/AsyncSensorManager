#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include "acquisitionManager.h"
#include "msg.h"
#include "iSensor.h"
#include "multitaskingAccumulator.h"
#include "iAcquisitionManager.h"
#include "debug.h"

#define BUFFER_SIZE 4

// Storage buffer
MSG_BLOCK buffer[BUFFER_SIZE];

unsigned int writeIndex = 0; // Index where the producer will write
unsigned int readIndex = 0;	 // Index where the consumer will read

// producer count storage
volatile unsigned int produceCount = 0;

pthread_t producers[4];

static void *produce(void *params);

/**
 * Semaphores and Mutex
 */
// mutexes
pthread_mutex_t bufferLock;

// Semaphores
sem_t buffEmpty; // semaphore to indicate the number of empty spots in the buffer
sem_t buffFull;	 // semaphore to indicate the number of full spots in the buffer

/*
 * Creates the synchronization elements.
 * @return ERROR_SUCCESS if the init is ok, ERROR_INIT otherwise
 */
static unsigned int createSynchronizationObjects(void);

/*
 * Increments the produce count.
 */
static void incrementProducedCount(void);

static unsigned int createSynchronizationObjects(void)
{

	if (sem_init(&buffEmpty, 0, BUFFER_SIZE) == -1 || sem_init(&buffFull, 0, 0) == -1)
	{
		perror("Semaphore creation failed");
		return ERROR_INIT;
	}
	pthread_mutex_init(&bufferLock, NULL);
	printf("[acquisitionManager]Semaphore created\n");
	return ERROR_SUCCESS;
}

static void incrementProducedCount(void)
{
	produceCount++;
}

unsigned int getProducedCount(void)
{
	unsigned int p = 0;
	p = produceCount;
	return p;
}

// accessors to limit semaphore and mutex usage outside of this C module.
void lockBuffer(void)
{
	pthread_mutex_lock(&bufferLock);
}

void unlockBuffer(void)
{
	pthread_mutex_unlock(&bufferLock);
}

void waitForEmptyBuffer(void)
{
	sem_wait(&buffEmpty);
}

void signalEmptyBuffer(void)
{
	sem_post(&buffEmpty);
}

void waitForFullBuffer(void)
{
	sem_wait(&buffFull);
}

void signalFullBuffer(void)
{
	sem_post(&buffFull);
}

MSG_BLOCK getMessage(void)
{
	MSG_BLOCK message;

	waitForFullBuffer();
	lockBuffer();

	message = buffer[readIndex];
	readIndex = (readIndex + 1) % BUFFER_SIZE;

	signalEmptyBuffer();
	unlockBuffer();
	return message;
}

unsigned int acquisitionManagerInit(void)
{
	unsigned int i;
	printf("[acquisitionManager]Synchronization initialization in progress...\n");
	fflush(stdout);
	if (createSynchronizationObjects() == ERROR_INIT)
		return ERROR_INIT;

	printf("[acquisitionManager]Synchronization initialization done.\n");

	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		if (pthread_create(&producers[i], NULL, &produce, NULL) != 0)
		{
			perror("Producer thread creation failed");
		}
		else
		{
			printf("Producer thread created\n");
		}
	}

	return ERROR_SUCCESS;
}

void acquisitionManagerJoin(void)
{
	unsigned int i;
	for (i = 0; i < PRODUCER_COUNT; i++)
	{
		// join the producer threads
		if (pthread_join(producers[i], NULL) != 0)
		{
			perror("Producer thread join failed");
		}
		else
		{
			printf("Producer thread joined\n");
		}
	}

	// destroy semaphores and mutex
	printf("Deleting the semaphores and mutex\n");
	sem_destroy(&buffEmpty);
	sem_destroy(&buffFull);
	pthread_mutex_destroy(&bufferLock);

	printf("[acquisitionManager]Semaphore cleaned\n");
}

void *produce(void *params)
{
	D(printf("[acquisitionManager]Producer created with id %d\n", gettid()));
	unsigned int i = 0;
	MSG_BLOCK message;

	while (i < PRODUCER_LOOP_LIMIT)
	{
		i++;
		sleep(PRODUCER_SLEEP_TIME + (rand() % 5));
		// get the message from iSensor after it has been checked
		do
		{
			// getInput(r_rand(), &message); // Get input for the message
			getInput(0, &message); // Get input for the message

		} while (messageCheck(&message) == 0); // Repeat until the checksum is correct

		// wait for an empty slot in the buffer
		waitForEmptyBuffer();
		// Lock access to the buffer
		lockBuffer();
		// write the message in the buffer
		buffer[writeIndex] = message;
		// increment the write index within wrap bounds
		writeIndex = (writeIndex + 1) % BUFFER_SIZE;
		// increment the produced count
		incrementProducedCount();
		// post semaphore to account for filled slot
		signalFullBuffer();
		// unlock the buffer
		unlockBuffer();
	}

	printf("[acquisitionManager] %d termination\n", gettid());
	pthread_exit(NULL); // pour tuer le thread correctement
}

/*
Potential improvements to ask the teacher about
- incrementing the producer count could cause a race condition, what is the solution
a mutex or an elegant c11 atomic?
- the number of sensor? I used r_rand for a thred safe execution but it could be a misunderstanding of the software architecutre and
I could somehow get it from the iSensor Interface
-


*/