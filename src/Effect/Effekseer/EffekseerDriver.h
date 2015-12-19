
#pragma once
#include <EffekseerRuntime101/src/Effekseer/Effekseer.h>
#include <EffekseerRuntime101/src/EffekseerRendererDX9/EffekseerRendererDX9.h>
#include <EffekseerRuntime101/src/EffekseerSoundXAudio2/EffekseerSoundXAudio2.h>
#include <Lumino/Base/Cache.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Effect/VisualEffect.h>
#include <Lumino/Effect/VisualEffectInstance.h>
#include "../EffectDriver.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class EffekseerEffectInstance;
class EffekseerEffectCore;

// Effekseer::FileReader �̎���
class EffekseerFileReader
	: public ::Effekseer::FileReader
{
public:
	EffekseerFileReader(Stream* stream) : m_stream(stream, false) {}
	virtual ~EffekseerFileReader() = default;

	virtual size_t Read(void* buffer, size_t size) override { return m_stream->Read(buffer, size); }
	virtual void Seek(int position) override { m_stream->Seek(position, SeekOrigin_Begin); }
	virtual int GetPosition() override { return (int)m_stream->GetPosition(); }
	virtual size_t GetLength() override { return (size_t)m_stream->GetLength(); }

private:
	RefPtr<Stream>	m_stream;
};

// Effekseer::FileInterface �̎���
class EffekseerFileInterface
	: public ::Effekseer::FileInterface
{
public:
	EffekseerFileInterface(FileManager* fileManager);

	virtual ::Effekseer::FileReader* OpenRead(const EFK_CHAR* path) override;
	virtual ::Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path) override;

private:
	FileManager* m_fileManager;
};

// EffekseerEffect �̊Ǘ�
class EffekseerEffectEngine
	: public EffectEngine
{
public:
	EffekseerEffectEngine();
	virtual ~EffekseerEffectEngine();

	void Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize, int maxSpriteCount);
	void Finalize();
	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void UpdateRenderContents();

	/// EffectCore �̍쐬 (�L���b�V�������L��)
	///		�G�t�F�N�g�����^�C�����t�@�C���t�H�[�}�b�g�G���[�ƂȂ����ꍇ�͗�O�ɂ����ANULL ��Ԃ��B
	///		(���������t�@�C����������Ȃ��ꍇ�͗�O�ƂȂ�)
	virtual VisualEffect* CreateEffectCore(const PathName& filePath) override;

	virtual void SetViewProjectin(const Matrix& view, const Matrix& proj) override;
	virtual void UpdateFrame(float elapsedTime) override;
	virtual void Render() override;

public:
	::Effekseer::Manager* GetEffekseerManager() { return m_efkManager; }
	::EffekseerRenderer::Renderer* GetEffekseerRenderer() { return m_efkRenderer; }

private:
	::Effekseer::Manager*			m_efkManager;
	::EffekseerRenderer::Renderer*	m_efkRenderer;
	::EffekseerSound::Sound*		m_efkSound;
	EffekseerFileInterface*			m_fileInterface;
	EncodingConverter				m_TCharToUTF16Converter;
};

// Effekseer �̃G�t�F�N�g�I�u�W�F�N�g Wrapper�B�L���b�V���Ǘ������B
class EffekseerEffectCore
	: public VisualEffect
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
public:
	EffekseerEffectCore(EffekseerEffectEngine* engine, ::Effekseer::Effect* efkEffeect);
	virtual ~EffekseerEffectCore();

	virtual VisualEffectInstance* PlayNewInstance() override;

	EffekseerEffectEngine*	GetEffectEngine() { return m_effectEngine; }
	::Effekseer::Effect*	GetEffekseerEffect() { return m_efkEffect; }

private:
	EffekseerEffectEngine*	m_effectEngine;
	::Effekseer::Effect*	m_efkEffect;
};

// Effekseer �̃G�t�F�N�g�C���X�^���X Wrapper
class EffekseerEffectInstance
	: public VisualEffectInstance//EffectInstance
{
public:
	EffekseerEffectInstance(EffekseerEffectCore* ownerCore, ::Effekseer::Handle handle);
	virtual ~EffekseerEffectInstance();

	EffekseerEffectCore* GetEffectCore() { return m_ownerEffectCore; }

	//virtual void Play(bool overlap) override;
	virtual void Stop() override;
	virtual bool IsPlaying() override;
	virtual void SetWorldMatrix(const Matrix& matrix) override;

	void UpdateFrame();	// �X�V�X���b�h
	void Draw();	// �`��X���b�h

	static void LNToEFKMatrix43(const Matrix& mat, ::Effekseer::Matrix43* efkMat);

private:
	EffekseerEffectCore*		m_ownerEffectCore;
	::Effekseer::Handle			m_currentHandle;
	Array<::Effekseer::Handle>	m_drawHandleArray;
	Matrix						m_worldMatrix;
};

} // namespace detail
LN_NAMESPACE_END
