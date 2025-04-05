/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:42:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/05 01:23:59 by caguillo         ###   ########.fr       */
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
        std::vector<std::string> _operators;
        std::vector<Client> _chnlclts;
    
    public:
        Channel();
        ~Channel();
        std::string get_name(void) const;
        std::string get_key(void) const;
        std::string get_topic(void) const;
        std::vector<std::string> get_operators(void) const;
        std::vector<Client> get_chnlclts(void) const;
        void set_name(std::string name);    
        void set_key(std::string key);        
        void set_chnlclts(Client client);
        void add_operator(std::string nick);
        void rem_operator(std::string nick);
        bool is_operator(std::string nick);        
        void rem_client(int idx);
        
};

#endif