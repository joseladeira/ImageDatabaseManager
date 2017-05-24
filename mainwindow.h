#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QTableView>
#include <QMessageBox>
#include <QSqlTableModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void save_as();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void setLabelPicture (const QModelIndex qmi);

    void set_current_row (QModelIndex qmi) {current_row = (const int) qmi.row();}

private:
    Ui::MainWindow *ui;
    QString dbFilename;
    QSqlDatabase db;
    QSqlTableModel *model;
    const char tablename[20] = "imgTable";
    int current_row;


    void connection (); // DB connection

};

#endif // MAINWINDOW_H
