#pragma once

#include <QDialog>
#include "ui_DlgForTest.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgForTestClass; };
QT_END_NAMESPACE

class ocrwork;
class DlgForTest : public QDialog
{
    Q_OBJECT

public:
    DlgForTest(QWidget *parent = nullptr);

    ~DlgForTest();

private:
    ocrwork* m_ocrwork{nullptr};

    void ini_ocrwork();

private:
    void ini_connect();

    void ini_ui();

private:
    Ui::DlgForTestClass *ui;

private slots:
    void pbtn_openFile_clicked();

    void pbtn_recognize_clicked();

};
