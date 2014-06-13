#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QRectF>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
private:
    void parse_text();
    void parse_text_simple();
private slots:
    void on_textEdit_textChanged();

    void on_actionSave_triggered();

    void on_actionToggle_editor_triggered();

    void on_actionAbout_Qt5_triggered();

    void on_actionOpen_triggered();

    void on_actionToggle_Console_triggered();

private:
    Ui::MainWindow *ui;
    QFile file;

    std::vector<QRectF> rect;
};

#endif // MAINWINDOW_H
