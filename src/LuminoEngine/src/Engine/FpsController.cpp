﻿
#include "Internal.hpp"
#include "FpsController.hpp"

namespace ln {
namespace detail {

//=============================================================================
// FpsController

FpsController::FpsController()
    : m_frameRate(0)
    , m_frameRateRec(0.0)
    , m_fps(0.0)
    , m_currentTime(0.0)
    , m_averageTime(0.0)
    , m_frameCount(0)
    , m_term(0.0)
    , m_lastTime(0.0)
    , m_baseTime(0.0)
    , m_frameTimes(NULL)
    , m_elapsedGameTime(0.0)
    , m_lastRealTime(0.0)
    , m_elapsedRealTime(0.0)
    , m_startTime(0.001 * static_cast<double>(Environment::getTickCount()))
    , m_totalGameTime(0)
    , m_totalRealTime(0)
    , m_capacityFps(0.0)
    , m_capaFpsLastTime(0.0)
    , m_capaFrameTimes(NULL)
    , m_capaAverageTime(0.0)
	, m_minTime(FLT_MAX)
	, m_maxTime(FLT_MIN)
	, m_minTimePerSeconds(0.0f)
	, m_maxTimePerSeconds(0.0f)
    , m_enableFpsTest(false)
{
	m_capaFpsLastTime = m_startTime;
    setFrameRate(60);
}

FpsController::~FpsController()
{
    LN_SAFE_DELETE_ARRAY(m_frameTimes);
    LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
}

void FpsController::setFrameRate(int frameRate)
{
    if (LN_REQUIRE(frameRate >= 0)) return;

    m_frameRate = frameRate;
    m_frameRateRec = 1.0f / m_frameRate;
    m_frameCount = 0;

    LN_SAFE_DELETE_ARRAY(m_frameTimes);
    m_frameTimes = LN_NEW float[m_frameRate];

    LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
    m_capaFrameTimes = LN_NEW float[m_frameRate];

    m_term = 0.0f;
}

void FpsController::refreshSystemDelay()
{
    uint64_t currentTime = Environment::getTickCount();
    m_lastTime = m_baseTime = m_capaFpsLastTime = (0.001f * (currentTime - m_startTime));// + m_frameRateRec;
    m_baseTime -= 1.0f - m_frameRateRec;

    // 現在の m_frameCount までのフレーム数から平均時間を計算する
    m_averageTime = 0;
    for (int i = 0; i < m_frameCount; ++i)
    {
        m_averageTime += m_frameTimes[i];
    }
    m_averageTime /= m_frameCount;

    m_frameCount = 0;
    m_term = 0.0f;
}

void FpsController::process()
{
    std::cout << "process(" << m_frameCount << ")" << std::endl;
    //if (!mEnableFrameWait) {
    //    processForMeasure();
    //    return;
    //}
    m_currentTime = 0.001f * (Environment::getTickCount() - m_startTime);

    std::cout << m_currentTime << std::endl;

    m_elapsedGameTime = m_currentTime - m_lastTime;
    m_elapsedRealTime = m_currentTime - m_lastRealTime;

    addingToTotalTime();


    // (frame_rate_)フレームの1回目なら
    if (m_frameCount == 0)
    {
        // 完全に最初
        if (m_lastTime == 0.0)
        {
            m_term = m_frameRateRec;
        }
        // 前回記録した時間を元に計算
        else
        {
            m_term = m_baseTime + 1.0f - m_currentTime;
        }
    }
    else
    {
        // 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
        m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;
    }

    std::cout << m_term << std::endl;

    // 待つべき時間だけ待つ
    if (m_term > 0.0)
    {
        Thread::sleep(static_cast< uint32_t >(m_term * 1000));
    }

    // 現在の時間
    float nt = m_currentTime;

    // m_frameRate フレームに1度基準を作る
    if (m_frameCount == 0)
    {
        // + m_frameRateRec で1フレームずらさないと、
        // 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
        // m_currentTime の時間が現在あるべき時刻を超えてしまって、
        // 待ち時間が -0.0003 とかになってしまう。
        // 結果、一瞬だけｶｸｯという動作をすることがあった。
        m_baseTime = nt + m_frameRateRec;
    }

    // 今回の分の1周した時間を記録
    m_frameTimes[m_frameCount] = nt - m_lastTime;

    // 現在の時間を、ひとつ前の時間として記憶
    m_lastRealTime = nt;
    m_lastTime = nt;

    // m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
    if (m_frameCount == m_frameRate - 1)
    {
        m_averageTime = 0;
        for (int i = 0; i < m_frameRate; ++i)
        {
            m_averageTime += m_frameTimes[i];
        }
        m_averageTime /= m_frameRate;
    }

	measureTimes(false);

    // m_frameCount を frame で一周するようにする
    m_frameCount = (++m_frameCount) % m_frameRate;

    m_fps = (m_averageTime > 0) ? (1.0f / m_averageTime) : 0;
}

void FpsController::processForMeasure()
{
    uint64_t tick = Environment::getTickCount();
    if (tick < m_startTime) {
        LN_NOTIMPLEMENTED();
        return;
    }

    m_currentTime = (0.001 * static_cast<double>(tick)) - m_startTime;

    m_elapsedGameTime = m_currentTime - m_lastTime;
    m_elapsedRealTime = m_currentTime - m_lastRealTime;

    addingToTotalTime();

    // (frame_rate_)フレームの1回目なら
    if (m_frameCount == 0)
    {
        // 完全に最初
        if (m_lastTime == 0.0)
        {
            m_term = m_frameRateRec;
        }
        // 前回記録した時間を元に計算
        else
        {
            m_term = m_baseTime + 1.0f - m_currentTime;
        }
    }
    else
    {
        // 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
        m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;
    }

    // 現在の時間
    float nt = m_currentTime;

    // m_frameRate フレームに1度基準を作る
    if (m_frameCount == 0)
    {
        // + m_frameRateRec で1フレームずらさないと、
        // 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
        // m_currentTime の時間が現在あるべき時刻を超えてしまって、
        // 待ち時間が -0.0003 とかになってしまう。
        // 結果、一瞬だけｶｸｯという動作をすることがあった。
        m_baseTime = nt + m_frameRateRec;
    }

    // 今回の分の1周した時間を記録
    m_frameTimes[m_frameCount] = nt - m_lastTime;

    // 現在の時間を、ひとつ前の時間として記憶
    m_lastRealTime = nt;
    m_lastTime = nt;


    // m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
    if (m_frameCount == m_frameRate - 1)
    {
        m_averageTime = 0;
        for (int i = 0; i < m_frameRate; ++i)
        {
            m_averageTime += m_frameTimes[i];
        }
        m_averageTime /= m_frameRate;
    }

	measureTimes(true);

    // m_frameCount を frame で一周するようにする
    m_frameCount = (++m_frameCount) % m_frameRate;

    m_fps = (m_averageTime > 0) ? (1.0f / m_averageTime) : 0;
}

void FpsController::addingToTotalTime()
{
    m_totalGameTime += static_cast< uint32_t >(1000.0f * m_elapsedGameTime);
    m_totalRealTime += static_cast< uint32_t >(1000.0f * m_elapsedRealTime);
}

void FpsController::measureTimes(bool fromProcessForMeasure)
{
	if (m_enableFpsTest)
	{
		// 今回の分の1周した時間を記録
		float elapsedTime = m_currentTime - m_capaFpsLastTime;
		m_capaFrameTimes[m_frameCount] = elapsedTime;

		m_minTime = std::max(std::min(m_minTime, elapsedTime), 0.0f);
		m_maxTime = std::max(m_maxTime, elapsedTime);

		// m_frameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		if (m_frameCount == m_frameRate - 1)
		{
			m_capaAverageTime = 0;
			for (int i = 0; i < m_frameRate; ++i)
			{
				m_capaAverageTime += m_capaFrameTimes[i];
			}

			if (m_capaAverageTime == 0.0f)
			{
				m_capaAverageTime = 0.01f;
			}
			m_capaAverageTime /= m_frameRate;

			m_capacityFps = 1.0f / m_capaAverageTime;
			if (m_capacityFps > 100 * m_frameRate)
			{
				m_capacityFps = 100.0f * m_frameRate;
			}

			// get snapshot
			m_minTimePerSeconds = m_minTime;
			m_maxTimePerSeconds = m_maxTime;
			m_minTime = FLT_MAX;
			m_maxTime = FLT_MIN;
		}

		if (fromProcessForMeasure)
			m_capaFpsLastTime = m_currentTime;
		else
			m_capaFpsLastTime = 0.001f * (Environment::getTickCount() - m_startTime);
	}
}


//=============================================================================
// FpsController2

FpsController2::FpsController2()
    : m_frameRate(0)
    , m_frameCount(0)
    //, m_lastTime(0)
    , m_measureTimes(true)
    , m_frameTimes()
    , m_externalTimes()
    , m_totalFps(0.f)
    , m_externalFps(0.f)
    , m_averageTime(0)
    , m_externalAverageTime(0)
    , m_minTime(0)
    , m_maxTime(0)
    , m_minTimePerSeconds(0)
    , m_maxTimePerSeconds(0)
{
    setFrameRate(60);
    //m_lastTime = 0;
    m_timer.start();
}

void FpsController2::setFrameRate(int value)
{
    m_frameRate = value;
    m_frameTime = 1000 / m_frameRate;
    m_frameTimes.resize(m_frameRate);
    m_externalTimes.resize(m_frameRate);
}

void FpsController2::process()
{
    uint64_t externalElapsedTime = m_timer.elapsedMilliseconds();
    m_currentGameTime += externalElapsedTime;

    // for overflow.
    //if (m_currentGameTime < m_lastTime) {
    //    m_lastTime = m_currentGameTime;
    //}
    
    // 前回の process() を抜けたときから、今回の process が呼ばれたこの行までの経過時間
    // (= 1フレームにどれだけ時間がかかったか)

    // Make base-time in first frame of second.
    if (m_frameCount == 0) {
        m_baseTime = m_currentGameTime;
    }

    int64_t term = 0;
    //if (m_frameCount == 0)
    {
        term = m_frameTime - externalElapsedTime;// (m_currentGameTime - m_lastTime);
    }
    term--;
    //else {
    //    // 今回待つべき時間 = 次のフレームのあるべき時刻 - 現在の時刻
    //    uint64_t nextFrameTime = (m_baseTime + m_frameTime * (m_frameCount + 1));
    //    term = nextFrameTime - m_currentGameTime;
    //}

    //if (term > 17) {
    //    std::cout << term << std::endl;
    //}

    if (term > 0) 
    {
        ElapsedTimer t;
        t.start();
        Thread::sleep(term);
        //std::cout << externalElapsedTime << ":" << term << ":" << t.elapsedMilliseconds() << ", ext:" << g_extt.elapsedMilliseconds() << std::endl;
    }
    std::cout << externalElapsedTime << ":" << term << std::endl;

    //std::cout << term << std::endl;

    // 待ち時間も考慮した、このフレームの締め
    //uint64_t postTime = 
    uint64_t frameElapsedTime = m_timer.elapsedMilliseconds(); ;// postTime;// -m_lastTime;

    //std::cout << "post time: " << Environment::getTickCount() << std::endl;

    //m_lastTime = postTime;


    measureTimes(externalElapsedTime, frameElapsedTime);

    m_frameCount = (++m_frameCount) % m_frameRate;

    m_timer.start();
}

void FpsController2::processForMeasure()
{
    uint64_t externalElapsedTime = m_timer.elapsedMilliseconds();
    m_currentGameTime += externalElapsedTime;

    //if (m_currentGameTime < m_lastTime) {
    //    m_lastTime = m_currentGameTime;
    //}

    //uint64_t externalElapsedTime = m_currentGameTime - m_lastTime;

    measureTimes(externalElapsedTime, externalElapsedTime);

    m_frameCount = (++m_frameCount) % m_frameRate;

    m_timer.start();
}

void FpsController2::refreshSystemDelay()
{
    m_timer.start();
    m_frameCount = 0;
}

void FpsController2::measureTimes(uint64_t externalElapsedTime, uint64_t frameElapsedTime)
{
    if (m_measureTimes) {

        m_externalTimes[m_frameCount] = externalElapsedTime;
        m_frameTimes[m_frameCount] = frameElapsedTime;

        m_minTime = std::max(std::min(m_minTime, frameElapsedTime), 0ULL);
        m_maxTime = std::max(m_maxTime, frameElapsedTime);

        if (m_frameCount == m_frameTimes.size() - 1) {

            // calc average time (internal time)
            {
                uint64_t total = 0;
                for (auto t : m_frameTimes) {
                    total += t;
                }
                m_averageTime = total / m_frameRate;

                m_totalFps = (m_averageTime > 0) ? (1000.f / m_averageTime) : 0.f;
            }

            // calc average time (external time)
            {
                uint64_t total = 0;
                for (auto t : m_externalTimes) {
                    total += t;
                }
                m_externalAverageTime = total / m_frameRate;

                m_externalFps = (m_externalAverageTime > 0) ? (1000.f / m_externalAverageTime) : 0.f;
            }

            // get snapshot
            m_minTimePerSeconds = m_minTime;
            m_maxTimePerSeconds = m_maxTime;
            m_minTime = FLT_MAX;
            m_maxTime = FLT_MIN;
        }
    }
}

} // namespace detail
} // namespace ln

