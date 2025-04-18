/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandm <alexandm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:37:38 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/18 15:42:50 by alexandm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>

class Data
{
	private:
		std::vector<std::string> _subjects;
		std::vector<std::string> _verbs;
		std::vector<std::string> _co;
		std::vector<std::string> _cc;
		std::vector<std::string> _forbidden_words;

		static _signal; // static makes _signal shared across all instances

	
	public:
		Data();
		~Data();
		void loadData(std::string csv_name, std::vector<std::string> &vec);
};