#ifndef CANNONFIELD_H
#define CANNONFIELD_H

#include <QWidget>

class QTimer;
class QMouseEvent;

class CannonField : public QWidget
{
  Q_OBJECT

  public:
    CannonField(QWidget *parent = 0);

    int angle() const { return currentAngle; }
    int force() const { return currentForce; }
    bool gameOver() const { return gameEnded; }
    bool isShooting() const;

  public slots:
    void setAngle(int angle);
    void setForce(int force);
    void shoot();
    void newTarget();
    void setGameOver();
    void restartGame();

  private slots:
    void moveShot();

  signals:
    void hit();
    void missed();
    void angleChanged(int newAngle);
    void forceChanged(int newForce);
    void canShoot(bool can);

  protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

  private:
    void paintShot(QPainter &painter);
    void paintTarget(QPainter &painter);
    void paintCannon(QPainter &painter);
    void paintBarrier(QPainter &painter);
    QRect barrierRect() const;
    QRect cannonRect() const;
    QRect shotRect() const;
    QRect targetRect() const;
    bool barrelHit(const QPoint &pos) const;
    bool barrelPressed;

    int currentAngle;
    int currentForce;

    int timerCount;
    QTimer *autoShootTimer;
    float shootAngle;
    float shootForce;

    QPoint target;

    bool gameEnded;
};

 #endif
 