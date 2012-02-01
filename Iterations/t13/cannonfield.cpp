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
    target = QPoint(0, 0);
    gameEnded = false;
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    newTarget();
}

void CannonField::setAngle(int angle)
{
  if (angle < 5)
    angle = 5;
  if (angle > 70)
    angle = 70;
  if (currentAngle == angle)
    return;
  currentAngle = angle;
  update(cannonRect());
  emit angleChanged(currentAngle);
  return;
}

void CannonField::setForce(int force)
{
  if (force < 0)
    force = 0;
  if (currentForce == force)
    return;
  currentForce = force;
  emit forceChanged(currentForce);
  return;
}

void CannonField::shoot()
{
  if (isShooting())
    return;
  timerCount = 0;
  shootAngle = currentAngle;
  shootForce = currentForce;
  autoShootTimer->start(5);
  emit canShoot(false);
  return;
}

void CannonField::newTarget()
{
  static bool firstTime = true;
  if (firstTime)
  {
    firstTime = false;
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
  }
  target = QPoint(200 + qrand() % 190, 10 + qrand() % 255);
  update();
  return;
}

void CannonField::setGameOver()
{
  if (gameEnded)
    return;
  if (isShooting())
    autoShootTimer->stop();
  gameEnded = true;
  update();
  return;
}

void CannonField::restartGame()
{
  if (isShooting())
    autoShootTimer->stop();
  gameEnded = false;
  update();
  emit canShoot(true);
  return;
}

void CannonField::moveShot()
{
  QRegion region = shotRect();
  ++timerCount;

  QRect shotR = shotRect();

  if (shotR.intersects(targetRect())) 
  {
    autoShootTimer->stop();
    emit hit();
    emit canShoot(true);
  } 
  else if (shotR.x() > width() || shotR.y() > height())
  {
    autoShootTimer->stop();
    emit missed();
    emit canShoot(true);
  }
  else
  {
    region = region.unite(shotR);
  }
  update(region);
  return;
}

void CannonField::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  if (gameEnded)
  {
    painter.setPen(Qt::black);
    painter.setFont(QFont("Courier", 48, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, tr("Game Over"));
  }
  paintCannon(painter);
  if (isShooting())
    paintShot(painter);
  if (!gameEnded)
    paintTarget(painter);
  return;
}

void CannonField::paintShot(QPainter &painter)
{
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(shotRect());
  return;
}

void CannonField::paintTarget(QPainter &painter)
{
  painter.setPen(Qt::black);
  painter.setBrush(Qt::red);
  painter.drawRect(targetRect());
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
  double y = y0 + vely * time - 0.5 * gravity * time * time;

  QRect result(0, 0, 6, 6);
  result.moveCenter(QPoint(qRound(x), height() - 1 - qRound(y)));
  return result;
}

QRect CannonField::targetRect() const
{
  QRect result(0, 0, 20, 10);
  result.moveCenter(QPoint(target.x(), height() - 1 - target.y()));
  return result;
}

bool CannonField::isShooting() const
{
  return autoShootTimer->isActive();
}
