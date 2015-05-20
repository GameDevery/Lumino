
#pragma once

#include "../Graphics/Color.h"

namespace Lumino
{
namespace Imaging
{
class FileManager;

/**
	@brief		�t�H���g�̃N���X
*/
class Font
	: public RefObject
{
public:
	static Font* CreateBitmapFont(FileManager* fileManager, Bitmap* bitmap);

public:
#if 0
	/// �t�H���g���̐ݒ�
	virtual void setName(const TCHAR* fontName) = 0;

	/// �t�H���g���̎擾
	virtual const TCHAR* getName() const = 0;

	/// �t�H���g�T�C�Y�̐ݒ�
	virtual void setSize(int size) = 0;

	/// �t�H���g�T�C�Y�̎擾
	virtual int getSize() const = 0;
/*
	/// �t�H���g�J���[�̐ݒ�
	virtual void setColor(const Color& color) = 0;

	/// �t�H���g�J���[�̎擾
	virtual const Color& getColor() const = 0;

	/// �G�b�W�J���[�̐ݒ�
	virtual void setEdgeColor(const Color& color) = 0;

	/// �G�b�W�J���[�̎擾
	virtual const Color& getEdgeColor() const = 0;*/

	///// �G�b�W�̕��̐ݒ� (0 �ŃG�b�W����)
	//virtual void setEdgeSize(int size) = 0;

	///// �G�b�W�̕��̎擾
	//virtual int getEdgeSize() const = 0;

	/// �������̐ݒ�
	virtual void setBold(bool flag) = 0;

	/// �������̔���
	virtual bool isBold() const = 0;

	/// �C�^���b�N�̂̐ݒ�
	virtual void setItalic(bool flag) = 0;

	/// �C�^���b�N�̂̔���
	virtual bool isItalic() const = 0;

	/// �A���`�G�C���A�X�̗L���ݒ�
	virtual void setAntiAlias(bool flag) = 0;

	/// �A���`�G�C���A�X�̗L������
	virtual bool isAntiAlias() const = 0;

	/// �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾 (length = -1 �� \0 �܂�)
	virtual void getTextSize(const char* text, int length, Geometry::Rect* outRect) = 0;

	/// �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾 (length = -1 �� \0 �܂�)
	virtual void getTextSize(const wchar_t* text, int length, Geometry::Rect* outRect) = 0;

	/// ���̃t�H���g�̃R�s�[���쐬����
	virtual Font* copy() = 0;

	/// �O���t�f�[�^�̎擾 (�ŏ��̕����̏ꍇ�AprevData �� NULL ��n���B�ȍ~�͖߂�l��n��������B��X���b�h�Z�[�t)
	virtual FontGlyphData* makeGlyphData(int utf32code, FontGlyphData* prevData) = 0;

	/// �O���t�f�[�^�̎擾���I������ (����������B��A�� makeGlyphData() ���ĂяI�������A�Ō�ɌĂ�)
	virtual void postGlyphData(FontGlyphData* glyphData) = 0;
#endif
protected:
	Font();
	~Font();
};

} // namespace Imaging
} // namespace Lumino
