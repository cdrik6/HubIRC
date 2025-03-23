/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:01:31 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/23 20:15:23 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{    
    if (argc != 3)
        return(std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, OK);       
	if (check_port(argv[1]) == OK && strlen(argv[2]) > 0)
    {           
        signal(SIGINT, Server::handle_signal);
        signal(SIGQUIT, Server::handle_signal);
        Server irc(argv[1], std::string(argv[2])); //srv_skt here if ok, exit() if not
        try
        {            
            irc.polling();
        }
        catch (std::exception &e)
        {
            // **** destructor called (otherwise would need to close fds) *** /
            return (std::cerr << "main exception: " << e.what() << std::endl, KO); // destructor
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

/*****be carefull delete all spaces "H   W" = ["H","W"] **********/
std::vector<std::string> split(std::string str)
{
    std::vector<std::string> tab;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token)
    {
        tab.push_back(token);
        // std::cout << "token =" << token << std::endl;
        
    }       
    return (tab);
}

// Copy inside the function is better because it avoids unnecessary work 
// when calling the function, making it more efficient for large strings.
std::string toUpper(const std::string& str)
{
    std::string UpStr = str;
    for (size_t i = 0; i < UpStr.length(); i++)    
        UpStr[i] = toupper(UpStr[i]);
    return (UpStr);
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
