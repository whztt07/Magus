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

#ifndef MAGUS_NODE_SCENE_H
#define MAGUS_NODE_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace Magus
{
    /****************************************************************************
     * Subclass of a QGraphicsScene. This is needed to prevent that a right
     * mousebutton press disables selected nodes. A right mousebutton press is
     * used to show a context menu (if enabled). Some contextmenu actions apply
     * to the selected nodes.
    ***************************************************************************/
    class QtNodeGraphicsScene : public QGraphicsScene
    {
        public:
            QtNodeGraphicsScene(QObject* parent = 0) : QGraphicsScene(parent){};
            virtual ~QtNodeGraphicsScene(void){};

            void mousePressEvent(QGraphicsSceneMouseEvent *event)
            {
                if (event->button() == Qt::RightButton)
                {
                    event->accept();
                    return;
                }
                QGraphicsScene::mousePressEvent(event);
            };
    };

}

#endif
