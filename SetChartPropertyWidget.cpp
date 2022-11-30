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
#include <QLineEdit>

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
    fontVerticalLayout->addLayout(fontLayout);
    fontVerticalLayout->addLayout(fontSizeLayout);
    fontVerticalLayout->addLayout(fontStyleLayout);

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



    QHBoxLayout *legendTitleLayout = new QHBoxLayout(this);
    QLabel *legendTitleLabel = new QLabel(u8"set legend title: ",this);
    m_pLegendTitleEdit = new QLineEdit(this);
    connect(m_pLegendTitleEdit,&QLineEdit::textChanged,[=](const QString &text){emit legendTextChange(text);});
    legendTitleLayout->addWidget(legendTitleLabel);
    legendTitleLayout->addWidget(m_pLegendTitleEdit);

    QVBoxLayout *seriesTransparentLayout = new QVBoxLayout(this);
    QLabel *seriesTransparentLabel = new QLabel(u8"set opacity of the polyline: ",this);
    m_pSeriesTransparentSlider = new QSlider(this);
    m_pSeriesTransparentSlider->setRange(0,100);
    m_pSeriesTransparentSlider->setSingleStep(1);
    m_pSeriesTransparentSlider->setValue(0);
    m_pSeriesTransparentSlider->setOrientation(Qt::Horizontal);
    m_pSeriesTransparentSlider->setTickPosition(QSlider::TicksBelow);
    connect(m_pSeriesTransparentSlider,&QSlider::valueChanged,[=](int value){emit setSeriesTransparent(value);});

    QCheckBox *hideLegendBox = new QCheckBox(u8"hide legend",this);
    connect(hideLegendBox,&QCheckBox::stateChanged,[=](int state){ emit setLegendHideState(state); });
    seriesTransparentLayout->addWidget(seriesTransparentLabel);
    seriesTransparentLayout->addWidget(m_pSeriesTransparentSlider);
    seriesTransparentLayout->addWidget(hideLegendBox);

    QHBoxLayout* lineCustomIcoLayout = new QHBoxLayout(this);
    QLabel *lineCustomIcoLabel = new QLabel(u8"set the polyline icon: ",this);
    QComboBox *lineCustomIcoBox = new QComboBox(this);
    connect(lineCustomIcoBox,&QComboBox::currentTextChanged,[=](const QString &text){emit setScatterIcon(text);});
    QStringList customIconList;
    customIconList<<u8"icon1"<<u8"icon2"<<u8"icon3";
    lineCustomIcoBox->addItems(customIconList);
    lineCustomIcoLayout->addWidget(lineCustomIcoLabel);
    lineCustomIcoLayout->addWidget(lineCustomIcoBox);


    //QSpacerItem *spacerItem = new QSpacerItem(0,160,QSizePolicy::Expanding,QSizePolicy::Fixed);

    tabLayout->addWidget(gridPropertyGroup);
    tabLayout->addWidget(unitPropertyGroup);
    tabLayout->addWidget(fontPropertyGroup);
    tabLayout->addWidget(linePropertyGroup);
    tabLayout->addLayout(legendLayout);
    tabLayout->addLayout(legendTitleLayout);
    tabLayout->addLayout(seriesTransparentLayout);
    tabLayout->addLayout(lineCustomIcoLayout);
 //   tabLayout->addSpacerItem(spacerItem);


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

void SetChartPropertyWidget::setLengendTitle(QString title)
{
    if(m_pLegendTitleEdit)
    {
        m_pLegendTitleEdit->setText(title);
    }
}

void SetChartPropertyWidget::setTransparent(qreal value)
{
    value*=100;
    m_pSeriesTransparentSlider->setValue(value);

}
