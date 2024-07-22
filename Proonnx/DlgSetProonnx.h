#pragma once

#include <QDialog>
#include "ui_DlgSetProonnx.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSetProonnxClass; };
QT_END_NAMESPACE

class DlgSetProonnx : public QDialog
{
	Q_OBJECT

public:
	DlgSetProonnx(QWidget *parent = nullptr);
	~DlgSetProonnx();

private:
	Ui::DlgSetProonnxClass *ui;
};
