#ifndef BOOMBOXPRODUCERWIDGET_H
#define BOOMBOXPRODUCERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <streamproducer.h>

namespace Ui {
class BoomBoxProducerWidget;
}

class BoomBoxProducerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoomBoxProducerWidget(QWidget *parent = 0);
    ~BoomBoxProducerWidget();

private slots:
    void on_addButton_clicked();
    void on_rowInTable_doubleClicked(const QModelIndex &);
    void on_nextTrackButton_clicked();
    void on_previousTrackButton_clicked();
    void on_pauseButton_clicked();

private:
    Ui::BoomBoxProducerWidget *ui;
    QStandardItemModel *playListModel;
    StreamProducer *streamProducer;
    QFuture<void> future;
    QThread *thread;

private:
    void prepareButtons();
    void setButtonIcon(QString, QPushButton*);
    void startStream(QString);
};

#endif // BOOMBOXPRODUCERWIDGET_H
