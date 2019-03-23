#ifndef VISUALIZATIONTYPE_H
#define VISUALIZATIONTYPE_H

#include <tuple>

class VisualizationType {
 public:
  virtual ~VisualizationType();
  virtual void draw(std::tuple<float*, float*>) = 0;
};

#endif  // VISUALIZATIONTYPE_H
