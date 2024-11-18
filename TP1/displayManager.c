#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "displayManager.h"
#include "iDisplay.h"
#include "iAcquisitionManager.h"
#include "iMessageAdder.h"
#include "msg.h"
#include "multitaskingAccumulator.h"
#include "debug.h"

// DisplayManager thread.
pthread_t displayThread;

/**
 * Display manager entry point.
 * */
static void *display(void *parameters);

void displayManagerInit(void)
{
	if (pthread_create(&displayThread, NULL, display, NULL) != 0)
	{
		perror("displayManager thread creation error");
	}
	else
	{
		printf("displayManager thread created");
	}
}

void displayManagerJoin(void)
{
	if (pthread_join(displayThread, NULL) != 0)
	{
		perror("displayManager joining error");
	}
	else
	{
		printf("displayManager thread created");
	}
}

static void *display(void *parameters)
{
	D(printf("[displayManager]Thread created for display with id %d\n", gettid()));
	unsigned int diffCount = 0;
	MSG_BLOCK currentsum;
	while (diffCount < DISPLAY_LOOP_LIMIT)
	{
		sleep(DISPLAY_SLEEP_TIME);
		currentsum = getCurrentSum();
		print(getProductioncount(), getConsumedCount());
		messageDisplay(&currentsum);
	}
	printf("[displayManager] %d termination\n", gettid());
	pthread_exit(NULL); // pour tuer le thread correctement
}