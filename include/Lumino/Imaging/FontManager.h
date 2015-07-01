
#pragma once
#include <map>
#include <Lumino/IO/FileManager.h>

// TODO: �������v����������J���Ă�͉̂��Ƃ��������c
typedef int  FT_Error;
typedef void*  FT_Pointer;
typedef FT_Pointer  FTC_FaceID;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FTC_ManagerRec_*  FTC_Manager;
typedef struct FTC_CMapCacheRec_*  FTC_CMapCache;
typedef struct FTC_ImageCacheRec_*  FTC_ImageCache;
typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_StrokerRec_*  FT_Stroker;
typedef struct FT_OutlineGlyphRec_*  FT_OutlineGlyph;
typedef struct FT_StreamRec_*  FT_Stream;
typedef struct FT_GlyphRec_*  FT_Glyph;

namespace Lumino
{
namespace Imaging
{
class Font;

/**
	@brief		�t�H���g�̊Ǘ��N���X
*/
class FontManager
	: public RefObject
{
public:
	static FontManager* Create(FileManager* fileManager);

public:

	/// �t�H���g�t�@�C����ǉ����� (ttf) (����o�^�̏ꍇ�̓f�t�H���g�t�H���g���Ƃ��ēo�^����)
	void RegisterFontFile(const String& fontFilePath);

	/// �f�t�H���g�̃t�H���g���Z�b�g����
	void SetDefaultFont(Font* font);

	/// �f�t�H���g�̃t�H���g���擾����
	Font* GetDefaultFont() const { return m_defaultFont; }

	void Dispose();


	Text::EncodingConverter* GetCharToUTF32Converter() { return &m_charToUTF32Converter; }
	Text::EncodingConverter* GetWCharToUTF32Converter() { return &m_wcharToUTF32Converter; }

private:
	FontManager(FileManager* fileManager);
	virtual ~FontManager();

	friend class FreeTypeFont;
	FT_Library GetFTLibrary() const { return m_ftLibrary; }
	FTC_Manager GetFTCacheManager() const { return m_ftCacheManager; }
	FTC_CMapCache GetFTCacheMapCache() const { return m_ftCMapCache; }
	FTC_ImageCache GetFTCImageCache() const { return m_ftImageCache; }

	// �L���b�V�������R�[���o�b�N
	FT_Error FaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

	// FaceRequester() �̌Ăяo����
	static FT_Error CallbackFaceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface);

#ifdef LN_WIN32
	typedef struct TSystemFontData
	{
		HDC hdc;
		HFONT hFont;
		HFONT hOldFont;
		unsigned char *fontdata;
		size_t size;
		int index;
	} TSystemFontData;

	/// Windows�ɓo�^����Ă���t�H���g�̃o�C�i���f�[�^�𖼏̂���擾
	TSystemFontData* GetWindowsSystemFontData(LPCTSTR name);
	unsigned char* LockWindowsSystemFontData(TSystemFontData *fnt, size_t *size, int *index);
	static void FreeWindowsSystemFontData(TSystemFontData *fnt);
	static unsigned long StreamIoFunc(FT_Stream stream, unsigned long offset, unsigned char* buffer, unsigned long count);
	static void StreamCloseFunc(FT_Stream stream);
#endif

	struct TTFDataEntry
	{
		ByteBuffer		DataBuffer;
		int				CollectionIndex;

		/* ��������̃f�[�^����Face�����ꍇ�AFT_Done_Face() ����܂Ń��������J�����Ă͂Ȃ�Ȃ��B
		* �t�@�C���X�g���[�����g�����Ƃ��ł��邪�A��������ƃt�H���g�����݂��Ă����
		* �X�g���[����1��L���Ă��܂����ƂɂȂ�B
		* �t�H���g�t�@�C���ЂƂ����� 4M �Ƃ����ʂɂ��邯�ǁA���1000x1000�r�b�g�}�b�v�ЂƂ��B
		* �m��Ȃ��Ƃ���ŃX�g���[�����J�����ςȂ��ɂȂ�̂͂ǂ����Ǝv�����A
		* (�����̃X�g���[�~���O�ƈ���āA�t�H���g�̃X�g���[�~���O�Ȃ�Č����Ă����ʂ̓s���Ɨ��Ȃ����낤���c)
		* ������`��͑��x�I�ɂ��Ȃ�N���e�B�J���Ȃ��́B��������ɒu�����ςȂ��ł����Ǝv���B
		*/
	};
	typedef std::map<uint32_t, TTFDataEntry>     TTFDataEntryMap;
	typedef std::pair<uint32_t, TTFDataEntry>    TTFDataEntryPair;
	TTFDataEntryMap		m_ttfDataEntryMap;

	RefPtr<FileManager>	m_fileManager;
	Font*  m_defaultFont;

	Text::EncodingConverter	m_charToUTF32Converter;
	Text::EncodingConverter	m_wcharToUTF32Converter;

	FT_Library      m_ftLibrary;
	FTC_Manager     m_ftCacheManager;
	FTC_CMapCache   m_ftCMapCache;
	FTC_ImageCache  m_ftImageCache;


	// FaceRequester() �� Windows �̃V�X�e���t�H���g���E�����߂̍׍H�B
	// FreeType �� FTC_Manager_LookupFace() �ɓn���ꂽ�A�h���X�𒼐ڎ����̃L�[�Ƃ���B
	// (�A�h���X�̒��g�܂ł͌��Ȃ��B���̂��߁A��������L�[�ɂ��邱�Ƃ͂ł��Ȃ�)
	// �܂�AfaceRequester() �Ƀt�H���g����`���邱�Ƃ͂ł��Ȃ��B�Ȃ̂ŁA�O���Ɉ�x�Ƃ��Ă����K�v������B
	// ���̕ϐ��ɂ́AFTC_Manager_LookupFace() �̒��O�Ńt�H���g�����Z�b�g���Ă����B
	// ���[�J���ϐ��̃|�C���^�ł悢�BFaceRequester() �� NULL ���i�[�����B
	const TCHAR*		m_requesterFaceName;

};

} // namespace Imaging
} // namespace Lumino
