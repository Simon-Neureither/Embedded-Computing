#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <errno.h>

#include <semaphore.h>


#define BILLION 1000000000L

void task1(void*);
void task2(void*);
void changeSystemTick(unsigned int nanos);

int main(int argc, char *argv[]) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int ret;
	pthread_t task1ID, task2ID;


	changeSystemTick(4e6);

	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	pthread_attr_getschedparam(&attr, &param);
	param.sched_priority = 5; // TODO 255.
	ret = pthread_attr_setschedparam(&attr, &param);



	ret = pthread_create(&task1ID, &attr, &task1, NULL);
	if (ret != EOK)
	{
		fprintf(stderr, "pthread_create: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}
	ret = pthread_create(&task2ID, &attr, &task2, NULL);
	if (ret != EOK)
	{
		fprintf(stderr, "pthread_create: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}

	ret = pthread_join(task1ID, NULL);
	if (ret != EOK)
	{
		fprintf(stderr, "pthread_join: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}
	ret = pthread_join(task2ID, NULL);
	if (ret != EOK)
	{
		fprintf(stderr, "pthread_join: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


void notBusyWait(int nanos)
{
	int ret;
	struct timespec timeOld;

	ret = clock_gettime(CLOCK_REALTIME, &timeOld);
	if (ret != 0)
	{
		fprintf(stderr, "error time: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}


	timeOld.tv_nsec+= nanos;
	if (timeOld.tv_nsec >= BILLION)
	{
		timeOld.tv_sec++;
		timeOld.tv_nsec=timeOld.tv_nsec - BILLION;
	}
	ret = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &timeOld, NULL);
	if (ret != 0)
	{
		fprintf(stderr, "error: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
}


void task1(void* arg)
{
	int i = 0;
	while (1)
	{
		notBusyWait(2e6);

		i = (i + 1) % 3;

		if (i == 1)
		{
			printf("semaphore\n");
		}
	}
}

void task2(void* arg)
{

	while (1)
	{
		printf("wait semaphore\n");
		noBusyWait(3e6);
	}

}


void changeSystemTick(unsigned int nanos)
{
	struct timespec res;
	int ret = clock_getres(CLOCK_REALTIME, &res);

	if (ret == -1)
	{
		perror("clock_getres");
		exit(EXIT_FAILURE);
	}

	struct _clockperiod NewClockPeriod;

	NewClockPeriod.nsec = nanos;
	NewClockPeriod.fract = 0;

	ret = ClockPeriod_r(CLOCK_REALTIME, &NewClockPeriod, NULL, 0);

	if (ret != EOK)
	{
		fprintf(stderr, "error: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
}
