
#include "../Internal.h"
#include <Lumino/Threading/Thread.h>
#include <Lumino/Game/FpsController.h>

namespace Lumino
{

/*
�ҋ@�������s�������ꍇ�͏�������Aprocess() ���Ăяo��������
//              �R���X�g���N�^�ɓn���ꂽ�t���[�����[�g�ɂȂ�悤�ɑҋ@���܂��B
//
//  @note
//              �����̃m�[�g PC �ŁA�Ȃ񂩓��������ɂ������Ȃ��Ǝv����
//              1�t���[���̌o�ߎ��Ԃ𒲂ׂ���A�s�����1�t���[��70ms������
//              ���̂��������B(�ق�Ƃɕs����B1�b(60���0�񂾂�����4�񂾂�����B)
//              ���̃t���[���ł͂ق�0.016�L�[�v)
//              �ڂ������ׂĂ݂�ƁAwinAPI �� Sleep �����܂ɂ��������Ԃ������Ă��B
//              Sleep( 16 ) �őҋ@���Ă�̂ɁA�������Ɠ����� 70ms �������Ă��Ƃ��B
//              Sleep() �͊m���Ɏw�肵�����Ԃ����҂��Ă����֐�����Ȃ�
//              ���Ƃ͂킩���Ă����ǁA���ɂ���Ă����܂ŕς��Ƃ�
//              (�f�X�N�g�b�v�̕��ł͖��Ȃ�����)�����v��Ȃ������̂ŁA
//              �������㎗���悤�Ȃ��Ƃ��������ꍇ�͂��̕ӂ��Q�l�ɂ��Ă݂�B
*/
//==============================================================================
// FpsController
//==============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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
	, m_startTime(Environment::GetTickCount())
	, m_totalGameTime(0)
	, m_totalRealTime(0)
	, m_capacityFps(0.0)
	, m_capaFpsLastTime(0.0)
	, m_capaFrameTimes(NULL)
	, m_capaAverageTime(0.0)
	, m_enableFpsTest(false)
{
	SetFrameRate(60);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FpsController::~FpsController()
{
	LN_SAFE_DELETE_ARRAY(m_frameTimes);
	LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::SetFrameRate(int frameRate)
{
	LN_VERIFY(frameRate > 0) { return; }

	m_frameRate = frameRate;
	m_frameRateRec = 1.0f / m_frameRate;
	m_frameCount = 0;

	LN_SAFE_DELETE_ARRAY(m_frameTimes);
	m_frameTimes = LN_NEW float[m_frameRate];

	LN_SAFE_DELETE_ARRAY(m_capaFrameTimes);
	m_capaFrameTimes = LN_NEW float[m_frameRate];

	m_term = 0.0f;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::RefreshSystemDelay()
{
	uint64_t currentTime = Environment::GetTickCount();
	m_lastTime = m_baseTime = m_capaFpsLastTime = (0.001f * (currentTime - m_startTime));// + m_frameRateRec;
	m_baseTime -= 1.0f - m_frameRateRec;

	// ���݂� m_frameCount �܂ł̃t���[�������畽�ώ��Ԃ��v�Z����
	m_averageTime = 0;
	for (int i = 0; i < m_frameCount; ++i)
	{
		m_averageTime += m_frameTimes[i];
	}
	m_averageTime /= m_frameCount;

	m_frameCount = 0;
	m_term = 0.0f;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::Process()
{
	//if (!mEnableFrameWait) {
	//	processForMeasure();
	//	return;
	//}

	m_currentTime = 0.001f * (Environment::GetTickCount() - m_startTime);

	m_elapsedGameTime = m_currentTime - m_lastTime;
	m_elapsedRealTime = m_currentTime - m_lastRealTime;

	m_totalGameTime += static_cast< uint32_t >(1000.0f * m_elapsedGameTime);
	m_totalRealTime += static_cast< uint32_t >(1000.0f * m_elapsedRealTime);

	// (frame_rate_)�t���[����1��ڂȂ�
	if (m_frameCount == 0)
	{
		// ���S�ɍŏ�
		if (m_lastTime == 0.0)
		{
			m_term = m_frameRateRec;
		}
		// �O��L�^�������Ԃ����Ɍv�Z
		else
		{
			m_term = m_baseTime + 1.0f - m_currentTime;
		}

		//printf( "f: a:%f bt:%f rc:%f ct:%f lt:%f\n", m_term, m_baseTime, m_frameRateRec * m_frameCount, m_currentTime, m_currentTime - m_lastRealTime );
	}
	else
	{
		// ����҂ׂ����� = ���݂���ׂ����� - ���݂̎���
		m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;

		//if ( m_term < 0 )
		//{
		//    printf( "c:%f\n", m_term );
		//}
		//printf( "s: a:%f bt:%f rc:%f ct:%f lt:%f\n", m_term, m_baseTime, m_frameRateRec * m_frameCount, m_currentTime, m_currentTime - m_lastRealTime );
	}

	//printf( "frame:%d term:%f etime:%f\n", m_frameCount, m_term, m_elapsedRealTime );
	//printf( "etime:%f term:%f \n", m_elapsedRealTime, m_term );
	//static int ii = 0;
	//++ii;

	// �҂ׂ����Ԃ����҂�
	if (m_term > 0.0)
	{
		//printf( "t:%lf c:%d\n", m_term, m_frameCount );


		//if ( ii > 120 && m_term < 0.016 )
		{
			/*
			ii = 120;

			double bb = 0;
			for ( int i = 0; i < 60; ++i )
			{
			_p( m_frameTimes[ i ] );
			bb += m_frameTimes[ i ];
			}
			_p((double)ii / 60.0);

			printf( "a\n" );
			*/
		}

		//::Sleep( 16 );

		Threading::Thread::Sleep(static_cast< uint32_t >(m_term * 1000));
	}

	// ���݂̎���
	float nt = m_currentTime;

	// m_frameRate �t���[����1�x������
	if (m_frameCount == 0)
	{
		// + m_frameRateRec ��1�t���[�����炳�Ȃ��ƁA
		// 60 �t���[������ 1 �t���[���ڂő҂����Ԃ̌v�Z������Ƃ���
		// m_currentTime �̎��Ԃ����݂���ׂ������𒴂��Ă��܂��āA
		// �҂����Ԃ� -0.0003 �Ƃ��ɂȂ��Ă��܂��B
		// ���ʁA��u��������Ƃ�����������邱�Ƃ��������B
		m_baseTime = nt + m_frameRateRec;
	}

	// ����̕���1���������Ԃ��L�^
	m_frameTimes[m_frameCount] = nt - m_lastTime;

	// ���݂̎��Ԃ��A�ЂƂO�̎��ԂƂ��ċL��
	m_lastRealTime = nt;
	m_lastTime = nt;

	// m_frameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
	if (m_frameCount == m_frameRate - 1)
	{
		m_averageTime = 0;
		for (int i = 0; i < m_frameRate; ++i)
		{
			m_averageTime += m_frameTimes[i];
		}
		m_averageTime /= m_frameRate;
	}

	// FPS �]�T�x�𑪒肷��
	if (m_enableFpsTest)
	{
		// ����̕���1���������Ԃ��L�^
		m_capaFrameTimes[m_frameCount] = m_currentTime - m_capaFpsLastTime;

		// m_frameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
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
		}

		m_capaFpsLastTime = m_currentTime;//0.001f * (Environment::getSystemTime() - m_startTime);
	}

	// m_frameCount �� frame �ň������悤�ɂ���
	m_frameCount = (++m_frameCount) % m_frameRate;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void FpsController::ProcessForMeasure()
{
	m_currentTime = 0.001f * (Environment::GetTickCount() - m_startTime);

	m_elapsedGameTime = m_currentTime - m_lastTime;
	m_elapsedRealTime = m_currentTime - m_lastRealTime;

	m_totalGameTime += static_cast< uint32_t >(1000.0f * m_elapsedGameTime);
	m_totalRealTime += static_cast< uint32_t >(1000.0f * m_elapsedRealTime);

	// (frame_rate_)�t���[����1��ڂȂ�
	if (m_frameCount == 0)
	{
		// ���S�ɍŏ�
		if (m_lastTime == 0.0)
		{
			m_term = m_frameRateRec;
		}
		// �O��L�^�������Ԃ����Ɍv�Z
		else
		{
			m_term = m_baseTime + 1.0f - m_currentTime;
		}
	}
	else
	{
		// ����҂ׂ����� = ���݂���ׂ����� - ���݂̎���
		m_term = (m_baseTime + m_frameRateRec * m_frameCount) - m_currentTime;
	}

	static int ii = 0;

	++ii;

	// �҂ׂ����Ԃ����҂�
	if (m_term > 0.0)
	{
		//Threading::sleep( static_cast< uint32_t >( m_term * 1000 ) );
	}

	// ���݂̎���
	float nt = m_currentTime;

	// m_frameRate �t���[����1�x������
	if (m_frameCount == 0)
	{
		// + m_frameRateRec ��1�t���[�����炳�Ȃ��ƁA
		// 60 �t���[������ 1 �t���[���ڂő҂����Ԃ̌v�Z������Ƃ���
		// m_currentTime �̎��Ԃ����݂���ׂ������𒴂��Ă��܂��āA
		// �҂����Ԃ� -0.0003 �Ƃ��ɂȂ��Ă��܂��B
		// ���ʁA��u��������Ƃ�����������邱�Ƃ��������B
		m_baseTime = nt + m_frameRateRec;
	}

	// ����̕���1���������Ԃ��L�^
	m_frameTimes[m_frameCount] = nt - m_lastTime;

	// ���݂̎��Ԃ��A�ЂƂO�̎��ԂƂ��ċL��
	m_lastRealTime = nt;
	m_lastTime = nt;


	// m_frameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
	if (m_frameCount == m_frameRate - 1)
	{
		m_averageTime = 0;
		for (int i = 0; i < m_frameRate; ++i)
		{
			m_averageTime += m_frameTimes[i];
		}
		m_averageTime /= m_frameRate;
	}

	if (m_enableFpsTest)
	{
		// ����̕���1���������Ԃ��L�^
		m_capaFrameTimes[m_frameCount] = m_currentTime - m_capaFpsLastTime;

		// m_frameRate �̍Ō�̃t���[�� ( 60 �̏ꍇ�� 59 �t���[���� ) �ɕ��ς��v�Z
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
		}

		m_capaFpsLastTime = m_currentTime;
	}

	// m_frameCount �� frame �ň������悤�ɂ���
	m_frameCount = (++m_frameCount) % m_frameRate;
}

} // namespace Lumino
