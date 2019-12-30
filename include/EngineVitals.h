#ifndef _H_ENGVIT_
#define _H_ENGVIT_
#include "cstdio"
#include <functional>
#define MAXDTIMES 10000
class EngineVitals
{
public:
    inline float GetSecondFrameTime(){return avgFT;}
    inline float GetSecondFrameRate(){return avgFR;}
    friend class Engine;
private:
    void FrameFinished(const float& dTime);
    std::function<void(const float&, const float&)> FrameFinishCallback;
    float dTimes[MAXDTIMES];
    float cumulativeDTime;
    std::size_t topDT = 0;
    float avgFT;
    float avgFR;
};
#endif