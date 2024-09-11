#include "oulq_LabelCustom.h"

#include <QMouseEvent>

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

        LabelFrameSelectable::LabelFrameSelectable(QWidget* parent)
            : QLabel(parent), isSelecting(false), selectionEnabled(false) 
        {
        
        }

        void LabelFrameSelectable::enableSelection(bool enable)
        {
            selectionEnabled = enable;
        }

        void LabelFrameSelectable::setLastSelectionRect(const QRect& rect)
        {
            lastSelectionRect = rect;
            QPainter painter(this);
            QPen pen;
            pen.setColor(Qt::red);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            painter.drawRect(lastSelectionRect);
        }

        void LabelFrameSelectable::mousePressEvent(QMouseEvent* event)
        {
            if (selectionEnabled && event->button() == Qt::LeftButton) {
                isSelecting = true;
                startPoint = event->pos();
                endPoint = startPoint;
                update();
            }
        }

        void LabelFrameSelectable::mouseMoveEvent(QMouseEvent* event)
        {
            if (isSelecting) {
                endPoint = event->pos();
                currentSelectionRect = QRect(startPoint, endPoint).normalized();
                update();
            }
        }

        void LabelFrameSelectable::mouseReleaseEvent(QMouseEvent* event)
        {
            if (isSelecting && event->button() == Qt::LeftButton) {
                isSelecting = false;
                endPoint = event->pos();
                currentSelectionRect = QRect(startPoint, endPoint).normalized();
                lastSelectionRect = currentSelectionRect;//Save the previous selected rectangle
                emit selectionMade(currentSelectionRect);
                update();
            }
        }

        void LabelFrameSelectable::paintEvent(QPaintEvent* event)
        {
            QLabel::paintEvent(event);
            QPainter painter(this);
            QPen pen;
            pen.setColor(Qt::red);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);

            //Draw the previous selection rectangle
            if (!lastSelectionRect.isNull()) {
                painter.drawRect(lastSelectionRect);
            }

            //Draw the current bounding rectangle
            if (isSelecting) {
                painter.drawRect(currentSelectionRect);
            }
        }

        void LabelFrameSelectable::paintLastRange()
        {
            QPainter painter(this);
            QPen pen;
            pen.setColor(Qt::red);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);

            //Draw the previous selection rectangle
            if (!lastSelectionRect.isNull()) {
                painter.drawRect(lastSelectionRect);
            }
        }

    }

}