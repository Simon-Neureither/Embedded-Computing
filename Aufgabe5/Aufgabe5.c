#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

	unsigned int done = 0, led1 = 0, led2 = 0;
	unsigned char leds;

	int ledfile = open("/dev/leds", O_WRONLY);
	if (ledfile < 0)
	{
		perror("file open");
		return EXIT_FAILURE;
	}

	while (!done)
	{
		printf("LED 1 und LED 2:\n");
		if (scanf("%d %d", &led1, &led2) != 2)
		{
			printf("Atompilz!\n");
			return EXIT_FAILURE;
		}
		if (led1 > 1 || led2 > 1)
		{
			printf("Falsche Eingabe.\n");
			continue;
		}
		leds = 0;
		leds = led1 | (led2 << 1) | ((led1 ^ led2) << 2) | ((led1 | led2) << 3);
		if (write(ledfile, &leds, 1) < 1)
		{
			perror("write");
			return EXIT_FAILURE;
		}
		printf("Wollen sie aufhören?\n");
		if (scanf("%d", &done) != 1)
		{
			printf("Atompilz #2!\n");
			return EXIT_FAILURE;
		}
	}

	if (close(ledfile))
	{
		perror("file close");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
