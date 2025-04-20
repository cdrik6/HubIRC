/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:54:15 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 04:12:53 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game(Bot *bot) : _bot(bot), _game_on(false)
{
}

Game::Game()
{	
}

Game::~Game()
{	
}

void Game::setGameOn(bool on)
{
	this->_game_on = on;
}

bool Game::getGameOn() const
{
	return (this->_game_on);
}

void Game::startGame(std::string channel)
{
	std::cout << "je suis la " << std::endl;
	_bot->reply("PRIVMSG " + channel + " :Donnez-moi le sujet de la phrase (ex: le chat)" + "\r\n");			
}

void Game::playing(Data& data, std::string channel, std::string word)
{		
	if (_words.size() == 0)	
	{
		std::string adj = data.get_adj();
		std::cout << "adj = " << adj << std::endl;
		
		_words.push_back(word);
		_words.push_back(adj);
		_bot->reply("PRIVMSG " + channel + " :Donnez-moi le verbe 3e p.s. de la phrase (ex: boit)" + "\r\n");
	}
	else if (_words.size() == 2)
	{
		std::string cc = data.get_cc();
		std::cout << "cc = " << cc << std::endl;
		
		_words.push_back(word);
		_words.push_back(cc);

		if (_words.size() == 4)
		{
			std::string sentence = _words.at(0) + " " + _words.at(1) + " " + _words.at(2) + " " + _words.at(3) + ".";
			_bot->reply("PRIVMSG " + channel + " :" + sentence + "\r\n");
		}
		_game_on = false;
	}
}
