#include "relationwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

RelationWindow::RelationWindow(SwarmModel *model, QWidget *parent) : QWidget(parent),model(model),SwarmIdFrom(0),SwarmIdTo(0)
{
    setWindowTitle(tr("Relation"));
    setMinimumWidth(200);
    QVBoxLayout *container = new QVBoxLayout();

    QHBoxLayout *subcontainer0 = new QHBoxLayout();

    QComboBox *swarmListFrom = new QComboBox(this);

    fillCombo(swarmListFrom);

    connect(swarmListFrom,SIGNAL(currentIndexChanged(int)),this,SLOT(chooseSwarmFrom(int)));
    QComboBox *swarmListTo = new QComboBox(this);

    fillCombo(swarmListTo);
    connect(swarmListTo,SIGNAL(currentIndexChanged(int)),this,SLOT(chooseSwarmTo(int)));

    subcontainer0->addWidget(swarmListFrom);
    subcontainer0->addWidget(swarmListTo);
    container->addLayout(subcontainer0);

    auto c1Label = new QLabel("(c1)",this);
    container->addWidget(c1Label);
    c1Line = new QDoubleSpinBox(this);
    c1Line->setSingleStep(0.01);
    c1Line->setDecimals(2);
    c1Line->setMaximum(100.0);
    c1Line->setMinimum(-100.0);
    connect(c1Line,SIGNAL(valueChanged(double)),this,SLOT(setC1(double)));
    container->addWidget(c1Line);

    auto c2Label = new QLabel("(c2)",this);
    container->addWidget(c2Label);
    c2Line = new QDoubleSpinBox(this);
    c2Line->setSingleStep(0.01);
    c2Line->setDecimals(2);
    c2Line->setMaximum(100.0);
    c2Line->setMinimum(-100.0);
    connect(c2Line,SIGNAL(valueChanged(double)),this,SLOT(setC2(double)));
    container->addWidget(c2Line);

    auto c3Label = new QLabel("(c3)",this);
    container->addWidget(c3Label);
    c3Line = new QDoubleSpinBox(this);
    c3Line->setSingleStep(0.01);
    c3Line->setDecimals(2);
    c3Line->setMaximum(100.0);
    c3Line->setMinimum(-100.0);
    connect(c3Line,SIGNAL(valueChanged(double)),this,SLOT(setC3(double)));
    container->addWidget(c3Line);

    auto c6Label = new QLabel("(c6)",this);
    container->addWidget(c6Label);
    c6Line = new QDoubleSpinBox(this);
    c6Line->setSingleStep(0.01);
    c6Line->setDecimals(2);
    c6Line->setMaximum(100.0);
    c6Line->setMinimum(-100.0);
    connect(c6Line,SIGNAL(valueChanged(double)),this,SLOT(setC6(double)));
    container->addWidget(c6Line);


    currentRel = model->relation.get(SwarmIdFrom,SwarmIdTo);
    updateOptions();
    setLayout(container);

}

void RelationWindow::chooseSwarmFrom(int value)
{
    SwarmIdFrom = value;
    currentRel = model->relation.get(SwarmIdFrom,SwarmIdTo);
    updateOptions();
}

void RelationWindow::chooseSwarmTo(int value)
{
    SwarmIdTo = value;
    currentRel = model->relation.get(SwarmIdFrom,SwarmIdTo);
    updateOptions();
}

void RelationWindow::setC1(double value)
{
    currentRel["c1"]=value;
    model->relation.set(SwarmIdFrom,SwarmIdTo,currentRel);
}

void RelationWindow::setC2(double value)
{
    currentRel["c2"]=value;
    model->relation.set(SwarmIdFrom,SwarmIdTo,currentRel);
}

void RelationWindow::setC3(double value)
{
    currentRel["c3"]=value;
    model->relation.set(SwarmIdFrom,SwarmIdTo,currentRel);
}

void RelationWindow::setC6(double value)
{
    currentRel["c6"]=value;
    model->relation.set(SwarmIdFrom,SwarmIdTo,currentRel);
}

void RelationWindow::updateOptions()
{
    c1Line->setValue(currentRel["c1"]);
    c2Line->setValue(currentRel["c2"]);
    c3Line->setValue(currentRel["c3"]);
    c6Line->setValue(currentRel["c6"]);
}

void RelationWindow::fillCombo(QComboBox * list)
{
    for(int i=0;i<model->swarmCount();i++){
        list->addItem(tr("Swarm ")+QString::number(i));
    }
}
