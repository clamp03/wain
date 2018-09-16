#ifndef ELEMENT_SECTION_H
#define ELEMENT_SECTION_H

#include <vector>

#include "init.h"
#include "type.h"

class ElementSegment {
public:
    ElementSegment(uint32_t index, InitExpr* offset);

    uint32_t numElement();
    void addElement(uint32_t elem);

private:
    uint32_t index_;
    InitExpr* offset_;
    std::vector<uint32_t> elements_;
};

class ElementSection {
public:
    uint32_t count();
    void addElementSegment(ElementSegment* segment);

private:
    std::vector<ElementSegment*> segments_;
};

#endif // ELEMENT_SECTION_H
