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
    peImage = NULL;
    sectionsView = NULL;
    this->setAcceptDrops(true);

    // connections
    connect(this, SIGNAL(fileNameIsReady()), this, SLOT(peImageLoad()));
    connect(ui.btnOpenSectionView, SIGNAL(clicked()), this, SLOT(openPESectionsView()));
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

void CrossPE::dragEnterEvent(QDragEnterEvent* event) {
    qDebug() << "dragEnterEvent";
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

