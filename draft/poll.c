/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:57:55 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/06 22:24:46 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// struct pollfd {
//     int fd;         // the socket descriptor
//     short events;   // bitmap of events we're interested in
//     short revents;  // when poll() returns, bitmap of events that occurred
// };
// events :
// POLLIN 	Alert me when data is ready to recv() on this socket.
// POLLOUT 	Alert me when I can send() data to this socket without blocking.
// POLLHUP 	Alert me when the remote closed the connection.

#include <stdio.h>
#include <poll.h>

int main(void)
{
    struct pollfd pfds[1]; // More if you want to monitor more

    pfds[0].fd = 0;          // Standard input
    pfds[0].events = POLLIN; // Tell me when ready to read
    printf("Hit RETURN or wait 2.5 seconds for timeout\n");    	
    if (poll(pfds, 1, 2500) == 0)	// 2.5 second timeout
    	printf("Poll timed out!\n");
    else
	{
        int pollin_happened = pfds[0].revents & POLLIN;
		//
        if (pollin_happened) // !=0 POLLIN returned (at least)
            printf("File descriptor %d is ready to read\n", pfds[0].fd);
        else // ==0 POLLIN not returned
            printf("Unexpected event occurred: %d\n", pfds[0].revents);        		
		//
		// /*********** wrong below : POLLIN may has been returned and
		//  the pfds[0].revents is not purely = to POLLIN
		//  --> so bitwise operator needed ****************/
        // if (pfds[0].revents == POLLIN) 
        //     printf("File descriptor %d is ready to read\n", pfds[0].fd);
        // else
        //     printf("Unexpected event occurred: %d\n", pfds[0].revents);        
    }

    return 0;
}