#include <iostream>
#include "../UltraSimpleUUID.h"
#include <map>
#include <vector>



int main(int argc, char* argv[])
{    
    for (int i=0; i<1024; i++)
        std::cout << UltraSimpleUUID::generate() << std::endl;

    std::cout << "Done\n";
    
}