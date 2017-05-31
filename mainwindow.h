#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QComboBox>
#include <QColorDialog>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QString>
#include "swarm2dwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QSlider *createSlider(int start,int finish,int step);
    void SaveConfiguration(QString file);
    void LoadConfiguration(QString file);
public slots:
    void chooseSwarm(int);
    void setColor(QColor);
    void showPicker();
    void startStop();

    void setAmount(int);
    void setAmount(QString);

    void setSize(int);
    void setSize(QString);

    void setRadius(int);
    void setRadius(double);

    void setNormVel(int);
    void setNormVel(double);

    void setMaxVel(int);
    void setMaxVel(double);

    void setC1(int);
    void setC1(double);

    void setC2(int);
    void setC2(double);

    void setC3(int);
    void setC3(double);

    void setC4(int);
    void setC4(double);

    void setC5(int);
    void setC5(double);

    void setC6(int);
    void setC6(double);

    void setC7(int);
    void setC7(double);

    void setVisc(int);
    void setVisc(double);

    void setAngle(int);
    void setAngle(double);

    void saveDialog();
    void loadDialog();
    void relDialog();

    void Add();

private:
    Swarm2dWindow *glWindow;
    SwarmModel *model;
    QPushButton *stBtn;
    QPushButton *picker;
    QComboBox *swarmList;
    QColorDialog *swarmColor;
    Swarm* currentSwarm;
    bool autoAdd;
    void updateOptions();
    //Options
    QSlider *amountSlider;
    QLineEdit *amountLine;

    QSlider *sizeSlider;
    QLineEdit *sizeLine;

    QSlider *radiusSlider;
    QDoubleSpinBox *radiusLine;

    QSlider *normVelSlider;
    QDoubleSpinBox *normVelLine;

    QSlider *maxVelSlider;
    QDoubleSpinBox *maxVelLine;

    QSlider *c1Slider;
    QDoubleSpinBox *c1Line;

    QSlider *c2Slider;
    QDoubleSpinBox *c2Line;

    QSlider *c3Slider;
    QDoubleSpinBox *c3Line;

    QSlider *c4Slider;
    QDoubleSpinBox *c4Line;

    QSlider *c5Slider;
    QDoubleSpinBox *c5Line;

    QSlider *c6Slider;
    QDoubleSpinBox *c6Line;

    QSlider *c7Slider;
    QDoubleSpinBox *c7Line;

    QSlider *viscSlider;
    QDoubleSpinBox *viscLine;

    QSlider *angleSlider;
    QDoubleSpinBox *angleLine;
    //
};

#endif // MAINWINDOW_H
