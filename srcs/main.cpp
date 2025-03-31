/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:01:31 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/31 00:40:41 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{    
    if (argc != 3)
        return(std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, OK);       
	if (check_port(argv[1]) == OK && strlen(argv[2]) > 0 && strlen(argv[2]) < 31)
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
            // return (std::cerr << "main exception: " << e.what() << std::endl, KO); // destructor
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
    // check / clean the ":"
    std::string msg = "";
    bool is_msg = false;
    int i = 0;
    if (!str.empty())
    {
        if (str.at(0) == ':')
            str = str.substr(1);
        i = str.find_first_of(":");
        if (i != std::string::npos) // && i + 1 < str.length()) // worst case  "... :\r\n"
        {
            msg = str.substr(i);  // msg worst ":\r\n" // keep the ":"
            msg = msg.substr(0, msg.length() - 2); // remove \r\n
            std::cout << "msg = [" << msg << "]" << std::endl;
            if (!msg.empty())
                is_msg = true;
        }
    }
    
    // the spit by spaces    
    std::vector<std::string> tab;
    std::istringstream iss(str);
    std::string token;   
    
    while (iss >> token)
    {
        if (token.at(0) == ':') //&& token.at(0) != str.at(0))            
            break;                        
        tab.push_back(token);        
        std::cout << "token = [" << token << "]" << std::endl;
    }
    if (is_msg == true) 
        tab.push_back(msg);
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

