#ifndef BOUNCEENGINE_H
#define BOUNCEENGINE_H

#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPixMap>
#include <QRandomGenerator>
#include <QTextItem>
#include <QTimer>
#include "bonddata.h"
#include "element.h"
#include <Box2D/Box2D.h>

/// @brief Governs interactions with atoms in physics environment
/// @authors Goobers team
class BounceEngine : public QObject
{
    Q_OBJECT

    /// @brief Runs the world tick
    QTimer timer;

    /// @brief Represents the area in which the physics happens
    b2World world;

    /// @brief Represents the currently selected physics body
    b2Fixture *focused;

    /// @brief Defines and allows mouse grab joint
    b2MouseJointDef mouseJointDef;

    /// @brief Mouse grab joint
    b2MouseJoint *mouseJoint;

    /// @brief Window height dimension
    int windowHeight;

    /// @brief Window width dimension
    int windowWidth;

    /// @brief Random generator to assist with generating random numbers
    QRandomGenerator randomGenerator;

public:
    /// @brief Constructs default
    explicit BounceEngine(QObject *parent = nullptr);

    /// @brief Returns a QPixmap representation of the world state
    const QPixmap getRender() const;

private:
    /// @brief Returns Box2D selection at point
    b2Fixture *getSelection(float xPos, float yPos);

    /// @brief Updates dragged mouse position
    void dragMouse(float xPos, float yPos);

    /// @brief Executes first mouse press
    void initMouse(float xPos, float yPos);

    /// @brief Releases mouse position
    void releaseMouse(float xPos, float yPos);

    /// @brief Returns a random linear impulse velocity vector
    b2Vec2 getRandomLinearImpulseVector();

    /// @brief Attempts to bind element bodies, returns empty string if successful
    static QString tryBond(b2Body *body1, b2Body *body2);

    /// @brief Returns the number of edges on a body
    static int countEdges(b2Body *body);

    /// @brief Returns the joints connecting a body (Recursive)
    static void getConnected(b2Body *current, QVector<b2Body *> &bodies);

    /// @brief Returns the joints connecting a body (Driver)
    static QVector<b2Body *> getConnected(b2Body *body);

    /// @brief Returns a set of bond data structs representing an entire compound
    static QVector<BondData> getAllBonds(b2Body *body);

    /// @brief Returns a struct of bond data for a single body
    static BondData getBonds(b2Body *body);

    /// @brief Returns number of joints between two dynamic bodies
    static int getNumJoints(const b2Body *body1, const b2Body *body2);

    /// @brief Returns appropriate set of bond joints for two bodies
    static b2DistanceJointDef defineJoint(b2Body *body1, b2Body *body2);

    /// @brief Generates a rectangle box for drawing a circular body
    static QRect getEllipseBox(int xPos, int yPos, int radius);

public slots:
    /// @brief Adds an atom
    void addAtom(float xPos, float yPos, Element* element);

    /// @brief Removes all atoms in the engine
    void clearAtoms();

    /// @brief Adds a wall
    void addWall(float xPos, float yPos, float width, float height);

    /// @brief Executes step world
    void stepWorld();

    /// @brief Starts simulation
    void run();

    /// @brief Mouse input receiver, delegates task based on input
    void receiveInput(QMouseEvent *event);

    /// @brief Change the world size
    void resizeWorld(QResizeEvent *event);

signals:
    /// @brief Notifies world stepped
    void worldStepped();

    /// @brief Notifies compound composition
    void compoundBuilt(const QString &discovery);

    /// @brief Notifies bond attempt failure
    void bondFailed(const QString &reason);
};

#endif // BOUNCEENGINE_H
