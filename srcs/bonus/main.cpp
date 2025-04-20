/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:30:22 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/20 03:35:14 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"
#include "Data.hpp"

int main(int argc, char **argv)
{    
    if (argc != 4)
        return(std::cerr << "Usage: ./bot <botname> <port> <password>" << std::endl, OK);       
	if (strlen(argv[1]) > 0 && strlen(argv[1]) < PASSLEN && check_port(argv[2]) == OK && strlen(argv[3]) > 0 && strlen(argv[3]) < PASSLEN)
    {           
        signal(SIGINT, Bot::handle_signal);
        signal(SIGQUIT, Bot::handle_signal);		
        Bot bot;
        Data data;
        try
        {         
            bot.initialize_connection(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]));   
            bot.connect_to_server();
			bot.select_initialisation();   
            while (bot.get_signal() == false)
			{
				if (bot.fd_ready_for_recv() == false)
					break;
                bot.routine(data);                
			}            
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

std::vector<std::string> split(std::string str)
{
    std::vector<std::string> tab;
    std::istringstream issbuff(str);
    std::string line;

    while (getline(issbuff, line))
    {
        // check / clean the ":"
        std::string msg = "";
        bool is_msg = false;
        int i = 0;        
        if (!line.empty())
        {
            if (line.at(0) == ':')
                line = line.substr(1);
            i = line.find_first_of(":");
            if (i != std::string::npos) // worst case  "... :\r\n"
            {
                msg = line.substr(i);  // msg worst ":\r" // keep the ":"
                if (msg.at(msg.length() - 1) == '\r')
                    msg = msg.substr(0, msg.length() - 1); // remove \r                
                // std::cout << "msg = [" << msg << "]" << std::endl;
                if (!msg.empty())
                    is_msg = true;
            }
        }
        
        // the split by spaces       
        std::istringstream iss(line);
        std::string token;  
        
        while (iss >> token)
        {
            if (token.at(0) == ':') //&& token.at(0) != str.at(0))            
                break;                        
            tab.push_back(token);        
            // std::cout << "token = [" << token << "]" << std::endl;
        }
        if (is_msg == true) 
            tab.push_back(msg);        
    }
    // std::cout << std::endl;
    return (tab);
}