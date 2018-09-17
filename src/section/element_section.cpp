#include "common.h"
#include "element_section.h"

ElementSegment::ElementSegment(uint32_t index, InitExpr* offset)
        : index_(index)
        , offset_(offset) {
}

uint32_t ElementSegment::numElement() {
    return elements_.size();
}

void ElementSegment::addElement(uint32_t elem) {
    elements_.emplace_back(elem);
}

uint32_t ElementSection::count() {
    return segments_.size();
}

void ElementSection::addElementSegment(ElementSegment* segment) {
    segments_.emplace_back(segment);
}
