
#pragma once
#include <Lumino/Base/NonCopyable.h>

namespace Lumino
{
class Application;

/**
	@brief		FPS (Frame Par Second) �̐���N���X
*/
class FpsController
    : public NonCopyable
{
public:
	FpsController();
	~FpsController();

public:
	
	/*
		@brief		���݂� FPS �l���擾���܂��B
	*/
	float GetFps() const { return (m_averageTime > 0) ? (1.0f / m_averageTime) : 0; }

	/*
		@brief		�O��̃t���[������o�߂����Q�[������ (�b) ���擾���܂��B
	*/
	float GetElapsedGameTime() const { return m_elapsedGameTime; }

	/*
		@brief		�O��̃t���[������o�߂��������� (�b) ���擾���܂��B
	*/
	float GetElapsedRealTime() const { return m_elapsedRealTime; }

	/*
		@brief		�J�n����̑��Q�[������ (�b) ���擾���܂��B
	*/
	float GetTotalGameTime() const { return 0.001f * m_totalGameTime; }

	/*
		@brief		�J�n����̑������� (�b) ���擾���܂��B
	*/
	float GetTotalRealTime() const { return 0.001f * m_totalRealTime; }

	/*
		@brief		�ő� FPS �l���擾���܂��B
		@details	�����ɂǂꂾ���]�T�����邩�������l���擾���܂��B
					���̒l���t���[�����[�g�̒l�����傫���قǁA�����ɗ]�T������܂��B<br>
					���̊֐��Œl���擾�ł���悤�ɂ���ɂ́ASetEnableFpsTest() �� true ��ݒ肵�Ă��������B
	*/
	float GetCapacityFps() const { return m_capacityFps; }

	/*
		@brief		�t���[�����[�g��ݒ肵�܂��B�����l�� 60 �ł��B
		@param[in]	frameRate	: �t���[�����[�g
	*/
	void SetFrameRate(int frameRate);

	/*
		@brief		�t���[�����[�g���擾���܂��B
	*/
    int GetFrameRate() const { return m_frameRate; }

	/*
		@brief		Fps �]�T�x�𑪒肷�邩��ݒ肵�܂��B�����l�� false �ł��B
	*/
	void SetEnableFpsTest(bool enabled) { m_enableFpsTest = enabled; }

	/*
		@brief		�t���[�����[�g���̃t���[�����o�߂����u�Ԃ𔻒肵�܂��B
		@details	�t���[�����[�g�� 60 �ł���΁A60 �t���[���� 1 �x�̊� true ��Ԃ��܂��B
	*/
	bool IsRoundFrame() const { return (m_frameCount == 0); }

	/*
		@brief		�x�����������Ă��邩���m�F���܂��B
		@details	���̊֐��� true ��Ԃ����ꍇ�A�O��̃t���[���� �t���[�����[�g�ȓ��̎��Ԃ�
					�������Ȃ��������Ƃ�\���܂��B
	*/
	bool IsFrameDelay() const { return (m_term <= 0); }

	/*
		@brief		�x�������Z�b�g���܂��B
		@details	���\�[�X�̃��[�h���Ŏ��Ԃ������蒷�����ԍX�V�������s���Ȃ������ꍇ�A
					�{������ׂ����Ԃɒǂ������Ƃ��Ă��΂炭�̊ԃm�[�E�F�C�g�Ńt���[���X�V���s���܂��B
					���̊Ԃ̓A�v���P�[�V���������ɍ����ɓ��삵�Ă���悤�Ɍ����Ă��܂��܂��B
					�����������邽�߁A���Ԃ̂����鏈���̒���ł��̊֐����ĂԂ��ƂŁAFPS ����ɒx�����������Ă��Ȃ����Ƃ�`���܂��B
	*/
	void RefreshSystemDelay();

private:	// internal
	friend class Application;
	void Process();
    void ProcessForMeasure();	// �E�F�C�g�͎�炸�A����̂ݍs�� (�c�[���p)

private:
	int			m_frameRate;		///< �t���[�����[�g
	float		m_frameRateRec;		///< �t���[�����[�g�̋t��
	float		m_fps;				///< ���݂� FPS �l
    
	float		m_currentTime;		///< ���݂̎��� ( �V�X�e���N������̎��� )
	float		m_averageTime;		///< 1 �t���[���̕��ώ���

	int		    m_frameCount;		///< fps �v���p�̌o�߃t���[���� ( 60fps�ɂ���ꍇ��0�`60�Ń��[�v )
	float		m_term;				///< wait() ���Ŏg���҂�����
	float		m_lastTime;			///< 1 ���O�̎��Ԃ��L������ϐ�
	float		m_baseTime;			///< 1 �b�ɂɈ�x����������
	float*		m_frameTimes;		///< �e�t���[���̎��Ԃ��i�[����z�� ( ���ς̌v�Z�Ɏg�� )

    float		m_elapsedGameTime;	///< �ЂƂO�̃t���[������̌o�߃Q�[������
    float		m_lastRealTime;		///< �ЂƂO�̃t���[���̎�����
    float		m_elapsedRealTime;	///< �ЂƂO�̃t���[������̌o�ߎ�����

    uint64_t	m_startTime;		///< �R���X�g���N�^�Őݒ肵���X�^�[�g����
    uint64_t	m_totalGameTime;	///< �o�߃Q�[�����Ԃ̍��v
    uint64_t	m_totalRealTime;	///< �o�ߎ����Ԃ̍��v

    float		m_capacityFps;
    float		m_capaFpsLastTime;
    float*		m_capaFrameTimes;	///< �e�t���[���̎��Ԃ��i�[����z�� ( ���ς̌v�Z�Ɏg�� )
    float		m_capaAverageTime;

    bool		m_enableFpsTest;		///< true �̏ꍇ�AFPS �e�X�g���s��
};

} // namespace Lumino
