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

    /*!
   * \brief SimpleVisualizationType
   * \param samplesPerChannel - liczba próbek na kanał
   * \param graphicView - obiekt QGraphicsView
   */
  SimpleVisualizationType(int samplesPerChannel, QGraphicsView* graphicView);
  ~SimpleVisualizationType();
  /*!
 * \brief draw
 *
 * @param channelsData krotka zawierająca tablice z obliczonymi fft dla lewego i prawego kanału
 *
 * Funckja rysuje wizualizacjie na podstawie pierwiastka z sumy kwadratów części rzeczywistej i urojonej watości fft
 */
  void draw(std::tuple<float*, float*> channelsData);

 private:
  void show();

 private:
  std::list<int16_t> leftBands;
  std::list<int16_t> rightBands;
  int samplesPerChannel;
  QGraphicsView* graphicView;
};

#endif  // SOMEVISUALIZATIONTYPE_H
