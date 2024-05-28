#include "mainwindow.h"
#include "bounceengine.h"
#include "bouncewindow.h"
#include "compounddialog.h"
#include "model.h"
#include "periodictabledialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model &model, BounceEngine &engine, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Chemistry)
    , model(&model)
    , PTDialog(new PeriodicTableDialog)
    , engine(&engine)
{
    goodSound.setSource(QUrl("qrc:/CompoundDiscoveredSound.wav"));
    badSound.setSource(QUrl("qrc:/BondFailedSound.wav"));
    goodSound.setVolume(0.3); // sets the volume of the sound effect player.
    badSound.setVolume(0.3);
    ui->setupUi(this);
    connect(ui->clearButton, &QAbstractButton::clicked, &engine, &BounceEngine::clearAtoms);
    connect(&engine, &BounceEngine::compoundBuilt, &model, &Model::checkDiscovery);
    connect(&model, &Model::newCompoundDiscovered, this, &MainWindow::addNewlyDiscoveredCompound);
    connect(ui->periodicTableButton, &QAbstractButton::clicked, PTDialog, [this] {
        PTDialog->show();
        PTDialog->setFocus();
        PTDialog->raise();
    });

    connect(&engine, &BounceEngine::bondFailed, this, &MainWindow::failBond);
    connect(PTDialog,
            &PeriodicTableDialog::periodicTableClicked,
            this,
            &MainWindow::acceptPeriodicTableClicked);
    ui->discoveredCompoundsLayout->setAlignment(Qt::AlignTop);
    ui->playSpace->setStyleSheet(QString("border: 1px solid black;"));
    ui->playSpace->connectEngine(&engine);
    engine.run();
}

void MainWindow::showMolecule(Compound* comp){
    compoundDialog = new CompoundDialog(comp);
    compoundDialog->show();
}

void MainWindow::failBond(QString failBonds){
    badSound.play();
    ui->userErrorMessage->setText(failBonds);
    QTimer::singleShot(6000, [&](){ ui->userErrorMessage->setText("   ");});
}

void MainWindow::addNewlyDiscoveredCompound(Compound* comp){

    goodSound.play();
    QPushButton* a = new QPushButton(comp->formula);
    a->setObjectName(comp->commonName);
    connect(a, &QPushButton::clicked, this, [this, comp](){showMolecule(comp);});
    a->setFixedHeight(20);
    ui->discoveredCompoundsLayout->addWidget(a);
    a->setVisible(true);
}

void MainWindow::acceptPeriodicTableClicked(QStringList atomInformation)
{
    QString atomicNumber("Atomic Number: " + atomInformation[0]);
    QString elementName("\nElement Name: " + atomInformation[1]);
    QString atomicWeight("\nAtomic Weight: " + atomInformation[2]);
    QString series("\nSeries: " + atomInformation[3]);
    QString phase("\nPhase: " + atomInformation[4]);
    ui->elementalInformation->setText(atomicNumber + elementName + atomicWeight + series + phase);
    engine->addAtom(300, 300, model->getElement(atomInformation[0].toInt()));
}

void MainWindow::testSlot(QStringList list){
    qDebug() << list;
}

MainWindow::~MainWindow()
{
    delete ui;
}
