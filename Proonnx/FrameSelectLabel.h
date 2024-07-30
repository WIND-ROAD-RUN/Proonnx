#ifndef FRAMESELECTLABEL_H_
#define FRAMESELECTLABEL_H_

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include<QPoint>

class FrameSelectLabel : public QLabel {
    Q_OBJECT

public:
    explicit FrameSelectLabel(QWidget* parent = nullptr);

    void enableSelection(bool enable);

    void setLastSelectionRect(const QRect & rect);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

public:
    void paintLastRange();

private:
    bool isSelecting;

    bool selectionEnabled;

    QPoint startPoint;

    QPoint endPoint;

    QRect currentSelectionRect;

    QRect lastSelectionRect;

signals:
    void selectionMade(const QRect& rect);

};

#endif // FRAMESELECTLABEL_H_