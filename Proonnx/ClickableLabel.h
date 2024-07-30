#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ClickableLabel(QWidget* parent = nullptr) : QLabel(parent) {
        // 初始化代码
    }

signals:
    void clicked(int index); // 定义点击信号，带一个 int 参数

public:
    int m_index{ 0 }; // 存储索引值
    int m_enbaleClicked{false};
protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (m_enbaleClicked) {
                emit clicked(m_index);
            }
             // 发送点击信号，传递 m_index
        }
        QLabel::mousePressEvent(event); // 调用基类的事件处理
    }
};