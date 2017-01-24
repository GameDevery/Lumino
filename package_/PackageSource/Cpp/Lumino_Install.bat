
set LUMINO_DIR=%~dp0

@echo off
setx "LuminoInstallDir" %LUMINO_DIR%

echo ----------
echo %LUMINO_DIR% �����ϐ� LuminoInstallDir �ɐݒ肵�܂����B���̃t�H���_�͈ړ�������폜���Ȃ��悤�ɂ��Ă��������B
echo ----------

set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS2013_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2013\Templates\ProjectTemplates\
set VS2015_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

echo %VS2013_TEMPLATES_DIR%
echo %VS2015_TEMPLATES_DIR%

if exist "%VS2013_TEMPLATES_DIR%" (
copy "tools\VS2013ProjectTemplate\LuminoProjectCpp.zip" "%VS2013_TEMPLATES_DIR%\LuminoProjectCpp.zip"
echo "VisualStudio2015 �Ƀv���W�F�N�g�e���v���[�g���C���X�g�[�����܂����B"
echo "[�V�����v���W�F�N�g] �� [�e���v���[�g] > [Visual C++] ���m�F���Ă��������B"
) else (
echo "VisualStudio2013 �̃v���W�F�N�g�e���v���[�g�t�H���_��������Ȃ��������߁A�e���v���[�g�̃C���X�g�[�����s���܂���ł����B"
)

if exist "%VS2015_TEMPLATES_DIR%" (
copy "tools\VS2015ProjectTemplate\LuminoProjectCpp.zip" "%VS2015_TEMPLATES_DIR%\LuminoProjectCpp.zip"
echo "VisualStudio2015 �Ƀv���W�F�N�g�e���v���[�g���C���X�g�[�����܂����B"
echo "[�V�����v���W�F�N�g] �� [�e���v���[�g] > [Visual C++] ���m�F���Ă��������B"
) else (
echo "VisualStudio2015 �̃v���W�F�N�g�e���v���[�g�t�H���_��������Ȃ��������߁A�e���v���[�g�̃C���X�g�[�����s���܂���ł����B"
)

pause
