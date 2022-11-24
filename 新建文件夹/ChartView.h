#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QMouseEvent>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QChart *chart, QWidget *parent = nullptr);

    ~ChartView();

//    void saveAxisRange();

signals:

public slots:

protected:
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void wheelEvent(QWheelEvent *event);

private:
    QPoint m_lastPoint;
    bool m_isPress = false;
    bool m_ctrlPress;
    bool m_alreadySaveRange;
    double m_xMin;
    double m_xMax;
    double m_yMin;
    double m_yMax;
    QGraphicsSimpleTextItem* m_coordItem = nullptr;
};

#endif // CHARTVIEW_H
