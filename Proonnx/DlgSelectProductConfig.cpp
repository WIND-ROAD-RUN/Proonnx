#include "DlgSelectProductConfig.h"

#include"cfgr/cfgr_CatalogueInitializer.h"

#include"ProductConfigLoader.h"

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QStringList>
#include <QStandardItemModel>


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
    //检查索引是否有效
    if (index.isValid()) {
        //获取视图中被点击的项
        auto item=m_standItemModelForTreeView->itemFromIndex(index);
        //获取文件路径
        auto filePath = item->data().toString();
        //将文件路径展示在UI界面
        ui->label_filePath->setText(filePath);
        //将确认按钮设为可选
        ui->pbtn_ok->setEnabled(true);
        ProductConfigLoader loader;
        //加载产品配置
        auto config = loader.loadProductConfig(filePath.toStdString());
        QString productName =  QString::fromStdString(config.productName) ;

        auto countinfo = loader.loadProductCountInfo(filePath.toStdString());
        QString totalNum =  QString::fromStdString(std::to_string(countinfo.totalCount));
        QString passNum = QString::fromStdString(std::to_string(countinfo.passCount));
        QString outNum = QString::fromStdString(std::to_string(countinfo.outCount));

        auto attribute = loader.loadRejectAttribute(filePath.toStdString());
        QString  productDelay =  QString::fromStdString(std::to_string(attribute.RejectDelay)) ;
        QString productOffset =  QString::fromStdString(std::to_string(attribute.OffsetsNumber));
        QString disposalTime=  QString::fromStdString(std::to_string(attribute.DisposalTime));


        QStandardItemModel* m_model = new QStandardItemModel(7,2);
        //std::unique_ptr<QStandardItemModel> m_model(new QStandardItemModel(4,2));
        QStringList headerHorizontal;
        headerHorizontal << "属性" << "值";
        QStringList headerVertical = { "名称" ,"延迟" , "偏移量" , "处理时间(ms)","产品总数","产品合格数","产品不合格数"};
        for (int i = 0; i < headerVertical.size(); ++i)
        {
            m_model->setItem(i, 0, new QStandardItem(headerVertical[i]));
            m_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        }

        QStringList text;
        text << productName << productDelay << productOffset << disposalTime << totalNum << passNum << outNum;

        for (int i = 0; i < headerVertical.size(); ++i)
        {
            m_model->setItem(i, 1, new QStandardItem(text[i]));
            m_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        }

        m_model->setHorizontalHeaderLabels(headerHorizontal);
        ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QFont font;
        font.setPointSize(12);
        ui->tableView->setFont(font);

        ui->tableView->setModel(m_model);
        ui->tableView->show();
        //delete m_model;
    }
    else {
        ui->pbtn_ok->setEnabled(false);
    }
}

void DlgSelectProductConfig::pbtn_cancel_clicked()
{
    this->reject();
}
