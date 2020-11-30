// Neil Chatterjee 2020
// The worlds simplest and smallest "random" UUID string generator. This is meant to be dependency-free and easy to build and run on different platforms.
// There are certainly UUID generation methods that are more 'industrial strength' but I found no header-only solutions like this.
// 
// I took inspiration for this from https://gist.github.com/fernandomv3/46a6d7656f50ee8d39dc, but randomness was broken there.
// If you want better randomness, check out  https://github.com/mariusbancila/stduuid, but I had problems building this on Linux.
//
#pragma once

#define DEBUG_UUIDS

#include <string>
#include <cstdlib>
#include <random>
#include <iostream>
#include <set>
#ifdef DEBUG_UUIDS
#include <cassert>
#endif //DEBUG_UUIDS

namespace UltraSimpleUUID {
    const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::set<int> HYPHEN_POSITIONS {8, 13, 18, 23};
    const unsigned short UUID_NUM_CHARS = 32;
    const unsigned short UUID_STR_LEN = UUID_NUM_CHARS + HYPHEN_POSITIONS.size();

    struct uuid 
    {
    public:
        int32_t values[6] = { 0,0,0,0,0,0 };

        std::string toString() {
            std::string uuid = std::string(UUID_STR_LEN, '-');
            int i = 0;
            for (int n = 0; n < UUID_NUM_CHARS; n++)
            {
                if (HYPHEN_POSITIONS.find(n) != HYPHEN_POSITIONS.end())
                    i++;
                uuid[i] = CHARS[extractNumberAtPosition(n)];
                i++;
            }
            return uuid;
        }

        void fromString(std::string inString)
        {
            assert(inString.length() == UUID_STR_LEN);
            int char_pos = 0;
            for (int i = 0; i < inString.length(); i++)
            {
                if (HYPHEN_POSITIONS.find(char_pos) != HYPHEN_POSITIONS.end())
                    continue;
                std::size_t found = CHARS.find(inString[i]);
                assert(found != std::string::npos);
                encodeIntAtPosition(found, i);
                char_pos++;
            }
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
#ifdef DEBUG_UUIDS
                assert((randoms[num_idx] & 63) == randoms[num_idx]);
#endif// DEBUG_UUIDS
                encodeIntAtPosition(randoms[num_idx], num_idx);
            };
#ifdef DEBUG_UUIDS
            for (int num_idx = 0; num_idx < UUID_NUM_CHARS; num_idx++)
            {
                assert(randoms[num_idx] == extractNumberAtPosition(num_idx));
            }
#endif //DEBUG_UUIDS
        }

    private:
        void encodeIntAtPosition(int32_t inNumber, int inPosition)
        {
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                values[bit_idx] &= ~(1 << inPosition); // Make sure the bit is zero
                values[bit_idx] |= ((inNumber >> bit_idx) & 1) << inPosition;
            }
        }

        int32_t extractNumberAtPosition(int inPosition)
        {
            assert(inPosition < UUID_NUM_CHARS);
            int32_t out_value = 0;
            for (int bit_idx = 0; bit_idx < 6; bit_idx++)
            {
                out_value |= (((values[bit_idx] >> inPosition) & 1) << bit_idx);
            }
            return out_value;
        }
    };

    std::string generate() {
        uuid throwaway_uuid = uuid();
        throwaway_uuid.randomize();
        return throwaway_uuid.toString();
    }
}