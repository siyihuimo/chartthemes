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
#include <QCategoryAxis>

#include <QApplication>
#include <QScatterSeries>

#include "SetChartPropertyWidget.h"

#include <QDebug>
#include <QTabWidget>
#include <QColorDialog>
#include <QLegendMarker>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(20),
    m_markerShapeRectangle{QScatterSeries::MarkerShapeCircle,QScatterSeries::MarkerShapeRectangle},
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount))
{
    QGridLayout *baseLayout = new QGridLayout();

    m_pLineChart = new QChart();
    m_vecLineSeries.clear();

    m_pChartView = new ChartView();
    connect(m_pChartView,&ChartView::mousePressChart,this,&ThemeWidget::recoverLineStatus);
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
    connect(tabWidget,&SetChartPropertyWidget::setLineColor,this,&ThemeWidget::setCustomColor);
    connect(tabWidget,&SetChartPropertyWidget::setXMinorTickCount,this,&ThemeWidget::setXAxisaTickCount);
    connect(tabWidget,&SetChartPropertyWidget::setYMinorTickCount,this,&ThemeWidget::setYAxisaTickCount);
    connect(tabWidget,&SetChartPropertyWidget::setDisplayLabelState,this,&ThemeWidget::setLabelDisplayState);
    connect(tabWidget,&SetChartPropertyWidget::setLineWidth,this,&ThemeWidget::setCurrentLineWidth);
    connect(tabWidget,&SetChartPropertyWidget::setlegendPosition,this,&ThemeWidget::changeLegendPosition);
    connect(tabWidget,&SetChartPropertyWidget::legendTextChange,this,&ThemeWidget::setLegendTitle);
    connect(tabWidget,&SetChartPropertyWidget::setSeriesTransparent,this,&ThemeWidget::serCurrentSeriesTrans);
    connect(tabWidget,&SetChartPropertyWidget::setLegendHideState,this,&ThemeWidget::setCurrentLegendHideState);
    connect(tabWidget,&SetChartPropertyWidget::setScatterIcon,this,&ThemeWidget::setCurrentScatterIcon);

    connect(this,&ThemeWidget::seriesClick,tabWidget,&SetChartPropertyWidget::setLengendTitle);
    connect(this,&ThemeWidget::currentSeriesTrans,tabWidget,&SetChartPropertyWidget::setTransparent);

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

    QString name(u8"point track ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable)
    {
        QScatterSeries *series1 = new QScatterSeries(m_pLineChart);             //散点
        connect(series1,&QScatterSeries::doubleClicked,[=](){qDebug()<<"double11111";});
        series1->setMarkerShape(m_markerShapeRectangle.at(nameIndex%2));
        int customIcoSize =10;
        series1->setMarkerSize(customIcoSize);
        QLineSeries *series = new QLineSeries(m_pLineChart);                //折线
        series->setObjectName(QString::number(nameIndex));

        m_LineMapScatter.insert(series,series1);

        connect(series,&QXYSeries::clicked,[=](){
            m_pCurrentSeries = series;
            setSeriesSelectState(QPointF(0,0));
            auto itLegend = m_LegendMapSeries.begin();
            while(itLegend != m_LegendMapSeries.end())
            {
                if(itLegend.value() == series)
                {
                    m_pCurrentEditLegend = itLegend.key();
                    setLegendSelectState();         //关联图例选中状态
                    break;
                }
                ++itLegend;
            }
            emit seriesClick(series->name());
        });

        m_pLineChart->addSeries(series1);
        m_pLineChart->addSeries(series);
        series1->setColor(series->color());

        m_vecLineSeries.push_back(series);
        m_vecLineSeries.push_back(series1);

        for (const Data &data : list)
        {
            series->append(data.first);
            series1->append(data.first);
        }
        series->setName(name + QString::number(nameIndex));

        QLegend *pChartLegend1 = m_pLineChart->legend();
        if(pChartLegend1)
        {
            QList<QLegendMarker *> plegendList = pChartLegend1->markers();
            for(QLegendMarker *tmpLegend : plegendList)
            {
                if(tmpLegend)
                {
                    if(tmpLegend->label().contains(series->name()))
                    {
                        connect(tmpLegend,&QLegendMarker::clicked,[=](){
                            m_pCurrentEditLegend = tmpLegend;
                            setLegendSelectState();
                            emit seriesClick(tmpLegend->label());
                            m_pLastEditLegend->setShape(QLegend::MarkerShapeCircle);
                        });
                        m_LegendMapSeries.insert(tmpLegend,series);
                    }
                    else if(tmpLegend->label().isEmpty())
                    {
                        tmpLegend->setVisible(false);
                    }
                }
            }
        }

        nameIndex++;
    }

    QCategoryAxis * axisX = new QCategoryAxis;
    axisX->setMinorTickCount(5);        //设置刻度数目
    axisX->setRange(0,10);
    axisX->append("no1",1);
    axisX->append("no2",2);
    axisX->append("no3",3);
    axisX->append("no4",4);
    axisX->append("no5",5);
    axisX->append("no6",6);
    axisX->append("no7",7);
    axisX->append("no8",8);
    axisX->append("no9",9);
    axisX->append("no10",10);
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->setTitleText(u8"no");

    QCategoryAxis* axisY = new QCategoryAxis;
    axisY->setMinorTickCount(5);
    axisY->setRange(0, 5);
    axisY->append("apple",1);
    axisY->append("banana",2);
    axisY->append("orange",3);
    axisY->append("tomato",4);
    axisY->append("carrot",5);
    axisY->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisY->setTitleText(u8"fruit");


    QLegend *pChartLegend = m_pLineChart->legend();
    if(pChartLegend)
    {
        pChartLegend->setAlignment(Qt::AlignBottom);          //设置标题位置
    }

    for(QXYSeries* tmpSeries : m_vecLineSeries)
    {
        m_pLineChart->setAxisX(axisX,tmpSeries);
        m_pLineChart->setAxisY(axisY,tmpSeries);
    }



}



void ThemeWidget::setLegendSelectState()
{
    if(m_pCurrentEditLegend)
    {
        QFont font = m_pCurrentEditLegend->font();     //默认字体大小为8

        //恢复上一次修改的设置
        if(m_pLastEditLegend)
        {
            font.setPointSizeF(8);
            font.setBold(false);
            m_pLastEditLegend->setFont(font);
        }

        //设置本次选中的状态
        font.setPointSizeF(16);
        font.setBold(true);
        m_pCurrentEditLegend->setFont(font);

        auto itSeries = m_LegendMapSeries.find(m_pCurrentEditLegend);
        if(itSeries != m_LegendMapSeries.end())
        {
            m_pCurrentSeries = itSeries.value();
            setSeriesSelectState(QPointF(0,0));         //关联折线选中状态
        }
        m_pLastEditLegend = m_pCurrentEditLegend;
    }

}

void ThemeWidget::setLegendTitle(QString title)
{
    if(!m_pCurrentSeries || !m_pCurrentEditLegend)
    {
        return;
    }
    m_pCurrentSeries->setName(title);
    m_pCurrentEditLegend->setLabel(title);
}

void ThemeWidget::serCurrentSeriesTrans(int transparent)
{
    if(m_pCurrentSeries)
    {
        double transValue = transparent / 100.0;
        m_pCurrentSeries->setOpacity(transValue);

        auto iterator = m_LineMapScatter.find(m_pCurrentSeries);
        if(iterator != m_LineMapScatter.end())
        {
            iterator.value()->setOpacity(transValue);
        }
    }
}

void ThemeWidget::setCurrentLegendHideState(int state)
{
    if(m_pLineChart && m_pLineChart->legend())
    {
        bool hideLegend = false;
        switch (state) {
        case Qt::Unchecked:
            hideLegend = true;
            break;
        case Qt::Checked:
            hideLegend = false;
            break;
        default:
            break;
        }
        m_pLineChart->legend()->setVisible(hideLegend);
    }
}

void ThemeWidget::setCurrentScatterIcon(QString iconTag)
{
    qDebug()<<"iconTag = "<<iconTag;

    if(m_pCurrentSeries)
    {
        auto iterator = m_LineMapScatter.find(m_pCurrentSeries);
        if(iterator != m_LineMapScatter.end())
        {
            int customIcoSize = 10;
            m_pCurrentSeries->setPointsVisible(false);
            QImage customIco(customIcoSize, customIcoSize, QImage::Format_ARGB32);
            customIco.fill(Qt::transparent);
            QPixmap tmpPix;
            if("icon1" == iconTag)
            {
                tmpPix.load(":/img/res/1.svg");
            }
            else if("icon2" == iconTag)
            {
                tmpPix.load(":/img/res/2.svg");
            }
            else if("icon3" == iconTag)
            {
                tmpPix.load(":/img/res/3.svg");
            }

            tmpPix = tmpPix.scaled(customIcoSize,customIcoSize);
            QPainter painter(&customIco);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(QRgb(0xf6a625));
            painter.setBrush(painter.pen().color());
            painter.drawPixmap(0,0,tmpPix);
            iterator.value()->setBrush(customIco);
            iterator.value()->setPen(QColor(Qt::transparent));
        }
    }
}


void ThemeWidget::setSeriesSelectState(const QPointF &point)
{
    Q_UNUSED(point) ;
    if(m_pCurrentSeries)
    {
        emit currentSeriesTrans(m_pCurrentSeries->opacity());
        const QPen oldPen = m_pCurrentSeries->pen();
        QPen pen(oldPen);
        pen.setWidth(3);
        pen.setStyle(Qt::DotLine);
        m_pCurrentSeries->setPointsVisible(true);         //显示点图
        m_pCurrentSeries->setPen(pen);
        m_pCurrentSeries->setPointLabelsFormat("(@xPoint,@yPoint)");
        m_pCurrentSeries->setPointLabelsVisible(m_bShowPointLabel);
    }
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
    if(m_pCurrentEditLegend)
    {
        QFont font = m_pCurrentEditLegend->font();
        font.setFamily(fontStr);
        m_pCurrentEditLegend->setFont(font);
    }
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

void ThemeWidget::setCustomColor(QColor color)
{
    m_pLineChart->focusProxy();
    if(m_pCurrentSeries)
    {
        m_pCurrentSeries->setColor(color);
    }
}

void ThemeWidget::setXAxisaTickCount(int value)
{
    if(m_pLineChart)
    {
        QValueAxis *tmpXisX = qobject_cast<QValueAxis*>(m_pLineChart->axisX());
        if(tmpXisX)
            tmpXisX ->setMinorTickCount(value);
    }
}

void ThemeWidget::setYAxisaTickCount(int value)
{
    if(m_pLineChart)
    {
        QValueAxis *tmpXisY = qobject_cast<QValueAxis*>(m_pLineChart->axisY());
        if(tmpXisY)
            tmpXisY->setMinorTickCount(value);
    }
}

void ThemeWidget::setLabelDisplayState(int value)
{
    if(m_pCurrentSeries)
    {
        if(Qt::Unchecked == value)
        {
            m_bShowPointLabel = false;
        }
        else if(Qt::Checked == value)
        {
            m_bShowPointLabel = true;
        }
        m_pCurrentSeries->setPointLabelsVisible(m_bShowPointLabel);
    }
}

void ThemeWidget::setCurrentLineWidth(int lineWidth)
{
    if(m_pCurrentSeries)
    {
        const QPen oldPen = m_pCurrentSeries->pen();
        QPen pen(oldPen);
        pen.setWidth(lineWidth);
        m_pCurrentSeries->setPen(pen);
    }
}

void ThemeWidget::changeLegendPosition(QString strPos)
{
    if(m_pLineChart)
    {
        if("top" == strPos)
        {
            m_pLineChart->legend()->setAlignment(Qt::AlignTop);
        }
        else if("bottom" == strPos)
        {
            m_pLineChart->legend()->setAlignment(Qt::AlignBottom);
        }
        else if("left" == strPos)
        {
            m_pLineChart->legend()->setAlignment(Qt::AlignLeft);
        }
        else if("right" == strPos)
        {
            m_pLineChart->legend()->setAlignment(Qt::AlignRight);
        }
    }
}

void ThemeWidget::recoverLineStatus()
{
    for(QXYSeries* tmpSeries : m_vecLineSeries)
    {
        const QPen oldPen = tmpSeries->pen();
        QPen pen(oldPen);
        pen.setWidth(2);
        pen.setStyle(Qt::SolidLine);
        tmpSeries->setPointsVisible(false);         //显示点图
        tmpSeries->setPen(pen);
        tmpSeries->setPointLabelsVisible(false);
    }
}


