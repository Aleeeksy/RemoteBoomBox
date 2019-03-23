#ifndef BOOMBOXPRODUCERWIDGET_H
#define BOOMBOXPRODUCERWIDGET_H

#include <streamproducer.h>
#include <QDir>
#include <QFileDialog>
#include <QFuture>
#include <QPushButton>
#include <QStandardItemModel>
#include <QThread>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

/*!
 * \brief The BoomBoxProducerWidget class
 *
 * Klasa ta odpowiada za przygotowanie interfejsu
 * graficznego aplikacji stremującej audio
 * jak również za obsługę eventów.
 *
 * Interfej graficzny umożliwia:
 * - wczyatnie listy plików
 * - dowolne przełączanie się między nimi, co skutkuje zmianą streamowanego utworu
 * - zatrzymanie / wznowienie streamu
 *
 */

namespace Ui {
class BoomBoxProducerWidget;
}

class BoomBoxProducerWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BoomBoxProducerWidget(QWidget* parent = 0);
  ~BoomBoxProducerWidget();

 private slots:
  void on_addButton_clicked();
  void on_rowInTable_doubleClicked(const QModelIndex&);
  void on_nextTrackButton_clicked();
  void on_previousTrackButton_clicked();
  void on_pauseButton_clicked();

 private:
  Ui::BoomBoxProducerWidget* ui;
  QStandardItemModel* playListModel;
  StreamProducer* streamProducer;
  QThread* thread;

 private:
  void prepareButtons();
  void setButtonIcon(QString, QPushButton*);
  void startStream(QString);
};

#endif  // BOOMBOXPRODUCERWIDGET_H
