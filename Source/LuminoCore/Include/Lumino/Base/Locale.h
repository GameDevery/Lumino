/**
	@file	Locale.h
*/
#pragma once
#include <locale>
#include "String.h"

LN_NAMESPACE_BEGIN

/**
	@brief	���P�[����\���܂��B
*/
class Locale
{
public:

	/**
		@brief		�f�t�H���g�̃��P�[�����g�p���ăI�u�W�F�N�g���\�z���܂��B
		@details	���̃��P�[���� GetDefault() �ŕԂ���郍�P�[���Ɠ����ɂȂ�܂��B
	*/
	Locale();

	/**
		@brief		���P�[�������w�肵�ăI�u�W�F�N�g���\�z���܂��B
	*/
	explicit Locale(const Char* name);

	Locale(const Locale& locale);
	Locale& operator=(const Locale& locale);
	~Locale();

public:

	/**
		@brief		���݂̃f�t�H���g�̃��P�[�����擾���܂��B
		@details	Windows �̏ꍇ�A���[�U�[���P�[�����g�p���܂��B
	*/
	static const Locale& getDefault();

	/**
		@brief		"C" ���P�[�����擾���܂��B
		@details	"C" ���P�[���� ANSI �ɏ��������ŏ�����\���܂��B
	*/
	static const Locale& getC();

public:
	const std::locale& getStdLocale() const;
	NativeLocale_t getNativeLocale() const;

private:
	void release();

private:
	std::locale		m_stdLocale;
	NativeLocale_t	m_nativeLocale;
#ifdef LN_USTRING
#ifdef LN_OS_WIN32
	std::wstring		m_nativeName;
#else
	std::string			m_nativeName;
#endif

#else
#ifdef LN_OS_WIN32
	StringW			m_nativeName;
#else
	StringA			m_nativeName;
#endif
#endif
};

/**
	@brief		
*/
template<typename TChar>
class GenericLocalizer
{
public:
	GenericLocalizer(const Locale& locale);

public:
	int format(TChar* outBuf, int outBufLength, const TChar* format, ...);

private:
	Locale	m_locale;
};

LN_NAMESPACE_END
