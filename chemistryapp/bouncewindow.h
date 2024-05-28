#ifndef BOUNCEWINDOW_H
#define BOUNCEWINDOW_H

#include <QPaintEvent>
#include <QWidget>
#include "bounceengine.h"

/// @brief Displays interactions with atoms in physics environment and receives user input
/// @authors Goobers team
class BounceWindow : public QWidget
{
    Q_OBJECT

    /// @brief Reference to world model
    const BounceEngine *engine;

public:
    /// @brief Constructs default
    explicit BounceWindow(QWidget *parent = nullptr);

    /// @brief Assigns corresponding model module
    void connectEngine(BounceEngine *engine);

private:
    /// @brief Governs mouse interact move event
    void mouseMoveEvent(QMouseEvent *event) override;

    /// @brief Governs mouse interact initial event
    void mousePressEvent(QMouseEvent *event) override;

    /// @brief Governs mouse interact release event
    void mouseReleaseEvent(QMouseEvent *event) override;

    /// @brief Governs view image update
    void paintEvent(QPaintEvent *event) override;

    /// @brief Governs view screen resize
    void resizeEvent(QResizeEvent *event) override;


public slots:
    /// @brief Redraws world
    void updateView();

signals:
    /// @brief Notifies mouse event
    void mouseOutput(QMouseEvent *event);

    /// @brief Notifies window resize event
    void screenResized(QResizeEvent *event);
};

#endif // BOUNCEWINDOW_H
