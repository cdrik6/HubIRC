/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:54:15 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/20 20:31:44 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game(Bot *bot, std::string channel) : _bot(bot), _game_on(false), _channel(channel)
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

void Game::startGame()
{
	_scenario = rand()%2;
	std::cout << _scenario << std::endl;
	if (_scenario == 0)
		_bot->reply("PRIVMSG " + _channel + " :Donnez-moi le sujet de la phrase (ex: le chat)" + "\r\n");			
	else
	{		
		_bot->reply("PRIVMSG " + _channel + " :Donnez-moi un adjectif (ex: aride)" + "\r\n");
	} 	
		
}

void Game::playing(Data& data, std::string word)
{		
	if (_words.size() == 0)	
	{
		if (_scenario == 0)
		{
			_words.push_back(word);
			_words.push_back(data.get_adj());
			_bot->reply("PRIVMSG " + _channel + " :Donnez-moi un verbe 3e p.s. de la phrase (ex: se saoule)" + "\r\n");
		}			
		else 	
		{
			_words.push_back(data.get_subjects());
			_words.push_back(word);
			_bot->reply("PRIVMSG " + _channel + " :Donnez-moi un complement de la phrase (ex: a cause de la pluie)" + "\r\n");
		}
	}
	else if (_words.size() == 2)
	{
		if (_scenario == 0)
		{
			
			_words.push_back(word);
			_words.push_back(data.get_cc());
		}
		else
		{
			_words.push_back(data.get_verbs());
			_words.push_back(word);
		}

		if (_words.size() == 4)
		{
			std::string sentence = _words.at(0) + " " + _words.at(1) + " " + _words.at(2) + " " + _words.at(3) + ".";
			_bot->reply("PRIVMSG " + _channel + " :" + sentence + "\r\n");
			
		}
		_words.clear();
		_game_on = false;
	}
}
