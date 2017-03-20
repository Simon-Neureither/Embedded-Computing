#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

void* task(void* arg)
{
	sleep(*((int*)arg));
	printf("Einen Text %d\r\n", pthread_self());
	pthread_exit(pthread_self());
}

int main( void )
{
   pthread_attr_t attr;

   pthread_attr_init( &attr );
   pthread_attr_setdetachstate(
      &attr, PTHREAD_CREATE_JOINABLE );

   pthread_t thread1;
   pthread_t thread2;

   int waitThread1 = 5;
   int waitThread2 = 2;

   int ret = pthread_create( &thread1, &attr, &task, &waitThread1 );

   if (ret != EOK)
   {
	   printf("Error while creating first thread.");
	   return EXIT_FAILURE;
   }

   ret = pthread_create( &thread2, &attr, &task, &waitThread2 );

   if (ret != EOK)
   {
	   printf("Error while creating second thread.");
	   return EXIT_FAILURE;
   }


   int taskID1;
   int taskID2;

   ret = pthread_join(thread2, (void*)&taskID2);
   if (ret != EOK)
   {
	   printf("Error while joining first thread.");
	   return EXIT_FAILURE;
   }

   ret = pthread_join(thread1, (void*)&taskID1);
   if (ret != EOK)
   {
	   printf("Error while joining second thread.");
	   return EXIT_FAILURE;
   }

   if (taskID1 != thread1)
   {
	   printf("Error thread id's do not match: %d %d\r\n", thread1, taskID1);
	   return EXIT_FAILURE;
   }

   if (taskID2 != thread2)
   {
	   printf("Error thread id's do not match: %d %d\r\n", thread2, taskID2);
	   return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
