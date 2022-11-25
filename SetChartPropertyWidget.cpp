#include "SetChartPropertyWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpacerItem>

SetChartPropertyWidget::SetChartPropertyWidget(QWidget *parent) : QWidget(parent)
{
    QTableWidget *pTabWidget = new QTableWidget(this);
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(pTabWidget);
    setLayout(verticalLayout);

    QVBoxLayout *tabLayout = new QVBoxLayout(pTabWidget);

    QGroupBox *gridPropertyGroup = new QGroupBox(u8"Show Gride",this);
    QHBoxLayout *gridHLayout = new QHBoxLayout(gridPropertyGroup);
    QRadioButton *yesRadioBtn = new QRadioButton(u8"yes",gridPropertyGroup);
    connect(yesRadioBtn,&QRadioButton::clicked,[=]{emit setGrideStatus(true);});
    QRadioButton *noRadioBtn = new QRadioButton(u8"no",gridPropertyGroup);
    connect(noRadioBtn,&QRadioButton::clicked,[=]{emit setGrideStatus(false);});
    gridHLayout->addWidget(yesRadioBtn);
    gridHLayout->addWidget(noRadioBtn);
    yesRadioBtn->setChecked(true);

    QGroupBox *unitPropertyGroup = new QGroupBox(u8"Unit Length",this);
    QHBoxLayout *unitHLayout = new QHBoxLayout(unitPropertyGroup);
    QLabel *unitLabel = new QLabel(u8"unit setting: ",unitPropertyGroup);
    QDoubleSpinBox *unitSpinbox = new QDoubleSpinBox(unitPropertyGroup);
    unitHLayout->addWidget(unitLabel);
    unitHLayout->addWidget(unitSpinbox);

    QGroupBox *fontPropertyGroup = new QGroupBox(u8"Font Setting",this);
    QVBoxLayout *fontVerticalLayout = new QVBoxLayout(fontPropertyGroup);

    QHBoxLayout *fontLayout = new QHBoxLayout(fontPropertyGroup);
    QLabel *fontLabel = new QLabel(u8"font: ",fontPropertyGroup);
    QComboBox *fontComboBox = new QComboBox(fontPropertyGroup);
    fontLayout->addWidget(fontLabel);
    fontLayout->addWidget(fontComboBox);

    QHBoxLayout *fontSizeLayout = new QHBoxLayout(fontPropertyGroup);
    QLabel *fontSizeLabel = new QLabel(u8"font size: ",fontPropertyGroup);
    QDoubleSpinBox *fontSizeSpinBox = new QDoubleSpinBox(fontPropertyGroup);
    fontSizeLayout->addWidget(fontSizeLabel);
    fontSizeLayout->addWidget(fontSizeSpinBox);

    QHBoxLayout *fontStyleLayout = new QHBoxLayout(fontPropertyGroup);
    QCheckBox *fontItalicsCheck = new QCheckBox(u8"italics",fontPropertyGroup);
    QCheckBox *fontBoldCheck = new QCheckBox(u8"bold",fontPropertyGroup);
    fontStyleLayout->addWidget(fontItalicsCheck);
    fontStyleLayout->addWidget(fontBoldCheck);

    QSpacerItem *spacerItem = new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed);

    fontVerticalLayout->addLayout(fontLayout);
    fontVerticalLayout->addLayout(fontSizeLayout);
    fontVerticalLayout->addLayout(fontStyleLayout);

    tabLayout->addWidget(gridPropertyGroup);
    tabLayout->addWidget(unitPropertyGroup);
    tabLayout->addWidget(fontPropertyGroup);
    tabLayout->addSpacerItem(spacerItem);
}
