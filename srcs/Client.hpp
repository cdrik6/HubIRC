/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:13:35 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/24 03:27:28 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class Client
{
    private:
        int _clt_skt;
        bool _registered;
        std::string _password;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _msg; //buffer
        
    public:
        Client();
        ~Client();
        //
        int get_clt_skt(void) const;
        std::string get_msg(void) const;
        std::string get_nickname(void) const;
        std::string get_password(void) const;
        void set_clt_skt(int clt_skt);
        void set_nickname(std::string nick);
        void set_password(std::string password);
        void set_hostname(std::string ip);
        void set_msg(std::string buffer);
        void clear_msg(void);
};

#endif
