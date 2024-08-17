#ifndef OULQ_CLICKEDLABEL_H_
#define OULQ_CLICKEDLABEL_H_

#include"oulq_global.h"

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

namespace rw {
    namespace oulq {
        class LabelClickable
            : public QLabel {
            Q_OBJECT

        public:
            LabelClickable(QWidget* parent = nullptr);

        signals:
            void clicked(int index);

        public:
            int m_index{ 0 };

            bool m_enbaleClicked{ false };

        protected:
            void mousePressEvent(QMouseEvent* event) override;

        };
    }

}


#endif // !OULQ_CLICKEDLABEL_H_
