#include "CrossPE.h"
#include <Windows.h>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

CrossPE::CrossPE(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void CrossPE::on_open_file_triggered() {
    QString curPath = QDir::currentPath();
    QString dlgTitle = "��PE�ļ�";
    QString filter = "PE�ļ�(*.exe, *.dll);;�����ļ�(*.*)";
    QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
    if (aFileName.isEmpty()) {
        return;
    }
    ui.lineEditFileName->setText(aFileName);
    peFileName = aFileName;
}


void CrossPE::on_btnViewSections_clicked() {
    if (peFileName.isEmpty()) {
        MessageBox(NULL, L"No file opened!", L"Error", MB_OK);
        return;
    }
}