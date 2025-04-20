/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:37:38 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 19:18:29 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

# include <iostream>
# include <vector>
# include <string>
# include <cstdlib>
# include <ctime>

class Data
{
	private:
		std::vector<std::string> _subjects;
		std::vector<std::string> _verbs;
		std::vector<std::string> _adj;
		std::vector<std::string> _cc;
		std::vector<std::string> _forbidden_words;
	
	public:
		Data();
		~Data();
		void loadData(std::string csv_name, std::vector<std::string> &vec);
		std::string get_adj(void);
		std::string get_cc(void);
		std::string get_verbs(void);
		std::string get_subjects(void);
};

#endif