#include "../../TimeProfiler.h"
#include <iostream>
#include <thread>

int main() {

    std::cout << "start test" << std::endl;

    TIMEPROFILE_BEGIN(one); // about 1000 [ms]
    TIMEPROFILE_BEGIN(two); // about 3000 [ms]
    TIMEPROFILE_BEGIN(three); // about 6000 [ms]

    // create scope for test
    {
        SCOPED_TIMEPROFILE(all);

        {
            SCOPED_TIMEPROFILE(a);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            TIMEPROFILE_END(one);
        }
        {
            SCOPED_TIMEPROFILE(b);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            TIMEPROFILE_END(two);
        }
        {
            SCOPED_TIMEPROFILE(c);
            std::this_thread::sleep_for(std::chrono::seconds(3));
            TIMEPROFILE_END(three);
        }

        // "all" ends here
    }

    const auto& profiles = TimeProfiler.getProfiles();
    for (const auto& p : profiles) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
}
