#include "oulq_clickedLabel.h"

namespace rw {
    namespace oulq {
        ClickableLabel::ClickableLabel(QWidget* parent)
            : QLabel(parent) {

        }

        void ClickableLabel::mousePressEvent(QMouseEvent* event)  {
            if (event->button() == Qt::LeftButton) {
                if (m_enbaleClicked) {
                    emit clicked(m_index);
                }

            }
            QLabel::mousePressEvent(event);
        }
    }

}