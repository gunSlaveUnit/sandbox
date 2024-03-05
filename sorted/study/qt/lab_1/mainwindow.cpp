//
// Created by gunSlaveUnit on 01.03.2022.
//

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent){
    resize(275, 208);
    setWindowTitle(WINDOW_TITLE);
    setMinimumSize(275, 208);
    setMaximumSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    centralWidget = new QGraphicsView(this);
    setCentralWidget(centralWidget);

    centralLayout = new QGridLayout(centralWidget);

    font = new QFont();
    font->setPixelSize(40);

    text = new QLineEdit(tr("It's me, Mario!"), centralWidget);
    text->setGeometry(WINDOW_WIDTH / 2 - 200, 100, 400, 50);
    text->setAlignment(Qt::AlignCenter);
    text->setFont(*font);
    centralLayout->addWidget(text, 0, 1, 1, 2, Qt::AlignCenter);

    labelForegroundColor = new QColor(0, 0, 0);

    labelPalette = new QPalette();
    text->setPalette(*labelPalette);

    updateColor();

    plus = new QPushButton(tr("+"), centralWidget);
    plus->setFixedSize(50, 50);
    plus->setFont(*font);
    centralLayout->addWidget(plus, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    minus = new QPushButton(tr("-"), centralWidget);
    minus->setFixedSize(50, 50);
    minus->setFont(*font);
    centralLayout->addWidget(minus, 1, 1, Qt::AlignRight | Qt::AlignVCenter);

    colorComponents = new QGroupBox(this);
    colorComponents->setTitle("Color Components");
    //colorComponents->setGeometry(320, 170, 150, 100);
    //colorComponents->setAlignment(Qt::AlignCenter);
    centralLayout->addWidget(colorComponents, 1, 2, 2, 1, Qt::AlignCenter);

    cp1 = new QRadioButton(tr("R"));
    cp2 = new QRadioButton(tr("G"));
    cp3 = new QRadioButton(tr("B"));

    cp1->setChecked(true);

    colorComponentsLayout = new QHBoxLayout;
    colorComponentsLayout->addWidget(cp1);
    colorComponentsLayout->addWidget(cp2);
    colorComponentsLayout->addWidget(cp3);
    colorComponentsLayout->addStretch(1);
    colorComponents->setLayout(colorComponentsLayout);

    slider = new QSlider(Qt::Orientation::Horizontal, this);
    slider->setGeometry(WINDOW_WIDTH / 2 - 150, 330, 300, 20);
    slider->setMinimum(0);
    slider->setMaximum(255);
    centralLayout->addWidget(slider, 3, 1, 1, 2, Qt::AlignBottom);
    slider->setPalette(*labelPalette);

    updateSliderValue();

    minColorComponentValue = new QLabel("0", this);
    minColorComponentValue->setGeometry(WINDOW_WIDTH / 2 - 150, 315, 20, 10);
    centralLayout->addWidget(minColorComponentValue, 3, 1, Qt::AlignLeft | Qt::AlignTop);

    maxColorComponentValue = new QLabel("255", this);
    maxColorComponentValue->setGeometry(WINDOW_WIDTH / 2 + 140, 315, 20, 10);
    centralLayout->addWidget(maxColorComponentValue, 3, 2, Qt::AlignRight | Qt::AlignTop);

    currentColorComponentValue  = new QLabel("0", this);
    currentColorComponentValue->setGeometry(WINDOW_WIDTH / 2 - 10, 315, 20, 10);
    centralLayout->addWidget(currentColorComponentValue, 3, 1, 1, 2, Qt::AlignCenter | Qt::AlignTop);

    connect(this, SIGNAL(colorComponentWasChanged(int)), currentColorComponentValue, SLOT(setNum(int)));

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeColorBySlider()));

    connect(cp1, SIGNAL(clicked(bool)), this, SLOT(updateSliderValue()));
    connect(cp2, SIGNAL(clicked(bool)), this, SLOT(updateSliderValue()));
    connect(cp3, SIGNAL(clicked(bool)), this, SLOT(updateSliderValue()));

    connect(plus, SIGNAL(clicked()), this, SLOT(incColorComponent()));
    connect(plus, SIGNAL(clicked()), this, SLOT(updateSliderValue()));
    connect(minus, SIGNAL(clicked()), this, SLOT(decColorComponent()));
    connect(minus, SIGNAL(clicked()), this, SLOT(updateSliderValue()));
}

void MainWindow::incColorComponent() {
    if (cp1->isChecked() && labelForegroundColor->red() < 255)
        labelForegroundColor->setRed(labelForegroundColor->red() + 1);
    if (cp2->isChecked() && labelForegroundColor->green() < 255)
        labelForegroundColor->setGreen(labelForegroundColor->green() + 1);
    if (cp3->isChecked() && labelForegroundColor->blue() < 255)
        labelForegroundColor->setBlue(labelForegroundColor->blue() + 1);
    updateColor();
}

void MainWindow::decColorComponent() {
    if (cp1->isChecked() && labelForegroundColor->red() > 0)
        labelForegroundColor->setRed(labelForegroundColor->red() - 1);
    if (cp2->isChecked() && labelForegroundColor->green() > 0)
        labelForegroundColor->setGreen(labelForegroundColor->green() - 1);
    if (cp3->isChecked() && labelForegroundColor->blue() > 0)
        labelForegroundColor->setBlue(labelForegroundColor->blue() - 1);
    updateColor();
}

void MainWindow::changeColorBySlider() {
    if (cp1->isChecked()) {
        emit colorComponentWasChanged(slider->value());
        labelForegroundColor->setRed(slider->value());
    }
    if (cp2->isChecked()) {
        emit colorComponentWasChanged(slider->value());
        labelForegroundColor->setGreen(slider->value());
    }
    if (cp3->isChecked()) {
        emit colorComponentWasChanged(slider->value());
        labelForegroundColor->setBlue(slider->value());
    }
    updateColor();
}

void MainWindow::updateSliderValue() {
    if (cp1->isChecked()) slider->setValue(labelForegroundColor->red());
    if (cp2->isChecked()) slider->setValue(labelForegroundColor->green());
    if (cp3->isChecked()) slider->setValue(labelForegroundColor->blue());
}

void MainWindow::updateColor() {
    labelPalette->setColor(QPalette::Text, *labelForegroundColor);
    text->setPalette(*labelPalette);
}

MainWindow::~MainWindow() {
    delete text;
    delete labelForegroundColor;
    delete labelPalette;
    delete plus;
    delete minus;
    delete cp1;
    delete cp2;
    delete cp3;
    delete colorComponentsLayout;
    delete colorComponents;
    delete slider;
    delete font;
    delete minColorComponentValue;
    delete maxColorComponentValue;
    delete currentColorComponentValue;
    delete centralLayout;
    delete centralWidget;
}
