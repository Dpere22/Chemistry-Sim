#ifndef BONDDATA_H
#define BONDDATA_H

#include <QObject>

/// @brief Stores bond information
struct BondData
{
    /// @brief Bond root
    int atomicNumber;

    /// @brief Atomic number of bonded atom; Number of bonds to that atom
    QVector<std::tuple<int, int>> bonds;

    /// @brief Class cast to QString
    operator QString() const;

    /// @brief Override equals operator. Vector can be in any order
    bool operator==(const BondData &other) const;

    /// @brief Reads single BondData from input QString
    static BondData singleFromQString(const QString &string);

    /// @brief Reads multiple BondData from input QString
    static QVector<BondData> bondsFromQString(const QString &string);

    /// @brief Casts vector of bond data to parseable QString
    static QString stringifyBonds(const QVector<BondData> &bonds);

    /// @brief Determines if both input strings are matching bond sets
    static bool matchBonds(const QString &string1, const QString &string2);
};

#endif // BONDDATA_H
