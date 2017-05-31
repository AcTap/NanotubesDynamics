#ifndef RELATIONWINDOW_H
#define RELATIONWINDOW_H
#include "swarmmodel.h"

#include <QtWidgets/QWidget>

#include <QDoubleSpinBox>

#include <QComboBox>

class RelationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RelationWindow(SwarmModel *model,QWidget *parent = 0);

signals:

public slots:
    void chooseSwarmFrom(int);
    void chooseSwarmTo(int);

    void setC1(double);
    void setC2(double);
    void setC3(double);
    void setC6(double);
private:
    SwarmModel *model;
    Relation currentRel;
    uint SwarmIdFrom;
    uint SwarmIdTo;
    void updateOptions();
    void fillCombo(QComboBox*);
    //Options
    QDoubleSpinBox *c1Line;
    QDoubleSpinBox *c2Line;
    QDoubleSpinBox *c3Line;
    QDoubleSpinBox *c6Line;
    //
};

#endif // RELATIONWINDOW_H
