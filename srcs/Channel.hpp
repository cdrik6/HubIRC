/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:42:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/03 21:12:20 by caguillo         ###   ########.fr       */
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
        std::vector<int> _tab_clt_idx;        
    
    public:
        Channel();
        ~Channel();
        std::string get_name(void) const;
        std::string get_key(void) const;
        std::string get_topic(void) const;
        std::vector<std::string> get_operators(void) const;
        std::vector<int> get_tab_clt_idx(void) const;
        void set_name(std::string name);    
        void set_key(std::string key);        
        void set_tab_clt_idx(int clt_idx);
        void add_operators(std::string nick);
        void sub_operators(std::string nick);
        bool is_operator(std::string nick);
};

#endif