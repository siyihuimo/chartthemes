#ifndef SETCHARTPROPERTYWIDGET_H
#define SETCHARTPROPERTYWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QButtonGroup>
#include <QSpinBox>
#include <QComboBox>

class SetChartPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SetChartPropertyWidget(QWidget *parent = nullptr);

    void setDefaultFontFamily(QString);
    void setDefaultFontSize(int);

signals:
    void setGrideStatus(bool);
    void setFontFamily(QString);
    void setFontSize(int);
    void setFontItalics(int);
    void setFontBold(int);
    void setLineColor(QColor);
    void setXMinorTickCount(int);
    void setYMinorTickCount(int);
    void setDisplayLabelState(int);
    void setLineWidth(int);
    void setlegendPosition(QString);
    void legendTextChange(QString);
    void setSeriesTransparent(int);
    void setLegendHideState(int);

public slots:

public slots:
    void setLengendTitle(QString);
    void setTransparent(qreal);

private:
    QSpinBox *m_pfontSizeSpinBox = nullptr;
    QComboBox *m_pfontComboBox = nullptr;

    QLineEdit *m_pLegendTitleEdit = nullptr;

    QSlider *m_pSeriesTransparentSlider = nullptr;
};

#endif // SETCHARTPROPERTYWIDGET_H
