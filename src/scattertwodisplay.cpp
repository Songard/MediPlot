#include "scattertwodisplay.h"
#include <algorithm>
#include <QMessageBox>
#include <string>

ScatterTwoDisplay::ScatterTwoDisplay(std::tuple<float,float> avgVar, QStandardItemModel* m, QModelIndexList* s, QWidget *parent)
    : QWidget{parent},
    ui(new Ui::chartDisplay),
    ui_option(new Ui::scatter2D_options),
    model(m), selectedColumns(s)
{
    ui->setupUi(this);
    ui_option->setupUi(ui->opitons);
    for (int i = 1; i < model->rowCount(); ++i) {//添加到vector里
        col1.push_back( model->item(i, selectedColumns->first().column())->data(Qt::DisplayRole).toFloat());
        col2.push_back( model->item(i, selectedColumns->at(1).column())->data(Qt::DisplayRole).toFloat());
    }
    connect(ui_option->degreeSpinbox,SIGNAL(valueChanged(int)),this,SLOT(degreeChanged(int)));
    connect(ui_option->showLineCheckbox,SIGNAL(clicked(bool)),this,SLOT(drawLineChanged(bool)));
    paintImage();


}


void ScatterTwoDisplay::paintImage(){

    chartView = new ChartView(this);
    QChart *chart = new QChart();
    chartView->setChart(chart);
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->setMarkerSize(8);
    float col1min =col1[0], col1max =col1[0], col2min =col2[0], col2max =col1[0];
    for(size_t i = 0 ; i < col1.size() ; i++){
        scatterSeries->append(QPointF(col1[i],col2[i]));
        col1min = std::min(col1min, col1[i]);
        col2min = std::min(col2min, col2[i]);
        col1max = std::max(col1max, col1[i]);
        col2max = std::max(col2max, col2[i]);
    }

    // 添加坐标轴和设置名称
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("col1");
    chartView->chart()->setAxisX(axisX, scatterSeries);
    axisX->setRange(col1min * 0.75 , col1max * 1.25);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("col2");
    chartView->chart()->setAxisY(axisY, scatterSeries);
    axisY->setRange(col2min * 0.75, col2max * 1.25);

    if(drawLine){
        auto res = fitLeastSquareAndPR(col1, col2, degree);
        Eigen::VectorXf coes = std::get<0>(res);
        QSplineSeries* lineseries = new QSplineSeries(this);
        for(int i = 0 ; i <= 30 ; i++){
            float x = (i * col1min * 0.75 + (30 - i) * col1max * 1.25) / 30.0f;
            float y = 0, xcount = 1;
            for(int currentDegree = 0; currentDegree < coes.size(); currentDegree++){
                y += xcount * coes[currentDegree];
                xcount *= x;
            }
            lineseries->append(QPointF(x,y));
        }
        chartView->chart()->addSeries(lineseries);
        chartView->chart()->setAxisX(axisX, lineseries);
        chartView->chart()->setAxisY(axisY, lineseries);
        QString tooltip = "p值：" + QString::number(std::get<1>(res)) + ", r方:" + QString::number(std::get<2>(res));
        ui_option->lineEdit->setText(tooltip);
    }

    if(ui->chart->layout()){
        QWidget *oldChart = ui->chart->layout()->itemAt(0)->widget();
        ui->chart->layout()->removeWidget(oldChart);
        delete oldChart;
        ui->chart->layout()->addWidget(chartView);
    }else{
        QVBoxLayout *layout = new QVBoxLayout(this);
        ui->chart->setLayout(layout);
        layout->addWidget(chartView);
    }




    chart->addSeries(scatterSeries);
    chart->setTitle("Histogram of " + model->item(0, selectedColumns->first().column())->data(Qt::DisplayRole).toString());
    chart->setAnimationOptions(QChart::SeriesAnimations);


    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);



    chart->legend()->setVisible(true);
    chartView->setMouseTracking(true);
    chartView->setToolTipDuration(0);

//


}

void ScatterTwoDisplay::degreeChanged(int dg){
    degree = dg;
    paintImage();
}

void ScatterTwoDisplay::drawLineChanged(bool ifDrawLine){
    drawLine = ifDrawLine;
    paintImage();
}
