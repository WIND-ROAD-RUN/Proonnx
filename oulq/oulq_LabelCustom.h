#ifndef OULQ_CLICKEDLABEL_H_
#define OULQ_CLICKEDLABEL_H_

#include"oulq_global.h"

#include <QLabel>
#include <QWidget>
#include <QPainter>
#include<QPoint>

class QMouseEvent;

namespace rw {
    namespace oulq {

        class LabelClickable
            : public QLabel {
            Q_OBJECT

        public:
            LabelClickable(QWidget* parent = nullptr);

        signals:
            void clicked(int index);

        public:
            int m_index{ 0 };

            bool m_enbaleClicked{ false };

        protected:
            void mousePressEvent(QMouseEvent* event) override;

        };

        class LabelFrameSelectable 
            : public QLabel {
            Q_OBJECT

        public:
            explicit LabelFrameSelectable(QWidget* parent = nullptr);

            void enableSelection(bool enable);

            void setLastSelectionRect(const QRect& rect);

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

    }

}


#endif // !OULQ_CLICKEDLABEL_H_
