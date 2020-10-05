# TimeProfiler

Time profiler for Arduino


## Usage

``` C++
#include <TimeProfiler.h>

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("start test");

    TIMEPROFILE_BEGIN(one); // about 1000 [ms]
    TIMEPROFILE_BEGIN(two); // about 3000 [ms]
    TIMEPROFILE_BEGIN(three); // about 6000 [ms]

    // create scope for test
    {
        SCOPED_TIMEPROFILE(all);

        {
            SCOPED_TIMEPROFILE(a);
            delay(1000);
            TIMEPROFILE_END(one);
        }
        {
            SCOPED_TIMEPROFILE(b);
            delay(2000);
            TIMEPROFILE_END(two);
        }
        {
            SCOPED_TIMEPROFILE(c);
            delay(3000);
            TIMEPROFILE_END(three);
        }

        // "all" ends here
    }

    Serial.print("all   : ");
    Serial.println(TimeProfiler.getProfile("all"));
    Serial.print("one   : ");
    Serial.println(TimeProfiler.getProfile("one"));
    Serial.print("two   : ");
    Serial.println(TimeProfiler.getProfile("two"));
    Serial.print("three : ");
    Serial.println(TimeProfiler.getProfile("three"));
    Serial.print("a     : ");
    Serial.println(TimeProfiler.getProfile("a"));
    Serial.print("b     : ");
    Serial.println(TimeProfiler.getProfile("b"));
    Serial.print("c     : ");
    Serial.println(TimeProfiler.getProfile("c"));
}
```


## APIs

``` C++
const ProfileMap& getProfiles() const;
float getProfile(const StringType& name) const;
void clear();
```


## Embedded Libraries

- [ArxContainer v0.3.10](https://github.com/hideakitai/ArxContainer)


## License

MIT

