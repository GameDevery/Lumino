﻿#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "LanguageContext.hpp"

//==============================================================================
// LanguageContext

LanguageContext::LanguageContext(Project* project)
	: m_project(project)
{
}

LanguageContext::~LanguageContext()
{
}

//==============================================================================
// CppLanguageContext

CppLanguageContext::CppLanguageContext(Project* project)
	: LanguageContext(project)
{
}

CppLanguageContext::~CppLanguageContext()
{
}

Result CppLanguageContext::applyTemplates()
{
	CLI::info("Copying NativeProject template...");

	auto projectTemplatesDir = project()->workspace()->buildEnvironment()->projectTemplatesDirPath();
	auto srcRoot = ln::Path(projectTemplatesDir, u"NativeProject");
	auto dstRoot = project()->rootDirPath();

	// Common
	{
		ln::String files[] = {
			u".gitignore",
			u"Sources/Application.cpp",
			u"Sources/Application.h",
		};

		for (auto& file : files)
		{
			auto filePath = ln::Path(dstRoot, file);
			auto dirPath = filePath.parent();
			if (!ln::FileSystem::existsDirectory(dirPath)) {
				ln::FileSystem::createDirectory(dirPath);
			}

			ln::FileSystem::copyFile(ln::Path(srcRoot, file), filePath, ln::FileCopyOption::Overwrite);
		}

		// Assets
		ln::FileSystem::copyDirectory(ln::Path(srcRoot, u"Assets"), ln::Path(dstRoot, u"Assets"), true, true);
	}

	// Windows
	{
		ln::FileSystem::copyFile(
			ln::Path(srcRoot, u"LuminoApp.sln"),
			ln::Path(dstRoot, u"LuminoApp.sln"), ln::FileCopyOption::Overwrite);

		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.Windows"),
			ln::Path(dstRoot, u"Projects/LuminoApp.Windows"),
			true, true);
	}

	// macOS
	{
		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.macOS"),
			ln::Path(dstRoot, u"Projects/LuminoApp.macOS"),
			true, true);
	}

	// Web
	{
		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.Web"),
			ln::Path(dstRoot, u"Projects/LuminoApp.Web"),
			true, true);
	}

	// iOS
	{
		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.iOS"),
			ln::Path(dstRoot, u"Projects/LuminoApp.iOS"),
			true, true);
	}

	// Android
	{
		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.Android"),
			ln::Path(dstRoot, u"Projects/LuminoApp.Android"),
			true, true);
	}

	CLI::info("Copied NativeProject template.");
	return Result::Success;
}

Result CppLanguageContext::applyEngine()
{
    if (project()->properties()->engine.indexOf(u"repo:") == 0)
    {
        auto engineRoot = ln::Path::combine(project()->engineDirPath(), u"Native");
        auto branch = project()->properties()->engine.substr(5);
        int result = 0;

        result = ln::Process::execute(
            u"git",
            {
                u"clone",
                u"--progress",
                u"-b", branch,
                u"https://github.com/lriki/Lumino.git",
                engineRoot,
            });
        if (result != 0) {
            return Result::Fail;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"BuildForCI_1" });
        if (result != 0) {
            return Result::Fail;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"BuildForCI_2" });
        if (result != 0) {
            return Result::Fail;
        }

        result = ln::Process::execute(u"dotnet", { u"run", u"--project", ln::Path(engineRoot, u"build.csproj"), u"--", u"CopyEngineLibsToRepoRoot" });
        if (result != 0) {
            return Result::Fail;
        }
    }
    else
    {
        if (!ln::FileSystem::getFile(project()->engineDirPath()).isEmpty()) {
            CLI::warning(u"File exists in the engine folder.");
        }
        else {
            CLI::info("Copying Engine...");

            ln::FileSystem::copyDirectory(
                ln::Path::combine(project()->workspace()->buildEnvironment()->luminoPackageRootDir(), u"Engine", u"Native"),
                ln::Path::combine(project()->engineDirPath(), u"Native"),
                true, true);

            CLI::info("Copied Engine.");
        }
    }
    return Result::Success;
}

void CppLanguageContext::restore()
{
    applyEngine();
}
