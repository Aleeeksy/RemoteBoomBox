#ifndef VISUALIZATIONTYPE_H
#define VISUALIZATIONTYPE_H

#include <tuple>

/*!
 * \brief The VisualizationType class
 *
 * Klasa służy jako interfejs, klasy wizualizujące streamu muszą ją implementować
 *
 */

class VisualizationType {
 public:
  virtual ~VisualizationType();
    /*!
   * \brief draw
   *
   * @param std::typle<float*, float*> krotka zawierająca tablice z obliczonymi fft dla lewego i prawego kanału
   *
   */
  virtual void draw(std::tuple<float*, float*>) = 0;
};

#endif  // VISUALIZATIONTYPE_H
