#include "DlgSelectProductConfig.h"

#include"cfgr/cfgr_CatalogueInitializer.h"

#include"ProductConfigLoader.h"

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QStringList>


using namespace rw::cfgr;

DlgSelectProductConfig::DlgSelectProductConfig(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgSelectProductConfigClass())
{
    ui->setupUi(this);

    ini_ui();

    ini_connect();
}

DlgSelectProductConfig::~DlgSelectProductConfig()
{
    delete ui;
}

void DlgSelectProductConfig::ini_ui()
{
    ui->pbtn_ok->setEnabled(false);
    ini_gbox_dataDisplay();
}

void DlgSelectProductConfig::ini_gbox_dataDisplay()
{
    m_standItemModelForTreeView= QSharedPointer<QStandardItemModel>::create();
    m_standItemModelForTreeView->setHorizontalHeaderLabels({ "配置列表" });
    
    auto productConfigDirectoryPath = QString::fromStdString(CatalogueInitializer::findWorkPath("ProductConfig"));
    ui->label_filePath->setText(productConfigDirectoryPath);

    QDir dir(productConfigDirectoryPath);

    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    auto rootItem = m_standItemModelForTreeView->invisibleRootItem();
    for (const QFileInfo& fileInfo : fileInfoList)
    {
        QStandardItem* standItem = new QStandardItem();
        standItem->setEditable(false);
        standItem->setText(fileInfo.fileName());
        standItem->setData(fileInfo.absoluteFilePath());
        rootItem->appendRow(standItem);
    }
    
    ui->treeView->setModel(m_standItemModelForTreeView.get());

}

void DlgSelectProductConfig::ini_connect()
{
    QObject::connect(ui->pbtn_ok, &QPushButton::clicked
        , this, &DlgSelectProductConfig::pbtn_ok_clicked);

    QObject::connect(ui->pbtn_cancel, &QPushButton::clicked
        , this, &DlgSelectProductConfig::pbtn_cancel_clicked);

    QObject::connect(ui->treeView, &QTreeView::clicked
        , this, &DlgSelectProductConfig::treeView_Clicked);
}

QString DlgSelectProductConfig::getSelectConfigPath()
{
    return ui->label_filePath->text();
}

void DlgSelectProductConfig::pbtn_ok_clicked()
{
    this->accept();
}

void DlgSelectProductConfig::treeView_Clicked(const QModelIndex& index)
{
    if (index.isValid()) {
        auto item=m_standItemModelForTreeView->itemFromIndex(index);
        auto filePath = item->data().toString();
        ui->label_filePath->setText(filePath);
        ui->pbtn_ok->setEnabled(true);
        ProductConfigLoader loader;
        auto config = loader.loadProductConfig(filePath.toStdString());

    }
    else {
        ui->pbtn_ok->setEnabled(false);
    }
}

void DlgSelectProductConfig::pbtn_cancel_clicked()
{
    this->reject();
}
