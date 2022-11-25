#ifndef SETCHARTPROPERTYWIDGET_H
#define SETCHARTPROPERTYWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QButtonGroup>

class SetChartPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SetChartPropertyWidget(QWidget *parent = nullptr);

signals:
    void setGrideStatus(bool);

public slots:

private:
};

#endif // SETCHARTPROPERTYWIDGET_H
