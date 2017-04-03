#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void* task(void* arg)
{
	sleep(*((int*)arg));
	printf("Einen Text %u\r\n", pthread_self());
	return (void*)pthread_self();
}

int main( void )
{
   pthread_attr_t attr;
   pthread_attr_init( &attr );

   pthread_t thread1;
   pthread_t thread2;

   int waitThread1 = 5;
   int waitThread2 = 2;

   int ret = pthread_create( &thread1, &attr, &task, &waitThread1 );

   if (ret != EOK)
   {
	   fprintf(stderr, "pthread_create: %s", strerror(ret));
	   return EXIT_FAILURE;
   }

   ret = pthread_create( &thread2, &attr, &task, &waitThread2 );

   if (ret != EOK)
   {
	   fprintf(stderr, "pthread_create: %s", strerror(ret));
	   return EXIT_FAILURE;
   }

   pthread_t taskID1;
   pthread_t taskID2;

   ret = pthread_join(thread2, (void*)&taskID2);
   if (ret != EOK)
   {
	   printf("pthread_join: %s", strerror(ret));
	   return EXIT_FAILURE;
   }

   ret = pthread_join(thread1, (void*)&taskID1);
   if (ret != EOK)
   {
	   printf("pthread_join: %s", strerror(ret));
	   return EXIT_FAILURE;
   }

   if (taskID1 != thread1)
   {
	   printf("Error thread id's do not match: %u %u\r\n", thread1, taskID1);
	   return EXIT_FAILURE;
   }

   if (taskID2 != thread2)
   {
	   printf("Error thread id's do not match: %u %u\r\n", thread2, taskID2);
	   return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
