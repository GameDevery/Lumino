
#pragma once
#include "../Base/ByteBuffer.h"
#include "Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		�e�L�X�g�Ԃ̃G���R�[�f�B���O�̕ϊ����s���N���X�ł��B
	@details	�����G���R�[�f�B���O�ŉ�����ϊ�����ہA�����I�ɕϊ����s�����Ƃ��ł��܂��B
*/
class EncodingConverter
{
public:
	EncodingConverter();
	~EncodingConverter();

public:

	/**
		@brief		�ϊ���e�L�X�g�̃G���R�[�f�B���O��ݒ肵�܂��B
	*/
	void setDestinationEncoding(Encoding* encoding);

	/**
		@brief		�ϊ���e�L�X�g�̃G���R�[�f�B���O���擾���܂��B
	*/
	Encoding* getDestinationEncoding() const;

	/**
		@brief		�ϊ����e�L�X�g�̃G���R�[�f�B���O��ݒ肵�܂��B
	*/
	void getSourceEncoding(Encoding* encoding);

	/**
		@brief		�ϊ����e�L�X�g�̃G���R�[�f�B���O���擾���܂��B
	*/
	Encoding* getSourceEncoding() const;

	/**
		@brief		�ϊ��̃I�v�V������ݒ肵�܂��B
	*/
	void setConversionOptions(const EncodingConversionOptions& options);

	/**
		@brief		�e�L�X�g��ϊ����܂��B
	*/
	const ByteBuffer& convert(const void* data, size_t byteCount, EncodingConversionResult* outResult = NULL);

	/**
		@brief		�Ō�ɌĂяo���� convert() �ŕϊ����ꂽ�o�b�t�@���擾���܂��B����� convert() �̖߂�l�Ɠ���ł��B
	*/
	const ByteBuffer& getLastBuffer() const;

	/**
		@brief		�Ō�ɌĂяo���� convert() �� EncodingConversionResult ���擾���܂��B
	*/
	const EncodingConversionResult& getLastResult() const;

private:
	void checkUpdateEncoderDecoder();

	void convertDecoderRemain(
		const void* src_, size_t srcByteCount, Decoder* srcDecoder,
		void* dest_, size_t destByteCount, Encoder* destEncoder,
		EncodingConversionResult* outResult);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(EncodingConverter);
	Encoding*					m_dstEncoding;
	Encoding*					m_srcEncoding;
	Encoder*					m_dstEncoder;
	Decoder*					m_srcDecoder;
	ByteBuffer					m_outputBuffer;
	EncodingConversionOptions	m_options;
	EncodingConversionResult	m_lastResult;
	ByteBuffer					m_tmpBuffer;			///< ��Ԃ�ێ��ł��Ȃ��f�R�[�_���g���Ă���ꍇ�Ɏg�p����ꎞ�o�b�t�@
	bool						m_encodingModified;
};

LN_NAMESPACE_END
