#include "EngineVitals.h"

void EngineVitals::FrameFinished(const float& dTime)
{
    dTimes[topDT] = dTime;
    topDT++;
    if (topDT >= MAXDTIMES) topDT = 0;
    cumulativeDTime += dTime;
    if (cumulativeDTime > 1.0f && topDT > 0)
    {
        avgFT = 0;
        float minFT = INT32_MAX;
        float maxFT = 0;
        for (std::size_t i = 0; i < topDT; i++)
        {
            avgFT += dTimes[i];
            if (dTimes[i] < minFT)
            {
                minFT = dTimes[i];
            }
            if (dTimes[i] > maxFT)
            {
                maxFT = dTimes[i];
            }
        }
        avgFT /= topDT;
        avgFR = 1.0f / avgFT;
        topDT = 0;
        cumulativeDTime = 0;
        if (FrameFinishCallback)
        {
            FrameFinishCallback(avgFT, avgFR, minFT, maxFT);
        }
    }
}