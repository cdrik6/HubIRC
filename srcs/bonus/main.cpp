/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:30:22 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/18 04:55:14 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int main(int argc, char **argv)
{    
    if (argc != 3)
        return(std::cerr << "Usage: ./bot localhost <port>" << std::endl, OK);       
	if (check_port(argv[2]) == OK && (std::string(argv[1]) == "localhost" || std::string(argv[1]) == "127.0.0.1"))
    {           
        signal(SIGINT, Client::handle_signal);
        signal(SIGQUIT, Client::handle_signal);
		Client bot(argv[2]); // clt_skt here if ok, exit() if not
        try
        {            
            bot.polling();
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
