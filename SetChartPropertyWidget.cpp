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
#include <QDebug>

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
    m_pfontComboBox = new QComboBox(fontPropertyGroup);
    m_pfontComboBox->addItem("NSimSun");
    m_pfontComboBox->addItem("FangSong");
    m_pfontComboBox->addItem("SimSun");
    connect(m_pfontComboBox,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int index){emit setFontFamily(m_pfontComboBox->itemText(index));});
    fontLayout->addWidget(fontLabel);
    fontLayout->addWidget(m_pfontComboBox);

    QHBoxLayout *fontSizeLayout = new QHBoxLayout(fontPropertyGroup);
    QLabel *fontSizeLabel = new QLabel(u8"font size: ",fontPropertyGroup);
    m_pfontSizeSpinBox = new QSpinBox(fontPropertyGroup);
    connect(m_pfontSizeSpinBox,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){emit setFontSize(value);});
    fontSizeLayout->addWidget(fontSizeLabel);
    fontSizeLayout->addWidget(m_pfontSizeSpinBox);

    QHBoxLayout *fontStyleLayout = new QHBoxLayout(fontPropertyGroup);
    QCheckBox *fontItalicsCheck = new QCheckBox(u8"italics",fontPropertyGroup);
    connect(fontItalicsCheck,&QCheckBox::stateChanged,[=](int state){emit setFontItalics(state);});
    QCheckBox *fontBoldCheck = new QCheckBox(u8"bold",fontPropertyGroup);
    connect(fontBoldCheck,&QCheckBox::stateChanged,[=](int state){emit setFontBold(state);});
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

void SetChartPropertyWidget::setDefaultFontFamily(QString font)
{
    m_pfontComboBox->setCurrentText(font);
}

void SetChartPropertyWidget::setDefaultFontSize(int size)
{
    m_pfontSizeSpinBox->setValue(size);
}
