
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
	};

public:
	Profiler();

public:
	/// �O���[�v���쐬����
	int RegisterGroup(const TCHAR* name);

	/// �Z�N�V�������쐬����
	int RegisterSection(int parentGroupIndex, const TCHAR* name);

	/// �v���J�n (��Ԃ̃l�X�g�͂ł��Ȃ�)
	void StartSection(int groupIndex, int sectionIndex);

	/// �v���I��
	void EndSection(int groupIndex, int sectionIndex);

	/// �v���̗L���ݒ�
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	/// �v���̗L���ݒ�
	bool IsEnabled() const { return m_enabled; }

	void Commit();

	const Array<CommitedGroup>& GetCommitedGroups() const { return m_commitedGroups; }

private:
	Array< std::shared_ptr<Group> >			m_groups;
	Array<CommitedGroup>	m_commitedGroups;
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
