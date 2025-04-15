/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:13:35 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/15 03:08:40 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <vector>

class Client
{
    private:
        int _clt_skt;
        bool _registered;        
        bool _pwd_ok;        
        std::string _nickname;        
        std::string _username;
        std::string _hostname;
        std::string _realname;
        std::string _msg; //buffer
        bool _is_bot;
        int _bot_step;
        std::vector<std::string> _words;
        
    public:
        Client();
        ~Client();
        Client(const Client& other);
	    Client& operator=(const Client& other);
        //
        int get_clt_skt(void) const;
        std::string get_msg(void) const;
        std::string get_nickname(void) const;
        std::string get_hostname(void) const;
        std::string get_username(void) const;
        std::string get_realname(void) const;
        bool get_pwd_ok(void) const;        
        bool get_registered(void) const;
        bool get_is_bot(void) const;
        int get_bot_step(void) const;
        std::vector<std::string> get_words(void) const;
        //
        void set_clt_skt(int clt_skt);
        void set_nickname(std::string nickname);        
        void set_username(std::string username);
        void set_realname(std::string realname);
        void set_pwd_ok(bool pwd_ok);        
        void set_registered(bool registered);
        void set_hostname(std::string ip);
        void set_msg(std::string buffer);
        void set_is_bot(bool is_bot);
        void set_bot_step(int step);
        void add_word(std::string word);
        void clear_msg(void);
        void clear_words(void);
};

#endif

