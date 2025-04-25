/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:42:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 23:38:57 by caguillo         ###   ########.fr       */
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
        int _limit;
        std::vector<int> _chnlclts; // clt_fd
        std::vector<int> _operators;        
        std::vector<int> _invitees;
        bool _mode_i; // set(true)-remove(false) Invite-only
        bool _mode_t; // set(true)-remove(false) restrict TOPIC to _operators
        // bool _mode_k; // set(true)-remove(false) channel key
        // bool _mode_o; // give(true)-take(false) channel operator privilege
        // bool _mode_l; // set(true)-remove(false) the user limit
        
    public:
        Channel();
        ~Channel();
        Channel(const Channel& other);
	    Channel& operator=(const Channel& other);
        //
        std::string get_name(void) const;
        std::string get_key(void) const;
        std::string get_topic(void) const;
        std::string get_setby(void) const;
        time_t get_setat(void) const;
        std::vector<int> get_operators(void) const;
        std::vector<int> get_invitees(void) const;        
        std::vector<int> get_chnlclts(void) const;
        bool get_mode_i(void) const;
        bool get_mode_t(void) const;        
        int get_limit(void) const;
        //        
        void set_name(std::string name);    
        void set_key(std::string key);
        void set_topic(std::string topic);
        void set_setby(std::string nick);
        void set_setat(void);        
        void set_chnlclts(int clt_fd);
        void set_mode_i(bool invite_only);
        void set_mode_t(bool restrict_topic);
        void set_limit(int limit);
        void add_operator(int clt_fd);
        void rem_operator(int clt_fd);
        bool is_operator(int clt_fd);
        void add_invitee(int clt_fd);
        void rem_invitee(int clt_fd);
        bool is_invitee(int clt_fd);        
        void rem_chnlclt(int chnlclt_idx);    
};

#endif

