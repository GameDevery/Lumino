/*

	���L�[���[�h
	�E�e���v���[�g
	�E�o�C���f�B���O
	�E���[�e�B���O �C�x���g	https://msdn.microsoft.com/ja-jp/library/ms742806.aspx

	[2015/7/28] TemplateBinding ���ĕK�v�H
		VisualTree �̗v�f���� TemplateParent �ɃA�N�Z�X���邽�߂ɕK�v�B
		Qt �ł������悤�Ȋ����ŃX�^�C����g�ށB
		http://relog.xii.jp/mt5r/2013/09/qmlqt-quick-controls.html

	[2015/7/25] Documents ���ĕK�v�H
		�ǂ��炩�Ƃ����Ɠ����f�[�^�ł���B
		���b�`�e�L�X�g���J�X�^�}�C�Y���������[�U�[�ȊO�A�ʏ�͎g��Ȃ��B

		�ł��A�����I�ɂ͉��炩�̎�i�Ń��b�`�e�L�X�g�I�Ȃ��̂�\����i���~�����B
		�Ⴆ�΃Q�[���̃��b�Z�[�W�E�B���h�E��
		�E�F����
		�E�t�H���g
		�E�n�C�p�[�����N
		�E�A�C�R��
		�𕶏����ɖ��ߍ��ނɂ͂ǂ����邩�H


	[2015/7/23] ScrollBar �� ScrollViewer �Ƀ}�E�X�h���b�O�̃X�N���[����ʒm����Ƃ��A
				���� RoutedEvent �ł͂Ȃ��ARoutedCommand �łȂ���΂Ȃ�Ȃ��̂��H



	[2015/7/20]
		RoutedEvent ���A�Ƃ肠���� Lumino ���O��Ԃɂ����Ă݂�B
		�ł���� GUI �Ɏ����Ă��������ǁACoreObject �� PropertyChanged �ƌ��т��Ă���B
		PropertyChanged ���� GUI �Ɏ����Ă���̂̓A�������A
		TypeInfo �ł������Ă���B�������͋�̓I�ȈĂ��ł܂��Ă���ɂ���B


	[2015/7/18] ���ۂ� Style �� Template ��K�p����^�C�~���O
		WPF�ł́c
			Style �́AStype.set�B
			Template �́AMeasureCore�B�������́A�h���ɂ���Ă� Loaded �������� GotFocus ��������B

		�{���C�u�����Ƃ��ẮA���ɉ����ăR���X�g���N�^�ł��O���[�o�����\�[�X���� Style ���擾���A�K�p����B
		�u�v���p�e�B�Ƀ��[�U�[�ݒ�ς݂��A�f�t�H���g�̂܂܂��v�������t���O�ł������Ă����
		�x���Őݒ肷�邱�Ƃ͂ł��邪�A�ʂ����Ă����܂ł��Ӗ�������̂��c�B



		�E�E�E
		�E�e CoreObject �T�u�N���X�ɂ́A�v���p�e�B�̍X�V�L��������bool�z�����������
		�ETypeInfo �ɓo�^���ꂽ�v���p�e�B�ɂ̓C���f�b�N�X��U��B
		�ESet�Ǝ�(SizeProperty, true);
		�EGet�ɂ��׍H���Ȃ��ƁA�e�v���p�e�B��ύX�����Ƃ��ɐ������l�����Ȃ��Ȃ�B
			������̋@�\�̐e��Style�B
			����Ή����悤�Ƃ���ƁAprivate �ł����ڃ����o�ϐ��ɃA�N�Z�X�ł����� GetValue �������A
			�S�Ă� getter �Ɍp�����l������׍H���K�v�ɂȂ�B
			Style �� static resource �Ȃ�A���蓖�Ă���ɕς�邱�Ƃ͂Ȃ��ƍl���āA
			�����������ʓ|�Ȃ��Ƃ͂��Ȃ��čςށB

			��
			�~�Ƃ肠�����AStyle �������Ɋ��蓖�Ă����ƁAStyle �� Setter �ǉ������肵�Ă��Ӗ��Ȃ���A�ŁB

			��
			�I�u�W�F�N�g������ Create �ɓ��ꂵ�āA���̒��� Style �����E�K�p���Ă݂�B
			���̃��W���[���ƃC���^�[�t�F�C�X�𓝈ꂵ�Ă݂����B
			�E�A�j���[�V��������Z�b�g���ꂽ�l�́A�u���[�U�[�ݒ肳�ꂽ�l�v�ł͂Ȃ��B
				��͂�v���p�e�B�� getter/setter �̓����o�ϐ��𒼐ڑ��삷��̂ł͂Ȃ��A
				GetValue/SetValue���Ăяo�������ɂ���悤�ɂ��Ȃ���΂Ȃ�Ȃ����E�E�E�B

			��
			�t�ɁA���I�e�[�}/�X�^�C���ύX���֎~������V���v���Ɏ����ł���H
			InitialzeComponet()/SetStyle() �Ńv���p�e�B�̒l�������I�ɏ㏑�����邾���ŗǂ��B


	[2015/7/17] WPF �̃f�t�H���g�X�^�C��
		�v���O���}��Style ���`���Ă��Ȃ����́AFrameworkElement.Style �� null ��Ԃ��B
		
		�Ⴆ�΁A
		<Window.Resource>
			<Style TargetType="Button">
		</Window.Resource>
		<Button />
		�́AButton �̓f�t�H���g�� Style �������Ă���B
		<Style TargetType="Button"> �������ƁAButton.Style �� null ��Ԃ��B

		�{���C�u�����Ƃ��ẮA�f�t�H���g�̎��� Style ��Ԃ��A�ł��������B

		���Ȃ݂ɁAStyle �͎w�肵�� TargetType �ɂ̂ݗL���ł���B
		<Style TargetType="Control"> �Ə����Ă��AButton �ɂ͔��f����Ȃ��B


	[2015/7/17] VisualTree �����̂͗ǂ����ǁAStyle �� Template ��p�ӂ���K�v�͂���́H
		�����Ă���邱�Ƃ͏o���邪�A����� Button �Ȃǂ̃N���X�ɒ��ځA���o�I�ȕ����̎������s��Ȃ���΂Ȃ�Ȃ��Ƃ������ƁB
		�F��ς�����A�j���[�V�����̎��Ԃ�ς����肷��̂̓v���p�e�B���炢����邯�ǁA


		�e�[�}�ύX��
		�@WPF�`��
		�AStyle�ƃX�N���v�g���g����Web����

		Style �͐�ΕK�v�BTemplate �́E�E�E

		�ETemplate vs �X�N���v�g
			�X�N���v�g�ɂ���΁AOnRender() �Ƃ��I�[�o�[���C�h������ł��A�_��͂����������B
			�����ǁA����̓��C�u������鑤�ɂ��������S�ɂȂ�B

			�Ƃ������A�������� OnRender() �Ƃ��A�C�x���g���I�[�o�[���C�h����̂���
			�R���g���[���̓����ς��邱�ƂƓ������Ƃ�����A����͕ʂ̃R���g���[�������ׂ����Ǝv���B

			�����l����ƁATemplate �̕K�v�����Ă����̂́A�u�e�[�}�v�Ƃ����_���猩���Ƃ��A
			VisualTree �ɑ�����R���g���[���̔z�u��ύX���邱�ƁB
			�Ⴆ�΃E�B���h�E�̃N���[�Y�{�^���̈ʒu�����炵����Ƃ��A��������Ƃ��B
			Rectangle �ŏC������̂͂����܂ł��܂��B



	[2015/7/15] ���X�g�̃v���p�e�B�ɂ���
		WPF �ł̓R���N�V�����͈ˑ��֌W�v���p�e�B�ɂ���Ă��Ȃ� (�ł��Ȃ��H)
		�e�R���N�V������ IAddChild ���p�����Ă��Ȃ��B
		�܂�AXAML ���牽�����Z�b�g����Ƃ��̓��t���N�V�����ŃR���N�V�����^�ł��邩���m�F���Ă���B

		C++ �ł͂���Ȃ��Ƃ͂ł��Ȃ��̂ŁA�{���C�u�����Ƃ��Ă̓R���N�V�������ˑ��֌W�v���p�e�B�Ƃ��ēo�^���Ă���B


	[2015/7/10]
		�ESetPropertyValue() �̃L�[�� Property �̃|�C���^�ɂ���H���O�ɂ���H
		�E�ˑ��֌W�v���p�e�B�̓|�C���^�Ƃ��Č��J����H���O�ɂ���H

		�r�n�C���h�R�[�h����A�N�Z�X����ꍇ�̓|�C���^�̕������R�����B
		XML ����A�N�Z�X����Ƃ��� "�v�f��.�v���p�e�B��" �Ƃ���������Ō�������K�v������B
		�L�[���|�C���^�ɂ��Ă����ƁA�܂�������A���Ƀ|�C���^�A�Ƃ�����2��̌������K�v�ɂȂ�B
		
		�Ƃ͌����Ă��A2��̌������K�v�ɂȂ�̂͏������������B
		�ނ���A�j���[�V�����Ƃ��̓��A���^�C���ɃA�N�Z�X����K�v������̂ŁA
		�|�C���^���邢�̓n�b�V���l�̂悤�Ȑ��l�Ō����ł����ق��������B


	[2015/7/8] ���W�̒��l�w��͕K�v�H
		WinForms �� Location �v���p�e�B�͕K�v���Ƃ������ƁB
		WPF �x�[�X�Ȃ̂� Canvas.SetLeft(button, 10); �Ƃ����K���B
		Siv3D ����΍��W�͎����ĂȂ��Ń}�[�W���������Ă����B


	[2015/7/8] �z���g�� VisualTree �K�v�H

		Arrange �ō��u�N���C�A���g�̈�̊O���v�Ɂu�R���g���[���v��z�u����������K�v�B
		���[�U�[�Ɍ��J���Ȃ��Ƃ��Ă��A�����I�ɂ͎����Ă����������ǂ��B
		����͖{���C�u������GUI���W���[��1.0�̎�����f�O���������������͂��B
		

	[2015/7/7] RoutedCommand �� RoutedEvent �̈Ⴂ�ɂ���

		RoutedCommand �� <Button Command="ScrollBar.LineUp" /> �̂悤�Ɏg���B
		����́A�{�^���N���b�N�Ɂu�X�N���[���o�[�́��ړ��v�Ƃ����Ӗ����������Ă���Ƃ������ƁB
		����ȊO�ɂ��A�E�B���h�E�̃N���[�Y�Ƃ��R�s�y�Ƃ����낢�날��B

		RoutedEvent �͒P�ɃC�x���g��ʒm���邾���B

	[2015/7/5] �C�x���g��������
		�{���C�u�����Ƃ��ẮA
		�EOn�` �� RaiseEvent() ���Ăяo�������œ��ꂷ��B
			����̓p�t�H�[�}���X���l�������Ή��B
			RaiseEvent() �͗v�f�����n���h����S���������肷�邽�߁A�����ɂ�⎞�Ԃ�������\��������B
			On�` �ŏ����ς݂��`�F�b�N���A�s�K�v�ȏꍇ�� RaiseEvent() ���Ȃ��悤�ɂ��邱�Ƃ�
			���ʂȏ������s��Ȃ��悤�ɂ˂炤�B
		
		�܂��A���݃R���g���[�������[�e�B���O�C�x���g���n���h�����O������@��2����A
		1�� AddHandler ���邱�ƁA������� LN_REGISTER_ROUTED_EVENT_HANDLER ���邱�ƁB
		(WPF �ł͑O�҂��u���I�n���h���v��҂��u�ÓI�n���h���v�ƌĂ�ł���݂���)

		�ÓI�n���h���͓��I�n���h�����Ăяo�����O�ɌĂяo����Ae.Handled ���}�[�N���ꂽ�ꍇ
		�㑱�̓��I�n���h���Ăяo���y�у��[�e�B���O�͍s���Ȃ��B
		https://msdn.microsoft.com/ja-jp/library/ms597875%28v=vs.110%29.aspx


	[2015/7/5] �C�x���g��������
		�EOn�` �̓��[�e�B���O�C�x���g�̃n���h���B
			�Ⴆ�΁ARaiseEvent(MouseDown) ����ƁA�������� OnMouseDown() ���Ă΂��B
			OnMouseDown() �� RaiseEvent(MouseDown) ����̂ł͂Ȃ��_�ɒ��ӁB
			OnMouseDown() ���� RaiseEvent(Click) �̂悤�ɑ��̃C�x���g�𔭍s����̂͂���B

			�����AWPF �̒��ł͓��ꂳ��Ă��Ȃ��݂����B
			MouseDown �� Raise��On �̂悤�ɂȂ��Ă��邪�AButtonBase.OnClick �� On��Raise�B




	[2015/6/30] �������� VisualTree �Ȃ�č��K�v����́H
		
		LogicalTree �ƕ�����d�g�݂͕K�v�B
		�Ⴆ�΁A�E�B���h�E�̃V�X�e���{�^���B
		����̓{�^�������ł͂Ȃ�Panel�Ƒg�ݍ��킹�ă��C�A�E�g�𒲐߂��������́B
		LogicalTree �Ɠ����悤�ȃ��C�A�E�g�̎d�g�݂�����ƕ֗��B

		�e�[�}�̐؂�ւ��� CSS ����_���Ȃ́H
		�� CSS �ŃE�B���h�E�̃V�X�e���{�^�����J�X�^�}�C�Y�ł���́H
		�� �E�B���h�E�̃N���C�A���g�̈�̔w�i�Ƙg�������āAGlass �����ɂ������Ƃ��́H
			�� �w�iON/OFF�v���p�e�B���K�v�ɂȂ肻���E�E�E



	[2015/6/30] Qt Quick �̂悤�ȃR���g���[���̃J�X�^�}�C�Y�́H
		
		Qt Quick �� JavaScript ���C�N�ȃ}�[�N�A�b�v�ŁA
		Button {
			background : <�q�R���g���[��>
			content : <�R���e���c>
		}
		�݂����Ȃ��񂶂ŃR���g���[���̃J�X�^�}�C�Y���ł���B
		���ׂāA
		�Ebackground
		�Econtent
		�Eforeground
		��3���C���[�ŃJ�X�^�}�C�Y�ł���Ώ����V���v���ɂȂ肻�������E�E�E�H
		
		��

		�e�R���e�i�� Padding ���l���ł��Ȃ��B
		BackgroundPadding �Ƃ���p�̃v���p�e�B���K�v�ɂȂ��Ă��܂��B


	[2015/6/9] AddChild
		����͘_���c���[�\�z�p�Ɍ�����B
		WPF �� ContentControll �� AddChild �́A�����Ŏ󂯎�����I�u�W�F�N�g�� Content �v���p�e�B�ɓ���Ă邾���������B
		��
		����ł����B
		�e���v���[�g�ō�����c���[�͂���܂� Logical �S�R�֌W�Ȃ��Ǝv���Ă����ǁA
		VisualTree �̗v�f�� LogcalTreeHelper.GetParent() ����ƁAVisualTree ��̐e�v�f������B


	[2015/6/9] �f�t�H���g�� Chrome �̏�ԕω��ɉ����g���H(TemplateBinding �� VisualState �� Trigger)
		.NET �E�G�ł͌����ڂ̕\���ɂ͂ł��邾�� VisualState ���g�����ق���������˓I�Ȋ����ɂȂ��Ă���B
		��VisualState �� Silverlight ���� WPF �ɗA�����ꂽ�A

		�����A��{�I�� Stopryboard (�A�j���[�V����) �ł����v���p�e�B�ω��������Ȃ��̂ŁATrigger �̕��p���K�v�B

		TemplateBinding �͏�Ԃ̒ʒm�Ɍ��炸���낢��Ȃ��̂� Template �̂ǂ��ɒu���������߂�̂ɂ��g���B
		�������ł̑Ή���D�悵�Ă݂�B
		



	[2015/6/9] Binding �� TemplateBinding
		����2�͎��Ă��邪�A�f�[�^�\�[�X�Ƃ��Ĉ����Ώۂ��Ⴄ�B
		Binding �� �_���v�f�� DataContext�A
		TemplateBinding �͘_���v�f���g���\�[�X�Ƃ���B

		���̂悤�ȃ}�[�N�A�b�v�́A1�ڂ̃��x���� Button.DataContext ���ς���
		����ɒǐ����ĕς�邪�A2�ڂ̃��x���͕ς�炸�A��� Button.IsPressed ���Q�Ƃ�������B

		<Button x:Name="_button1" Height="32">
			<Button.Template>
				<ControlTemplate TargetType="{x:Type Button}">
					<StackPanel Orientation="Horizontal">
						<Label Content="{Binding Name}" />
						<Label Content="{TemplateBinding IsPressed}" />
					</StackPanel>
				</ControlTemplate>
			</Button.Template>
		</Button>

	
	[2015/6/9] CEGUI �̃v���p�e�B
		Editbox::addEditboxProperties() ����H��Ƃ������Q�l�ɂȂ�B


	[2015/6/9] CEGUI �̕`��

		Button ���Ɍ��Ă݂�ƁE�E�E

		PushButton �N���X�͉�������Ԃ��Ǘ����邾���B�`����s���l�͕ʂɂ���B
		�`����s���̂� FalagardButton �N���X�ŁAWindowRenderer �N���X�̃T�u�N���X�B

		WindowRenderer �� Widget ���ƂɃN���X����`����Ă��āA���ɂ� TabControlWindowRenderer �� EditboxWindowRenderer ������B
		���傤�ǂ��ꂪ����낤�Ƃ��Ă��� Chrome �ɑ����������B
		Widget �� WindowRenderer �� 1��1�B


		�`��͂��ƕ��G�BFalagardButton::render() ����ȉ��̂悤�ɐ����Ă����B
		FalagardButton::render()
			WidgetLookFeel::render()
				StateImagery::render()
					LayerSpecification::render()
						SectionSpecification::render()
							ImagerySection::render()

		Window �̎��� LookFeel ���؂�ւ��΁AWidgetLookFeel::render() ���؂�ւ��d�g�݁B

		ImagerySection::render() �͎���3�����ɕ`�悵�Ă���B���ꂪ CEGUI �̍ŏ��`��P�ʁH
		�EFrameComponent	�E�E�E �g
		�EImageryComponent	�E�E�E �w�i
		�ETextComponent		�E�E�E �e�L�X�g


		WindowRenderer �̓I�[�i�[�R���g���[���𒼐ڎQ�Ƃ��A���̏�Ԃ��g���� StateImagery �������AStateImagery::render() ���Ăяo���B
		�Ⴆ�� PushButton �Ȃ�AisPushed() �Ȃ� "pushed" ��������L�[�Ɍ������s���B

		�f�t�H���g�̃e�[�}�t�@�C���́����ۂ��B
		datafiles\looknfeel\AlfiskoSkin.looknfeel
	
		<ImagerySection> �ŁACompnent �̔z�u�����߂Ė��O������B
		<StateImagery> �ŁA�����Ԃ̂Ƃ��ɂǂ� <ImagerySection> ��\�����邩�����߂�B
		�悭���Ă݂�΁A���̕��G�ȕ`��p�X���[���ł���B
	


	[2015/6/9] Unity GUI
		GUI �R���g���[���̃J�X�^�}�C�Y
		http://docs.unity3d.com/ja/current/Manual/gui-Customization.html

		GUI �X�L��
		http://docs.unity3d.com/ja/current/Manual/class-GUISkin.html
	
		CCS ���ӎ����Ă���B�������P���B

	[2015/6/7] Chrome ���̇A
		WPF �� WindowChrome �� UIElement �ł͂Ȃ��B
		�^�C�g���o�[�̍�����A�E�B���h�E�g�̕����`����A�P�Ȃ�f�[�^�N���X�B
		Window �N���X�̓Z�b�g����Ă��� WindowChrome ���炱�̊e��̈���擾���A�^�C�g���o�[��D&D�ړ���
		���ED&D�ɂ�郊�T�C�Y���s���Ă���B

		Button �̊�{�I�ȃe���v���[�g�͂���Ȋ����B
		<Button.Template>
			<ControlTemplate TargetType="{x:Type Button}">
				<Microsoft_Windows_Themes:ButtonChrome SnapsToDevicePixels="true" 
					x:Name="Chrome" Background="{TemplateBinding Background}" 
					BorderBrush="{TemplateBinding BorderBrush}" RenderDefaulted="{TemplateBinding IsDefaulted}" 
					RenderMouseOver="{TemplateBinding IsMouseOver}" RenderPressed="{TemplateBinding IsPressed}">
					<ContentPresenter Margin="{TemplateBinding Padding}"
						VerticalAlignment="{TemplateBinding VerticalContentAlignment}"
						HorizontalAlignment="{TemplateBinding HorizontalContentAlignment}"
						RecognizesAccessKey="True"
						SnapsToDevicePixels="{TemplateBinding SnapsToDevicePixels}"/>
				</Microsoft_Windows_Themes:ButtonChrome>
			</ControlTemplate>
		</Button.Template>

		Button �� IsMouseOver �� IsPressed �����J���AButtonChrome �͂���Ƀo�C���h���Ă���B
		�܂�AVisualState �Ƃ������ł͊֌W�Ȃ��B
	


	[2015/6/7] Chrome �̍l����
		ButtonChrome �� WindowChrome �́A�u������x��ʓI�ȃr�W���A����������r�W���A���c���[�\�z���[�e�B���e�B�v�ł���B
		
		�z���g�Ƀ[������J�X�^�}�C�Y�������Ƃ��� Chrome �͎g�킸�ARectangle ���� Shape ����g���Ď��삷��B
		
		�������A�قƂ�ǂ̏ꍇ�̓J�X�^�}�C�Y�������ƌ����Ă���肽�����Ƃ̓E�B���h�E�g�̉摜��ς������Ƃ��A
		���������u�f�t�H���g�R���g���[�����班���ς������v���x�ł���͂��B
		������x������̂����� Chrome�B

		�Ⴆ�΁A���삹���Ƀf�t�H���g�� Chrome ���g���Ă���ꍇ�̓E�B���h�E�X�L���� png �������ւ��邾����
		�e�[�}�ύX��B�����邱�Ƃ��ł���B


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
#include <Lumino/GUI/RootFrame.h>
#include <Lumino/GUI/Button.h>
#include <Lumino/GUI/ButtonChrome.h>
#include <Lumino/GUI/Controls/Thumb.h>
#include <Lumino/GUI/Controls/Track.h>
#include <Lumino/GUI/Controls/Grid.h>
#include <Lumino/GUI/Controls/Image.h>
#include <Lumino/GUI/Controls/ScrollBar.h>
#include <Lumino/GUI/Controls/ScrollViewer.h>
#include <Lumino/GUI/Controls/StackPanel.h>
#include <Lumino/GUI/Controls/ListBox.h>
#include <Lumino/GUI/GUIManager.h>

namespace Lumino
{
namespace GUI
{
	
//=============================================================================
// GUIManager
//=============================================================================

static const byte_t g_DefaultSkin_png_Data[] =
{
#include "Resource/DefaultSkin.png.h"
};
static const size_t g_DefaultSkin_png_Len = LN_ARRAY_SIZE_OF(g_DefaultSkin_png_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::GUIManager()
	: m_mouseHoverElement(NULL)
	, m_defaultTheme(NULL)
	, m_rootCombinedResource(NULL)
	, m_defaultRootFrame(NULL)
	, m_capturedElement(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManager::~GUIManager()
{
	LN_SAFE_RELEASE(m_rootCombinedResource);
	LN_SAFE_RELEASE(m_defaultTheme);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::Initialize(const ConfigData& configData)
{
	if (LN_VERIFY_ASSERT(configData.GraphicsManager != NULL)) { return; }

	m_graphicsManager = configData.GraphicsManager;
	m_mainWindow = configData.MainWindow;
	m_documentsManager = configData.DocumentsManager;


	RegisterFactory(ContentPresenter::TypeID,		[](GUIManager* m) -> CoreObject* { return ContentPresenter::Create(m); });
	RegisterFactory(ItemsPresenter::TypeID,			[](GUIManager* m) -> CoreObject* { return ItemsPresenter::Create(m); });
	RegisterFactory(ButtonChrome::TypeID,			[](GUIManager* m) -> CoreObject* { return ButtonChrome::Create(m); });
	RegisterFactory(Button::TypeID,					[](GUIManager* m) -> CoreObject* { return Button::Create(m); });
	RegisterFactory(ListBoxChrome::TypeID,			[](GUIManager* m) -> CoreObject* { return ListBoxChrome::Create(m); });
	RegisterFactory(ThumbChrome::TypeID,			[](GUIManager* m) -> CoreObject* { return ThumbChrome::Create(m); });
	RegisterFactory(Thumb::TypeID,					[](GUIManager* m) -> CoreObject* { return Thumb::Create(m); });
	RegisterFactory(Track::TypeID,					[](GUIManager* m) -> CoreObject* { return Track::Create(m); });
	RegisterFactory(Grid::TypeID,					[](GUIManager* m) -> CoreObject* { return Grid::Create(m); });
	RegisterFactory(ColumnDefinition::TypeID,		[](GUIManager* m) -> CoreObject* { return ColumnDefinition::Create(m); });
	RegisterFactory(RowDefinition::TypeID,			[](GUIManager* m) -> CoreObject* { return RowDefinition::Create(m); });
	RegisterFactory(Image::TypeID,					[](GUIManager* m) -> CoreObject* { return Image::Create(m); });
	RegisterFactory(ScrollBar::TypeID,				[](GUIManager* m) -> CoreObject* { return ScrollBar::Create(m); });
	RegisterFactory(ScrollContentPresenter::TypeID, [](GUIManager* m) -> CoreObject* { return ScrollContentPresenter::Create(m); });
	RegisterFactory(ScrollViewer::TypeID,			[](GUIManager* m) -> CoreObject* { return ScrollViewer::Create(m); });
	RegisterFactory(StackPanel::TypeID,				[](GUIManager* m) -> CoreObject* { return StackPanel::Create(m); });

	

	m_defaultTheme = LN_NEW ResourceDictionary();
	BuildDefaultTheme();

	m_rootCombinedResource = LN_NEW CombinedLocalResource();
	m_rootCombinedResource->Combine(NULL, m_defaultTheme);




	m_defaultRootFrame = ApplicationContext::CreateObject<RootFrame>(this);
	m_defaultRootFrame->ApplyTemplate();	// �e�[�}�𒼂��ɍX�V
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::Finalize()
{
	LN_SAFE_RELEASE(m_defaultRootFrame);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//RootFrame* GUIManager::CreateRootFrame()
//{
//	m_defaultRootFrame = LN_NEW RootFrame(this);
//	m_defaultRootFrame->ApplyTemplate(m_rootCombinedResource);	// �e�[�}�𒼂��ɍX�V
//	return m_rootPane;
//}

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
	LN_THROW(f != NULL, KeyNotFoundException, typeFullName.GetCStr());
	return f(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseMove(float clientX, float clientY)
{
	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != NULL) 
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, clientX, clientY));
		return m_capturedElement->OnEvent(EventType_MouseMove, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	UpdateMouseHover(PointF(clientX, clientY));
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, clientX, clientY));
	//if (m_mouseHoverElement != NULL)
	return m_mouseHoverElement->OnEvent(EventType_MouseMove, args);
	//bool r = m_defaultRootFrame->OnEvent(EventType_MouseMove, args);
	//return UpdateMouseHover(PointF(clientX, clientY));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonDown(MouseButton button, float clientX, float clientY)
{
	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
		return m_capturedElement->OnEvent(EventType_MouseButtonDown, args);
	}
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	//if (m_mouseHoverElement != NULL) {
	return m_mouseHoverElement->OnEvent(EventType_MouseButtonDown, args);
	//}
	//return m_defaultRootFrame->OnEvent(EventType_MouseButtonDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseButtonUp(MouseButton button, float clientX, float clientY)
{
	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
		return m_capturedElement->OnEvent(EventType_MouseButtonUp, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(button, 0, clientX, clientY));
	//return m_defaultRootFrame->OnEvent(EventType_MouseButtonUp, args);
	return m_mouseHoverElement->OnEvent(EventType_MouseButtonUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectMouseWheel(int delta, float clientX, float clientY)
{
	// �L���v�`�����̃R���g���[��������΂�����ɑ���
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, 0, clientX, clientY));
		return m_capturedElement->OnEvent(EventType_MouseMove, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(m_eventArgsPool.CreateMouseEventArgs(MouseButton_None, delta, clientX, clientY));
	//return m_defaultRootFrame->OnEvent(EventType_MouseWheel, args);
	return m_mouseHoverElement->OnEvent(EventType_MouseWheel, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_defaultRootFrame == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_defaultRootFrame->OnEvent(EventType_KeyDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_defaultRootFrame == NULL) { return false; }
	RefPtr<KeyEventArgs> args(m_eventArgsPool.CreateKeyEventArgs(keyCode, isAlt, isShift, isControl));
	return m_defaultRootFrame->OnEvent(EventType_KeyUp, args);
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
void GUIManager::CaptureMouse(UIElement* element)
{
	m_capturedElement = element;
	m_mainWindow->CaptureMouse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::ReleaseMouseCapture(UIElement* element)
{
	if (m_capturedElement == element)
	{
		m_capturedElement = NULL;
		m_mainWindow->ReleaseMouseCapture();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIManager::UpdateMouseHover(const PointF& mousePos)
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
	if (m_defaultRootFrame != NULL)
	{
		m_mouseHoverElement = m_defaultRootFrame->CheckMouseHoverElement(mousePos);
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
			return m_mouseHoverElement->OnEvent(EventType_MouseEnter, args);
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIManager::BuildDefaultTheme()
{
	// TODO: ���̂ւ�� WPF �� ScrollViewer.CreateDefaultControlTemplate() �݂����ɂ܂Ƃ߂���

	// RootFrame
	{
		RefPtr<Style> style = RefPtr<Style>::Create();
		style->SetTargetType(RootFrame::GetClassTypeInfo());

		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("RootFrame"));
		style->AddSetter(Control::TemplateProperty, t);

		RefPtr<UIElementFactory> presenter1(LN_NEW UIElementFactory(this));
		presenter1->SetTypeName(_T("ContentPresenter"));
		t->SetVisualTreeRoot(presenter1);

		m_defaultTheme->AddStyle(style);
	}

	// Brush (�{�^���g)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(0, 0, 32, 32));
		m_defaultTheme->AddItem(_T("ButtonNormalFrameBrush"), obj);
	}
	// Brush (�{�^���w�i)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(8, 8, 16, 16));
		m_defaultTheme->AddItem(_T("ButtonNormalBackgroundBrush"), obj);
	}
	// Brush (ListBox �g)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(0, 32, 32, 32));
		m_defaultTheme->AddItem(_T("ListBoxNormalFrameBrush"), obj);
	}
	// Brush (ListBox �w�i)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());	//TODO:
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(8, 32 + 8, 16, 16));
		m_defaultTheme->AddItem(_T("ListBoxNormalBackgroundBrush"), obj);
	}

	// Button
	{
		RefPtr<Style> style = RefPtr<Style>::Create();
		style->SetTargetType(Button::GetClassTypeInfo());

		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("Button"));
		style->AddSetter(Control::TemplateProperty, t);

		RefPtr<UIElementFactory> ef1(LN_NEW UIElementFactory(this));
		ef1->SetTypeName(_T("ButtonChrome"));
		ef1->AddTemplateBinding(ButtonChrome::IsMouseOverProperty, Button::IsMouseOverProperty);
		t->SetVisualTreeRoot(ef1);

		RefPtr<UIElementFactory> ef2(LN_NEW UIElementFactory(this));
		ef2->SetTypeName(_T("ContentPresenter"));
		ef1->AddChild(ef2);

		m_defaultTheme->AddStyle(style);
	}

	// Thumb (�g Brush)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(0, 64, 32, 32));
		m_defaultTheme->AddItem(_T("ThumbChromeBackgroundFrameBrush"), obj);
	}
	// Thumb (�g�w�i Brush)
	{
		RefPtr<Graphics::TextureBrush> obj(LN_NEW Graphics::TextureBrush());
		obj->Create(_T("../../src/GUI/Resource/DefaultSkin.png"), m_graphicsManager);
		obj->SetSourceRect(Rect(8, 64 + 8, 16, 16));
		m_defaultTheme->AddItem(_T("ThumbChromeBackgroundInnerBrush"), obj);
	}
	// Thumb
	{
		RefPtr<Style> style = RefPtr<Style>::Create();
		style->SetTargetType(Thumb::GetClassTypeInfo());

		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("Thumb"));
		style->AddSetter(Control::TemplateProperty, t);

		RefPtr<UIElementFactory> ef1(LN_NEW UIElementFactory(this));
		ef1->SetTypeName(_T("ThumbChrome"));
		//ef1->AddTemplateBinding(ButtonChrome::IsMouseOverProperty, Button::IsMouseOverProperty);
		t->SetVisualTreeRoot(ef1);

		m_defaultTheme->AddStyle(style);
	}

	// Track
	{
		RefPtr<Style> style = RefPtr<Style>::Create();
		style->SetTargetType(Track::GetClassTypeInfo());

		RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		t->SetTargetType(_T("Track"));
		style->AddSetter(Control::TemplateProperty, t);

		RefPtr<UIElementFactory> button1(LN_NEW UIElementFactory(this));
		button1->SetTypeName(_T("Button"));
		style->AddSetter(Track::DecreaseButtonProperty, button1);

		RefPtr<UIElementFactory> thumb1(LN_NEW UIElementFactory(this));
		thumb1->SetTypeName(_T("Thumb"));
		style->AddSetter(Track::ThumbProperty, thumb1);

		RefPtr<UIElementFactory> button2(LN_NEW UIElementFactory(this));
		button2->SetTypeName(_T("Button"));
		style->AddSetter(Track::IncreaseButtonProperty, button2);

		m_defaultTheme->AddStyle(style);
	}

	// ScrollBar
	{
		// H ScrollBar
		RefPtr<ControlTemplate> scrollBarTemplateH(LN_NEW ControlTemplate());
		scrollBarTemplateH->SetTargetType(_T("ScrollBar"));
		{
			RefPtr<UIElementFactory> grid1(LN_NEW UIElementFactory(this));
			grid1->SetTypeName(_T("Grid"));
			scrollBarTemplateH->SetVisualTreeRoot(grid1);

			auto columns = RefPtr<UIElementFactorylist>::Create();
			grid1->SetPropertyValue(Grid::ColumnDefinitionsProperty, columns);

			RefPtr<UIElementFactory> col1(LN_NEW UIElementFactory(this));
			col1->SetTypeName(_T("ColumnDefinition"));
			col1->SetPropertyValue(ColumnDefinition::WidthProperty, 16.0f);
			columns->Add(col1);

			RefPtr<UIElementFactory> col2(LN_NEW UIElementFactory(this));
			col2->SetTypeName(_T("ColumnDefinition"));
			col2->SetPropertyValue(ColumnDefinition::WidthProperty, ColumnDefinition::Star);
			columns->Add(col2);

			RefPtr<UIElementFactory> col3(LN_NEW UIElementFactory(this));
			col3->SetTypeName(_T("ColumnDefinition"));
			col3->SetPropertyValue(ColumnDefinition::WidthProperty, 16.0f);
			columns->Add(col3);

			//RefPtr<UIElementFactory> button1(LN_NEW UIElementFactory(this));
			//button1->SetTypeName(_T("Button"));
			//t->SetPropertyValue(Track::DecreaseButtonProperty, button1);

			RefPtr<UIElementFactory> track(LN_NEW UIElementFactory(this));
			track->SetTypeName(_T("Track"));
			track->SetKeyName(ScrollBar::PART_TrackKeyName);
			track->SetPropertyValue(Track::OrientationProperty, Orientation::Horizontal);
			track->SetPropertyValue(Grid::ColumnProperty, 1);	// �Y�t�v���p�e�B
			grid1->AddChild(track);

			//RefPtr<UIElementFactory> button2(LN_NEW UIElementFactory(this));
			//button2->SetTypeName(_T("Button"));
			//t->SetPropertyValue(Track::IncreaseButtonProperty, button2);
		}

		// V ScrollBar
		RefPtr<ControlTemplate> scrollBarTemplateV(LN_NEW ControlTemplate());
		scrollBarTemplateV->SetTargetType(_T("ScrollBar"));
		{
			RefPtr<UIElementFactory> grid1(LN_NEW UIElementFactory(this));
			grid1->SetTypeName(_T("Grid"));
			scrollBarTemplateV->SetVisualTreeRoot(grid1);

			auto columns = RefPtr<UIElementFactorylist>::Create();
			grid1->SetPropertyValue(Grid::RowDefinitionsProperty, columns);

			RefPtr<UIElementFactory> row1(LN_NEW UIElementFactory(this));
			row1->SetTypeName(_T("RowDefinition"));
			row1->SetPropertyValue(RowDefinition::HeightProperty, 16.0f);
			columns->Add(row1);

			RefPtr<UIElementFactory> row2(LN_NEW UIElementFactory(this));
			row2->SetTypeName(_T("RowDefinition"));
			row2->SetPropertyValue(RowDefinition::HeightProperty, RowDefinition::Star);
			columns->Add(row2);

			RefPtr<UIElementFactory> row3(LN_NEW UIElementFactory(this));
			row3->SetTypeName(_T("RowDefinition"));
			row3->SetPropertyValue(RowDefinition::HeightProperty, 16.0f);
			columns->Add(row3);

			//RefPtr<UIElementFactory> button1(LN_NEW UIElementFactory(this));
			//button1->SetTypeName(_T("Button"));
			//t->SetPropertyValue(Track::DecreaseButtonProperty, button1);

			RefPtr<UIElementFactory> track(LN_NEW UIElementFactory(this));
			track->SetTypeName(_T("Track"));
			track->SetKeyName(ScrollBar::PART_TrackKeyName);
			track->SetPropertyValue(Track::OrientationProperty, Orientation::Vertical);
			track->SetPropertyValue(Grid::RowProperty, 1);	// �Y�t�v���p�e�B
			grid1->AddChild(track);

			//RefPtr<UIElementFactory> button2(LN_NEW UIElementFactory(this));
			//button2->SetTypeName(_T("Button"));
			//t->SetPropertyValue(Track::IncreaseButtonProperty, button2);
		}

		RefPtr<Style> style = RefPtr<Style>::Create();
		style->SetTargetType(ScrollBar::GetClassTypeInfo());

		Trigger* trigger1 = style->AddPropertyTrigger(ScrollBar::OrientationProperty, Orientation::Horizontal);
		trigger1->AddSetter(Control::TemplateProperty, scrollBarTemplateH);

		Trigger* trigger2 = style->AddPropertyTrigger(ScrollBar::OrientationProperty, Orientation::Vertical);
		trigger2->AddSetter(Control::TemplateProperty, scrollBarTemplateV);

		m_defaultTheme->AddStyle(style);
	}

	// ScrollViewer
	{
		auto style = RefPtr<Style>::Create();
		style->SetTargetType(ScrollViewer::GetClassTypeInfo());


		auto controlTemplate = RefPtr<ControlTemplate>::Create();
		controlTemplate->SetTargetType(_T("ScrollViewer"));
		style->AddSetter(Control::TemplateProperty, controlTemplate);

		auto grid1 = RefPtr<UIElementFactory>::Create(this);
		grid1->SetTypeName(_T("Grid"));
		controlTemplate->SetVisualTreeRoot(grid1);

		// ColumnDefinitions
		auto columns = RefPtr<UIElementFactorylist>::Create();
		grid1->SetPropertyValue(Grid::ColumnDefinitionsProperty, columns);

		auto col1 = RefPtr<UIElementFactory>::Create(this);
		col1->SetTypeName(_T("ColumnDefinition"));
		col1->SetPropertyValue(ColumnDefinition::WidthProperty, ColumnDefinition::Star);
		columns->Add(col1);

		auto col2 = RefPtr<UIElementFactory>::Create(this);
		col2->SetTypeName(_T("ColumnDefinition"));
		col2->SetPropertyValue(ColumnDefinition::WidthProperty, 16.0f/*ColumnDefinition::Auto*/);
		columns->Add(col2);

		// RowDefinitions
		auto rows = RefPtr<UIElementFactorylist>::Create();
		grid1->SetPropertyValue(Grid::RowDefinitionsProperty, columns);

		auto row1 = RefPtr<UIElementFactory>::Create(this);
		row1->SetTypeName(_T("RowDefinition"));
		row1->SetPropertyValue(RowDefinition::HeightProperty, RowDefinition::Star);
		rows->Add(row1);

		auto row2 = RefPtr<UIElementFactory>::Create(this);
		row2->SetTypeName(_T("RowDefinition"));
		row2->SetPropertyValue(RowDefinition::HeightProperty, 16.0f/* RowDefinition::Auto*/);
		rows->Add(row2);

		// ScrollContentPresenter
		auto scrollContentPresenter = RefPtr<UIElementFactory>::Create(this);
		scrollContentPresenter->SetKeyName(ScrollViewer::PART_ScrollContentPresenterTemplateName);
		scrollContentPresenter->SetTypeName(_T("ScrollContentPresenter"));
		grid1->AddChild(scrollContentPresenter);

		// V ScrollBar
		auto vScrollBar = RefPtr<UIElementFactory>::Create(this);
		vScrollBar->SetKeyName(_T("PART_VerticalScrollBar"));
		vScrollBar->SetTypeName(_T("ScrollBar"));
		vScrollBar->SetPropertyValue(ScrollBar::OrientationProperty, Orientation::Vertical);
		vScrollBar->SetPropertyValue(Grid::ColumnProperty, 1);	// �Y�t�v���p�e�B
		grid1->AddChild(vScrollBar);

		// H ScrollBar
		auto hScrollBar = RefPtr<UIElementFactory>::Create(this);
		hScrollBar->SetKeyName(_T("PART_HorizontalScrollBar"));
		hScrollBar->SetTypeName(_T("ScrollBar"));
		hScrollBar->SetPropertyValue(ScrollBar::OrientationProperty, Orientation::Horizontal);
		hScrollBar->SetPropertyValue(Grid::RowProperty, 1);		// �Y�t�v���p�e�B
		grid1->AddChild(hScrollBar);

		m_defaultTheme->AddStyle(style);
	}

	// ListBox
	{
		// <Style TargetType="ListBox">  or <Style TargetType="{x:Type:ListBox}">
		auto style = RefPtr<Style>::Create();
		style->SetTargetType(ListBox::GetClassTypeInfo());

		auto controlTemplate = RefPtr<ControlTemplate>::Create();
		controlTemplate->SetTargetType(_T("ListBox"));	// TODO: TypeInfo�ɂ�����
		style->AddSetter(Control::TemplateProperty, controlTemplate);

		auto scrollViewer = RefPtr<UIElementFactory>::Create(this);
		scrollViewer->SetTypeName(_T("ScrollViewer"));
		controlTemplate->SetVisualTreeRoot(scrollViewer);

		auto itemsPresenter = RefPtr<UIElementFactory>::Create(this);
		itemsPresenter->SetTypeName(_T("ItemsPresenter"));
		scrollViewer->AddChild(itemsPresenter);



		auto stackPanel = RefPtr<ControlTemplate>::Create();
		stackPanel->SetTargetType(_T("StackPanel"));
		style->AddSetter(ItemsControl::ItemsPanelTemplateProperty, stackPanel);
		

		//RefPtr<ControlTemplate> t(LN_NEW ControlTemplate());
		//t->SetTargetType(_T("ListBox"));

		//RefPtr<UIElementFactory> ef1(LN_NEW UIElementFactory(this));
		//ef1->SetTypeName(_T("ListBoxChrome"));
		////ef1->AddTemplateBinding(ButtonChrome::IsMouseOverProperty, Button::IsMouseOverProperty);
		//t->SetVisualTreeRoot(ef1);

		//RefPtr<UIElementFactory> ef2(LN_NEW UIElementFactory(this));
		//ef2->SetTypeName(_T("ItemsPresenter"));
		//ef1->AddChild(ef2);

		m_defaultTheme->AddStyle(style);
	}

	// ListBoxItem
	{
		auto style = RefPtr<Style>::Create();
		style->SetTargetType(ListBoxItem::GetClassTypeInfo());

		auto controlTemplate = RefPtr<ControlTemplate>::Create();
		controlTemplate->SetTargetType(_T("ListBoxItem"));	// TODO: TypeInfo�ɂ�����
		style->AddSetter(Control::TemplateProperty, controlTemplate);

		auto contentPresenter = RefPtr<UIElementFactory>::Create(this);
		contentPresenter->SetTypeName(_T("ContentPresenter"));
		controlTemplate->SetVisualTreeRoot(contentPresenter);

		m_defaultTheme->AddStyle(style);
	}

}

} // namespace GUI
} // namespace Lumino
