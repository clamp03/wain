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
                if (!loadTableSection()) {
                    DEV_ASSERT(false, "Invalid table section");
                }
                break;
            case SectionId::MEMORY:
                if (!loadMemorySection()) {
                    DEV_ASSERT(false, "Invalid memory section");
                }
                break;
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

    type_section_ = new(AllocateClassInstance(mem_, TypeSection)) TypeSection();
    uint32_t type_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < type_count; entry++) {
        FuncType* func = new(AllocateClassInstance(mem_, FuncType)) FuncType();
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

    import_section_ = new(AllocateClassInstance(mem_, ImportSection)) ImportSection();
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
            import_type = new(AllocateClassInstance(mem_, ImportTypeFunction)) ImportTypeFunction(type);
        } else if (kind == ExternalKind::Table) {
            int8_t element_type = loader_.loadVarInt7();
            import_type = new(AllocateClassInstance(mem_, ImportTypeTable)) ImportTypeTable(element_type, loader_.loadResizableLimits());
        } else if (kind == ExternalKind::Memory) {
            import_type = new(AllocateClassInstance(mem_, ImportTypeMemory)) ImportTypeMemory(loader_.loadResizableLimits());
        } else if (kind == ExternalKind::Global) {
            int8_t content_type = loader_.loadVarInt7();
            uint8_t mutability = loader_.loadVarUint1();
            import_type = new(AllocateClassInstance(mem_, ImportTypeGlobal)) ImportTypeGlobal(content_type, mutability);
        } else {
            DEV_ASSERT(false, "INVALID EXTERNAL KIND");
        }
        ImportEntry* import_entry = new(AllocateClassInstance(mem_, ImportEntry)) ImportEntry(module_len, module_str, field_len, field_str, import_type);
        import_section_->addImportEntry(import_entry);
    }
    return true;
}

bool SectionsV1::loadFunctionSection() {
    PayloadChecker checker(loader_);
    uint32_t function_count = loader_.loadVarUint32();
    function_section_ = new(AllocateClassInstance(mem_, FunctionSection)) FunctionSection();
    for (uint32_t entry = 0; entry < function_count; entry++) {
        uint32_t type = loader_.loadVarUint32();
        function_section_->addFuncType(type);
    }
    return true;
}

bool SectionsV1::loadTableSection() {
    PayloadChecker checker(loader_);
    uint32_t table_count = loader_.loadVarUint32();
    table_section_ = new(AllocateClassInstance(mem_, TableSection)) TableSection();
    for (uint32_t entry = 0; entry < table_count; entry++) {
         TableType* table_type = new(AllocateClassInstance(mem_, TableType)) TableType(loader_.loadVarInt7(), loader_.loadResizableLimits());
         table_section_->addTableType(table_type);
    }
    return true;
}

bool SectionsV1::loadMemorySection() {
    PayloadChecker checker(loader_);
    uint32_t memory_count = loader_.loadVarUint32();
    memory_section_ = new(AllocateClassInstance(mem_, MemorySection)) MemorySection();
    for (uint32_t entry = 0; entry < memory_count; entry++) {
         MemoryType* memory_type = new(AllocateClassInstance(mem_, MemoryType)) MemoryType(loader_.loadResizableLimits());
         memory_section_->addMemoryType(memory_type);
    }
    return true;
}

bool SectionsV1::loadGlobalSection() {
    PayloadChecker checker(loader_);
    global_section_ = new(AllocateClassInstance(mem_, GlobalSection)) GlobalSection();
    uint32_t global_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < global_count; entry++) {
        // global_type
        int8_t content_type = loader_.loadVarInt7();
        uint8_t mutability = loader_.loadVarUint1();
        GlobalEntry* global_entry = new(AllocateClassInstance(mem_, GlobalEntry)) GlobalEntry(content_type, mutability, loadInitExpr());
        global_section_->addGlobalVariable(global_entry);
    }
    return true;
}

bool SectionsV1::loadExportSection() {
    PayloadChecker checker(loader_);

    export_section_ = new(AllocateClassInstance(mem_, ExportSection)) ExportSection();
    uint32_t export_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < export_count; entry++) {
        uint32_t field_len = loader_.loadVarUint32();
        uint8_t* field_str = static_cast<uint8_t*>(mem_.allocate(field_len));
        loader_.loadBytes(field_str, field_len);
        ExternalKind kind = static_cast<ExternalKind>(loader_.loadVarUint7());
        uint32_t index = loader_.loadVarUint32();
        ExportEntry* export_entry = new(AllocateClassInstance(mem_, ExportEntry)) ExportEntry(field_len, field_str, kind, index);
        export_section_->addExportEntry(export_entry);
    }

    return true;
}

bool SectionsV1::loadElementSection() {
    PayloadChecker checker(loader_);

    element_section_ = new(AllocateClassInstance(mem_, ElementSection)) ElementSection();
    uint32_t element_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < element_count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        InitExpr* offset = loadInitExpr();
        ElementSegment* segment = new(AllocateClassInstance(mem_, ElementSegment)) ElementSegment(index, offset);
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

    code_section_ = new(AllocateClassInstance(mem_, CodeSection)) CodeSection();

    uint32_t code_count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < code_count; entry++) {
        PayloadChecker bodychecker(loader_);

        FunctionBody* body = new(AllocateClassInstance(mem_, FunctionBody)) FunctionBody();
        uint32_t local_count = loader_.loadVarUint32();
        for (uint32_t local_entry = 0; local_entry < local_count; local_entry++) {
            uint32_t local_entry_count = loader_.loadVarUint32();
            int8_t value_type = loader_.loadVarInt7();
            Local* local = new(AllocateClassInstance(mem_, Local)) Local(local_entry_count, value_type);
            body->addLocal(local);
        }

        uint8_t opcode;
        uint32_t idx = 0;
        uint32_t block = 1;
        do {
            Instruction* inst;
            opcode = loader_.loadOpcode();
            switch (opcode) {
#if 0
            case UnreachableOpcode:
                inst = new(AllocateClassInstance(mem_, Unreachable)) Unreachable();
                break;
            case NopOpcode:
                inst = new(AllocateClassInstance(mem_, Nop)) Nop();
                break;
            case BlockOpcode:
                inst = new(AllocateClassInstance(mem_, Block)) Block(loader_.loadVarInt7());
                block++;
                break;
            case LoopOpcode:
                inst = new(AllocateClassInstance(mem_, Loop)) Loop(loader_.loadVarInt7());
                block++;
                break;
            case IfOpcode:
                inst = new(AllocateClassInstance(mem_, If)) If(loader_.loadVarInt7());
                block++;
                break;
            case ElseOpcode:
                inst = new(AllocateClassInstance(mem_, Else)) Else();
                break;
#endif
            case EndOpcode:
                block--;
                inst = new(AllocateClassInstance(mem_, End)) End();
                break;
#if 0
            case BrOpcode:
                inst = new(AllocateClassInstance(mem_, Br)) Br(loader_.loadVarUint32());
                break;
            case BrIfOpcode:
                inst = new(AllocateClassInstance(mem_, BrIf)) BrIf(loader_.loadVarUint32());
                break;
            case BrTableOpcode: {
                BrTable* brTable = new(AllocateClassInstance(mem_, BrTable)) BrTable();
                uint32_t target_count = loader_.loadVarUint32();
                for (uint32_t target_entry = 0; target_entry < target_count; target_entry++) {
                    brTable->addTargetTable(loader_.loadVarUint32());
                }
                brTable->setDefaultTarget(loader_.loadVarUint32());
                inst = brTable;
                break;
            }
            case ReturnOpcode:
                inst = new(AllocateClassInstance(mem_, Return)) Return();
                break;
#endif
            case CallOpcode:
                inst = new(AllocateClassInstance(mem_, Call)) Call(loader_.loadVarUint32());
                break;
#if 0
            case CallIndirectOpcode:
                inst = new(AllocateClassInstance(mem_, CallIndirect)) CallIndirect(loader_.loadVarUint32(), loader_.loadVarUint1());
                break;
            case DropOpcode:
                inst = new(AllocateClassInstance(mem_, Drop)) Drop();
                break;
#endif
            case GetLocalOpcode:
                inst = new(AllocateClassInstance(mem_, GetLocal)) GetLocal(loader_.loadVarUint32());
                break;
#if 0
            case SetLocalOpcode:
                inst = new(AllocateClassInstance(mem_, SetLocal)) SetLocal(loader_.loadVarUint32());
                break;
            case TeeLocalOpcode:
                inst = new(AllocateClassInstance(mem_, TeeLocal)) TeeLocal(loader_.loadVarUint32());
                break;
            case GetGlobalOpcode:
                inst = new(AllocateClassInstance(mem_, GetGlobal)) GetGlobal(loader_.loadVarUint32());
                break;
            case SetGlobalOpcode:
                inst = new(AllocateClassInstance(mem_, SetGlobal)) SetGlobal(loader_.loadVarUint32());
                break;
            case I32LoadOpcode:
                inst = new(AllocateClassInstance(mem_, I32Load)) I32Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64LoadOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load)) I64Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F32LoadOpcode:
                inst = new(AllocateClassInstance(mem_, F32Load)) F32Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F64LoadOpcode:
                inst = new(AllocateClassInstance(mem_, F64Load)) F64Load(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load8SOpcode:
                inst = new(AllocateClassInstance(mem_, I32Load8S)) I32Load8S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load8UOpcode:
                inst = new(AllocateClassInstance(mem_, I32Load8U)) I32Load8U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load16SOpcode:
                inst = new(AllocateClassInstance(mem_, I32Load16S)) I32Load16S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Load16UOpcode:
                inst = new(AllocateClassInstance(mem_, I32Load16U)) I32Load16U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load8SOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load8S)) I64Load8S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load8UOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load8U)) I64Load8U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load16SOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load16S)) I64Load16S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load16UOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load16U)) I64Load16U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load32SOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load32S)) I64Load32S(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Load32UOpcode:
                inst = new(AllocateClassInstance(mem_, I64Load32U)) I64Load32U(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32StoreOpcode:
                inst = new(AllocateClassInstance(mem_, I32Store)) I32Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64StoreOpcode:
                inst = new(AllocateClassInstance(mem_, I64Store)) I64Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F32StoreOpcode:
                inst = new(AllocateClassInstance(mem_, F32Store)) F32Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case F64StoreOpcode:
                inst = new(AllocateClassInstance(mem_, F64Store)) F64Store(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Store8Opcode:
                inst = new(AllocateClassInstance(mem_, I32Store8)) I32Store8(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I32Store16Opcode:
                inst = new(AllocateClassInstance(mem_, I32Store16)) I32Store16(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store8Opcode:
                inst = new(AllocateClassInstance(mem_, I64Store8)) I64Store8(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store16Opcode:
                inst = new(AllocateClassInstance(mem_, I64Store16)) I64Store16(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case I64Store32Opcode:
                inst = new(AllocateClassInstance(mem_, I64Store32)) I64Store32(loader_.loadVarUint32(), loader_.loadVarUint32());
                break;
            case CurrentMemoryOpcode:
                inst = new(AllocateClassInstance(mem_, CurrentMemory)) CurrentMemory(loader_.loadVarUint1());
                break;
            case GrowMemoryOpcode:
                inst = new(AllocateClassInstance(mem_, GrowMemory)) GrowMemory(loader_.loadVarUint1());
                break;
#endif
            case I32ConstOpcode:
                inst = new(AllocateClassInstance(mem_, I32Const)) I32Const(loader_.loadVarInt32());
                break;
#if 0
            case I64ConstOpcode:
                inst = new(AllocateClassInstance(mem_, I64Const)) I64Const(loader_.loadVarInt64());
                break;
            case F32ConstOpcode:
                inst = new(AllocateClassInstance(mem_, F32Const)) F32Const(loader_.loadUint32());
                break;
            case F64ConstOpcode:
                inst = new(AllocateClassInstance(mem_, F32Const)) F32Const(loader_.loadUint64());
                break;
            case I32EQZOpcode:
                inst = new(AllocateClassInstance(mem_, I32EQZ)) I32EQZ();
                break;
            case I32EQOpcode:
                inst = new(AllocateClassInstance(mem_, I32EQ)) I32EQ();
                break;
            case I32NEOpcode:
                inst = new(AllocateClassInstance(mem_, I32NE)) I32NE();
                break;
            case I32LT_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32LT_S)) I32LT_S();
            case I32LT_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32LT_U)) I32LT_U();
                break;
            case I32GT_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32GT_S)) I32GT_S();
                break;
            case I32GT_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32GT_U)) I32GT_U();
                break;
            case I32LE_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32LE_S)) I32LE_S();
                break;
            case I32LE_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32LE_U)) I32LE_U();
                break;
            case I32GE_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32GE_S)) I32GE_S();
                break;
            case I32GE_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32GE_U)) I32GE_U();
                break;
            case I64EQZOpcode:
                inst = new(AllocateClassInstance(mem_, I64EQZ)) I64EQZ();
                break;
            case I64EQOpcode:
                inst = new(AllocateClassInstance(mem_, I64EQ)) I64EQ();
                break;
            case I64NEOpcode:
                inst = new(AllocateClassInstance(mem_, I64NE)) I64NE();
                break;
            case I64LT_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64LT_S)) I32LT_S();
            case I64LT_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64LT_U)) I32LT_U();
                break;
            case I64GT_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64GT_S)) I32GT_S();
                break;
            case I64GT_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64GT_U)) I32GT_U();
                break;
            case I64LE_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64LE_S)) I32LE_S();
                break;
            case I64LE_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64LE_U)) I32LE_U();
                break;
            case I64GE_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64GE_S)) I32GE_S();
                break;
            case I64GE_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64GE_U)) I32GE_U();
                break;
            case F64EQOpcode:
                inst = new(AllocateClassInstance(mem_, F64EQ)) F64EQ();
                break;
            case F64NEOpcode:
                inst = new(AllocateClassInstance(mem_, F64NE)) F64NE();
                break;
            case I32AddOpcode:
                inst = new(AllocateClassInstance(mem_, I32Add)) I32Add();
                break;
            case I32SubOpcode:
                inst = new(AllocateClassInstance(mem_, I32Sub)) I32Sub();
                break;
            case I32MulOpcode:
                inst = new(AllocateClassInstance(mem_, I32Mul)) I32Mul();
                break;
            case I32Div_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32Div_S)) I32Div_S();
                break;
            case I32Div_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32Div_U)) I32Div_U();
                break;
            case I32Rem_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32Rem_S)) I32Rem_S();
                break;
            case I32Rem_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32Rem_U)) I32Rem_U();
                break;
            case I32AndOpcode:
                inst = new(AllocateClassInstance(mem_, I32And)) I32And();
                break;
            case I32OrOpcode:
                inst = new(AllocateClassInstance(mem_, I32Or)) I32Or();
                break;
            case I32XorOpcode:
                inst = new(AllocateClassInstance(mem_, I32Xor)) I32Xor();
                break;
#endif
            case I32ShlOpcode:
                inst = new(AllocateClassInstance(mem_, I32Shl)) I32Shl();
                break;
#if 0
            case I32Shr_SOpcode:
                inst = new(AllocateClassInstance(mem_, I32Shr_S)) I32Shr_S();
                break;
            case I32Shr_UOpcode:
                inst = new(AllocateClassInstance(mem_, I32Shr_U)) I32Shr_U();
                break;
            case I64AddOpcode:
                inst = new(AllocateClassInstance(mem_, I64Add)) I64Add();
                break;
            case I64SubOpcode:
                inst = new(AllocateClassInstance(mem_, I64Sub)) I64Sub();
                break;
            case I64MulOpcode:
                inst = new(AllocateClassInstance(mem_, I64Mul)) I64Mul();
                break;
            case I64Div_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64Div_S)) I64Div_S();
                break;
            case I64Div_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64Div_U)) I64Div_U();
                break;
            case I64AndOpcode:
                inst = new(AllocateClassInstance(mem_, I64And)) I64And();
                break;
            case I64OrOpcode:
                inst = new(AllocateClassInstance(mem_, I64Or)) I64Or();
                break;
            case I64XorOpcode:
                inst = new(AllocateClassInstance(mem_, I64Xor)) I64Xor();
                break;
            case I64ShlOpcode:
                inst = new(AllocateClassInstance(mem_, I64Xor)) I64Shl();
                break;
            case I64Shr_SOpcode:
                inst = new(AllocateClassInstance(mem_, I64Shr_S)) I64Shr_S();
                break;
            case I64Shr_UOpcode:
                inst = new(AllocateClassInstance(mem_, I64Shr_U)) I64Shr_U();
                break;
            case F64NegOpcode:
                inst = new(AllocateClassInstance(mem_, F64Neg)) F64Neg();
                break;
            case F64AddOpcode:
                inst = new(AllocateClassInstance(mem_, F64Add)) F64Add();
                break;
            case F64SubOpcode:
                inst = new(AllocateClassInstance(mem_, F64Sub)) F64Sub();
                break;
            case F64MulOpcode:
                inst = new(AllocateClassInstance(mem_, F64Mul)) F64Mul();
                break;
            case I32Wrap_I64Opcode:
                inst = new(AllocateClassInstance(mem_, I32Wrap_I64)) I32Wrap_I64();
                break;
            case I32Trunc_S_F64Opcode:
                inst = new(AllocateClassInstance(mem_, I32Trunc_S_F64)) I32Trunc_S_F64();
                break;
            case I32Trunc_U_F64Opcode:
                inst = new(AllocateClassInstance(mem_, I32Trunc_U_F64)) I32Trunc_U_F64();
                break;
            case I64Extend_S_I32Opcode:
                inst = new(AllocateClassInstance(mem_, I64Extend_S_I32)) I64Extend_S_I32();
                break;
            case I64Extend_U_I32Opcode:
                inst = new(AllocateClassInstance(mem_, I64Extend_U_I32)) I64Extend_U_I32();
                break;
            case F64Convert_S_I32Opcode:
                inst = new(AllocateClassInstance(mem_, F64Convert_S_I32)) F64Convert_S_I32();
                break;
            case F64Convert_U_I32Opcode:
                inst = new(AllocateClassInstance(mem_, F64Convert_U_I32)) F64Convert_U_I32();
                break;
            case I64Reinterpret_F64Opcode:
                inst = new(AllocateClassInstance(mem_, I64Reinterpret_F64)) I64Reinterpret_F64();
                break;
            case F64Reinterpret_I64Opcode:
                inst = new(AllocateClassInstance(mem_, F64Reinterpret_I64)) F64Reinterpret_I64();
                break;
#endif
            default:
                NOT_YET_IMPLEMENTED
            }
            body->addInstruction(inst);
        } while (block != 0 || opcode != 0xb);
        code_section_->addFunctionBody(body);
    }

    return true;
}

bool SectionsV1::loadDataSection() {
    PayloadChecker checker(loader_);

    data_section_ = new(AllocateClassInstance(mem_, DataSection)) DataSection();

    uint32_t count = loader_.loadVarUint32();
    for (uint32_t entry = 0; entry < count; entry++) {
        uint32_t index = loader_.loadVarUint32();
        InitExpr* offset = loadInitExpr();
        uint32_t size = loader_.loadVarUint32();
        uint8_t* data = static_cast<uint8_t*>(mem_.allocate(size));
        loader_.loadBytes(data, size);
        DataSegment* segment = new(AllocateClassInstance(mem_, DataSegment)) DataSegment(index, offset, size, data);
        data_section_->addDataSegment(segment);
    }
    return true;
}

InitExpr* SectionsV1::loadInitExpr() {
    InitExpr* init = new(AllocateClassInstance(mem_, InitExpr)) InitExpr();
    while (true) {
        Instruction* inst;
        uint8_t opcode = loader_.loadOpcode();
        switch (opcode) {
        case I32ConstOpcode:
            inst = new(AllocateClassInstance(mem_, I32Const)) I32Const(loader_.loadVarInt32());
            break;
        case I64ConstOpcode:
            inst = new(AllocateClassInstance(mem_, I64Const)) I64Const(loader_.loadVarInt64());
            break;
        case F32ConstOpcode:
            inst = new(AllocateClassInstance(mem_, F32Const)) F32Const(loader_.loadUint32());
            break;
        case F64ConstOpcode:
            inst = new(AllocateClassInstance(mem_, F64Const)) F64Const(loader_.loadUint64());
            break;
        case GetGlobalOpcode:
            inst = new(AllocateClassInstance(mem_, GetGlobal)) GetGlobal(loader_.loadVarUint32());
            break;
        case EndOpcode:
            inst = new(AllocateClassInstance(mem_, End)) End();
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

#define DefineGetSectionFunc(type, field)            \
    type##Section* SectionsV1::get##type##Section() {\
        return field##_section_;                     \
    }
DefineGetSectionFunc(Type, type);
DefineGetSectionFunc(Import, import);
DefineGetSectionFunc(Function, function);
DefineGetSectionFunc(Table, table);
DefineGetSectionFunc(Memory, memory);
DefineGetSectionFunc(Global, global);
DefineGetSectionFunc(Export, export);
DefineGetSectionFunc(Element, element);
DefineGetSectionFunc(Code, code);
DefineGetSectionFunc(Data, data);
#undef DefineGetSectionFunc
