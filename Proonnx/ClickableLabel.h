#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    ClickableLabel(QWidget* parent = nullptr) : QLabel(parent) {

    }

signals:
    void clicked(int index); 

public:
    int m_index{ 0 }; 
    bool m_enbaleClicked{false};
protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            if (m_enbaleClicked) {
                emit clicked(m_index);
            }

        }
        QLabel::mousePressEvent(event); 
    }
};