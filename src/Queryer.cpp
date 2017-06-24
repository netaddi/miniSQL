#include "stdafx.h"
#include "Queryer.h"
#include "Record.h"

template <class T>
bool SingleQuery<T>::match(Record record)
{
    Element elementToCompare(targetData);

    for (size_t attrIter = 0; attrIter < record.elements.size(); attrIter++)
    {
        if (record.table.attributes[attrIter].name == attrName)
        {
            return (*record.elements[attrIter]) == elementToCompare;
        }
    }
    return false;
}

template <class T>
bool RangeQuery<T>::match(Record record)
{
    Element minElement(minData);
    Element maxElement(maxData);
    for (size_t attrIter = 0; attrIter < record.elements.size(); attrIter++)
    {
        if (record.table.attributes[attrIter].name == attrName)
        {
            Element thisElement = *record.elements[attrIter];
            switch((minIncluded ? 2 : 0) + (maxIncluded ? 1 : 0))
            {
                case 3 : return thisElement >= minElement && thisElement <= maxElement;
                case 2 : return thisElement >= minElement && thisElement <  maxElement;
                case 1 : return thisElement >  minElement && thisElement <= maxElement;
                case 0 : return thisElement >  minElement && thisElement <  maxElement;
            }
        }
    }
    return false;
}
