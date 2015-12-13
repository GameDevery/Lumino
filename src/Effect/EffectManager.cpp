/*
�G�t�F�N�g�̍\���ɂ���
	Core �c �P�Ȃ�f�[�^�N���X�B

	Instance
		���ۂɍX�V�E�q���������s���B
		�ŏI�ړI�͕`��̂��߂̏��̐����B(�s��A�Fetc...)
		�`����s���̂� EmitterCore�B

	�e�q�֌W�ɂ���
		update() �Őe����e�����󂯂�p�����[�^�͍s��̂݁B(�����^�C�~���O�͕ʂƂ��āA�X�V���̗v�f)
		����ȊO�̃p�����[�^(�Ⴆ��Ring �̌a�Ƃ�)�́ANodeInstance �� EmitterCore ��
		�`�掞�Ɍ��݂̎��Ԃ�`���āA�����Œl���X�V����B

	�����V�[�h�ɂ���
		NodeInstance ���ƂɌŗL�̒l�������Ȃ���΂Ȃ�Ȃ��B
		�{���ł���΃G�f�B�^����ݒ�ł���悤�ɂ���ׂ������A
		�Ƃ肠���� this ���g���Ă���B

*/
#include "Internal.h"
#include "Effekseer\EffekseerDriver.h"
#include "EffectManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// Internal
//=============================================================================
static EffectManager* g_managerInstance = nullptr;

EffectManager* GetEffectManager(EffectManager* priority)
{
	if (priority != NULL) return priority;
	return g_managerInstance;
}

//=============================================================================
// EffectManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectManager::EffectManager()
	: m_fileManager(nullptr)
	, m_graphicsManager(nullptr)
	, m_audioManager(nullptr)
	, m_engine(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffectManager::~EffectManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Initialize(const Settings& settings)
{
	m_fileManager = settings.fileManager;
	m_graphicsManager = settings.graphicsManager;
	m_audioManager = settings.audioManager;

	auto engine = RefPtr<EffekseerEffectEngine>::Create();
	engine->Initialize(this, 32, 0, 2000);
	m_engine = engine.DetachAddRef();

	if (g_managerInstance == nullptr) {
		g_managerInstance = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Finalize()
{
	if (m_engine != nullptr) {
		m_engine->Finalize();
		LN_SAFE_RELEASE(m_engine);
	}

	if (g_managerInstance == this) {
		g_managerInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::PreRender()
{
	m_engine->UpdateFrame(0.016);	// TODO: time
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectManager::Render()
{
	m_engine->Render();
}

} // namespace detail
LN_NAMESPACE_END
