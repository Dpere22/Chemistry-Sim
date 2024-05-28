#include "bouncewindow.h"

BounceWindow::BounceWindow(QWidget *parent)
    : QWidget{parent}
{}

void BounceWindow::mousePressEvent(QMouseEvent *event)
{
    emit mouseOutput(event);
}

void BounceWindow::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseOutput(event);
}

void BounceWindow::paintEvent(QPaintEvent *)
{
    if (engine) {
        QPixmap image = engine->getRender();
        QPainter mainPainter(this);
        mainPainter.drawPixmap(0, 0, image);
    }
}

void BounceWindow::resizeEvent(QResizeEvent *event)
{
    emit screenResized(event);
}

void BounceWindow::updateView()
{
    update();
}

void BounceWindow::connectEngine(BounceEngine *engine)
{
    this->engine = engine;
    connect(this, &BounceWindow::mouseOutput, engine, &BounceEngine::receiveInput);
    connect(engine, &BounceEngine::worldStepped, this, &BounceWindow::updateView);
    connect(this, &BounceWindow::screenResized, engine, &BounceEngine::resizeWorld);
}

void BounceWindow::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseOutput(event);
}
