#include "stdafx.h"
#include "Queryer.h"
#include "Record.h"

template
bool SingleQuery<int>::match(Record record);
template
bool SingleQuery<float>::match(Record record);
template
bool SingleQuery<string>::match(Record record);

template
bool RangeQuery<int>::match(Record record);
template
bool RangeQuery<float>::match(Record record);
template
bool RangeQuery<string>::match(Record record);

template
bool infinityRangeQuery<int>::match(Record record);
template
bool infinityRangeQuery<float>::match(Record record);
template
bool infinityRangeQuery<string>::match(Record record);


template <class T>
bool SingleQuery<T>::match(Record record)
{
    Element elementToCompare(targetData);

    for (size_t attrIter = 0; attrIter < record.elements.size(); attrIter++)
    {
        #if DBG
            cout << "trying attr: " << record.table.attributes[attrIter].name << endl;
        #endif
        if (record.table.attributes[attrIter].name == attrName)
        {
            #if DBG
                cout << "thisElement: " << (*record.elements[attrIter]) << " target : " << elementToCompare << "----" << endl;
                cout << ((*record.elements[attrIter]) ==  elementToCompare) << endl;
                // cout << "inv value:" << INVALID_INT_DATA << endl;
            #endif
            if ( *(int*)(record.elements[attrIter]) == INVALID_INT_DATA) return false;
            return ((*record.elements[attrIter]) == elementToCompare) ^ matchUnequal;
            // return ((*record.elements[attrIter]) == elementToCompare);
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
        // #if DBG
        //     cout << "trying attr: " << record.table.attributes[attrIter].name;
        // #endif
        if (record.table.attributes[attrIter].name == attrName)
        {
            if ( *(int*)(record.elements[attrIter]) == INVALID_INT_DATA) return false;
            Element thisElement = *record.elements[attrIter];
            // #if DBG
            //     cout << "thisElement: " << thisElement << " minElement: " << minElement << " max E : " << maxElement << endl;
            //     cout << "mode : " << (minIncluded ? 2 : 0) + (maxIncluded ? 1 : 0) << endl;
            //     cout << "left :" << (thisElement >  minElement) << " right : " << (thisElement <  maxElement) << endl;
            // #endif
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

template <class T>
bool infinityRangeQuery<T>::match(Record record)
{
    Element elementToCompare(seperateData);

    for (size_t attrIter = 0; attrIter < record.elements.size(); attrIter++)
    {
        if (record.table.attributes[attrIter].name == attrName)
        {
            if ( *(int*)(record.elements[attrIter]) == INVALID_INT_DATA) return false;
            Element thisElement = *record.elements[attrIter];
            switch ((queryGreaterThan ? 2 : 0) + (sepInlcluded ? 1 : 0))
            {
                case 3 : return thisElement >= elementToCompare;
                case 2 : return thisElement >  elementToCompare;
                case 1 : return thisElement <= elementToCompare;
                case 0 : return thisElement <  elementToCompare;
            }
        }
    }
    return false;
}
