#ifndef __CANNONFIELD_H__
#define __CANNONFIELD_H__

#include <QWidget>

class CannonField : public QWidget
{
  Q_OBJECT
  
  public:
    CannonField(QWidget *parent = 0);
    int angle() const;
    int force() const;
  
  public slots:
    void setAngle(int angle);
    void setForce(int force);
    
  signals:
    void angleChanged(int newAngle);
    void forceChanged(int newForce);
    
  protected:
    void paintEvent(QPaintEvent *event);
    
  private:
    QRect cannonRect() const;
    
    int currentAngle;
    int currentForce;
};

#endif
