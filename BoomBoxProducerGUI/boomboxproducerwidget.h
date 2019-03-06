#ifndef BOOMBOXPRODUCERWIDGET_H
#define BOOMBOXPRODUCERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPushButton>

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

private:
    Ui::BoomBoxProducerWidget *ui;
    QStandardItemModel *playListModel;

private:
    void prepareButtons();
    void setButtonIcon(QString, QPushButton*);
    void startStream(QString);
};

#endif // BOOMBOXPRODUCERWIDGET_H
