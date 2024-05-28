///Goobers Team
/// Used to display the information about a compound as a popup
#ifndef COMPOUNDDIALOG_H
#define COMPOUNDDIALOG_H

#include <QDialog>
#include "compound.h"

namespace Ui {
class CompoundDialog;
}

class CompoundDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompoundDialog(Compound* compound, QWidget *parent = nullptr);
    ~CompoundDialog();

private:
    Ui::CompoundDialog *ui;
};

#endif // COMPOUNDDIALOG_H
