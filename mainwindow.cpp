#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ui
    ui->setupUi(this);
    this->setWindowTitle("Image Database manager");

    connection();

    model = new QSqlTableModel(this);
    model->setTable(tablename);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);


    model->setHeaderData(0, Qt::Horizontal, tr("File"));
    model->setHeaderData(1, Qt::Horizontal, tr("Resolution"));
    model->setHeaderData(2, Qt::Horizontal, tr("Size"));
    model->setHeaderData(3, Qt::Horizontal, tr("Date"));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(4);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->resizeColumnsToContents();

    model->select();

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(setLabelPicture(QModelIndex) ));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), this, SLOT(setLabelPicture(QModelIndex) ));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(set_current_row(QModelIndex)));
    connect(ui->tableView, SIGNAL(activated(QModelIndex)), this, SLOT(set_current_row(QModelIndex)));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)),this, SLOT(save_as()));



}

// DB connection
void MainWindow::connection() {

    // default db initializing
    dbFilename = "database.db";
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbFilename );
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This example needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }
    QSqlQuery query = QSqlQuery( db );
    query.prepare("CREATE TABLE IF NOT EXISTS imgTable "
                  "( filename varchar(100) PRIMARY KEY, resolution varchar(30), "
                  "size INT, datetime DATETIME, imagedata BLOB )" );
    //query.bindValue( ":tablename", tablename );
    if( !query.exec() )
        qDebug() << "Error creating table:\n" << query.lastError();
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

/*
 * add images to DB
 */
void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.png *.ps)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QStringList fileNames;
    if (dialog.exec()) fileNames = dialog.selectedFiles();

    for (QString filename : fileNames) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return;
        QByteArray inByteArray = file.readAll();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData( inByteArray );
        QFileInfo fi(filename);
        QString resolution = QString::number(outPixmap.width())+"x"
                +QString::number(outPixmap.height());

        //( "INSERT INTO imgTable (filename, resolution, size, datetime, imagedata) "
        //  "VALUES (:filename, :resolution, :size, :datetime, :imagedata)" );

        int row = model->rowCount(QModelIndex());
        model->insertRow(row, QModelIndex());
        model->setData(model->index(row,0,QModelIndex()),fi.fileName(), Qt::EditRole);
        model->setData(model->index(row,1,QModelIndex()),resolution, Qt::EditRole);
        model->setData(model->index(row,2,QModelIndex()),fi.size(), Qt::EditRole);
        model->setData(model->index(row,3,QModelIndex()),fi.created(), Qt::EditRole);
        model->setData(model->index(row,4,QModelIndex()),inByteArray, Qt::EditRole);

    }
    ui->tableView->resizeColumnsToContents();
}

/*
 * Remove image from DB
 */
void MainWindow::on_pushButton_2_clicked()
{    
    QModelIndexList selectedLines = ui->tableView->selectionModel()->selectedIndexes();
    for (QModelIndex line : selectedLines) {
        model->removeRow(line.row());
    }
}



// commit changes to DB
void MainWindow::on_pushButton_5_clicked()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }

    ui->tableView->resizeColumnsToContents();
    model->select();
}

// Undo all changes made to DB
void MainWindow::on_pushButton_6_clicked()
{
    model->revertAll();
    ui->tableView->resizeColumnsToContents();
    model->select();
}

// preview image box
void MainWindow::setLabelPicture (const QModelIndex  qmi) {

    QSqlRecord rec = model->record( (const int) qmi.row());
    QPixmap inPixmap = QPixmap();
    inPixmap.loadFromData( rec.value(4).toByteArray() );
    ui->label->setPixmap(inPixmap);

}

/*
 * Save as - image from DB
 */
void MainWindow::save_as()
{

    QSqlRecord rec = model->record( current_row );
    QString filename = rec.value(0).toString(); // hint filename
    QByteArray bArray = rec.value(4).toByteArray();


    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    filename = dialog.getSaveFileName(this, tr("Save File"),
                                              filename, tr("Images (*.png *.xpm *.jpg *.ps *.bmp)"));


    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) return;
    QDataStream out(&file);
    out << bArray;
}
