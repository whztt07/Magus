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
#include "tool_gradientwidget.h"
#include "tool_gradientmarker.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <QRegExpValidator>
#include <QMessageBox>

namespace Magus
{
    //****************************************************************************/
    QtGradientWidget::QtGradientWidget(QWidget* parent) : QWidget(parent)
    {
        setContentsMargins(-4, -4, -4, -4);
        QGroupBox* alphaGroup = new QGroupBox(QString("Alpha"));
        QGroupBox* colorGroup = new QGroupBox(QString("Color"));
        QGroupBox* generalGroup = new QGroupBox();
        QVBoxLayout* alphaLayout = new QVBoxLayout;
        QVBoxLayout* colorLayout = new QVBoxLayout;
        QHBoxLayout* generalLayout = new QHBoxLayout;

        idCounter = 0;
        mCurrentMarker = 0;
        qreal mGradientHeightOffsetFraction = 0.13f;
        qreal mGradientWidth = size().width();
        mGradient = new QtGradient();
        setGradientDimension(mGradientWidth, mGradientHeightOffsetFraction);
        QVBoxLayout* mainLayout = new QVBoxLayout;
        mView = new QGraphicsView(this);
        mScene = new QGraphicsScene();
        mScene->installEventFilter(this);
        mView->setRenderHint(QPainter::Antialiasing, true);
        mView->setInteractive(true);
        mView->setMouseTracking(true);
        mView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mView->setScene(mScene);
        mScene->addItem(mGradient);
        QRegExp regularExpression1("^([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$"); // unsigned integer [0..255]
        QRegExp regularExpression2("^[0-9-a-fA-F]{0,6}$"); // max 6 characters
        QRegExpValidator* validator1 = new QRegExpValidator(regularExpression1);
        QRegExpValidator* validator2 = new QRegExpValidator(regularExpression2);

        // Alpha widgets
        QHBoxLayout* alphaEditAndDeleteButtonLayout = new QHBoxLayout;
        mAlphaSlider = new QSlider();
        mAlphaEdit = new QLineEdit();
        mAlphaEdit->setValidator(validator1);
        mDeleteAlphaMarkerButton = new QPushButton(QString("Delete alpha marker"));
        mAlphaSlider->setOrientation(Qt::Horizontal);
        mAlphaSlider->setMinimum(0);
        mAlphaSlider->setMaximum(255);
        mAlphaSlider->setTickInterval(1);
        connect(mAlphaSlider, SIGNAL(valueChanged(int)), this, SLOT(alphaSliderValueChanged(int)));
        connect(mAlphaEdit, SIGNAL(textChanged(QString)), this, SLOT(alphaEditValueChanged(QString)));
        connect(mDeleteAlphaMarkerButton, SIGNAL(clicked()), this, SLOT(deleteAlphaMarker()));
        alphaLayout->addWidget(mAlphaSlider);
        alphaEditAndDeleteButtonLayout->addWidget(mAlphaEdit);
        alphaEditAndDeleteButtonLayout->addStretch(2);
        alphaEditAndDeleteButtonLayout->addWidget(mDeleteAlphaMarkerButton);
        alphaLayout->addLayout(alphaEditAndDeleteButtonLayout);
        enableAlphaWidgets(false);
        alphaGroup->setLayout(alphaLayout);

        // Color widgets
        QHBoxLayout* horizontalLayoutRGB = new QHBoxLayout;
        QHBoxLayout* horizontalLayoutHex = new QHBoxLayout;
        QLabel* labelR = new QLabel(QString("Red"));
        QLabel* labelG = new QLabel(QString("Green"));
        QLabel* labelB = new QLabel(QString("Blue"));
        QLabel* labelHex = new QLabel(QString("Hex"));
        mR = new QLineEdit;
        mG = new QLineEdit;
        mB = new QLineEdit;
        mHex = new QLineEdit;
        mColorFill = new QLabel;
        mColorFill->setAutoFillBackground(true);
        mDeleteColorMarkerButton = new QPushButton(QString("Delete color marker"));
        updateColorFill();
        mR->setValidator(validator1);
        mG->setValidator(validator1);
        mB->setValidator(validator1);
        mHex->setValidator(validator2);
        mDialogButton = new QPushButton(QString(" .. "));
        connect(mR, SIGNAL(textEdited(QString)), this, SLOT(valueChangedRGB(void)));
        connect(mG, SIGNAL(textEdited(QString)), this, SLOT(valueChangedRGB(void)));
        connect(mB, SIGNAL(textEdited(QString)), this, SLOT(valueChangedRGB(void)));
        connect(mHex, SIGNAL(textEdited(QString)), this, SLOT(valueChangedHex(void)));
        connect(mDialogButton, SIGNAL(clicked(bool)), this, SLOT(dialogButtonClicked(void)));
        connect(mDeleteColorMarkerButton, SIGNAL(clicked()), this, SLOT(deleteColorMarker()));
        horizontalLayoutRGB->addWidget(labelR);
        horizontalLayoutRGB->addWidget(mR, 2);
        horizontalLayoutRGB->addWidget(labelG);
        horizontalLayoutRGB->addWidget(mG, 2);
        horizontalLayoutRGB->addWidget(labelB);
        horizontalLayoutRGB->addWidget(mB, 2);
        horizontalLayoutHex->addWidget(labelHex);
        horizontalLayoutHex->addWidget(mHex, 2);
        horizontalLayoutHex->addWidget(mColorFill, 2);
        horizontalLayoutHex->addWidget(mDialogButton);
        horizontalLayoutHex->addStretch(1);
        horizontalLayoutHex->addWidget(mDeleteColorMarkerButton);
        colorLayout->addLayout(horizontalLayoutRGB);
        colorLayout->addLayout(horizontalLayoutHex);
        enableColorWidgets(false);
        colorGroup->setLayout(colorLayout);

        // General
        mClearButton = new QPushButton(QString("Clear gradient"));
        connect(mClearButton, SIGNAL(clicked()), this, SLOT(clearGradient()));
        generalLayout->addStretch(3);
        generalLayout->addWidget(mClearButton, 1);
        generalGroup->setLayout(generalLayout);

        // Set the main layout
        mainLayout->addWidget(alphaGroup, 1);
        mainLayout->addWidget(mView, 2);
        mainLayout->addWidget(colorGroup, 2);
        mainLayout->addWidget(generalGroup, 1);
        mScene->setSceneRect(frameGeometry());
        setLayout(mainLayout);
    }

    //****************************************************************************/
    QtGradientWidget::~QtGradientWidget(void)
    {
        //QMessageBox::information(0, "test", "test"); // Test
    }


    //****************************************************************************/
    void QtGradientWidget::setGradientDimension (qreal width, qreal heightOffsetFraction)
    {
        mGradientHeightOffsetFraction = heightOffsetFraction;
        mGradientWidth = width;
        qreal heightOffset = heightOffsetFraction * (height() - DEFAULT_GRADIENT_HEIGHT);
        mMarkerSize.setWidth(16.0f);
        mMarkerSize.setHeight(16.0f);
        mAlphaY = heightOffset - 1.5f * mMarkerSize.width() - 1.0f;
        mColorY = heightOffset + DEFAULT_GRADIENT_HEIGHT - 6.0f + mMarkerSize.width();
        mGradient->setPos(0.5 * mMarkerSize.width(), heightOffset);
        QSizeF gradientSize;
        gradientSize.setWidth(width - 0.5 * mMarkerSize.width());
        gradientSize.setHeight(DEFAULT_GRADIENT_HEIGHT);
        mMarkerMinX = 0.5 * mMarkerSize.width();
        mMarkerMaxX = gradientSize.width();
        mGradient->setSize(gradientSize);
    }

    //****************************************************************************/
    bool QtGradientWidget::eventFilter(QObject* object, QEvent* event)
    {
        QGraphicsSceneMouseEvent* mouseEvent = (QGraphicsSceneMouseEvent*) event;
        switch ((int) event->type())
        {
            case QEvent::GraphicsSceneMousePress:
                mouseClickHandler(mouseEvent);
            break;

            case QEvent::GraphicsSceneMouseMove:
            {
                mouseMoveHandler(mouseEvent);
            }
            break;

            case QEvent::GraphicsSceneWheel:
            {
                mouseEvent->accept();
            }
            break;
        }
        return QObject::eventFilter(object, event);
    }

    //****************************************************************************/
    QGraphicsItem* QtGradientWidget::itemAt(const QPointF& pos)
    {
        QList<QGraphicsItem*> items = mScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

        foreach(QGraphicsItem* item, items)
            if (item->isVisible())
                return item;

        return 0;
    }

    //****************************************************************************/
    void QtGradientWidget::mouseClickHandler(QGraphicsSceneMouseEvent* mouseEvent)
    {
        switch ((int) mouseEvent->button())
        {
            case Qt::LeftButton:
            {
                // Don't proceed if there is an item at the mouse position
                QGraphicsItem* item = itemAt(mouseEvent->scenePos());
                if (item)
                {
                    if (item->data(KEY_GRADIENT_MARKER).toInt() != VALUE_GRADIENT_MARKER_COLOR &&
                        item->data(KEY_GRADIENT_MARKER).toInt() != VALUE_GRADIENT_MARKER_ALPHA)
                            return;

                    if (mCurrentMarker)
                        mCurrentMarker->selectMarker(false);

                    QtGradientMarker* marker = static_cast<QtGradientMarker*>(item->parentItem());
                    marker->selectMarker(true);
                    mCurrentMarker = marker;

                    if (item->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_COLOR)
                    {
                        enableAlphaWidgets(false);
                        mCurrentColorValue = marker->getColor();
                        updateHexWithCurrentColor();
                        valueChangedHex();
                        enableColorWidgets(true);
                    }
                    else if (item->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_ALPHA)
                    {
                        enableColorWidgets(false);
                        mAlphaSlider->setValue(marker->getAlpha());
                        mAlphaEdit->setText(QVariant(marker->getAlpha()).toString());
                        enableAlphaWidgets(true);
                    }

                    return;
                }

                // If the mouse is at the top-level, add an alpha marker
                // If the mouse is at the bottom-level, add a color marker
                if (mouseEvent->scenePos().y() < mAlphaY + 1.5f * mMarkerSize.width())
                {
                    createGradientMarkerAlpha(mouseEvent);
                    enableColorWidgets(false);
                    enableAlphaWidgets(true);
                }
                else if (mouseEvent->scenePos().y() > mColorY - mMarkerSize.width())
                {
                    createGradientMarkerColor(mouseEvent);
                    enableColorWidgets(true);
                    enableAlphaWidgets(false);
                }
            }
            break;
        }
    }

    //****************************************************************************/
    void QtGradientWidget::mouseMoveHandler(QGraphicsSceneMouseEvent* mouseEvent)
    {
        if (!mCurrentMarker)
            return;

        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            QPointF pos = mouseEvent->scenePos();
            if (pos.x() < mMarkerMinX)
                pos.setX(mMarkerMinX);
            if (pos.x() > mMarkerMaxX + 0.5 * mMarkerSize.width())
                pos.setX(mMarkerMaxX + 0.5 * mMarkerSize.width());

            pos.setX(pos.x() - 0.5 * mMarkerSize.width());
            mCurrentMarker->setPos(pos.x(), mCurrentMarker->pos().y());
            qreal newFraction = pos.x() / (mMarkerMaxX - mMarkerMinX + 0.5 * mMarkerSize.width());

            if (mCurrentMarker->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_COLOR)
                mGradient->changeColorFraction(mCurrentMarker->getId(), newFraction);
            else if (mCurrentMarker->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_ALPHA)
                mGradient->changeAlphaFraction(mCurrentMarker->getId(), newFraction);

            mCurrentMarker->setFraction(newFraction);
        }
    }

    //****************************************************************************/
    QtGradientMarkerColor* QtGradientWidget::createGradientMarkerColor(QGraphicsSceneMouseEvent* mouseEvent)
    {
        QPointF pos = mouseEvent->scenePos();
        if (pos.x() < mMarkerMinX)
            pos.setX(0.0f);
        if (pos.x() > mMarkerMaxX)
            pos.setX(mMarkerMaxX);

        qreal fraction = pos.x() / (mMarkerMaxX - mMarkerMinX);
        QtGradientMarkerColor* marker = createGradientMarkerColor(fraction);
        marker->setPos(pos.x(), mColorY);
        return marker;
    }

    //****************************************************************************/
    QtGradientMarkerColor* QtGradientWidget::createGradientMarkerColor(qreal fraction, const QColor color)
    {
        QtGradientMarkerColor* marker = new QtGradientMarkerColor();
        marker->setFraction(fraction);
        marker->setColor(color);
        mScene->addItem(marker);
        marker->setMarkerSize(mMarkerSize);
        if (mCurrentMarker)
            mCurrentMarker->selectMarker(false);

        marker->selectMarker(true);
        mCurrentMarker = marker;
        mCurrentColorValue = marker->getColor();
        updateHexWithCurrentColor();
        valueChangedHex();
        marker->setId(idCounter);
        ++idCounter;
        mGradient->addColor(mCurrentMarker->getId(), fraction, marker->getColor());
        return marker;
    }

    //****************************************************************************/
    QtGradientMarkerAlpha* QtGradientWidget::createGradientMarkerAlpha(QGraphicsSceneMouseEvent* mouseEvent)
    {
        QPointF pos = mouseEvent->scenePos();
        if (pos.x() < mMarkerMinX)
            pos.setX(0.0f);
        if (pos.x() > mMarkerMaxX)
            pos.setX(mMarkerMaxX);

        qreal fraction = pos.x() / (mMarkerMaxX - mMarkerMinX);
        QtGradientMarkerAlpha* marker = createGradientMarkerAlpha(fraction);
        marker->setPos(pos.x(), mAlphaY);
        return marker;
    }

    //****************************************************************************/
    QtGradientMarkerAlpha* QtGradientWidget::createGradientMarkerAlpha(qreal fraction, qreal alpha)
    {
        QtGradientMarkerAlpha* marker = new QtGradientMarkerAlpha();
        marker->setFraction(fraction);
        marker->setAlpha(alpha);
        mScene->addItem(marker);
        marker->setMarkerSize(mMarkerSize);
        if (mCurrentMarker)
            mCurrentMarker->selectMarker(false);

        marker->selectMarker(true);
        mCurrentMarker = marker;
        mAlphaSlider->setValue(marker->getAlpha());
        mAlphaEdit->setText(QVariant(marker->getAlpha()).toString());
        marker->setId(idCounter);
        ++idCounter;
        mGradient->addAlpha(mCurrentMarker->getId(), fraction, marker->getColor().alpha());
        return marker;
    }

    //****************************************************************************/
    void QtGradientWidget::alphaSliderValueChanged(int value)
    {
        // Synchronize editbox and slider
        mAlphaEdit->setText(QVariant(value).toString());
        if (mCurrentMarker)
        {
            mCurrentMarker->setAlpha(value);
            mGradient->changeAlpha(mCurrentMarker->getId(), value);
        }
    }

    //****************************************************************************/
    void QtGradientWidget::alphaEditValueChanged(QString value)
    {
        // Synchronize editbox and slider
        int intValue = QVariant(mAlphaEdit->text()).toInt();
        mAlphaSlider->setValue(intValue);
        if (mCurrentMarker)
        {
            mCurrentMarker->setAlpha(intValue);
            mGradient->changeAlpha(mCurrentMarker->getId(), intValue);
        }
    }

    //****************************************************************************/
    void QtGradientWidget::updateColorFill (void)
    {
        QString left = QString("background-color: rgb(");
        QString mid = QVariant(mCurrentColorValue.red()).toString() + QString(",") +
                      QVariant(mCurrentColorValue.green()).toString() + QString(",") +
                      QVariant(mCurrentColorValue.blue()).toString();
        QString right = QString("); border: 1px solid #000000;");
        mColorFill->setStyleSheet(left + mid + right);

        // Update the marker and the gradient
        if (mCurrentMarker)
        {
            mCurrentMarker->setColor(mCurrentColorValue);
            mGradient->changeColor(mCurrentMarker->getId(), mCurrentColorValue);
        }
    }

    //****************************************************************************/
    void QtGradientWidget::updateRGBWithCurrentColor(void)
    {
        mR->setText(QVariant(mCurrentColorValue.red()).toString());
        mG->setText(QVariant(mCurrentColorValue.green()).toString());
        mB->setText(QVariant(mCurrentColorValue.blue()).toString());
    }

    //****************************************************************************/
    void QtGradientWidget::updateHexWithCurrentColor(void)
    {
        QString h = mCurrentColorValue.name();
        h.remove(QRegExp(QString::fromUtf8("#")));
        mHex->setText(h);
    }

    //****************************************************************************/
    void QtGradientWidget::valueChangedRGB(void)
    {
        mCurrentColorValue = QColor(QVariant(mR->text()).toInt(),
                             QVariant(mG->text()).toInt(),
                             QVariant(mB->text()).toInt());
        updateHexWithCurrentColor();
        updateColorFill();
    }

    //****************************************************************************/
    void QtGradientWidget::dialogButtonClicked(void)
    {
        QColorDialog dialog;
        QColor c = dialog.getColor();
        mCurrentColorValue = c;
        updateHexWithCurrentColor();
        valueChangedHex();
    }

    //****************************************************************************/
    void QtGradientWidget::valueChangedHex(void)
    {
        mCurrentColorValue.setNamedColor(QString("#") + mHex->text());
        updateRGBWithCurrentColor();
        updateColorFill();
    }

    //****************************************************************************/
    void QtGradientWidget::enableAlphaWidgets(bool enabled)
    {
        mAlphaSlider->setEnabled(enabled);
        mAlphaEdit->setEnabled(enabled);
        mDeleteAlphaMarkerButton->setEnabled(enabled);
    }

    //****************************************************************************/
    void QtGradientWidget::enableColorWidgets(bool enabled)
    {
        mR->setEnabled(enabled);
        mG->setEnabled(enabled);
        mB->setEnabled(enabled);
        mHex->setEnabled(enabled);
        mDialogButton->setEnabled(enabled);
        mDeleteColorMarkerButton->setEnabled(enabled);
    }

    //****************************************************************************/
    void QtGradientWidget::deleteAlphaMarker(void)
    {
        if (mCurrentMarker)
        {
            if (mCurrentMarker->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_ALPHA)
            {
                mGradient->deleteAlpha(mCurrentMarker->getId());
                enableAlphaWidgets(false);
            }

            mScene->removeItem(mCurrentMarker);
            mCurrentMarker = 0;
        }
    }

    //****************************************************************************/
    void QtGradientWidget::deleteColorMarker(void)
    {
        if (mCurrentMarker)
        {
            if (mCurrentMarker->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_COLOR)
            {
                mGradient->deleteColor(mCurrentMarker->getId());
                enableColorWidgets(false);
            }

            mScene->removeItem(mCurrentMarker);
            mCurrentMarker = 0;
        }
    }

    //****************************************************************************/
    void QtGradientWidget::clearGradient(void)
    {
        // Remove markers
        QList<QGraphicsItem*> items = mScene->items();

        foreach(QGraphicsItem* item, items)
            if (item->isVisible())
                if (item->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_COLOR ||
                    item->data(KEY_GRADIENT_MARKER).toInt() == VALUE_GRADIENT_MARKER_ALPHA)
                        mScene->removeItem(item);

        // Initialize the gradient and buttons of the widget
        mGradient->init();
        enableColorWidgets(false);
        enableAlphaWidgets(false);
        mCurrentMarker = 0;
    }

    //****************************************************************************/
    QMultiMap<qreal, QColor> QtGradientWidget::getColorMap(void)
    {
        QMultiMap<qreal, QColor> map;
        map = mGradient->getColorMap();
        return map;
    }

    //****************************************************************************/
    void QtGradientWidget::setColorMap(QMultiMap<qreal, QColor> colorMap)
    {
        QMultiMap<qreal, QColor>::iterator iC;
        qreal fraction;
        QColor color;
        for (iC = colorMap.begin(); iC != colorMap.end(); ++iC)
        {
            fraction = iC.key();
            color = iC.value();
            color.setAlpha(255);
            QtGradientMarkerColor* colorMarker = createGradientMarkerColor(fraction, color);
            colorMarker->setPos(fraction * mMarkerMaxX, mColorY);
        }
    }

    //****************************************************************************/
    QMultiMap<qreal, int> QtGradientWidget::getAlphaMap(void)
    {
        QMultiMap<qreal, int> map;
        map = mGradient->getAlphaMap();
        return map;
    }

    //****************************************************************************/
    void QtGradientWidget::setAlphaMap(QMultiMap<qreal, int> alphaMap)
    {
        QMultiMap<qreal, int>::iterator iA;
        qreal fraction;
        qreal alpha;
        for (iA = alphaMap.begin(); iA != alphaMap.end(); ++iA)
        {
            fraction = iA.key();
            alpha = iA.value();
            QtGradientMarkerAlpha* alphaMarker = createGradientMarkerAlpha(fraction, alpha);
            alphaMarker->setPos(fraction * mMarkerMaxX, mAlphaY);
        }
    }

    //****************************************************************************/
    QMultiMap<qreal, QColor> QtGradientWidget::exportColorAndAlpha(void)
    {
        QMultiMap<qreal,QColor> map = mGradient->exportColorAndAlpha();
        return map;
    }

    //****************************************************************************/
    void QtGradientWidget::importColorAndAlpha(QMultiMap<qreal, QColor> colorMap)
    {
        QMultiMap<qreal, QColor>::iterator iC;
        qreal fraction;
        qreal alpha;
        QColor color;
        for (iC = colorMap.begin(); iC != colorMap.end(); ++iC)
        {
            fraction = iC.key();
            color = iC.value();
            alpha = color.alpha();
            color.setAlpha(255);
            QtGradientMarkerColor* colorMarker = createGradientMarkerColor(fraction, color);
            colorMarker->setPos(fraction * mMarkerMaxX, mColorY);
            QtGradientMarkerAlpha* alphaMarker = createGradientMarkerAlpha(fraction, alpha);
            alphaMarker->setPos(fraction * mMarkerMaxX, mAlphaY);
        }
    }

}
