#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QString>
#include <QTextStream>
#include <QDialog>
#include <QFileDialog>

#include <QRegExp>
#include <QRegExpValidator>
#include <QRectF>
#include <QListView>
#include <QFile>
#include "task.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    file.setFileName(QString("tempFile.plgn"));
    file.open(QIODevice::ReadWrite );


    QTextStream stream(&file);
    ui->textEdit->setText(stream.readAll());
    stream.seek(0);


}

MainWindow::~MainWindow()
{
    file.close();
    delete ui;
}

void MainWindow::on_textEdit_textChanged()
{
  // parse_text();
    parse_text_simple();
}

void MainWindow::parse_text()
{
    QString text=ui->textEdit->toPlainText();
    QTextStream stream(&text);

    QRegExp reg_double(QString("\d+(\\.\d+)?$"));

    QString line=stream.readLine();
    while (!line.isNull())
    {
        int numbers[4];
        if (reg_double.indexIn(text)!=-1)
            line=stream.readLine();
    }


}
void MainWindow::parse_text_simple()
{
    QString text=ui->textEdit->toPlainText();
    QTextStream stream(&text);

    double a,b,c,d;

    ui->listWidget->clear();
    rect.clear();
    QString line=stream.readLine();
    while (!line.isNull())
    {
        QTextStream line_stream(&line);
        line_stream>>a>>b>>c>>d;

        QRectF tmp_rect(QPointF(a,b),QPointF(c,d));
        rect.push_back(tmp_rect);
        QString lbl("Rect: (");

        double x1,y1,x2,y2;
        tmp_rect.getCoords(&x1,&y1,&x2,&y2);

        lbl.append(QString::number(x1)).append(QString(", ")).
                append(QString::number(y1)).append(QString(")  (")).
                append(QString::number(x2)).append(QString(", ")).
                append(QString::number(y2)).append(QString(")"));
        ui->listWidget->addItem(lbl);

        line=stream.readLine();
    }

    if (file.isOpen())
    {
        file.resize(0);
        QTextStream stream_temp_file(&file);

        QString all_data(ui->textEdit->toPlainText());

        stream_temp_file<<all_data;
    }


    ui->widget->slot_set_rectVector(this->rect);

    std::vector<C2DRect> crect;
    for(int i=0;i<rect.size();i++)
    {
        crect.push_back(QRect_to_C2DRect(rect[i]));
    }

    find_perimeter_polygon(crect);
    int breakPoint=0;
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                 QDir::currentPath(),
                                                 tr(" Result (*.plgn)"));

    QFile saveFile(fileName);
    saveFile.open(QIODevice::WriteOnly);

    QTextStream stream(&saveFile);
    stream<<ui->textEdit->toPlainText();


    saveFile.close();

}


void MainWindow::on_actionToggle_editor_triggered()
{
    if (ui->dockWidget_editor->isHidden())
        ui->dockWidget_editor->show();
    else
        ui->dockWidget_editor->hide();

}

void MainWindow::on_actionAbout_Qt5_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 QDir::currentPath(),
                                                 tr(" Result (*.plgn)"));
    QFile openFile(fileName);
    openFile.open(QIODevice::ReadOnly);

    QTextStream stream(&openFile);

//    double a,b,c,d;
//    ui->listWidget->clear();
//    rect.clear();
//    QString line=stream.readLine();
//    while (!line.isNull())
//    {
//        QTextStream line_stream(&line);
//        line_stream>>a>>b>>c>>d;

//        QRectF tmp_rect(QPointF(a,b),QPointF(c,d));
//        rect.push_back(tmp_rect);
//        QString lbl("Rect: (");

//        double x1,y1,x2,y2;
//        tmp_rect.getCoords(&x1,&y1,&x2,&y2);

//        lbl.append(QString::number(x1)).append(QString(", ")).
//                append(QString::number(y1)).append(QString(")  (")).
//                append(QString::number(x2)).append(QString(", ")).
//                append(QString::number(y2)).append(QString(")"));
//        ui->listWidget->addItem(lbl);

//        line=stream.readLine();
//    }

    ui->textEdit->setText(stream.readAll());

    openFile.close();

}

void MainWindow::on_actionToggle_Console_triggered()
{
    if (ui->dockWidget_console->isHidden())
        ui->dockWidget_console->show();
    else
        ui->dockWidget_console->hide();
}
