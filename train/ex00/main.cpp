#include "ClapTrap.hpp"

int main(int argc, char** argv)
{
    srand(time(NULL));
    ClapTrap toto("TOTO");
    ClapTrap titi("TITI");
    std::cout << toto.get_name() << " : hit : " << toto.get_hit() << std::endl;
    std::cout << toto.get_name() << " : energy : " << toto.get_energy() << std::endl;
    std::cout << toto.get_name() << " : damage : " << toto.get_energy() << std::endl;


}
