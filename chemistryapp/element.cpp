#include "element.h"

const QVector<QColor> Element::GroupColors{
    {0, 128, 0},    // Reactive Nonmetals
    {156, 72, 16},  // Alkali Metals
    {201, 133, 30}, // Alkaline Earth Metals
    {135, 64, 34},  // Lanthanoids
    {25, 181, 171}, // Metalloids
    {54, 20, 179},  // Post Transition Metals
    {162, 18, 201}, // Actinoids
    {109, 15, 133}, // Noble Gasses
    {140, 10, 42},  // Transition Metals
    {82, 82, 82},   // Unknown
};

const char *FAIL_REASON_NONE = "";
const char *FAIL_REASON_NOBLEGAS
    = "One of these atoms is a Noble Gas. All of its electron orbitals are satisfied.";
const char *FAIL_REASON_NOTSUPPORTED
    = "Metals Not Supported Yet!";
const char *FAIL_REASON_SATISFIED
    = "One of these atoms has satisfied all of its electron orbitals";

bool Element::isMetal() const
{
    return atomicGroup == AtomicGroup::Alkali_Metals
           || atomicGroup == AtomicGroup::Alkaline_Earth_Metals
           || atomicGroup == AtomicGroup::Lanthanoids
           || atomicGroup == AtomicGroup::Post_Transition_Metals
           || atomicGroup == AtomicGroup::Actinoids
           || atomicGroup == AtomicGroup::Transition_Metals;
}

bool Element::isNonMetal() const
{
    return atomicGroup == AtomicGroup::Reactive_Nonmetals || atomicGroup == AtomicGroup::Metalloids;
}

bool Element::isSatisfied() const
{
    if (valence == 0) {
        return false;
    }

    if (currentValence == 0) {
        return true;
    }

    if (atomicNumber <= 2) {
        return currentValence == 2;
    }

    return currentValence == 8;
}

QString Element::bondTo(Element &other)
{
    if (atomicGroup == AtomicGroup::Noble_Gases || other.atomicGroup == AtomicGroup::Noble_Gases) {
        return FAIL_REASON_NOBLEGAS;
    }

    if (isSatisfied() || other.isSatisfied()) {
        return FAIL_REASON_SATISFIED;
    }

    /// Can form ionic bond
    if (isMetal() && other.isNonMetal()) {
        doIonicBond(this, &other);
        return FAIL_REASON_NONE;
    }

    /// Can form ionic bond
    if (isNonMetal() && other.isMetal()) {
        doIonicBond(&other, this);
        return FAIL_REASON_NONE;
    }

    /// Can form covalent bond
    if (isNonMetal() && other.isNonMetal()) {
        currentValence++;
        other.currentValence++;
        return FAIL_REASON_NONE;
    }

    return FAIL_REASON_NOTSUPPORTED;
}

void Element::doIonicBond(Element *metal, Element *nonMetal)
{
    metal->currentCharge++;
    nonMetal->currentCharge--;
    metal->currentValence--;
    nonMetal->currentValence++;
}

QColor Element::getColor(const Element *element)
{
    auto color = GroupColors.at((int) element->atomicGroup);
    return color;
}

Element *Element::fromJSON(const QJsonObject &object)
{
    return new Element{
        object["atomicNumber"].toInt(),
        object["atomicWeight"].toDouble(),
        object["symbol"].toString(),
        object["series"].toString(),
        AtomicGroup{object["group"].toInt()},
        object["valence"].toInt(),
        object["valence"].toInt(),
        0,
    };
}
