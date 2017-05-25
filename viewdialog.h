#ifndef VIEWDIALOG_H
#define VIEWDIALOG_H

#include <QDialog>

namespace Ui {
class ViewDialog;
}

class ViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewDialog(const QPixmap pix, QWidget *parent = 0);
    ~ViewDialog();

private:
    Ui::ViewDialog *ui;
    QPixmap pixmap;
};

#endif // VIEWDIALOG_H
