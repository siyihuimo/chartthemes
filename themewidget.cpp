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
#include <QtWidgets/QGridLayout>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>

#include <QApplication>
#include <QScatterSeries>

#include "SetChartPropertyWidget.h"

#include <QDebug>
#include <QTabWidget>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(20),
    m_markerShapeRectangle{QScatterSeries::MarkerShapeCircle,QScatterSeries::MarkerShapeRectangle},
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    QGridLayout *baseLayout = new QGridLayout();

    m_pLineChart = new QChart();
    m_vecLineSeries.clear();

    m_pChartView = new ChartView();
    m_pChartView->setChart(m_pLineChart);
    m_pChartView->setRenderHint(QPainter::Antialiasing);       //抗锯齿
    baseLayout->addWidget(m_pChartView, 0, 0);


    SetChartPropertyWidget * tabWidget = new SetChartPropertyWidget();
    tabWidget->setDefaultFontFamily(m_pLineChart->legend()->font().family());
    tabWidget->setDefaultFontSize(m_pLineChart->legend()->font().pointSizeF());

    connect(tabWidget,&SetChartPropertyWidget::setGrideStatus,this,&ThemeWidget::setGrideState);

    connect(tabWidget,&SetChartPropertyWidget::setFontFamily,this,&ThemeWidget::setFontFamily);
    connect(tabWidget,&SetChartPropertyWidget::setFontSize,this,&ThemeWidget::setFontSize);
    connect(tabWidget,&SetChartPropertyWidget::setFontItalics,this,&ThemeWidget::setItalicsFont);
    connect(tabWidget,&SetChartPropertyWidget::setFontBold,this,&ThemeWidget::setBoldFont);

    baseLayout->addWidget(tabWidget, 0, 1);

    setLayout(baseLayout);

    initLineChart();
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


void ThemeWidget::initLineChart()
{    
    m_pLineChart->setTitle(u8"折线图");


    m_valueLabel = new QLabel(this);
    m_valueLabel->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                                        " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
    m_valueLabel->setFixedSize(44, 24);
    m_valueLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_valueLabel->hide();

    QString name(u8"商品 ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable)
    {
        QScatterSeries *series1 = new QScatterSeries(m_pLineChart);             //散点
        connect(series1, &QScatterSeries::hovered, this, &ThemeWidget::showPointHoverd);
        series1->setMarkerShape(m_markerShapeRectangle.at(nameIndex%2));
        series1->setMarkerSize(12);

        QLineSeries *series = new QLineSeries(m_pLineChart);                //折线

        m_pLineChart->addSeries(series1);
        m_pLineChart->addSeries(series);

        m_vecLineSeries.push_back(series);
        m_vecLineSeries.push_back(series1);

        for (const Data &data : list)
        {
            series->append(data.first);
            series1->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
    }

    QValueAxis* axisX = new QValueAxis;
    axisX->setMinorTickCount(9);        //设置刻度数目
    axisX->setRange(0,10);

    QValueAxis* axisY = new QValueAxis;
    axisY->setRange(0, 5);


    for(QXYSeries* tmpSeries : m_vecLineSeries)
    {
        m_pLineChart->setAxisX(axisX,tmpSeries);
        m_pLineChart->setAxisY(axisY,tmpSeries);
    }

    m_pLineChart->legend()->setAlignment(Qt::AlignBottom);          //设置标题位置
}

void ThemeWidget::showPointHoverd(const QPointF &point, bool state)
{
    if (state) {
       m_valueLabel->setText(QString::asprintf("%1.0f%", point.y()));

       QPoint curPos = mapFromGlobal(QCursor::pos());
       m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);//移动数值

      // m_valueLabel->show();//显示出来
     }
     else
        m_valueLabel->hide();//进行隐藏
}

void ThemeWidget::setGrideState(bool state)
{
    m_pLineChart->axisX()->setGridLineVisible(state);
    m_pLineChart->axisY()->setGridLineVisible(state);
}

void ThemeWidget::setFontFamily(QString fontStr)
{
    QFont font = m_pLineChart->legend()->font();
    font.setFamily(fontStr);
    m_pLineChart->legend()->setFont(font);
}

void ThemeWidget::setFontSize(int size)
{
    QFont font = m_pLineChart->legend()->font();
    font.setPointSizeF(size);
    m_pLineChart->legend()->setFont(font);
}

void ThemeWidget::setItalicsFont(int state)
{
    QFont font = m_pLineChart->legend()->font();

    if(Qt::Unchecked == state)
    {
        font.setItalic(false);
    }
    else if(Qt::Checked == state)
    {
        font.setItalic(true);
    }
    m_pLineChart->legend()->setFont(font);
}

void ThemeWidget::setBoldFont(int state)
{
    QFont font = m_pLineChart->legend()->font();
    if(Qt::Unchecked == state)
    {
        font.setBold(false);
    }
    else if(Qt::Checked == state)
    {
        font.setBold(true);
    }
    m_pLineChart->legend()->setFont(font);
}
