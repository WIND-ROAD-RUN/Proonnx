#include "DlgSetProonnx.h"

DlgSetProonnx::DlgSetProonnx(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgSetProonnxClass())
{
	ui->setupUi(this);
}

DlgSetProonnx::~DlgSetProonnx()
{
	delete ui;
}
