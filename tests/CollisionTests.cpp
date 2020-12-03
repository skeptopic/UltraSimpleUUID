#include "../UltraSimpleUUID.h"

#include <iostream>
#include <set>
#include <cassert>
#include <chrono>
#include <algorithm>


int main(int argc, char* argv[])
{
	std::set<std::string> uuids;
	for (int i = 0; i < 4096; i++)
	{
		uuids.insert(UltraSimpleUUID::generate());
	}
	
	std::chrono::time_point<std::chrono::system_clock> sec_countdown_time = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::system_clock::now();
	int num_tries = 0;
	int seconds_recorded = 0;
	int avg_tries_per_second = -1;

	// start a timer (or a counter) here
	UltraSimpleUUID::Uuid uuid;
	uuid.randomize();
	while (uuids.size() > 0)
	{
		if (uuids.count(uuid.toString()) > 0)
		{
			int seconds_elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time).count();
			uuids.erase(uuid.toString());
			std::cout << "Match found after " << seconds_elapsed << " seconds at a rate of " << avg_tries_per_second << " attempts per seond. " << uuids.size() << " UUIDs remaining" << std::endl;
		}
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - sec_countdown_time).count() >= 1)
		{
			sec_countdown_time = std::chrono::system_clock::now();
			avg_tries_per_second = avg_tries_per_second * seconds_recorded + num_tries;
			seconds_recorded++;
			avg_tries_per_second /= seconds_recorded;
			seconds_recorded = std::min(seconds_recorded, 256);
			num_tries = 0;
		}

		num_tries++;
		uuid.randomize();
	}

	// end a timer here
	std::cout << "Collision Found";
} 