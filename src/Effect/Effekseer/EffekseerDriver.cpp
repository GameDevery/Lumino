/*
	[2015/12/19] Effekseer �̌ʕ`��ɂ���
		�@�\�Ƃ��Ă͗p�ӂ���Ă��邪�����͂��܂�悭�Ȃ������B

		DrawHandle() �̂��тɃN���e�B�J���Z�N�V�����ɓ����Ă��邵�A�n���h������C���X�^���X�����߂�̂� set �̌���������B
		(�ꊇ�`��Ȃ�C�e���[�g���邾��)
		���̎d�g�݂� Update() �������B

		�܂��A�ʂ� BeginRendering()�AEndRedering() ����K�v������A�����őS�ẴX�e�[�g��ۑ�����B
		(����̓X�e�[�g�������ۑ����Ȃ��悤�ɂ��邱�Ƃ͂ł��邪)


	[2015/12/15] Effekseer ����

	InstanceContainer
		InstanceContainer
	InstanceContainer
		InstanceContainer
		InstanceContainer

	ManagerImplemented
		Flip() �� m_renderingDrawSets �� DrawSet ���������B
		Draw() �� m_renderingDrawSets �ɓ����Ă��� DrawSet ��`�悷��B
		��
		Flip() �� Draw() �͕�����s�ł��Ȃ��B

	ManagerImplemented::Play()
		InstanceContainer ������� m_DrawSets �ɓ����B

	ManagerImplemented::Update()
		�T���v���ł� Play() �ƕ�����s�ł��邱�ƂɂȂ��Ă���B
		���g�� m_renderingDrawSets �ɑ΂��čX�V���s���Ă���B

	�܂Ƃ�
		�EPlay() �� Update() �͕�����s�ł���B
		�E1�t���[���� {Flip()} �� {Update()} �� {Draw()} �̏��Ŏ��s���Ȃ���΂Ȃ�Ȃ��B
		�EPlay() �� Update() �܂��� Draw() �ƕ���Ŏ��s�ł���BFlip() �Ƃ͂ł��Ȃ��B

	�΍�
		�EUpdate() �͍X�V�X���b�h��V�������āADraw() �͕`��X���b�h����Ăт����B
		�EUpdate() ���x��Ă��邩��ƌ����āADraw() ���Ȃ��͖̂����B

		�O��
		�ELumino �́A�`��X���b�h���x��Ă���Ȃ� Draw() �͌Ă΂Ȃ��B

		�E2�t���[���ڈȍ~�ŁA
										[�X�V�X���b�h]		[�`��X���b�h]
			PreUpdateFrame Flip()	��	�J�n ��1				��
			UpdateFrame					�E�E�E				��
			PostUpdateFrame				�E�E�E				��
			PreDraw						�I��					�J�n ��2
			Draw						
			PostDraw				��						�R�}���h���s�J�n

			��1 �܂��� Draw() ���I���܂ő҂B
			��2 �܂��� Update() ���I���܂ő҂B

			��������������ƁA�`�悪�x��Ă��鎞�����ɂȂ肻���B
				�X�V�X���b�h�̊J�n�́A�J�n���_�ł܂� Draw �����s����������s�����܂ő҂B
				�܂��́A����̃t���[���ł͍X�V���s��Ȃ��B
				�҂p�^�[���ł́A�`��x�������C���X���b�h�ɋ����̂ł�肽���Ȃ��B
				�X�L�b�v����ꍇ�A�X�L�b�v�����t���[���Œx�������������Ƃ��A
				���� Draw �ł���Ƃ��� Update �����{�B�܂�G�t�F�N�g���~�܂��Č�����B
				�x�����͉��t���[�������ɒx������������͂�������A
				�x�����������x���������E�E�E�Ƒ����Ɖi���Ɏ~�܂��Č�����B

				�΍�ł��Ȃ��͂Ȃ��Ǝv�����ǁA���G������B
				���̈Ăłǂ����Ă����x���łȂ���Ή��߂čl����B

		�E�V���v���ň��S�Ɏ�������Ȃ� Update ��`��X���b�h�ōs���̂�����B
			�E���C���X���b�h�� Manager::Render() ������`��X���b�h�ɃR�}���h������B
			�E�����ɍX�V�X���b�h�ɊJ�n��ʒm����B
			�E�R�}���h���s�͍X�V���I���܂ő҂B
			��
			�܂�AUpdateFrame �̏I�� �` �`��R�}���h�̎��s�J�n �܂ł���񉻂���Ƃ������@�B
			�����܂ŕ`��̈�ƍl���ADraw() ����ׂ����͕K���Z�b�g�ōX�V�J�n����B

			�X�V�X���b�h�ɊJ�n��ʒm����̂́A���ݕ`��x�����Ă��邩�𔻕ʂ��A�`�悷�邱�ƂɂȂ������オ�x�X�g�B
			���� Draw �͏I����Ă���̂ő҂��Ƃ��l����K�v�͂Ȃ��B
			�܂��A�ʃ��W���[���̕`��J�n�O���������ƕ��񉻂ł���B

*/
#include "../Internal.h"
#include "../../Audio/XAudio2/XAudio2AudioDevice.h"
#include "../../Graphics/Device/DirectX9/DX9GraphicsDevice.h"
#include "../../Audio/AudioManager.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include "../EffectManager.h"
#include "EffekseerDriver.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// EffekseerFileInterface
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerFileInterface::EffekseerFileInterface(FileManager* fileManager)
	: m_fileManager(fileManager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
::Effekseer::FileReader* EffekseerFileInterface::OpenRead(const EFK_CHAR* path)
{
	return new EffekseerFileReader(m_fileManager->CreateFileStream((wchar_t*)path));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
::Effekseer::FileWriter* EffekseerFileInterface::OpenWrite(const EFK_CHAR* path)
{
	LN_THROW(0, NotImplementedException);
	return nullptr;
}


//=============================================================================
// EffekseerEffectEngine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectEngine::EffekseerEffectEngine()
	: m_efkManager(nullptr)
	, m_efkRenderer(nullptr)
	, m_efkSound(nullptr)
{
	EncodingConversionOptions options;
	options.NullTerminated = true;
	m_TCharToUTF16Converter.SetConversionOptions(options);
	m_TCharToUTF16Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_TCharToUTF16Converter.SetDestinationEncoding(Encoding::GetUTF16Encoding());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectEngine::~EffekseerEffectEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize, int maxSpriteCount)
{
	EffectEngine::Initialize(manager, cacheObjectCount, cacheMemorySize);

	// �t�@�C��IO�C���^�[�t�F�C�X
	m_fileInterface = LN_NEW EffekseerFileInterface(m_manager->GetFileManager());

	auto* dx9Device = dynamic_cast<Driver::DX9GraphicsDevice*>(m_manager->GetGraphicsManager()->GetGraphicsDevice());
	if (dx9Device != nullptr)
	{
		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���(�����t���b�v�͍s��Ȃ�)
		m_efkManager = ::Effekseer::Manager::Create(2000, false);

		// �`��p�C���X�^���X�̐���
		m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(dx9Device->GetIDirect3DDevice9(), maxSpriteCount);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
		m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
		m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
		m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
		m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader(m_fileInterface));
		m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader(m_fileInterface));
		m_efkManager->SetEffectLoader(::Effekseer::Effect::CreateEffectLoader(m_fileInterface));
	}
	else {
		LN_THROW(0, NotImplementedException);
	}

	// Audio
	if (m_efkManager != nullptr)
	{
		auto* xa2Device = dynamic_cast<XAudio2AudioDevice*>(m_manager->GetAudioManager()->GetAudioDevice());
		if (xa2Device != nullptr)
		{
			// ���Đ��p�C���X�^���X�̐���
			m_efkSound = ::EffekseerSound::Sound::Create(xa2Device->GetXAudio2(), 16, 16);

			// ���Đ��p�C���X�^���X����Đ��@�\���w��
			m_efkManager->SetSoundPlayer(m_efkSound->CreateSoundPlayer());

			// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
			m_efkManager->SetSoundLoader(m_efkSound->CreateSoundLoader(m_fileInterface));
		}
		else {
			// �T�E���h�h���C�o�������Ƃ��� NullDevice ���g�p���Ă��鎞
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::Finalize()
{
	EffectEngine::Finalize();

	// Effekseer �̃T���v���ł͐�ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j������K�v������悤��
	if (m_efkManager != nullptr) {
		m_efkManager->Destroy();
		m_efkManager = NULL;
	}
	if (m_efkRenderer != nullptr) {
		m_efkRenderer->Destory();
		m_efkRenderer = NULL;
	}
	if (m_efkSound != nullptr) {
		m_efkSound->Destory();
		m_efkSound = NULL;
	}

	LN_SAFE_DELETE(m_fileInterface);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	if (m_efkRenderer)
	{
		m_efkRenderer->SetCameraMatrix((const ::Effekseer::Matrix44&)view);
		m_efkRenderer->SetProjectionMatrix((const ::Effekseer::Matrix44&)proj);
	}
}

//-----------------------------------------------------------------------------
// �񓯊��X�V�E�`��̓����t�F�[�Y�ŌĂ΂��
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::UpdateRenderContents()
{
	if (m_efkManager) {
		m_efkManager->Flip();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualEffect* EffekseerEffectEngine::CreateEffectCore(const PathName& filePath)
{
	CacheKey key(filePath);

	// �L���b�V������
	RefPtr<EffekseerEffectCore> core(static_cast<EffekseerEffectCore*>(m_effectCoreCache->FindObjectAddRef(key)), false);
	if (core != nullptr) {
		core.SafeAddRef();
		return core;
	}

	// unicode ��
	ByteBuffer utf16 = m_TCharToUTF16Converter.Convert(filePath.c_str(), _tcslen(filePath.c_str()) * sizeof(TCHAR));

	// �G�t�F�N�g�̓Ǎ�
	Effekseer::Effect* efkEffect = Effekseer::Effect::Create(
		m_efkManager,
		(const EFK_CHAR*)utf16.GetConstData());
	if (efkEffect == NULL) {
		return NULL;
	}

	// EffectCore �쐬
	core.Attach(LN_NEW EffekseerEffectCore(this, efkEffect), false);

	// �L���b�V���ɓo�^
	if (!key.IsNull()) {
		m_effectCoreCache->RegisterCacheObject(key, core);
	}

	core.SafeAddRef();
	return core;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::SetViewProjectin(const Matrix& view, const Matrix& proj)
{
	m_efkRenderer->SetCameraMatrix((const ::Effekseer::Matrix44&)view);
	m_efkRenderer->SetProjectionMatrix((const ::Effekseer::Matrix44&)proj);

	// ���e�s���ݒ�
	//	::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)640 / (float)480, 1.0f, 50.0f));

	//// �J�����s���ݒ�
	//m_efkRenderer->SetCameraMatrix(
	//	::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(10.0f, 5.0f, 20.0f), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::UpdateFrame(float elapsedTime)
{
	m_efkManager->Flip();
	m_efkManager->Update();	// TODO: time
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectEngine::Render()
{
	// �G�t�F�N�g�̕`��J�n�������s���B
	m_efkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	m_efkManager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	m_efkRenderer->EndRendering();
}

//=============================================================================
// EffekseerEffectCore
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectCore::EffekseerEffectCore(EffekseerEffectEngine* engine, ::Effekseer::Effect* efkEffeect)
	: m_effectEngine(engine)
	, m_efkEffect(efkEffeect)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectCore::~EffekseerEffectCore()
{
	ES_SAFE_RELEASE(m_efkEffect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualEffectInstance* EffekseerEffectCore::PlayNewInstance()
{
	auto handle = GetEffectEngine()->GetEffekseerManager()->Play(GetEffekseerEffect(), 0, 0, 0);
	return LN_NEW EffekseerEffectInstance(this, handle);
}

//=============================================================================
// EffekseerEffectCore
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectInstance::EffekseerEffectInstance(EffekseerEffectCore* ownerCore, ::Effekseer::Handle handle)
	: m_ownerEffectCore(ownerCore)
	, m_currentHandle(handle)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EffekseerEffectInstance::~EffekseerEffectInstance()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void EffekseerEffectInstance::Play(bool overlap)
//{
//	if (m_currentHandle != 0)
//	{
//		// ���̏�ԂōX�V������A�o�b�N��
//		::Effekseer::Matrix43 efkMat;
//		LNToEFKMatrix43(m_worldMatrix, &efkMat);
//		m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager()->SetMatrix(m_currentHandle, efkMat);
//		m_drawHandleArray.Add(m_currentHandle);
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectInstance::Stop()
{
	if (m_currentHandle != 0)
	{
		::Effekseer::Manager* efkManager = m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager();
		efkManager->StopEffect(m_currentHandle);
		for (::Effekseer::Handle handle : m_drawHandleArray)
		{
			efkManager->StopEffect(handle);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool EffekseerEffectInstance::IsPlaying()
{
	if (m_currentHandle == 0) {
		return false;
	}
	return m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager()->Exists(m_currentHandle);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectInstance::SetWorldMatrix(const Matrix& matrix)
{
	m_worldMatrix = matrix;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectInstance::UpdateFrame()
{
	::Effekseer::Manager* efkManager = m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager();

	::Effekseer::Matrix43 efkMat;
	LNToEFKMatrix43(m_worldMatrix, &efkMat);
	efkManager->SetMatrix(m_currentHandle, efkMat);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectInstance::Draw()
{
	//::Effekseer::Manager* efkManager = mEffectCore->getEffectEngine()->getEffekseerManager();
	//::EffekseerRenderer::Renderer* r = mEffectCore->getEffectEngine()->getEffekseerRenderer();

	//r->BeginRendering();
	//ln_foreach(::Effekseer::Handle handle, mDrawHandleArray) {
	//	efkManager->DrawHandle(handle);
	//}
	//r->EndRendering();

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffekseerEffectInstance::LNToEFKMatrix43(const Matrix& mat, ::Effekseer::Matrix43* efkMat)
{
	efkMat->Value[0][0] = mat.M[0][0];
	efkMat->Value[0][1] = mat.M[0][1];
	efkMat->Value[0][2] = mat.M[0][2];

	efkMat->Value[1][0] = mat.M[1][0];
	efkMat->Value[1][1] = mat.M[1][1];
	efkMat->Value[1][2] = mat.M[1][2];

	efkMat->Value[2][0] = mat.M[2][0];
	efkMat->Value[2][1] = mat.M[2][1];
	efkMat->Value[2][2] = mat.M[2][2];

	efkMat->Value[3][0] = mat.M[3][0];
	efkMat->Value[3][1] = mat.M[3][1];
	efkMat->Value[3][2] = mat.M[3][2];
}

} // namespace detail
LN_NAMESPACE_END

