#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include "messageAdder.h"
#include "msg.h"
#include "iMessageAdder.h"
#include "multitaskingAccumulator.h"
#include "iAcquisitionManager.h"
#include "debug.h"

// consumer thread
pthread_t consumer;
// Message computed
volatile MSG_BLOCK out;
// Consumer count storage
volatile unsigned int consumeCount = 0;

volatile MSG_BLOCK in;

pthread_mutex_t consumerCountLock;



void lockConsumerCount(void)
{
	pthread_mutex_lock(&consumerCountLock);
}

void unlockConsumerCount(void)
{
	pthread_mutex_unlock(&consumerCountLock);
}
/**
 * Increments the consume count.
 */
static void incrementConsumeCount(void);

/**
 * Consumer entry point.
 */
static void *sum(void *parameters);

MSG_BLOCK getCurrentSum()
{
	return out;
}

unsigned int getConsumedCount()
{
	unsigned int c = 0;
	lockConsumerCount();
	c = consumeCount;
	unlockConsumerCount();
	return c;
}

// increment consume count
static void incrementConsumeCount(void){
	consumeCount++;
}

void messageAdderInit(void)
{
	out.checksum = 0;
	for (size_t i = 0; i < DATA_SIZE; i++)
	{
		out.mData[i] = 0;
	}
	if (pthread_create(&consumer, NULL, sum, NULL) != 0)
	{
		perror("Consumer thread creation failed");
	}
	else
	{
		printf("Consumer thread created\n");
	}
}

void messageAdderJoin(void)
{
	printf("Joining the messageAdderJoin\n");
	if (pthread_join(consumer, NULL) != 0)
	{
		perror("Consumer thread join failed");
	}
	else
	{
		printf("Consumer thread joined\n");
	}
}

static void *sum(void *parameters)
{
	D(printf("[messageAdder]Thread created for sum with id %d\n", gettid()));
	unsigned int i = 0;
	
	while (i < ADDER_LOOP_LIMIT)
	{
		i++;
		sleep(ADDER_SLEEP_TIME);

		in = getMessage();

		messageAdd(&out, &in);
		lockConsumerCount();
		incrementConsumeCount();
		unlockConsumerCount();
	}
	printf("[messageAdder] %d termination\n", gettid());
	pthread_exit(NULL); // pour tuer le thread correctement
}


/*
Potential improvements to ask the teacher about
*/