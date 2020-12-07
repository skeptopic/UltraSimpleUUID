/*

Neil Chatterjee 2020

This simple test provides timing in microseconds for common UUID functions. I used this during development to seek performance gains.

*/
#include "../UltraSimpleUUID.h"
#include <iostream>
#include <map>
#include <chrono>
#include <vector>

const int TEST_ITERATION_COUNT = 4096 * 64;;

/*
@brief: A simple class to take multiple time measurements and retrieve the average time elapsed in microseconds
*/
class AverageTimer
{
public:
    void startTiming() { mStartTime = std::chrono::system_clock::now(); }       // Start timing an event

    void stopTiming() {                                                         // Stop timing an event
        mMeasurementDurationsMicroseconds.push_back(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now() - mStartTime
                ).count());
    }

    unsigned long long average()                                                // Retrieve the average
    {
        unsigned long long avg = 0;
        for (auto val : mMeasurementDurationsMicroseconds) { avg += val; }
        return avg / mMeasurementDurationsMicroseconds.size();
    }

protected:
    std::chrono::time_point<std::chrono::system_clock> mStartTime;              // Time last timer was started
    std::vector<unsigned long long> mMeasurementDurationsMicroseconds;          // All recorded durations
};


int main(int argc, char* argv[])
{    
    std::cout << "Testing Performance: ";
    AverageTimer constructor_timer;
    AverageTimer randomize_timer;
    AverageTimer to_string_timer;
    AverageTimer from_string_timer;
    AverageTimer set_from_binary;
    AverageTimer destructor_timer;
    std::string test_uuid = "usS5mo9r-MvTB-hxHb-xNVw-PDh9SgaLrH9f";
    int test_uuid_buffer[UltraSimpleUUID::BUFFER_SIZE] = { 47, 23, -67, 32, -24, 33, -97, 25, 106, -10, 2, -86, 63, 92, -3, 86, 123, -40, 45, -123, -121, 47, 60, 123 };
    char buffer[UltraSimpleUUID::BUFFER_SIZE] = { 0, };
    for (int i = 0; i < UltraSimpleUUID::BUFFER_SIZE; i++)
    {
        char c = test_uuid_buffer[i];
        buffer[i] = c;
    }

    UltraSimpleUUID::Uuid* id;
    int last_reported_percentage = -1;
    std::string last_reported;

    // Test the creation of random UUIDs, serialize to/from string
    for (int i = 0; i < TEST_ITERATION_COUNT; i++)
    {
        constructor_timer.startTiming();
        id = new UltraSimpleUUID::Uuid();
        constructor_timer.stopTiming();

        randomize_timer.startTiming();
        id->randomize();
        randomize_timer.stopTiming();

        to_string_timer.startTiming();
        id->toString();
        to_string_timer.stopTiming();

        from_string_timer.startTiming();
        id->fromString(test_uuid);
        from_string_timer.stopTiming();

        set_from_binary.startTiming();
        id->fromBuffer(buffer, UltraSimpleUUID::BUFFER_SIZE);
        set_from_binary.stopTiming();

        destructor_timer.startTiming();
        delete(id);
        destructor_timer.stopTiming();


        // Every time the percent progress goes up by a full int, report it.
        int current_percentage = (float)i * 100.0f / (float)TEST_ITERATION_COUNT;
        if (current_percentage > last_reported_percentage)
        {
            if (last_reported_percentage >= 0)
            {
                // delete last reported percentage
                for (auto c : last_reported)
                    std::cout << '\b';
            }
            last_reported = std::to_string(current_percentage);
            last_reported += "%";
            std::cout << last_reported;;
            last_reported_percentage = current_percentage;
        }
    }
    std::cout << std::endl << "Done! " << std::endl << std::endl;

    std::cout << "Number of Iterations: " << TEST_ITERATION_COUNT << std::endl;
    std::cout << "UUID construction (avg): " << constructor_timer.average() << " microseconds" << std::endl;
    std::cout << "UUID randomization (avg): " << randomize_timer.average() << " microseconds" << std::endl;
    std::cout << "UUID serialization to string (avg): " << to_string_timer.average() << " microseconds" << std::endl;
    std::cout << "UUID deserialization to string (avg): " << from_string_timer.average() << " microseconds" << std::endl;
    std::cout << "UUID set from binary buffer (avg): " << set_from_binary.average() << " microseconds" << std::endl;
    std::cout << "UUID destruction (avg): " << destructor_timer.average() << " microseconds" << std::endl;
}

