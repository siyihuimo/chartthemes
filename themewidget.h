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

#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QScatterSeries>
#include <QList>
#include <QLabel>

#include "ChartView.h"

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;

    void initLineChart();

signals:
    void seriesClick(QString);
    void currentSeriesTrans(qreal opacity);

private slots:
    void showPointHoverd(const QPointF &point, bool state);
    void setGrideState(bool);
    void setFontFamily(QString);
    void setFontSize(int);
    void setItalicsFont(int);
    void setBoldFont(int);
    void setCustomColor(QColor);

    void setXAxisaTickCount(int value);
    void setYAxisaTickCount(int value);
    void setLabelDisplayState(int);
    void setCurrentLineWidth(int);
    void changeLegendPosition(QString);

    void recoverLineStatus();
    void setSeriesSelectState(const QPointF &point);
    void setLegendSelectState();    

    void setLegendTitle(QString);
    void serCurrentSeriesTrans(int);
    void setCurrentLegendHideState(int);

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;

    DataTable m_dataTable;

    QChart *m_pLineChart = nullptr;
    ChartView *m_pChartView = nullptr;

    QVector<QXYSeries*> m_vecLineSeries;

    QLabel* m_valueLabel = nullptr;

    const QVector<QScatterSeries::MarkerShape>  m_markerShapeRectangle;

    QXYSeries* m_pCurrentSeries = nullptr;
    bool m_bShowPointLabel = false;

    QMap<QLegendMarker*,QXYSeries*> m_LegendMapSeries;
    QMap<QXYSeries*,QXYSeries*> m_LineMapScatter;

    QLegendMarker* m_pLastEditLegend=nullptr;
    QLegendMarker* m_pCurrentEditLegend = nullptr;
};

#endif /* THEMEWIDGET_H */
