/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:13:35 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/20 02:58:07 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
    private:
        int _clt_skt;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _msg; //buffer
    public:
        Client(){};
        ~Client(){};
        void set_clt_skt(int clt_skt);
        void set_hostname(std::string ip);
};


#endif