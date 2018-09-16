#include "common.h"
#include "section_v1.h"

SectionsV1::SectionsV1(Loader& l)
    : loader_(l)
    , type_section_(nullptr)
    , import_section_(nullptr)
    , function_section_(nullptr)
    , global_section_(nullptr)
    , export_section_(nullptr) {
}

SectionsV1::~SectionsV1() {
}

bool SectionsV1::load() {
    uint8_t prev_id = 0;
    while (loader_.index() < loader_.size()) {
        uint8_t id = loader_.loadVarUint7();

        DEV_ASSERT(id >= prev_id || id == 0, "INVALID SECTION ID");
        switch (static_cast<SectionId>(id)) {
            case SectionId::NAME:
                NOT_YET_IMPLEMENTED
            case SectionId::TYPE:
                if (!loadTypeSection()) {
                    DEV_ASSERT(false, "Invalid type section");
                }
                break;
            case SectionId::IMPORT:
                if (!loadImportSection()) {
                    DEV_ASSERT(false, "Invalid import section");
                }
                break;
            case SectionId::FUNCTION:
                if (!loadFunctionSection()) {
                    DEV_ASSERT(false, "Invalid function section");
                }
                break;
            case SectionId::TABLE:
                NOT_YET_IMPLEMENTED
            case SectionId::MEMORY:
                NOT_YET_IMPLEMENTED
            case SectionId::GLOBAL:
                if (!loadGlobalSection()) {
                    DEV_ASSERT(false, "Invalid global section");
                }
                break;
            case SectionId::EXPORT:
                if (!loadExportSection()) {
                    DEV_ASSERT(false, "Invalid export section");
                }
                break;
            case SectionId::START:
                NOT_YET_IMPLEMENTED
            case SectionId::ELEMENT:
                if (!loadElementSection()) {
                    DEV_ASSERT(false, "Invalid element section");
                }
                break;
            case SectionId::CODE:
                if (!loadCodeSection()) {
                    DEV_ASSERT(false, "Invalid code section");
                }
                break;
            case SectionId::DATA:
                if (!loadDataSection()) {
                    DEV_ASSERT(false, "Invalid data section");
                }
                break;
            default:
                DEV_ASSERT(false, "Invalid section");
                NOT_YET_IMPLEMENTED
        }
        prev_id = id;
    }
    return true;
}

class PayloadChecker {
public:
    explicit PayloadChecker(Loader& l)
            : loader_(l) {
        payload_len_ = loader_.loadVarUint32();
        start_ = loader_.index();
    }

    ~PayloadChecker() {
        uint32_t end = loader_.index();
        DEV_ASSERT((end - start_ == payload_len_), "Invalid payload len");
    }

private:
    Loader& loader_;
    size_t payload_len_;
    size_t start_;
};

bool SectionsV1::loadTypeSection() {
    PayloadChecker checker(loader_);

    type_section_ = new TypeSection();
    uint32_t type_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < type_count; entry++) {
        FuncType* func = new FuncType();
        int8_t form = loader_.loadVarInt7();
        func->setForm(form);

        uint32_t param_count = loader_.loadVarUint32();
        for (uint32_t param = 0; param < param_count; param++) {
            ValueType value_type = static_cast<ValueType>(loader_.loadVarInt7());
            func->addParamType(value_type);
        }
        uint8_t return_count = loader_.loadVarUint1();
        if (return_count) {
            ValueType return_type = static_cast<ValueType>(loader_.loadVarInt7());
            func->setReturnType(return_type);
        }
        type_section_->addFuncType(func);
    }
    return true;
}

bool SectionsV1::loadImportSection() {
    PayloadChecker checker(loader_);

    import_section_ = new ImportSection();
    uint32_t import_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < import_count; entry++) {
        uint32_t module_len = loader_.loadVarUint32();
        uint8_t* module_str = static_cast<uint8_t*>(mem_.allocate(module_len));
        loader_.loadBytes(module_str, module_len);

        uint32_t field_len = loader_.loadVarUint32();
        uint8_t* field_str = static_cast<uint8_t*>(mem_.allocate(field_len));
        loader_.loadBytes(field_str, field_len);
        ExternalKind kind = static_cast<ExternalKind>(loader_.loadVarUint7());
        ImportType* import_type;
        if (kind == ExternalKind::Function) {
            uint32_t type = loader_.loadVarUint32();
            import_type = new ImportTypeFunction(type);
        } else if (kind == ExternalKind::Table) {
            int8_t element_type = loader_.loadVarInt7();
            import_type = new ImportTypeTable(element_type, loader_.loadResizableLimits()); // TODO: Check memory for ResizableLimits reference passing
        } else if (kind == ExternalKind::Memory) {
            import_type = new ImportTypeMemory(loader_.loadResizableLimits()); // TODO: Check memory for ResizableLimits reference passing
        } else if (kind == ExternalKind::Global) {
            int8_t content_type = loader_.loadVarInt7();
            uint8_t mutability = loader_.loadVarUint1();
            import_type = new ImportTypeGlobal(content_type, mutability);
        } else {
            DEV_ASSERT(false, "INVALID EXTERNAL KIND");
        }
        ImportEntry* import_entry = new ImportEntry(module_len, module_str, field_len, field_str, import_type);
        import_section_->addImportEntry(import_entry);
    }
    return true;
}

bool SectionsV1::loadFunctionSection() {
    PayloadChecker checker(loader_);
    uint32_t function_count = loader_.loadVarUint32();
    function_section_ = new FunctionSection();
    for (uint32_t entry = 0; entry < function_count; entry++) {
        uint32_t type = loader_.loadVarUint32();
        function_section_->addFuncType(type);
    }
    return true;
}

bool SectionsV1::loadGlobalSection() {
    PayloadChecker checker(loader_);
    uint32_t global_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < global_count; entry++) {
        // global_type
        int8_t content_type = loader_.loadVarInt7();
        uint8_t mutability = loader_.loadVarUint1();
        // TODO init init_expr
        while (true) {
            uint8_t opcode = loader_.loadOpcode();
            if (opcode == 0x41) {
                loader_.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                loader_.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                loader_.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                loader_.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = loader_.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
    }
    return true;
}

bool SectionsV1::loadExportSection() {
    PayloadChecker checker(loader_);

    export_section_ = new ExportSection();
    uint32_t export_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < export_count; entry++) {
        uint32_t field_len = loader_.loadVarUint32();
        uint8_t* field_str = static_cast<uint8_t*>(mem_.allocate(field_len));
        loader_.loadBytes(field_str, field_len);
        ExternalKind kind = static_cast<ExternalKind>(loader_.loadVarUint7());
        uint32_t index = loader_.loadVarUint32();
        ExportEntry* export_entry = new ExportEntry(field_len, field_str, kind, index);
        export_section_->addExportEntry(export_entry);
    }

    return true;
}

bool SectionsV1::loadElementSection() {
    PayloadChecker checker(loader_);
    uint32_t element_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < element_count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        while (true) {
            uint8_t opcode = loader_.loadOpcode();
            if (opcode == 0x41) {
                loader_.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                loader_.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                loader_.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                loader_.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = loader_.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t num_elem = loader_.loadVarUint32();
        for (uint32_t elem_entry = 0; elem_entry < num_elem; elem_entry++) {
            uint32_t elem = loader_.loadVarUint32();
        }
    }
    return true;
}

bool SectionsV1::loadCodeSection() {
    PayloadChecker checker(loader_);
    uint32_t code_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < code_count; entry++) {
        PayloadChecker bodychecker(loader_);
        uint32_t local_count = loader_.loadVarUint32();
        for (uint32_t local_entry = 0; local_entry < local_count; local_entry++) {
            uint32_t local_entry_count = loader_.loadVarUint32();
            int8_t value_type = loader_.loadVarInt7();
        }

        uint8_t opcode;
        uint32_t idx = 0;
        uint32_t block = 1;
        do {
            opcode = loader_.loadOpcode();
            if (opcode >= 0x02 && opcode <= 0x04) {
                int8_t block_type = loader_.loadVarInt7();
                block++;
            } else if (opcode >= 0x0c && opcode <= 0x0d) {
                uint32_t relative_depth = loader_.loadVarUint32();
            } else if (opcode == 0x0b) {
                block--;
            } else if (opcode == 0x0e) {
                uint32_t target_count = loader_.loadVarUint32();
                for (uint32_t target_entry = 0; target_entry < target_count; target_entry++) {
                    uint32_t target_table = loader_.loadVarUint32();
                }
                uint32_t default_target = loader_.loadVarUint32();
            } else if (opcode == 0x10) {
                uint32_t function_index = loader_.loadVarUint32();
            } else if (opcode == 0x11) {
                uint32_t type_index = loader_.loadVarUint32();
                int8_t reserved = loader_.loadVarUint1();
            } else if (opcode >= 0x20 && opcode <= 0x24) {
                uint32_t index = loader_.loadVarUint32();
            } else if (opcode >= 0x28 && opcode <= 0x3e) {
                uint32_t flags = loader_.loadVarUint32();
                uint32_t offset = loader_.loadVarUint32();
            } else if (opcode >= 0x3f && opcode <= 0x40) {
                uint8_t reserved = loader_.loadVarUint1();
            } else if (opcode == 0x41) {
                int32_t value = loader_.loadVarInt32();
            } else if (opcode == 0x42) {
                int64_t value = loader_.loadVarInt64();
            } else if (opcode == 0x43) {
                uint32_t value = loader_.loadUint32();
            } else if (opcode == 0x44) {
                uint64_t value = loader_.loadUint64();
            }

        } while (block != 0 || opcode != 0xb);
    }

    return true;
}

bool SectionsV1::loadDataSection() {
    PayloadChecker checker(loader_);

    uint32_t count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        while (true) {
            uint8_t opcode = loader_.loadOpcode();
            if (opcode == 0x41) {
                loader_.loadVarInt32(); // FIXME
            } else if (opcode == 0x42) {
                loader_.loadVarInt64(); // FIXME
            } else if (opcode == 0x43) {
                loader_.loadUint32(); // FIXME
            } else if (opcode == 0x44) {
                loader_.loadUint64(); // FIXME
            } else if (opcode == 0x23) {
                uint32_t global_index = loader_.loadVarUint32(); // FIXME
            } else if (opcode == 0x0b) {
                break;
            } else {
                DEV_ASSERT(false, "Invalid opcode in global section");
            }
        }
        uint32_t size = loader_.loadVarUint32();
        char* data = static_cast<char*>(mem_.allocate(size));
        loader_.loadBytes(data, size);
    }
    return true;
}
