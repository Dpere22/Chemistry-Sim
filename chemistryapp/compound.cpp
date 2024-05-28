#include "compound.h"

Compound *Compound::fromJSON(const QJsonObject &object)
{
    return new Compound{
        object["atomicFormula"].toString(),
        object["molecularWeight"].toString(),
        object["formula"].toString(),
        object["commonName"].toString(),
        object["description"].toString(),
    };
}
