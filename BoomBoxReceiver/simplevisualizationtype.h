#ifndef SOMEVISUALIZATIONTYPE_H
#define SOMEVISUALIZATIONTYPE_H

#define HEIGHT 32
#define WIDTH 32

#include <math.h>
#include <visualizationtype.h>
#include <iostream>
#include <QGraphicsView>

class SimpleVisualizationType : public VisualizationType {
 public:
  SimpleVisualizationType(int samplesPerChannel, QGraphicsView*);
  ~SimpleVisualizationType();
  void draw(std::tuple<float*, float*>);

 private:
  void show();

 private:
  std::list<int16_t> leftBands;
  std::list<int16_t> rightBands;
  int samplesPerChannel;
  QGraphicsView* graphicView;
};

#endif  // SOMEVISUALIZATIONTYPE_H
