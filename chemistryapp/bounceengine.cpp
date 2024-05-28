#include "bounceengine.h"

// TODO:
// Implement Valence electron calculations
// Center Element abbreviation on atom
// Remove temporary atom generator clicks
// Add different colors - DONE

/// Base simulation gravity
static const int GRAVITY_X = 0;
static const int GRAVITY_Y = 0;

/// Mouse joint properties
static const int MOUSE_JOINT_MAX_FORCE = 500;
static const int MOUSE_JOINT_DAMPING = 10;
static const float MOUSE_JOINT_FREQUENCY = 0.3f;

/// Distance joint properties
static const float DIST_JOINT_FREQUENCY = 0.01f;
static const float DIST_JOINT_DAMPING = 5.0f;
static const float DIST_JOINT_SCALE = 3.0f;

/// Atom properties
static const int MIN_ATOM_SIZE = 40;
static const float ATOM_DENSITY = 0.008;
static const float ATOM_FRICTION = 0.3;
static const float ATOM_RESTITUTION = 0.8;

/// Simulation properties
static const float TIME_STEP = 2.0f / 60.0f;
static const int VELOCITY_ITERATIONS = 6;
static const int POSITION_ITERATIONS = 2;
static const int WORLD_TIME_EXEC_MS = 2;
static const int LINEAR_IMPULSE_BASE = 20;

/// Display settings
static const int SYMBOL_FONT_SIZE = 18;
static const float BOND_SPACE_MULT = 4.0f;

BounceEngine::BounceEngine(QObject *parent)
    : QObject{parent}
    , world({GRAVITY_X, GRAVITY_Y})
    , focused(nullptr)
    , mouseJoint(nullptr)
    , randomGenerator(std::chrono::system_clock::now().time_since_epoch().count())
{
    mouseJointDef.bodyA = world.CreateBody(new b2BodyDef());
    mouseJointDef.collideConnected = true;
    mouseJointDef.maxForce = MOUSE_JOINT_MAX_FORCE;
    mouseJointDef.frequencyHz = MOUSE_JOINT_FREQUENCY;
    mouseJointDef.dampingRatio = MOUSE_JOINT_DAMPING;

    connect(&timer, &QTimer::timeout, this, &BounceEngine::stepWorld);
}

const QPixmap BounceEngine::getRender() const
{
    QPixmap image(windowWidth, windowHeight);
    image.fill(Qt::transparent);
    QPainter imagePainter(&image);
    QPen pen(Qt::black, 3);

    imagePainter.setPen(pen);

    /// Draw bonds
    for (auto *body = world.GetBodyList(); body; body = body->GetNext()) {
        for (auto edge = body->GetJointList(); edge; edge = edge->next) {
            auto other = edge->other;

            int joints = getNumJoints(body, other);

            auto [x1, y1] = edge->joint->GetAnchorA();
            auto [x2, y2] = edge->joint->GetAnchorB();

            QLineF line(x1, y1, x2, y2);
            QVector2D vector(line.dy(), -line.dx());
            vector.normalize();
            auto point = vector.toPointF() * BOND_SPACE_MULT;

            switch (joints) {
            case 1: {
                imagePainter.drawLine(line);
                break;
            }
            case 2: {
                imagePainter.drawLine(line.translated(point));
                imagePainter.drawLine(line.translated(point * -1.0f));
                break;
            }
            case 3: {
                imagePainter.drawLine(line);
                imagePainter.drawLine(line.translated(point * 2.0f));
                imagePainter.drawLine(line.translated(point * -2.0f));
                break;
            }
            case 4: {
                imagePainter.drawLine(line.translated(point));
                imagePainter.drawLine(line.translated(point * 3.0f));
                imagePainter.drawLine(line.translated(point * -3.0f));
                break;
            }
            }
        }
    }

    QFont font = imagePainter.font();
    font.setPointSize(SYMBOL_FONT_SIZE);
    imagePainter.setFont(font);
    imagePainter.setPen(Qt::white);
    QFontMetrics metrics(imagePainter.font());

    /// Draw atoms
    for (auto body = world.GetBodyList(); body; body = body->GetNext()) {
        auto [bodyX, bodyY] = body->GetPosition();

        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            if (fixture->GetType() == b2Shape::e_circle) {
                b2CircleShape *circle = (b2CircleShape *) fixture->GetShape();
                auto [vertX, vertY] = circle->GetVertex(0);
                auto box = getEllipseBox(bodyX + vertX, bodyY + vertY, circle->m_radius);
                Element *data = static_cast<Element *>(body->GetUserData());

                imagePainter.setBrush(Element::getColor(data));
                imagePainter.drawEllipse(box);

                auto text = data->symbol;
                auto textSize = metrics.size(Qt::TextWordWrap, text);
                float textX = bodyX - textSize.width();
                float textY = bodyY;
                imagePainter.drawText(textX, textY, text);

                text = QString::number(data->currentValence);
                textSize = metrics.size(Qt::TextWordWrap, text);
                textX = bodyX;
                textY = bodyY + textSize.height() / 1.5f;
                imagePainter.drawText(textX, textY, text);

                int charge = data->currentCharge;

                if (charge != 0) {
                    auto prefix = charge > 0 ? "+" : "";
                    text = prefix + QString::number(charge);
                    textSize = metrics.size(Qt::TextWordWrap, text);
                    textX = bodyX;
                    textY = bodyY;
                    imagePainter.drawText(textX, textY, text);
                }
            }
        }
    }

    /// Draw mouse joint
    if (mouseJoint) {
        auto [x1, y1] = mouseJoint->GetAnchorA();
        auto [x2, y2] = mouseJoint->GetAnchorB();

        imagePainter.setPen(Qt::black);
        imagePainter.drawLine(x1, y1, x2, y2);
    }

    return image;
}

int BounceEngine::countEdges(b2Body *body)
{
    int edges = 0;

    for (auto edge = body->GetJointList(); edge; edge = edge->next) {
        edges++;
    }

    return edges;
}

void BounceEngine::getConnected(b2Body *current, QVector<b2Body *> &bodies)
{
    for (auto edge = current->GetJointList(); edge; edge = edge->next) {
        auto body = edge->other;

        if (!bodies.contains(body) && body->GetType() == b2_dynamicBody) {
            bodies.push_back(body);
            getConnected(body, bodies);
        }
    }
}

QVector<b2Body *> BounceEngine::getConnected(b2Body *body)
{
    QVector<b2Body *> bodies;
    getConnected(body, bodies);
    return bodies;
}

QVector<BondData> BounceEngine::getAllBonds(b2Body *body)
{
    QVector<BondData> bondDataVector;

    for (auto element : getConnected(body)) {
        BondData bondData = getBonds(element);
        bondDataVector.push_back(bondData);
    }

    return bondDataVector;
}

BondData BounceEngine::getBonds(b2Body *body)
{
    int parentAtomicNumber = static_cast<Element *>(body->GetUserData())->atomicNumber;
    QHash<b2Body *, std::tuple<int, int>> visitedBodies;

    for (auto edge = body->GetJointList(); edge; edge = edge->next) {
        auto boundBody = edge->other;

        if (boundBody->GetType() == b2_dynamicBody && !visitedBodies.contains(boundBody)) {
            int atomicNumber = static_cast<Element *>(boundBody->GetUserData())->atomicNumber;
            int bonds = getNumJoints(body, boundBody);

            if (bonds > 0) {
                visitedBodies[boundBody] = {atomicNumber, bonds};
            }
        }
    }

    return {parentAtomicNumber, visitedBodies.values()};
}

int BounceEngine::getNumJoints(const b2Body *body1, const b2Body *body2)
{
    int count = 0;

    if (body1->GetType() != b2_dynamicBody || body2->GetType() != b2_dynamicBody) {
        return 0;
    }

    for (auto edge = body1->GetJointList(); edge; edge = edge->next) {
        if (body2 == edge->other) {
            count++;
        }
    }

    return count;
}

b2DistanceJointDef BounceEngine::defineJoint(b2Body *body1, b2Body *body2)
{
    b2DistanceJointDef jointDef;
    jointDef.bodyA = body1;
    jointDef.bodyB = body2;
    jointDef.collideConnected = true;
    jointDef.frequencyHz = DIST_JOINT_FREQUENCY;
    jointDef.dampingRatio = DIST_JOINT_DAMPING;
    jointDef.length = MIN_ATOM_SIZE * DIST_JOINT_SCALE;
    return jointDef;
}

QRect BounceEngine::getEllipseBox(int xPos, int yPos, int radius)
{
    return {xPos - radius, yPos - radius, radius * 2, radius * 2};
}

b2Vec2 BounceEngine::getRandomLinearImpulseVector()
{
    float x = LINEAR_IMPULSE_BASE * (randomGenerator.generateDouble() < 0.5 ? -1 : 1);
    float y = LINEAR_IMPULSE_BASE * (randomGenerator.generateDouble() < 0.5 ? -1 : 1);
    return {x, y};
}

QString BounceEngine::tryBond(b2Body *body1, b2Body *body2)
{
    Element *elem1 = static_cast<Element *>(body1->GetUserData());
    Element *elem2 = static_cast<Element *>(body2->GetUserData());
    return elem1->bondTo(*elem2);
}

b2Fixture *BounceEngine::getSelection(float xPos, float yPos)
{
    b2Vec2 point(xPos, yPos);

    for (auto body = world.GetBodyList(); body; body = body->GetNext()) {
        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
            if (fixture->TestPoint(point)) {
                return fixture;
            }
        }
    }

    return nullptr;
}

void BounceEngine::dragMouse(float xPos, float yPos)
{
    if (mouseJoint) {
        mouseJoint->SetTarget(b2Vec2(xPos, yPos));
    }
}

void BounceEngine::initMouse(float xPos, float yPos)
{
    focused = getSelection(xPos, yPos);

    if (focused) {
        mouseJointDef.bodyB = focused->GetBody();
        mouseJointDef.target.Set(xPos, yPos);
        mouseJoint = (b2MouseJoint *) world.CreateJoint(&mouseJointDef);
    }
}

void BounceEngine::releaseMouse(float xPos, float yPos)
{
    if (mouseJoint) {
        world.DestroyJoint(mouseJoint);
        mouseJoint = nullptr;
        auto target = getSelection(xPos, yPos);

        if (target && target != focused) {
            auto body1 = focused->GetBody();
            auto body2 = target->GetBody();
            auto failString = tryBond(body1, body2);

            if (failString.isEmpty()) {
                auto joint = defineJoint(body1, body2);
                world.CreateJoint(&joint);
                auto bonds = getAllBonds(body1);
                auto bondString = BondData::stringifyBonds(bonds);
                qDebug() << bondString;
                emit compoundBuilt(bondString);
            } else {
                qDebug() << failString;
                emit bondFailed(failString);
            }
        }
    }

    focused = nullptr;
}

void BounceEngine::addAtom(float xPos, float yPos, Element* element)
{
    Element *elementCopy = new Element(*element);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(xPos, yPos);

    b2CircleShape circleShape;
    circleShape.m_radius = MIN_ATOM_SIZE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = ATOM_DENSITY;
    fixtureDef.friction = ATOM_FRICTION;
    fixtureDef.restitution = ATOM_RESTITUTION;
    fixtureDef.userData = elementCopy;

    b2Body *body = world.CreateBody(&bodyDef);
    body->SetUserData(elementCopy);
    body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity(getRandomLinearImpulseVector());
}

void BounceEngine::clearAtoms()
{
    for (auto body = world.GetBodyList(); body; body = body->GetNext()) {
        if (body->GetType() == b2_dynamicBody) {
            world.DestroyBody(body);
        }
    }
}

void BounceEngine::addWall(float xPos, float yPos, float width, float height)
{
    b2BodyDef wallBodyDef;
    wallBodyDef.type = b2_staticBody;
    wallBodyDef.position.Set(xPos, yPos);

    b2PolygonShape wallBox;
    wallBox.SetAsBox(width, height);

    b2Body *wallBody = world.CreateBody(&wallBodyDef);
    wallBody->CreateFixture(&wallBox, 0);
}

void BounceEngine::stepWorld()
{
    world.Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    emit worldStepped();
}

void BounceEngine::run()
{
    timer.start(WORLD_TIME_EXEC_MS);
}

void BounceEngine::receiveInput(QMouseEvent *event)
{
    auto [x, y] = event->pos();
    if (event->isBeginEvent() && event->button() == Qt::LeftButton) {
        initMouse(x, y);
    } else if (event->isUpdateEvent() && event->buttons().testFlag(Qt::LeftButton) && focused) {
        dragMouse(x, y);
    } else if (event->isEndEvent() && event->button() == Qt::LeftButton) {
        releaseMouse(x, y);
    }
}

void BounceEngine::resizeWorld(QResizeEvent *event)
{
    /// Destroy walls
    for (auto body = world.GetBodyList(); body; body = body->GetNext()) {
        if (body->GetType() == b2_staticBody) {
            world.DestroyBody(body);
        }
    }

    /// Draw walls
    auto [width, height] = event->size();
    windowHeight = height;
    windowWidth = width;
    int halfHeight = height / 2;
    int halfWidth = width / 2;
    addWall(halfWidth, height, halfWidth, 0);
    addWall(0, halfHeight, 0, halfHeight);
    addWall(width, halfHeight, 0, halfHeight);
    addWall(halfWidth, 0, halfWidth, 0);
}
