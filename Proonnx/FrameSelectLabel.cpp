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
    QPen pen;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
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
        lastSelectionRect = currentSelectionRect;//Save the previous selected rectangle
        emit selectionMade(currentSelectionRect);
        update();
    }
}

void FrameSelectLabel::paintEvent(QPaintEvent* event)
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

void FrameSelectLabel::paintLastRange()
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
