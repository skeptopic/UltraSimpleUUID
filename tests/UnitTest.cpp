/*
Neil Chatterjee 2020

This serves as a unit test for the functionality of UltraSimpleUUID.

This includes:
* Generation
* String Serialization/Deserialization
* IsNil
* Combining UUIDs
* Buffers
* Equality Operator

*/

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

    // Test Buffer
    {
        // Test getting the buffer directly and then setting it again (is it equal?)
        for (int i = 0; i < TEST_ITERATION_COUNT; i++)
        {
            UltraSimpleUUID::Uuid id;
            id.randomize();
            char* buffer = nullptr;
            unsigned short size = id.getBuffer(&buffer);
            assert(size == UltraSimpleUUID::BUFFER_SIZE);
            UltraSimpleUUID::Uuid oid;
            assert(oid.fromBuffer(buffer, size));
            assert(id.toString() == oid.toString());
        }

        // Test that a given known buffer translates to a given known UUID
        {
            int buffer_ints[UltraSimpleUUID::BUFFER_SIZE] = { 47, 23, -67, 32, -24, 33, -97, 25, 106, -10, 2, -86, 63, 92, -3, 86, 123, -40, 45, -123, -121, 47, 60, 123 };
            std::string uuid_text = "vzfVOVMY-Zbju-TcSK-R6xx-hv8BoiOcgbeK";

            char buffer[UltraSimpleUUID::BUFFER_SIZE] = { 0, };
            for (int i = 0; i < UltraSimpleUUID::BUFFER_SIZE; i++)
            {
                char c = buffer_ints[i];
                buffer[i] = c;
            }
            UltraSimpleUUID::Uuid c;
            c.fromBuffer(buffer, UltraSimpleUUID::BUFFER_SIZE);
            assert(c.toString() == uuid_text);
        }
        
    }


    // Test Equality Operator
    for (int i = 0; i < TEST_ITERATION_COUNT; i++)
    {
        UltraSimpleUUID::Uuid id;
        id.randomize();
        std::string u = id.toString();
        UltraSimpleUUID::Uuid oid;
        oid.fromString(u);
        assert(id == oid);
        oid.combine("Some Aribrary Text");
        assert(id != oid);
    }

    std::cout << "Done\n";
}