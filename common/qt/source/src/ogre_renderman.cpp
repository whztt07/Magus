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

#include "ogre_renderman.h"

//****************************************************************************/
OgreManager::OgreManager(void)
{
#ifdef _DEBUG
    mResourcesCfg = "config_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "config.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    // Create Ogre root and initialize
    mRoot = new Ogre::Root(mPluginsCfg);
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        mRoot->initialise(false);
    }

    // Start timer
    mTimer = new Ogre::Timer();
    mTimer->reset();
}

//****************************************************************************/
OgreManager::~OgreManager(void)
{
    // Delete Ogre root
    delete mRoot;
}

//****************************************************************************/
void OgreManager::renderOgreWidgetsOneFrame(void)
{
    if (mRoot)
    {
        // Determine time since last frame
        int timeSinceLastFrame = 1;
        int startTime = 0;

        // Render an one frame
        startTime = mTimer->getMillisecondsCPU();
        mRoot->renderOneFrame();
        timeSinceLastFrame = (mTimer->getMillisecondsCPU() - startTime) / 1000.0f;
        if(timeSinceLastFrame == 0) return;

        // Update all QtOgreWidgets
        QMap<int, QOgre::Widget*>::iterator i;
        QOgre::Widget* item = 0;
        for (i = mQtOgreWidgetMap.begin(); i != mQtOgreWidgetMap.end(); ++i)
        {
            item = i.value();
            item->updateOgre(timeSinceLastFrame);
        }
    }
}

//****************************************************************************/
void OgreManager::registerQtOgreWidget(int widgetId, QOgre::Widget* widget)
{
    mQtOgreWidgetMap[widgetId] = widget;
}

//****************************************************************************/
void OgreManager::unregisterQtOgreWidget(int widgetId)
{
    mQtOgreWidgetMap.remove(widgetId);
}

//****************************************************************************/
QOgre::Widget* OgreManager::getOgreWidget(int widgetId) const
{
    return mQtOgreWidgetMap.value(widgetId);
}
