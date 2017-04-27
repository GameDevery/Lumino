using System;
using System.IO;
using LuminoBuildTool;
using System.Text;

class CPackageRule : ModuleRule
{
    /// <summary>
    /// ���[�������s���邽�߂̃R�}���h��
    /// </summary>
    public override string CommandName { get { return "c-p"; } }

    /// <summary>
    /// ���[���̐���
    /// </summary>
    public override string Description { get { return "Make package for C."; } }

    /// <summary>
    /// �O������̊m�F
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        Buildable = true;
    }

    /// <summary>
    /// �r���h���s
    /// </summary>
    public override void Build(Builder builder)
    {
        string capiDir = builder.LuminoBindingsDir + "C_API/";
        string capiBuildDir = capiDir + "build/";
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoC_" + builder.VersionString + "/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoC_" + builder.VersionString + ".zip";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/C/";
        Directory.CreateDirectory(releaseDir);

        Logger.WriteLine("copying files...");

        // include
        Utils.CopyDirectory(capiDir + "include", releaseDir + "include", true);
        
        // lib
        Directory.CreateDirectory(releaseDir + "lib/x86");
        Utils.CopyFile(builder.LuminoLibDir + "MSVC140/x86/Debug/LuminoCd.dll", releaseDir + "lib/x86");
        Utils.CopyFile(builder.LuminoLibDir + "MSVC140/x86/Debug/LuminoCd.lib", releaseDir + "lib/x86");
        Utils.CopyFile(builder.LuminoLibDir + "MSVC140/x86/Release/LuminoC.dll", releaseDir + "lib/x86");
        Utils.CopyFile(builder.LuminoLibDir + "MSVC140/x86/Release/LuminoC.lib", releaseDir + "lib/x86");

        // doc
        Directory.CreateDirectory(releaseDir + "doc");
        Utils.CopyDirectory(capiBuildDir + "html", releaseDir + "doc");

        // Readme.txt (�o�[�W�������𖄂ߍ���)
        string text = File.ReadAllText(pkgSrcDir + "Readme.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);

        // .zip �Ɉ��k����
        Logger.WriteLine("compressing files...");
        Utils.CreateZipFile(releaseDir, zipFilePath);
    }
}
