/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoberon <aoberon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:30:22 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/18 17:21:29 by aoberon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Bot.hpp"

int main(int argc, char **argv)
{    
    if (argc != 4)
        return(std::cerr << "Usage: ./bot <botname> <port> <password>" << std::endl, OK);       
	if (strlen(argv[1]) > 0 && strlen(argv[1]) < 31 && check_port(argv[2]) == OK && strlen(argv[3]) > 0 && strlen(argv[3]) < 31)
    {           
        // signal(SIGINT, Client::handle_signal);
        // signal(SIGQUIT, Client::handle_signal);
		// Client bot(argv[2]); // clt_skt here if ok, exit() if not
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
