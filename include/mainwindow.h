#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PEManager.h"
#include "sectionsview.h"
#include "homepage.h"

#include <QMainWindow>
#include <qdockwidget.h>
#include <qdrag.h>
#include <qevent.h>
#include <qpoint.h>
#include <qrect.h>
#include <qfile.h>
#include <qiodevice.h>
#include <qmimedata.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qdebug.h>
#include <qfiledialog.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void externalFilepathGot(QString filepath); // TODO: 增加一个枚举参数，可指定具体来源，方便细化程序行为表现

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private slots:
    void on_actionOpen_triggered();
    void on_actionSections_View_triggered();
    //void on_actionHomepage_triggered();
    void onPeImageMemoryStatus(bool isReady);

    void displaySectionsView();

private:
    void checkArgs();
    void createDockWindows();

private:
    Ui::MainWindow *ui;

    QDockWidget *dock_sv = nullptr;
    QDockWidget *dock_hp = nullptr;
};
#endif // MAINWINDOW_H


