#pragma once

#include <QDialog>
#include "ui_DlgManageProductConfig.h"
#include <QSharedPointer>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class DlgManageProductConfigClass; };
QT_END_NAMESPACE

class DlgManageProductConfig : public QDialog
{
    Q_OBJECT

public:
    DlgManageProductConfig(QWidget *parent = nullptr);
    ~DlgManageProductConfig();
private:
    QSharedPointer<QStandardItemModel> m_tandItemModelForTreeView;
private:
    void ini_ui();

    void ini_gbox_dataDisplay();

private:
    void ini_connect();

public:
    QString getSelectConfigPath();

private:
    Ui::DlgManageProductConfigClass *ui;

private slots:
    void pbtn_ok_clicked();

    void pbtn_cancle_clicked();

    void treeView_Clicked(const QModelIndex& index);
};
