#include "FrameSelectLabel.h"

FrameSelectLabel::FrameSelectLabel(QWidget* parent) 
    : QLabel(parent), isSelecting(false), selectionEnabled(false) {}

void FrameSelectLabel::enableSelection(bool enable)
{
	selectionEnabled = enable;
}

void FrameSelectLabel::setLastSelectionRect(const QRect& rect)
{
    lastSelectionRect = rect;
    QPainter painter(this);
    painter.setPen(Qt::DashLine);
    painter.drawRect(lastSelectionRect);
}

void FrameSelectLabel::mousePressEvent(QMouseEvent* event)
{
    if (selectionEnabled && event->button() == Qt::LeftButton) {
        isSelecting = true;
        startPoint = event->pos();
        endPoint = startPoint;
        update();
    }
}

void FrameSelectLabel::mouseMoveEvent(QMouseEvent* event)
{
    if (isSelecting) {
        endPoint = event->pos();
        currentSelectionRect = QRect(startPoint, endPoint).normalized();
        update();
    }
}

void FrameSelectLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if (isSelecting && event->button() == Qt::LeftButton) {
        isSelecting = false;
        endPoint = event->pos();
        currentSelectionRect = QRect(startPoint, endPoint).normalized();
        lastSelectionRect = currentSelectionRect; // ������һ�εĿ�ѡ����
        emit selectionMade(currentSelectionRect);
        update();
    }
}

void FrameSelectLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::DashLine);

    // ������һ�εĿ�ѡ����
    if (!lastSelectionRect.isNull()) {
        painter.drawRect(lastSelectionRect);
    }

    // ���Ƶ�ǰ�Ŀ�ѡ����
    if (isSelecting) {
        painter.drawRect(currentSelectionRect);
    }
}
