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
#include <QPushButton>
#include <QColorDialog>

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
    QVBoxLayout *unitVeritalLayout = new QVBoxLayout(unitPropertyGroup);

    QHBoxLayout *unitXHLayout = new QHBoxLayout(unitPropertyGroup);
    QLabel *unitXLabel = new QLabel(u8"x axis unit setting: ",unitPropertyGroup);
    QSpinBox *unitXSpinbox = new QSpinBox(unitPropertyGroup);
    unitXSpinbox->setValue(5);
    connect(unitXSpinbox,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){
        emit setXMinorTickCount(value);});
    unitXHLayout->addWidget(unitXLabel);
    unitXHLayout->addWidget(unitXSpinbox);

    QHBoxLayout *unitYHLayout = new QHBoxLayout(unitPropertyGroup);
    QLabel *unitYLabel = new QLabel(u8"y axis unit setting: ",unitPropertyGroup);
    QSpinBox *unitYSpinbox = new QSpinBox(unitPropertyGroup);
    unitYSpinbox->setValue(5);
    connect(unitYSpinbox,static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){
        emit setYMinorTickCount(value);});
    unitYHLayout->addWidget(unitYLabel);
    unitYHLayout->addWidget(unitYSpinbox);

    unitVeritalLayout->addLayout(unitXHLayout);
    unitVeritalLayout->addLayout(unitYHLayout);

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


    QGroupBox* linePropertyGroup = new QGroupBox(this);

    QVBoxLayout *linePropertyLayout = new QVBoxLayout(linePropertyGroup);
    QHBoxLayout *lineColorLayout = new QHBoxLayout(linePropertyGroup);
    QLabel *lineColorSettingLabel = new QLabel(u8"line color: ",linePropertyGroup);
    QPushButton *lineColorBtn = new QPushButton(u8"",linePropertyGroup);
    lineColorBtn->setStyleSheet("QPushButton{background-color:rgb(255,255,0)};");
    connect(lineColorBtn, &QPushButton::clicked,[=]{
        QColor color = QColorDialog::getColor(QColor(255,0,0));
        QString tmpColor = QString("QPushButton{background-color:rgb(%1,%2,%3)};").arg(color.red()).arg(color.green()).arg(color.blue());
        lineColorBtn->setStyleSheet(tmpColor);
        emit setLineColor(color);});
    lineColorLayout->addWidget(lineColorSettingLabel);
    lineColorLayout->addWidget(lineColorBtn);

    QHBoxLayout *lineWidthLayout = new QHBoxLayout(linePropertyGroup);
    QLabel *lineWidthLabel = new QLabel(u8"line width: ",linePropertyGroup);
    QSpinBox *lineWidthSpinBox = new QSpinBox(linePropertyGroup);
    connect(lineWidthSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){emit setLineWidth(value);});

    lineWidthLayout->addWidget(lineWidthLabel);
    lineWidthLayout->addWidget(lineWidthSpinBox);
    QCheckBox *lineDisplayLabelCheck = new QCheckBox(u8"display label",fontPropertyGroup);
    lineDisplayLabelCheck->setChecked(false);
    connect(lineDisplayLabelCheck,&QCheckBox::stateChanged,[=](int state){emit setDisplayLabelState(state);});
    linePropertyLayout->addLayout(lineColorLayout);
    linePropertyLayout->addLayout(lineWidthLayout);
    linePropertyLayout->addWidget(lineDisplayLabelCheck);


    QHBoxLayout *legendLayout = new QHBoxLayout(this);
    QLabel *legendLabel = new QLabel(u8"legend position: ",this);
    QComboBox *legendPosBox = new QComboBox(this);
    connect(legendPosBox,&QComboBox::currentTextChanged,[=](const QString &text){emit setlegendPosition(text);});
    QStringList legendPositionlist;
    legendPositionlist<<u8"top"<<u8"bottom"<<u8"left"<<u8"right";
    legendPosBox->addItems(legendPositionlist);
    legendPosBox->setCurrentText(u8"bottom");
    legendLayout->addWidget(legendLabel);
    legendLayout->addWidget(legendPosBox);



    QSpacerItem *spacerItem = new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed);

    fontVerticalLayout->addLayout(fontLayout);
    fontVerticalLayout->addLayout(fontSizeLayout);
    fontVerticalLayout->addLayout(fontStyleLayout);

    tabLayout->addWidget(gridPropertyGroup);
    tabLayout->addWidget(unitPropertyGroup);
    tabLayout->addWidget(fontPropertyGroup);
    tabLayout->addWidget(linePropertyGroup);
    tabLayout->addLayout(legendLayout);
    tabLayout->addSpacerItem(spacerItem);


    setMaximumWidth(240);
}

void SetChartPropertyWidget::setDefaultFontFamily(QString font)
{
    m_pfontComboBox->setCurrentText(font);
}

void SetChartPropertyWidget::setDefaultFontSize(int size)
{
    m_pfontSizeSpinBox->setValue(size);
}
