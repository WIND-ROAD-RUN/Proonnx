#pragma once
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>

class IndexButton : public QPushButton {
    Q_OBJECT

public:
    IndexButton( QWidget* parent = nullptr)
        : QPushButton(parent) {
        // 初始化代码
    }

signals:
    void clickedWithIndex(int index); // 定义带有索引的点击信号

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit clickedWithIndex(m_index); // 发送带有索引的点击信号
        }
        QPushButton::mousePressEvent(event); // 调用基类的事件处理
    }

public:
    int m_index; // 存储序号
};