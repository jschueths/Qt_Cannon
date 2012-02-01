#include <QPainter>
#include "cannonfield.h"

CannonField::CannonField(QWidget *parent) : QWidget(parent)
{
  currentAngle = 45;
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
  update();
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

void CannonField::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::blue);
  
  painter.translate(0, rect().height());
  painter.drawPie(QRect(-35, -35, 70, 70), 0, 90 * 16);
  painter.rotate(-currentAngle);
  painter.drawRect(QRect(30, -5, 20, 10));
  return;
}

QRect CannonField::cannonRect() const
{
  QRect result(0, 0, 50, 50);
  result.moveBottomLeft(rect().bottomLeft());
  return result;
}
