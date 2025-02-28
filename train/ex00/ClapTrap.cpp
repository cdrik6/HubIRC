#include "ClapTrap.hpp"

ClapTrap::ClapTrap(): _name("default"), _hit(10), _energy(10), _damage(0)
{    
    std::cout << "Default ClapTrap constructor called" << std::endl;
}

ClapTrap::ClapTrap(std::string name): _name(name), _hit(10), _energy(10), _damage(0)
{
    std::cout << "ClapTrap " << _name << " constructed" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap& other)
{
    std::cout << "ClapTrap copy constructor of " << other._name << std::endl;
    *this = other;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other)
{
    std::cout << "ClapTrap copy assignment operator called" << std::endl;
    if (this == &other)
        return (*this);
    _name = other._name;
    _hit = other._hit;
    _energy = other._energy;
    _damage = other._damage;
    return (*this);
}

ClapTrap::~ClapTrap()
{
    std::cout << "ClapTrap " << _name << " destruted" << std::endl;
}

//
std::string ClapTrap::get_name(void) const
{
    return (_name);
}

unsigned int ClapTrap::get_hit(void) const
{
    return (_hit);
}

unsigned int ClapTrap::get_energy(void) const
{
    return (_energy);
}

unsigned int ClapTrap::get_damage(void) const
{
    return (_damage);
}

void ClapTrap::set_hit(unsigned int hit_points)
{
    _hit = hit_points;
}

void ClapTrap::set_energy(unsigned int energy_points)
{
    _energy = energy_points;
}

void ClapTrap::set_damage(unsigned int damage_points)
{
    _damage = damage_points;
}

//
void ClapTrap::attack(const std::string& target)
{
    if (_hit == 0)
		std::cout << "ClapTrap " << _name << " is dead, can't attack " << target << std::endl;
	else if (_energy == 0)
		std::cout << "ClapTrap " << _name << " is tired, can't attack " << target << std::endl;
	else
	{
		_energy = _energy - 1;
		std::cout << "ClapTrap " << _name << " attacks " << target;
		std::cout << ", causing " << _damage << " points of damage!" << std::endl;
	}
}

void ClapTrap::takeDamage(unsigned int amount)
{
    if (_hit == 0)
        std::cout << "ClapTrap " << _name << " is already dead" << std::endl;
    else
    {
        if (_hit > amount)
        {
            _hit = _hit - amount; 
            std::cout << "ClapTrap " << _name << " has taken " << amount << " points of damage" << std::endl;
            std::cout << "ClapTrap " << _name << " health level: " << _hit << std::endl;
        }
        else
        {
            _hit = 0;
            std::cout << "ClapTrap " << _name << " has taken " << amount << " points of damage" << std::endl;
            std::cout << "ClapTrap " << _name << " is now dead" << std::endl;
        }          
    }    
}

void ClapTrap::beRepaired(unsigned int amount)
{
    if (_hit == 0)
        std::cout << "ClapTrap " << _name << " is dead, too late to be repaired" << std::endl;
    else if (_energy == 0)
    {
        std::cout << "ClapTrap " << _name << " is too tired to be repaired" << std::endl;
    }
    else    
    {
        _hit = _hit + amount;
        _energy = _energy - 1;
        std::cout << "ClapTrap " << _name << " has been repaired by " << amount << " points" << std::endl;
        std::cout << "ClapTrap " << _name << " health level: " << _hit << std::endl;
    }
}