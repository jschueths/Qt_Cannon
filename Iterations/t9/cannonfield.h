#ifndef __CANNONFIELD_H__
#define __CANNONFIELD_H__

#include <QWidget>

class CannonField : public QWidget
{
  Q_OBJECT
  
  public:
    CannonField(QWidget *parent = 0);
    int angle() const;
  
  public slots:
    void setAngle(int angle);
    
  signals:
    void angleChanged(int newAngle);
    
  protected:
    void paintEvent(QPaintEvent *event);
    
  private:
    int currentAngle;
};

#endif
