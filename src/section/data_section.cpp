#include "common.h"
#include "data_section.h"

DataSegment::DataSegment(uint32_t index, InitExpr* offset, uint32_t size, uint8_t* bytes)
        : index_(index)
        , offset_(offset)
        , size_(size)
        , bytes_(bytes) {
}

uint32_t DataSection::count() {
    return segments_.size();
}

void DataSection::addDataSegment(DataSegment* segment) {
    segments_.emplace_back(segment);
}
