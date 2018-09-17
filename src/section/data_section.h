#ifndef DATA_SECTION_H
#define DATA_SECTION_H

#include <vector>

#include "init_expr.h"

class DataSegment {
public:
    DataSegment(uint32_t index, InitExpr* offset, uint32_t size, uint8_t* bytes);

private:
    uint32_t index_;
    InitExpr* offset_;
    uint32_t size_;
    uint8_t* bytes_;
};

class DataSection {
public:
    uint32_t count();
    void addDataSegment(DataSegment* segment);

private:
    std::vector<DataSegment*> segments_;
};

#endif // DATA_SECTION_H
