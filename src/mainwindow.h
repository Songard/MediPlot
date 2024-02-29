#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <mainpage.h>
#include <QMainWindow>
#include <QTableWidget>
#include <QFileDialog>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addpage();

private:
    Ui::MainWindow *ui;
    QVector<MainPage*> Pages;
};
#endif // MAINWINDOW_H
