#include "CrossPE.h"
#include <Windows.h>
#include <qdebug.h>
#include <qbytearray.h>
#include <qmessagebox.h>
#include <qtextedit.h>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

CrossPE::CrossPE(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    peImage = NULL;
    sectionsView = NULL;
    this->setAcceptDrops(true);

    // connections
    connect(this, SIGNAL(fileNameIsReady()), this, SLOT(peImageLoad()));
    connect(ui.btnOpenSectionView, SIGNAL(clicked()), this, SLOT(openPESectionsView()));
    connect(this, SIGNAL(imageLoaded()), this, SLOT(afterImageLoaded()));
}

CrossPE::~CrossPE() {
    // �ͷž���
    if(peImage != NULL){
        delete peImage;
        peImage = NULL;
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

void CrossPE::fileOpenWithDialog() {
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

void CrossPE::dragEnterEvent(QDragEnterEvent* event) {
    // ���������һ���ļ��Ϸ�
    //qDebug() << "dragEnterEvent";
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void CrossPE::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;
    if (urls.count() > 1) {
        QMessageBox::information(this, "Unsupported operation", "One file at a time!", QMessageBox::Ok);
    }
    QString file_name = urls.last().toLocalFile();
    QString oldFileName = ui.lineEditFileName->text();
    if (!file_name.compare(oldFileName)) {
        QMessageBox::information(this, "Try again", "You opened the same file", QMessageBox::Ok);
        return;
    }
    ui.lineEditFileName->setText(file_name);
    peFileName = file_name;
    emit fileNameIsReady();
}

void CrossPE::peImageLoad() {
    QByteArray ba = peFileName.toLatin1();
    if (peImage != NULL) {
        delete peImage;
    }
    peImage = new PEImage();
    peImage->loadPEImage(ba.data());
    emit imageLoaded();
}


void CrossPE::afterImageLoaded() {
    // PE�ļ����ص��ڴ��Ժ�������Ҫ����Ĺ�������Ҫ�����û�����һЩ���ܣ���ʾpe�ļ������Ϣ�ȵȡ�
    ui.btnOpenSectionView->setEnabled(true);
    
    // ����te_basicInfo�ı����PE�ļ���Ϣ
    QTextEdit *te_basicInfo = ui.basicPEinfo;
    QString arch;
    QString baseAddr;
    QString entryAddr;
    WORD subsys;
    QString subSystem;

    QString filesize;
    filesize.sprintf("%X bytes", peImage->fileSize);

    if (peImage->is32bitPE) {
        arch = "x86";
        baseAddr.sprintf("%08X", peImage->ImageBase);
        entryAddr.sprintf("%08X", peImage->EntryPoint);
        subsys = peImage->OptHeader32->Subsystem;
    }
    else {
        arch = "x86-64";
        baseAddr.sprintf("%016X", peImage->ImageBase);
        entryAddr.sprintf("%016X", peImage->EntryPoint);
        subsys = peImage->OptHeader64->Subsystem;
    }
    char* subsys_vals[] = { "Unknown", "Native", "Windows GUI", "Windows CUI", "Other"};
    subSystem = (subsys < 4) ? subsys_vals[subsys] : subsys_vals[4];

    QString info;
    info = "Arch: " + arch + "   " + "File size: " + filesize + "\n" + 
        "Subsystem: " + subSystem + '\n' +
        "Base address: " + baseAddr + '\n' + "Entry: " + entryAddr;
    te_basicInfo->setEnabled(true);
    te_basicInfo->setText(info);
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

