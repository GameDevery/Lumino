
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Imaging/Font.h>
#include "SpriteRendererImpl.h"

namespace Lumino
{
namespace Graphics
{
namespace detail
{

/*
	�O���t�L���b�V�� (�e�N�X�`��) �͊i�q��̃Z���̂悤�ɗ̈�𕪊����A
	�O���t�r�b�g�}�b�v��ۑ����Ă����B
	�Z�����̃r�b�g�}�b�v�͂��ׂč��㑵���Ƃ��Ă���B
*/
class TextRendererImplemented
	: public RefObject
{
public:
	// �R�}���h���o�b�`�����邽�߂ɐݒ���O���[�v������
	struct FontInfo
	{
		RefPtr<Imaging::Font> Font;
		Color	ForeColor;
		Color	StrokeColor;
		int		StrokeSize;
	};
	// �R�}���h���o�b�`�����邽�߂ɐݒ���O���[�v������
	struct LayoutInfo
	{
		TextAlignment	Alignment;
		TextTrimming	Trimming;
		FlowDirection	Direction;
		Rect			DrawingArea;	// ����Ȃ�����
	};
	// �R�}���h���o�b�`�����邽�߂ɐݒ���O���[�v������
	struct CharInfo
	{
		UTF32	Ch;
		Vector3	Position;
	};

public:
	TextRendererImplemented(GraphicsManager* manager);
	~TextRendererImplemented();

public:
	void SetTransform(const Matrix& matrix) { m_spriteRenderer->SetTransform(matrix); }
	void SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize) { m_spriteRenderer->SetViewProjMatrix(view, proj); m_spriteRenderer->SetViewPixelSize(viewPixelSize); }

	void SetFontInfo(const FontInfo& info);
	void SetLayoutInfo(const LayoutInfo& info);

	void DrawText(const UTF32* text, int length, bool takeOverKerning);	// ��A�̕������`�悷��B���̃N���X�����Ń��C�A�E�g���s���B
	void DrawChar(UTF32 ch, const Rect& area);
	void DrawChars(CharInfo* chars, int count);		// ���Ɉʒu�̌��܂��Ă��镶���̕`��Ɏg��
	void Flash();

private:
	struct CachedGlyphInfo
	{
		int		Index;
		Size	Size;
	};
	void DrawSprite(const CachedGlyphInfo& info, int x, int y);
	void Reset();

private:
	typedef std::map<UTF32, CachedGlyphInfo>	CachedGlyphInfoMap;

	RefPtr<SpriteRendererImpl>	m_spriteRenderer;
	Imaging::Font*		m_font;
	Color				m_foreColor;
	Color				m_strokeColor;
	int					m_strokeSize;
	TextAlignment		m_textAlignment;
	TextTrimming		m_textTrimming;
	FlowDirection		m_flowDirection;
	Rect				m_drawingArea;

	RefPtr<Device::ITexture>	m_glyphCacheTexture;
	CachedGlyphInfoMap	m_cachedGlyphInfoMap;
	int					m_glyphWidthCount;
	Size				m_glyphMaxBitmapSize;	///< ���݂̃t�H���g����1�������̃r�b�g�}�b�v�̍ő�T�C�Y
	Stack<int>			m_indexStack;			///< �󂫃L���b�V���C���f�b�N�X

public:
	class TextRendererCommandBase : public RenderingCommand
	{
	protected:
		TextRendererImplemented*	m_renderer;

		virtual ~TextRendererCommandBase()
		{
			LN_SAFE_RELEASE(m_renderer);
		}
	};

	class SetTransformCommand : public TextRendererCommandBase
	{
		Matrix	m_transform;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const Matrix& transform)
		{
			HandleCast<SetTransformCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetTransformCommand>(cmd)->m_transform = transform;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetTransform(m_transform);
		}
	};

	class SetViewProjectionCommand : public TextRendererCommandBase
	{
		Matrix	m_view;
		Matrix	m_proj;
		Size	m_size;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const Matrix& view, const Matrix& proj, const Size& size)
		{
			HandleCast<SetViewProjectionCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetViewProjectionCommand>(cmd)->m_view = view;
			HandleCast<SetViewProjectionCommand>(cmd)->m_proj = proj;
			HandleCast<SetViewProjectionCommand>(cmd)->m_size = size;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetViewProjection(m_view, m_proj, m_size);
		}
	};

	class SetFontInfoCommand : public TextRendererCommandBase
	{
		FontInfo	m_info;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const FontInfo& info)
		{
			HandleCast<SetFontInfoCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetFontInfoCommand>(cmd)->m_info = info;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetFontInfo(m_info);
		}
	};

	class SetLayoutInfoCommand : public TextRendererCommandBase
	{
		LayoutInfo	m_info;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const LayoutInfo& info)
		{
			HandleCast<SetLayoutInfoCommand>(cmd)->m_renderer = renderer;
			HandleCast<SetLayoutInfoCommand>(cmd)->m_info = info;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->SetLayoutInfo(m_info);
		}
	};

	class DrawTextCommand : public TextRendererCommandBase
	{
		size_t	m_stringHandle;
		int m_len;
		bool m_takeOverKerning;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const UTF32* text, int len, bool takeOverKerning)
		{
			size_t dataHandle = Alloc(cmd, sizeof(UTF32) * len, text);
			HandleCast<DrawTextCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawTextCommand>(cmd)->m_stringHandle = dataHandle;
			HandleCast<DrawTextCommand>(cmd)->m_len = len;
			HandleCast<DrawTextCommand>(cmd)->m_takeOverKerning = takeOverKerning;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawText((UTF32*)commandList->GetBuffer(m_stringHandle), m_len, m_takeOverKerning);
		}
	};

	class DrawCharsCommand : public TextRendererCommandBase
	{
		size_t	m_infoListHandle;
		int m_count;
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer, const Array<CharInfo>& infoList)
		{
			size_t dataHandle = Alloc(cmd, sizeof(CharInfo) * infoList.GetCount(), &infoList[0]);
			HandleCast<DrawCharsCommand>(cmd)->m_renderer = renderer;
			HandleCast<DrawCharsCommand>(cmd)->m_infoListHandle = dataHandle;
			HandleCast<DrawCharsCommand>(cmd)->m_count = infoList.GetCount();
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->DrawChars((CharInfo*)commandList->GetBuffer(m_infoListHandle), m_count);
		}
	};

	class FlashCommand : public TextRendererCommandBase
	{
	public:
		static void Create(CmdInfo& cmd, TextRendererImplemented* renderer)
		{
			HandleCast<FlashCommand>(cmd)->m_renderer = renderer;
			LN_SAFE_ADDREF(renderer);
		}
		virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
		{
			m_renderer->Flash();
		}
	};
};

} // namespace detail
} // namespace Graphics
} // namespace Lumino
