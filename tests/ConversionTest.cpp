#include <iostream>
#include "../UltraSimpleUUID.h"
#include <map>
#include <cassert>

const int TEST_ITERATION_COUNT = 16;

int main(int argc, char* argv[])
{    

    // Test the creation of random UUIDs, serialize to/from string
    for (int i = 0; i < TEST_ITERATION_COUNT; i++)
    {
        UltraSimpleUUID::Uuid id;
        id.randomize();
        std::string u = id.toString();
        UltraSimpleUUID::Uuid oid;
        oid.fromString(u);
        assert(u == oid.toString());
    }

    // Test isNil
    {
        UltraSimpleUUID::Uuid id;
        assert(id.isNil());
        id.randomize();
        assert(!id.isNil());
    }

    // Test Combine
    {
        std::string base_uuid = "usS5mo9r-MvTB-hxHb-xNVw-PDh9SgaLrH9f";
        std::string long_string = "The Quick Brown Fox Jumped Over the Lazy Dog! N@n-^1p¨"; // Long, includes nonalphanumeric
        std::string short_string = "hi!"; // short, includes nonaplhanumeric
        UltraSimpleUUID::Uuid id;
        assert(id.fromString(base_uuid));
        id.combine(long_string);
        assert("8Ei7JGoP-bXKg-1OJ9-rtxF-PwSx5I6cluzB" == id.toString());
        id.fromString(base_uuid);
        id.combine(short_string);
        assert("7ZzlTLpY-tbAi-NeoH-euBd-wtOg8N71YopM" == id.toString());
    }



    std::cout << "Done\n";
}