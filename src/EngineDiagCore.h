
#pragma once

LN_NAMESPACE_BEGIN

class EngineDiagCore
{
public:
	typedef int SectionId;

	struct Section
	{
		String		name;			// �Z�N�V������
		uint64_t	elapsedTime;	// �o�ߎ��� (ns)
	};

	// �O���[�v�̓X���b�h����
	struct SectionGroup
	{
		String							name;		// �O���[�v��
		SortedArray<String, Section>	sections;
		ElapsedTimer					timer;
		//float							limitElapsedTime;	// ���e���� (ns �P�ʁB�t���[�����[�g�̋t��)
	};

public:
	static EngineDiagCore	Instance;		// �O���[�o���C���X�^���X

	void Initialize(EngineManager* manager);

	float GetMainFPS() const;
	float GetMainFPSCapacity() const;


	SectionId StartSection(uint32_t threadId, const StringRef& sectionName);
	void EndSection(SectionId sectionId);
	
private:
	EngineDiagCore();
	~EngineDiagCore();

	EngineManager*	m_manager;
};

LN_NAMESPACE_END
