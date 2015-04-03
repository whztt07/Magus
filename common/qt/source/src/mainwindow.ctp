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
#include <QString>
#include <QFile>
#include <QMenuBar>
#include "mainwindow.h"
${mainwindow.includes}

//****************************************************************************/
MainWindow::MainWindow(void) : mIsClosing(false)
{
${MainWindow::MainWindow}
${MainWindow::ogreConstructor}
    
	// Perform standard functions
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

${MainWindow::createOgreWidget}
    // Set the title
    setWindowTitle(QString("${MainWindow::MainWindow.title}"));

    // Set the stylesheet of the application
    QFile File(QString("${MainWindow::MainWindow.style}"));
    File.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(File.readAll());
    setStyleSheet(styleSheet);
	showMaximized();
}

//****************************************************************************/
MainWindow::~MainWindow(void)
{
${MainWindow::ogreDestructor}
}

//****************************************************************************/
void MainWindow::closeEvent(QCloseEvent* event)
{
    mIsClosing = true;
}

//****************************************************************************/
void MainWindow::createActions(void)
{
${MainWindow::createActions}
}

//****************************************************************************/
void MainWindow::createMenus(void)
{
${MainWindow::createMenus}
}

//****************************************************************************/
void MainWindow::createToolBars(void)
{
${MainWindow::createToolBars}
}

//****************************************************************************/
void MainWindow::createStatusBar(void)
{
${MainWindow::createStatusBar}
}

//****************************************************************************/
void MainWindow::createDockWindows(void)
{
${MainWindow::createDockWindows}
}

${MainWindow::doActions}
//****************************************************************************/
void MainWindow::update(void)
{
${MainWindow::ogreUpdate}
}