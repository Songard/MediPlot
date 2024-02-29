#include "dimenreductiondisplay.h"
#include <algorithm>
#include <QMessageBox>
#include <string>
#include <QForwardRenderer>
#include <QStyle>
DimenReductionDisplay::DimenReductionDisplay(QStandardItemModel* m, QModelIndexList* s, QWidget *parent , int classlist)
    : QWidget{parent},
    ui(new Ui::chartDisplay),
    ui_options(new Ui::DimenOption),
    model(m), selectedColumns(s),
    classList(classlist)
{
    ui->setupUi(this);
    //ui_options->setupUi(ui->opitons);
    std::vector<std::vector<float>> inputMatrix;
    for(int i = 0 ; i < model->rowCount(); i++){
        std::vector<float> colVector;
        int count = selectedColumns->count();
        for(int j = 1 ; j < count ; j++){
            colVector.push_back(model->item(i, selectedColumns->at(j).column())->data(Qt::DisplayRole).toFloat());
        }
        inputMatrix.push_back(colVector);
    }
    lowDimenData = PCA(inputMatrix, 2);
    for(int i = 0 ; i < model->rowCount(); i++){
        col1.push_back(lowDimenData(i,0));
        col2.push_back(lowDimenData(i,1));
    }
    lowDimenData = PCA(inputMatrix, 3);
    //connect(ui_options->spinBox,SIGNAL(valueChanged(int)),this,SLOT(degreeChanged(int)));




}


void DimenReductionDisplay::paintImage(){

    chartView = new ChartView2D(this);
    QChart *chart = new QChart();
    chartView->setChart(chart);
    graph = std::vector<QScatterSeries>(20);
    QScatterSeries *scatterSeries = new QScatterSeries();
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();




    float col1min =col1[0], col1max =col1[0], col2min =col2[0], col2max =col1[0];
    if(classList ==1 ){
        for(size_t i = 0 ; i < col1.size() ; i++){
            graph[model->item(i, 1)->data(Qt::DisplayRole).toString()=="B" ? 0 : 1].append(QPointF(col1[i],col2[i]));
        }
    }else{
        for(size_t i = 0 ; i < col1.size() ; i++){
            graph[model->item(i, classList)->data(Qt::DisplayRole).toInt()].append(QPointF(col1[i],col2[i]));
        }
    }
    for(size_t i = 0 ; i < col1.size() ; i++){
        col1min = std::min(col1min, col1[i]);
        col2min = std::min(col2min, col2[i]);
        col1max = std::max(col1max, col1[i]);
        col2max = std::max(col2max, col2[i]);
    }

    for(int i = 0 ;graph[i].count() > 0 ; i++){
        graph[i].setMarkerSize(8);
        graph[i].attachAxis(axisX);
        graph[i].attachAxis(axisY);
        graph[i].setMarkerShape(QScatterSeries::MarkerShapeCircle);  // 设置点的形状为圆形
        graph[i].setMarkerSize(12);  // 设置点的大小为12
        graph[i].setBorderColor(Qt::black);  // 移除边框
        chartView->chart()->setAxisX(axisX, &graph[i]);
        chartView->chart()->setAxisY(axisY, &graph[i]);
        chart->addSeries(&graph[i]);
        int value = i;
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
        graph[i].setColor(color);  // 设置点的填充颜色为蓝色
    }
    // 添加坐标轴和设置名称
    axisX->setTitleText("col1");

    axisX->setRange(2 * col1min - 1.0* col1max , 2 * col1max -  1 * col1min );

    axisY->setTitleText("col2");

    axisY->setRange(2 * col2min - 1 *col2max , 2 * col2max - 1 * col2min);




    if(ui->chart->layout()){
        QWidget *oldChart = ui->chart->layout()->itemAt(0)->widget();
        ui->chart->layout()->removeWidget(oldChart);
        ui->chart->layout()->addWidget(chartView);
    }else{
        QVBoxLayout *layout = new QVBoxLayout(this);
        ui->chart->setLayout(layout);
        layout->addWidget(chartView);
    }



    chart->setTitle("Histogram of " + model->item(0, selectedColumns->first().column())->data(Qt::DisplayRole).toString());
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->legend()->setVisible(true);
    chartView->setMouseTracking(true);
    chartView->setToolTipDuration(0);

//


}
void DimenReductionDisplay::paint3DImage(){
    Q3DScatter* scatter = new Q3DScatter();
    graph3d = std::vector<QScatter3DSeries>(20);
    std::vector<QScatterDataArray> data(20);
    float col1min =lowDimenData(0,0), col1max =lowDimenData(0,0), col2min =lowDimenData(0,1), col2max = lowDimenData(0,1), col3min =lowDimenData(0,2), col3max = lowDimenData(0,2);
    if(classList == 1){
        for(size_t i = 0 ; i < col1.size() ; i++){
            data[model->item(i, 1)->data(Qt::DisplayRole).toString()=="B" ? 0 : 1] << QVector3D(lowDimenData(i, 0), lowDimenData(i, 1), lowDimenData(i, 2));
        }
    }else{
        for(size_t i = 0 ; i < col1.size() ; i++){
            data[model->item(i, classList)->data(Qt::DisplayRole).toInt()] << QVector3D(lowDimenData(i, 0), lowDimenData(i, 1), lowDimenData(i, 2));
        }
    }
    for(size_t i = 0 ; i < col1.size() ; i++){
        col1min = std::min(col1min, lowDimenData(i, 0));
        col2min = std::min(col2min, lowDimenData(i, 1));
        col3min = std::min(col3min, lowDimenData(i, 2));
        col1max = std::max(col1max, lowDimenData(i, 0));
        col2max = std::max(col2max, lowDimenData(i, 1));
        col3max = std::max(col3max, lowDimenData(i, 2));

    }

    for(int i = 0 ; i <20;i++){
        //QMessageBox::information(this,"",QString::number(graph3d[i].dataProxy()->itemCount()));
        graph3d[i].dataProxy()->addItems(data[i]);
        scatter->addSeries(&graph3d[i]);
        int value = i;
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
        graph3d[i].setBaseColor(color);
    }



    // 设置散点图的坐标轴范围
    scatter->axisX()->setRange(col1min, col1max);
    scatter->axisY()->setRange(col2min, col2max);
    scatter->axisZ()->setRange(col3min, col3max);


    if(ui->chart->layout()){
        QWidget *oldChart = ui->chart->layout()->itemAt(0)->widget();
        ui->chart->layout()->removeWidget(oldChart);
        delete oldChart;
        Qt3DExtras::Qt3DWindow *window = new  Qt3DExtras::Qt3DWindow();
        ui->chart->layout()->addWidget(QWidget::createWindowContainer(window,ui->chart));
    }else{
        QVBoxLayout *layout = new QVBoxLayout;
        Qt3DExtras::Qt3DWindow *window = new  Qt3DExtras::Qt3DWindow();
        layout->addWidget(QWidget::createWindowContainer(scatter,this));
        ui->chart->setLayout(layout);
    }
    scatter->show();



}

void DimenReductionDisplay::degreeChanged(int dg){
    if(dg == 2)
        paintImage();
    else{
        paint3DImage();
    }
}

void DimenReductionDisplay::drawLineChanged(bool ifDrawLine){
    paintImage();
}
