// Helper class to count frame time

#ifndef EG_TIME_MANAGER_H
#define EG_TIME_MANAGER_H

#include "SDL/SDL.h"

#define FRAME_VALUES 10

class EG_TimeManager
{
    public:
        EG_TimeManager( float maxTimeStep = 0.03333f );
        float GetElapsedTime() const;
   //     long long GetCurrentTimeMillis();

        void setStartTime(int t);
        void setRunningTime(float t);
        long long getStartTime();
        long long getRunningTime();

        void addTick(unsigned tick);
        float computeAverageFPS();


        float m_fps;
        float m_curTick;
        float m_prevTick;
        float m_iterRefreshRate;
        float m_curIter;
        unsigned int m_frameCount;
        unsigned int m_frameTicks[FRAME_VALUES];
        unsigned int m_frameTicksIndex;
        unsigned int m_prevFrameTick;


    private:
        float m_fMaxTimeStep;
        mutable float m_fPrevious;

        /// both are in milli seconds
        long long m_startTime;
        long long m_runningTime;





};

#endif
