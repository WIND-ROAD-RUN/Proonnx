#pragma once

#include <QDialog>
#include "ui_DlgSelectProductConfig.h"
#include <QSharedPointer>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class DlgSelectProductConfigClass; };
QT_END_NAMESPACE

class DlgSelectProductConfig : public QDialog
{
    Q_OBJECT

public:
    DlgSelectProductConfig(QWidget *parent = nullptr);
    ~DlgSelectProductConfig();
private:
    QSharedPointer<QStandardItemModel> m_standItemModelForTreeView;
private:
    void ini_ui();

    void ini_gbox_dataDisplay();

private:

    void ini_connect();
public:
    QString getSelectConfigPath();

private:
    Ui::DlgSelectProductConfigClass *ui;

private slots:
    void pbtn_ok_clicked();

    void pbtn_cancel_clicked();

    void treeView_Clicked(const QModelIndex& index);
};
