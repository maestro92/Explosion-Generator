#include "define.h"
#include "EG_TimeManager.h"
#include <algorithm>
EG_TimeManager::EG_TimeManager( float maxTimeStep /* = 0.03333f */ )
: m_fMaxTimeStep( maxTimeStep )
, m_fPrevious ( std::clock() / (float)CLOCKS_PER_SEC )
{}



float EG_TimeManager::GetElapsedTime() const
{
    float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
    float fDeltaTime = fCurrentTime - m_fPrevious;
    m_fPrevious = fCurrentTime;

    // Clamp to the max time step
    fDeltaTime = std::min( fDeltaTime, m_fMaxTimeStep );

    return fDeltaTime;
}


/// SDL_GetTicks - Use this function to get the number of milliseconds since the SDL library initialization.
void EG_TimeManager::setStartTime(int t)
{
    m_startTime = t;
}

long long EG_TimeManager::getStartTime()
{
    return m_startTime;
}

void EG_TimeManager::setRunningTime(float t)
{
    m_runningTime = t;
}


long long EG_TimeManager::getRunningTime()
{
    return m_runningTime;
}

# if 0
long long ElapsedTime::GetCurrentTimeMillis()
{
/*
#ifdef WIN32
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif
*/
    return 1;
}
#endif

