/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:30:22 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/19 02:35:14 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Bot.hpp"

int main(int argc, char **argv)
{    
    if (argc != 4)
        return(std::cerr << "Usage: ./bot <botname> <port> <password>" << std::endl, OK);       
	if (strlen(argv[1]) > 0 && strlen(argv[1]) < PASSLEN && check_port(argv[2]) == OK && strlen(argv[3]) > 0 && strlen(argv[3]) < PASSLEN)
    {           
        // signal(SIGINT, Client::handle_signal);
        // signal(SIGQUIT, Client::handle_signal);		
        Bot bot;
        try
        {         
            bot.initialize_connection( std::string(argv[1]), atoi(argv[2]), std::string(argv[3]) );   
            bot.connect_to_server();
            // bot.routine();
        }
        catch (std::exception &e)
        {            
            std::cerr << "main exception: " << e.what() << std::endl;
        }        
    }
    // destructor
    return (OK);
}

int check_port(char* port)
{   
    char *endval = NULL;     
    errno = 0;    
    long num = std::strtol(port, &endval, 10);
    if (*endval)
        return (std::cerr << "Invalid port: not a number\n", KO);
    if (errno == ERANGE) // overflow-underflow
        return (std::cerr << "Invalid port: overflow-underflow error\n", KO);    
    if (num < 1024)
        return (std::cerr << "Invalid port (port available >= 1024)\n", KO);
    if (num > 49151)
        return (std::cerr << "Invalid port (port available <= 49151\n", KO);    
    return (OK);
}
