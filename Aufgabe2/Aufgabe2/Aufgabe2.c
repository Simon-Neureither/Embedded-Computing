#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/neutrino.h>
#include <errno.h>

int clockMeasure();
void changeSystemTick(unsigned int microsecs);


int main(int argc, char *argv[]) {
	changeSystemTick(536869);
	int ret = clockMeasure();
	if (ret == EXIT_FAILURE) return ret;
	//changeSystemTick(10);
	//clockMeasure();
	//if (ret == EXIT_FAILURE) return ret;
	changeSystemTick(1000);

	return EXIT_SUCCESS;
}

int clockMeasure()
{
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

	ret = clock_gettime(CLOCK_REALTIME, &timeNew); // TODO
	long seconds = timeNew.tv_sec - startTime.tv_sec;
	long nanos = timeNew.tv_nsec - startTime.tv_nsec;
	if (nanos < 0)
	{
		nanos+=BILLION;
		seconds--;
	}
	printf("%lu:%lu\r\n", seconds, nanos);
}

void changeSystemTick(unsigned int microsecs)
{
	struct timespec res;
	int ret = clock_getres(CLOCK_REALTIME, &res);

	if (ret == -1)
	{
		perror("clock_getres");
		exit(EXIT_FAILURE);
	}

	printf("Old: %lu\n", res.tv_nsec);

	struct _clockperiod NewClockPeriod;

	NewClockPeriod.nsec = microsecs * 1000;
	NewClockPeriod.fract = 0;

	ret = ClockPeriod_r(CLOCK_REALTIME, &NewClockPeriod, NULL, 0);

	if (ret != EOK)
	{
		fprintf(stderr, "error: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}

	ret = clock_getres(CLOCK_REALTIME, &res);
	if (ret == -1)
	{
		perror("clock_getres");
		exit(EXIT_FAILURE);
	}

	printf("New: %lu\n", res.tv_nsec);
}
