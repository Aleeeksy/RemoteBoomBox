#include "boomboxreceiver.h"
#include "ui_boomboxreceiver.h"

BoomBoxReceiver::BoomBoxReceiver(QWidget* parent)
    : QWidget(parent), ui(new Ui::BoomBoxReceiver) {
  qRegisterMetaType<std::tuple<float*, float*>>("<std::tuple<float*, float*>>");
  ui->setupUi(this);
  ui->inputPlainText->setPlainText("udp://127.0.0.1:2300");
  streamReceiver = new StreamReceiver();
  dataForListView = new QStandardItemModel(this);
  prepare();
}

void BoomBoxReceiver::prepare() {
  initVisualizationTypeList();

  thread = new QThread();
  streamReceiver->moveToThread(thread);
  connect(thread, SIGNAL(started()), streamReceiver,
          SLOT(startRecevingStream()));
  connect(streamReceiver, SIGNAL(transferData(std::tuple<float*, float*>)),
          this, SLOT(transferData(std::tuple<float*, float*>)));
}

void BoomBoxReceiver::initVisualizationTypeList() {
  mapOfTypes["SimpleType"] = new SimpleVisualizationType(
      streamReceiver->getSamplesPerChannel(), ui->graphicsView);

  for (auto& type : mapOfTypes) {
    dataForListView->appendRow(new QStandardItem(type.first));
  }
  ui->visualizationTypelistView->setModel(dataForListView);

  ui->visualizationTypelistView->setCurrentIndex(dataForListView->index(0, 0));
}

BoomBoxReceiver::~BoomBoxReceiver() {
  delete ui;
}

void BoomBoxReceiver::on_connectButton_clicked() {
  streamReceiver->prepareToRecevingStream(
      ui->inputPlainText->toPlainText().toStdString());

  thread->start();
}

void BoomBoxReceiver::transferData(std::tuple<float*, float*> channelsData) {
  mapOfTypes[dataForListView
                 ->index(ui->visualizationTypelistView->currentIndex().row(), 0)
                 .data()
                 .toString()]
      ->draw(channelsData);
}
