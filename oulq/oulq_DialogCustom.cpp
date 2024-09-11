#include"oulq_DialogCustom.h"

#include "ui_oulq_DialogInitialization.h"

namespace rw {
    namespace oulq {
        DialogInit::DialogInit(QWidget* parent)
            : QDialog(parent) {
            setWindowTitle("Initializing...");
            QVBoxLayout* layout = new QVBoxLayout(this);
            QLabel* label = new QLabel("Initializing, please wait...", this);
            layout->addWidget(label);
            setLayout(layout);
            setModal(true); // 设置为模态窗口
        }

        DialogInitialization::DialogInitialization(QWidget* parent)
            : QDialog(parent)
            , ui(new Ui::DialogInitializationClass())
        {
            ui->setupUi(this);
            setModal(true); // 设置为模态窗口
            setWindowFlags(Qt::FramelessWindowHint);
        }

        DialogInitialization::~DialogInitialization()
        {
            delete ui;
        }

        void DialogInitialization::setInitializeInfo(const QString& info)
        {
            ui->label_initializeInfo->setText(info);
        }

    }
}