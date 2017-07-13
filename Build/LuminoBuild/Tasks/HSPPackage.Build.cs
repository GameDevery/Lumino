using System;
using System.IO;
using LuminoBuild;
using System.Text;

class HSPPackageRule : BuildTask
{
    /// <summary>
    /// ���[�������s���邽�߂̃R�}���h��
    /// </summary>
    public override string CommandName { get { return "hsp-p"; } }

    /// <summary>
    /// ���[���̐���
    /// </summary>
    public override string Description { get { return "Make package for HSP."; } }

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
        string hspDir = builder.LuminoBindingsDir + "HSP/";
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoHSP_" + builder.VersionString + "/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoHSP_" + builder.VersionString + ".zip";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/HSP/";
        Directory.CreateDirectory(releaseDir);

        // dll
        Utils.CopyFile(hspDir + "LuminoHSP/Release/LuminoHSP.dll", releaseDir);

        // header, help
        Utils.CopyFile(hspDir + "lumino.as", releaseDir);
        Utils.CopyFile(hspDir + "lumino.hs", releaseDir);
        Utils.CopyFile(hspDir + "CommandList.txt", releaseDir);

        // sample
        Utils.CopyDirectory(hspDir + "Samples", releaseDir + "sample");

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
