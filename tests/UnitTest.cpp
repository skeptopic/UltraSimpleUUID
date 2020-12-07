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

const int TEST_ITERATION_COUNT = 64;
#define UNIT_TEST_ASSERT(expr, msg) if(!expr) { std::cout << "ERROR: " << msg << std::endl; return -1; }

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
        UNIT_TEST_ASSERT((u == oid.toString()), "UUID string serialization/deserialization failed");
    }

    // Test isNil
    {
        UltraSimpleUUID::Uuid id;
        UNIT_TEST_ASSERT((id.isNil()), "Default contrsutcted UUID was not Nil");
        id.randomize();
        UNIT_TEST_ASSERT((!id.isNil()), "Randomized UUID had NIL value");
    }

    // Test Combine
    {
        std::string base_uuid = "usS5mo9r-MvTB-hxHb-xNVw-PDh9SgaLrH9f";
        std::string long_string = "The Quick Brown Fox Jumped Over the Lazy Dog! N@n-^1p¨"; // Long, includes nonalphanumeric
        std::string short_string = "hi!"; // short, includes nonaplhanumeric
        UltraSimpleUUID::Uuid id;
        UNIT_TEST_ASSERT((id.fromString(base_uuid)), "Deserialization from known string failed");
        id.combine(long_string);
        UNIT_TEST_ASSERT(("8Ei7JGoP-bXKg-1OJ9-rtxF-PwSx5I6cluzB" == id.toString()), "Combination with a known long string produced unexpected result");
        id.fromString(base_uuid);
        id.combine(short_string);
        UNIT_TEST_ASSERT(("7ZzlTLpY-tbAi-NeoH-euBd-wtOg8N71YopM" == id.toString()), "Combination with a known short string produced unexpected result");
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
            UNIT_TEST_ASSERT((size == UltraSimpleUUID::BUFFER_SIZE), "Size of retrieved buffer is unexpected");
            UltraSimpleUUID::Uuid oid;
            UNIT_TEST_ASSERT((oid.fromBuffer(buffer, size)), "Setting UUID from buffer failed");
            UNIT_TEST_ASSERT((id.toString() == oid.toString()), "UUID set from buffer does not equal reference UUID");
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
            UNIT_TEST_ASSERT((c.toString() == uuid_text), "UUID set from known buffer did not produce exected result");
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
        UNIT_TEST_ASSERT((id == oid), "Equality operator failed on two UUIDs set from the same string");
        oid.combine("Some Aribrary Text");
        UNIT_TEST_ASSERT((id != oid), "Inequality operator failed on two UUIDs that should be different");
    }

    std::cout << "Done\n";
    return 0;
}