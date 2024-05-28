#include "compounddialog.h"
#include "ui_compounddialog.h"
#include "compound.h"

CompoundDialog::CompoundDialog(Compound* compound, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompoundDialog)
{
    ui->setupUi(this);
    connect(ui->exitButton, &QAbstractButton::clicked, this, &QDialog::accept);
    ui->commonName->setText(compound->commonName);
    ui->formulaText->setText(compound->formula);
    ui->descriptionText->setText(compound->description);
    ui->molWeightText->setText(compound->molecularWeight);
}

CompoundDialog::~CompoundDialog()
{
    delete ui;
}
