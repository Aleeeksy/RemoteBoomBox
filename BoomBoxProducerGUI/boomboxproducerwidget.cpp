#include "boomboxproducerwidget.h"
#include "ui_boomboxproducerwidget.h"

BoomBoxProducerWidget::BoomBoxProducerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::BoomBoxProducerWidget) {
    ui->setupUi(this);
    playListModel = new QStandardItemModel(this);
    playListModel->setHorizontalHeaderLabels(QStringList("Filename"));
    streamProducer = new StreamProducer();

    prepareButtons();

    ui->playlistView->setModel(playListModel);
    ui->playlistView->hideColumn(1);
    ui->playlistView->verticalHeader()->setVisible(true);
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->playlistView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(on_rowInTable_doubleClicked(const QModelIndex &)));
}

BoomBoxProducerWidget::~BoomBoxProducerWidget() {
    delete ui;
}

void BoomBoxProducerWidget::prepareButtons() {
    setButtonIcon(":/icons/icons/add-button.png", ui->addButton);
    setButtonIcon(":/icons/icons/play-button.png", ui->playButton);
    setButtonIcon(":/icons/icons/pause-button.png", ui->pauseButton);
    setButtonIcon(":/icons/icons/stop-button.png", ui->stopButton);
    setButtonIcon(":/icons/icons/previous-track-button.png", ui->previousTrackButton);
    setButtonIcon(":/icons/icons/next-track-button.png", ui->nextTrackButton);
}

void BoomBoxProducerWidget::setButtonIcon(QString iconPath, QPushButton* button) {
    QPixmap pixmap(iconPath);
    QIcon buttonIcon(pixmap);
    button->setIcon(buttonIcon);
    button->setIconSize(button->size());
}

void BoomBoxProducerWidget::on_addButton_clicked()
{
    // Using the file selection dialog to make multiple selections of mp3 files
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), QString(), tr("Audio Files (*.mp3)"));

    // Next, set the data names and file paths
    // into the playlist and table displaying the playlist
    foreach (QString filePath, files) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        playListModel->appendRow(items);
    }
}

void BoomBoxProducerWidget::on_rowInTable_doubleClicked(const QModelIndex &index) {
    startStream(playListModel->index(index.row(), 1).data().toString());
}



void BoomBoxProducerWidget::on_nextTrackButton_clicked() {
    QModelIndex currentIndex = ui->playlistView->currentIndex();
    QModelIndex newIndex;
    if (currentIndex.row() + 1 >= ui->playlistView->verticalHeader()->count()) {
        newIndex = ui->playlistView->model()->index(0, 0);
        ui->playlistView->setCurrentIndex(newIndex);
        ui->playlistView->scrollTo(newIndex);
    } else {
        newIndex = ui->playlistView->model()->index(currentIndex.row() + 1, 0);
        ui->playlistView->setCurrentIndex(newIndex);
        ui->playlistView->scrollTo(newIndex);
    }
    startStream(playListModel->index(newIndex.row(), 1).data().toString());
}

void BoomBoxProducerWidget::on_previousTrackButton_clicked() {
    QModelIndex currentIndex = ui->playlistView->currentIndex();
    QModelIndex newIndex;
    if (currentIndex.row() - 1 < 0) {
        newIndex = ui->playlistView->model()->index(ui->playlistView->verticalHeader()->count() - 1, 0);
        ui->playlistView->setCurrentIndex(newIndex);
        ui->playlistView->scrollTo(newIndex);
    } else {
        newIndex = ui->playlistView->model()->index(currentIndex.row() - 1, 0);
        ui->playlistView->setCurrentIndex(newIndex);
        ui->playlistView->scrollTo(newIndex);
    }
    startStream(playListModel->index(newIndex.row(), 1).data().toString());
}

void BoomBoxProducerWidget::startStream(QString pathToFile) {
    ui->songTitleNowPlayingPlainText->setPlainText(pathToFile);
    streamProducer->startStreaming(pathToFile.toStdString());
}
