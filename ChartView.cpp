#include "ChartView.h"
#include <QApplication>
#include <QDebug>

ChartView::ChartView(QWidget *parent)
{

}

ChartView::~ChartView()
{

}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    QChartView::mousePressEvent(event);

    if(event->buttons() == Qt::LeftButton)
    {
        m_bHoldMove = true;
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor); // 使鼠标指针暂时改变形状
        m_OffsetPoint = event->globalPos();
    }
    update();
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton )&& m_bHoldMove)
    {
        QPoint temp;
        QPoint movePoint = event->globalPos();
        temp = movePoint - m_OffsetPoint;
        chart()->scroll(-temp.x(),temp.y());
        m_OffsetPoint = event->globalPos();
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bHoldMove = false;
    QApplication::restoreOverrideCursor();
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        chart()->zoomIn();
    }

    else
    {
        chart()->zoomOut();
    }
}
