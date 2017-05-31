#include "mainwindow.h"
#include "relationwindow.h"
#include <QStringListModel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QIntValidator>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QIcon>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Main window"));
    QWidget *central = new QWidget();
    autoAdd=true;
    model = new Swarm2d();
    model->setSize(500,500);
    model->addSwarm(100,QColor(255,0,0),20,50,0.75,0.75,60,0.3,0.3,5,0.0,0.0,0.0,0,90);
    //model->addSwarm(140,QColor(155,0,155),2,93.28,0.64,0.58,96.71,0.07,0.41,5,5.15,0.2,0.2,0,90);
    currentSwarm = model->getSwarm(0);
    glWindow = new Swarm2dWindow((Swarm2d*)model,central);

    QWidget *buttonWid = new QWidget();
    QHBoxLayout *container = new QHBoxLayout();

    container->addWidget(glWindow);

    QHBoxLayout *subcontainer0 = new QHBoxLayout();

    QVBoxLayout *subcontainer1 = new QVBoxLayout();

    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *saveBtn = new QPushButton("Save",buttonWid);
    connect(saveBtn,SIGNAL(clicked(bool)),this,SLOT(saveDialog()));
    btnLayout->addWidget(saveBtn);

    QPushButton *loadBtn = new QPushButton("Load",buttonWid);
    connect(loadBtn,SIGNAL(clicked(bool)),this,SLOT(loadDialog()));
    btnLayout->addWidget(loadBtn);

    QPushButton *relBtn = new QPushButton("Relation",buttonWid);
    connect(relBtn,SIGNAL(clicked(bool)),this,SLOT(relDialog()));
    btnLayout->addWidget(relBtn);


    swarmList = new QComboBox(buttonWid);
    swarmList->addItem(tr("Swarm 0"));
    //swarmList->addItem(tr("Swarm 1"));
    connect(swarmList,SIGNAL(currentIndexChanged(int)),this,SLOT(chooseSwarm(int)));
    subcontainer1->addWidget(swarmList);
    QPushButton *addbtn = new QPushButton("+",buttonWid);
    connect(addbtn,SIGNAL(clicked(bool)),this,SLOT(Add()));
    btnLayout->addWidget(addbtn);

    auto amountLabel = new QLabel("Amount",buttonWid);
    subcontainer1->addWidget(amountLabel);
    amountSlider = MainWindow::createSlider(10,5000,1);
    connect(amountSlider,SIGNAL(valueChanged(int)),this,SLOT(setAmount(int)));
    subcontainer1->addWidget(amountSlider);
    amountLine = new QLineEdit();
    amountLine->setValidator(new QIntValidator());
    connect(amountLine,SIGNAL(textChanged(QString)),this,SLOT(setAmount(QString)));
    subcontainer1->addWidget(amountLine);


    swarmColor = new QColorDialog(buttonWid);
    swarmColor->setOption(QColorDialog::DontUseNativeDialog);
    connect(swarmColor,SIGNAL(currentColorChanged(QColor)),this,SLOT(setColor(QColor)));

    QPixmap icon(16, 16);
    icon.fill(currentSwarm->color);

    picker = new QPushButton(QIcon(icon),tr("Choose color"),buttonWid);
    connect(picker,SIGNAL(clicked()),this,SLOT(showPicker()));
    subcontainer1->addWidget(picker);

    auto sizeLabel = new QLabel("Point size",buttonWid);
    subcontainer1->addWidget(sizeLabel);
    sizeSlider = MainWindow::createSlider(1,50,1);
    connect(sizeSlider,SIGNAL(valueChanged(int)),this,SLOT(setSize(int)));
    subcontainer1->addWidget(sizeSlider);
    sizeLine = new QLineEdit(buttonWid);
    sizeLine->setValidator(new QIntValidator());
    connect(sizeLine,SIGNAL(textChanged(QString)),this,SLOT(setSize(QString)));
    subcontainer1->addWidget(sizeLine);

    auto radiusLabel = new QLabel("Perseption range",buttonWid);
    subcontainer1->addWidget(radiusLabel);
    radiusSlider = MainWindow::createSlider(0,50000,1);
    connect(radiusSlider,SIGNAL(valueChanged(int)),this,SLOT(setRadius(int)));
    subcontainer1->addWidget(radiusSlider);
    radiusLine = new QDoubleSpinBox(buttonWid);
    radiusLine->setSingleStep(0.01);
    radiusLine->setMaximum(500.0);
    radiusLine->setMinimum(0);
    connect(radiusLine,SIGNAL(valueChanged(double)),this,SLOT(setRadius(double)));
    subcontainer1->addWidget(radiusLine);

    auto normVelLabel = new QLabel("Normal speed",buttonWid);
    subcontainer1->addWidget(normVelLabel);
    normVelSlider = MainWindow::createSlider(0,2000,1);
    connect(normVelSlider,SIGNAL(valueChanged(int)),this,SLOT(setNormVel(int)));
    subcontainer1->addWidget(normVelSlider);
    normVelLine = new QDoubleSpinBox(buttonWid);
    normVelLine->setSingleStep(0.01);
    normVelLine->setMaximum(20.0);
    normVelLine->setMinimum(0);
    connect(normVelLine,SIGNAL(valueChanged(double)),this,SLOT(setNormVel(double)));
    subcontainer1->addWidget(normVelLine);

    auto maxVelLabel = new QLabel("Maximum speed",buttonWid);
    subcontainer1->addWidget(maxVelLabel);
    maxVelSlider = MainWindow::createSlider(0,4000,1);
    connect(maxVelSlider,SIGNAL(valueChanged(int)),this,SLOT(setMaxVel(int)));
    subcontainer1->addWidget(maxVelSlider);
    maxVelLine = new QDoubleSpinBox(buttonWid);
    maxVelLine->setSingleStep(0.01);
    maxVelLine->setMaximum(40.0);
    maxVelLine->setMinimum(0);
    connect(maxVelLine,SIGNAL(valueChanged(double)),this,SLOT(setMaxVel(double)));
    subcontainer1->addWidget(maxVelLine);

    auto viscLabel = new QLabel("Viscosity",buttonWid);
    subcontainer1->addWidget(viscLabel);
    viscSlider = MainWindow::createSlider(0,90,5);
    connect(viscSlider,SIGNAL(valueChanged(int)),this,SLOT(setVisc(int)));
    subcontainer1->addWidget(viscSlider);
    viscLine = new QDoubleSpinBox(buttonWid);
    viscLine->setSingleStep(0.05);
    viscLine->setMaximum(0.90);
    viscLine->setMinimum(0);
    connect(viscLine,SIGNAL(valueChanged(double)),this,SLOT(setVisc(double)));
    subcontainer1->addWidget(viscLine);

    auto angleLabel = new QLabel("Angle",buttonWid);
    subcontainer1->addWidget(angleLabel);
    angleSlider = MainWindow::createSlider(-90,90,1);
    connect(angleSlider,SIGNAL(valueChanged(int)),this,SLOT(setAngle(int)));
    subcontainer1->addWidget(angleSlider);
    angleLine = new QDoubleSpinBox(buttonWid);
    angleLine->setSingleStep(1);
    angleLine->setMaximum(90);
    angleLine->setMinimum(-90);
    connect(angleLine,SIGNAL(valueChanged(double)),this,SLOT(setAngle(double)));
    subcontainer1->addWidget(angleLine);

    subcontainer1->addStretch();


    QVBoxLayout *subcontainer2 = new QVBoxLayout();
    auto c1Label = new QLabel("Strength of cohesive force(c1)",buttonWid);
    subcontainer2->addWidget(c1Label);
    c1Slider = MainWindow::createSlider(0,1000,1);
    connect(c1Slider,SIGNAL(valueChanged(int)),this,SLOT(setC1(int)));
    subcontainer2->addWidget(c1Slider);
    c1Line = new QDoubleSpinBox(buttonWid);
    c1Line->setSingleStep(0.001);
    c1Line->setDecimals(3);
    c1Line->setMaximum(1.0);
    c1Line->setMinimum(0);
    connect(c1Line,SIGNAL(valueChanged(double)),this,SLOT(setC1(double)));
    subcontainer2->addWidget(c1Line);

    auto c2Label = new QLabel("Strength of aligning force(c2)");
    subcontainer2->addWidget(c2Label);
    c2Slider = MainWindow::createSlider(0,1000,1);
    connect(c2Slider,SIGNAL(valueChanged(int)),this,SLOT(setC2(int)));
    subcontainer2->addWidget(c2Slider);
    c2Line = new QDoubleSpinBox(buttonWid);
    c2Line->setSingleStep(0.001);
    c2Line->setDecimals(3);
    c2Line->setMaximum(1.0);
    c2Line->setMinimum(0);
    connect(c2Line,SIGNAL(valueChanged(double)),this,SLOT(setC2(double)));
    subcontainer2->addWidget(c2Line);

    auto c3Label = new QLabel("Strength of separating force(c3)",buttonWid);
    subcontainer2->addWidget(c3Label);
    c3Slider = MainWindow::createSlider(0,10000,1);
    connect(c3Slider,SIGNAL(valueChanged(int)),this,SLOT(setC3(int)));
    subcontainer2->addWidget(c3Slider);
    c3Line = new QDoubleSpinBox(buttonWid);
    c3Line->setSingleStep(0.01);
    c3Line->setMaximum(100.0);
    c3Line->setMinimum(0);
    connect(c3Line,SIGNAL(valueChanged(double)),this,SLOT(setC3(double)));
    subcontainer2->addWidget(c3Line);

    auto c4Label = new QLabel("Probability of random steering(c4)",buttonWid);
    subcontainer2->addWidget(c4Label);
    c4Slider = MainWindow::createSlider(0,50,1);
    connect(c4Slider,SIGNAL(valueChanged(int)),this,SLOT(setC4(int)));
    subcontainer2->addWidget(c4Slider);
    c4Line = new QDoubleSpinBox(buttonWid);
    c4Line->setSingleStep(0.01);
    c4Line->setDecimals(2);
    c4Line->setMaximum(0.5);
    connect(c4Line,SIGNAL(valueChanged(double)),this,SLOT(setC4(double)));
    subcontainer2->addWidget(c4Line);

    auto c5Label = new QLabel("Tendency of self-propulsion(c5)",buttonWid);
    subcontainer2->addWidget(c5Label);
    c5Slider = MainWindow::createSlider(0,100,1);
    connect(c5Slider,SIGNAL(valueChanged(int)),this,SLOT(setC5(int)));
    subcontainer2->addWidget(c5Slider);
    c5Line = new QDoubleSpinBox(buttonWid);
    c5Line->setSingleStep(0.01);
    c5Line->setDecimals(1);
    c5Line->setMaximum(1.0);
    connect(c5Line,SIGNAL(valueChanged(double)),this,SLOT(setC5(double)));
    subcontainer2->addWidget(c5Line);

    auto c6Label = new QLabel("Alignment(c6)",buttonWid);
    subcontainer2->addWidget(c6Label);
    c6Slider = MainWindow::createSlider(0,100,1);
    connect(c6Slider,SIGNAL(valueChanged(int)),this,SLOT(setC6(int)));
    subcontainer2->addWidget(c6Slider);
    c6Line = new QDoubleSpinBox(buttonWid);
    c6Line->setSingleStep(0.01);
    c6Line->setDecimals(2);
    c6Line->setMaximum(1.0);
    connect(c6Line,SIGNAL(valueChanged(double)),this,SLOT(setC6(double)));
    subcontainer2->addWidget(c6Line);

    auto c7Label = new QLabel("Involvement(c7)",buttonWid);
    subcontainer2->addWidget(c7Label);
    c7Slider = MainWindow::createSlider(0,100,1);
    connect(c7Slider,SIGNAL(valueChanged(int)),this,SLOT(setC7(int)));
    subcontainer2->addWidget(c7Slider);
    c7Line = new QDoubleSpinBox(buttonWid);
    c7Line->setSingleStep(0.01);
    c7Line->setDecimals(2);
    c7Line->setMaximum(1.0);
    connect(c7Line,SIGNAL(valueChanged(double)),this,SLOT(setC7(double)));
    subcontainer2->addWidget(c7Line);

    subcontainer2->addStretch();

    QGridLayout *sizeLayout = new QGridLayout();
    auto heightLabel = new QLabel("Height",buttonWid);
    sizeLayout->addWidget(heightLabel,0,0);
    auto widthLabel = new QLabel("Width",buttonWid);
    sizeLayout->addWidget(widthLabel,0,1);

    QDoubleSpinBox *height = new QDoubleSpinBox(buttonWid);
    height->setDecimals(0);
    height->setMaximum(500000.0);
    height->setValue(500);
    connect(height,SIGNAL(valueChanged(QString)),glWindow,SLOT(setHeight(QString)));
    sizeLayout->addWidget(height,1,0);

    QDoubleSpinBox *width = new QDoubleSpinBox(buttonWid);
    width->setDecimals(0);
    width->setMaximum(500000.0);
    width->setValue(500);
    connect(width,SIGNAL(valueChanged(QString)),glWindow,SLOT(setWidth(QString)));
    sizeLayout->addWidget(width,1,1);
    subcontainer2->addLayout(sizeLayout);

    auto timeLabel = new QLabel("Speed",buttonWid);
    subcontainer2->addWidget(timeLabel);
    auto time = createSlider(0,100,1);
    connect(time,SIGNAL(valueChanged(int)),glWindow,SLOT(setTpp(int)));
    time->setValue(50);
    stBtn = new QPushButton(tr("Start"),central);
    connect(stBtn,SIGNAL(clicked()),this,SLOT(startStop()));
    subcontainer2->addWidget(time);
    subcontainer2->addWidget(stBtn);

    subcontainer1->addLayout(btnLayout);

    subcontainer0->addLayout(subcontainer1);
    subcontainer0->addLayout(subcontainer2);
    buttonWid->setMaximumWidth(500);
    updateOptions();
    buttonWid->setLayout(subcontainer0);
    container->addWidget(buttonWid);
    central->setLayout(container);
    setCentralWidget(central);
}

MainWindow::~MainWindow()
{

}

QSlider *MainWindow::createSlider(int start, int finish, int step)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(start, finish);
    slider->setSingleStep(step);
    slider->setPageStep(10);
    slider->setTickInterval(10*step);
    slider->setMaximumWidth(250);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void MainWindow::SaveConfiguration(QString file)
{
    QFile data(file);
    if(data.open(QFile::WriteOnly)){
        QTextStream ts(&data) ;
        for(uint i=0;i<model->swarmCount();++i){
            Swarm *s = model->getSwarm(i);
            ts<<s->swarm.size()<<"*("<<s->detectR<<","<<s->velNorm<<","<<s->velMax<<","
             <<s->c1<<","<<s->c2<<","<<s->c3<<","<<s->c4<<","<<s->c5<<","<<s->c6<<","<<s->c7<<","<<s->viscos<<")\n";
        }
    }
}

void MainWindow::LoadConfiguration(QString file)
{
    QFile data(file);
    if(data.open(QFile::ReadOnly)){
        swarmList->clear();
        currentSwarm = nullptr;
        autoAdd=false;
        model->hardReset();
        QTextStream ts(&data);
        QString line;
        int i=0;
        while (ts.readLineInto(&line)) {
            QStringList list1 = line.split("\*",QString::SkipEmptyParts);
            int amount = list1.at(0).toInt();
            QString recipe = list1.at(1);
            recipe = recipe.trimmed();
            recipe = recipe.remove(0,1);
            recipe.chop(1);
            QStringList options = recipe.split(",");
            if (options.size()<11){
                continue;
            }
            model->addSwarm(amount,QColor(255/(i+1),10*i+50,10*i*i),20,
                            options[0].toFloat(),options[3].toFloat(),options[4].toFloat(),
                            options[5].toFloat(),options[6].toFloat(),options[7].toFloat(),
                            options[2].toFloat(),options[1].toFloat(),options[8].toFloat(),options[9].toFloat(),options[10].toFloat(),90);
            swarmList->addItem(tr("Swarm ")+QString::number(i++));
        }
        if(i>0){
            currentSwarm = model->getSwarm(0);
            updateOptions();
        }
    }

}



void MainWindow::chooseSwarm(int index)
{
    if(index<0) return;
    currentSwarm = model->getSwarm((uint)index);
    updateOptions();
}


void MainWindow::setColor(QColor value)
{
    currentSwarm->color = value;
    QPixmap icon(16, 16);
       icon.fill(currentSwarm->color);
       picker->setIcon(QIcon(icon));
}


void MainWindow::showPicker()
{
    swarmColor->show();
    swarmColor->setCurrentColor(currentSwarm->color);
}


void MainWindow::startStop()
{
    if(glWindow->running){
        glWindow->running = false;
        stBtn->setText(tr("Start"));
    }else{
        glWindow->running = true;
        stBtn->setText(tr("Pause"));
    }
}


void MainWindow::setAmount(int amount)
{
    model->setSwarmAmount((uint)swarmList->currentIndex(),amount);
    amountLine->setText(QString::number(amount));
}


void MainWindow::setAmount(QString value)
{
    int amount = value.toInt();
    model->setSwarmAmount((uint)swarmList->currentIndex(),amount);
    amountSlider->setValue(amount);
}


void MainWindow::setSize(int amount)
{
    currentSwarm->size = (float)amount;
    sizeLine->setText(QString::number(amount));
}


void MainWindow::setSize(QString value)
{
    int amount = value.toInt();
    currentSwarm->size = (float)amount;
    sizeSlider->setValue(amount);
}


void MainWindow::setRadius(int value)
{
    float rad = value/100.0f;
    currentSwarm->detectR = rad;
    radiusLine->setValue(rad);
}


void MainWindow::setRadius(double value)
{
    currentSwarm->detectR = value;
    radiusSlider->setValue((int)(value*100));
}


void MainWindow::setNormVel(int value)
{
    float vel = value/100.0f;
    currentSwarm->velNorm = vel;
    normVelLine->setValue(vel);
}


void MainWindow::setNormVel(double value)
{
    currentSwarm->velNorm = value;
    normVelSlider->setValue((int)(value*100));
}


void MainWindow::setMaxVel(int value)
{
    float vel = value/100.0f;
    currentSwarm->velMax = vel;
    maxVelLine->setValue(vel);
}


void MainWindow::setMaxVel(double value)
{
    currentSwarm->velMax = value;
    maxVelSlider->setValue((int)(value*100));
}


void MainWindow::setC1(int value)
{
    float vel = value/1000.0f;
    currentSwarm->c1 = vel;
    c1Line->setValue(vel);
}


void MainWindow::setC1(double value)
{
    currentSwarm->c1 = value;
    c1Slider->setValue((int)(value*1000));
}


void MainWindow::setC2(int value)
{
    float vel = value/1000.0f;
    currentSwarm->c2 = vel;
    c2Line->setValue(vel);
}


void MainWindow::setC2(double value)
{
    currentSwarm->c2 = value;
    c2Slider->setValue((int)(value*1000));
}


void MainWindow::setC3(int value)
{
    float vel = value/100.0f;
    currentSwarm->c3 = vel;
    c3Line->setValue(vel);
}


void MainWindow::setC3(double value)
{
    currentSwarm->c3 = value;
    c3Slider->setValue((int)(value*100));
}


void MainWindow::setC4(int value)
{
    float vel = value/100.0f;
    currentSwarm->c4 = vel;
    c4Line->setValue(vel);
}


void MainWindow::setC4(double value)
{
    currentSwarm->c4 = value;
    c4Slider->setValue((int)(value*100));
}


void MainWindow::setC5(int value)
{
    float vel = value/100.0f;
    currentSwarm->c5 = vel;
    c5Line->setValue(vel);
}


void MainWindow::setC5(double value)
{
    currentSwarm->c5 = value;
    c5Slider->setValue((int)(value*100));
}


void MainWindow::setC6(int value)
{
    float vel = value/100.0f;
    currentSwarm->c6 = vel;
    c6Line->setValue(vel);
}


void MainWindow::setC6(double value)
{
    currentSwarm->c6 = value;
    c6Slider->setValue((int)(value*100));
}

void MainWindow::setC7(int value)
{
    float vel = value/100.0f;
    currentSwarm->c7 = vel;
    c7Line->setValue(vel);
}


void MainWindow::setC7(double value)
{
    currentSwarm->c7 = value;
    c7Slider->setValue((int)(value*100));
}

void MainWindow::setVisc(int value)
{
    float val = value/100.0f;
    currentSwarm->viscos = val;
    viscLine->setValue(val);
}

void MainWindow::setVisc(double value)
{
    currentSwarm->viscos = value;
    viscSlider->setValue((int)(value*100));
}

void MainWindow::setAngle(int value)
{
    float val = value;
    currentSwarm->angle = val;
    angleLine->setValue(val);
}

void MainWindow::setAngle(double value)
{
    currentSwarm->angle = value;
    angleSlider->setValue((int)(value));
}

void MainWindow::saveDialog()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save config"),
            QDir::currentPath(),
            tr("Config files (*.txt *.dat *.ini *.conf);;All files (*.*)") );
    SaveConfiguration(filename);
}


void MainWindow::loadDialog()
{
    QString filename = QFileDialog::getOpenFileName(
            this,
            tr("Open config"),
            QDir::currentPath(),
            tr("Config files (*.txt *.dat *.ini *.conf);;All files (*.*)") );
    LoadConfiguration(filename);
}

void MainWindow::relDialog()
{
    RelationWindow *wnd = new RelationWindow(model);
    wnd->show();
}

void MainWindow::Add()
{
    model->addSwarm(150,QColor(255/(1),10+50,10),2,
                    0,0,0,
                    0,0,0,
                    0,0,0,0,0,90);
    swarmList->addItem(tr("Swarm ")+QString::number(model->swarmCount()-1));
    swarmList->setCurrentIndex(swarmList->count()-1);
}

void MainWindow::updateOptions()
{
    amountSlider->setValue(currentSwarm->swarm.size());
    amountLine->setText(QString::number(currentSwarm->swarm.size()));

    QPixmap icon(16, 16);
    icon.fill(currentSwarm->color);
    picker->setIcon(QIcon(icon));

    sizeSlider->setValue((int)currentSwarm->size);
    sizeLine->setText(QString::number((int)currentSwarm->size));

    radiusLine->setValue(currentSwarm->detectR);
    radiusSlider->setValue((int)(currentSwarm->detectR*100));

    normVelLine->setValue(currentSwarm->velNorm);
    normVelSlider->setValue((int)(currentSwarm->velNorm*100));

    maxVelLine->setValue(currentSwarm->velMax);
    maxVelSlider->setValue((int)(currentSwarm->velMax*100));

    c1Line->setValue(currentSwarm->c1);
    c1Slider->setValue((int)(currentSwarm->c1*1000));

    c2Line->setValue(currentSwarm->c2);
    c2Slider->setValue((int)(currentSwarm->c2*1000));

    c3Line->setValue(currentSwarm->c3);
    c3Slider->setValue((int)(currentSwarm->c3*100));

    c4Line->setValue(currentSwarm->c4);
    c4Slider->setValue((int)(currentSwarm->c4*100));

    c5Line->setValue(currentSwarm->c5);
    c5Slider->setValue((int)(currentSwarm->c5*100));

    c6Line->setValue(currentSwarm->c6);
    c6Slider->setValue((int)(currentSwarm->c6*100));

    c7Line->setValue(currentSwarm->c7);
    c7Slider->setValue((int)(currentSwarm->c7*100));

    viscLine->setValue(currentSwarm->viscos);
    viscSlider->setValue((int)(currentSwarm->viscos*100));

    angleLine->setValue(currentSwarm->angle);
    angleSlider->setValue((int)(currentSwarm->angle));
}
