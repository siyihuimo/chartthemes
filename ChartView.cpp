#include "ChartView.h"
#include <QApplication>
#include <QDebug>
#include <QCursor>

ChartView::ChartView(QWidget *parent)
{

}

ChartView::~ChartView()
{

}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    //鼠标点击空白处，取消线条选中状态
    emit mousePressChart();

    if(event->buttons() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        b_isLeftPress = true;
        if(! m_pRectItem)
        {
            m_pRectItem = new QGraphicsRectItem(this->chart());
        }
        m_pRectItem->setZValue(10);
        m_pRectItem->setRect(QRect(m_lastPoint, m_lastPoint));
        m_pRectItem->show();
    }

    else if (event->button() == Qt::RightButton)
    {
        m_OffsetPoint = event->globalPos();
        m_bHoldMove = true;
        QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor); // 使鼠标指针暂时改变形状
    }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::RightButton )&& m_bHoldMove)
    {
        QPoint temp;
        QPoint movePoint = event->globalPos();
        temp = movePoint - m_OffsetPoint;
        chart()->scroll(-temp.x(),temp.y());
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);           //清除拖影
        m_OffsetPoint = event->globalPos();
    }
    else if((event->buttons() & Qt::LeftButton )&& b_isLeftPress)
    {
        const QPoint curPos = event->pos();
        setCursor(QCursor(Qt::CrossCursor));

        QRectF choseRect = QRect(qMin(curPos.x(), m_lastPoint.x()),
                                 qMin(curPos.y(), m_lastPoint.y()),
                                 qAbs(m_lastPoint.x() - curPos.x()),
                                 qAbs(m_lastPoint.y() - curPos.y()));

        m_pRectItem->setRect(choseRect);
    }
    QChartView::mouseMoveEvent(event);
    update();
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bHoldMove = false;
    b_isLeftPress = false;

    setCursor(QCursor(Qt::ArrowCursor));
    if (m_pRectItem)
    {
        if( m_pRectItem->rect().width() > 5 &&
                m_pRectItem->rect().height() > 5 )
        {

            float f_left = this->chart()->mapToValue(m_pRectItem->rect().topLeft()).x();
            float f_right = this->chart()->mapToValue(m_pRectItem->rect().bottomRight()).x();
            float f_top = this->chart()->mapToValue(m_pRectItem->rect().topLeft()).y();
            float f_bottom = this->chart()->mapToValue(m_pRectItem->rect().bottomRight()).y();
            chart()->axisX()->setRange(f_left, f_right);
            chart()->axisY()->setRange(f_bottom, f_top);
        }
        delete m_pRectItem;
        m_pRectItem = nullptr;
    }

    QApplication::restoreOverrideCursor();
    QChartView::mouseReleaseEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    qreal rVal = 0;
    if(event->delta() > 0)
    {
        rVal = 0.8;
    }

    else
    {
        rVal = 1.2;
    }

     QRectF oPlotAreaRect = chart()->plotArea();
     QPointF oCenterPoint = oPlotAreaRect.center();
     oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
     oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
     QPointF oNewCenterPoint(2 * oCenterPoint - event->pos() - (oCenterPoint - event->pos()) / rVal);
     oPlotAreaRect.moveCenter(oNewCenterPoint);
     this->chart()->zoomIn(oPlotAreaRect);
     QChartView::wheelEvent(event);
}
