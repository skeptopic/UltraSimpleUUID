// Neil Chatterjee 2020
// The worlds simplest and smallest "random" UUID string generator. This is meant to be dependency-free and easy to build and run on different platforms.
// There are certainly UUID generation methods that are more 'industrial strength' but I found no header-only solutions like this.
// 
// I took inspiration for this from https://gist.github.com/fernandomv3/46a6d7656f50ee8d39dc, but randomness was broken there and it was
// feature-incomplete for my needs. 
//
// If you want a more correct implementaiton, check out  https://github.com/mariusbancila/stduuid, but I had problems building
// this on Linux and wanted something truly dependency-free!
//
// The simple idea here is that UUID version 4 id's are created by generating random numbers and using those to look up values
// from a lookup string that transfers them to one of the 62 valid characters. When not using the UUID as a string, it's encoded as
// 6 int32s. (24 bytes) This encoding should be platform independent, so it should be safe to transmit these bytes from one system to another.
//
#pragma once
#include <string>
#include <cstdlib>
#include <random>
#include <iostream>
#include <set>
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
#include <cassert>
#endif //DEEP_TEST_ULTRA_SIMPLE_UUIDS


namespace UltraSimpleUUID {
    const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // lookup table of characters
    const std::set<int> HYPHEN_POSITIONS {8, 13, 18, 23};                                       // The positions of the hyphens in a UUID string
    const unsigned short UUID_NUM_CHARS = 32;                                                   // Number of nonhyphen characters in UUID string
    const unsigned short UUID_STR_LEN = UUID_NUM_CHARS + HYPHEN_POSITIONS.size();               // Number of total characters in UUID string
    const unsigned short BUFFER_SIZE = 24;                                                      // Size of UUID buffer

    /*
    @brief: UUID object class. Constructs by default to a Nil UUID
    */
    class Uuid 
    {
    public:
        ///Useful UUID Funcitons


        ///@brief: Is this UUID Nil?
        bool isNil()
        {
            for (int32_t value : mValues)
            {
                if (value != 0)
                    return false;
            }
            return true;
        }


        ///@brief: Randomize this UUID. See readme section on randomness
        void randomize() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0, CHARS.size());
            std::vector<int32_t> randoms;
            for (int i = 0; i < UUID_NUM_CHARS; i++)
                randoms.push_back(floor(dist(mt)));

            for (int num_idx = 0; num_idx < UUID_NUM_CHARS; num_idx++)
            {
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
                assert((randoms[num_idx] & 63) == randoms[num_idx]);
#endif// DEEP_TEST_ULTRA_SIMPLE_UUIDS
                encodeIntAtPosition(randoms[num_idx], num_idx);
            };
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
            for (int num_idx = 0; num_idx < UUID_NUM_CHARS; num_idx++)
            {
                assert(randoms[num_idx] == extractIntFromPosition(num_idx));
            }
#endif //DEEP_TEST_ULTRA_SIMPLE_UUIDS
        }

        ///@brief: Combine a string into the UUID. This will deterministically change the UUID
        void combine(std::string inString)
        {
            std::string combine_string = inString;
            while (combine_string.length() < UUID_NUM_CHARS)
                combine_string += inString;
            int uuid_idx = 0;
            for (int combine_idx = 0; combine_idx < combine_string.length(); combine_idx++)
            {
                char c = combine_string[combine_idx];
                int32_t i = (extractIntFromPosition(uuid_idx) + (int32_t)c) % CHARS.length();
                encodeIntAtPosition(i, uuid_idx);
                uuid_idx++;
                uuid_idx = uuid_idx % UUID_NUM_CHARS;
            }
        }

        /// Conversion Functions

        ///@brief: Get the string version of this UUID
        std::string toString() {
            std::string uuid = std::string(UUID_STR_LEN, '-');
            int i = 0;
            for (int n = 0; n < UUID_NUM_CHARS; n++)
            {
                if (HYPHEN_POSITIONS.find(i) != HYPHEN_POSITIONS.end())
                    i++;
                uuid[i] = CHARS[extractIntFromPosition(n)];
                i++;
            }
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
            for (int hyphen_position : HYPHEN_POSITIONS)
            {
                assert(uuid[hyphen_position] == '-');
            }
#endif DEEP_TEST_ULTRA_SIMPLE_UUIDS
            return uuid;
        }

        
        ///@brief: Set UUID from a string. Returs false if string is malformed.
        bool fromString(std::string inString)
        {
            // check the string is well formed
            if (inString.length() != UUID_STR_LEN) { return false; }
            for (int hyphen_position : HYPHEN_POSITIONS)
            {
                if (inString[hyphen_position] != '-') { return false; }
            }
            int char_pos = 0;
            for (int i = 0; i < inString.length(); i++)
            {
                if (HYPHEN_POSITIONS.find(i) != HYPHEN_POSITIONS.end())
                    i++;
                std::size_t found = CHARS.find(inString[i]);
                if (found == std::string::npos) { return false; } // there's an illegal character in the string
                encodeIntAtPosition(found, char_pos);
                char_pos++;
            }
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
            assert(inString == toString());
#endif //DEEP_TEST_ULTRA_SIMPLE_UUIDS
            return true;
        }

        ///@brief: Get the buffer that represents this UUID
        unsigned short getBuffer(char* outBuffer[])
        {
            *outBuffer = (char*)&mValues;
            return BUFFER_SIZE;
        }

        ///@brief: Set this UUID from a buffer. Returns false if the buffer is invalid.
        bool fromBuffer(char inBuffer[], unsigned short inSize)
        {
            if (inSize != BUFFER_SIZE) { return false; }
            memcpy(mValues, inBuffer, BUFFER_SIZE);
            return true;
        }

        /// Operators

        ///@brief: equals operator
        bool operator==(const Uuid& inRhs)
        {
            for (int i = 0; i < 6; i++)
                if (mValues[i] != inRhs.mValues[i]) { return false; }
            return true;
        }

        ///@brief: inequal operator
        bool operator!=(const Uuid& inRhs)
        {
            return !operator==(inRhs);
        }

    private:
        int32_t mValues[6] = { 0,0,0,0,0,0 };                           // The actual bytes of the UUID

        ///@brief: Encode a a charater index into this UUID at a given position
        void encodeIntAtPosition(int32_t inNumber, int inPosition)
        {
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                mValues[bit_idx] &= ~(1 << inPosition); // Make sure the bit is zero
                mValues[bit_idx] |= ((inNumber >> bit_idx) & 1) << inPosition;
            }
        }

        ///@brief: extract a character index from the UUID at a given position
        int32_t extractIntFromPosition(int inPosition)
        {
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
            assert(inPosition < UUID_NUM_CHARS);
#endif //DEEP_TEST_ULTRA_SIMPLE_UUIDS
            int32_t out_value = 0;
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                out_value |= (((mValues[bit_idx] >> inPosition) & 1) << bit_idx);
            }
            return out_value;
        }
    };


    ///@brief: Useful utility function that generates a randomized UUID and returns it as a string
    std::string generate() {
        Uuid throwaway_uuid = Uuid();
        throwaway_uuid.randomize();
        return throwaway_uuid.toString();
    }
}