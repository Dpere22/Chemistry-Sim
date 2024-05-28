#ifndef ELEMENT_H
#define ELEMENT_H

#include <QColor>
#include <QJsonObject>
#include <QObject>

/// @brief Enumeration of Atomic Groups
enum class AtomicGroup {
    Reactive_Nonmetals,
    Alkali_Metals,
    Alkaline_Earth_Metals,
    Lanthanoids,
    Metalloids,
    Post_Transition_Metals,
    Actinoids,
    Noble_Gases,
    Transition_Metals,
    Unknown,
};

/// @brief Stores information about an element
struct Element
{
    /// @brief The atomic number of this element
    int atomicNumber;

    /// @brief The atomic weight of this element (average)
    double atomicWeight;

    /// @brief The abbreviation of this element
    QString symbol;

    /// @brief The atomic group (series) of this element
    QString series;

    /// @brief The atomic group (series) of this element enumerated
    AtomicGroup atomicGroup;

    /// @brief The base number of valence electrons of this element
    int valence;

    /// @brief The current number of valence electrons of this element
    int currentValence;

    /// @brief The current charge of this element
    int currentCharge;

    /// @brief Whether this element is some form of metal
    bool isMetal() const;

    /// @brief Whether this element is a reactive nonmetal or metalloid
    bool isNonMetal() const;

    /// @brief Whether this atom has satisfied its orbitals
    bool isSatisfied() const;

    /// @brief Handles internal resources when forming a bond
    /// and returns reason for failure.
    /// If successful, returns empty string.
    QString bondTo(Element &other);

    /// @brief Helper method for bondTo that conducts appropriate ionic bond
    static void doIonicBond(Element *metal, Element *nonMetal);

    /// @brief Dictionary of atomic group colors
    static const QVector<QColor> GroupColors;

    /// @brief Returns the proper color scheme for this element
    static QColor getColor(const Element *element);

    /// @brief Creates element from JSON object
    static Element *fromJSON(const QJsonObject &object);
};

#endif // ELEMENT_H
