#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSoundEffect>
#include "bounceengine.h"
#include "compounddialog.h"
#include "model.h"
#include "periodictabledialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Chemistry;
}
QT_END_NAMESPACE

/// @brief Main display window containing interface objects
/// @authors Goobers team
class MainWindow : public QMainWindow
{
    Q_OBJECT

    /// @brief Reference to main window ui
    Ui::Chemistry *ui;

    /// @brief Reference to model
    Model *model;

public:
    /// @brief Constructs with reference to Model
    MainWindow(Model &, BounceEngine &, QWidget *parent = nullptr);

    /// @brief Destructor
    ~MainWindow();

public slots:
    void showMolecule(Compound*);
    void addNewlyDiscoveredCompound(Compound*);
    void acceptPeriodicTableClicked(QStringList atomInformation);
    void testSlot(QStringList list);
    void failBond(QString failBonds);

signals:

    QString initDisplay();
private:
    CompoundDialog *compoundDialog;
    PeriodicTableDialog *PTDialog;
    BounceEngine *engine;
    QSoundEffect goodSound;
    QSoundEffect badSound;
};

#endif // MAINWINDOW_H
