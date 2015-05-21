/*


�Evirtual �֐�
	�EBinderLib �̊֐����R�[���o�b�N�ŌĂяo���B(���̌Ăяo����� BinderLib �̊֐��� Ploxy �֐��ƌĂ�)
	�EPloxy �֐��� static �֐��ł��邱�ƁB(C# �̏ꍇ�A�����o���\�b�h����GC�Ώۂ���O�����̏������K�v�Ŗʓ|)
	�EPloxy �֐��̌^�� LFTypedef.h �ɒ�`(typedef)����֐��|�C���^�^�Ɠ����ł��邱�ƁB
	  �܂�ARefObject ���󂯎��ꍇ�� APILib �������Ă��� Handle ���󂯎�邱�ƂɂȂ�B
	�EPloxy �֐��� Handle ���� this �ɂ����� Wrapper �I�u�W�F�N�g���O���[�o���� Manager ���猟�����A
	  �ړI�̃����o���\�b�h���Ăяo���BPloxy �֐��� virtual �֐��� 1:1 �ŌĂяo���B
	  �܂�Avirtual �֐��̐����� Ploxy �֐�����`����邱�ƂɂȂ�B
	  
	�EAPILib �́A���J����S�Ă� RefObject �N���X�ɑ΂��āA�T�u�N���X����������B(���̃T�u�N���X�� Ploxy �N���X�ƌĂ�)
	  APILib ������J����钼�ڂ̃I�u�W�F�N�g�́A���� Ploxy �ł���B
	�EPloxy �N���X�́ACoreLib �N���X�����J����S�Ă� virtual �֐����I�[�o�[���C�h����B(�X�[�p�[�N���X���܂߂�)
	  �Ⴆ�΁ASprite �� SceneNode ���p�����邪�A���̂Ƃ��T�u�N���X SpritePloxy �� DrawSubset() ����������B
	�EPloxy �̓����o�ϐ��� LFTypedef.h �ɒ�`(typedef)����֐��|�C���^�^�̃����o�ϐ������B
	  BinderLib ���ɒ�`���� Ploxy �֐��̃|�C���^�́A�����ɐݒ肳���B
	�E�I�[�o�[���C�h�̎����́A���̊֐��|�C���^���o�R���� Ploxy �֐����Ăяo���B
	�E���J API �֐��́Asprite->DrawSubset() �̂悤�Ɋ֐����ĂԂ̂ł͂Ȃ��A
	  sprite->Sprite::DrawSubset() �̂悤�ɁACoreLib �̃N���X�����w�肵�ČĂяo���B
	  �������Ȃ��ƁA�����ċN�Ăяo���Ɋׂ�B
	  
	
�E�R�[���o�b�N�֐�
	�E��{���j�� virtual �֐��Ɠ������APloxy �N���X�𗘗p������@���g���B
	
	�ECoreLib �Ɏ�������R�[���o�b�N�� setter ���̈����^�� Delegate �N���X���g�p���Ă悢�B
	  �Ⴆ�΁AArrayList::Sort(Delegate02<T, T> pred) �� XmlParser::SetFoundElementCallback(Delegate01<XmlElement> callback) �Ƃ��Ē�`���A
	  Ploxy �N���X�̊֐����Z�b�g����B
	
	
	�E�e���v���[�g�^�̃R�[���o�b�N�́H
	  �^�� RefObject ���p�����Ă��鐧���ɂ���B

�Eevent

�E���b�Z�[�W�L���[
�E�I�u�W�F�N�g�z��
�E�\���̔z��
	�E���_�z��ADrawLines ���B
	
	�E�ʒ�`�����������BPointList �݂����ɁB(C# �� PointCollection) �Ƃ��B
		�E���� List �� RefObject �ł���ق������R�ȋC������B
			�E��������ƁACoreLib �����ł� Point[] �ł͂Ȃ� PointList �N���X������Ď����Ă����ׂ��B

	�E���_�o�b�t�@�̂悤�ȃJ�X�^���^�\���̔z��
		�ESlimDX �� VertexBuffer.Lock �� DataStream �Ƃ����N���X��Ԃ��B
		  ����� SetRange() �ɍ\���̔z���n���A�������R�s�[���Ă���B
		�EXNA �� VertexBuffer.SetData() �ō\���̔z���n���B
		�E��2�͍\���̔z���O��ɂ��Ă���̂� Ruby �Ƃ��Ɏ����Ă����ɂ͂��܂�����Ȃ������E�E�E�B
		
		�ERefObjectList �̑΂ɂȂ� StructList ���K�v��������Ȃ��B
			�ERefObjectList �͎Q�ƃJ�E���g�����삷��List�B
			�EStructList �͎��̃R�s�[���s�� List�B
			  �o�C���_�ł͌^�����Ɏw�肳�ꂽ�^�̃C���X�^���X�� new ���ĕԂ��B
			
		�E���_�o�b�t�@�͓���ȃ��X�g�ɂ��邵���Ȃ������B
		  �܂��߂ɂ��Ȃ�A�o�C���_���� set ���悤�Ƃ��Ă���z��̒��g���S�� LNote �̍\���̌^�ł��邱�ƂƁA
		  �����̃t�B�[���h�����ׂč\���̌^�ł��邱�Ƃ��`�F�b�N���Ȃ���΂Ȃ�Ȃ��B
		


-------------------------------------------------------------------------------
�� UI�f�U�C���p�^�[���H

	MVC �Ƃ� MVVM �Ƃ��A����̃p�^�[���͑z�肵�Ȃ��B
	�^�C�g���ɂ���Ď����p�^�[���͂��Ȃ�ς��͂��Ȃ̂ŁA
	�l�C�e�B�u��WinAPI �Ƃ� gtk �Ƃ��A�ق�ƂɒP���ȃE�B���h�E�V�X�e���Ƃ��Ă܂Ƃ߂�B
	

-------------------------------------------------------------------------------
�� ��������XAML�Ȃ�ĕK�v�Ȃ́H

	��肽�����Ƃ�
	�E�O�ς̃X���[�Y�ȕύX�B
	�EItemTemplate
	�EUI�p�[�c�̃��C�A�E�g��XML�Œ�`�������B
	�E�r�w�C�r�A�������������B
	
	���3�̓R�[�h�ł��ł��邪�E�E�E�B
	
	���ɂ�肽���̂� ItemTemplate�B
	�Ƃ������A�f�t�H���g�ŗp�ӂ���Ă���R���g���[���̓����A�C�e����ς������B
	�c���AMVVM����Ȃ��̂Ƀ\�[�X�f�[�^�ǂ�����H
	�� ���ʂ� ListBox �Ȃ� string �� Add ���Ă������A����� Control �� Add ����悤�ɂ���B
	   (Control�u���v���H)
	
	���z���́H
	����΂�����̓R�[���o�b�N�K�{�ɂȂ肻���B
	WPF�I�ɂ�낤�Ƃ����� UIObject.SetValue("prop", "�l") �݂����Ȃ��Ƃ��āA
	UIObject �̃R���N�V���������Ȃ��ƃ_���B
	�m�� ToolKit EX �� DataGrid ���AVirtualizationCollectionView �� �R�[���o�b�N���������c�B
	�e����p�ɃR�[���o�b�N�̎d�g�݂𐮂��āA���̂����ōl�������������Ǝv���B
	
	WPF �� Modarn-UI �݂����ɁA�e�[�}�؂�ւ����ɃA�j���[�V�������������B
	������̓R�A���̋@�\�B�ύX�̂��������\�[�X���ɃX�g�[���[�{�[�h��}������B
	
	

-------------------------------------------------------------------------------
�� �e�v���O��������ł̃R�[���o�b�N�������@

	

	[2014/11/27] �߂�
	Ruby �� C# �� GC �̓s���ŁAdelegate �Ƃ��������� C ���Ŏ����Ă������Ƃ͂ł��Ȃ��B
	�i�撣��΂ł��邪�c�j
	���AC�ɃR�[���o�b�N�֐��n���Ȃ�����ł͓����d�g�݂��g�����Ƃ��ł��Ȃ��B
	
	�O��c
	PeekEvent �̃��b�Z�[�W���[�v�͂��ꂼ��̌���Ŏ������A
	������C�x���g�Ɋ�Â��ă��\�b�h�Ăяo�����胉�x���W�����v�����肷��B
	�R�[���o�b�N�̌Ăяo���͂��ꂼ��̌���̖�ڂƂ���B
	
	��������
	���݂̌���o�C���_�́A�Ⴆ�� ViewPane.Layers �v���p�e�B�̂悤�ɁA
	get �����Ƃ��� C �I�u�W�F�N�g�ɑ΂��� Managed �C���X�^���X�� "�t�B�[���h��" �����Ă��Ȃ����
	���̏�Ń��b�v���邽�߂� Managed �C���X�^���X�����B
	�܂�A�ʁX�� Managed �C���X�^���X 2 ������ C �I�u�W�F�N�g���w�����Ƃ�����B
	
	1��1�őΉ�������ɂ́A���x LNote::C_API �ł���Ă�Q�ƊǗ��Ƃقړ������Ƃ��Ȃ��ƂȂ�Ȃ��B
	�O���[�o���Ȕz��Ȃ�map�Ȃ�ŁAID�� Managed �I�u�W�F�N�g��S�ĊǗ�����B
	
	�|�C���g�͍폜�̃^�C�~���O�B
	�o�C���_���ɃR�[���o�b�N�ł��Ȃ��O�񂾂ƁAC �I�u�W�F�N�g�� C �I�u�W�F�N�g��
	delete ����̂͌��ցBManaged �I�u�W�F�N�g�������Ă��鎞�͕K�� C �I�u�W�F�N�g�͐����Ă��Ȃ��ƃ_���B
	�Ⴆ�΁u�q�v�f������delete�v�݂����Ȃ̂͐�΃_���B(�Q�ƃJ�E���g�f�N�������g�Ȃ�OK)
	
	��{�͈ȉ���2���B
	�EDispose �� C�I�u�W�F�N�g�� Release + �O���[�o��map���� this ���폜
	�E�O���[�o�� map �� Value �� WeakReference �Ŏ��B
	
	���O�҂́~�BRelease ���Ă��ASceneNode �Ƃ��͂܂��e�m�[�h����̎Q�Ƃ��c���Ă��邩������Ȃ��B
	
	
	Managed �I�u�W�F�N�g�� �Q�Ƃ��O���Ă����̂́AC_APIManager �̊Ǘ����X�g���� Remove ���ꂽ��B(C�����m���� delete ���ꂽ��)
	����܂ł� Managed ���̊Ǘ����X�g�ɂ����Ǝ����Ă����K�v������B(�Q�Ƃ������Ă����K�v������)
	�܂�AWeakRef �ł��_���B�t�@�C�i���C�U�� Release ���ł��Ȃ��B
	
	
	
	XML ���� C# �ō�����N���X�̃C���X�^���X�����ɂ́H
	XML�̃p�[�T���C���X�^���X����肽���Ƃ��́c �R�[���o�b�N�����Ȃ��B
	���̃R�[���o�b�N (C#)�ł́AManaged �C���X�^���X������āA�O���[�o���ȊǗ��z��ɓ���Ă����B
	
	�c�ꍇ�ɂ���ẮAC++Core ���ŉ��� new �����炻�̓s�x callback �ĂԂ悤�Ȏd�g�݂����������������̂�������Ȃ��B
	

-------------------------------------------------------------------------------
�� WPF �� Visual�AUIElement�AFramworkElement�AControl�AShape �̈Ⴂ

�E�p���֌W�͂���Ȋ����B
	Control < FramworkElement < UIElement < Visual
	Shape   < FramworkElement < UIElement < Visual
	
�EVisual �͈ȉ��̂悤�ȗv�f������
	�E�����x
	�E�N���b�s���O�̈�
	�EVisual �q�v�f
	�E�q�b�g�e�X�g
	�ETransform
	�EViewport3DVisual��DrawingVisual(�����N���X�H)�ɔh�����Ă���B

�EUIElement
	�E���C�A�E�g�Ɋւ����{��� (measuer ���\�b�h)
	�EMouseDown �� DragDrop ���قڋ��ʂ̃C�x���g�͂����B
	�EMSDN��ł͔h���� FramworkElement �̂�

�EFramworkElement
	�EAcutualHeight�AMaxHeight SizeChanged �C�x���g���A�T�C�Y�Ɋւ����
	�EDataContext
	�EStyle
	�EToolTip

�EControl �͈ȉ��̂悤�ȗv�f������
	�E�O�ς̐F (�w�i�F��)
	�E�t�H���g
	�ETabIndex
	�E���� Control �ɓK�p���Ă��� ControlTemplate
	

-------------------------------------------------------------------------------
��



�E�I�u�W�F�F�y���X�g
�E�\���̃��X�g (���_�o�b�t�@�Ƃ�)
�E�K�w�\��












=======



-------------------------------------------------------------------------------
�� WPF �� Visual�AUIElement�AFramworkElement�AControl�AShape �̈Ⴂ

�E�p���֌W�͂���Ȋ����B
	Control < FramworkElement < UIElement < Visual
	Shape   < FramworkElement < UIElement < Visual
	
�EVisual �͈ȉ��̂悤�ȗv�f������
	�E�����x
	�E�N���b�s���O�̈�
	�EVisual �q�v�f
	�E�q�b�g�e�X�g
	�ETransform
	�EViewport3DVisual��DrawingVisual(�����N���X�H)�ɔh�����Ă���B

�EUIElement
	�E���C�A�E�g�Ɋւ����{��� (measuer ���\�b�h)
	�EMouseDown �� DragDrop ���قڋ��ʂ̃C�x���g�͂����B
	�EMSDN��ł͔h���� FramworkElement �̂�

�EFramworkElement
	�EAcutualHeight�AMaxHeight SizeChanged �C�x���g���A�T�C�Y�Ɋւ����
	�EDataContext
	�EStyle
	�EToolTip

�EControl �͈ȉ��̂悤�ȗv�f������
	�E�O�ς̐F (�w�i�F��)
	�E�t�H���g
	�ETabIndex
	�E���� Control �ɓK�p���Ă��� ControlTemplate
	

-------------------------------------------------------------------------------
�� ����o�C���_ (GUI�ɂ�����炸�S�̂ɂ���)

�����b�v�I�u�W�F�N�g�̓o�C���_���ň�ӂ�ID�����蓖�āA�S�Ẵ��b�v�I�u�W�F�N�g���O���[�o���z��ŊǗ�����B
	���� ID �� C�I�u�W�F�N�g�̃��[�U�[�f�[�^�Ƃ��Ċ��蓖�āAC�I�u�W�F�N�g����Ή����郉�b�v�I�u�W�F�N�g����肷�邽�߂Ɏg�p����B
	�ȉ��̂悤�ȏ����ŕK�v�ɂȂ�B
	�E�C�x���g�n���h�� (�R�[���o�b�N)
	�Eget/set�ő΂ƂȂ�v���p�e�B�ȊO�̃��[�g����� get (�V�F�[�_�ϐ��ւ� set �� get ��)

	������
		���b�v�I�u�W�F�N�g �� C�I�u�W�F�N�g ��1�����݂̂̎Q�Ƃŗǂ���΁A��ӂ�ID�Ƃ��K�v�Ȃ��B
		�������AGUI���W���[���p�̃C�x���g�n���h���̎����ŕK�v�ɂȂ����B


-------------------------------------------------------------------------------
�� UI�v�f�̃x�[�X�I�u�W�F�N�g





-------------------------------------------------------------------------------
�� Behavior


-------------------------------------------------------------------------------
�� NumUpDown �����[�U�[�R���g���[���Ƃ��č���Ă݂�B

�� C#
	class NumUpDown : UserControl
	{
		TextBox _textBox;
		RepeatButton _incrButton;
		RepeatButton _decrButton;
	
		public NumUpDown()
		{
			_textBox = new TextBox();
		}
	};




-------------------------------------------------------------------------------
�� ���ʂ� RepertButton ���e����Ń��[�U�[�R���g���[���Ƃ��č���Ă݂�B
�E���̗�� RepertButton �̓���
	�E�w�i�F�ƕ������ݒ�ł���B(������͎q�v�f)
	�E�����p�� Border ��p�ӂ���B
	�Eprivate �t�B�[���h�I�� _updateFrames ������ (�ق�Ƃ͎��ԒP�ʂ̕����������ǁA�����ł͗�Ƃ���)
	�EClicked �C�x���g�����삷��B
	�E�}�E�X���N���b�N�ŉ�����ԂɂȂ�A���̌�}�E�X�{�^���𗣂��� Click �C�x���g����������B
	�E���[�U�[�R���g���[���Ƃ��� GUI �V�X�e���ɓo�^���A�ė��p�ł���悤�ɂ���B

�� C����
	// private �ȏ�Ԃ�ێ����郆�[�U�[�f�[�^�B
	// C++ class ���� private �����o�ϐ��ɂ�����B
	struct MyRepertButtonState
	{
		bool IsPressed;		// �}�E�X�_�E����
	};
	
	void main()
	{
		// ���[�U�[�R���g���[���Ƃ��čė��p�ł���悤�Ƀe���v���[�g�Ƃ��� GUI �V�X�e���ɓo�^����B
		
		lnHandle myRepertButtonTemplate;
		LNUIControlTemplate_Create(&myRepertButtonTemplate);
		
		// �g����4px�Ԙg
		lnHandle borderFactory;
		LNUIElementFactory_Create(&borderFactory, "Border");
		LNUIObject_SetName(&borderFactory, "_border");			// "_border" �Ƃ������O��t����
		LNUIElementFactory_SetValue(&borderFactory, "BorderThickness", "4");
		LNUIElementFactory_SetValue(&borderFactory, "BorderBrush", "Red");
		LNUIControlTemplate_AddChild(myRepertButtonTemplate, borderFactory);
		
		// ContentPresenter (����� RepertButton �� ContentControl �Ƃ��č�邽�߁A1�K�v)
		lnHandle contentPresenterFactory;
		LNUIElementFactory_Create(&contentPresenterFactory, "ContentPresenter");
		LNUIElementFactory_SetValue(&contentPresenterFactory, "HorizontalAlignment", "Center");	// ���̗v�f�𒆉�����
		LNUIElementFactory_AddChild(borderFactory, contentPresenterFactory);			// Border �̎q�Ƃ��Ēǉ�
		
		
		LNUIManaer_RegisterUIElement(
			"MyRepertButton",				// ���̖��O�œo�^����
			"ContentControl",			// �x�[�X�v�f�� ContentControl
			myRepertButtonTemplate);		// create ���ɂ��̃e���v���[�g��K�p����
		
		
		lnHandle button;
		LNUIManaer_CreateUIElement(&button, "MyRepertButton");
		
		do
		{
			while (LNUIManager_PeekEvent(&e))
			{
				lnHandle sender;
				LNUIEvent_GetSender(e, &sender);
				
				const char* typeName;
				LNUIElement_GetTypeName(e, &typeName);
				
				if (strcmp(typeName, "MyRepertButton"))
				{
					lnEventType et;
					LNUIEvent_GetEventType(e, &et);
					
					if (et == LN_EVENT_MOUSE_DOWN)
					{
						LNUIEvent_GetEventType(e, &et);
						LNUIElement_GoToVisualState("Pressed");		// Pressed ��Ԃֈړ� (���̖��O�̏�Ԃ�������Ή������Ȃ�)
					}
				}
			}
		
		} while(update):
	}


�� C++

�� C#



-------------------------------------------------------------------------------
�� OnRender

*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>

namespace Lumino
{
namespace GUI
{
	
//=============================================================================
// GUIManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::GUIManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
}

} // namespace GUI
} // namespace Lumino
