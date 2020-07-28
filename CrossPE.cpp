#include "CrossPE.h"
#include <Windows.h>
#include <qdebug.h>
#include <qbytearray.h>
#include <qmessagebox.h>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

CrossPE::CrossPE(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    peImage = new PEImage();
    sectionsView = NULL;

    // connections
    connect(this, SIGNAL(fileNameIsReady()), this, SLOT(peImageLoad()));
    connect(ui.btnOpenSectionView, SIGNAL(clicked()), this, SLOT(openPESectionsView()));
}

CrossPE::~CrossPE() {
    // �ͷž���
    if(peImage != nullptr){
        delete peImage;
        peImage = nullptr;
    }
}

void CrossPE::dealWithArgsFile() {
    // ������϶��ļ���exe�ϻ��߿�ݷ�ʽ���֣�windows����ļ�·���ŵ�����������
    QStringList cmdline_args = QCoreApplication::arguments();
    if (!cmdline_args.isEmpty()) {
        QString aFileName = cmdline_args[0];
        // TODO: check file validity
        ui.lineEditFileName->setText(aFileName);
    }
    emit fileNameIsReady();
}

void CrossPE::fileOpenFromMemuBar() {
    // ͨ���˵���һ���ļ�
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
    QByteArray ba = peFileName.toLatin1();
    peImage->loadPEImage(ba.data());
}



void CrossPE::openPESectionsView() {
    if (peFileName.isEmpty()) {
        //MessageBox(NULL, L"No file opened!", L"Error", MB_OK);
        QMessageBox::information(this, "Error", "No file opened!", MB_OK);
        return;
    }

    sectionsView = new SectionsView(peImage);
    sectionsView->setAttribute(Qt::WA_DeleteOnClose);
    sectionsView->setWindowFlag(Qt::Window, true);
    sectionsView->show();
}

