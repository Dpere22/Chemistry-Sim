#include "model.h"
#include "bonddata.h"

Model::Model(QObject *parent)
    : QObject{parent}
{
    QJsonObject elements, compounds;

    if (loadJson(":/elements.json", elements)) {
        QJsonArray elementsArray(elements["elements"].toArray());

        for (auto elementObject : elementsArray) {
            auto element = Element::fromJSON(elementObject.toObject());
            allElements[element->atomicNumber] = element;
        }
    }

    if (loadJson(":/compounds.json", compounds)) {
        QJsonArray compoundsArray(compounds["compounds"].toArray());

        for (auto compoundObject : compoundsArray) {
            auto compound = Compound::fromJSON(compoundObject.toObject());
            allCompounds[compound->atomicFormula] = compound;
        }
    }
}

bool Model::loadJson(const QString &filename, QJsonObject &object)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "file not found";
        return false;
    }

    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return false;
    }

    object = jsonDoc.object();
    return true;
}

Element *Model::getElement(int atomicNumber)
{
    return allElements.value(atomicNumber);
}

void Model::checkDiscovery(const QString &discovery)
{
    for (auto [compoundFormula, compound] : allCompounds.asKeyValueRange()) {
        if (BondData::matchBonds(compoundFormula, discovery)) {
            if (!discoveredCompounds.contains(compound->commonName)) {
                discoveredCompounds.push_back(compound->commonName);
                emit newCompoundDiscovered(compound);
            }
            return;
        }
    }
}
