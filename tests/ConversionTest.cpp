#include <iostream>
#include "uuid.h"
#include <set>
#include <map>
#include <vector>
#include "cpp-instrumentation/Instrumentation.h"



int main(int argc, char* argv[])
{
    bool fast_mode = true;
    if (argc == 2)
    {
        std::string flag = argv[1];
        if (flag == "-complete")
        {
            fast_mode = false;
            std::cout << "Running in complete mode, will collide every UUID. This will run until you stop it\n";
        }
    }
    else
    {
        std::cout << "Running in fast mode, will stop at first collision\n";
    }


    std::map<std::string, Instrumentation::ElapsedTimer*> comparison_uuids;
    uint64_t num_uuids = 1024 * 64; // Near Theoretical limit...
    uint64_t num_collisions = 0;
    Instrumentation::Counter counter;
    Instrumentation::Countdown timer(5); // update every 5 seconds
    Instrumentation::MovingAverage average_collision_time(60); // 1 minute moving average
    Instrumentation::ElapsedTimer time_since_started;

    while (true)
    {
        
        if (comparison_uuids.size() < num_uuids)
        {
            while (comparison_uuids.size() < num_uuids)
            {
                std::string& comparison_uuid = SimpleUUID::generateUUID();
                while (comparison_uuids.find(comparison_uuid) != comparison_uuids.end())
                {
                    comparison_uuid = SimpleUUID::generateUUID();
               }
                comparison_uuids[comparison_uuid] = new Instrumentation::ElapsedTimer();
            }
        }
       
        std::string& uuid = SimpleUUID::generateUUID();
        counter.tick();
        if (comparison_uuids.find(uuid) != comparison_uuids.end())
        {
            float time = comparison_uuids.find(uuid)->second->GetElapsedSeconds()/60.0f;
            average_collision_time.push(time);
            num_collisions++;
            if (timer.isExpired())
            {
                std::cout << "Total Running Time " << time_since_started.GetElapsedSeconds() / 60.0f << " minutes\n";
                std::cout << "Collision in " << time << " minutes\n";
                std::cout << "Total Colliisons : " << num_collisions << "\n";
                std::cout << "Average Collision Time: " << average_collision_time.average() << " minutes.\n";
                std::cout << "Generation Rate: " << counter.rate() << " UUIDs/second\n\n";
                timer.reset();
            }
            delete(comparison_uuids[uuid]);
            comparison_uuids.erase(uuid);
        }
        if (fast_mode)
            break;

    }


    std::cout << "Done\n";
    
}