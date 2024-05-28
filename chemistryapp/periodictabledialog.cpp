#include "periodictabledialog.h"
#include "ui_periodictabledialog.h"

PeriodicTableDialog::PeriodicTableDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PeriodicTableDialog)
{
    ui->setupUi(this);
    setMaximumSize(742, 392);
    initializePeriodicTable(ui);
}

PeriodicTableDialog::~PeriodicTableDialog()
{
    delete ui;
}

void PeriodicTableDialog::initializePeriodicTable(Ui::PeriodicTableDialog *ui)
{
    qApp->setStyleSheet("QToolTip { border: 0px;}");

    for (int i = 0; i < ui->PeriodicTable->columnCount(); i++) {
        ui->PeriodicTable->setColumnMinimumWidth(i, 40);
    }
    for (int i = 0; i < ui->PeriodicTable->rowCount(); i++) {
        ui->PeriodicTable->setRowMinimumHeight(i, 40);
    }
    ui->PeriodicTable->setRowMinimumHeight(7, 10);

    for (int i = 0; i < ui->PeriodicTable->count(); i++) {
        QLayoutItem *item = ui->PeriodicTable->itemAt(i);
        QString baseStyleSheet("\npadding: 0px; color:white;\n");
        if (item) {
            QWidget *widget = item->widget();
            if (widget) {
                if (qobject_cast<QPushButton *>(widget)) {
                    QPushButton *button = qobject_cast<QPushButton *>(item->widget());
                    int atomicNumber = button->text().split("\n")[0].toInt();
                    connect(button, &QPushButton::clicked, [this, atomicNumber, ui] {
                        // Check for atomic numbers to match the button text
                        for (int i = 0; i < ui->PeriodicTable->count(); i++) {
                            QLayoutItem *item = ui->PeriodicTable->itemAt(i);
                            if (item) {
                                QWidget *widget = item->widget();
                                if (widget) {
                                    if (qobject_cast<QPushButton *>(widget)) {
                                        QPushButton *button = qobject_cast<QPushButton *>(
                                            item->widget());
                                        if (button->text().split("\n")[0].toInt() == atomicNumber) {
                                            QString tooltip = button->toolTip().split("<p>")[1].split(
                                                "</p>")[0];
                                            QStringList toolTips = tooltip.split("<br>");
                                            QStringList finalStrings = {
                                                QString::number(atomicNumber)};
                                            for (int i = 0; i < toolTips.length(); i++) {
                                                finalStrings.append(toolTips[i]);
                                            }
                                            emit periodicTableClicked(finalStrings);
                                            //PeriodicTableDialog::hide();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    });
                    button->setStyleSheet(baseStyleSheet);
                    initializeSeries(button, atomicNumber);
                    initializePhase(button, atomicNumber);
                    button->setToolTip(
                        button->toolTip().replace(QString("</p><p>"), QString("<br>")));
                    button->setStyleSheet(
                        QString("QPushButton {") + button->styleSheet()
                        + QString("} QToolTip {background-color: white; color: black;}"));
                }
            }
        }
    }
}

void PeriodicTableDialog::initializeSeries(QPushButton *button, int atomicNumber)
{
    std::vector<int> alkaliMetals = {3, 11, 19, 37, 55, 87};
    std::vector<int> alkalineEarthMetals = {4, 12, 20, 38, 56, 88};
    std::vector<int> transitionMetals = {21, 22, 23, 24, 25, 26,  27,  28,  29,  30, 39, 40,
                                         41, 42, 43, 44, 45, 46,  47,  48,  72,  73, 74, 75,
                                         76, 77, 78, 79, 80, 104, 105, 106, 107, 108};
    std::vector<int> lanthanoids = {57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71};
    std::vector<int> actinoids = {89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103};
    std::vector<int> postTransitionMetals = {13, 31, 49, 50, 81, 82, 83, 84};
    std::vector<int> metalloids = {5, 14, 32, 33, 51, 52, 85};
    std::vector<int> reactiveNonmetals = {1, 6, 7, 8, 9, 15, 16, 17, 34, 35, 53};
    std::vector<int> nobleGases = {2, 10, 18, 36, 54, 86};
    std::vector<int> unknown = {109, 110, 111, 112, 113, 114, 115, 116, 117, 118};

    QStringList tooltips = button->toolTip().split("</p>");

    if (std::find(alkaliMetals.begin(), alkaliMetals.end(), atomicNumber) != alkaliMetals.end()) {
        button->setStyleSheet(QString("background-color: rgb(120, 72, 10);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Alkali Metal</p>") + tooltips[1]);
    } else if (std::find(alkalineEarthMetals.begin(), alkalineEarthMetals.end(), atomicNumber)
               != alkalineEarthMetals.end()) {
        button->setStyleSheet(QString("background-color: rgb(214, 148, 56);")
                              + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Alkaline Earth Metal</p>") + tooltips[1]);
    }

    else if (std::find(transitionMetals.begin(), transitionMetals.end(), atomicNumber)
             != transitionMetals.end()) {
        button->setStyleSheet(QString("background-color: rgb(128, 3, 32);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Transition Metal</p>") + tooltips[1]);
    }

    else if (std::find(lanthanoids.begin(), lanthanoids.end(), atomicNumber) != lanthanoids.end()) {
        button->setStyleSheet(QString("background-color: rgb(61, 34, 0);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Lanthanoid</p>") + tooltips[1]);
    }

    else if (std::find(actinoids.begin(), actinoids.end(), atomicNumber) != actinoids.end()) {
        button->setStyleSheet(QString("background-color: rgb(133, 52, 107);")
                              + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Actinoid</p>") + tooltips[1]);
    }

    else if (std::find(postTransitionMetals.begin(), postTransitionMetals.end(), atomicNumber)
             != postTransitionMetals.end()) {
        button->setStyleSheet(QString("background-color: rgb(46, 40, 161);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Post-Transition Metal</p>") + tooltips[1]);
    }

    else if (std::find(metalloids.begin(), metalloids.end(), atomicNumber) != metalloids.end()) {
        button->setStyleSheet(QString("background-color: rgb(42, 112, 115);")
                              + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Metalloid</p>") + tooltips[1]);
    }

    else if (std::find(reactiveNonmetals.begin(), reactiveNonmetals.end(), atomicNumber)
             != reactiveNonmetals.end()) {
        button->setStyleSheet(QString("background-color: rgb(74, 125, 30);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Reactive Nonmetal</p>") + tooltips[1]);
    }

    else if (std::find(nobleGases.begin(), nobleGases.end(), atomicNumber) != nobleGases.end()) {
        button->setStyleSheet(QString("background-color: rgb(84, 30, 125);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Noble Gas</p>") + tooltips[1]);
    }

    else {
        button->setStyleSheet(QString("background-color: rgb(30, 30, 30);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Unknown</p>") + tooltips[1]);
    }
}

void PeriodicTableDialog::initializePhase(QPushButton *button, int atomicNumber)
{
    std::vector<int> liquid = {35, 80};
    std::vector<int> gas = {1, 2, 7, 8, 9, 10, 17, 18, 36, 54, 86};
    std::vector<int> unknown
        = {104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118};

    QStringList tooltips = button->toolTip().split("</p>");

    if (std::find(liquid.begin(), liquid.end(), atomicNumber) != liquid.end()) {
        button->setStyleSheet(QString("border:2px solid rgb(181, 152, 250);")
                              + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Liquid</p>") + tooltips[1]);
    } else if (std::find(gas.begin(), gas.end(), atomicNumber) != gas.end()) {
        button->setStyleSheet(QString("border:2px solid rgb(222, 62, 70);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Gas</p>") + tooltips[1]);
    } else if (std::find(unknown.begin(), unknown.end(), atomicNumber) != unknown.end()) {
        button->setStyleSheet(QString("border:2px solid rgb(128,128,128);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Unknown</p>") + tooltips[1]);
    } else {
        button->setStyleSheet(QString("border:2px solid rgb(0,0,0);") + button->styleSheet());
        button->setToolTip(tooltips[0] + QString("<br>Solid</p>") + tooltips[1]);
    }
}
