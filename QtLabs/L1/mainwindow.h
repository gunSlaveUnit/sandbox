//
// Created by gunSlaveUnit on 01.03.2022.
//

#pragma once

#ifndef LAB1_TEXTCOLORCHANGER_MAINWINDOW_H
#define LAB1_TEXTCOLORCHANGER_MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QGraphicsView>
#include <QSizePolicy>


class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() final;
public slots:
    void incColorComponent();
    void decColorComponent();
    void changeColorBySlider();
    void updateSliderValue();
signals:
    void colorComponentWasChanged(int);
private:
    void updateColor();

    const int32_t WINDOW_WIDTH = 600, WINDOW_HEIGHT = 400;
    const QString WINDOW_TITLE = "Color Changer";
    QGraphicsView *centralWidget;
    QGridLayout *centralLayout;

    QLineEdit *text;
    QColor *labelForegroundColor;
    QPalette *labelPalette;
    QPushButton *plus, *minus;
    QGroupBox *colorComponents;
    QRadioButton *cp1, *cp2, *cp3;
    QHBoxLayout *colorComponentsLayout;
    QSlider *slider;
    QFont *font;
    QLabel *minColorComponentValue;
    QLabel *maxColorComponentValue;
    QLabel *currentColorComponentValue;
};


#endif //LAB1_TEXTCOLORCHANGER_MAINWINDOW_H
