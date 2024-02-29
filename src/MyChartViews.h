#ifndef MYCHARTVIEWS_H
#define MYCHARTVIEWS_H
#include <QChartView>
#include <QScatterSeries>
#include <QToolTip>
#include <QtDataVisualization>
#include <QScatter3DSeries>


class ChartView : public QChartView {
    Q_OBJECT
public:
    ChartView(QWidget *parent = nullptr) : QChartView(parent) {}
    float detectrange = 20;

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        QPointF mousePoint = chart()->mapToValue(event->pos());
        qreal minDistance = qInf();
        QPointF closestPoint;

        // 查找距离最近的散点
        for (QAbstractSeries *series : chart()->series()) {
            QScatterSeries *scatterSeries = qobject_cast<QScatterSeries *>(series);
            if (scatterSeries) {
                for (const QPointF &scatterPoint : scatterSeries->points()) {
                    qreal distance = qPow(scatterPoint.x() - mousePoint.x(), 2) +
                                     qPow(scatterPoint.y() - mousePoint.y(), 2);
                    if (distance < minDistance) {
                        minDistance = distance;
                        closestPoint = scatterPoint;
                    }
                }
            }
        }

        if (minDistance < 20) {
            QString tooltip = QString("Coordinates: (%1, %2)").arg(closestPoint.x(), 0, 'f', 2).arg(closestPoint.y(), 0, 'f', 2);
            QToolTip::showText(event->globalPos(), tooltip, this);
        } else {
            QToolTip::hideText();
        }
    }
};

class ChartView2D : public QChartView {
    Q_OBJECT
public:
    ChartView2D(QWidget *parent = nullptr) : QChartView(parent) {}
    float detectrange = 256;

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        QPointF mousePoint = chart()->mapToValue(event->pos());
        qreal minDistance = qInf();
        QPointF closestPoint;

        // 查找距离最近的散点
        for (QAbstractSeries *series : chart()->series()) {
            QScatterSeries *scatterSeries = qobject_cast<QScatterSeries *>(series);
            if (scatterSeries) {
                for (const QPointF &scatterPoint : scatterSeries->points()) {
                    qreal distance = qPow(scatterPoint.x() - mousePoint.x(), 2) +
                                     qPow(scatterPoint.y() - mousePoint.y(), 2);
                    if (distance < minDistance) {
                        minDistance = distance;
                        closestPoint = scatterPoint;
                    }
                }
            }
        }

        if (minDistance < detectrange) {
            QString tooltip = QString("Coordinates: (%1, %2)").arg(closestPoint.x(), 0, 'f', 2).arg(closestPoint.y(), 0, 'f', 2);
            QToolTip::showText(event->globalPos(), tooltip, this);
        } else {
            QToolTip::hideText();
        }
    }
};
#endif // MYCHARTVIEWS_H
