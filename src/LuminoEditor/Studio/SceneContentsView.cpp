﻿
#include "MainWindow.h"
#include "DocumentManager.h"
#include "SceneEditorDocumentView.h"
#include "SceneContentsView.h"

//==============================================================================
// SceneAssetTreeView

SceneAssetTreeView::SceneAssetTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_model = new SceneAssetTreeModel(this);
    QModelIndex rootModelIndex = m_model->setRootPath("D:/Proj/LN/PrivateProjects/HC0/Assets");

    //setHeaderHidden(true);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextElideMode(Qt::ElideNone);
    setModel(m_model);
    setRootIndex(rootModelIndex);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //setFixedHeight(500);
    //setColumnWidth(0, INT_MAX);

    // ダブルクリックで編集を開始しないようにする
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

    //connect(this, SIGNAL(doubleclicked(QModelIndex)), this, SLOT(OnDoubleclicked(QModelIndex)));
}

void SceneAssetTreeView::onDoubleClicked(const QModelIndex &index)
{
    QString filePath = m_model->fileInfo(index).absoluteFilePath();

    // TODO: ここで addDocument ではなく、MainWindow に openFile を設けて、フォーマットもそっちで判断して Dcument 作ってもらう。Tiledもそんな感じ。
    MainWindow::instance()->documentManager()->addDocument(new SceneEditorDocument());
    
}

//==============================================================================
// SceneContentsViewProvider

SceneContentsViewProvider::SceneContentsViewProvider()
    : m_rootWidget(nullptr)
{

}

QWidget* SceneContentsViewProvider::createView()
{
    LN_CHECK(!m_rootWidget);

    auto* vbox1 = new QVBoxLayout();
    {
        auto* hbox1 = new QHBoxLayout();
        auto* addButton = new QPushButton("Add");
        hbox1->addWidget(addButton);
        auto* removeButton = new QPushButton("Remove");
        hbox1->addWidget(removeButton);
        vbox1->addLayout(hbox1);
    }
    {
        auto* treeView = new SceneAssetTreeView();
        vbox1->addWidget(treeView);
    }

    m_rootWidget = new QFrame();
    m_rootWidget->setLayout(vbox1);

    return m_rootWidget;
}
