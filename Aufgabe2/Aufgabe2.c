#include <stdlib.h>
#include <stdio.h>

#include <time.h>

void changeSystemTick(unsigned int microsecs);

int main(int argc, char *argv[]) {
	int ret;
	struct timespec startTime;
	struct timespec timeOld;
	struct timespec timeNew;

	unsigned long BILLION = 1000000000L;

	int duration = 0;

	ret = clock_gettime(CLOCK_REALTIME, &startTime);
	if (ret != 0)
	{
		fprintf(stderr, "error starttime: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}

	ret = clock_gettime(CLOCK_REALTIME, &timeOld);
	if (ret != 0)
	{
		fprintf(stderr, "error time: %s\n", strerror(ret));
		return EXIT_FAILURE;
	}

	while (duration++ < 4500)
	{
		timeOld.tv_nsec+= 1000000;
		if (timeOld.tv_nsec >= BILLION)
		{
			timeOld.tv_sec++;
			timeOld.tv_nsec=timeOld.tv_nsec - BILLION;
		}
		ret = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &timeOld, NULL);
		if (ret != 0)
		{
			fprintf(stderr, "error: %s\n", strerror(ret));
			return EXIT_FAILURE;
		}
	}

	ret = clock_gettime(CLOCK_REALTIME, &timeNew);
	long seconds = timeNew.tv_sec - startTime.tv_sec;
	long nanos = timeNew.tv_nsec - startTime.tv_nsec;
	if (nanos < 0)
	{
		nanos+=BILLION;
		seconds--;
	}
	printf("%lu:%lu", seconds, nanos);

	return EXIT_SUCCESS;
}

void changeSystemTick(unsigned int microsecs)
{

}

}
