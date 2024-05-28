#ifndef COMPOUND_H
#define COMPOUND_H

#include <QJsonObject>
#include <QObject>

/// @brief Stores information about a compound
struct Compound
{
    /// @brief Serialized structure of entire compound
    QString atomicFormula;

    /// @brief Molecular Weight
    QString molecularWeight;

    /// @brief Human-Readable Formula
    QString formula;

    /// @brief Common Name
    QString commonName;

    /// @brief Description of compound
    QString description;

    /// @brief Creates compound from JSON object
    static Compound *fromJSON(const QJsonObject &object);
};

#endif // COMPOUND_H
