/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:42:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/08 21:42:58 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _key;
        std::string _topic;
        std::string _setby;
        time_t _setat;
        std::vector<std::string> _operators;
        std::vector<Client> _chnlclts;
        bool _mode_i; // set(true)-remove(false) Invite-only //--> to implement in JOIN / INVITE /************** */
        bool _mode_t; // set(true)-remove(false) restrict TOPIC to _operators //--> to implement in TOPIc /************** */
        // bool _mode_k; // set(true)-remove(false) channel key
        // bool _mode_o; // give(true)-take(false) channel operator privilege
        // bool _mode_l; // set(true)-remove(false) the user limit
        int _limit; //--> to implement in JOIN /************** */
    
    public:
        Channel();
        ~Channel();
        std::string get_name(void) const;
        std::string get_key(void) const;
        std::string get_topic(void) const;
        std::string get_setby(void) const;
        time_t get_setat(void) const;
        std::vector<std::string> get_operators(void) const;
        std::vector<Client> get_chnlclts(void) const;
        bool get_mode_i(void) const;
        bool get_mode_t(void) const;
        // bool get_mode_k(void) const;
        // bool get_mode_o(void) const;
        // bool get_mode_l(void) const;
        int get_limit(void) const;
        void set_name(std::string name);    
        void set_key(std::string key);
        void set_topic(std::string topic);
        void set_setby(std::string nick);
        void set_setat(void);
        void set_chnlclts(Client client);
        void set_mode_i(bool invite_only);
        void set_mode_t(bool restrict_topic);
        void set_limit(int limit);
        void add_operator(std::string nick);
        void rem_operator(std::string nick);
        bool is_operator(std::string nick);        
        void rem_client(int idx);        
};

#endif