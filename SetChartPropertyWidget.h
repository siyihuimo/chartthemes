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

public slots:

private:
    QSpinBox *m_pfontSizeSpinBox = nullptr;
    QComboBox *m_pfontComboBox = nullptr;
};

#endif // SETCHARTPROPERTYWIDGET_H
