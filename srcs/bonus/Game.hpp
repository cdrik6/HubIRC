/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:24:54 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 23:01:33 by caguillo         ###   ########.fr       */
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
		Game(Bot *bot, std::string channel);
		Game();
		~Game();
		void setGameOn(bool);
		bool getGameOn() const;
		void startGame();
		void playing(Data& data, std::string word);
		
	private:
		Bot *_bot;
		bool _game_on;
		std::vector<std::string> _words;
		std::string _channel;
		int _scenario;
};

#endif