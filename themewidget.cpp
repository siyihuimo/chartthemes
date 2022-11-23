/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "themewidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>

#include <QApplication>

#include <QDebug>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(20),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    // create layout
    QGridLayout *baseLayout = new QGridLayout();

    m_pLineChart = new QChart();

    m_pChartView = new QChartView(m_pLineChart);
    m_pChartView->setRenderHint(QPainter::Antialiasing);       //抗锯齿
    baseLayout->addWidget(m_pChartView, 0, 0);

    setLayout(baseLayout);

    initLineChart();
    setMouseTracking(true);
    m_pChartView->setMouseTracking(true);
}

ThemeWidget::~ThemeWidget()
{
}



DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    for (int i = 0; i < listCount; i++)
    {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++)
        {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}


void ThemeWidget::initLineChart() const
{    
    m_pLineChart->setTitle(u8"折线图");

    QString name(u8"商品 ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(m_pLineChart);
        for (const Data &data : list)
        {
            series->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        m_pLineChart->addSeries(series);
    }
    m_pLineChart->createDefaultAxes();
}

void ThemeWidget::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        m_pLineChart->zoomIn();
    }

    else
    {
        m_pLineChart->zoomOut();
    }
}

void ThemeWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mousePressEvent ...";
    qDebug()<<"event->buttons() = "<<event->buttons();
    if(event->buttons() == Qt::LeftButton)
    {
        m_bHoldMove = true;
        //记录位置
        /*QCursor cursor;
        cursor.setShape(Qt::ClosedHandCursor);
        QApplication::setOverrideCursor(cursor); */// 使鼠标指针暂时改变形状
        m_OffsetPoint = event->globalPos()-pos();
        qDebug()<<"m_OffsetPoint = "<<m_OffsetPoint;
    }
}

void ThemeWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"mouseMoveEvent .....";
    qDebug()<<"event->buttons() = "<<event->buttons();
    if((event->buttons() & Qt::LeftButton )&& m_bHoldMove)
    {
        QPoint temp;
        temp = event->globalPos() - m_OffsetPoint;
        qDebug()<<"temp = "<<temp;
        m_pChartView->move(temp);
    }
}

void ThemeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<"mouseReleaseEvent .....";

    Q_UNUSED(event);
    m_bHoldMove = false;
    QApplication::restoreOverrideCursor();
}

void ThemeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug()<<"dragMoveEvent .....";
}

bool ThemeWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_pChartView)
    {
        if (e->type() == QEvent::MouseButtonPress){
           qDebug()<<"1111......";
        }
        else if(e->type() == QEvent::MouseButtonRelease)
        {
            qDebug()<<"22222......";
        }
        else if(e->type() == QEvent::MouseMove)
        {
            qDebug()<<"121212 ....";
        }
    }
    return QWidget::eventFilter(obj,e);

}

