#include "ChartView.h"

ChartView::ChartView(QChart *chart, QChartView *parent)
{
    //
}

ChartView::~ChartView()
{
    //
}

void ChartView::saveAxisRange()
{
    //
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        m_isPress = true;
    }
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_coordItem)
    {
        m_coordItem = new QGraphicsSimpleTextItem(this->chart());
        m_coordItem->setZValue(5);
        m_coordItem->setPos(100, 60);
        m_coordItem->show();
    }
    const QPoint curPos = event->pos();
    QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
    QString coordStr = QString("X = %1, Y = %2").arg(curVal.x()).arg(curVal.y());
    m_coordItem->setText(coordStr);

    if (m_isPress)
    {
        QPoint offset = curPos - m_lastPoint;
        m_lastPoint = curPos;
        if (!m_alreadySaveRange)
        {
            this->saveAxisRange();
            m_alreadySaveRange = true;
        }
        this->chart()->scroll(-offset.x(), offset.y());
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
        if (event->button() == Qt::RightButton)
        {
            if (m_alreadySaveRange)
            {
                this->chart()->axisX()->setRange(m_xMin, m_xMax);
                this->chart()->axisY()->setRange(m_yMin, m_yMax);
            }
        }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    //
}
