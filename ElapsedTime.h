// Helper class to count frame time
class ElapsedTime
{
    public:
        ElapsedTime( float maxTimeStep = 0.03333f );
        float GetElapsedTime() const;
        long long GetCurrentTimeMillis();



    private:
        float m_fMaxTimeStep;
        mutable float m_fPrevious;
};
