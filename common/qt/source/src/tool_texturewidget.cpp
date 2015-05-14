/****************************************************************************
**
** Copyright (C) 2014
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

// Include
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QImageReader>
#include "tool_texturewidget.h"
#include "generic_funtions.h"

namespace Magus
{
    //****************************************************************************/
    QtTextureWidget::QtTextureWidget(QWidget* parent) : QWidget(parent)
    {
        setWindowTitle(QString("Texture selection"));
        mFileNameTexture = QString("");
        mBaseFileNameTexture = QString("");
        mTextureSize = QSize(128, 128);
        QHBoxLayout* mainLayout = new QHBoxLayout;
        QVBoxLayout* textureSelectionLayout = new QVBoxLayout;

        // Define selection widget (QListView)
        mSelectionList = new QListView(this);
        mSelectionList->setViewMode(QListView::IconMode);
        mSelectionList->setIconSize(mTextureSize);
        mSelectionList->setSpacing(2);
        mSelectionList->setMovement(QListView::Snap);
        mSelectionList->setFlow(QListView::LeftToRight);
        mSelectionList->setWrapping(true);
        mSelectionList->setDragEnabled(false);
        mSelectionList->setAcceptDrops(false);
        mSelectionList->setDropIndicatorShown(false);
        mSelectionList->setMouseTracking(true);
        mSelectionModel = new QtTextureModel();
        mSelectionList->setModel(mSelectionModel);
        connect(mSelectionList, SIGNAL(clicked(QModelIndex)), this, SLOT(mouseClicked(void)));
        connect(mSelectionList, SIGNAL(entered(QModelIndex)), this, SLOT(mouseOver(QModelIndex)));

        // Layout
        textureSelectionLayout->addWidget(mSelectionList);
        mainLayout->addLayout(textureSelectionLayout);
        setLayout(mainLayout);
    }

    //****************************************************************************/
    QtTextureWidget::~QtTextureWidget(void)
    {
    }

    //****************************************************************************/
    void QtTextureWidget::fillTextures(const QString& searchPath)
    {
        // Get all texture files from all dirs/subdirs
        QDir dir(searchPath);
        dir.makeAbsolute();

        if (dir.exists())
        {
            mSelectionModel->clear();
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
            {
                if (info.isDir())
                {
                    fillTextures(info.absoluteFilePath());
                }
                else
                {
                    QString fileName = info.absoluteFilePath();
                    if (isImageBasedOnExtension(fileName))
                    {
                        // It is an image
                        mSelectionModel->addTexture(info.absoluteFilePath(), mTextureSize);
                    }
                }
            }
        }
    }

    //****************************************************************************/
    const QString& QtTextureWidget::getTextureFileName(void)
    {
        return mFileNameTexture;
    }

    //****************************************************************************/
    const QString& QtTextureWidget::getTextureBaseFileName(void)
    {
        return mBaseFileNameTexture;
    }

    //****************************************************************************/
    void QtTextureWidget::mouseClicked(void)
    {
        QModelIndex selectedTexture = mSelectionList->currentIndex();
        QString fileNameTexture = mSelectionModel->getName(selectedTexture);
        if (!fileNameTexture.isEmpty())
        {
            mFileNameTexture = fileNameTexture;
            emit textureSelected(mFileNameTexture);
        }
    }

    //****************************************************************************/
    void QtTextureWidget::mouseOver(QModelIndex index)
    {
        // TODO: Get base filename instead of full qualified name
        mSelectionList->setToolTip(index.data(Qt::UserRole + 1).toString());
    }

    //****************************************************************************/
    void QtTextureWidget::setTextureSize (QSize size)
    {
        mTextureSize = size;
        mSelectionList->setIconSize(mTextureSize);
    }

    //****************************************************************************/
    void QtTextureWidget::setDragEnabled (bool enabled)
    {
        mSelectionList->setDragEnabled(enabled);
    }
}