#include <QDateTime>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

#include <cmath>
#include <cstdlib>

#include "cannonfield.h"

CannonField::CannonField(QWidget *parent) : QWidget(parent)
{
  currentAngle = 45;
  currentForce = 0;
  timerCount = 0;
  autoShootTimer = new QTimer(this);
  connect(autoShootTimer, SIGNAL(timeout()), this, SLOT(moveShot()));
  shootAngle = 0;
  shootForce = 0;
  setPalette(QPalette(QColor(250, 250, 200)));
  setAutoFillBackground(true);
}

int CannonField::angle() const
{
  return currentAngle;
}

int CannonField::force() const
{
  return currentForce;
}

void CannonField::setAngle(int angle)
{
  if(angle < 5)
    angle = 5;
  if(angle > 70)
    angle = 70;
  if(currentAngle == angle)
    return;
  currentAngle = angle;
  update(cannonRect());
  emit angleChanged(currentAngle);
  return;
}

void CannonField::setForce(int force)
{
  if(force < 0)
    force = 0;
  if(currentForce == force)
    return;
  currentForce = force;
  emit forceChanged(currentForce);
  return;
}

void CannonField::shoot()
{
  if(autoShootTimer->isActive())
    return;
  timerCount = 0;
  shootAngle = currentAngle;
  shootForce = currentForce;
  autoShootTimer->start(5);
  return;
}

void CannonField::moveShot()
{
  QRegion region = shotRect();
  ++timerCount;
  
  QRect shotR = shotRect();
  
  if(shotR.x() > width() ||shotR.y() > height())
  {
    autoShootTimer->stop();
  }
  else
  {
    region = region.unite(shotR);
  }
  update(region);
  return;
}

void CannonField::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  
  paintCannon(painter);
  if(autoShootTimer->isActive())
    paintShot(painter);
  return;
}

void CannonField::paintShot(QPainter &painter)
{
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(shotRect());
  return;
}

const QRect barrelRect(30, -5, 20, 10);

void CannonField::paintCannon(QPainter &painter)
{
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::blue);
 
  painter.save();
  painter.translate(0, height());
  painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
  painter.rotate(-currentAngle);
  painter.drawRect(barrelRect);
  painter.restore();
  return;
}

QRect CannonField::cannonRect() const
{
  QRect result(0, 0, 50, 50);
  result.moveBottomLeft(rect().bottomLeft());
  return result;
}

QRect CannonField::shotRect() const
{
  const double gravity = 4;
  
  double time = timerCount / 20.0;
  double velocity = shootForce;
  double radians = shootAngle * 3.14159265 / 180;
  
  double velx = velocity * cos(radians);
  double vely = velocity * sin(radians);
  double x0 = (barrelRect.right() + 5) * cos(radians);
  double y0 = (barrelRect.right() + 5) * sin(radians);
  double x = x0 + velx * time;
  double y = y0 + vely *time - 0.5 * gravity * time * time;
  
  QRect result(0, 0, 6, 6);
  result.moveCenter(QPoint(qRound(x), height() -1 - qRound(y)));
  return result;
}
