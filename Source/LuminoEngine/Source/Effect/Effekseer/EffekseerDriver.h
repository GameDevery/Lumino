﻿
#pragma once
#include <Effekseer.h>
#if defined(LN_OS_WIN32)
	#include <EffekseerRendererDX9.h>
	#include <EffekseerSoundXAudio2.h>
#else
	#include <EffekseerRuntime101/src/EffekseerSoundAL/EffekseerSoundAL.h>
#endif
#include <EffekseerRendererGL.h>
#include <Lumino/Base/Cache.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Effect/VisualEffect.h>
#include <Lumino/Effect/VisualEffectInstance.h>
#include "../EffectDriver.h"

namespace EffekseerSound
{
    class Sound;
}

LN_NAMESPACE_BEGIN
namespace detail
{
class EffekseerEffectInstance;
class EffekseerEffectCore;

// Effekseer::FileReader の実装
class EffekseerFileReader
	: public ::Effekseer::FileReader
{
public:
	EffekseerFileReader(Stream* stream) : m_stream(stream, false) {}
	virtual ~EffekseerFileReader() = default;

	virtual size_t Read(void* buffer, size_t size) override { return m_stream->read(buffer, size); }
	virtual void Seek(int position) override { m_stream->seek(position, SeekOrigin_Begin); }
	virtual int GetPosition() override { return (int)m_stream->getPosition(); }
	virtual size_t GetLength() override { return (size_t)m_stream->getLength(); }

private:
	Ref<Stream>	m_stream;
};

// Effekseer::FileInterface の実装
class EffekseerFileInterface
	: public ::Effekseer::FileInterface
{
public:
	EffekseerFileInterface(FileManager* fileManager);
	virtual ~EffekseerFileInterface() = default;

	virtual ::Effekseer::FileReader* OpenRead(const EFK_CHAR* path) override;
	virtual ::Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path) override;

private:
	FileManager* m_fileManager;
};

// EffekseerEffect の管理
class EffekseerEffectEngine
	: public EffectEngine
{
public:
	EffekseerEffectEngine();
	virtual ~EffekseerEffectEngine();

	void initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize, int maxSpriteCount);
	void dispose();
	void SetViewProjection(const Matrix& view, const Matrix& proj);
	void UpdateRenderContents();

	/// EffectCore の作成 (キャッシュ検索有効)
	///		エフェクトランタイムがファイルフォーマットエラーとなった場合は例外にせず、NULL を返す。
	///		(そもそもファイルが見つからない場合は例外となる)
	virtual VisualEffect* CreateEffectCore(const PathName& filePath) override;

	virtual void SetViewProjectin(const Matrix& view, const Matrix& proj) override;
	virtual void updateFrame(float elapsedTime) override;
	virtual void render() override;
	virtual void onLostDevice() override;
	virtual void onResetDevice() override;

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

// Effekseer のエフェクトオブジェクト Wrapper。キャッシュ管理される。
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

// Effekseer のエフェクトインスタンス Wrapper
class EffekseerEffectInstance
	: public VisualEffectInstance//EffectInstance
{
public:
	EffekseerEffectInstance(EffekseerEffectCore* ownerCore, ::Effekseer::Handle handle);
	virtual ~EffekseerEffectInstance();

	EffekseerEffectCore* GetEffectCore() { return m_ownerEffectCore; }

	//virtual void play(bool overlap) override;
	virtual void stop() override;
	virtual bool isPlaying() override;
	virtual void SetWorldMatrix(const Matrix& matrix) override;

	void updateFrame();	// 更新スレッド
	void draw();	// 描画スレッド

	static void LNToEFKMatrix43(const Matrix& mat, ::Effekseer::Matrix43* efkMat);

private:
	EffekseerEffectCore*		m_ownerEffectCore;
	::Effekseer::Handle			m_currentHandle;
	List<::Effekseer::Handle>	m_drawHandleArray;
	Matrix						m_worldMatrix;
};

} // namespace detail
LN_NAMESPACE_END
