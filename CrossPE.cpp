#include "CrossPE.h"
#include <Windows.h>
#include <qdebug.h>


#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

CrossPE::CrossPE(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    peImage = new PEImage();

    // connections
    connect(this, SIGNAL(fileNameIsReady()), this, SLOT(peImageLoad()));
}

CrossPE::~CrossPE() {
    // �ͷž���
    if(peImage != nullptr){
        delete peImage;
        peImage = nullptr;
    }
}

void CrossPE::dealWithArgsFile() {
    QStringList cmdline_args = QCoreApplication::arguments();
    if (!cmdline_args.isEmpty()) {
        QString aFileName = cmdline_args[0];
        // TODO: check file validity
        ui.lineEditFileName->setText(aFileName);
    }
    emit fileNameIsReady();
}

void CrossPE::fileOpenFromMemuBar() {
    QString curPath = QDir::currentPath();
    QString dlgTitle = "��PE�ļ�";
    QString filter = "PE�ļ�(*.exe, *.dll);;�����ļ�(*.*)";
    QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
    if (aFileName.isEmpty()) {
        return;
    }
    ui.lineEditFileName->setText(aFileName);
    peFileName = aFileName;
    emit fileNameIsReady();
}



void CrossPE::peImageLoad() {
    // peImage->loadimage();
    int a = 100;
}



void CrossPE::openPESectionsView() {
    if (peFileName.isEmpty()) {
        MessageBox(NULL, L"No file opened!", L"Error", MB_OK);
        return;
    }
}

