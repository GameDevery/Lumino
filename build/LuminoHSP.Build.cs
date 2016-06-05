using System;
using System.IO;
using LuminoBuildTool;
using System.Text.RegularExpressions;

class LuminoHSPRule : ModuleRule
{
    private string _msbuild;
    private string _SHFBROOT;

    /// <summary>
    /// ���[���̖��O
    /// </summary>
    public override string Name
    {
        get { return "hspb"; }
    }

    /// <summary>
    /// �O������̊m�F
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        // MSBuild
        _msbuild = Utils.FindMSBuild();
        if (_msbuild == null)
        {
            Logger.WriteLineError("Not found MSBuild or xbuild.");
            return;
        }

        Buildable = true;
    }

    /// <summary>
    /// �r���h���s
    /// </summary>
    public override void Build(Builder builder)
    {
        var hspDir = builder.LuminoBindingsDir + "HSP/";

        Utils.CallProcess(_msbuild, hspDir + "LuminoHSP/LuminoHSP.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m");
    }
}
