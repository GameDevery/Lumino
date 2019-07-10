﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakeReleasePackage : BuildTask
    {
        public override string CommandName => "MakeReleasePackage";

        public override string Description => "MakeReleasePackage";

        public bool FileMoving = false;

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);

            string targetRootDir = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName);
            Directory.CreateDirectory(targetRootDir);

            string nativeEngineRoot = Path.Combine(targetRootDir, "Engine", "Native");
            string nativeEngineLib = Path.Combine(nativeEngineRoot, "lib");
            Directory.CreateDirectory(nativeEngineRoot);
            Directory.CreateDirectory(nativeEngineLib);

            // docs
            {
                // Readme
                Utils.GenerateFile(
                    Path.Combine(targetRootDir, "Readme.txt"),
                    Path.Combine(builder.LuminoPackageSourceDir, "Readme.txt.template"),
                    new Dictionary<string, string> { { "%%LuminoVersion%%", builder.VersionString } });
            }

            // C++ Engine (common)
            {
                string nativeEngineCMakeDir = Path.Combine(nativeEngineRoot, "lib", "cmake");
                Directory.CreateDirectory(nativeEngineCMakeDir);

                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoSetup.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoSetup.cmake"), true);
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoCommon.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoCommon.cmake"), true);
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoConfig.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoConfig.cmake"), true);

                File.Copy(
                    Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "ImportExternalLibraries.cmake"), true);

                File.WriteAllText(
                    Path.Combine(nativeEngineRoot, ".gitignore"),
                    "include/\nlib/\n");
            }

            // C++ Engine
            {
                // include files
                {
                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoRootDir, "include"),
                        Path.Combine(nativeEngineRoot, "include"));
                }

                // lib files
                CopyEngineLibs(builder, tempInstallDir, nativeEngineRoot, FileMoving);

                // bin files
                {
                    if (Utils.IsWin32)
                    {
                        Utils.CopyDirectory(
                            Path.Combine(tempInstallDir, "MSVC2017-x86-MT", "bin"),
                            Path.Combine(targetRootDir, "Tools"));

                        Utils.DownloadFile(
                            BuildEnvironment.VSWhereUrl,
                            Path.Combine(targetRootDir, "Tools", "vswhere.exe"));
                    }
                    else if (Utils.IsMac)
                    {
                        Utils.CopyDirectory(
                            Path.Combine(tempInstallDir, "macOS-Release", "bin"),
                            Path.Combine(targetRootDir, "Tools"));

                        string file = Path.Combine(targetRootDir, "setup.sh");
                        File.Copy(
                            Path.Combine(builder.LuminoRootDir, "Tools", "PackageSource", "macOS", "setup.sh"),
                            file, true);

                        Utils.chmod(file, Utils.S_0755);
                    }
                }
            }

            // Templates
            {
                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoToolsDir, "LuminoCLI", "Templates", "NativeProject"),
                    Path.Combine(targetRootDir, "Tools", "Templates", "NativeProject"));

                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoToolsDir, "LuminoCLI", "Templates", "SimpleDesktop"),
                    Path.Combine(targetRootDir, "Tools", "Templates", "SimpleDesktop"));
            }

            // FIXME: CI サーバのストレージ不足対策
            if (FileMoving)
            {
                Directory.Delete(tempInstallDir, true);

                foreach (var arch in BuildEnvironment.TargetArchs)
                {
                    var path = Path.Combine(builder.LuminoBuildDir, arch.SourceDirName);
                    if (Directory.Exists(path))
                        Directory.Delete(path, true);
                }
            }
        }

        public static string[] externalLibs = new string[]
        {
                "Box2D",
                "bullet3",
                "freetype2",
                "glad",
                "glfw",
                "glslang",
                "libpng",
                "ogg",
                "openal-soft",
                "pcre",
                "SDL2",
                "SPIRV-Cross",
                "tmxlite",
                "vorbis",
                "zlib",
        };

        public static void CopyEngineLibs(Builder builder, string tempInstallDir, string nativeEngineRoot, bool fileMoving)
        {

            foreach (var arch in BuildEnvironment.TargetArchs)
            {
                if (Directory.Exists(Path.Combine(tempInstallDir, arch.SourceDirName)))   // copy if directory exists.
                {
                    var targetDir = Path.Combine(nativeEngineRoot, "lib", arch.DestDirName);

                    // Engine libs
                    {
                        var srcDir = Path.Combine(tempInstallDir, arch.SourceDirName, "lib");

                        Console.WriteLine($"Copy {srcDir} to {targetDir}");
                        Utils.CopyDirectory(srcDir, targetDir);
                        if (fileMoving)
                            Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策

                        // cmake
                        var cmakeDir = Path.Combine(tempInstallDir, arch.SourceDirName, "cmake");
                        if (Directory.Exists(cmakeDir))
                            Utils.CopyDirectory(cmakeDir, targetDir);
                    }

                    // External libs
                    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, arch.SourceDirName, "ExternalInstall");
                    foreach (var lib in externalLibs)
                    {
                        var srcDir = Path.Combine(externalInstallDir, lib, "lib");
                        if (Directory.Exists(srcDir))   // copy if directory exists. openal-soft etc are optional.
                        {
                            Console.WriteLine($"Copy {srcDir} to {targetDir}");
                            Utils.CopyDirectory(srcDir, targetDir);

                            if (fileMoving)
                                Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策
                        }
                    }

                    // .pdb
                    // CMake では static library の PDB 出力先をコントロールできない。https://cmake.org/cmake/help/v3.1/prop_tgt/PDB_OUTPUT_DIRECTORY.html
                    // そのためビルドスクリプト側でコントロールする。
                    // 以下、パスに "Debug" を含む者のうち、lib と同じ名前の pdb ファイルをコピーする。
                    if (arch.PdbCopy)
                    {
                        var libfiles = Directory.GetFiles(targetDir, "*.lib", SearchOption.TopDirectoryOnly);
                        var libnames = new HashSet<string>(libfiles.Select(x => Path.GetFileNameWithoutExtension(x)));
                        var files1 = Directory.GetFiles(Path.Combine(builder.LuminoBuildDir, arch.SourceDirName), "*.pdb", SearchOption.AllDirectories);
                        foreach (var file in files1)
                        {
                            if (file.Contains("Debug") && libnames.Contains(Path.GetFileNameWithoutExtension(file)))
                            {
                                // FIXME: CI サーバのストレージ不足対策
                                if (fileMoving)
                                    File.Move(file, Path.Combine(targetDir, Path.GetFileName(file)));
                                else
                                    File.Copy(file, Path.Combine(targetDir, Path.GetFileName(file)), true);
                                Console.WriteLine(file);
                            }
                        }
                    }
                }
            }
        }
    }
}