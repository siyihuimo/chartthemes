﻿/****************************************************************************
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

#include <QDebug>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(20),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    QGridLayout *baseLayout = new QGridLayout();

    m_pLineChart = new QChart();
    m_vecLineSeries.clear();

    m_pChartView = new ChartView();
    m_pChartView->setChart(m_pLineChart);
    m_pChartView->setRenderHint(QPainter::Antialiasing);       //抗锯齿
    baseLayout->addWidget(m_pChartView, 0, 0);



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

    QString name(u8"商品 ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable)
    {

        QScatterSeries *series1 = new QScatterSeries();
        series1->setMarkerShape(QScatterSeries::MarkerShapeCircle);//圆形的点
        series1->setBorderColor(QColor(21, 100, 255)); //边框颜色
        series1->setBrush(QBrush(QColor(21, 100, 255)));//背景颜色
        series1->setMarkerSize(12);           //点大小

        QLineSeries *series = new QLineSeries(m_pLineChart);

        m_pLineChart->addSeries(series1);
        m_pLineChart->addSeries(series);
        series1->clear();


        m_vecLineSeries.push_back(series);

        for (const Data &data : list)
        {
            series->append(data.first);
            series1->append(data.first.x(),data.first.y());
        }
        series->setName(name + QString::number(nameIndex));
        nameIndex++;


    }

    QValueAxis* axisX = new QValueAxis;
    axisX->setRange(0,10);

    QValueAxis* axisY = new QValueAxis;
    axisY->setRange(0, 10);


    for(QLineSeries* tmpSeries : m_vecLineSeries)
    {
        m_pLineChart->setAxisX(axisX,tmpSeries);
        m_pLineChart->setAxisY(axisY,tmpSeries);
    }

    m_pLineChart->legend()->setAlignment(Qt::AlignBottom);          //设置标题位置
}