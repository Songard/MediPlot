#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PagesTable->removeTab(1);
    connect(ui->OpenNewFileButton,SIGNAL(clicked()),this,SLOT(addpage()));
    connect(ui->actionImport_Data,SIGNAL(triggered(bool)),this,SLOT(addpage()));
}

void MainWindow::addpage(){
    QString filePath = QFileDialog::getOpenFileName(this, "请选择CSV文件", "", "CSV 文件 (*.csv)");
    if (filePath.isEmpty()){
        return;
    }
    MainPage *newpage = new MainPage(filePath);
    Pages.push_back(newpage);
    ui->PagesTable->addTab(Pages.back(),"page"+QString::number(Pages.length()));
    ui->PagesTable->setCurrentWidget(newpage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

