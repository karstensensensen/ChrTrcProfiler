#include "ChrTrcProfiler.h"
#include "ChrTrcMacros.h"

using namespace ChrTrcProfiler;
using namespace std::chrono_literals;

int main()
{
    CTProfiler::beginSession(sizeof(profiledFunction) * 4, "Profile.json");
    {
        CT_MEASURE_N("ALL");

        // A section
        {
            CT_MEASURE_C("A");

            std::this_thread::sleep_for(1s);

            CT_MEASURE_NC("MIDDLE", "A");

            std::this_thread::sleep_for(1s);

        }
        // B section
        {
            CT_MEASURE_C("B");

            std::this_thread::sleep_for(2s);

            // session is ended here, as the buffer is full at this point
        }
    }
    CTProfiler::endSession(); // technically no need for this
}