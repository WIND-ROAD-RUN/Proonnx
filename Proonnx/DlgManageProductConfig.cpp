#include "DlgManageProductConfig.h"

#include "cfgr/cfgr_CatalogueInitializer.h"

#include "ProductConfigLoader.h"

#include <QDir>
#include <QString>
#include <QStringList>
#include <QStandardItemModel>
#include <QFileInfoList>

using namespace rw::cfgr;


DlgManageProductConfig::DlgManageProductConfig(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgManageProductConfigClass())
{
    ui->setupUi(this);

    ini_ui();

    ini_connect();
}

DlgManageProductConfig::~DlgManageProductConfig()
{
    delete ui;
}

void DlgManageProductConfig::ini_ui()
{
    ui->pbtn_ok->setEnabled(false);
    ini_gbox_dataDisplay();
}

void DlgManageProductConfig::ini_gbox_dataDisplay()
{
    m_tandItemModelForTreeView = QSharedPointer<QStandardItemModel>::create();
    m_tandItemModelForTreeView->setHorizontalHeaderLabels({ "配置列表" });

    auto productConfigDorectoryPath = QString::fromStdString(CatalogueInitializer::findWorkPath("ProductConfig"));
    ui->label_filePath->setText(productConfigDorectoryPath);

    QDir dir(productConfigDorectoryPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    auto rootItem = m_tandItemModelForTreeView->invisibleRootItem();
    for (const QFileInfo& fileInfo : fileInfoList)
    {
        QStandardItem* standItem = new QStandardItem();
        standItem->setEditable(false);
        standItem->setText(fileInfo.fileName());
        standItem->setData(fileInfo.absoluteFilePath());
        rootItem->appendRow(standItem);
    }

    ui->treeView->setModel(m_tandItemModelForTreeView.get());

}

void DlgManageProductConfig::ini_connect()
{
    QObject::connect(ui->pbtn_ok, &QPushButton::clicked
        , this, &DlgManageProductConfig::pbtn_ok_clicked);

    QObject::connect(ui->pbtn_cancle, &QPushButton::clicked
        , this, &DlgManageProductConfig::pbtn_cancle_clicked);

    QObject::connect(ui->treeView, &QTreeView::clicked
        , this, &DlgManageProductConfig::treeView_Clicked);
}

QString DlgManageProductConfig::getSelectConfigPath()
{
    return ui->label_filePath->text();
}

void DlgManageProductConfig::pbtn_ok_clicked()
{
    this->accept();
}

void DlgManageProductConfig::treeView_Clicked(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto item = m_tandItemModelForTreeView->itemFromIndex(index);
        auto filePath = item->data().toString();

        ui->label_filePath->setText(filePath);
        ui->pbtn_ok->setEnabled(true);

    }
    else
    {
        ui->pbtn_ok->setEnabled(false);
    }
}

void DlgManageProductConfig::pbtn_cancle_clicked()
{
    this->reject();
}
