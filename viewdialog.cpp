#include "viewdialog.h"
#include "ui_viewdialog.h"

ViewDialog::ViewDialog(const QPixmap pix, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewDialog)
{
    ui->setupUi(this);
    pixmap = pix;
    ui->label->setPixmap(pixmap);
    ui->label->adjustSize();
    this->adjustSize();


}

ViewDialog::~ViewDialog()
{
    delete ui;
}

