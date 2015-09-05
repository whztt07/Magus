/****************************************************************************
**
** Copyright (C) 2015
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

#ifndef MAGUS_TOOL_GENERIC_ASSET_WIDGET_H
#define MAGUS_TOOL_GENERIC_ASSET_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPixmap>
#include <QPlainTextEdit>
#include "magus_core.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace Magus
{
    /****************************************************************************
    Helper class to combine the generic asset name and the image
    ***************************************************************************/
    class QtGenericAssetAndText : public QWidget
    {
        Q_OBJECT

        public:
            QString mBaseName;
            QString mName;
            Source mSource;
            QtGenericAssetAndText(const QPixmap& assetPixmap,
                                  const QString& name,
                                  const QString& baseName,
                                  const QSize& size,
                                  QWidget* parent = 0);

            virtual ~QtGenericAssetAndText(void);

        private:
            QLabel* mTextureLabel;
            QLineEdit* mBaseNameEdit;
            QPixmap mPixmapGenericAsset;
    };

    //****************************************************************************/
    //****************************************************************************/
    /****************************************************************************
    /****************************************************************************
    The QtGenericAssetListWidget is a QListWidget with convenience functions regarding
    drag and drop.
    ***************************************************************************/
    class QtGenericAssetListWidget : public QListWidget
    {
        Q_OBJECT

        public:
            QtGenericAssetListWidget(QWidget* parent = 0);
            virtual ~QtGenericAssetListWidget(void);

            // Determine which file types are allowed, based on their extensions
            void setAllowedExtensions(const QString ext[], int arrayLength);

        signals:
            // Emitted when a file is dropped
            void fileDropped(const QString& name, const QString& baseName);

        protected:
            QString mAllowedExtensions[200];
            int mAllowedExtensionsLength;
            virtual void dropEvent(QDropEvent* event);
            virtual void dragEnterEvent(QDragEnterEvent *event);
            virtual void dragMoveEvent(QDragMoveEvent *event);
    };

    //****************************************************************************/
    //****************************************************************************/
    /****************************************************************************
    Main class for generic asset selection widget
    ***************************************************************************/
    class QtGenericAssetWidget : public QWidget
    {
        Q_OBJECT

        public:
            QtGenericAssetWidget(QPixmap defaultPixmap, bool viewEnabled = false, QWidget* parent = 0);
            virtual ~QtGenericAssetWidget(void);

            // Add an 'asset' to this widget. The name is a (fully qualified) filename for example.
            // E.g. name = "c:/temp/assets/meshes/character.mesh"
            // E.g. baseName = "character.mesh"
            // assetPixmap defines the pixmap of the icon with which the asset is presented
            void addAsset(const QPixmap& assetPixmap, const QString& name, const QString& baseName);

            // Delete an item from the QtGenericAssetWidget. Use the full qualified name as search criterium if nameIsFullName = true;
            // else use the baseName as search criterium.
            void deleteAsset(const QString& name, bool nameIsFullName = true);

            // Delete an item from the QtGenericAssetWidget. Both name (full qualified name) and baseName must match
            void deleteAsset(const QString& name, const QString& baseName);

            // If a n asset is originated from a file, setOriginIsFile must be set to 'true'
            void setOriginIsFile(bool originIsFile);

            // Clear the content of the widget
            void clearContent(void);

            // Return the name of the selected texture
            // E.g. "c:/temp/assets/meshes/character.mesh"
            const QString& getNameAsset(void);

            // Return the base name of the selected asset
            // E.g. "character.mesh" in case the name of the asset is a full qualified filename.
            const QString& getBaseNameAsset(void);

            // Define the width and height of a texture in the selection box
            void setTextureSize (QSize size);

            // Apply filtering; only the items that meet the pattern are displayed
            void filter(const QString& pattern);

            // Reset the filtering
            void resetFilter(void);

            // Determine whether dropping files from the file explorer is allowed
            void setDropFilesAllowed(bool allowed);

            // Determine which file types are allowed, based on their extensions
            void setAllowedExtensions(const QString ext[], int arrayLength);

        signals:
            // Emitted when an asset is selected (via the mouse)
            void selected(const QString& name, const QString& baseName);

            // Emitted when an asset is doubleclicked (via the mouse)
            void doubleClicked(const QString& name, const QString& baseName);

            // Emitted when a file is dropped
            void fileDropped(const QString& name, const QString& baseName);

        protected slots:
            void handleSelected(QListWidgetItem* item);
            void handleDoubleClicked(QListWidgetItem* item);
            void handleFileDropped (const QString& name, const QString& baseName);
            void handleMouseOver(QListWidgetItem* item);

        protected:
            void loadFileInViewer(const QString& fileName, const QString& baseFileName);

        private:
            QPixmap mDefaultPixmapAsset;
            QtGenericAssetListWidget* mSelectionList;
            QPlainTextEdit* mTextViewer;
            QSize mTextureSize;
            QString mNameAsset; // In case of a filename, this is the fully qualified filename (path + filename)
            QString mBaseNameAsset; // If mNameAsset is a filename, this is the basename.
            bool mOriginIsFile;
            bool mViewEnabled;
    };
}

#endif
