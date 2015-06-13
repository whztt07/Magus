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

namespace Magus
{
    //****************************************************************************/
    QtTextureWidget::QtTextureWidget(QWidget* parent) : QWidget(parent)
    {
        setWindowTitle(QString("Texture selection"));
        mNameTexture = QString("");
        mBaseNameTexture = QString("");
        mTextureSize = QSize(128, 128);
        mOriginIsFile = true;
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
    void QtTextureWidget::addTexture(const QPixmap& pixmap, const QString name)
    {
        mSelectionModel->addTexture(pixmap, name, mTextureSize);
    }

    //****************************************************************************/
    void QtTextureWidget::setOriginIsFile(bool originIsFile)
    {
        mOriginIsFile = originIsFile;
    }

    //****************************************************************************/
    void QtTextureWidget::clearContent(void)
    {
        mSelectionModel->clear();
    }

    //****************************************************************************/
    const QString& QtTextureWidget::getNameTexture(void)
    {
        return mNameTexture;
    }

    //****************************************************************************/
    const QString& QtTextureWidget::getBaseNameTexture(void)
    {
        return mBaseNameTexture;
    }

    //****************************************************************************/
    void QtTextureWidget::mouseClicked(void)
    {
        QModelIndex selectedTexture = mSelectionList->currentIndex();
        QString name = mSelectionModel->getName(selectedTexture);
        if (!name.isEmpty())
        {
            mNameTexture = name;

            if (mOriginIsFile)
            {
                QFileInfo fileInfo(name);
                name = fileInfo.fileName();
            }

            mBaseNameTexture = name;
            emit textureSelected(mNameTexture);
        }
    }

    //****************************************************************************/
    void QtTextureWidget::mouseOver(QModelIndex index)
    {
        QString name = index.data(Qt::UserRole + 1).toString();
        if (mOriginIsFile)
        {
            QFileInfo fileInfo(name);
            name = fileInfo.fileName();
        }
        mSelectionList->setToolTip(name);
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
