/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_example.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:03:13 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/05 11:12:29 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

// thread_routine is the function the thread invokes right after its
// creation. The thread ends at the end of this function.
void	*thread_routine(void *data)
{
	// pthread_t tid;

	(void) data;
	// The pthread_self() function provides
	// this thread's own ID.
	// tid = pthread_self();
	printf("Thread: The heaviest burden is to exist without living.\n");
	return (NULL); // The thread ends here.
}

int	main(void)
{
	pthread_t tid1; // First thread's ID
	pthread_t tid2; // Second thread's ID

	// Creating the first thread that will go
	// execute its thread_routine function.
	pthread_create(&tid1, NULL, thread_routine, NULL);
	printf("Main: Created first thread\n");
	// Creating the second thread that will also execute thread_routine.
	pthread_create(&tid2, NULL, thread_routine, NULL);
	printf("Main: Created second thread\n");
	// The main thread waits for the new threads to end
	// with pthread_join.
	pthread_join(tid1, NULL);
	printf("Main: Joining first thread\n");
	pthread_join(tid2, NULL);
	printf("Main: Joining second thread\n");
	return (0);
}
