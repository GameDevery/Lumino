;============================================================
; Lumino �w���v�t�@�C��
;============================================================

%dll
Lumino

%ver
0.2.0.0

%date
2016/05/26

%author
lriki

%note
lumino.as ���C���N���[�h���Ă�������

%type
�g������

%url
http://nnmy.sakura.ne.jp/

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetApplicationLogEnabled
�f�o�b�O�p�̃��O�t�@�C���̏o�͗L����ݒ肵�܂��B(����l:LN_FALSE)

%group
�G���W��
%prm
enabled
[in] enabled : LN_TRUE:�o�͂��� / LN_FALSE:�o�͂��Ȃ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetConsoleEnabled
�W�����o�͗p�̃R���\�[���E�B���h�E�����蓖�Ă邩�ǂ�����ݒ肵�܂��B(����l:LN_FALSE)

%group
�G���W��
%prm
enabled
[in] enabled : LN_TRUE:���蓖�Ă� / LN_FALSE:���蓖�ĂȂ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_RegisterArchive
�t�@�C�����J���Ƃ��ɃA�N�Z�X����Í����A�[�J�C�u��o�^���܂��B

%group
�G���W��
%prm
filePath, password
[in] filePath : �A�[�J�C�u�t�@�C���p�X
[in] password : �A�[�J�C�u�t�@�C�����J�����߂̃p�X���[�h

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetFileAccessPriority
�t�@�C���ւ̃A�N�Z�X�D�揇�ʂ�ݒ肵�܂��B

%group
�G���W��
%prm
priority
[in] priority : �������@ (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
                   LN_FILEACCESSPRIORITY_ARCHIVE_FIRST
                       �A�[�J�C�u�D��
                   LN_FILEACCESSPRIORITY_DIRECTORY_FIRST
                       �f�B���N�g���D��
                   LN_FILEACCESSPRIORITY_ARCHIVE_ONLY
                       �A�[�J�C�u�̂�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetUserWindowHandle
���[�U�[��`�̃E�B���h�E�n���h����ݒ肵�܂��B(����l:NULL)

%group
�G���W��
%prm
windowHandle
[in] windowHandle : ���[�U�[��`�̃E�B���h�E�n���h��

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetSoundCacheSize
�T�E���h�I�u�W�F�N�g�̃L���b�V���T�C�Y�̐ݒ�

%group
�G���W��
%prm
objectCount, memorySize
[in] objectCount : �L���b�V���ł���T�E���h�I�u�W�F�N�g�̍ő吔 (����l:32)
[in] memorySize  : �T�E���h�I�u�W�F�N�g�̃L���b�V�����g�p�ł���ő僁�����T�C�Y (����l:0)

%inst
objectCount �� 0 �̏ꍇ�A�L���b�V�����g�p���܂���B
memorySize �� 0 �̏ꍇ�A�������g�p�ʂɐ�����݂��܂���B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetDirectMusicMode
DirectMusic �̏��������@��ݒ肵�܂��B(����l:LN_DIRECTMUSICMODE_NOT_USE)

%group
�G���W��
%prm
mode
[in] mode : DirectMusic �̏��������@
               LN_DIRECTMUSICMODE_NOT_USE
                   DirectMusic ���g�p���Ȃ�
               LN_DIRECTMUSICMODE_NORMAL
                   �ʏ�
               LN_DIRECTMUSICMODE_THREAD_WAIT
                   �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͑҂�
               LN_DIRECTMUSICMODE_THREAD_REQUEST
                   �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͍Đ���\�񂷂�

%inst
DirectMusic �̏������ɂ͔�r�I���Ԃ�������܂��B
�����������邽�߂ɏ�������p�̃X���b�h�ŏ��������s�����Ƃ��o���܂��B
�Ȃ��ADirectMusic ���g�p���邽�߂ɂ� LNConfig_SetUserWindowHandle �ŃE�B���h�E�n���h����ݒ肷��K�v������܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetDirectMusicReverbLevel
DirectMusic �̃��o�[�u�G�t�F�N�g�̋�����ݒ肵�܂��B(����l:0.75)

%group
�G���W��
%prm
level
[in] level : ���o�[�u�̋��� (0.0 �` 1.0)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_Initialize
���C�u���������������܂��B

%group
�G���W��
%prm
�@

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_InitializeAudio
���C�u���������������܂��B�����@�\�݂̂��g�p����ꍇ�ɌĂяo���܂��B

%group
�G���W��
%prm
�@

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_UpdateFrame
1�t���[�����̍X�V�������s���܂��B

%group
�G���W��
%prm
�@

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
���̖��߂̓O���t�B�b�N�X�Ɠ��͂��X�V���A�w�肳�ꂽ�t���[�����[�g�ɂȂ�悤�ɑҋ@���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_IsEndRequested
�A�v���P�[�V�������I������ׂ������m�F���܂��B

%group
�G���W��
%prm
outRequested
[out] outRequested : �I���v���̗L�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�E�B���h�E�̃N���[�Y�{�^���������ꂽ�ꍇ���A
�A�v���P�[�V�������I������ׂ����ɂ� LN_FALSE ��Ԃ��܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_Terminate
���C�u�����̏I���������s���܂��B

%group
�G���W��
%prm
�@

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetMajor
���W���[�o�[�W�������擾���܂��B

%group
�G���W��
%prm
outMajor
[out] outMajor : �o�[�W�����ԍ����i�[����ϐ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetMinor
�}�C�i�[�o�[�W�������擾���܂��B

%group
�G���W��
%prm
outMinor
[out] outMinor : �o�[�W�����ԍ����i�[����ϐ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetRevision
���r�W�����o�[�W�������擾���܂��B

%group
�G���W��
%prm
outRevision
[out] outRevision : �o�[�W�����ԍ����i�[����ϐ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetBuild
�r���h�o�[�W�������擾���܂��B

%group
�G���W��
%prm
outBuild
[out] outBuild : �o�[�W�����ԍ����i�[����ϐ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetString
�o�[�W������������擾���܂��B

%group
�G���W��
%prm
outStr
[out] outStr : ������ւ��i�[����ϐ�

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_IsAtLeast
�w�肵���o�[�W�����ԍ��ƁA���C�u�����t�@�C���̃R���p�C���o�[�W�����ԍ����r���܂��B

%group
�G���W��
%prm
major, minor, revision, outResult
[in]  major     : ���W���[�o�[�W����
[in]  minor     : �}�C�i�[�o�[�W����
[in]  revision  : ���r�W�����o�[�W����
[out] outResult : ���ʂ��i�[����ϐ�

%inst
�w��o�[�W���� >= �R���p�C���o�[�W���� �ł���ꍇ�ALN_TRUE �ƂȂ�܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNError_GetLastErrorCode
�Ō�ɔ��������G���[�̃G���[�R�[�h���擾���܂��B

%group
��{�@�\
%prm
�@

stat : �G���[�R�[�h
    LN_OK
        ����
    LN_ERROR_UNKNOWN
        �s���ȃG���[
    LN_ERROR_VERIFY
        �O������̌��؃G���[
    LN_ERROR_ARGUMENT
        ����������
    LN_ERROR_INVALID_OPERATION
        �I�u�W�F�N�g�̌��݂̏�Ԃɑ΂��Ė����ȌĂяo�����s��ꂽ
    LN_ERROR_NOT_IMPLEMENTED
        �������̋@�\���Ăяo����
    LN_ERROR_OUT_OF_MEMORY
        �������m�ۂɎ��s
    LN_ERROR_OUT_OF_RANGE
        �l���L���Ȕ͈͂ɑ��݂��Ȃ�
    LN_ERROR_KEY_NOT_FOUND
        �����Ɏg�p�����L�[�����݂��Ȃ�
    LN_ERROR_OVERFLOW
        ���l���Z�ɂ��I�[�o�[�t���[����������
    LN_ERROR_IO
        ���̑���IO�G���[
    LN_ERROR_FILE_NOT_FOUND
        �t�@�C���ɃA�N�Z�X���悤�Ƃ��Ď��s����
    LN_ERROR_DIRECTORY_NOT_FOUND
        �����ȃf�B���N�g���ɃA�N�Z�X���悤�Ƃ���
    LN_ERROR_INVALID_FORMAT
        �t�@�C���╶���񓙂̌`�����s��
    LN_ERROR_END_OF_STREAM
        �X�g���[���̖������z���ăA�N�Z�X���悤�Ƃ���
    LN_ERROR_ENCODING
        �����R�[�h�̕ϊ����A�}�b�s���O�ł��Ȃ������܂��͕s���V�[�P���X����������
    LN_ERROR_WIN32
        WindowsAPI �̃G���[
    LN_ERROR_COM
        COM �̃G���[


%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNError_GetLastErrorMessage
�Ō�ɔ��������G���[�̃G���[���b�Z�[�W���擾���܂��B

%group
��{�@�\
%prm
outStr
[out] outStr : ���b�Z�[�W��������i�[����ϐ��B

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_Release
�I�u�W�F�N�g��������܂��B

%group
��{�@�\
%prm
hadnleObject
[in] hadnleObject : �I�u�W�F�N�g�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�w�肳�ꂽ�I�u�W�F�N�g�̎Q�Ƃ�������܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_AddRef
�I�u�W�F�N�g�̎Q�ƃJ�E���g���C���N�������g���܂��B

%group
��{�@�\
%prm
hadnleObject
[in] hadnleObject : �I�u�W�F�N�g�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
���̖��߂́u�Q�ƃJ�E���g�v�Ƃ����T�O�ɂ��ď\���ɗ���������Ŏg�p���Ă��������B
�Q�ƃJ�E���g�� LNObject_AddRef �ŃC���N�������g����ALNObject_Release �Ńf�N�������g����܂��B
Release �R�ꂪ����ƃ��������[�N�̌����ɂȂ�܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_GetRefCount
�I�u�W�F�N�g�̎Q�ƃJ�E���g�擾���܂��B

%group
��{�@�\
%prm
hadnleObject, count
[in]  hadnleObject : �I�u�W�F�N�g�n���h��
[out] count        : �Q�ƃJ�E���g���i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_GetLength
2D �x�N�g���̒������擾���܂��B

%group
�Z�p�@�\
%prm
vec, outLength
[in]  vec       : �����̊�ɂȂ� 2D �x�N�g��
[out] outLength : �x�N�g���̒������i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_GetSquareLength
2D �x�N�g���̒�����2����擾���܂��B

%group
�Z�p�@�\
%prm
vec, outLength
[in]  vec       : �����̊�ɂȂ� 2D �x�N�g��
[out] outLength : �x�N�g���̒�����2����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_Set
2D �x�N�g���ɒl��ݒ肵�܂��B

%group
�Z�p�@�\
%prm
vec, x, y
[in] vec : �l���i�[���� 2D �x�N�g���ϐ�
[in] x   : X �l
[in] y   : Y �l

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_Normalize
2D �x�N�g���𐳋K�����܂��B

%group
�Z�p�@�\
%prm
vec, outVec
[in]  vec    : �����̊�ɂȂ� 2D �x�N�g��
[out] outVec : ���Z���ʂ��i�[���� 2D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_NormalizeV
2D �x�N�g���𐳋K�����܂��B

%group
�Z�p�@�\
%prm
vec
[in] vec : �����Ώۂ� 2D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_GetLength
3D�x�N�g���̒������擾���܂��B

%group
�Z�p�@�\
%prm
vec, outLength
[in]  vec       : �����̊�ɂȂ�x�N�g��
[out] outLength : �x�N�g���̒������i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_GetSquareLength
3D �x�N�g���̒�����2����v�Z����

%group
�Z�p�@�\
%prm
vec, outLength
[in]  vec       : �����̊�ɂȂ� Vector3
[out] outLength : �x�N�g���̒�����2����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Set
3D�x�N�g���ɒl��ݒ肵�܂��B

%group
�Z�p�@�\
%prm
vec, x, y, z
[in] vec : �l���i�[���� 3D �x�N�g���ϐ�
[in] x   : X �l
[in] y   : Y �l
[in] z   : Z �l

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_SetVZ
2D �x�N�g�� �� Z�l ���w�肵�āA3D�x�N�g���ɒl��ݒ肵�܂��B

%group
�Z�p�@�\
%prm
vec, vec2, z
[out] vec  : �l���i�[���� 3D�x�N�g�� �ϐ�
[in]  vec2 : 2D �x�N�g��
[in]  z    : Z �l

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Normalize
3D �x�N�g���𐳋K�����܂��B

%group
�Z�p�@�\
%prm
vec, outVec
[in]  vec    : �����̊�ɂȂ� 3D �x�N�g��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_NormalizeV
3D �x�N�g���𐳋K�����܂��B

%group
�Z�p�@�\
%prm
vec
[in] vec : �����Ώۂ� 2D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Dot
2 �� 3D �x�N�g���̓��ς��v�Z���܂��B

%group
�Z�p�@�\
%prm
vec1, vec2, dot
[in]  vec1 : �����̊�ɂȂ� 3D �x�N�g��
[in]  vec2 : �����̊�ɂȂ� 3D �x�N�g��
[out] dot  : �x�N�g���̓��ς��i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Cross
2 �� 3D �x�N�g���̊O�ς��v�Z���܂��B

%group
�Z�p�@�\
%prm
vec1, vec2, outVec
[in]  vec1   : �����̊�ɂȂ� 3D �x�N�g��
[in]  vec2   : �����̊�ɂȂ� 3D �x�N�g��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Reflect
�w�肳�ꂽ�@���Ŕ��˂������x�N�g�����v�Z���܂��B

%group
�Z�p�@�\
%prm
vec, normal, outVec
[in]  vec    : �����̊�ɂȂ� 3D �x�N�g��
[in]  normal : �ʕ�����\���@�� 3D �x�N�g��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Slide
�w�肳�ꂽ�@���ƕ������犊��x�N�g�����v�Z���܂��B

%group
�Z�p�@�\
%prm
vec, normal, outVec
[in]  vec    : �����̊�ɂȂ� 3D �x�N�g��
[in]  normal : �ʕ�����\���@�� 3D �x�N�g��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Lerp
2 �� 3D �x�N�g���Ԃ���`��Ԃ��܂��B

%group
�Z�p�@�\
%prm
vec1, vec2, t, outVec
[in]  vec1   : �����̊�ɂȂ� 3D �x�N�g�� ( t = 0.0 �̂Ƃ��̒l )
[in]  vec2   : �����̊�ɂȂ� 3D �x�N�g�� ( t = 1.0 �̂Ƃ��̒l )
[in]  t      : ��Ԓl ( 0.0 �` 1.0 )
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_CatmullRom
�w�肳�ꂽ 3D �x�N�g���� Catmull-Rom ��Ԃ��܂��B

%group
�Z�p�@�\
%prm
vec1, vec2, vec3, vec4, t, outVec
[in]  vec1   : �����̊�ɂȂ� 3D �x�N�g��
[in]  vec2   : �����̊�ɂȂ� 3D �x�N�g�� ( t = 0.0 �̂Ƃ��̒l )
[in]  vec3   : �����̊�ɂȂ� 3D �x�N�g�� ( t = 1.0 �̂Ƃ��̒l )
[in]  vec4   : �����̊�ɂȂ� 3D �x�N�g��
[in]  t      : ��Ԓl ( 0.0 �` 1.0 )
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Transform
�w�肳�ꂽ�s����g�p���č��W�ϊ����܂��B

%group
�Z�p�@�\
%prm
vec, mat, outVec
[in]  vec    : �����̊�ɂȂ� 3D �x�N�g��
[in]  mat    : �����̊�ɂȂ� �s��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�x�N�g���� ( x, y, z, 1 ) �Ƃ��č��W�ϊ����܂��B
���ʂ� w �͏o�͂���܂���B<br>
���ʂ� w = 1 �Ɏˉe���� (x y z �� w �ŏ��Z) �ꍇ��
Vector3TransformCoord() ���g�p���Ă��������B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_TransformCoord
�w�肳�ꂽ�s����g�p���č��W�ϊ����A���ʂ� w = 1 �Ɏˉe���܂��B

%group
�Z�p�@�\
%prm
vec, mat, outVec
[in]  vec    : �����̊�ɂȂ� 3D �x�N�g��
[in]  mat    : �����̊�ɂȂ� �s��
[out] outVec : ���Z���ʂ��i�[���� 3D �x�N�g��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�x�N�g���� ( x, y, z, 1 ) �Ƃ��č��W�ϊ����A
���ʂ� w = 1 �Ɏˉe (x y z �� w �ŏ��Z) ���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector4_Set
4D�x�N�g���ɒl��ݒ肵�܂��B

%group
�Z�p�@�\
%prm
vec, x, y, z, w
[in] vec : �l���i�[����4D�x�N�g���ϐ�
[in] x   : X �l
[in] y   : Y �l
[in] z   : Z �l
[in] w   : W �l

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetRight
�E���������� 3D �x�N�g�����擾���܂��B

%group
�Z�p�@�\
%prm
mat, outVec
[in]  mat    : �����̊�ɂȂ�s��
[out] outVec : ���ʂ��i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetUp
����������� 3D �x�N�g�����擾���܂��B

%group
�Z�p�@�\
%prm
mat, outVec
[in]  mat    : �����̊�ɂȂ�s��
[out] outVec : ���ʂ��i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetFront
���ʕ��������� 3D �x�N�g�����擾���܂��B

%group
�Z�p�@�\
%prm
mat, outVec
[in]  mat    : �����̊�ɂȂ�s��
[out] outVec : ���ʂ��i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetPosition
�ʒu������ 3D �x�N�g�����擾���܂��B

%group
�Z�p�@�\
%prm
mat, outVec
[in]  mat    : �����̊�ɂȂ�s��
[out] outVec : ���ʂ��i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Identity
�P�ʍs����쐬���܂��B

%group
�Z�p�@�\
%prm
mat
[out] mat : ���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Translate
�s��𕽍s�ړ����܂��B

%group
�Z�p�@�\
%prm
mat, x, y, z
[in] mat : ���Z���ʂ��i�[����s��
[in] x   : X �����̈ړ���
[in] y   : Y �����̈ړ���
[in] z   : Z �����̈ړ���

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ�������畽�s�ړ��s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_TranslateVec3
�s��𕽍s�ړ����܂��B

%group
�Z�p�@�\
%prm
mat, vec
[in] mat : ���Z���ʂ��i�[����s��
[in] vec : �ړ���

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ�������畽�s�ړ��s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateX
X ������]���ɂ��čs�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, radian
[in] mat    : ���Z���ʂ��i�[����s��
[in] radian : ��]�p�x ( �������ɑ΂��Ĕ����v��� )

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateY
Y ������]���ɂ��čs�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, radian
[in] mat    : ���Z���ʂ��i�[����s��
[in] radian : ��]�p�x

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateZ
Z ������]���ɂ��čs�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, radian
[in] mat    : ���Z���ʂ��i�[����s��
[in] radian : ��]�p�x

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Rotate
�s�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, xRad, yRad, zRad, rotOrder
[in] mat                            : ���Z���ʂ��i�[����s��
[in] xRad                           : X ������]���Ƃ����p�x
[in] yRad                           : Y ������]���Ƃ����p�x
[in] zRad                           : Z ������]���Ƃ����p�x
[in] rotOrder(LN_ROTATIONORDER_XYZ) : ��]�����̎w��
                                         LN_ROTATIONORDER_XYZ
                                             X �� Y �� Z
                                         LN_ROTATIONORDER_XZY
                                             X �� Z �� Y
                                         LN_ROTATIONORDER_YXZ
                                             Y �� X �� Z
                                         LN_ROTATIONORDER_YZX
                                             Y �� Z �� X
                                         LN_ROTATIONORDER_ZXY
                                             Z �� X �� Y
                                         LN_ROTATIONORDER_ZYX
                                             Z �� Y �� X

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateVec3
�s�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, vec, rotOrder
[in] mat                            : ���Z���ʂ��i�[����s��
[in] vec                            : �e���̉�]�p�x
[in] rotOrder(LN_ROTATIONORDER_XYZ) : ��]�����̎w��
                                         LN_ROTATIONORDER_XYZ
                                             X �� Y �� Z
                                         LN_ROTATIONORDER_XZY
                                             X �� Z �� Y
                                         LN_ROTATIONORDER_YXZ
                                             Y �� X �� Z
                                         LN_ROTATIONORDER_YZX
                                             Y �� Z �� X
                                         LN_ROTATIONORDER_ZXY
                                             Z �� X �� Y
                                         LN_ROTATIONORDER_ZYX
                                             Z �� Y �� X

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateAxis
�C�ӂ̎�����]���ɂ��čs�����]���܂��B (���W�A���P��)

%group
�Z�p�@�\
%prm
mat, axis, radian
[in] mat    : ���Z���ʂ��i�[����s��
[in] axis   : ��]�������� Vector3
[in] radian : ��]�p�x

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ���������]�s������A���݂̍s��ɏ�Z���܂��B<br>
�^����ꂽ�������K������Ă��Ȃ��ꍇ�͐��K���������̂��g���Čv�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateQuaternion
�N�H�[�^�j�I�����g���čs�����]���܂��B

%group
�Z�p�@�\
%prm
mat, qua
[in] mat : ���Z���ʂ��i�[����s��
[in] qua : �����̊�ɂȂ� Quaternion

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�N�H�[�^�j�I�������]�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Scale
�s����X�P�[�����O���܂��B

%group
�Z�p�@�\
%prm
mat, xyz
[in] mat : ���Z���ʂ��i�[����s��
[in] xyz : X Y Z �S�Ă̕����ɓK�p����g�嗦 (X Y Z �𓯂��l�Ŋg��)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ��������X�P�[�����O�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ScaleXYZ
�s����X�P�[�����O���܂��B

%group
�Z�p�@�\
%prm
mat, x, y, z
[in] mat : ���Z���ʂ��i�[����s��
[in] x   : X �����̊g�嗦
[in] y   : Y �����̊g�嗦
[in] z   : Z �����̊g�嗦

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ��������X�P�[�����O�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ScaleVec3
�s����X�P�[�����O���܂��B

%group
�Z�p�@�\
%prm
mat, scale
[in] mat   : ���Z���ʂ��i�[����s��
[in] scale : �e�����̊g�嗦

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ��������X�P�[�����O�s������A���݂̍s��ɏ�Z���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Multiply
�s�����Z���܂��B

%group
�Z�p�@�\
%prm
mat1, mat2, matOut
[in]  mat1   : �����̊�ɂȂ�s��
[in]  mat2   : �����̊�ɂȂ�s��
[out] matOut : ���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Inverse
�t�s������߂܂��B

%group
�Z�p�@�\
%prm
mat, matOut
[in]  mat    : �����̊�ɂȂ�s��
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Transpose
�]�u�s������߂܂��B

%group
�Z�p�@�\
%prm
mat, matOut
[in]  mat    : �����̊�ɂȂ�s��
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ViewTransformLH
������W�n�r���[�s����쐬���܂��B

%group
�Z�p�@�\
%prm
pos, lookAt, upDir, matOut
[in]  pos    : ���_�̈ʒu������ Vector3
[in]  lookAt : �����_������ Vector3
[in]  upDir  : ����������� Vector3
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ViewTransformRH
�E����W�n�r���[�s����쐬���܂��B

%group
�Z�p�@�\
%prm
pos, lookAt, upDir, matOut
[in]  pos    : ���_�̈ʒu������ Vector3
[in]  lookAt : �����_������ Vector3
[in]  upDir  : ����������� Vector3
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_PerspectiveFovLH
������W�n�ˉe�s����쐬���܂��B

%group
�Z�p�@�\
%prm
fovY, aspect, nearZ, farZ, matOut
[in]  fovY   : Y �����ւ̎���p (���W�A���P��)
[in]  aspect : �A�X�y�N�g��
[in]  nearZ  : �߂��̃r���[���ʂ� Z �l
[in]  farZ   : �����̃r���[���ʂ� Z �l
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_PerspectiveFovRH
�E����W�n�ˉe�s��쐬���܂��B

%group
�Z�p�@�\
%prm
fovY, aspect, nearZ, farZ, matOut
[in]  fovY   : Y �����ւ̎���p (���W�A���P��)
[in]  aspect : �A�X�y�N�g��
[in]  nearZ  : �߂��̃r���[���ʂ� Z �l
[in]  farZ   : �����̃r���[���ʂ� Z �l
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_OrthoLH
������W�n���ˉe�s��쐬���܂��B

%group
�Z�p�@�\
%prm
width, height, nearZ, farZ, matOut
[in]  width  : �r���[�̕�
[in]  height : �r���[�̍���
[in]  nearZ  : �߂��̃r���[���ʂ� Z �l
[in]  farZ   : �����̃r���[���ʂ� Z �l
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_OrthoRH
�E����W�n���ˉe�s��쐬���܂��B

%group
�Z�p�@�\
%prm
width, height, nearZ, farZ, matOut
[in]  width  : �r���[�̕�
[in]  height : �r���[�̍���
[in]  nearZ  : �߂��̃r���[���ʂ� Z �l
[in]  farZ   : �����̃r���[���ʂ� Z �l
[out] matOut : ���Z���ʂ��i�[����s��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetEulerAngles
��]�s�񂩂�I�C���[�p���v�Z���܂��B

%group
�Z�p�@�\
%prm
mat, outVec
[in]  mat    : �����̊�ɂȂ�s��
[out] outVec : �e���̉�]�p�x(���W�A��)���i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Decompose
�s����X�P�[�����O�A��]�A�ړ������ɕ������܂��B

%group
�Z�p�@�\
%prm
mat, scale, rot, trans
[in]  mat   : �����̊�ɂȂ�s��
[out] scale : �e���̃X�P�[�����O�ʂ��i�[���� Vector3 �ϐ�
[out] rot   : �e���̉�]�����i�[���� Quaternion �ϐ�
[out] trans : �e���̕��s�ړ��ʂ��i�[���� Vector3 �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Set
�N�H�[�^�j�I���ɒl��ݒ肵�܂��B

%group
�Z�p�@�\
%prm
qua, x, y, z, w
[in] qua : �l���i�[���� Quaternion �ϐ�
[in] x   : X �l
[in] y   : Y �l
[in] z   : Z �l
[in] w   : W �l

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Identity
�P�ʃN�H�[�^�j�I�����쐬���܂��B

%group
�Z�p�@�\
%prm
qua
[out] qua : ���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationAxis
�C�ӂ̎�����]���ɂ��ĉ�]����N�H�[�^�j�I�����쐬���܂��B

%group
�Z�p�@�\
%prm
axis, r, outQua
[in]  axis   : ��]�������� Vector3 �ϐ�
[in]  r      : ��]�p�x (���W�A��)
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�^����ꂽ�������K������Ă��Ȃ��ꍇ�͐��K�����ꂽ�����g�p���܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationMatrix
��]�s�񂩂�N�H�[�^�j�I�����쐬���܂��B

%group
�Z�p�@�\
%prm
mat, outQua
[in]  mat    : �����̊�ɂȂ�s��
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationYawPitchRoll
���[�A�s�b�`�A����у��[�����w�肵�ăN�H�[�^�j�I�����쐬���܂��B

%group
�Z�p�@�\
%prm
yaw, pitch, roll, outQua
[in]  yaw    : y ���𒆐S�Ƃ��郈�[ (���W�A���P��)
[in]  pitch  : x ���𒆐S�Ƃ���s�b�` (���W�A���P��)
[in]  roll   : z ���𒆐S�Ƃ��郍�[�� (���W�A���P��)
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Normalize
�N�H�[�^�j�I���𐳋K�����܂��B

%group
�Z�p�@�\
%prm
qua, outQua
[in]  qua    : �����̊�ɂȂ�N�H�[�^�j�I��
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Conjugate
�N�H�[�^�j�I���̋������v�Z���܂��B

%group
�Z�p�@�\
%prm
qua, outQua
[in]  qua    : �����̊�ɂȂ�N�H�[�^�j�I��
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Multiply
2 �̃N�H�[�^�j�I���̐ς��v�Z���܂��B

%group
�Z�p�@�\
%prm
qua1, qua2, outQua
[in]  qua1   : �����̊�ɂȂ�N�H�[�^�j�I��
[in]  qua2   : �����̊�ɂȂ�N�H�[�^�j�I��
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Slerp
2 �̃N�H�[�^�j�I�������ʐ��`��Ԃ��܂��B

%group
�Z�p�@�\
%prm
qua1, qua2, t, outQua
[in]  qua1   : �����̊�ɂȂ�N�H�[�^�j�I��
[in]  qua2   : �����̊�ɂȂ�N�H�[�^�j�I��
[in]  t      : ��Ԓl
[out] outQua : ���Z���ʂ��i�[���� Quaternion �ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGM
BGM �����t���܂��B

%group
�����@�\
%prm
filePath, volume, pitch, fadeTime
[in] filePath      : �t�@�C���p�X
[in] volume(1.0f)  : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)   : �s�b�` (0.5�`2.0)
[in] fadeTime(0.0) : �t�F�[�h�C���ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
BGM �͂ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ����܂��B
�V���� BGM �����t�悤�Ƃ����ꍇ�A�Đ����� BGM �͒�~���܂��B
�t�F�[�h�C�����Ԃ��w�肵���ꍇ�̓N���X�t�F�[�h���Ȃ��特����؂�ւ��܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGMMem
��������̉����t�@�C���f�[�^���g�p���� BGM �����t���܂��B

%group
�����@�\
%prm
data, dataSize, volume, pitch, fadeTime
[in] data          : ��������̉����t�@�C���f�[�^
[in] dataSize      : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(1.0f)  : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)   : �s�b�` (0.5�`2.0)
[in] fadeTime(0.0) : �t�F�[�h�C���ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopBGM
BGM �̉��t���~���܂��B

%group
�����@�\
%prm
fadeTime
[in] fadeTime(0.0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGS
BGS �����t���܂��B

%group
�����@�\
%prm
filePath, volume, pitch, fadeTime
[in] filePath      : �t�@�C���p�X
[in] volume(1.0f)  : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)   : �s�b�` (0.5�`2.0)
[in] fadeTime(0.0) : �t�F�[�h�C���ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
BGS �͕����̉��Ȃǂ̊����ł��B
BGM �Ɠ��l�A�ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ����܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGSMem
��������̉����t�@�C���f�[�^���� BGS �����t���܂��B

%group
�����@�\
%prm
data, dataSize, volume, pitch, fadeTime
[in] data          : ��������̉����t�@�C���f�[�^
[in] dataSize      : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(1.0f)  : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)   : �s�b�` (0.5�`2.0)
[in] fadeTime(0.0) : �t�F�[�h�C���ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopBGS
BGS �̉��t���~���܂��B�A

%group
�����@�\
%prm
fadeTime
[in] fadeTime(0.0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayME
ME �����t���܂��B

%group
�����@�\
%prm
filePath, volume, pitch
[in] filePath     : �t�@�C���p�X
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
ME �� ���x���A�b�v��A�C�e���擾���̃t�@���t�@�[�����̌��ʉ��y�ł��B
ME �͂ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ������Ɉ�x�������t����܂��B
BGM ���Đ����ł���Έꎞ��~���AME �I����ɍĊJ�����܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayMEMem
��������̉����t�@�C���f�[�^���� ME �����t���܂��B

%group
�����@�\
%prm
data, dataSize, volume, pitch
[in] data         : ��������̉����t�@�C���f�[�^
[in] dataSize     : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopME
ME �̉��t���~���܂��B

%group
�����@�\
%prm
�@

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE
SE �����t���܂��B

%group
�����@�\
%prm
filePath, volume, pitch
[in] filePath     : �t�@�C���p�X
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
SE �̓Q�[�����̌��ʉ��ł��B SE �͓����ɕ����Đ����邱�Ƃ��ł��A�������������t�����ꍇ�� �d�˂�������ĉ��t����܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3D
SE �����t���܂��B(3D �T�E���h)

%group
�����@�\
%prm
filePath, position, distance, volume, pitch
[in] filePath     : �t�@�C���p�X
[in] position     : 3D ��ԏ�̍��W
[in] distance     : ��������
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DXYZ
SE �����t���܂��B(3D �T�E���h)

%group
�����@�\
%prm
filePath, x, y, z, distance, volume, pitch
[in] filePath     : �t�@�C���p�X
[in] x            : 3D ��ԏ�� X ���W
[in] y            : 3D ��ԏ�� Y ���W
[in] z            : 3D ��ԏ�� Z ���W
[in] distance     : ��������
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySEMem
��������̉����f�[�^���� SE �����t���܂��B

%group
�����@�\
%prm
data, dataSize, volume, pitch
[in] data         : ��������̉����t�@�C���f�[�^
[in] dataSize     : �f�[�^�T�C�Y (�o�C�g�P��)
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DMem
��������̉����t�@�C���f�[�^���� SE �����t���܂��B (3D �T�E���h)

%group
�����@�\
%prm
data, dataSize, position, distance, volume, pitch
[in] data         : ��������̉����t�@�C���f�[�^
[in] dataSize     : �f�[�^�T�C�Y (�o�C�g�P��)
[in] position     : 3D ��ԏ�̍��W
[in] distance     : ��������
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DMemXYZ
��������̉����t�@�C���f�[�^���� SE �����t���܂��B(3D �T�E���h)

%group
�����@�\
%prm
data, dataSize, x, y, z, distance, volume, pitch
[in] data         : ��������̉����t�@�C���f�[�^
[in] dataSize     : �f�[�^�T�C�Y (�o�C�g�P��)
[in] x            : 3D ��ԏ�� X ���W
[in] y            : 3D ��ԏ�� Y ���W
[in] z            : 3D ��ԏ�� Z ���W
[in] distance     : ��������
[in] volume(1.0f) : �{�����[�� (0.0�`1.0)
[in] pitch(1.0f)  : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopSE
���ׂĂ� SE �̉��t���~���܂��B

%group
�����@�\
%prm
�@

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_SetBGMVolume
�Đ�����BGM�̉��ʂ�ݒ肵�܂��B(�t�F�[�h�A�E�g���͖���)

%group
�����@�\
%prm
volume, fadeTime
[in] volume        : �{�����[�� (0.0�`1.0)
[in] fadeTime(0.0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_SetBGSVolume
�Đ�����BGS�̉��ʂ�ݒ肵�܂��B(�t�F�[�h�A�E�g���͖���)

%group
�����@�\
%prm
volume, fadeTime
[in] volume        : �{�����[�� (0.0�`1.0)
[in] fadeTime(0.0) : �t�F�[�h�A�E�g�ɂ����鎞�� (�b)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetPosition
3D�����̃��X�i�[�̈ʒu��ݒ肵�܂��B

%group
�����@�\
%prm
position
[in] position : 3D ��ԏ�̍��W

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetDirection
3D�����̃��X�i�[�̐��ʕ�����ݒ肵�܂��B

%group
�����@�\
%prm
direction
[in] direction : ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetUpDirection
3D�����̃��X�i�[�̏������ݒ肵�܂��B(���ʕ����Ƃ͒����ł��邱��)

%group
�����@�\
%prm
direction
[in] direction : �����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetVelocity
3D�����̃��X�i�[�̑��x��ݒ肵�܂��B

%group
�����@�\
%prm
velocity
[in] velocity : ���x

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetPositionXYZ
3D�����̃��X�i�[�̈ʒu��ݒ肵�܂��B

%group
�����@�\
%prm
x, y, z
[in] x : 3D ��ԏ�� X ���W
[in] y : 3D ��ԏ�� Y ���W
[in] z : 3D ��ԏ�� Z ���W

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetDirectionXYZ
3D�����̃��X�i�[�̐��ʕ�����ݒ肵�܂��B

%group
�����@�\
%prm
x, y, z
[in] x : ������ X ����
[in] y : ������ Y ����
[in] z : ������ Z ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetUpDirectionXYZ
3D�����̃��X�i�[�̏������ݒ肵�܂��B(���ʕ����Ƃ͒����ł��邱��)

%group
�����@�\
%prm
x, y, z
[in] x : ������ X ����
[in] y : ������ Y ����
[in] z : ������ Z ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetVelocityXYZ
3D�����̃��X�i�[�̑��x��ݒ肵�܂��B

%group
�����@�\
%prm
x, y, z
[in] x : ���x�� X ����
[in] y : ���x�� Y ����
[in] z : ���x�� Z ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Create
�t�@�C������T�E���h�I�u�W�F�N�g���쐬���܂��B

%group
�����@�\
%prm
filePath, sound
[in]  filePath : �����t�@�C���̃p�X
[out] sound    : �쐬���ꂽ�T�E���h�I�u�W�F�N�g�̃n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_CreateMem
��������̉����t�@�C���f�[�^����T�E���h�I�u�W�F�N�g���쐬���܂��B

%group
�����@�\
%prm
data, dataSize, sound
[in]  data     : ��������̉����f�[�^��
[in]  dataSize : �f�[�^�T�C�Y (�o�C�g�P��)
[out] sound    : �쐬���ꂽ�T�E���h�I�u�W�F�N�g�̃n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetVolume
�T�E���h�̃{�����[�����擾���܂��B

%group
�����@�\
%prm
sound, outVolume
[in]  sound     : �T�E���h�n���h��
[out] outVolume : �{�����[�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetVolume
�T�E���h�̃{�����[����ݒ肵�܂��B

%group
�����@�\
%prm
sound, volume
[in] sound  : �T�E���h�n���h��
[in] volume : �{�����[�� (0.0�`1.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPitch
�T�E���h�̃s�b�`���擾���܂��B

%group
�����@�\
%prm
sound, outPitch
[in]  sound    : �T�E���h�n���h��
[out] outPitch : �s�b�`���i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetPitch
�T�E���h�̃s�b�`��ݒ肵�܂��B

%group
�����@�\
%prm
sound, pitch
[in] sound : �T�E���h�n���h��
[in] pitch : �s�b�` (0.5�`2.0)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_IsLoopEnabled
�T�E���h�̃��[�v�Đ����L���ł��邩���m�F���܂��B

%group
�����@�\
%prm
sound, outEnabled
[in]  sound      : �T�E���h�n���h��
[out] outEnabled : ���[�v�Đ��̗L����Ԃ��i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetLoopEnabled
�T�E���h�̃��[�v�Đ��̗L����ݒ肵�܂��B

%group
�����@�\
%prm
sound, loopEnable
[in] sound      : �T�E���h�n���h��
[in] loopEnable : LN_TRUE = ���[�v�Đ����� / LN_FALSE = ���Ȃ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Is3DEnabled
�T�E���h�� 3D �����ł��邩���m�F���܂��B

%group
�����@�\
%prm
sound, outEnabled
[in]  sound      : �T�E���h�n���h��
[out] outEnabled : ��Ԃ��i�[����ϐ� (LN_TRUE = 3D���� / LN_FALSE = �� 3D)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Set3DEnabled
�T�E���h�� 3D �����ł��邩��ݒ肵�܂��B

%group
�����@�\
%prm
sound, enabled
[in] sound   : �T�E���h�n���h��
[in] enabled : LN_TRUE = 3D���� / LN_FALSE = ��3D

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayingMode
�T�E���h�Đ����̉����f�[�^�̍Đ����@���擾���܂��B

%group
�����@�\
%prm
sound, outMode
[in]  sound   : �T�E���h�n���h��
[out] outMode : �Đ����@���i�[����ϐ�
                   LN_SOUNDPLAYINGMODE_UNKNOWN
                       �s���ȍĐ����@
                   LN_SOUNDPLAYINGMODE_AUTO
                       �����I�� (�f�t�H���g�ł̓f�R�[�h��̃T�C�Y�� 10000 �o�C�g�ȏ�ɂȂ�ꍇ�̓X�g���[�~���O�A�����łȂ��ꍇ�̓I���������Đ��ɂȂ�܂�)
                   LN_SOUNDPLAYINGMODE_ONMEMORY
                       �I��������
                   LN_SOUNDPLAYINGMODE_STREAMING
                       �X�g���[�~���O
                   LN_SOUNDPLAYINGMODE_MIDI
                       SMF

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetPlayingMode
�T�E���h�Đ����̉����f�[�^�̍Đ����@��ݒ肵�܂��B

%group
�����@�\
%prm
sound, mode
[in] sound : �T�E���h�n���h��
[in] mode  : �ǂݍ��ݕ��@
                LN_SOUNDPLAYINGMODE_UNKNOWN
                    �s���ȍĐ����@
                LN_SOUNDPLAYINGMODE_AUTO
                    �����I�� (�f�t�H���g�ł̓f�R�[�h��̃T�C�Y�� 10000 �o�C�g�ȏ�ɂȂ�ꍇ�̓X�g���[�~���O�A�����łȂ��ꍇ�̓I���������Đ��ɂȂ�܂�)
                LN_SOUNDPLAYINGMODE_ONMEMORY
                    �I��������
                LN_SOUNDPLAYINGMODE_STREAMING
                    �X�g���[�~���O
                LN_SOUNDPLAYINGMODE_MIDI
                    SMF

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayingState
�T�E���h�̍Đ���Ԃ��擾���܂��B

%group
�����@�\
%prm
sound, outState
[in]  sound    : �T�E���h�n���h��
[out] outState : ��Ԃ��i�[����ϐ�
                    LN_SOUNDPLAYINGSTATE_STOPPED
                        ��~��
                    LN_SOUNDPLAYINGSTATE_PLAYING
                        �Đ���
                    LN_SOUNDPLAYINGSTATE_PAUSING
                        �ꎞ��~��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayedSamples
�T�E���h�̍Đ������T���v�������擾���܂��B

%group
�����@�\
%prm
sound, outSamples
[in]  sound      : �T�E���h�n���h��
[out] outSamples : �Đ������T���v�������i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetTotalSamples
�T�E���h�̉����f�[�^�S�̂̃T���v�������擾���܂��B

%group
�����@�\
%prm
sound, outSamples
[in]  sound      : �T�E���h�n���h��
[out] outSamples : �����f�[�^�S�̂̃T���v�������i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetSamplingRate
�T�E���h�̃T���v�����O���[�g���擾���܂��B

%group
�����@�\
%prm
sound, outRate
[in]  sound   : �T�E���h�n���h��
[out] outRate : �T���v�����O���[�g���i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterPosition
�T�E���h�� 3D �����Ƃ��Ă̈ʒu��ݒ肵�܂��B

%group
�����@�\
%prm
sound, position
[in] sound    : �T�E���h�n���h��
[in] position : 3D ��ԏ�̍��W

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterVelocity
�T�E���h�� 3D �����Ƃ��Ă̑��x��ݒ肵�܂��B

%group
�����@�\
%prm
sound, velocity
[in] sound    : �T�E���h�n���h��
[in] velocity : ���x

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterMaxDistance
�T�E���h�� 3D �����̌������� (�������Ȃ��Ȃ鋗��) ��ݒ肵�܂��B(default:100)

%group
�����@�\
%prm
sound, distance
[in] sound    : �T�E���h�n���h��
[in] distance : ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetLoopRange
�T�E���h�̃��[�v�Đ��͈̔͂�ݒ肵�܂��B

%group
�����@�\
%prm
sound, begin, length
[in] sound  : �T�E���h�n���h��
[in] begin  : ���[�v�̈�̐擪�ʒu (�T���v�����P��)
[in] length : ���[�v�̈撷�� (�T���v�����P��)

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
begin �� length �� 0 ���w�肷��ƁA�S�̂����[�v�̈�Ƃ��Đݒ肵�܂��B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Play
�T�E���h���Đ����܂��B

%group
�����@�\
%prm
sound
[in] sound : �T�E���h�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Stop
�T�E���h�̍Đ����~���܂��B

%group
�����@�\
%prm
sound
[in] sound : �T�E���h�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Pause
�T�E���h�̍Đ����ꎞ��~���܂��B

%group
�����@�\
%prm
sound
[in] sound : �T�E���h�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Resume
�T�E���h�̍Đ����ꎞ��~���܂��B

%group
�����@�\
%prm
sound
[in] sound : �T�E���h�n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_FadeVolume
�T�E���h���ʂ̃t�F�[�h������s���܂��B

%group
�����@�\
%prm
sound, targetVolume, time, behavior
[in] sound        : �T�E���h�n���h��
[in] targetVolume : �ύX��̉���
[in] time         : �t�F�[�h�ɂ����鎞�� (�b)
[in] behavior     : �t�F�[�h������̓���̎w��
                       LN_SOUNDFADEBEHAVIORBEHAVIOR_CONTINUE
                           �p��(�Ȃɂ����Ȃ�)
                       LN_SOUNDFADEBEHAVIOR_STOP
                           ��~����
                       LN_SOUNDFADEBEHAVIOR_STOP_RESET
                           ��~���āA���ʂ����̒l�ɖ߂�
                       LN_SOUNDFADEBEHAVIOR_PAUSE
                           �ꎞ��~����
                       LN_SOUNDFADEBEHAVIOR_PAUSE_RESET
                           �ꎞ��~���āA���ʂ����̒l�ɖ߂�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterPositionXYZ
�T�E���h�� 3D �����Ƃ��Ă̈ʒu��ݒ肵�܂��B

%group
�����@�\
%prm
sound, x, y, z
[in] sound : �T�E���h�n���h��
[in] x     : 3D ��ԏ�� X ���W
[in] y     : 3D ��ԏ�� Y ���W
[in] z     : 3D ��ԏ�� Z ���W

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterVelocityXYZ
�T�E���h�� 3D �����Ƃ��Ă̑��x��ݒ肵�܂��B

%group
�����@�\
%prm
sound, x, y, z
[in] sound : �T�E���h�n���h��
[in] x     : ���x�� X ����
[in] y     : ���x�� Y ����
[in] z     : ���x�� Z ����

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture_GetSize
�e�N�X�`���̃T�C�Y���擾���܂��B

%group
�O���t�B�b�N�X�@�\
%prm
texture, outSize
[in]  texture : �e�N�X�`���n���h��
[out] outSize : �T�C�Y���i�[����ϐ��̃A�h���X

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture2D_Create
�T�C�Y���w�肵��2D�e�N�X�`�����쐬���܂��B

%group
�O���t�B�b�N�X�@�\
%prm
width, height, format, mipmap, outTexture2D
[in]  width                   : �e�N�X�`���̕� (�s�N�Z���P��)
[in]  height                  : �e�N�X�`���̍��� (�s�N�Z���P��)
[in]  format(LN_FMT_A8R8G8B8) : �e�N�X�`���̃s�N�Z���t�H�[�}�b�g
                                   LN_FMT_UNKNOWN
                                       �s���ȃt�H�[�}�b�g
                                   LN_FMT_A8R8G8B8
                                       32 �r�b�g�̃A���t�@�t���t�H�[�}�b�g
                                   LN_FMT_X8R8G8B8
                                       32 �r�b�g�̃A���t�@�����t�H�[�}�b�g
                                   LN_FMT_A16B16G16R16F
                                       64 �r�b�g�̕��������_�t�H�[�}�b�g
                                   LN_FMT_A32B32G32R32F
                                       128 �r�b�g�̕��������_�t�H�[�}�b�g
                                   LN_FMT_D24S8
                                       32 �r�b�g�� Z �o�b�t�@�t�H�[�}�b�g
[in]  mipmap(LN_FALSE)        : �~�b�v�}�b�v�̗L��
[out] outTexture2D            : �쐬���ꂽ2D�e�N�X�`���̃n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture2D_CreateFromFile
�t�@�C������2D�e�N�X�`�����쐬���܂��B

%group
�O���t�B�b�N�X�@�\
%prm
filePath, outTexture2D
[in]  filePath     : �摜�t�@�C���̃p�X
[out] outTexture2D : �쐬���ꂽ2D�e�N�X�`���̃n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�S�Ẵv���b�g�t�H�[���ŃT�|�[�g����Ă���t�@�C���t�H�[�}�b�g�� .png �ł��B
�O���t�B�b�N�XAPI �� DirectX9 ���g�p���Ă���ꍇ�� MSDN �� D3DXCreateTextureFromFileEx ���Q�Ƃ��Ă��������B

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSceneNode_SetPosition
�m�[�h�̈ʒu��ݒ肵�܂��B

%group
�V�[���O���t
%prm
sceneNode, position
[in] sceneNode : �V�[���m�[�h�n���h��
[in] position  : ���W

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite_GetTexture
�X�v���C�g�ɐݒ肳��Ă���e�N�X�`�����擾���܂��B

%group
�V�[���O���t
%prm
sprite, outTexture
[in]  sprite     : �X�v���C�g�n���h��
[out] outTexture : �e�N�X�`���n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite_SetTexture
�X�v���C�g�Ƀe�N�X�`����ݒ肵�܂��B

%group
�V�[���O���t
%prm
sprite, texture
[in] sprite  : �X�v���C�g�n���h��
[in] texture : �e�N�X�`���n���h��

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite2D_Create
2D�X�v���C�g�I�u�W�F�N�g���쐬���܂��B

%group
�V�[���O���t
%prm
outSprite2D
[out] outSprite2D : �쐬���ꂽ2D�X�v���C�g�n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite2D_CreateFromTexture
2D�X�v���C�g�I�u�W�F�N�g���쐬���܂��B

%group
�V�[���O���t
%prm
texture, outSprite2D
[in]  texture     : 2D�X�v���C�g���\������e�N�X�`���̃n���h��
[out] outSprite2D : �쐬���ꂽ2D�X�v���C�g�n���h�����i�[����ϐ�

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst


%href

