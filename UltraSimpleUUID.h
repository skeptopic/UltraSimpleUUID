/*
Neil Chatterjee 2020

@TODO: Document the ever living hell out of this.
*/

// Neil Chatterjee 2020
// The worlds simplest and smallest "random" UUID string generator. This is meant to be dependency-free and easy to build and run on different platforms.
// There are certainly UUID generation methods that are more 'industrial strength' but I found no header-only solutions like this.
// 
// I took inspiration for this from https://gist.github.com/fernandomv3/46a6d7656f50ee8d39dc, but randomness was broken there.
// If you want better randomness, check out  https://github.com/mariusbancila/stduuid, but I had problems building this on Linux.
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
    const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::set<int> HYPHEN_POSITIONS {8, 13, 18, 23};
    const unsigned short UUID_NUM_CHARS = 32;
    const unsigned short UUID_STR_LEN = UUID_NUM_CHARS + HYPHEN_POSITIONS.size();
    const unsigned short BUFFER_SIZE = 24;

    class Uuid 
    {
    public:
        bool operator==(const Uuid& inRhs)
        {
            for (int i = 0; i < 6; i++)
                if (values[i] != inRhs.values[i]) { return false; }
            return true;
        }

        bool operator!=(const Uuid& inRhs)
        {
            return !operator==(inRhs);
        }

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

        unsigned short getBuffer(char *outBuffer[])
        {
            *outBuffer = (char*)&values;
            return BUFFER_SIZE;
        }

        bool fromBuffer(char inBuffer[], unsigned short inSize)
        {
            if (inSize != BUFFER_SIZE) { return false; }
            memcpy(values, inBuffer, BUFFER_SIZE);
            return true;
        }

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

        bool isNil()
        {
            for (int32_t value : values)
            {
                if (value != 0)
                    return false;
            }
            return true;
        }

        void combine(std::string inString)
        {
            // @TODO: consider if this can be this simplistic, or if we need an MD5 hash here
            // Ensure the string we're combining is long enough to effect all bits
            std::string combine_string = inString;
            while (combine_string.length() < UUID_NUM_CHARS)
                combine_string += inString;
            int uuid_idx = 0;
            for (int combine_idx = 0; combine_idx < combine_string.length(); combine_idx++)
            {
                char c = combine_string[combine_idx];
                int32_t i = (extractIntFromPosition(uuid_idx) + (int32_t)c)%CHARS.length();
                encodeIntAtPosition(i, uuid_idx);
                uuid_idx++;
                uuid_idx = uuid_idx % UUID_NUM_CHARS;
            }
        }

    private:
        int32_t values[6] = { 0,0,0,0,0,0 };

        void encodeIntAtPosition(int32_t inNumber, int inPosition)
        {
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                values[bit_idx] &= ~(1 << inPosition); // Make sure the bit is zero
                values[bit_idx] |= ((inNumber >> bit_idx) & 1) << inPosition;
            }
        }

        int32_t extractIntFromPosition(int inPosition)
        {
#ifdef DEEP_TEST_ULTRA_SIMPLE_UUIDS
            assert(inPosition < UUID_NUM_CHARS);
#endif //DEEP_TEST_ULTRA_SIMPLE_UUIDS
            int32_t out_value = 0;
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                out_value |= (((values[bit_idx] >> inPosition) & 1) << bit_idx);
            }
            return out_value;
        }
    };



    std::string generate() {
        Uuid throwaway_uuid = Uuid();
        throwaway_uuid.randomize();
        return throwaway_uuid.toString();
    }
}