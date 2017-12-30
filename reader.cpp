#include <assert.h>
#include <string.h>

#include <iostream>

#include "reader.h"
#include "common.h"

using namespace std;

Reader::Reader(const char* fname)
        : fname_(fname),
          fp_(nullptr),
          version_(0) {}

Reader::~Reader() {
}

bool Reader::open() {
    fp_ = fopen(fname_, "rb");
    if (fp_) return true;
    cerr << "Cannot open file: " << fname_ << endl;
    return false;
}

void Reader::close() {
    fclose(fp_);
    fp_ = nullptr;
}

void Reader::read() {
    fseek(fp_, 0 , SEEK_END);
    uint64_t size = ftell(fp_);
    rewind(fp_);
    DEV_ASSERT(readMagicNumber(), "Invalid magic number");
    DEV_ASSERT(readVersion(), "Invalid version number");
    if (version_ == 1) {
        ReaderV1 v1(fp_, size);
        DEV_ASSERT(v1.readSections(), "Invalid sections");
    }
}

bool Reader::readMagicNumber() {
#define MAGIC_NUMBER "\0asm"
#define MAGIC_NUMBER_SIZE sizeof(uint32_t)
    char magic_number[MAGIC_NUMBER_SIZE];
    fread(magic_number, 1, MAGIC_NUMBER_SIZE, fp_);
    return strncmp(magic_number, MAGIC_NUMBER, MAGIC_NUMBER_SIZE) == 0;
}

bool Reader::readVersion() {
#define VERSION_SIZE sizeof(uint32_t)
    return fread(&version_, 1, VERSION_SIZE, fp_) == VERSION_SIZE;
}

ReaderV1::ReaderV1(FILE* fp, uint64_t size)
        : fp_(fp), size_(size) {
}

bool ReaderV1::readSections() {
    uint8_t prev_id = 0;
    while (ftell(fp_) < size_) {
        uint8_t id = readVarUint7();
        uint32_t payload_len = readVarUint32();
        uint32_t name_len = 0;
        const char* name = nullptr;
        if (id == 0) {
            uint32_t name_start = ftell(fp_);
            name_len = readVarUint32();
            name = readBytes(name_len);
            uint32_t name_end = ftell(fp_);
            payload_len -= (name_end - name_start);
        }

        DEV_ASSERT(id >= prev_id || id == 0, "INVALID SECTION ID");
        switch (static_cast<SectionId>(id)) {
            case SectionId::NAME:
                NOT_YET_IMPLEMENTED
            case SectionId::TYPE:
                if (!readTypeSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid type section");
                }
                break;
            case SectionId::IMPORT:
                if (!readImportSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid import section");
                }
                break;
            case SectionId::FUNCTION:
                if (!readFunctionSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid function section");
                }
                break;
            case SectionId::TABLE:
                NOT_YET_IMPLEMENTED
            case SectionId::MEMORY:
                NOT_YET_IMPLEMENTED
            case SectionId::GLOBAL:
                if (!readGlobalSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid global section");
                }
                break;
            case SectionId::EXPORT:
                if (!readExportSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid export section");
                }
                break;
            case SectionId::START:
                NOT_YET_IMPLEMENTED
            case SectionId::ELEMENT:
                if (!readElementSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid element section");
                }
                break;
            case SectionId::CODE:
                if (!readCodeSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid code section");
                }
                break;
            case SectionId::DATA:
                if (!readDataSection(payload_len)) {
                    DEV_ASSERT(false, "Invalid data section");
                }
                break;
            default:
                cerr << "Section Id: " << static_cast<int>(id) << endl;
                NOT_YET_IMPLEMENTED
        }
        prev_id = id;
    }
    return true;
}

bool ReaderV1::readTypeSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t type_count = readVarUint32();
    for (uint32_t entry = 0; entry < type_count; entry++) {
        int8_t form = readVarInt7();
        uint32_t param_count = readVarUint32();
        for (uint32_t param = 0; param < param_count; param++) {
            int8_t value_type = readVarInt7();
            // TODO
        }
        uint8_t return_count = readVarUint1();
        if (return_count) {
            int8_t return_type = readVarInt7();
            // TODO
        }
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readImportSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t import_count = readVarUint32();
    for (uint32_t entry = 0; entry < import_count; entry++) {
        uint32_t module_len = readVarUint32();
        const char* module_str = readBytes(module_len);
        uint32_t field_len = readVarUint32();
        const char* field_str = readBytes(field_len);
        ExternalKind kind = static_cast<ExternalKind>(*readBytes(1));
        if (kind == ExternalKind::Function) {
            uint32_t type = readVarUint32();
        } else if (kind == ExternalKind::Table) {
            // element_type
            int8_t element_type = readVarInt7();
            // limits
            uint8_t flags = readVarUint1();
            uint32_t initial = readVarUint32();
            if (flags) {
                uint32_t maximum = readVarUint32();
            }
        } else if (kind == ExternalKind::Memory) {
            // limits
            uint8_t flags = readVarUint1();
            uint32_t initial = readVarUint32();
            if (flags) {
                uint32_t maximum = readVarUint32();
            }
        } else if (kind == ExternalKind::Global) {
            int8_t content_type = readVarInt7();
            uint8_t mutability = readVarUint1();
        } else {
            DEV_ASSERT(false, "INVALID EXTERNAL KIND");
        }
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readFunctionSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t function_count = readVarUint32();
    for (uint32_t entry = 0; entry < function_count; entry++) {
        uint32_t type = readVarUint32();
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readGlobalSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t global_count = readVarUint32();
    for (uint32_t entry = 0; entry < global_count; entry++) {
        // global_type
        int8_t content_type = readVarInt7();
        uint8_t mutability = readVarUint1();
        // TODO init init_expr
        while (true) {
            uint8_t opcode = readOpcode();
            if (opcode == 0x41) {
                readVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                readVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                readBytes(4); // FIXME
            } else if (opcode == 0x44) {
                readBytes(8); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = readVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = ftell(fp_);
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readExportSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t export_count = readVarUint32();
    for (uint32_t entry = 0; entry < export_count; entry++) {
        uint32_t field_len = readVarUint32();
        const char* field_str = readBytes(field_len);
        ExternalKind kind = static_cast<ExternalKind>(*readBytes(1));
        uint32_t index = readVarUint32();
    }

    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readElementSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t element_count = readVarUint32();
    for (uint32_t entry = 0; entry < element_count; entry++) {
        uint32_t index = readVarUint32();
        while (true) {
            uint8_t opcode = readOpcode();
            if (opcode == 0x41) {
                readVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                readVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                readBytes(4); // FIXME
            } else if (opcode == 0x44) {
                readBytes(8); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = readVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = ftell(fp_);
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t num_elem = readVarUint32();
        for (uint32_t elem_entry = 0; elem_entry < num_elem; elem_entry++) {
            uint32_t elem = readVarUint32();
        }
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readCodeSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t code_count = readVarUint32();
    for (uint32_t entry = 0; entry < code_count; entry++) {
        uint32_t body_size = readVarUint32();
        uint32_t start_body = ftell(fp_);
        uint32_t local_count = readVarUint32();
        for (uint32_t local_entry = 0; local_entry < local_count; local_entry++) {
            uint32_t local_entry_count = readVarUint32();
            int8_t value_type = readVarInt7();
        }

        uint8_t opcode;
        uint32_t idx = 0;
        uint32_t block = 1;
        do {
            opcode = readOpcode();
            if (opcode >= 0x02 && opcode <= 0x04) {
                int8_t block_type = readVarInt7();
                block++;
            } else if (opcode >= 0x0c && opcode <= 0x0d) {
                uint32_t relative_depth = readVarUint32();
            } else if (opcode == 0x0b) {
                block--;
            } else if (opcode == 0x0e) {
                uint32_t target_count = readVarUint32();
                for (uint32_t target_entry = 0; target_entry < target_count; target_entry++) {
                    uint32_t target_table = readVarUint32();
                }
                uint32_t default_target = readVarUint32();
            } else if (opcode == 0x10) {
                uint32_t function_index = readVarUint32();
            } else if (opcode == 0x11) {
                uint32_t type_index = readVarUint32();
                int8_t reserved = readVarUint1();
            } else if (opcode >= 0x20 && opcode <= 0x24) {
                uint32_t index = readVarUint32();
            } else if (opcode >= 0x28 && opcode <= 0x3e) {
                uint32_t flags = readVarUint32();
                uint32_t offset = readVarUint32();
            } else if (opcode >= 0x3f && opcode <= 0x40) {
                uint8_t reserved = readVarUint1();
            } else if (opcode == 0x41) {
                int32_t value = readVarInt32();
            } else if (opcode == 0x42) {
                int64_t value = readVarInt64();
            } else if (opcode == 0x43) {
                uint32_t value = readUint32();
            } else if (opcode == 0x44) {
                uint64_t value = readUint64();
            }

        } while (block != 0 || opcode != 0xb);
        uint32_t end_body = ftell(fp_);
        if (!((end_body - start_body) == body_size)) return false;
    }

    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

bool ReaderV1::readDataSection(uint32_t payload_len) {
    uint32_t start = ftell(fp_);
    uint32_t count = readVarUint32();
    for (uint32_t entry = 0; entry < count; entry++) {
        uint32_t index = readVarUint32();
        while (true) {
            uint8_t opcode = readOpcode();
            if (opcode == 0x41) {
                readVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                readVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                readBytes(4); // FIXME
            } else if (opcode == 0x44) {
                readBytes(8); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = readVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = ftell(fp_);
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t size = readVarUint32();
        const char* data = readBytes(size);
    }
    uint32_t end = ftell(fp_);
    return ((end - start) == payload_len);
}

uint8_t ReaderV1::readVarUint1() {
    uint8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x1;
}

uint8_t ReaderV1::readVarUint7() {
    uint8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x7f;
}

uint32_t ReaderV1::readVarUint32() {
    uint32_t val = 0;
    uint32_t shift = 0;
    for (int i = 0; i < 5; i++) {
        uint8_t byte = 0;
        fread(&byte, 1, 1, fp_);
        val |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7; 
    }
    return val;
}

int8_t ReaderV1::readVarInt7() {
    int8_t val = 0;
    fread(&val, 1, 1, fp_);
    return val & 0x7f;
}

int32_t ReaderV1::readVarInt32() {
    int32_t val = 0;
    uint32_t shift = 0;
    uint32_t size = 32;
    uint8_t byte = 0;
    do {
        fread(&byte, 1, 1, fp_);
        val |= (byte & 0x7) << shift;
        shift += 7;
    } while (byte & 0x80);

    if ((shift < size) && (byte & 0x40)) {
        val |= (~0 << shift);
    }
    return val;
}

int64_t ReaderV1::readVarInt64() {
    int64_t val = 0;
    uint32_t shift = 0;
    uint32_t size = 64;
    uint8_t byte = 0;
    do {
        fread(&byte, 1, 1, fp_);
        val |= (byte & 0x7) << shift;
        shift += 7;
    } while (byte & 0x80);

    if ((shift < size) && (byte & 0x40)) {
        val |= (~0 << shift);
    }
    return val;
}

uint32_t ReaderV1::readUint32() {
    uint32_t val = 0;
    fread(&val, 1, 4, fp_);
    return val;
}

uint64_t ReaderV1::readUint64() {
    uint64_t val = 0;
    fread(&val, 1, 8, fp_);
    return val;
}

uint8_t ReaderV1::readOpcode() {
    int8_t opcode = 0;
    fread(&opcode, 1, 1, fp_);
    return opcode;
}

const char* ReaderV1::readBytes(uint32_t len) {
    char* val = static_cast<char*>(malloc(len));
    fread(val, 1, len, fp_);
    return val;
}


