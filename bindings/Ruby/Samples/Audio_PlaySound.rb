#==============================================================================
# �����t�@�C�������[�v�Đ����܂��B
#==============================================================================
require './Lumino'
include Lumino

# �����@�\������������
Application.initialize_audio

# �����t�@�C������ Sound �I�u�W�F�N�g�����
sound = Sound.new("Media/ln23.ogg")
sound.volume = 1.0            # ����
sound.pitch = 1.0             # �s�b�`
sound.is_loop_enabled = true  # ���[�vON

# �Đ�
sound.play

# Enter �҂�
gets

# ��~
sound.stop

# ���C�u�����̏I������
Application.terminate

