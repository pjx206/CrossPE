#include "ImportView.h"

ImportView::ImportView(PEImage* peimage) {
	image = peimage;
	ui.setupUi(this);

	this->setLayout(ui.verticalLayout);

	iidImport = image->iidImport;
	currentDllRow = 0;
	setupImportTable();

	// ���dll�б��ʱ�򣬸���dll��import descriptor��Ҳ���Ǹ���INT
	connect(ui.dllsTable, SIGNAL(itemClicked(QTableWidgetItem * item)),
		ui.impDespTable, SLOT(updataImpDespTable(QTableWidgetItem * item)));
}

ImportView::~ImportView() {

}

void ImportView::setupImportTable() {
	QTableWidget* table = ui.dllsTable;

	// ������������
	int dllsCnt = image->getImportNum();
	table->setRowCount(dllsCnt);
	table->setColumnCount(6);

	// ����Ҫչʾ����
	QStringList headerText;
	headerText << "Dll Name" << "OriginalFirstThunk" << "TimeDataStamp"
			   << "ForwardChain" << "Name" << "FirstThunk";
	table->setHorizontalHeaderLabels(headerText);

	QString tmp;
	PIMAGE_IMPORT_DESCRIPTOR iid = (PIMAGE_IMPORT_DESCRIPTOR)
		image->getPtrInBufferByVA(iidImport->VirtualAddress);
	for (int row = 0; row < dllsCnt; row++, iid++) {
		int col = 0;
		// Dll Name:
		tmp = (char*)(image->getPtrInBufferByVA(iid->Name));
		table->setItem(row, col++, new QTableWidgetItem(tmp));

		// Other:
		tmp.sprintf("%08X", iid->OriginalFirstThunk);
		table->setItem(row, col++, new QTableWidgetItem(tmp));
		tmp.sprintf("%08X", iid->TimeDateStamp);
		table->setItem(row, col++, new QTableWidgetItem(tmp));
		tmp.sprintf("%08X", iid->ForwarderChain);
		table->setItem(row, col++, new QTableWidgetItem(tmp));
		tmp.sprintf("%08X", iid->Name);
		table->setItem(row, col++, new QTableWidgetItem(tmp));
		tmp.sprintf("%08X", iid->FirstThunk);
		table->setItem(row, col++, new QTableWidgetItem(tmp));
	}

}

void ImportView::updataImpDespTable(QTableWidgetItem* item) {
	//int clicked_row = item->row();
	//// ��֮ǰ��ͬһ�е�dll)
	//if(clicked_row == currentDllRow)
	//	return;

	//// ����Ҫ��������

	//int thunkNum = ;

	//// �����
	//for (int row = 0; row < thunkNum; row++) {
	//	fillOneRowWithQList(ui.impDespTable, row, )
	//}
}
