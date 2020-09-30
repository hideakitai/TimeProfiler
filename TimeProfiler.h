#pragma once
#ifndef HT_UTIL_TIMEPROFILER_H
#define HT_UTIL_TIMEPROFILER_H

#ifdef ARDUINO
    #include <Arduino.h>
    #include "util/ArxContainer/ArxContainer.h"
#else
    #include <map>
    #include <string>
    #include <chrono>
#endif

namespace ht {
namespace util {
namespace time {

#ifdef ARDUINO
    using StringType = String;
    #if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L // Have libstdc++11
        using ProfileData = std::pair<StringType, float>;
        using ProfileMap = std::map<StringType, float>;
        using namespace std;
    #else
        using ProfileData = arx::pair<StringType, float>;
        using ProfileMap = arx::map<StringType, float>;
        using namespace arx;
    #endif
    using TimePoint = uint32_t;
    using MicroSeconds = uint32_t;
    #define TIMEPROFILER_CURRENT_TIMEPOINT() micros()
    #define TIMEPROFILER_GET_COUNT(t) t
    #define TIMEPROFILER_TIMEPOINT_TO_MICROSECONDS(t) t
#else
    using StringType = std::string;
    using ProfileData = std::pair<StringType, float>;
    using ProfileMap = std::map<StringType, float>;
    using TimePoint = std::chrono::system_clock::time_point;
    using MicroSeconds = std::chrono::microseconds;
    #define TIMEPROFILER_CURRENT_TIMEPOINT() std::chrono::system_clock::now()
    #define TIMEPROFILER_GET_COUNT(t) t.count()
    #define TIMEPROFILER_TIMEPOINT_TO_MICROSECONDS(t) std::chrono::duration_cast<std::chrono::microseconds>(t)
    using namespace std;
#endif

    class Profiler
    {
        Profiler(){}
        Profiler(const Profiler&) = delete;
        Profiler& operator=(const Profiler&) = delete;

        ProfileMap profiles;

    public:

        static Profiler& get()
        {
            static Profiler p;
            return p;
        }

        void add(const StringType& name, const float ms)
        {
            auto it = profiles.find(name);
            if (it != profiles.end())
                profiles[name] = ms;
            else
                profiles.emplace(make_pair(name, ms));
        }

        const ProfileMap& getProfiles() const
        {
            return profiles;
        }

        float getProfile(const StringType& name) const
        {
            if (profiles.find(name) != profiles.end())
                return profiles.at(name);
            else
                return 0.f;
        }

        void clear()
        {
            profiles.clear();
        }
    };

    class Profile
    {
        const StringType name;
        TimePoint origin;

    public:

        Profile(const StringType& name)
        : name(name)
        , origin(TIMEPROFILER_CURRENT_TIMEPOINT())
        {
        }

        void end()
        {
            MicroSeconds us = TIMEPROFILER_TIMEPOINT_TO_MICROSECONDS(TIMEPROFILER_CURRENT_TIMEPOINT() - origin);
            Profiler::get().add(name, TIMEPROFILER_GET_COUNT(us) * 0.001);
        }
    };

    class ScopedProfile
    {
        const StringType name;
        TimePoint origin;

    public:

        ScopedProfile(const StringType& name)
        : name(name)
        , origin(TIMEPROFILER_CURRENT_TIMEPOINT())
        {
        }

        ~ScopedProfile()
        {
            MicroSeconds us = TIMEPROFILER_TIMEPOINT_TO_MICROSECONDS(TIMEPROFILER_CURRENT_TIMEPOINT() - origin);
            Profiler::get().add(name, TIMEPROFILER_GET_COUNT(us) * 0.001);
        }
    };

} // time
} // util
} // ht

#define SCOPED_TIMEPROFILE(A) ht::util::time::ScopedProfile stp(#A)

#define TIMEPROFILE_BEGIN(A) ht::util::time::Profile A(#A)
#define TIMEPROFILE_END(A) A.end()

#define TimeProfiler ht::util::time::Profiler::get()

#endif // HT_UTIL_TIMEPROFILER_H
