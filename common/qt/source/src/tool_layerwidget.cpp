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
#include <QHeaderView>
#include <QEvent>
#include <QPixmap>
#include <QImage>
#include <QMimeData>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include "tool_layerwidget.h"

namespace Magus
{
    //****************************************************************************/
    QtLayerWidget::QtLayerWidget(const QString iconDir, QtSceneViewWidget* sceneViewWidget, QWidget* parent) : QWidget(parent)
    {
        setWindowTitle(QString("Layers"));
        QHBoxLayout* mainLayout = new QHBoxLayout;
        QVBoxLayout* tableLayout = new QVBoxLayout;
        mIconDir = iconDir;
        mLayerIdCounter = 1;
        mSceneViewWidget = sceneViewWidget;
        mSceneViewWidgetForDragDrop = 0;
        mSceneIdForDragDrop = 0;

        // Create table
        mTable = new QTableWidget(0, 3, this);
        mTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mTable->setAcceptDrops(true);
        mTable->setShowGrid(false);
        mTable->viewport()->installEventFilter(this);
        mTable->setColumnWidth(TOOL_LAYER_COLUMN_ICON, 2 * TOOL_LAYER_ICON_WIDTH);
        mTable->setColumnWidth(TOOL_LAYER_COLUMN_NAME, TOOL_LAYER_NAME_WIDTH);
        mTable->setColumnWidth(TOOL_LAYER_COLUMN_VISIBILITY, 2 * TOOL_LAYER_ICON_WIDTH);
        mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        setStyleSheet("QLineEdit{padding: 0 0px; margin-left: 0px; margin-right: 0px; max-height: 28px; height: 28px;}");
        connect(mTable, SIGNAL(clicked(QModelIndex)), this, SLOT(tableClicked(QModelIndex)));

        // Set headers
        QStringList headers;
        headers << tr("Layer") << tr("") << tr("Visibility");
        mTable->setHorizontalHeaderLabels(headers);
        QFont font;
        font.setBold(true);
        mTable->horizontalHeader()->setFont(font);

        // Contextmenu
        setContextMenuPolicy(Qt::CustomContextMenu);
        mContextMenu = new QMenu(mTable);
        mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_CREATE_LAYER, mTable));
        mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_DELETE_LAYER, mTable));
        mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_RENAME_LAYER, mTable));
        mContextMenu->addAction(new QAction(TOOL_LAYER_ACTION_ALL_VISIBLE, mTable));
        //mContextMenu->addAction(new QAction(QString("TEST"), mTable));
        connect(mContextMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuItemSelected(QAction*)));

        // Layout
        tableLayout->addWidget(mTable);
        mainLayout->addLayout(tableLayout);
        setLayout(mainLayout);
    }

    //****************************************************************************/
    QtLayerWidget::~QtLayerWidget(void)
    {
        QVector<QtLayer*>::iterator it;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
            delete *it;

        mLayerVec.clear(); // The table is automatically deleted when this widget is deleted
    }

    //****************************************************************************/
    bool QtLayerWidget::eventFilter(QObject* object, QEvent* event)
    {
        QMouseEvent* mouseEvent = (QMouseEvent*) event;
        switch ((int) event->type())
        {
            case QEvent::MouseButtonPress:
                mouseClickHandler(mouseEvent);
            break;

            case QEvent::MouseButtonDblClick:
                mouseDblClickHandler(mouseEvent);
            break;

            case QEvent::Drop:
                dropHandler(object, event);
            break;
        }
        return QObject::eventFilter(object, event);
    }

    //****************************************************************************/
    void QtLayerWidget::tableClicked(QModelIndex index)
    {
        // Toggle visibility
        int row = index.row();
        int col = index.column();
        QtLayer* layer = mLayerVec.at(row);
        if (col == TOOL_LAYER_COLUMN_VISIBILITY)
        {
            if (layer->visible == true)
                updateVisibilityIcon(row, false);
            else
                updateVisibilityIcon(row, true);
        }

        emit layerSelected(layer->layerId, layer->name);

        // Set associated sceneview to visible (if mSceneView available)
        if (mSceneViewWidget)
            mSceneViewWidget->setSceneViewVisible(layer->layerId);
    }

    //****************************************************************************/
    void QtLayerWidget::mouseClickHandler(QMouseEvent* event)
    {
        switch ((int) event->button())
        {
            case Qt::LeftButton:
            {
                // TODO
            }
            break;

            case Qt::RightButton:
            {
                QPoint pos;
                pos.setX(event->screenPos().x());
                pos.setY(event->screenPos().y());
                mContextMenu->popup(pos);
            }
            break;
        }
    }

    //****************************************************************************/
    void QtLayerWidget::mouseDblClickHandler(QMouseEvent* event)
    {
        switch ((int) event->button())
        {
            case Qt::LeftButton:
            {
                if (mTable->currentColumn() == TOOL_LAYER_COLUMN_NAME)
                {
                    QTableWidgetItem* item = mTable->currentItem();
                    if (item->column() == TOOL_LAYER_COLUMN_NAME)
                    {
                        // Its the name; edit it
                        mTable->editItem(item);
                    }
                }
            }
            break;
        }
    }

    //****************************************************************************/
    void QtLayerWidget::dropHandler(QObject* object, QEvent* event)
    {
        event->accept();
        if (!mSceneViewWidgetForDragDrop)
            return;

        // Determine whether the data was from an abstractitem modellist
        QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
        const QMimeData* mimeData = dropEvent->mimeData();
        QString mimeType("application/x-qabstractitemmodeldatalist");
        if (!mimeData->hasFormat(mimeType))
            return;

        // Do not use the mimeData to retrieve the sceneview tree item, because a standard model is used,
        // which does not return the data that is needed.
        // Use the data of the selected item in the sceneview (mSceneIdForDragDrop) as an alternative.
        if (mTable->rowCount() > 0 && mSceneViewWidget)
        {
            // Get the dropped item (this is the currently selected item of mSceneIdForDragDrop in widget mSceneViewWidgetForDragDrop)
            QTreeWidgetItem* item = mSceneViewWidgetForDragDrop->getCurrentItem(mSceneIdForDragDrop);
            if (item)
            {
                // The layerId of the selected layer = the sceneId of the visible sceneview tree in mSceneViewWidget
                // The currently selected group in the mSceneViewWidgetForDragDrop = the destination group of mSceneViewWidget
                int layerId = getCurrentLayerId();
                int groupId = mSceneViewWidgetForDragDrop->getCurrentGroupId(item);

                // 1. Add the group; this is ignored if the group is already available
                QtAssetGroup assetGroupInfo = mSceneViewWidgetForDragDrop->getGroupInfo(groupId);
                mSceneViewWidget->addGroupToSceneView(layerId,
                                                      assetGroupInfo.groupIcon,
                                                      groupId,
                                                      assetGroupInfo.groupName);

                // 2. Determine the item type and add either the asset or all assets in a group
                if (mSceneViewWidgetForDragDrop->itemIsGroup(item))
                {
                    // The dropped item is a GROUP
                    // Add all the items of the source group to the destination group
                    QVector<QTreeWidgetItem*> assetVec = mSceneViewWidgetForDragDrop->getAssetItemsOfGroup(mSceneIdForDragDrop, groupId);
                    foreach (QTreeWidgetItem* assetItem, assetVec)
                    {
                        mSceneViewWidget->addAssetToSceneView(layerId,
                                                              groupId,
                                                              mSceneViewWidget->getAssetIdOfAssetItem(assetItem),
                                                              assetItem->text(0));
                    }

                    //QMessageBox::information(0, "test", item->text(1)); // Test
                }
                else if (mSceneViewWidgetForDragDrop->itemIsAsset(item))
                {
                    // The dropped item is an ASSET
                    // The assetId of the item to be dropped = the assetId of the destination item
                    mSceneViewWidget->addAssetToSceneView(layerId,
                                                          groupId,
                                                          mSceneViewWidget->getAssetIdOfAssetItem(item),
                                                          item->text(0));
                }
            }
        }
    }

    //****************************************************************************/
    void QtLayerWidget::contextMenuItemSelected(QAction* action)
    {
        // ---------------- Create layer action ----------------
        if (action->text() == TOOL_LAYER_ACTION_CREATE_LAYER)
        {
            createLayer(QString(""));
            return;
        }
        else if (action->text() == TOOL_LAYER_ACTION_DELETE_LAYER)
        {
            QList<QTableWidgetItem*> itemList = mTable->selectedItems();
            foreach(QTableWidgetItem* item, itemList)
                deleteLayer((item->data(Qt::UserRole)).toInt());
        }
        else if (action->text() == TOOL_LAYER_ACTION_RENAME_LAYER)
        {
            QTableWidgetItem* item = mTable->currentItem();
            if (item->column() == TOOL_LAYER_COLUMN_NAME)
            {
                // Its the name; edit it
                mTable->editItem(item);
            }
        }
        else if (action->text() == TOOL_LAYER_ACTION_ALL_VISIBLE)
        {
            int rows = mTable->rowCount();
            for(int row = 0; row < rows; ++row)
                if (mLayerVec.at(row)->visible == false)
                    updateVisibilityIcon(row, true);
        }
        else if (action->text() == QString ("TEST"))
        {
            //setLayerName(3, QString("Test"));
            //deleteLayer(QString("New layer 4"));
        }
    }

    //****************************************************************************/
    void QtLayerWidget::updateVisibilityIcon(int row, bool visible)
    {
        QTableWidgetItem* item = mTable->item(row, TOOL_LAYER_COLUMN_VISIBILITY);
        if (!item)
            return;

        QPixmap pixMap;
        QtLayer* layer = mLayerVec.at(row);
        if (visible)
        {
            QImage image(mIconDir + TOOL_ICON_VIEW_VISIBLE);
            pixMap = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
            item->setData(Qt::DecorationRole, QVariant(pixMap));
            layer->visible = true;
        }
        else
        {
            QImage image(mIconDir + TOOL_ICON_VIEW_INVISIBLE);
            pixMap = QPixmap::fromImage(image).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
            item->setData(Qt::DecorationRole, QVariant(pixMap));
            layer->visible = false;
        }

        emit layerVisibiltyChanged(layer->layerId, item->text(), layer->visible);
    }

    //****************************************************************************/
    int QtLayerWidget::newLayerId(void)
    {
        int maxId = 0;
        QtLayer* layer;
        QVector<QtLayer*>::iterator it;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            layer = *it;
            if (layer->layerId > maxId)
                maxId = layer->layerId;
        }

        ++maxId;
        return maxId;
    }

    //****************************************************************************/
    void QtLayerWidget::addLayer(QtLayer* layer)
    {
        // Add the layer to the vector create a new row in the table
        int row = mTable->rowCount();
        mLayerVec.append(layer);
        mTable->insertRow(row);

        // Set the layer icon
        QTableWidgetItem* item = new QTableWidgetItem();
        QImage imageLayer(mIconDir + TOOL_ICON_LAYER);
        QPixmap pixMapLayer = QPixmap::fromImage(imageLayer).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
        item->setData(Qt::DecorationRole, QVariant(pixMapLayer));
        mTable->setItem(row, TOOL_LAYER_COLUMN_ICON, item);

        // Set the name
        item = new QTableWidgetItem();
        QString newName = layer->name;
        if (newName.size() == 0)
            newName = QString("New layer ") + QVariant(layer->layerId).toString();
        layer->name = newName;
        item->setText(newName);
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        mTable->setItem(row, TOOL_LAYER_COLUMN_NAME, item);

        // Set the visibility icon
        item = new QTableWidgetItem();
        QImage imageVis(mIconDir + TOOL_ICON_VIEW_VISIBLE);
        QPixmap pixMapVis = QPixmap::fromImage(imageVis).scaled(TOOL_LAYER_ICON_WIDTH, TOOL_LAYER_ICON_WIDTH);
        item->setData(Qt::DecorationRole, QVariant(pixMapVis));
        mTable->setItem(row, TOOL_LAYER_COLUMN_VISIBILITY, item);

        // Set the layerId
        item->setData(Qt::UserRole, QVariant(layer->layerId));

        //mTable->resizeColumnToContents(0);

        // Set the item to selected
        mTable->selectRow(row);

        // Create a sceneview for this layer
        if (mSceneViewWidget)
        {
            mSceneViewWidget->setSceneViewsInvisible();
            mSceneViewWidget->createSceneView(layer->layerId); // Create a sceneView per layer
        }

        // Emit signal
        emit layerCreatedOrAdded(layer->layerId, layer->name);
    }

    //****************************************************************************/
    const QtLayer* QtLayerWidget::createLayer(const QString& name)
    {
        // Fill the layer object
        QtLayer* layer = new QtLayer();
        layer->layerId = newLayerId();
        layer->name = name;
        layer->visible = true;

        // Add the layer to the layer vector and the table
        addLayer(layer);
        return layer;
    }


    //****************************************************************************/
    const QtLayer* QtLayerWidget::createLayer(int layerId, const QString& name)
    {
        // Fill the layer object
        QtLayer* layer = new QtLayer();
        layer->layerId = layerId;
        layer->name = name;
        layer->visible = true;

        // Add the layer to the layer vector and the table
        addLayer(layer);
        return layer;
    }

    //****************************************************************************/
    int QtLayerWidget::getRow(const QString& name)
    {
        int rows = mTable->rowCount();
        QTableWidgetItem* item;
        for (int row = 0; row < rows; ++row)
        {
            item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
            if (item->text() == name)
                return row;
        }

        return -1; // Not found
    }

    //****************************************************************************/
    int QtLayerWidget::getRow(int layerId)
    {
        int row = 0;
        QtLayer* layer;
        QVector<QtLayer*>::iterator it;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            layer = *it;
            if (layer->layerId == layerId)
                return row;

            ++row;
        }

        return -1; // Not found
    }

    //****************************************************************************/
    void QtLayerWidget::deleteLayer (const QString& name)
    {
        int row = getRow(name);
        if (row != -1)
        {
            // Delete the layer from the table and the vector
            QString name = getName(row);
            mTable->removeRow(row);
            int layerId = removeFromLayerVec(row);

            // If a sceneview is asociated, delete also the sceneview
            if (mSceneViewWidget)
                mSceneViewWidget->deleteSceneView(layerId);

            emit layerDeleted(layerId, name);
        }
    }

    //****************************************************************************/
    void QtLayerWidget::deleteLayer (const QtLayer* layer)
    {
        // Delete the layer
        int row = 0;
        QVector<QtLayer*>::iterator it;
        int layerId;
        QString name;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            if (layer == *it)
            {
                // Delete the layer from the table and the vector
                layerId = layer->layerId;
                mLayerVec.erase(it);
                name = getName(row);
                mTable->removeRow(row);
                delete layer;

                // If a sceneview is asociated, delete also the sceneview
                if (mSceneViewWidget)
                    mSceneViewWidget->deleteSceneView(layerId);

                emit layerDeleted(layerId, name);
            }
            ++row;
        }
    }

    //****************************************************************************/
    void QtLayerWidget::deleteLayer (int layerId)
    {
        int row = getRow(layerId);
        if (row != -1)
        {
            // Delete the layer from the table and the vector
            QString name = getName(row);
            mTable->removeRow(row);
            int layerId = removeFromLayerVec(row);

            // If a sceneview is asociated, delete also the sceneview
            if (mSceneViewWidget)
                mSceneViewWidget->deleteSceneView(layerId);

            emit layerDeleted(layerId, name);
        }
    }

    //****************************************************************************/
    int QtLayerWidget::removeFromLayerVec(int row)
    {
        int layerId;
        QtLayer* layer = mLayerVec.at(row);
        layerId = layer->layerId;
        mLayerVec.removeAt(row);
        delete layer;
        return layerId;
    }

    //****************************************************************************/
    void QtLayerWidget::setLayerName(int layerId, const QString& newName)
    {
        int row = getRow(layerId);
        if (row != -1)
        {
            QTableWidgetItem* item;
            item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
            item->setText(newName);
        }
    }

    //****************************************************************************/
    QtLayer* QtLayerWidget::getLayer(const QString& name)
    {
        int row = 0;
        QVector<QtLayer*>::iterator it;
        QtLayer* layer;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            layer = *it;
            if (name == getName(row))
            {
                layer->name = name; // Explicitly set the name, because it is not automatically in sync with the table
                return layer;
            }

            ++row;
        }

        return 0;
    }

    //****************************************************************************/
    QtLayer* QtLayerWidget::getLayer(int layerId)
    {
        QVector<QtLayer*>::iterator it;
        QtLayer* layer;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            layer = *it;
            if (layer->layerId = layerId)
                return layer;
        }

        return 0;
    }

    //****************************************************************************/
    QVector<QtLayer*> QtLayerWidget::getLayers(void)
    {
        // Sync the name in the table with the vector
        int row = 0;
        QVector<QtLayer*>::iterator it;
        QtLayer* layer;
        for (it = mLayerVec.begin(); it != mLayerVec.end(); ++it)
        {
            layer = *it;
            layer->name = getName(row); // Explicitly set the name, because it is not automatically in sync with the table
            ++row;
        }

        return mLayerVec;
    }

    //****************************************************************************/
    const QString& QtLayerWidget::getName(int row)
    {
        QTableWidgetItem* item = mTable->item(row, TOOL_LAYER_COLUMN_NAME);
        if (item)
            mTempName = item->text();

        return mTempName;
    }

    //****************************************************************************/
    void QtLayerWidget::setSceneViewWidgetForDragDrop(QtSceneViewWidget* sceneViewWidget, int sceneId)
    {
        mSceneViewWidgetForDragDrop = sceneViewWidget;
        mSceneIdForDragDrop = sceneId;
    }

    //****************************************************************************/
    int QtLayerWidget::getCurrentLayerId(void)
    {
        QtLayer* layer = mLayerVec[mTable->currentRow()];
        if (layer)
            return layer->layerId;

        if (mLayerVec.count() > 0)
            return 0;

        return -1;
    }

}
