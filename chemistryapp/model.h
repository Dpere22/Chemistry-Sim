#ifndef MODEL_H
#define MODEL_H

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QVector>
#include "compound.h"
#include "element.h"

/// @brief Stores and operates on application data
/// @authors Goobers team
class Model : public QObject
{
    Q_OBJECT

    /// @brief Stores loaded compound data from a json object
    QMap<QString, Compound *> allCompounds;

    /// @brief Stores loaded element data from a json object
    QMap<int, Element *> allElements;

public:
    /// @brief Constructs default
    explicit Model(QObject *parent = nullptr);

    /// @brief Set of discovered compounds by formula
    QVector<QString> discoveredCompounds;

    /// @brief Get Element data
    Element *getElement(int atomicNumber);

public slots:
    /// @brief Checks input compound string against dictionary of internal compounds
    void checkDiscovery(const QString &discovery);

private:
    /// @brief Loads data from a JSON file with input filename, return whether able to read file
    static bool loadJson(const QString &filename, QJsonObject &object);

signals:
    /// @brief Notifies new Compound discovered
    void newCompoundDiscovered(Compound *);
};

#endif // MODEL_H
