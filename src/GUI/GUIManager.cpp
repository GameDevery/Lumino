/*
	[2015/5/31]
		�������� VisualTree �͉��ŕK�v�Ȃ́H
		�� ��ԍŏ��� GUI �݌v�Ŏ��s�����A�u�E�B���h�E�̃V�X�e���{�^���v��������g���Ƃ��������R�ɍ���B


		Template �Ƃ��A���\�[�X�V�X�e���̖ړI�́H
		�����ƃ_���Ȃ́H�����ł��Ȃ��Ȃ�́H
		�� �Ԃ����Ⴏ�����Ă����Ƃ��Ȃ�B
			���ۂɕK�v�ɂȂ�̂̓e�[�}�ύX���������ĂȂ��������炢�����B
			
			���ƁA�f�t�H���g�� VisualTree �̃��C�A�E�g��ύX�������Ƃ��B
			�R�����R���g���[����ύX���������Ă����̂͂قƂ�ǂȂ��Ǝv���BUI�f�U�C���̊w�K�R�X�g�I�ɁB
			�ύX����Ƃ�����A�Q�[���ɂ͋��ʂł��邯�ǃ^�C�g�����ƂɃf�U�C���̈Ⴄ���B�Ⴆ�Α����X���b�g�Ƃ��ɂȂ邩�Ȃ��c�B
			������������� GUI ���W���[���̃R�����R���g���[���Ƃ���Ȃ�Ӗ��������Ă���B
			�ł��A������ň� VisualTree �𒼐ڂ�����΂����������E�E�E�B







	[2015/5/31]
		�Ή�����r�n�C���h�R�[�h�̂���A�����镁�ʂ�UI�p�[�c�̔z�u�Ɏg�� XML �́A�u���C�A�E�g�v�Ƃ��������������Ă݂�B
		x:Class �����������Ă����肷��B

		�R���g���[������������闬��́A

		
		�Enew Window()
		�E���C�A�E�g���� LogicalTree �̃C���X�^���X�����B
			�E�q�v�f�� <Button> �Ƃ������ GUIManager::CreateUIElement("Button")
		�EUpdateTemplate() ���ċA�I�Ɏ��s���Ă����B(VisualTree �����)
			�֐��̈����ɂ� ResourceDictionary ��n���B




	[2015/5/31] VisualTree �� UIElement �ł��邩�̋��

		�EControlTemplate �����������̂� VisualTree�B


	[2015/5/29] �e�[�}�̓��I�ύX
		
		�������S�� MVVM �Ŏ���������Ă���Ȃ�AView �����[�g����S�č�蒼���Ă��܂��Ηǂ��B
		�E�E�E���A���S�� MVVM �͌����I�ł͂Ȃ��E�E�E�B
		�uVisualTree���č\�z����v�Ƃ��������ōl���Ă����B
		
		VisualTree �� UIElement �͑S�� Release() �����B
		�΂��āALogicalTree �� UIElement �͎c�葱����B
		�����e�[�}�̓��I�ύX���ł���悤�ɂ���Ȃ�ALogicalTree �ɂ͕`��Ɋ֌W����I�u�W�F�N�g����ؒu���Ȃ��B
		�E�E�E�悤�ɂ���K�v�͖������B
		�u���V�̐F���炢��������



	[2015/5/29] Template �ƌ���o�C���_�ƃ��[�U�[�R���g���[��

		C# ���� Control ���p���������[�U�[�R���g���[����������Ƃ���B
		����� XML ��ɏ������Ƃ͂ł���H

		�� C++ �����m��Ȃ��V�����^��o�^����Ƃ������ƂɂȂ�B���I�Ȍ^���̒ǉ��B
		�� �܂��́AC++ ���� new �������Ƃ��ɃN���X�����g���ăt�@�N�g���֐����R�[���o�b�N����B
		   C# ���̓T�u�N���X�� new ���� Control �� Handle ��Ԃ������B
		   Managed �C���X�^���X�̓O���[�o���Ǘ��z��ɓ���Ă����B���ꂾ���ł��Ԃ�OK



	[2015/5/29] �A�j���[�V����

		Animation ���W���[���̓��e�͊�{�I�� GUI ���Ń��b�v���� (AnimationTimeline �̃T�u�N���X)
		����� XML ���炢�낢���`�ł���悤�ɂ��邽�߁B

		WPF �̃A�j���[�V�����͂���Ȋ����B
		   System.Windows.Media.Animation.AnimationTimeline
              System.Windows.Media.Animation.BooleanAnimationBase
              System.Windows.Media.Animation.ByteAnimationBase
              System.Windows.Media.Animation.CharAnimationBase
              System.Windows.Media.Animation.ColorAnimationBase
              System.Windows.Media.Animation.DecimalAnimationBase
              System.Windows.Media.Animation.DoubleAnimationBase
              System.Windows.Media.Animation.Int16AnimationBase
              System.Windows.Media.Animation.Int32AnimationBase
              System.Windows.Media.Animation.Int64AnimationBase
              System.Windows.Media.Animation.MatrixAnimationBase
              System.Windows.Media.Animation.ObjectAnimationBase
              System.Windows.Media.Animation.Point3DAnimationBase
              System.Windows.Media.Animation.PointAnimationBase
              System.Windows.Media.Animation.QuaternionAnimationBase
              System.Windows.Media.Animation.RectAnimationBase
              System.Windows.Media.Animation.Rotation3DAnimationBase
              System.Windows.Media.Animation.SingleAnimationBase
              System.Windows.Media.Animation.SizeAnimationBase
              System.Windows.Media.Animation.StringAnimationBase
              System.Windows.Media.Animation.ThicknessAnimationBase
              System.Windows.Media.Animation.Vector3DAnimationBase
              System.Windows.Media.Animation.VectorAnimationBase

		���܂̂Ƃ���L�[�t���[���A�j���[�V�����͍l���Ȃ��ėǂ��Ǝv���B
		2�_�ԂŁA�X�������w��ł���΁B

		AnimationTimeline �͋��L���\�[�X�B
		Storyboard �����L���\�[�X�B

		�EStoryboard::Begin() �� AnimationClock �����BAnimationClock �ɂ̓^�[�Q�b�g�v�f�� AnimationTimeline ���Z�b�g�B
		�EAnimationClock �� GUIManager �ɓo�^�B
		�EGUIManager �ւ� InjectTime() �őS�Ă� AnimationClock ��J�ڂ�����B
		�EAnimationClock �͌��݂̒l���^�[�Q�b�g�ɃZ�b�g����B

		���Ȃ݂� WPF �� AnimationClock ���^�[�Q�b�g�������Ȃ��݂����B
		AnimationClock �����L�ł��ă������g�p�ʌ��邩������Ȃ����ǁA
		�����܂ō�肱�ނƕ��G�ɂȂ肷����C������̂łƂ肠������L���@�Ői�߂�B



	[2015/5/29] �l�C�e�B�u�ȃt���[�e�B���O�E�B���h�E�̕K�v��
	
		�E�E�B���h�E���j���[�h���b�v�_�E��
		�E�R���e�L�X�g���j���[
		�E�R���{�{�b�N�X�̃h���b�v�_�E��
		�E�o���[��
		�EIME
		�E�h�b�L���O�E�B���h�E
		
		�Ⴆ�΃R���{�{�b�N�X�̃h���b�v�_�E���́AVisual �I�Ȑe�q�֌W�͎����Ă��Ȃ��B
		
		�����A�����E�B���h�E���Ɓu�z���g�̃t���X�N���[���v���ł��Ȃ��Ȃ�B
		

	[2015/5/28] RoutedEvent �̕K�v��
		
		�����v������Ԍ��ʂ��������̂� ItemsControl �� ItemClicked ���Ǝv���B
		�Ⴆ�΃N���b�N�����Ƃ��̃n�C���C�g���J�X�^�}�C�Y�������ăe���v���[�g��g�񂾂Ƃ���B
		�P���� ListBox �ł���� ListBoxItem ���N���b�N���ꂽ���A���ڂ̐e�R���g���[���ł��� ListBox ��
		�C�x���g��ʒm����΂悢�B�������A�e���v���[�g��g�ނƕ��G�� VisualTree ���ł��邱�ƂɂȂ�A
		���R���� VisualTree �̒��ł��C�x���g���󂯎�肽���B����ɁA����� Handled=false ��
		�e�R���g���[���֒ʒm����Ă����ׂ��B


	[2015/5/28] ���ʂ̃C�x���g�� RoutedEvent

		<Window x:Class="WpfApplication1.MainWindow"
				xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
				xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
				Title="MainWindow" Height="350" Width="525">
			<StackPanel>
				<Button x:Name="_button1" Height="32"/>
				<Button x:Name="_button2" Height="32"/>
			</StackPanel>
		</Window>

		public partial class MainWindow : Window
		{
			public MainWindow()
			{
				InitializeComponent();
				this._button1.Click += new RoutedEventHandler(Button1_Click_CLR);
				this.AddHandler(Button.ClickEvent, new RoutedEventHandler(Button1_Click_Routed)); 
			}

			private void Button1_Click_CLR(object sender, RoutedEventArgs e)
			{
				Console.WriteLine("Button1_Click_CLR");
				//e.Handled = true;
			} 

			private void Button1_Click_Routed(object sender, RoutedEventArgs e)
			{
				Console.WriteLine("Button1_Click_Routed");
			} 
		}


		Button1_Click_CLR �́A_button1 ���N���b�N�����u�ԂɌĂ΂��B
		���̌�ARoutedEvent �ɂ�肳���̂ڂ��Ă����� Button1_Click_Routed ���Ă΂��B
		���� Button1_Click_CLR �� e.Handled = true; �Ƃ������� Button1_Click_Routed �͌Ă΂�Ȃ��Ȃ�B

		�܂��A_button2 ���N���b�N����� Button1_Click_Routed �����Ă΂��B




	[2015/5/26] Property �̕K�v��
		�����ꍇ�A�}�[�N�A�b�v�p�[�T���� SetValue("Width", 10) �����Ƃ����A
		�v���p�e�B���ɑ΂���l�̊i�[�惁���o�ϐ������߂� if �Ȃ� switch �Ȃ���A
		�T�u�N���X�ɂ��ׂĎ������Ȃ���΂Ȃ�Ȃ��B
		C++ �����Ȃ�܂������A�e����o�C���_�ł�������̂͂�����Ǝ��s���ԓI�ȃR�X�g��������c�B
		�Ȃɂ��߂�ǂ���������B


	[2015/5/21] �`��@�\

		�E�r�b�g�}�b�v�`��
			�E�r�b�g�}�b�v�x�[�X�̘g�`�� (RGSS �E�B���h�E�X�L��)
		�E��`

		�E

	��UIElementTemplate(ControlTemplate) �ƁAUIElementFactory(FrameworkElementFactory) �̈Ⴂ
		�����Ƃ� SetValue �� Style�ATrigger ��ݒ肷�邱�Ƃ��ł�
		�Ȃ�ƂȂ����Ă��邪�A�g�p�ړI�͂��񂺂�Ⴄ�B
		�EUIElementFactory
			�P����1�� UIElement �����B�������Ƃ������B
			�q�v�f��ǉ����邱�ƂŁA�c���[���̂��̂�\����B
			�C���X�^���X���������̂͂��̂܂� Visual�c���[�ɂ�Logical�c���[�ɂ��g����B
		�EUIElementTemplate
			1�̘_���I�ȃR���g���[���̍쐬�ɕK�v�ȏ��̏W���B�Ⴆ�� Button �� Chrome �� TextBlock ���琬�藧���Ă��邪�A
			�g���Ƃ��͂���Ȃ̋C�ɂ��Ȃ��ŁA�ЂƂ� Button �Ƃ��� Visual �܂��� Logical �c���[�Ɍq�����Ƃ��ł���B
			UIElementTemplate ���K�w�\���ɂ��邱�Ƃ͂ł��Ȃ��B
			�y�Ή����� Presenter ���K���K�v�ɂȂ�B�z
	
	��UIElementFactory����̃C���X�^���X��
		UIElementFactory::createUIElement() �Ŏ��g���쐬�B
		�q�v�f Factor ������΁A���ꂼ��� createUIElement() ���ĂсA
		�쐬���ꂽ UIElement ���K�w�\���Ɍ������ĕԂ��B
		UIElement ���쐬����Ƃ��́A���O�ɂ����Manager ���� UIElementTemplate ���擾���A
		UIElementTemplate.createUIElement() ���Ăяo���B
		1�� create ������ UIElementTemplate �� UIElementFactory ���݂��Ⴂ�ɌĂяo����Ă��Ȃ蕡�G�ɂȂ�̂Œ��ӁB
		�yManager �ɃR���g���[���Ƃ��Ē��ړo�^�����̂� UIElementTemplate �ł���z

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
	

	������Ȏ����ɂ���̂́ACoreLib ���̂̎g������𗎂Ƃ������Ȃ�����BDelegate ���g�������B
	  Delegate ���g���Ȃ��Ă��Astatic �֐��ƃ��[�U�[�|�C���^��o�^���邱�ƂŎ����͂��̂��B
	  �������ADelegate ���g���Ȃ��Ƃ���ƁAevent �̊Ǘ������̂������ʓ|�Ȃ��ƂɂȂ�B

	
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
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>
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
	: m_mouseHoverElement(NULL)
	, m_rootPane(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
	LN_SAFE_RELEASE(m_defaultTheme);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::Initialize(const ConfigData& configData)
{
	LN_VERIFY(configData.GraphicsManager != NULL) { return; }

	m_graphicsManager = configData.GraphicsManager;


	RegisterFactory(ContentPresenter::TypeName, ContentPresenter::CreateInstance);
	RegisterFactory(ButtonChrome::TypeName, ButtonChrome::CreateInstance);

	m_defaultTheme = LN_NEW ResourceDictionary();

	// Brush
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("D:/Proj/Lumino/src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(0, 0, 32, 32));
		m_defaultTheme->AddItem(_T("ButtonNormalFrameBrush"), obj);
	}

	// Button
	{
		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("Button"));

		RefPtr<UIElementFactory> ef1(LN_NEW UIElementFactory(this));
		ef1->SetTypeName(_T("ButtonChrome"));
		t->SetVisualTreeRoot(ef1);

		RefPtr<UIElementFactory> ef2(LN_NEW UIElementFactory(this));
		ef2->SetTypeName(_T("ContentPresenter"));
		ef1->AddChild(ef2);

		m_defaultTheme->AddControlTemplate(t);
	}

	m_rootCombinedResource = LN_NEW CombinedLocalResource();
	m_rootCombinedResource->Combine(NULL, m_defaultTheme);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane* GUIManager::CreateRootPane()
{
	m_rootPane = LN_NEW RootPane(this);
	m_rootPane->ApplyTemplate(m_rootCombinedResource);	// �e�[�}�𒼂��ɍX�V
	return m_rootPane;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::RegisterFactory(const String& typeFullName, ObjectFactory factory)
{
	m_objectFactoryMap[typeFullName] = factory;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* GUIManager::CreateObject(const String& typeFullName)
{
	ObjectFactory f = m_objectFactoryMap[typeFullName];
	return f(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseMove(float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, clientX, clientY));
	bool r = m_rootPane->OnEvent(EventType_MouseMove, args);
	UpdateMouseHover(PointF(clientX, clientY));
	return r;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonDown(MouseButton button, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseButtonDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonUp(MouseButton button, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseButtonUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseWheel(int delta, float clientX, float clientY)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, delta, clientX, clientY));
	return m_rootPane->OnEvent(EventType_MouseWheel, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_rootPane->OnEvent(EventType_KeyDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_rootPane == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_rootPane->OnEvent(EventType_KeyUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectElapsedTime(float elapsedTime)
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::UpdateMouseHover(const PointF& mousePos)
{
	UIElement* old = m_mouseHoverElement;

	// TODO:IME���̃C�x���g����������
	//if ( m_pIme != NULL )
	//{
	//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
	//	{
	//		goto EXIT;
	//	}
	//}

	// �ʏ�̃E�B���h�E�̃C�x���g����������
	if (m_rootPane != NULL)
	{
		m_mouseHoverElement = m_rootPane->CheckMouseHoverElement(mousePos);
		if (m_mouseHoverElement != NULL) {
			goto EXIT;
		}
	}

	m_mouseHoverElement = NULL;

EXIT:
	// �V�����ꂼ��� Element �� MouseLeave�AMouseEnter �C�x���g�𑗂�
	if (m_mouseHoverElement != old)
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, mousePos.X, mousePos.Y));
		if (old != NULL) {
			old->OnEvent(EventType_MouseLeave, args);
		}
		if (m_mouseHoverElement != NULL) {
			m_mouseHoverElement->OnEvent(EventType_MouseEnter, args);
		}
	}
}

} // namespace GUI
} // namespace Lumino
