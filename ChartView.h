#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QWidget>

#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QChart>
#include <QChartView>
QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QWidget *parent = nullptr);
    ~ChartView();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPoint m_OffsetPoint;
    bool m_bHoldMove;

    QGraphicsRectItem* m_pRectItem = nullptr;

    QPoint m_lastPoint;
    QPoint m_currentPoint;
    bool b_isRightPress = false;
    bool b_isLeftPress = false;
};

#endif // CHARTVIEW_H
