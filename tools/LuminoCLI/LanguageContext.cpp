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

		ln::FileSystem::copyDirectory(
			ln::Path::combine(project()->workspace()->buildEnvironment()->luminoPackageRootDir(), u"Engine", u"Native"),
			project()->engineDirPath(), true, true);
	}

	// Win32
	{
		ln::FileSystem::copyFile(
			ln::Path(srcRoot, u"LuminoApp.sln"),
			ln::Path(dstRoot, u"LuminoApp.sln"), ln::FileCopyOption::Overwrite);

		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.Win32"),
			ln::Path(dstRoot, u"Projects/LuminoApp.Win32"),
			true, true);
	}

	// macOS
	{
		ln::FileSystem::copyDirectory(
			ln::Path(srcRoot, u"Projects/LuminoApp.macOS"),
			ln::Path(dstRoot, u"Projects/LuminoApp.macOS"),
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
	return Result::OK;
}
