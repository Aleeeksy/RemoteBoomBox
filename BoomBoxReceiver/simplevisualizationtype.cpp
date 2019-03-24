#include "simplevisualizationtype.h"

SimpleVisualizationType::SimpleVisualizationType(int samplesPerChannel,
                                                 QGraphicsView* graphicView) {
  this->samplesPerChannel = samplesPerChannel;
  this->graphicView = graphicView;
  this->leftBands = std::list<int16_t>(graphicView->width(), int16_t(0));
  this->rightBands = std::list<int16_t>(graphicView->width(), int16_t(0));
}

SimpleVisualizationType::~SimpleVisualizationType() {}

void SimpleVisualizationType::draw(std::tuple<float*, float*> channelsData) {
  int size =
      samplesPerChannel / 2 * 2;

  for (int i = 0, index = 0; i < size; i += size / WIDTH) {
    if (index % 300 == 0) {
      show();
    }
    float im = std::get<0>(channelsData)[i];
    float re = std::get<0>(channelsData)[i + 1];
    double magnitude = sqrt(im * im + re * re);

    leftBands.pop_front();
    leftBands.push_back((int16_t)(magnitude * HEIGHT));

    float im_r = std::get<1>(channelsData)[i];
    float re_r = std::get<1>(channelsData)[i + 1];
    double magnitude_r = sqrt(im_r * im_r + re_r * re_r);

    rightBands.pop_front();
    rightBands.push_back((int16_t)(magnitude * HEIGHT));
  }
}

void SimpleVisualizationType::show() {
  QGraphicsScene* scene = new QGraphicsScene();

  int index = 0;
  int middleValue = graphicView->height() / 2;
  for (auto const& value : leftBands) {
    scene->addLine(index, middleValue, index, value);
    index++;
  }

  graphicView->setScene(scene);
}
