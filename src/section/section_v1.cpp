#include "common.h"
#include "section_v1.h"
#include "instruction.h"

SectionsV1::SectionsV1(Loader& l)
        : loader_(l)
        , type_section_(nullptr)
        , import_section_(nullptr)
        , function_section_(nullptr)
        , global_section_(nullptr)
        , export_section_(nullptr)
        , element_section_(nullptr)
        , code_section_(nullptr)
        , data_section_(nullptr) {
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
    global_section_ = new GlobalSection();
    uint32_t global_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < global_count; entry++) {
        // global_type
        int8_t content_type = loader_.loadVarInt7();
        uint8_t mutability = loader_.loadVarUint1();
        GlobalEntry* global_entry = new GlobalEntry(content_type, mutability, loadInitExpr());
        global_section_->addGlobalVariable(global_entry);
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

    element_section_ = new ElementSection();
    uint32_t element_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < element_count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        InitExpr* offset = loadInitExpr();
        ElementSegment* segment = new ElementSegment(index, offset);
        uint32_t num_elem = loader_.loadVarUint32();
        for (uint32_t elem_entry = 0; elem_entry < num_elem; elem_entry++) {
            uint32_t elem = loader_.loadVarUint32();
            segment->addElement(elem);
        }
        element_section_->addElementSegment(segment);
    }
    return true;
}

bool SectionsV1::loadCodeSection() {
    PayloadChecker checker(loader_);

    code_section_ = new CodeSection();

    uint32_t code_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < code_count; entry++) {
        PayloadChecker bodychecker(loader_);

        FunctionBody* body = new FunctionBody();
        uint32_t local_count = loader_.loadVarUint32();
        for (uint32_t local_entry = 0; local_entry < local_count; local_entry++) {
            uint32_t local_entry_count = loader_.loadVarUint32();
            int8_t value_type = loader_.loadVarInt7();
            body->addLocal(local_entry_count, value_type);
        }

        uint8_t opcode;
        uint32_t idx = 0;
        uint32_t block = 1;
        do {
            Instruction* inst;
            opcode = loader_.loadOpcode();
            switch (opcode) {
            case BlockOpcode:
                inst = new Block(loader_.loadVarInt7());
                block++;
                break;
            case LoopOpcode:
                inst = new Loop(loader_.loadVarInt7());
                block++;
                break;
            case IfOpcode:
                inst = new If(loader_.loadVarInt7());
                block++;
                break;
            case EndOpcode:
                block--;
                inst = new End();
                break;
            case BrOpcode:
                inst = new Br(loader_.loadVarUint32());
                break;
            case BrIfOpcode:
                inst = new BrIf(loader_.loadVarUint32());
                break;
            case BrTableOpcode: {
                BrTable* brTable = new BrTable();
                uint32_t target_count = loader_.loadVarUint32();
                for (uint32_t target_entry = 0; target_entry < target_count; target_entry++) {
                    brTable->addTargetTable(loader_.loadVarUint32());
                }
                brTable->setDefaultTarget(loader_.loadVarUint32());
                inst = brTable;
                break;
            }
            case CallOpcode:
                inst = new Call(loader_.loadVarUint32());
                break;
            case CallIndirectOpcode:
                inst = new CallIndirect(loader_.loadVarUint32(), loader_.loadVarUint1());
                break;
            case GetLocalOpcode:
                inst = new GetLocal(loader_.loadVarUint32());
                break;
            case SetLocalOpcode:
                inst = new SetLocal(loader_.loadVarUint32());
                break;
            case TeeLocalOpcode:
                inst = new TeeLocal(loader_.loadVarUint32());
                break;
            case GetGlobalOpcode:
                inst = new GetGlobal(loader_.loadVarUint32());
                break;
            case SetGlobalOpcode:
                inst = new SetGlobal(loader_.loadVarUint32());
                break;
            case I32LoadOpcode:
                inst = new I32Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64LoadOpcode:
                inst = new I64Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F32LoadOpcode:
                inst = new F32Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F64LoadOpcode:
                inst = new F64Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load8SOpcode:
                inst = new I32Load8S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load8UOpcode:
                inst = new I32Load8U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load16SOpcode:
                inst = new I32Load16S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load16UOpcode:
                inst = new I32Load16U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load8SOpcode:
                inst = new I64Load8S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load8UOpcode:
                inst = new I64Load8U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load16SOpcode:
                inst = new I64Load16S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load16UOpcode:
                inst = new I64Load16U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load32SOpcode:
                inst = new I64Load32S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load32UOpcode:
                inst = new I64Load32U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32StoreOpcode:
                inst = new I32Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64StoreOpcode:
                inst = new I64Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F32StoreOpcode:
                inst = new F32Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F64StoreOpcode:
                inst = new F64Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Store8Opcode:
                inst = new I32Store8(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Store16Opcode:
                inst = new I32Store16(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store8Opcode:
                inst = new I64Store8(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store16Opcode:
                inst = new I64Store16(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store32Opcode:
                inst = new I64Store32(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case CurrentMemoryOpcode:
                inst = new CurrentMemory(loader_.loadVarUint1());
                break;
            case GrowMemoryOpcode:
                inst = new GrowMemory(loader_.loadVarUint1());
                break;
            case I32ConstOpcode:
                inst = new I32Const(loader_.loadVarInt32());
                break;
            case I64ConstOpcode:
                inst = new I64Const(loader_.loadVarInt64());
                break;
            case F32ConstOpcode:
                inst = new F32Const(loader_.loadUint32());
                break;
            case F64ConstOpcode:
                inst = new F32Const(loader_.loadUint64());
                break;
            }

            body->addInstruction(inst);
        } while (block != 0 || opcode != 0xb);
    }

    return true;
}

bool SectionsV1::loadDataSection() {
    PayloadChecker checker(loader_);

    data_section_ = new DataSection();

    uint32_t count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        InitExpr* offset = loadInitExpr();
        uint32_t size = loader_.loadVarUint32();
        uint8_t* data = static_cast<uint8_t*>(mem_.allocate(size));
        loader_.loadBytes(data, size);
        DataSegment* segment = new DataSegment(index, offset, size, data);
        data_section_->addDataSegment(segment);
    }
    return true;
}

InitExpr* SectionsV1::loadInitExpr() {
    InitExpr* init = new InitExpr();
    while (true) {
        Instruction* inst;
        uint8_t opcode = loader_.loadOpcode();
        switch (opcode) {
        case I32ConstOpcode:
            inst = new I32Const(loader_.loadVarInt32());
            break;
        case I64ConstOpcode:
            inst = new I64Const(loader_.loadVarInt64());
            break;
        case F32ConstOpcode:
            inst = new F32Const(loader_.loadUint32());
            break;
        case F64ConstOpcode:
            inst = new F64Const(loader_.loadUint64());
            break;
        case GetGlobalOpcode:
            inst = new GetGlobal(loader_.loadVarUint32());
            break;
        case EndOpcode:
            inst = new End();
            break;
        default:
            DEV_ASSERT(false, "Invalid opcode in global section");
        }
        init->addInstruction(inst);
        if (opcode == EndOpcode) {
            break;
        }
    }
    return init;
}
