#include <iostream>
#include "section.h"

using namespace std;

bool SectionsV1::load(Loader& l) {
    uint8_t prev_id = 0;
    while (l.index() < l.size()) {
        uint8_t id = l.loadVarUint7();
        // uint32_t payload_len = l.loadVarUint32();

        DEV_ASSERT(id >= prev_id || id == 0, "INVALID SECTION ID");
        switch (static_cast<SectionId>(id)) {
            case SectionId::NAME:
                NOT_YET_IMPLEMENTED
                /*
                if (id == 0) {
                    uint32_t name_len = 0;
                    uint32_t name_start = l.index();
                    name_len = l.loadVarUint32();
                    char* name = static_cast<char*>(mem.allocate(module_len));
                    l.loadBytes(name, name_len);
                    uint32_t name_end = l.index();
                    payload_len -= (name_end - name_start);
                }
                */

            case SectionId::TYPE:
                if (!loadTypeSection(l)) {
                    DEV_ASSERT(false, "Invalid type section");
                }
                break;
            case SectionId::IMPORT:
                if (!loadImportSection(l)) {
                    DEV_ASSERT(false, "Invalid import section");
                }
                break;
            case SectionId::FUNCTION:
                if (!loadFunctionSection(l)) {
                    DEV_ASSERT(false, "Invalid function section");
                }
                break;
            case SectionId::TABLE:
                NOT_YET_IMPLEMENTED
            case SectionId::MEMORY:
                NOT_YET_IMPLEMENTED
            case SectionId::GLOBAL:
                if (!loadGlobalSection(l)) {
                    DEV_ASSERT(false, "Invalid global section");
                }
                break;
            case SectionId::EXPORT:
                if (!loadExportSection(l)) {
                    DEV_ASSERT(false, "Invalid export section");
                }
                break;
            case SectionId::START:
                NOT_YET_IMPLEMENTED
            case SectionId::ELEMENT:
                if (!loadElementSection(l)) {
                    DEV_ASSERT(false, "Invalid element section");
                }
                break;
            case SectionId::CODE:
                if (!loadCodeSection(l)) {
                    DEV_ASSERT(false, "Invalid code section");
                }
                break;
            case SectionId::DATA:
                if (!loadDataSection(l)) {
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

bool SectionsV1::loadTypeSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t type_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < type_count; entry++) {
        int8_t form = l.loadVarInt7();
        uint32_t param_count = l.loadVarUint32();
        for (uint32_t param = 0; param < param_count; param++) {
            int8_t value_type = l.loadVarInt7();
            // TODO
        }
        uint8_t return_count = l.loadVarUint1();
        if (return_count) {
            int8_t return_type = l.loadVarInt7();
            // TODO
        }
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadImportSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t import_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < import_count; entry++) {
        uint32_t module_len = l.loadVarUint32();
        char* module_str = static_cast<char*>(mem.allocate(module_len));
        l.loadBytes(module_str, module_len);

        uint32_t field_len = l.loadVarUint32();
        char* field_str = static_cast<char*>(mem.allocate(field_len));
        l.loadBytes(field_str, field_len);
        ExternalKind kind = static_cast<ExternalKind>(l.loadVarUint7());
        if (kind == ExternalKind::Function) {
            uint32_t type = l.loadVarUint32();
        } else if (kind == ExternalKind::Table) {
            // element_type
            int8_t element_type = l.loadVarInt7();
            // limits
            uint8_t flags = l.loadVarUint1();
            uint32_t initial = l.loadVarUint32();
            if (flags) {
                uint32_t maximum = l.loadVarUint32();
            }
        } else if (kind == ExternalKind::Memory) {
            // limits
            uint8_t flags = l.loadVarUint1();
            uint32_t initial = l.loadVarUint32();
            if (flags) {
                uint32_t maximum = l.loadVarUint32();
            }
        } else if (kind == ExternalKind::Global) {
            int8_t content_type = l.loadVarInt7();
            uint8_t mutability = l.loadVarUint1();
        } else {
            DEV_ASSERT(false, "INVALID EXTERNAL KIND");
        }
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadFunctionSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t function_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < function_count; entry++) {
        uint32_t type = l.loadVarUint32();
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadGlobalSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t global_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < global_count; entry++) {
        // global_type
        int8_t content_type = l.loadVarInt7();
        uint8_t mutability = l.loadVarUint1();
        // TODO init init_expr
        while (true) {
            uint8_t opcode = l.loadOpcode();
            if (opcode == 0x41) {
                l.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                l.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                l.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                l.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = l.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = l.index();
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadExportSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t export_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < export_count; entry++) {
        uint32_t field_len = l.loadVarUint32();
        char* field_str = static_cast<char*>(mem.allocate(field_len));
        l.loadBytes(field_str, field_len);
        ExternalKind kind = static_cast<ExternalKind>(l.loadVarUint7());
        uint32_t index = l.loadVarUint32();
    }

    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadElementSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t element_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < element_count; entry++) {
        uint32_t index = l.loadVarUint32();
        while (true) {
            uint8_t opcode = l.loadOpcode();
            if (opcode == 0x41) {
                l.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                l.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                l.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                l.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = l.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = l.index();
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t num_elem = l.loadVarUint32();
        for (uint32_t elem_entry = 0; elem_entry < num_elem; elem_entry++) {
            uint32_t elem = l.loadVarUint32();
        }
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadCodeSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t code_count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < code_count; entry++) {
        uint32_t body_size = l.loadVarUint32();
        uint32_t start_body = l.index();
        uint32_t local_count = l.loadVarUint32();
        for (uint32_t local_entry = 0; local_entry < local_count; local_entry++) {
            uint32_t local_entry_count = l.loadVarUint32();
            int8_t value_type = l.loadVarInt7();
        }

        uint8_t opcode;
        uint32_t idx = 0;
        uint32_t block = 1;
        do {
            opcode = l.loadOpcode();
            if (opcode >= 0x02 && opcode <= 0x04) {
                int8_t block_type = l.loadVarInt7();
                block++;
            } else if (opcode >= 0x0c && opcode <= 0x0d) {
                uint32_t relative_depth = l.loadVarUint32();
            } else if (opcode == 0x0b) {
                block--;
            } else if (opcode == 0x0e) {
                uint32_t target_count = l.loadVarUint32();
                for (uint32_t target_entry = 0; target_entry < target_count; target_entry++) {
                    uint32_t target_table = l.loadVarUint32();
                }
                uint32_t default_target = l.loadVarUint32();
            } else if (opcode == 0x10) {
                uint32_t function_index = l.loadVarUint32();
            } else if (opcode == 0x11) {
                uint32_t type_index = l.loadVarUint32();
                int8_t reserved = l.loadVarUint1();
            } else if (opcode >= 0x20 && opcode <= 0x24) {
                uint32_t index = l.loadVarUint32();
            } else if (opcode >= 0x28 && opcode <= 0x3e) {
                uint32_t flags = l.loadVarUint32();
                uint32_t offset = l.loadVarUint32();
            } else if (opcode >= 0x3f && opcode <= 0x40) {
                uint8_t reserved = l.loadVarUint1();
            } else if (opcode == 0x41) {
                int32_t value = l.loadVarInt32();
            } else if (opcode == 0x42) {
                int64_t value = l.loadVarInt64();
            } else if (opcode == 0x43) {
                uint32_t value = l.loadUint32();
            } else if (opcode == 0x44) {
                uint64_t value = l.loadUint64();
            }

        } while (block != 0 || opcode != 0xb);
        uint32_t end_body = l.index();
        if (!((end_body - start_body) == body_size)) return false;
    }

    uint32_t end = l.index();
    return ((end - start) == payload_len);
}

bool SectionsV1::loadDataSection(Loader& l) {
    uint32_t payload_len = l.loadVarUint32();
    uint32_t start = l.index();
    uint32_t count = l.loadVarUint32();
    for (uint32_t entry = 0; entry < count; entry++) {
        uint32_t index = l.loadVarUint32();
        while (true) {
            uint8_t opcode = l.loadOpcode();
            if (opcode == 0x41) {
                l.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                l.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                l.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                l.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = l.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                uint32_t mid = l.index();
                cerr << "[" << entry << "] opcode: " << (int)opcode << " " << (mid - start) << endl;
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t size = l.loadVarUint32();
        char* data = static_cast<char*>(mem.allocate(size));
        l.loadBytes(data, size);
    }
    uint32_t end = l.index();
    return ((end - start) == payload_len);
}
