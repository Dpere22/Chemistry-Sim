#ifndef PERIODICTABLEDIALOG_H
#define PERIODICTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class PeriodicTableDialog;
}

class PeriodicTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeriodicTableDialog(QWidget *parent = nullptr);
    ~PeriodicTableDialog();

    /// @brief Sets all the stylesheets of the periodic table, the minimum size of each box, and connects the buttons to emit their atomic number when clicked
    void initializePeriodicTable(Ui::PeriodicTableDialog *ui);

    ///@brief Initializes stylesheets of a series in the periodic table
    void initializeSeries(QPushButton *button, int atomicNumber);

    ///@brief Initializes stylesheets of a phase of matter in the periodic table
    void initializePhase(QPushButton *button, int atomicNumber);
signals:
    ///@brief Signal containing the atomic number of the element clicked on the periodic table
    void periodicTableClicked(QStringList atomicInformation);

private:
    Ui::PeriodicTableDialog *ui;
};

#endif // PERIODICTABLEDIALOG_H
