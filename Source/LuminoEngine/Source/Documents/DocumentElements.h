
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
	void SetFontFamily(const StringRef& value) { m_fontData.Family = value; m_fontDataModified = true; }

	/** �t�H���g�t�@�~�������擾���܂��B*/
	const String& GetFontFamily() const { return m_fontData.Family; }

	/** �t�H���g�T�C�Y��ݒ肵�܂��B*/
	void SetFontSize(int value) { m_fontData.Size = value; m_fontDataModified = true; }

	/** �t�H���g�T�C�Y���擾���܂��B*/
	int GetFontSize() const { return m_fontData.Size; }

	/** �t�H���g�̑����L����ݒ肵�܂��B*/
	void SetFontBold(bool value) { m_fontData.IsBold = value; m_fontDataModified = true; }

	/** �t�H���g�̑����L�����擾���܂��B*/
	bool IsFontBold() const { return m_fontData.IsBold; }

	/** �t�H���g�̃C�^���b�N�̗L����ݒ肵�܂��B*/
	void SetFontItalic(bool value) { m_fontData.IsItalic = value; m_fontDataModified = true; }

	/** �t�H���g�̃C�^���b�N�̗L�����擾���܂��B*/
	bool IsFontItalic() const { return m_fontData.IsItalic; }

	/** �t�H���g�̃A���`�G�C���A�X�L����ݒ肵�܂��B*/
	void SetFontAntiAlias(bool value) { m_fontData.IsAntiAlias = value; m_fontDataModified = true; }

	/** �t�H���g�̃A���`�G�C���A�X�L�����擾���܂��B*/
	bool IsFontAntiAlias() const { return m_fontData.IsAntiAlias; }

	Brush* GetForeground() const;


	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer);

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void OnFontDataChanged(const FontData& newData);

	// ILayoutElement interface
	virtual const PointF& GetLayoutPosition() const override;
	virtual Size GetLayoutSize() const override;
	virtual const ThicknessF& GetLayoutMargin() const override;
	virtual const ThicknessF& GetLayoutPadding() const override;
	virtual AlignmentAnchor GetLayoutAnchor() const override;
	virtual HAlignment GetLayoutHAlignment() const override;
	virtual VAlignment GetLayoutVAlignment() const override;
	virtual void GetLayoutMinMaxInfo(Size* outMin, Size* outMax) const override;
	virtual ILayoutElement* GetLayoutParent() const override;
	virtual const HAlignment* GetLayoutContentHAlignment() override;
	virtual const VAlignment* GetLayoutContentVAlignment() override;
	virtual const Size& GetLayoutDesiredSize() const override;
	virtual void SetLayoutDesiredSize(const Size& size) override;
	virtual void SetLayoutFinalLocalRect(const Rect& rect) override;
	virtual const Rect& GetLayoutFinalLocalRect() const override;
	virtual void SetLayoutFinalGlobalRect(const Rect& rect) override;
	virtual int GetVisualChildrenCount() const override;
	virtual ILayoutElement* GetVisualChild(int index) const override;
	virtual int GetLayoutColumn() const override;
	virtual int GetLayoutRow() const override;
	virtual int GetLayoutColumnSpan() const override;
	virtual int GetLayoutRowSpan() const override;

	// ILayoutElement interface
	virtual Size MeasureOverride(const Size& constraint) override;

LN_CONSTRUCT_ACCESS:
	TextElement();
	virtual ~TextElement();
	void initialize();

LN_INTERNAL_ACCESS:
	DocumentsManager* GetManager() const { return m_manager; }
	virtual InternalTextElementType GetInternalTextElementType() const;
	void SetParent(TextElement* parent) { m_parent = parent; }
	TextElement* GetParent() const { return m_parent; }
	const Size& GetDesiredSize() const { return m_desiredSize; }
	const Size& GetRenderSize() const { return m_finalLocalRect.getSize(); }

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

	void AddInline(Inline* inl);
	void ClearInlines();

	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	void InsertInlines(int index, const List<RefPtr<Inline>>& inlines);
	const List<RefPtr<Inline>>& GetInlines() const { return m_inlines; }

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
class Run
	: public Inline
{
public:
	Run();
	virtual ~Run();
	void initialize();
	void initialize(const UTF32* str, int len);

	void setText(const StringRef& text);

protected:
	// TextElement interface
	virtual void OnFontDataChanged(const FontData& newData) override;
	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer) override;

	// ILayoutElement interface
	virtual Size MeasureOverride(const Size& constraint);

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
	virtual InternalTextElementType GetInternalTextElementType() const;
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
	void SetParagraph(Paragraph* paragraph);

private:
	void RebuildVisualLineList();

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
