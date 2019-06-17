﻿
#include "External/QtAwesome/QtAwesome.h"
#include "DocumentManager.h"
#include "InspectorPaneContainer.h"
#include "ToolPaneContainer.h"
#include "AudioContentsView.h"
#include "SceneContentsView.h"
#include "ProblemsPane.h"
#include "OutputPane.h"
#include "PropertyPane.h"
#include "MainWindow.h"

static MainWindow* s_mainWindow = nullptr;

MainWindow* MainWindow::instance()
{
    return s_mainWindow;
}

void MainWindow::initializeLumino()
{
    static bool init = false;
    if (!init) {

        ln::GlobalLogger::addStdErrAdapter();

        ln::EngineSettings::setUserMainWindow(instance()->winId());
        ln::EngineSettings::setDefaultObjectsCreation(false);
        ln::EngineSettings::setUseGLFWWindowSystem(false);
        ln::EngineSettings::setGraphicsContextManagement(false);
        ln::EngineSettings::setExternalMainLoop(false);
        ln::EngineSettings::setExternalRenderingManagement(true);
        ln::EngineSettings::setEngineFeatures(ln::EngineFeature::Experimental);
        ln::Engine::initialize();

        init = true;
    }
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
	, m_resourceContext(nullptr)
	, m_offscreenSurface(nullptr)
	, m_contentsViewManager(nullptr)
	, m_documentManager(nullptr)
{
    s_mainWindow = this;

    setGeometry(0, 0, 1200, 800);


    // init OpenGL
    {
        auto* globalContext = QOpenGLContext::globalShareContext();

        m_resourceContext = new QOpenGLContext();
        m_resourceContext->setFormat(globalContext->format());
        m_resourceContext->setShareContext(globalContext);
        if (!m_resourceContext->create()) {
            qCritical() << "m_resourceContext->create()";
            return;
        }

        m_offscreenSurface = new QOffscreenSurface();
        m_offscreenSurface->setFormat(m_resourceContext->format());
        m_offscreenSurface->create();   // context を使用するスレッドで呼び出す必要がある

        m_resourceContext->makeCurrent(m_offscreenSurface);
        MainWindow::initializeLumino();
        m_resourceContext->doneCurrent();
    }

    m_awesome = new QtAwesome(this);
    m_awesome->initFontAwesome();

    m_contentsViewManager = new ContentsViewManager();
    m_documentManager = new DocumentManager();

	auto* rootFrame = new QFrame();
    
	{
        m_contentsViewManager->addContentsViewProvider(ln::makeObject<SceneContentsViewProvider>());
        m_contentsViewManager->addContentsViewProvider(ln::makeObject<AudioContentsViewProvider>());
    }

#if 0

	auto* hbox1 = new QHBoxLayout();
	hbox1->addWidget(m_contentsViewManager->sidebar());
	hbox1->addWidget(m_contentsViewManager->viewContainer());
	hbox1->addWidget(m_documentManager->rootWidget());
	rootFrame->setLayout(hbox1);

#else



    auto* hbox1 = new QHBoxLayout();
    hbox1->setAlignment(Qt::AlignLeft);
    hbox1->setMargin(0);
    hbox1->addWidget(m_contentsViewManager->sidebar());
    rootFrame->setLayout(hbox1);

    QSplitter* split1 = new QSplitter(Qt::Horizontal);
    QSplitter* split2 = new QSplitter(Qt::Vertical);
    //split1->setStyleSheet("background-color: yellow;");
    split1->setContentsMargins(0, 0, 0, 0);
    {
        split1->addWidget(m_contentsViewManager->viewContainer());

        {
            //split2->setStyleSheet("background-color: blue;");
            split2->setContentsMargins(0, 0, 0, 0);
            {
                split2->addWidget(m_documentManager->rootWidget());

				m_toolPaneContainer = new ToolPaneContainer();

				m_outputPane = new OutputPane();
				m_toolPaneContainer->addStandalonePane(m_outputPane);

				m_problemsPane = new ProblemsPane();
				m_toolPaneContainer->addStandalonePane(m_problemsPane);

                split2->addWidget(m_toolPaneContainer);
            }
            split1->addWidget(split2);
        }


		m_inspectorPaneContainer = new InspectorPaneContainer();
        split1->addWidget(m_inspectorPaneContainer);
    }



    hbox1->addWidget(split1);

#endif

    setCentralWidget(rootFrame);

    // Initial layout
    {
        QList<int> sizes1;
        sizes1 << 200 << 600 << 200;
        split1->setSizes(sizes1);

        QList<int> sizes2;
        sizes2 << 600 << 200;
        split2->setSizes(sizes2);
    }
}

MainWindow::~MainWindow()
{
    delete m_documentManager;
    delete m_contentsViewManager;
    delete m_offscreenSurface;
    delete m_resourceContext;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_resourceContext) {
		// MainWindow close 前に終了しないと、OpenGL コンテキストが先に開放されてしまうのでエラーとなる
		m_resourceContext->makeCurrent(m_offscreenSurface);
		ln::Engine::finalize();
		m_resourceContext->doneCurrent();
	}
}
