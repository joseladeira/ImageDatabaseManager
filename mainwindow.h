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
#include <QMap>
#include <QException>

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

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    QString dbFilename; // Name of the database;
    QSqlDatabase db;  // SQLdata-model-view variable
    QSqlTableModel *model; // model to the table
    const char tablename[20] = "imgTable";
    int current_row;  // current row selected on the tableview table.
    enum C {_file, _width, _height, _size, _date, _imagedata}; // column header enum
    QMap <C, int >  * col; // column header map    

    void connection (); // initialize DB connection
};

#endif // MAINWINDOW_H
