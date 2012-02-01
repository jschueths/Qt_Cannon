#ifndef __LCDRANGE_H__
#define __LCDRANGE_H__

#include <QWidget>

class QSlider;

class LCDRange : public QWidget
{
  Q_OBJECT
  
  public:
    LCDRange(QWidget *parent = 0);
    int value() const;
  
  public slots:
    void setValue(int value);
    void setRange(int minValue, int maxValue);
    
  signals:
    void valueChanged(int newValue);
    
  private:
    QSlider *slider;
};

#endif
