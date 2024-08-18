#ifndef OULQ_DIALOGCUSTOM_H_
#define OULQ_DIALOGCUSTOM_H_

#include"oulq_global.h"

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QThread>
#include<QString>

QT_BEGIN_NAMESPACE
namespace Ui { class DialogInitializationClass; };
QT_END_NAMESPACE

namespace rw {
    namespace oulq {
        class DialogInit
            : public QDialog {
        public:
            DialogInit(QWidget* parent = nullptr);

        };


        class DialogInitialization 
            : public QDialog
        {
            Q_OBJECT

        public:
            DialogInitialization(QWidget* parent = nullptr);
            ~DialogInitialization();

        private:
            Ui::DialogInitializationClass* ui;

        public:
            void setInitializeInfo(const QString & info);

        };
    }
}

#endif // !OULQ_DIALOGCUSTOM_H_
