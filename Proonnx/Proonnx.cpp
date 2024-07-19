#include "Proonnx.h"

#include<QMessageBox>
#include<QComboBox>

#include"DlgAddProductConfig.h"

void Proonnx::ini_ui()
{
    ini_localizationStringLoader();
    ini_localizationStringLoaderUI();
}

void Proonnx::ini_localizationStringLoader()
{
    m_locStrLoader = LocalizationStringLoaderXML::getInstance();
    m_locStrLoader->setFilePath("C:\\Users\\61795\\Desktop\\Project\\Proonnx\\Proonnx\\languageString.xml");
    m_locStrLoader->setLanguage("CHN");
    auto loadStrDataResult = m_locStrLoader->loadData();
    if (!loadStrDataResult) {
        QMessageBox::warning(this, "ERROR", "Failed to load data file languageString.xml");
    }
}

void Proonnx::ini_localizationStringLoaderUI()
{
    ui->gBox_monitoringDisplay->setTitle(QString::fromStdString(m_locStrLoader->getString("1")));
    ui->gBox_setPutton->setTitle(QString::fromStdString(m_locStrLoader->getString("2")));
    ui->pbt_modProductConfig->setText(QString::fromStdString(m_locStrLoader->getString("3")));
    ui->pbt_addCamera->setText(QString::fromStdString(m_locStrLoader->getString("4")));
    ui->pbt_addProductCongfig->setText(QString::fromStdString(m_locStrLoader->getString("5")));
    ui->pbt_modCameraConfig->setText(QString::fromStdString(m_locStrLoader->getString("6")));
}

void Proonnx::ini_connect()
{
    QObject::connect(ui->cBox_changeLanguage, SIGNAL(currentIndexChanged(int)),
        this, SLOT(cBox_changeLanguage_index_change_on(int)));
    QObject::connect(ui->pbt_addProductCongfig, SIGNAL(clicked()),
        this, SLOT(pbt_addProductCongfig_clicked()));
}

void Proonnx::des_com()
{
    delete m_locStrLoader;
}

Proonnx::Proonnx(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProonnxClass())
{
    ui->setupUi(this);
    ini_ui();
    ini_connect();

}

Proonnx::~Proonnx()
{
    delete ui;
}

void Proonnx::pbt_addProductCongfig_clicked()
{
    DlgAddProductConfig dlg;
    dlg.exec();

}

void Proonnx::cBox_changeLanguage_index_change_on(int index)
{
    if (index==0) {
        m_locStrLoader->setLanguage("CHN");
        auto loadStrDataResult = m_locStrLoader->loadData();
    }
    else if(index == 1){
        m_locStrLoader->setLanguage("USA");
        auto loadStrDataResult = m_locStrLoader->loadData();
    }
    ini_localizationStringLoaderUI();
}
