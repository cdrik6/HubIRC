/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandm <alexandm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 22:54:15 by alexandm          #+#    #+#             */
/*   Updated: 2025/04/19 22:58:49 by alexandm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

Game::Game(Bot *bot) : _bot(bot), _game_on(false), _step(0)
{
}

Game::~Game()
{}

void Game::setGameOn(bool on)
{
	this->_game_on = on;
}

bool Game::getGameOn() const
{
	return (this->_game_on);
}