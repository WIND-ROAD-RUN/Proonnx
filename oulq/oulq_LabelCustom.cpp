#include "oulq_LabelCustom.h"

namespace rw {
    namespace oulq {
        LabelClickable::LabelClickable(QWidget* parent)
            : QLabel(parent) {

        }

        void LabelClickable::mousePressEvent(QMouseEvent* event)  {
            if (event->button() == Qt::LeftButton) {
                if (m_enbaleClicked) {
                    emit clicked(m_index);
                }

            }
            QLabel::mousePressEvent(event);
        }
    }

}