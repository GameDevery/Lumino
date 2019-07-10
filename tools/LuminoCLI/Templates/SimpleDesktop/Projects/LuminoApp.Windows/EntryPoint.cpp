
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

void Main();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#ifdef LN_DEBUG
    auto projectRoot = ln::Path(ln::Environment::executablePath()).parent().parent().parent().parent().parent();
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u"Assets"));
	ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u".ln", u"Assets"));
#endif

    auto archive = u"Assets.lca";
    if (ln::FileSystem::existsFile(archive)) {
        ln::EngineSettings::addAssetArchive(archive);
    }

    ln::EngineSettings::setEngineFeatures(ln::EngineFeature::Experimental);
    ln::EngineSettings::setStandaloneFpsControl(true);
    ln::Engine::initialize();

    Main();

    ln::Engine::finalize();
}

int main(int argc, char** argv)
{
    return wWinMain(0, 0, 0, 0);
}