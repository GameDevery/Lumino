
#include "Internal.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H /* <freetype/freetype.h> */
#include FT_CACHE_H /* <freetype/ftcache.h> */
#include FT_TRUETYPE_TAGS_H /* <freetype/tttags.h> */
#include FT_TRUETYPE_TABLES_H /* <freetype/tttables.h> */
#include FT_SFNT_NAMES_H
#include FT_STROKER_H
#include "FontManager.hpp"
#include "FreeTypeFont.hpp"

#define FLValueToFloatPx(x) (0.015625f * (x))	// x >> 6
#define FloatPxToFLValue(x) ((x) * 64)			// x << 6

namespace ln {
namespace detail {

//==============================================================================
// FreeTypeFont
/*
	���Ԍv���c
	FTC_CMapCache_Lookup, FTC_ImageCache_Lookup
	�����ݒ�              �T�C�Y�ݒ�E�O���t�擾

	�����t�H���g�T�C�Y�E��������
	4, 1522
	1, 1
	0, 0
	0, 0
	0, 0

	�����t�H���g�T�C�Y�E����++      2 �T�ڂ͂��ׂ� 0ns
	5, 1167
	1, 78
	1, 52
	1, 46
	1, 33

	�t�H���g�T�C�Y++�E��������      2�T�ڂ����l
	4, 1147 (ns)
	0, 60
	0, 62
	0, 66
	0, 61

	�t�H���g�T�C�Y++�E����++        2�T�ڂ����l
	5, 1158
	1, 85
	1, 81
	1, 55
	1, 40

	�T�C�Y�A�����ǂ��炩����ł��V�������̂�����Ƃ��낢��č\�z�����݂����B
	�������A��̊e�����ł���ɂ���1���������ꍇ�A
	�t�H���g�T�C�Y�������ł���� 0ns �������B
	�L���b�V���̒��ł̓T�C�Y�̕����̂��݂����B

	�܂��AFace �����̂ɕK�v�Ȏ��Ԃ� FT_Open_Face �� 300ns ���x�B
	�t�@�C�����[�h�E�������m�ۓ����� 3000ns ���炢�ɂȂ�B

	�݌v���j�c
	�E�t�H���g�t�@�C���ЂƂ��̃������̈�́A�����t�H���g�ŋ��L
	�EFreeTypeFont �N���X�ЂƂɑ΂��� FT_Face �͂ЂƂB

*/

FreeTypeFont::FreeTypeFont()
{
}

void FreeTypeFont::initialize(FontManager* manager, const FontDesc& desc)
{
	if (LN_REQUIRE(manager)) return;
	FontCore::initialize(manager);

	m_desc = desc;

	const String* name = &m_desc.Family;
	if (LN_REQUIRE(!name->isEmpty())) return;

	m_ftFaceId = (FTC_FaceID)CRCHash::compute(name->c_str());
	FTC_Manager ftc_manager = manager->ftCacheManager();
	//m_manager->m_requesterFaceName = name->c_str();

	FT_Error err = FTC_Manager_LookupFace(ftc_manager, m_ftFaceId, &m_ftFace);
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupFace : %d\n", err)) return;

	if (m_desc.isItalic)
	{
		// �C�^���b�N�̂̏ꍇ�� transform �ŌX����
		FT_Vector transform = { 0, 0 };
		FT_Matrix matrix;
		matrix.xx = 1 << 16;
		matrix.xy = 0x5800;
		matrix.yx = 0;
		matrix.yy = 1 << 16;
		FT_Set_Transform(m_ftFace, &matrix, &transform);
	}
	else
	{
		FT_Set_Transform(m_ftFace, NULL, NULL);
	}

	// m_ftFace->charmap �� m_ftFace �̒��Ō��݃A�N�e�B�u�� FT_CharMap�B
	// �O���t����肾���Ƃ��͂��̃C���f�b�N�X���w�肷��K�v������̂ŁA�����Ŋo���Ă����B
	m_ftCacheMapIndex = FT_Get_Charmap_Index(m_ftFace->charmap);

#define RESOLUTION_X 72
#define RESOLUTION_Y 72
	//#define SCALE_X(x) (FT_MulFix(x, em_scale))
	//#define SCALE_Y(y) (FT_MulFix(y, em_scale))

	// m_fontSize �ɑ΂���{���̕����T�C�Y���擾����
	FTC_ScalerRec scaler;
	scaler.face_id = m_ftFaceId;
	scaler.width = 0;
	scaler.height = m_desc.Size << 6;
	scaler.pixel = 0;
	scaler.x_res = RESOLUTION_X;
	scaler.y_res = RESOLUTION_Y;
	FT_Size ft_size;
	err = FTC_Manager_LookupSize(ftc_manager, &scaler, &ft_size);
	if (LN_ENSURE(err == 0, "failed FTC_Manager_LookupSize : %d\n", err)) return;

	//m_lineHeight = ft_size->metrics.height >> 6;

	if (FT_IS_SCALABLE(m_ftFace)) {	// has outline?
		if (!getOutlineTextMetrix()) {
			return;
		}
	}
	else {
		if (!getBitmapTextMetrix()) {
			return;
		}
	}
	//int hh = m_ftFace->height;


	//m_glyphData.releaseGlyph();

	//if (m_edgeSize > 0)
	//{
	//	// �G�b�W�̕`����
	//	FT_Stroker_New(m_manager->getFTLibrary(), &m_ftStroker);
	//	FT_Stroker_Set(m_ftStroker,
	//		(int)(m_edgeSize * 64),
	//		FT_STROKER_LINECAP_ROUND,	// �����̗��[�͔��~�Ń����_�����O����
	//		FT_STROKER_LINEJOIN_ROUND,	// �����̐ڍ��_�͔��~�Ń����_�����O����
	//		0);
	//}


	// font_type��ݒ�
	m_ftImageType.face_id = m_ftFaceId;
	m_ftImageType.width = 0;
	m_ftImageType.height = m_desc.Size;
	updateImageFlags();

	// �O���t�i�[�p�r�b�g�}�b�v (���m��)
	//m_glyphBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));
	//m_outlineBitmap.attach(LN_NEW RawBitmap(SizeI(m_desc.Size, m_desc.Size), PixelFormat::A8));

	// ���X�^���C�Y�Ŏg�p����
	//mPixelList = LN_NEW PixelData[m_fontSize * m_fontSize * 4];
}

void FreeTypeFont::dispose()
{
	// ���̂̎����� FreeType �� CacheManager ���Ǘ�����̂ŁA�����ł͎Q�Ƃ��O��������OK
	m_ftFace = nullptr;

	//if (m_ftStroker)
	//{
	//	FT_Stroker_Done(m_ftStroker);
	//	m_ftStroker = nullptr;
	//}

	FontCore::dispose();
}

void FreeTypeFont::getGlobalMetrics(FontGlobalMetrics* outMetrics)
{
	if (LN_REQUIRE(outMetrics)) return;
	if (LN_REQUIRE(m_ftFace)) return;
	outMetrics->ascender = FLValueToFloatPx(m_ftFace->size->metrics.ascender);
	outMetrics->descender = FLValueToFloatPx(m_ftFace->size->metrics.descender);
	outMetrics->lineSpace = outMetrics->ascender - outMetrics->descender;
	outMetrics->outlineSupported = FT_IS_SCALABLE(m_ftFace);
	outMetrics->boundingMinX = FLValueToFloatPx(m_ftFace->bbox.xMin);
	outMetrics->boundingMaxX = FLValueToFloatPx(m_ftFace->bbox.xMax);
	outMetrics->boundingMinY = FLValueToFloatPx(m_ftFace->bbox.yMin);
	outMetrics->boundingMaxY = FLValueToFloatPx(m_ftFace->bbox.yMax);
	// FIXME: Bitmap font �̏ꍇ�� bbox �� FT_Bitmap_Size ���g���ׂ��炵���B
	// (FT_FaceRec_ �̃R�����g)
}

void FreeTypeFont::getGlyphMetrics(UTF32 utf32Code, FontGlyphMetrics* outMetrics)
{
	LN_NOTIMPLEMENTED();
}

Vector2 FreeTypeFont::getKerning(UTF32 prev, UTF32 next)
{
	LN_NOTIMPLEMENTED();
	return Vector2();
}

void FreeTypeFont::decomposeOutline(UTF32 utf32code, VectorGlyphInfo* outInfo)
{
	LN_NOTIMPLEMENTED();
}

void FreeTypeFont::updateImageFlags()
{
	/* �r�b�g�}�b�v�܂ŃL���b�V������ꍇ��FT_LOAD_RENDER | FT_LOAD_TARGET_*
	* �Ƃ���B�������r����TARGET��ύX�����ꍇ���̓L���b�V�����ז�����B
	* ������������FT_LOAD_DEFAULT�ɂ���FTC_ImageCache_Lookup���
	* FT_Glyph_To_Bitmap�����ق����s���������Ǝv���B
	*/
	//m_ftImageType.flags = FT_LOAD_NO_BITMAP;

	/* m_ftImageType.flags �Ɂ@FT_LOAD_FORCE_AUTOHINT ���g�p����ƁA
	* �ꕔ�̃t�H���g(VL �S�V�b�N��)�Ŏ��̕����܂ł̃I�t�Z�b�g�����������Ȃ����Ƃ�����B
	*/

	// �A�E�g���C�� ON
	if (/*m_edgeSize > 0 || */m_desc.isBold) {
		// TODO: ���������[�N����
		m_ftImageType.flags = FT_LOAD_NO_BITMAP;
	}
	// �A�E�g���C�� OFF
	else {
		// TODO: FT_LOAD_RENDER �����Ă���ƁA�A�E�g���C�������Ȃ��B
		m_ftImageType.flags = FT_LOAD_RENDER;
	}
	// �A���`�G�C���A�X ON
	if (m_desc.isAntiAlias) {
		//m_ftImageType.flags = ; ���̂܂�
	}
	// �A���`�G�C���A�X OFF
	else {
		m_ftImageType.flags |= FT_LOAD_MONOCHROME;
	}
}

bool FreeTypeFont::getOutlineTextMetrix()
{
	// TrueType OS/2 table
	TT_OS2* os2 = (TT_OS2*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_os2);
	if (LN_ENSURE(os2 != nullptr, "FT_Get_Sfnt_Table")) return false;

	// TrueType horizontal header
	TT_HoriHeader* hori = (TT_HoriHeader*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_hhea);
	if (LN_ENSURE(hori != nullptr, "FT_Get_Sfnt_Table")) return false;

	FT_Fixed x_scale = m_ftFace->size->metrics.x_scale;
	FT_Fixed y_scale = m_ftFace->size->metrics.y_scale;

	int tmAscent = (FT_MulFix(os2->usWinAscent, y_scale) + 32) >> 6;
	int tmDescent = (FT_MulFix(os2->usWinDescent, y_scale) + 32) >> 6;
	int tmInternalLeading = (FT_MulFix(os2->usWinAscent + os2->usWinDescent - m_ftFace->units_per_EM, y_scale) + 32) >> 6;
	int tmExternalLeading = std::max(0L, (FT_MulFix(hori->Line_Gap -
		((os2->usWinAscent + os2->usWinDescent) -
		(hori->Ascender - hori->Descender)), y_scale) + 32) >> 6);
	//m_lineHeight = tmInternalLeading + tmAscent + tmDescent + tmExternalLeading;


	//#define SCALE_X(x) (FT_MulFix(x, m_ftFace->size->metrics.x_scale))
	//#define SCALE_Y(y) (FT_MulFix(y, m_ftFace->size->metrics.y_scale))
	//
	//	TT_HoriHeader* hori = (TT_HoriHeader*)FT_Get_Sfnt_Table(m_ftFace, ft_sfnt_hhea);
	//
	//	int tmAscent = SCALE_Y(hori->Ascender);
	//	int tmDescent = SCALE_Y(hori->Descender);
	//	int tmInternalLeading = SCALE_Y(hori->Ascender + hori->Descender - m_ftFace->units_per_EM);
	//	m_lineHeight = tmInternalLeading + tmAscent + tmDescent;
	return false;
}

bool FreeTypeFont::getBitmapTextMetrix()
{
	LN_NOTIMPLEMENTED();
	return false;
}

} // namespace detail
} // namespace ln

