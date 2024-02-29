#include "histogramChartdisplay.h"
#include <algorithm>
#include <QMessageBox>
#include <string>

HistogramChartDisplay::HistogramChartDisplay(std::tuple<float,float> avgVar, QStandardItemModel* m, QModelIndexList* s, QWidget *parent)
    : QWidget{parent},
    ui(new Ui::chartDisplay),
    ui_paint(new Ui::histogram_paint),
    model(m), selectedColumns(s),
    avg(std::get<0>(avgVar)),var(std::get<1>(avgVar))
{
    ui->setupUi(this);
    ui_paint->setupUi(ui->painting);
    drawLineCheckbox = new QCheckBox("绘制正态分布曲线");
    drawLineCheckbox->setCheckable(false);//默认不能修改
    QVBoxLayout* optionLayout = new QVBoxLayout(ui->opitons);
    ui->opitons->setLayout(optionLayout);
    optionLayout->addWidget(drawLineCheckbox);
    connect(drawLineCheckbox,SIGNAL(clicked(bool)),this,SLOT(drawLineChanged(bool)));
    connect(ui_paint->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(barCountChanged(int)));
    for (int i = 1; i < model->rowCount(); ++i) {//添加到vector里
        datas.push_back( model->item(i, selectedColumns->first().column())->data(Qt::DisplayRole).toFloat());
    }
    std::sort(datas.begin(),datas.end(),[](float x,float y){return x>y;});
    paintImage();

}


void HistogramChartDisplay::paintImage(){
    QMap<QString, int>* histogramMap = new QMap<QString,int>();
    chartView = new QChartView(this);
    QChart *chart = new QChart();
    QBarSeries *barSeries = new QBarSeries(this);
    QBarSet *barSet = new QBarSet("数量");
    auto lineSeries = new QSplineSeries(this);
    barSeries->append(barSet);
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    QValueAxis *axisY = new QValueAxis();
    QValueAxis *axisL = new QValueAxis();

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


    QStringList keys;
    for (int i = 1; i < model->rowCount(); ++i) {
        QString value = model->item(i, selectedColumns->first().column())->data(Qt::DisplayRole).toString();
        (*histogramMap)[value]++;
    }
    if(histogramMap->size() > barCount){
        drawLineCheckbox->setCheckable(true);
        QMap<QString,int>* newHistogramMap = new QMap<QString,int>();
        float firstKey = datas[0];
        float lastKey = datas[datas.size()-1];
        float setp = (firstKey - lastKey) / float(barCount) , now = lastKey;
        auto rank =datas.size()-1;
        for (int i = 1 ; i < barCount ; i++) {
            std::stringstream ss;
            ss << '['<<std::fixed << std::setprecision(2) << now<<','<<' ';
            now += setp;
            ss << std::fixed << std::setprecision(2) << now<<')';
            std::string newKey = ss.str();
            (*newHistogramMap)[QString::fromStdString(newKey)] = 0;
            while(rank >= 0 && datas[rank] < now){
                (*newHistogramMap)[QString::fromStdString(newKey)]++;
                rank--;
            }
            lineSeries->append(i,density(now-setp/2));
            keys.append(QString::fromStdString(newKey));
        }
        std::stringstream ss;
        ss << '['<<std::fixed << std::setprecision(2) << now<<','<<' '<< std::fixed << std::setprecision(2) << firstKey<<']';
        std::string newKey = ss.str();
        (*newHistogramMap)[QString::fromStdString(newKey)]=1;
        while(rank > 0 && datas[rank] <= firstKey){
            (*newHistogramMap)[QString::fromStdString(newKey)]++;
            rank--;
        }
        lineSeries->append(barCount,density(now-setp/2));
        keys.append(QString::fromStdString(newKey));
        delete histogramMap;
        histogramMap = newHistogramMap;
    }

    int yTop = 0;

    for (const QString& value : ((keys.size() <= 0) ? (histogramMap->keys()):(keys))) {
        *barSet << (*histogramMap)[value];
        yTop = std::max(yTop, (*histogramMap)[value]);
    }
    chart->addSeries(barSeries);
    chart->setTitle("Histogram of " + model->item(0, selectedColumns->first().column())->data(Qt::DisplayRole).toString());
    chart->setAnimationOptions(QChart::SeriesAnimations);


    chart->addAxis(axisX, Qt::AlignBottom);
    axisX->append(keys);
    barSeries->attachAxis(axisX);
    axisY->setRange(0, yTop);
    axisL->setRange(0,1.2);
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisL);
    if(drawLine){
        chart->addAxis(axisL,Qt::AlignRight);
        chart->addSeries(lineSeries);
    }



    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setVisible(true);

    chartView->setChart(chart);


    delete histogramMap;
}

void HistogramChartDisplay::barCountChanged(int bc){
    barCount = bc;
    paintImage();
}

void HistogramChartDisplay::drawLineChanged(bool ifDrawLine){
    drawLine = ifDrawLine;
    paintImage();
}
