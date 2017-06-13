
#pragma once
#include <Lumino/Documents/Common.h>
#include <Lumino/UI/LayoutElement.h>
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DocumentsManager;
class Inline;
class Block;

enum class InternalTextElementType
{
	Common,
	LineBreak,
};

class DocumentSelection
{
public:
	int	start;		///< text.GetLength() �͗L���l�BSelect(text.GetLength(), 0) �ŃR���e���c�̖����ɃL�����b�g���ړ�����
	int length;

public:
	DocumentSelection()
		: start(0)
		, length(0)
	{}
};

/**
	@brief
*/
class Document
	: public Object
{
public:
	Document();
	virtual ~Document();
	void initialize();

	// ���݂̓��e�����ׂĔj�����āA�V����1�� Paragraph �����
	void setText(const StringRef& text);

LN_INTERNAL_ACCESS:
	void replace(int offset, int length, const StringRef& text);

private:
	void replace(int offset, int length, const UTF32* text, int len);

	DocumentsManager*		m_manager;
	List<RefPtr<Block>>		m_blockList;
};

/**
	@brief
*/
class TextElement
	: public Object
	, public ILayoutElement
{
public:

	/** �t�H���g�t�@�~������ݒ肵�܂��B*/
	void setFontFamily(const StringRef& value) { m_fontData.Family = value; m_fontDataModified = true; }

	/** �t�H���g�t�@�~�������擾���܂��B*/
	const String& getFontFamily() const { return m_fontData.Family; }

	/** �t�H���g�T�C�Y��ݒ肵�܂��B*/
	void setFontSize(int value) { m_fontData.Size = value; m_fontDataModified = true; }

	/** �t�H���g�T�C�Y���擾���܂��B*/
	int getFontSize() const { return m_fontData.Size; }

	/** �t�H���g�̑����L����ݒ肵�܂��B*/
	void setFontBold(bool value) { m_fontData.IsBold = value; m_fontDataModified = true; }

	/** �t�H���g�̑����L�����擾���܂��B*/
	bool isFontBold() const { return m_fontData.IsBold; }

	/** �t�H���g�̃C�^���b�N�̗L����ݒ肵�܂��B*/
	void setFontItalic(bool value) { m_fontData.IsItalic = value; m_fontDataModified = true; }

	/** �t�H���g�̃C�^���b�N�̗L�����擾���܂��B*/
	bool isFontItalic() const { return m_fontData.IsItalic; }

	/** �t�H���g�̃A���`�G�C���A�X�L����ݒ肵�܂��B*/
	void setFontAntiAlias(bool value) { m_fontData.IsAntiAlias = value; m_fontDataModified = true; }

	/** �t�H���g�̃A���`�G�C���A�X�L�����擾���܂��B*/
	bool isFontAntiAlias() const { return m_fontData.IsAntiAlias; }

	Brush* getForeground() const;


	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer);

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void onFontDataChanged(const FontData& newData);

	// ILayoutElement interface
	virtual const PointF& getLayoutPosition() const override;
	virtual Size getLayoutSize() const override;
	virtual const ThicknessF& getLayoutMargin() const override;
	virtual const ThicknessF& getLayoutPadding() const override;
	virtual AlignmentAnchor getLayoutAnchor() const override;
	virtual HAlignment getLayoutHAlignment() const override;
	virtual VAlignment getLayoutVAlignment() const override;
	virtual void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const override;
	virtual ILayoutElement* getLayoutParent() const override;
	virtual const HAlignment* getLayoutContentHAlignment() override;
	virtual const VAlignment* getLayoutContentVAlignment() override;
	virtual const Size& getLayoutDesiredSize() const override;
	virtual void setLayoutDesiredSize(const Size& size) override;
	virtual void setLayoutFinalLocalRect(const Rect& rect) override;
	virtual const Rect& getLayoutFinalLocalRect() const override;
	virtual void setLayoutFinalGlobalRect(const Rect& rect) override;
	virtual int getVisualChildrenCount() const override;
	virtual ILayoutElement* getVisualChild(int index) const override;
	virtual int getLayoutColumn() const override;
	virtual int getLayoutRow() const override;
	virtual int getLayoutColumnSpan() const override;
	virtual int getLayoutRowSpan() const override;

	// ILayoutElement interface
	virtual Size measureOverride(const Size& constraint) override;

LN_CONSTRUCT_ACCESS:
	TextElement();
	virtual ~TextElement();
	void initialize();

LN_INTERNAL_ACCESS:
	DocumentsManager* getManager() const { return m_manager; }
	virtual InternalTextElementType getInternalTextElementType() const;
	void setParent(TextElement* parent) { m_parent = parent; }
	TextElement* getParent() const { return m_parent; }
	const Size& getDesiredSize() const { return m_desiredSize; }
	const Size& getRenderSize() const { return m_finalLocalRect.getSize(); }

private:
	DocumentsManager*		m_manager;
	FontData				m_fontData;
	RefPtr<Brush>			m_foreground;
	bool					m_fontDataModified;

	// layout data
	PointF					m_position;
	Size					m_size;
	ThicknessF				m_margin;
	ThicknessF				m_padding;
	AlignmentAnchor			m_anchor;
	HAlignment				m_horizontalAlignment;
	VAlignment				m_verticalAlignment;
	TextElement*			m_parent;
	Size					m_desiredSize;
	Rect					m_finalLocalRect;
	Rect					m_finalGlobalRect;
	detail::GridLayoutInfo	m_gridLayoutInfo;
};

/**
	@brief	�R���e���c���O���[�v�����Ēi���ɂ��邽�߂Ɏg�p�����B
*/
class Block
	: public TextElement
{
public:
	Block();
	virtual ~Block();
	void initialize();

	void addInline(Inline* inl);
	void clearInlines();

	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	void insertInlines(int index, const List<RefPtr<Inline>>& inlines);
	const List<RefPtr<Inline>>& getInlines() const { return m_inlines; }

private:
	List<RefPtr<Inline>>	m_inlines;
};

/**
	@brief	�R���e���c���O���[�v�����Ēi���ɂ��邽�߂Ɏg�p�����B
*/
class Paragraph
	: public Block
{
public:
	Paragraph();
	virtual ~Paragraph();
	void initialize();


private:
};

/**
	@brief
*/
class Inline
	: public TextElement
{
public:
	Inline();
	virtual ~Inline();
	void initialize();

private:
};

/**
	@brief
*/
class run
	: public Inline
{
public:
	run();
	virtual ~run();
	void initialize();
	void initialize(const UTF32* str, int len);

	void setText(const StringRef& text);

protected:
	// TextElement interface
	virtual void onFontDataChanged(const FontData& newData) override;
	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer) override;

	// ILayoutElement interface
	virtual Size measureOverride(const Size& constraint);

private:
	GenericStringBuilderCore<UTF32>	m_text;
	RefPtr<GlyphRun>				m_glyphRun;
};

/**
	@brief
*/
class Span
	: public Inline
{
public:
	Span();
	virtual ~Span();
	void initialize();

private:
	// Inline List
};


/**
	@brief
*/
class LineBreak
	: public Inline
{
public:
	LineBreak();
	virtual ~LineBreak();
	void initialize();

private:
	virtual InternalTextElementType getInternalTextElementType() const;
};




class VisualTextElement
	: public Object
{
public:
	//TextElement*	m_element;
	int				m_documentLength;	// Document ��ł� TextLength

};



// �����s
class VisualLine
	: public Object
{
public:

private:
public:	// TODO:
	List<RefPtr<VisualTextElement>>	m_visualTextElementList;
};


/**
	@brief
	@note	���� float(�u���b�N�̉�����) �Ȃǂ��l���Ȃ��B�Ƃ���ƁAParagraph �P�ʂŃf�[�^���f���g�񂾂ق����킩��₷���B
*/
class VisualBlock
	: public Object
{
public:
	void setParagraph(Paragraph* paragraph);

private:
	void rebuildVisualLineList();

	RefPtr<Paragraph>			m_paragraph;
	List<RefPtr<VisualLine>>	m_visualLineList;
};

//
class DocumentView
	: public Object
{
public:

private:
	void initialize(Document* document);

	Document*	m_document;
};







} // namespace detail
LN_NAMESPACE_END
