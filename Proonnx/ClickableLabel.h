#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ClickableLabel(QWidget* parent = nullptr) : QLabel(parent) {
        // ��ʼ������
    }

signals:
    void clicked(int index); // �������źţ���һ�� int ����

public:
    int m_index{ 0 }; // �洢����ֵ
    int m_enbaleClicked{false};
protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (m_enbaleClicked) {
                emit clicked(m_index);
            }
             // ���͵���źţ����� m_index
        }
        QLabel::mousePressEvent(event); // ���û�����¼�����
    }
};