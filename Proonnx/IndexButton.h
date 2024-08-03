#pragma once
#include <QPushButton>
#include <QMouseEvent>
#include <QWidget>

class IndexButton : public QPushButton {
    Q_OBJECT

public:
    IndexButton( QWidget* parent = nullptr)
        : QPushButton(parent) {
        // ��ʼ������
    }

signals:
    void clickedWithIndex(int index); // ������������ĵ���ź�

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit clickedWithIndex(m_index); // ���ʹ��������ĵ���ź�
        }
        QPushButton::mousePressEvent(event); // ���û�����¼�����
    }

public:
    int m_index; // �洢���
};