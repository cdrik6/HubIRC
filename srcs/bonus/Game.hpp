/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandm <alexandm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:24:54 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/19 23:09:20 by alexandm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GAME_HPP
# define GAME_HPP

class Bot;

#include <iostream>
#include <string>
#include <vector>

class Game
{
	public:
		Game(Bot *bot);
		~Game();
		void setGameOn(bool); // ou init 
		bool getGameOn() const;
		// void continueGame(std::string word); use this input to continue the game , will do the reply thanks to ptr bot
		// void reset(); // reset all member variable to initial state or function to restart the game ???

		
	private:
		Bot *_bot;
		bool _game_on;
		int	 _step; // maybe useless if we use _words.size to know which step we are on
		std::vector<std::string> _words;

		
};

#endif