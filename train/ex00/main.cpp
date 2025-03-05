#include "ClapTrap.hpp"

int main(int argc, char** argv)
{
    srand(time(NULL));
    ClapTrap toto("TOTO");
    ClapTrap titi("TITI");
    std::cout << toto.get_name() << " : hit : " << toto.get_hit() << std::endl;
    std::cout << toto.get_name() << " : energy : " << toto.get_energy() << std::endl;
    std::cout << toto.get_name() << " : damage : " << toto.get_damage() << std::endl;
    std::cout << titi.get_name() << " : hit : " << titi.get_hit() << std::endl;
    std::cout << titi.get_name() << " : energy : " << titi.get_energy() << std::endl;
    std::cout << titi.get_name() << " : damage : " << titi.get_damage() << std::endl;
    toto.set_hit(30);
    toto.set_energy(10);
    toto.set_damage(10);    
    titi.set_hit(30);
    titi.set_energy(10);
    titi.set_damage(10);
    unsigned int tmp_energy;
    std::cout << std::endl;
    // fights
    for (int i = 0; i < 10; i++)
    {
        std::cout << "------------------------- Fight " << i + 1 << " ---------------------------" <<std::endl;
        std::cout << std::endl;        
        // fight
        tmp_energy = toto.get_energy();       
        toto.attack(titi.get_name());        
        if (toto.get_hit() && tmp_energy)
        {
            titi.takeDamage(toto.get_damage());    
            titi.beRepaired(rand()%10);
        }
        std::cout << std::endl;
        // fight back
        tmp_energy = titi.get_energy();
        titi.attack(toto.get_name());
        if (titi.get_hit() && tmp_energy)
        {
            toto.takeDamage(titi.get_damage());    
            toto.beRepaired(rand()%10);    
        }
        std::cout << std::endl;
    }
    // result
    std::cout << "------------------------- Result ----------------------------" <<std::endl;
    std::cout << toto.get_name() << ": health level: " << toto.get_hit() << std::endl;
    std::cout << titi.get_name() << ": health level: " << titi.get_hit() << std::endl;        
    std::cout << "Winner: "<< (toto.get_hit() > titi.get_hit() ? toto.get_name() : titi.get_name()) << std::endl;
    std::cout << std::endl;
    return (0);



}
