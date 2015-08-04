
#pragma once
#include <memory>
#include <Lumino/Base/Array.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Mutex.h>

namespace Lumino
{

/**
	@brief
*/
class Profiler
{
public:
	static Profiler	Instance;		///< �O���[�o���C���X�^���X


	static int		Group_MainThread;
	static int		Group_RenderThread;

	static int		Section_MainThread_Update;
	static int		Section_MainThread_PrepareRendering;
	static int		Section_MainThread_GUILayput;
	static int		Section_RenderThread_CommandExecute;

	struct Section
	{
		String		Name;					///< �Z�N�V������
		uint64_t	ElapsedTime;			///< �o�ߎ��� (ns)
	};

	// �O���[�v�̓X���b�h����
	struct Group
	{
		String		Name;					///< �O���[�v��
		Array< std::shared_ptr<Section> >	Sections;
		//Section		Sections[MaxSections];
		ElapsedTimer	Timer;
		float			LimitElapsedTime;	///< ���e���� (ns �P�ʁB�t���[�����[�g�̋t��)
	};

	struct CommitedSection
	{
		String		Name;					///< �Z�N�V������
		uint64_t	ElapsedTime;			///< �o�ߎ��� (ns)
	};

	struct CommitedGroup
	{
		String					Name;		///< �O���[�v��
		Array<CommitedSection>	Sections;
		uint64_t				TotalTime;	///< CommitedSection �̃g�[�^�� ElapsedTime
		float					LimitElapsedTime;	///< ���e���� (ns �P�ʁB�t���[�����[�g�̋t��)
	};

public:
	Profiler();

public:
	/// �O���[�v���쐬����
	int RegisterGroup(const TCHAR* name);

	/// �Z�N�V�������쐬����
	int RegisterSection(int parentGroupIndex, const TCHAR* name);

	/// �O���[�v�̑����ƂȂ�t���[�����[�g��ݒ肷��
	void SetBaseFrameRate(int group, float baseFrameRate);

	/// �v���J�n (��Ԃ̃l�X�g�͂ł��Ȃ�)
	void StartSection(int groupIndex, int sectionIndex);

	/// �v���I��
	void EndSection(int groupIndex, int sectionIndex);

	/// �v���̗L���ݒ�
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	/// �v���̗L���ݒ�
	bool IsEnabled() const { return m_enabled; }

	/// ���C���X���b�h�� FPS
	void SetMainFPS(float fps) { m_mainFPS = fps; }

	/// ���C���X���b�h�� FPS �]�T�x
	void SetMainFPSCapacity(float fps) { m_mainFPSCapacity = fps; }

	/// ���C���E�B���h�E�̃T�C�Y
	void SetMainWindowSize(const Size& size) { m_mainWindowSize = size; }

	/// ���C���o�b�N�o�b�t�@�̃T�C�Y
	void SetMainBackBufferSize(const Size& size) { m_mainBackbufferSize = size; }


	void Commit();

	const Array<CommitedGroup>& GetCommitedGroups() const { return m_commitedGroups; }
	float GetCommitedMainFPS() const { return m_commitedMainFPS; }
	float GetCommitedMainFPSCapacity() const { return m_commitedMainFPSCapacity; }
	const Size&  GetCommitedMainWindowSize() const { return m_commitedMainWindowSize; }
	const Size&  GetCommitedMainBackbufferSize() const { return m_commitedMainBackbufferSize; }

private:
	Array< std::shared_ptr<Group> >			m_groups;
	float					m_mainFPS;
	float					m_mainFPSCapacity;
	Size					m_mainWindowSize;
	Size					m_mainBackbufferSize;

	Array<CommitedGroup>	m_commitedGroups;
	float					m_commitedMainFPS;
	float					m_commitedMainFPSCapacity;
	Size					m_commitedMainWindowSize;
	Size					m_commitedMainBackbufferSize;

	Threading::Mutex		m_commitMutex;
	bool					m_enabled;
};

class ScopedProfilerSection
{
public:
	ScopedProfilerSection(int group, int section)
		: m_group(group)
		, m_section(section)
	{
		Profiler::Instance.StartSection(m_group, m_section);
	}

	~ScopedProfilerSection()
	{
		Profiler::Instance.EndSection(m_group, m_section);
	}

private:
	int	m_group;
	int m_section;
};


} // namespace Lumino
