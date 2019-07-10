﻿
#include "../../src/LuminoEngine/src/Asset/AssetArchive.hpp"
#include "../../src/LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "BuildCommand.hpp"
#include "FxcCommand.hpp"

int BuildCommand::execute(lna::Workspace* workspace, lna::Project* project)
{
	m_project = project;

	if (ln::String::compare(target, u"Windows", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
		if (!package) {
			if (!buildWindowsTarget(workspace, true)) {
				return 1;
			}
		}
		else {
			if (!buildWindowsTarget(workspace, false)) {
				return 1;
			}
			if (!buildWindowsPackage(project)) {
				return 1;
			}
		}
	}
	else if (ln::String::compare(target, u"Web", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
		if (!buildWebTarget(workspace)) {
			return 1;
		}
	}
	else if (ln::String::compare(target, u"Assets", ln::CaseSensitivity::CaseInsensitive) == 0) {
		if (!buildAssets()) {
			return 1;
		}
	}
	else {
		CLI::error(ln::String::format(u"{0} is invalid target.", target));
		return 1;
	}

    return 0;
}

ln::Result BuildCommand::buildWindowsTarget(lna::Workspace* workspace, bool debug)
{
	auto file = ln::FileSystem::getFile(m_project->rootDirPath(), u"*.sln");
	if (file.isEmpty()) {
		CLI::error(".sln file not found.");
		return false;
	}

	if (debug) {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), u"/t:build", u"/p:Configuration=Debug;Platform=\"x86\"" }) != 0) {
			CLI::error("Failed MSBuild.");
			return false;
		}
	}
	else {
		if (ln::Process::execute(workspace->buildEnvironment()->msbuild(), { file.str(), u"/t:build", u"/p:Configuration=Release;Platform=\"x86\"" }) != 0) {
			CLI::error("Failed MSBuild.");
			return false;
		}
	}

	return true;
}

ln::Result BuildCommand::buildWindowsPackage(lna::Project* project)
{
	auto dstDir = ln::Path::combine(project->releaseDir(), u"Windows");
	ln::FileSystem::createDirectory(dstDir);

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), u"bin", u"Release", u"LuminoApp.exe"),
		ln::Path::combine(dstDir, u"LuminoApp.exe"));

	ln::FileSystem::copyFile(
		ln::Path::combine(project->windowsProjectDir(), u"Assets.lca"),
		ln::Path::combine(dstDir, u"Assets.lca"));

	return true;
}

ln::Result BuildCommand::buildWebTarget(lna::Workspace* workspace)
{
	// emsdk がなければインストールする
	workspace->buildEnvironment()->prepareEmscriptenSdk();

	auto buildDir = ln::Path::combine(m_project->buildDir(), u"Web").canonicalize();
	auto installDir = ln::Path::combine(buildDir, u"Release");
	auto cmakeSourceDir = m_project->emscriptenProjectDir();
	auto script = ln::Path::combine(buildDir, u"build.bat");

	ln::FileSystem::createDirectory(buildDir);

	{
		ln::List<ln::String> emcmakeArgs = {
			u"-DCMAKE_BUILD_TYPE=Release",
			u"-DCMAKE_INSTALL_PREFIX=" + installDir,
			u"-DLUMINO_ENGINE_ROOT=\"" + ln::Path(m_project->engineDirPath(), u"Native").str().replace("\\", "/") + u"\"",
			u"-DLN_TARGET_ARCH=Emscripten",
			u"-G \"MinGW Makefiles\"",
			cmakeSourceDir,
		};

		ln::StreamWriter sw(script);
		sw.writeLineFormat(u"cd /d \"{0}\"", workspace->buildEnvironment()->emsdkDirPath());
		sw.writeLineFormat(u"call emsdk activate " + workspace->buildEnvironment()->emsdkName());
		sw.writeLineFormat(u"call emsdk_env.bat");
		sw.writeLineFormat(u"cd /d \"{0}\"", buildDir);
		sw.writeLineFormat(u"call emcmake cmake " + ln::String::join(emcmakeArgs, u" "));
		sw.writeLineFormat(u"call cmake --build .");
	}

	ln::Process::execute(script);
	return true;
}

ln::Result BuildCommand::buildAndroidTarget()
{
#if 0
	// Android
	else if (ln::String::compare(target, u"Android", ln::CaseSensitivity::CaseInsensitive) == 0)
	{
	putenv("JAVA_HOME=\"D:\\Program Files\\Android\\Android Studio\\jre\"");

	ln::Process::execute(u"gradlew.bat", { u"assemble" });	// Debug, Release 両方ビルド

	// https://qiita.com/tkc_tsuchiya/items/6485714615ace9e19918

#if 0
	ln::String abi = u"x86_64";
	ln::String platform = "android-26";
	ln::String buildType = "Release";
	ln::String targetName = u"Android-" + abi;
	ln::Path outputDir = ln::Path(m_project->androidProjectDir(), u"app/build/intermediates/cmake/release/obj/" + abi);
	ln::Path luminoPackageDir = ln::Path(m_devTools->luminoPackageRootDir(), u"Engine/Cpp/Android-" + abi);
	ln::Path cmakeHomeDir = ln::Path(m_project->androidProjectDir(), u"app");
	ln::Path buildDir = ln::Path::combine(m_project->buildDir(), targetName);

	ln::List<ln::String> args = {
		u"-H" + cmakeHomeDir,
		u"-B" + buildDir,
		u"-DLN_TARGET_ARCH_NAME=" + targetName,
		u"-DANDROID_ABI=" + abi,
		u"-DANDROID_PLATFORM=" + platform,
		u"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + outputDir.str().replace("\\", "/"),
		u"-DCMAKE_BUILD_TYPE=" + buildType,
		u"-DANDROID_NDK=" + m_environmentSettings->androidNdkRootDir(),
		u"-DCMAKE_CXX_FLAGS=-std=c++14",
		u"-DCMAKE_TOOLCHAIN_FILE=" + m_environmentSettings->androidCMakeToolchain(),
		u"-DCMAKE_MAKE_PROGRAM=" + m_environmentSettings->androidSdkNinja(),

		u"-DANDROID_STL=c++_shared",
		u"-DLumino_DIR=" + luminoPackageDir.str().replace("\\", "/"),

		u"-G\"Android Gradle - Ninja\"",
	};

	ln::Process::execute(m_environmentSettings->androidSdkCMake(), args);

	ln::Process::execute(m_environmentSettings->androidSdkCMake(), { u"--build", buildDir });
#endif
	}
#endif
	return true;
}

ln::Result BuildCommand::buildAssets()
{
	ln::detail::CryptedAssetArchiveWriter writer;
	auto outputFilePath = ln::Path(m_project->buildDir(), u"Assets.lca");
	writer.open(outputFilePath, ln::detail::CryptedArchiveHelper::DefaultPassword);

	for (auto& file : ln::FileSystem::getFiles(m_project->assetsDir(), ln::StringRef(), ln::SearchOption::Recursive)) {
		if (file.hasExtension(".fx")) {

			ln::Path outputFile;
			if (!buildAsset_Shader(file, &outputFile)) {
				return false;
			}

			writer.addFile(outputFile, m_project->assetsDir().makeRelative(file).replaceExtension(ln::detail::UnifiedShader::FileExt));
			CLI::info(file);
		}
		else {
			writer.addFile(file, m_project->assetsDir().makeRelative(file));
			CLI::info(file);
		}
	}

	writer.close();

	// Android
	{
        if (ln::FileSystem::existsDirectory(m_project->androidProjectDir())) {
            auto dst = ln::Path::combine(m_project->androidProjectDir(), u"app", u"src", u"main", u"assets", u"Assets.lca");
            ln::FileSystem::createDirectory(dst.parent());
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(u"Copy to " + dst);
        }
	}

	// macOS
	{
        if (ln::FileSystem::existsDirectory(m_project->macOSProjectDir())) {
            auto dst = ln::Path::combine(m_project->macOSProjectDir(), u"LuminoApp.macOS", u"Assets.lca");
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(u"Copy to " + dst);
        }
	}

	// iOS
	{
        if (ln::FileSystem::existsDirectory(m_project->iOSProjectDir())) {
            auto dst = ln::Path::combine(m_project->iOSProjectDir(), u"LuminoApp.iOS", u"Assets.lca");
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(u"Copy to " + dst);
        }
	}

	// Windows
	{
        if (ln::FileSystem::existsDirectory(m_project->windowsProjectDir())) {
            auto dst = ln::Path::combine(m_project->windowsProjectDir(), u"Assets.lca");
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(u"Copy to " + dst);
        }
	}

	// Web
	{
        if (ln::FileSystem::existsDirectory(m_project->buildDir())) {
            auto dstDir = ln::Path::combine(m_project->buildDir(), u"Web");
            ln::FileSystem::createDirectory(dstDir);

            auto dst = ln::Path::combine(dstDir, u"Assets.lca");
            ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
            CLI::info(u"Copy to " + dst);
        }
	}

	CLI::info(u"Compilation succeeded.");

	return true;
}

ln::Result BuildCommand::buildAsset_Shader(const ln::Path& inputFile, ln::Path* outputFile)
{
	auto rel = m_project->assetsDir().makeRelative(inputFile);
	auto workFile = ln::Path::combine(m_project->intermediateAssetsDir(), rel.parent(), inputFile.fileName().replaceExtension(ln::detail::UnifiedShader::FileExt));
	ln::FileSystem::createDirectory(workFile.parent());

	FxcCommand cmd;
	cmd.outputFile = workFile;
	if (cmd.execute(inputFile) != 0) {
		return false;
	}

	*outputFile = workFile;
	return true;
}