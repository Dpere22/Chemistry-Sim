#include "bonddata.h"

const std::tuple<int, int> TUPLE_CHECK_GRAVE{0, 0};
const BondData BOND_CHECK_GRAVE{0, {}};
const char *BOND_DELIMITER = " ";
const char *BOND_SEPARATOR = "|";
const char *BOND_MULTIPLES = ":";

BondData::operator QString() const
{
    QString string = QString::number(atomicNumber);

    for (auto [bondedAtomicNumber, bondStrength] : bonds) {
        string += BOND_SEPARATOR + QString::number(bondedAtomicNumber);
        string += BOND_MULTIPLES + QString::number(bondStrength);
    }

    return string;
}

bool BondData::operator==(const BondData &other) const
{
    if (atomicNumber != other.atomicNumber) {
        return false;
    }

    if (bonds.size() != other.bonds.size()) {
        return false;
    }

    auto otherBonds(other.bonds);

    for (auto &bond : bonds) {
        int index = otherBonds.indexOf(bond);
        if (index == -1) {
            return false;
        } else {
            otherBonds[index] = TUPLE_CHECK_GRAVE;
        }
    }

    return true;
}

BondData BondData::singleFromQString(const QString &string)
{
    auto data = string.split(BOND_SEPARATOR);
    int rootAtomicNumber = data.takeFirst().toInt();
    BondData output{rootAtomicNumber, {}};

    for (auto &bond : data) {
        auto list = bond.split(BOND_MULTIPLES);
        int atomicNumber = list.takeFirst().toInt();
        int bondStrength = list.takeFirst().toInt();
        output.bonds.push_back({atomicNumber, bondStrength});
    }

    return output;
}

QVector<BondData> BondData::bondsFromQString(const QString &string)
{
    auto data = string.split(BOND_DELIMITER);
    QVector<BondData> output;

    for (auto &section : data) {
        output.push_back(BondData::singleFromQString(section));
    }

    return output;
}

QString BondData::stringifyBonds(const QVector<BondData> &bonds)
{
    QString string;

    for (auto &bond : bonds) {
        string += BOND_DELIMITER + QString(bond);
    }

    return string.sliced(1);
}

bool BondData::matchBonds(const QString &string1, const QString &string2)
{
    auto bonds1 = BondData::bondsFromQString(string1);
    auto bonds2 = BondData::bondsFromQString(string2);

    if (bonds1.size() != bonds2.size()) {
        return false;
    }

    for (auto &bond : bonds1) {
        int index = bonds2.indexOf(bond);
        if (index == -1) {
            return false;
        } else {
            bonds2[index] = BOND_CHECK_GRAVE;
        }
    }

    return true;
}
