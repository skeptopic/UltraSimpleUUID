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

namespace SimpleUUID {
    const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string generateUUID() {
        std::string uuid = std::string(36, ' ');

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0, CHARS.size());
        std::vector<double> randoms;
        for (int i = 0; i < uuid.size(); ++i)
            randoms.push_back(dist(mt));

        for (int i = 0; i < uuid.size(); i++)
        {
            if (i == 8 || i == 13 || i == 18 || i == 23) // Sequence of 8-4-4-4-12
                uuid[i] = '-';
            else
                uuid[i] = CHARS[floor(randoms[i])];
        }
        return uuid;
    }
}