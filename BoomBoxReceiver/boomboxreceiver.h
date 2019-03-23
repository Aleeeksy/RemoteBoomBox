#ifndef BOOMBOXRECEIVER_H
#define BOOMBOXRECEIVER_H

#include <streamreceiver.h>
#include <QFuture>
#include <QStandardItemModel>
#include <QThread>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <tuple>

/*!
 * \brief The BoomBoxReceiver class
 *
 * Klasa ta odpowiada za przygotowanie interfejsu
 * graficznego aplikacji odbierającej stream audio
 * oraz ubsługę akcji użytkownika.
 *
 * Interfej graficzny umożliwia
 * - podanie adresu URL na którym na nasłuchiwać(protokół UDP)
 * - wybranie typu wizualizacji
 *
 */
namespace Ui {
class BoomBoxReceiver;
}

class BoomBoxReceiver : public QWidget {
  Q_OBJECT

 public:
  explicit BoomBoxReceiver(QWidget* parent = 0);
  ~BoomBoxReceiver();

 private:
  void prepare();
  void initVisualizationTypeList();

 private slots:
  void on_connectButton_clicked();
  void transferData(std::tuple<float*, float*>);

 private:
  Ui::BoomBoxReceiver* ui;
  std::map<QString, VisualizationType*> mapOfTypes;
  QStandardItemModel* dataForListView;
  StreamReceiver* streamReceiver;
  QThread* thread;
  QThread* drawThread;
  VisualizationType* visualizationType1;
  std::list<int> channel1;
  std::list<int> channel2;
};

#endif  // BOOMBOXRECEIVER_H
