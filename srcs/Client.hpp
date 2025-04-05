/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:13:35 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/05 03:51:25 by caguillo         ###   ########.fr       */
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
        bool _pwd_ok;
        bool _is_op;        
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
        std::string get_hostname(void) const;
        std::string get_username(void) const;
        bool get_pwd_ok(void) const;
        bool get_is_op(void) const;
        bool get_registered(void) const;
        //
        void set_clt_skt(int clt_skt);
        void set_nickname(std::string nick);
        // void set_oldnick(std::string nick);
        void set_username(std::string username);
        void set_pwd_ok(bool pwd_ok);
        void set_registered(bool registered);
        void set_hostname(std::string ip);
        void set_msg(std::string buffer);
        void clear_msg(void);
};

#endif
