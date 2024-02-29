#include "mainpage.h"
#include "dimenreductiondisplay.h"
#include <QInputDialog>
MainPage::MainPage(QString& filePath,QWidget *parent)
    : QWidget{parent},
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    file = new QFile(filePath);
    QTextStream in(file);
    QList<QStringList> tableData;
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(nullptr, "false", "fail to open the file!");

    }


    while (!in.atEnd()) {
        QStringList rowData = in.readLine().split(",");
        tableData.append(rowData);
    }

    file->close();

    // 创建表格模型
    model = new QStandardItemModel(this);
    for (const QStringList& row : tableData) {
        QList<QStandardItem*> items;
        for (const QString& itemData : row){
            items.append(new QStandardItem(itemData));
        }
        model->appendRow(items);
    }



    ui->dataTable->setModel(model);
    connect(ui->dataTable->selectionModel(),SIGNAL(selectionChanged()),this,SLOT(tableSelectionChanged()));
    connect(ui->AvgVarButton,SIGNAL(clicked()),this,SLOT(computeAverageAndSquare()));
    connect(ui->SquareButton,SIGNAL(clicked()),this,SLOT(drawHistogram()));
    connect(ui->matrixButton,SIGNAL(clicked()),this,SLOT(drawMatrix()));
    connect(ui->Scatter2DButton,SIGNAL(clicked()),this,SLOT(drawScatter2D()));
    connect(ui->DimenReductionButton,SIGNAL(clicked()),this,SLOT(drawLowDimenChart()));
    connect(ui->ClusterButton,SIGNAL(clicked()),this,SLOT(cluster()));


}

void MainPage::tableSelectionChanged(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();
    // 获取选中的列数量
    selectedColNumber = selectedColumns.count();
}
void MainPage::cluster(){
    selectedColumns = ui->dataTable->selectionModel()->selectedColumns();// 获取选中的列数量
    selectedColNumber = selectedColumns.count();
    if(selectedColNumber <= 1){
        QMessageBox::information(this,"Warning","Please Select at least Three Columns!");
        return;
    }
    bool ok;
    int iterationCount = QInputDialog::getInt(nullptr, "聚类中心数", "请输入聚类中心数:", 1, 1, 5, 1, &ok);
         if (!ok) {
             // 用户取消了输入
             return ;
         }
    classList.clear();
    std::vector<std::vector<float>> inputMatrix;
    int count = model->rowCount();
    for(int i = 0 ; i < count; i++){
        std::vector<float> colVector;

        for(int j = 1 ; j < selectedColumns.count() ; j++){
            colVector.push_back(model->item(i, selectedColumns.at(j).column())->data(Qt::DisplayRole).toFloat());
        }
        inputMatrix.push_back(colVector);
    }
    auto result = std::get<1>(clusterKMeans(inputMatrix, iterationCount, maxIter));
    for(int& i :result){
        classList.push_back(i);
    }
    int colcount = model->columnCount();
    for (size_t i = 0; i < classList.size(); i++){
        // 在已有的QStandardItemModel的每一行中添加一个新的单元格
        QStandardItem* item = new QStandardItem(QString::number(classList[i]));
        model->setItem(i, colcount, item);

        // 设置单元格的颜色
        int value = classList[i];
        QColor color;
        if (value == 1) {
            color = QColor(Qt::red);
        } else if (value == 2) {
            color = QColor(Qt::yellow);
        } else if (value == 3) {
            color = QColor(Qt::blue);
        } else if (value == 4) {
            color = QColor(Qt::white);
        } else if (value == 5) {
            color = QColor(Qt::gray);
        } else if (value == 6) {
            color = QColor(QColorConstants::DarkCyan);
        } else if (value == 7) {
            color = QColor(QColorConstants::DarkYellow);
        }  else if (value == 8) {
            color = QColor(QColorConstants::Magenta);
        } else if (value == 9) {
            color = QColor(QColorConstants::DarkRed);
        }
        else {
            color = QColor(Qt::green);
        }
        for(int col = 0 ; col <= colcount; col++){
            model->item(i,col)->setData(color, Qt::BackgroundRole);
        }
    }
    DimenReductionDisplay* dimen = new DimenReductionDisplay(model, &selectedColumns, this, model->columnCount()-1);
    ui->ImageDisplay->addTab(dimen, "DimenReduced_from" + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(dimen);
    dimen->paintImage();
    DimenReductionDisplay* dimen3d = new DimenReductionDisplay(model, &selectedColumns, this, model->columnCount()-1);
    ui->ImageDisplay->addTab(dimen3d, "DimenReduced_from" + QString::number(selectedColNumber)+"columns");
    ui->ImageDisplay->setCurrentWidget(dimen3d);
    dimen3d->paint3DImage();


}
MainPage::~MainPage(){
    delete ui;
}
