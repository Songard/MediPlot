#include "matrixdisplay.h"
#include <QPainter>
#include <QPushButton>
#include <QFont>
#include <algorithm>
#include <QComboBox>

//颜色转换
QColor getColor(int hue, float brightness){
    if(!(brightness <= 1 && brightness >= -1)){
        return QColorConstants::White;
    }
    QColor color;
    if(hue >= 0 && hue <=360){
        if(brightness>=0){
            color = QColor::fromHsl(hue, 255, 255 - 120 * brightness);
        }else{
            color = QColor::fromHsl((180 + hue) % 360, 255, 255 + 120 * brightness);
        }
    }else{
        if(brightness >=0){
            color = QColor::fromHsl(120 - 120 * brightness, 255,255 - 120 * brightness);
        }else{
            color = QColor::fromHsl(120 - 120 * brightness, 255,255 + 120 * brightness);
        }
    }
    return color;
}
class GridWidget : public QWidget
{
private:
    int hue;
    int numCells;
    static const int topicSize = 20;
    QStringList topicList;
    Eigen::MatrixXf matrix;
public:
    explicit GridWidget(int cellnum, QStringList topics, Eigen::MatrixXf _dataMatrix,  int h = -1, QWidget *parent = nullptr) : QWidget(parent),hue(h),numCells(cellnum),topicList(topics), matrix(_dataMatrix) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        float gridSize = (width() - topicSize)/numCells;  // 网格大小


        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", std::max(1,(int)(100/numCells))));


        // 填充网格
        for (int row = 0; row < numCells; ++row)
        {
            for (int col = 0; col < numCells; ++col)
            {
                int x = col * gridSize + topicSize;
                int y = row * gridSize + topicSize;
                painter.fillRect(x + 1, y + 1, gridSize - 1, gridSize - 1, getColor(hue, matrix(row, col)));
                painter.setPen(Qt::black);
                QString label = QString::number(round(matrix(row,col) * 100) / 100.0);
                int labelWidth = painter.fontMetrics().averageCharWidth()*label.length();
                int labelHeight = painter.fontMetrics().height();

                x = col * gridSize + gridSize / 2 - labelWidth / 2 + topicSize;
                y = row * gridSize + gridSize / 2 + labelHeight / 2 + topicSize;
                painter.drawText(x, y, label);
            }
        }
        // 绘制垂直线
        for (int col = 0; col <= numCells; ++col)
        {
            int x = col * gridSize + topicSize;
            painter.drawLine(x, 0, x, height());
        }

        // 绘制水平线
        for (int row = 0; row <= numCells; ++row)
        {
            int y = row * gridSize + topicSize;
            painter.drawLine(0, y, width(), y);
        }

        //绘制标题
        int topicNum = 0;
        painter.setFont(QFont("Arial", std::max(1,(int)(50/numCells))));
        for(auto col:topicList){
            int x = topicNum * gridSize + gridSize / 2 - painter.fontMetrics().averageCharWidth()*col.length() / 2 + topicSize;;
            topicNum++;
            painter.drawText(x, 15, col);
        }
        topicNum = 0;
        painter.rotate(-90);
        for(auto col:topicList){
            int x = topicNum * gridSize + gridSize / 2 + painter.fontMetrics().averageCharWidth()*col.length() / 2 + topicSize;;
            topicNum++;
            painter.drawText(-1*x, 15, col);
            //QMessageBox::information(this,"",QString::number(topicNum++));
        }
    }

    //保持正方形
    void resizeEvent(QResizeEvent *event) override
    {
        QSize newSize = event->size();
        int squareSize = qMin(newSize.width(), newSize.height());
        resize(squareSize, squareSize);
    }

};


//bar
class ColorScaleWidget : public QWidget
{
private:
    int hue;//在0-360范围内设置为固定色相，否则默认为彩色
    bool isCov;
public:
    explicit ColorScaleWidget(bool iscov, int h = -1, QWidget *parent = nullptr) : QWidget(parent), hue(h), isCov(iscov) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);

        int scaleWidth = 30; // 刻度宽度
        int scaleHeight = height() - 10; // 刻度高度
        int numSteps; // 刻度数量
        int x = width() - scaleWidth,y;
        if (isCov){
            numSteps = 120 ;
            for (int i = 0; i <= numSteps; ++i){
                y = height() - 10 - i * 4;
                float rulerNumber = (float)(i + i - numSteps) / numSteps;
                painter.fillRect(x, y, scaleWidth, 5, getColor(hue, rulerNumber));

                painter.setPen(Qt::black);
                if(i%5 == 0){
                    if(i + i > numSteps){
                        painter.drawText(x - 30, y + 4, tr("1e") + QString::number(i / 5 - 19));
                    }else if(i+i == numSteps){
                        painter.drawText(x - 25, y + 4, tr("0"));
                    }else{
                        painter.drawText(x - 33, y + 4, tr("-1e") + QString::number(5 - i / 5));
                    }
                }
            }

        }else{
            numSteps = 100;
            for (int i = 0; i <= numSteps; ++i){

                y = height() - 10 - i * 4;

                float rulerNumber = (float)(i + i - numSteps) / numSteps;
                painter.fillRect(x, y, scaleWidth, 5, getColor(hue, rulerNumber));

                if(i%5 ==0){
                    painter.drawText(x - 25, y + 5, QString::number(rulerNumber));
                }
            }
        }

    }
};

MatrixDisplay::MatrixDisplay(QStandardItemModel* m, QModelIndexList* s, QWidget *parent)
    : QWidget{parent},
    ui(new Ui::chartDisplay()),
    ui_paint(new Ui::MatrixColorBar),
    model(m), selectedColumns(s)
{
    ui->setupUi(this);
    ui_paint->setupUi(ui->painting);
    ui_paint->horizontalSlider->setVisible(false);
    QComboBox* comboBox = new QComboBox(this);
    comboBox->addItem("相关系数矩阵");
    comboBox->addItem("协方差矩阵");
    comboBox->setCurrentIndex(1);
    ui->opitons->setLayout(new QVBoxLayout);
    ui->opitons->layout()->addWidget(comboBox);
    //设置topiclist
    for(auto col:(*selectedColumns)){
        topicList.append(model->item(0,col.column())->data(Qt::DisplayRole).toString());
    }

    //计算协方差矩阵
    std::vector<std::vector<float>> inputMatrix;
    for(int i = 0 ; i < selectedColumns->count(); i++){
        std::vector<float> colVector;
        int count = model->rowCount();
        for(int j = 1 ; j < count ; j++){
            colVector.push_back(model->item(j, selectedColumns->at(i).column())->data(Qt::DisplayRole).toFloat());
        }
        inputMatrix.push_back(colVector);
    }
    std::vector<float> vars;
    for (auto vec : inputMatrix)
    {
        auto avgVar = getAvgVar(vec);
        vars.push_back(std::get<1>(avgVar));
    }

    covMatrix = getCovariance(inputMatrix);
    relMatrix = getPearsonCorr(covMatrix, vars);
    for (int i = 0; i < covMatrix.rows(); ++i) {
        for (int j = 0; j < covMatrix.cols(); ++j) {
            if(covMatrix(i, j) > 100000.0f){
                covMatrix(i, j) = 1.0f;
            }else if(covMatrix(i, j) > 0.0f){
                covMatrix(i, j) = (7.0f +log10(covMatrix(i, j))) / 12.0f;
            }else if(covMatrix(i, j) < -100000.0f){
                covMatrix(i, j) = -1.0f;
            }else if(covMatrix(i, j) < 0.0f){
                covMatrix(i, j) = (7.0f +log10(-1.0f * covMatrix(i, j))) / -12.0f;
            }
        }
    }
    connect(ui_paint->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(paintMatrix(int)));
    connect(ui_paint->checkBox,SIGNAL(clicked(bool)),this,SLOT(styleChanged(bool)));
    connect(comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(matrixChanged(int)));
    paintMatrix(-1);


}



void MatrixDisplay::paintMatrix(int h = -1){
    GridWidget* grid;
    ColorScaleWidget * colorScale;
    if(isCov){
        grid = new GridWidget(selectedColumns->count(), topicList, covMatrix, ui_paint->checkBox->isChecked() ? h : -1);
        colorScale = new ColorScaleWidget(true, ui_paint->checkBox->isChecked() ? h : -1);
    }else{
        grid = new GridWidget(selectedColumns->count(), topicList, relMatrix, ui_paint->checkBox->isChecked() ? h : -1);
        colorScale = new ColorScaleWidget(false, ui_paint->checkBox->isChecked() ? h : -1);
    }
    if(ui->chart->layout()){
        QWidget *oldChart = ui->chart->layout()->itemAt(1)->widget();
        ui->chart->layout()->removeWidget(oldChart);
        delete oldChart;
        oldChart = ui->chart->layout()->itemAt(0)->widget();
        ui->chart->layout()->removeWidget(oldChart);
        delete oldChart;
    }else{
        QHBoxLayout *layout = new QHBoxLayout(this);
        ui->chart->setLayout(layout);
    }
    ui->chart->layout()->addWidget(grid);
    grid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->chart->layout()->addWidget(colorScale);
    colorScale->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    colorScale->setFixedSize(70,colorScale->height());

}

void MatrixDisplay::matrixChanged(int index){
    isCov = (index == 1);
    paintMatrix(ui_paint->checkBox->isChecked() ? ui_paint->horizontalSlider->value() : -1);
}

void MatrixDisplay::styleChanged(bool isDefault){
    ui_paint->checkBox->setChecked(isDefault);
    paintMatrix(isDefault ? ui_paint->horizontalSlider->value() : -1);
}
