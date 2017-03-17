
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

/**
	@brief
*/
class Document
	: public Object
{
public:
	Document();
	virtual ~Document();
	void Initialize(DocumentsManager* manager);

	// ���݂̓��e�����ׂĔj�����āA�V����1�� Paragraph �����
	void SetText(const StringRef& text);

private:
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
	TextElement();
	virtual ~TextElement();
	void Initialize(DocumentsManager* manager);

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
	virtual ILayoutElement* GetLayoutParent() const override;
	virtual const HAlignment* GetLayoutContentHAlignment() override;
	virtual const VAlignment* GetLayoutContentVAlignment() override;
	virtual const Size& GetLayoutDesiredSize() const override;
	virtual void SetLayoutDesiredSize(const Size& size) override;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) override;
	virtual const RectF& GetLayoutFinalLocalRect() const override;
	virtual void SetLayoutFinalGlobalRect(const RectF& rect) override;
	virtual int GetVisualChildrenCount() const override;
	virtual ILayoutElement* GetVisualChild(int index) const override;
	virtual int GetLayoutColumn() const override;
	virtual int GetLayoutRow() const override;
	virtual int GetLayoutColumnSpan() const override;
	virtual int GetLayoutRowSpan() const override;

	// ILayoutElement interface
	virtual Size MeasureOverride(const Size& constraint) override;

LN_INTERNAL_ACCESS:
	DocumentsManager* GetManager() const { return m_manager; }
	virtual InternalTextElementType GetInternalTextElementType() const;
	void SetParent(TextElement* parent) { m_parent = parent; }
	TextElement* GetParent() const { return m_parent; }
	const Size& GetDesiredSize() const { return m_desiredSize; }
	const Size& GetRenderSize() const { return m_finalLocalRect.GetSize(); }

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
	RectF					m_finalLocalRect;
	RectF					m_finalGlobalRect;
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
	void Initialize(DocumentsManager* manager);

	void AddChildElement(TextElement* inl);
	void ClearChildElements();

	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	const List<RefPtr<TextElement>>& GetChildElements() const { return m_childElements; }

private:
	List<RefPtr<TextElement>>	m_childElements;
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
	void Initialize(DocumentsManager* manager);


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
	void Initialize(DocumentsManager* manager);

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
	void Initialize(DocumentsManager* manager);

	void SetText(const StringRef& text);

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
	void Initialize(DocumentsManager* manager);

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
	void Initialize(DocumentsManager* manager);

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
class ParagraphViewer
	: public Object
{
public:
	void SetParagraph(Paragraph* paragraph);

private:
	void RebuildVisualLineList();

	RefPtr<Paragraph>			m_paragraph;
	List<RefPtr<VisualLine>>	m_visualLineList;
};

/**
	@brief
*/
class DocumentViewer
	: public Object
{
public:

private:
};







} // namespace detail
LN_NAMESPACE_END
