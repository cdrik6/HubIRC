/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:01:31 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/16 03:20:16 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{    
    if (argc != 3)
        return(std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, OK);       
	if (check_port(argv[1]) == OK && strlen(argv[2]) > 0)
    {           
        Server irc(argv[1], std::string(argv[2]));
        signal(SIGINT,  &Server::handle_signal);
        signal(SIGQUIT, irc.handle_signal);        
        try
        {            
            irc.polling();
        }
        catch (std::exception &e)
        {
            return (std::cerr << e.what() << std::endl, KO);
        }
    }    
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
    if (num <= 1024)
        return (std::cerr << "Invalid port (port available > 1024)\n", KO);
    if (num >= 10000)
        return (std::cerr << "Invalid port (port available < 10000\n", KO);    
    return (OK);
}

// int check_input(char *argv1, char *argv2)
// {
//     if (check_port(argv1) == KO)
//         return (KO);
//     if (check_pwd(std::string(argv2)) == KO)
//         return (KO);
//     return (OK);
// }

// int check_pwd(std::string pwd)
// {
//     std::istringstream iss2(std::string(argv2));
// }

/**
 * SIGINT = ctrl+c
 * SIGQUIT = ctrl+\
 * ctrl+d = not a signal
**/ 
