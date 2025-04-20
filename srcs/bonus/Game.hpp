/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:24:54 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 02:40:30 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Data.hpp"
# include "Bot.hpp"

class Bot;

class Game
{
	public:
		Game(Bot *bot);
		Game();
		~Game();
		void setGameOn(bool); // ou init 
		bool getGameOn() const;
		void startGame(std::string channel);
		void playing(Data& data, std::string channel, std::string word);
		// void continueGame(std::string word); use this input to continue the game , will do the reply thanks to ptr bot
		// void reset(); // reset all member variable to initial state or function to restart the game ???
		
	private:
		Bot *_bot;
		bool _game_on;
		std::vector<std::string> _words;
		// int	 _step; // maybe useless if we use _words.size to know which step we are on
		
};

#endif