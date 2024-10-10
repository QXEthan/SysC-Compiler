#include "IRVisitor.hpp"
#include "RegisterManager.hpp"

using namespace std;

unique_ptr<AssemblyRiscV> encapAssembly(int param1, RegisterType param1_t, int param2, RegisterType param2_t, int ret, RegisterType ret_t, string name) {
    std::unique_ptr<AssemblyRiscV> assemblyRiscV = make_unique<AssemblyRiscV>();
    assemblyRiscV->name = make_unique<string>(name);
    assemblyRiscV->paramR1 = make_unique<int>(param1);
    assemblyRiscV->paramR1_t = make_unique<int>(param1_t);
    assemblyRiscV->paramR2 = make_unique<int>(param2);
    assemblyRiscV->paramR2_t = make_unique<int>(param2_t);
    assemblyRiscV->retR = make_unique<int>(ret);
    assemblyRiscV->retR_t = make_unique<int>(ret_t);
    return assemblyRiscV;
};

unique_ptr<BaseRiscV> ValueIRVisitor::visit(const BaseIR *baseNode) const {
    cout << "In ValueIRVisitor" << endl;
    const auto* node = dynamic_cast<const ValueIR*>(baseNode);
    unique_ptr<AssemblySetRiscV> setRiscV = make_unique<AssemblySetRiscV>();
    setRiscV->instructions = make_unique<std::vector<unique_ptr<AssemblyRiscV>>>();

    // not, neg
    if (node->isRet) {
        // ret
        std::unique_ptr<AssemblyRiscV> assemblyRiscV = 
                encapAssembly(rm.allocate_a(), R_A_REGISTER, 
                                rm.currentFree() - 1, R_T_REGISTER, 0, R_NOT_USED, "mov");
        setRiscV->instructions->push_back(std::move(assemblyRiscV));

        std::unique_ptr<AssemblyRiscV> assemblyRiscV2 = 
                encapAssembly(0, R_NOT_USED, 0, R_NOT_USED, 0, R_NOT_USED, "ret");
        setRiscV->instructions->push_back(std::move(assemblyRiscV2));    
        cout << "RET assembly end" << endl;
    }
    else if (*(node->op) == "eq") {
        // eq
        if (node->isLeftVar == false) {
            // li
            std::unique_ptr<AssemblyRiscV> assemblyRiscV = encapAssembly(rm.allocate_t(), R_T_REGISTER, 
                                *(node->left), R_NUMBER, 0, R_NOT_USED, "li");
            setRiscV->instructions->push_back(std::move(assemblyRiscV));
        }

        std::unique_ptr<AssemblyRiscV> assemblyRiscV2 = 
                encapAssembly(rm.currentFree() - 1, R_T_REGISTER, 
                                0, R_X_REGISTER, rm.currentFree() - 1, R_T_REGISTER, "xor");
        setRiscV->instructions->push_back(std::move(assemblyRiscV2));

    }
    else {
        // prepare assignment and optimization
        int leftR = rm.currentFree() - 2;
        int rightR = rm.currentFree() - 1;
        bool hasAssign = false;
        if (node->isLeftVar == false && *(node->left) != 0) {
            // li
            std::unique_ptr<AssemblyRiscV> assemblyRiscV = 
                encapAssembly(rm.allocate_t(), R_T_REGISTER, 
                                *(node->left), R_NUMBER, 0, R_NOT_USED, "li");
            setRiscV->instructions->push_back(std::move(assemblyRiscV));
            leftR = rm.currentFree() - 1;
            hasAssign = true;
        }

        if (node->isRightVar == false && *(node->right) != 0) {
            // li
            std::unique_ptr<AssemblyRiscV> assemblyRiscV = 
                encapAssembly(rm.allocate_t(), R_T_REGISTER, 
                                *(node->right), R_NUMBER, 0, R_NOT_USED, "li");
            setRiscV->instructions->push_back(std::move(assemblyRiscV));
            rightR = rm.currentFree() - 1;
            if (leftR != (rightR - 1)) {
                leftR = rightR - 1;
            }
            hasAssign = true;
        }
        // encap instruction
        if (*(node->op) == "and") {
            std::unique_ptr<AssemblyRiscV> assemblyRiscV = 
                encapAssembly(leftR, R_T_REGISTER, leftR, R_T_REGISTER, 0, R_NOT_USED, "snez");
            std::unique_ptr<AssemblyRiscV> assemblyRiscV2 = 
                encapAssembly(rightR, R_T_REGISTER, rightR, R_T_REGISTER, 0, R_NOT_USED, "snez");
            std::unique_ptr<AssemblyRiscV> assemblyRiscV3 = 
                encapAssembly(((*(node->left) == 0 && node->isLeftVar == false)? 0: leftR), 
                        ((*(node->left) == 0 && node->isLeftVar == false)? R_X_REGISTER: R_T_REGISTER), 
                        (*(node->right) == 0 && node->isRightVar == false)? 0: rightR,
                        (*(node->right) == 0 && node->isRightVar == false)? R_X_REGISTER: R_T_REGISTER,
                        hasAssign ? (rm.currentFree() - 1) : rm.allocate_t(), R_T_REGISTER, *(node->op));
            setRiscV->instructions->push_back(std::move(assemblyRiscV));
            setRiscV->instructions->push_back(std::move(assemblyRiscV2));
            setRiscV->instructions->push_back(std::move(assemblyRiscV3));
        } else {
            unique_ptr<string> op;
            if (*(node->op) == "eq" || *(node->op) == "ne") {
                op = make_unique<string>("xor");
            }
            else if (*(node->op) == "lt" || *(node->op) == "ge") {
                op = make_unique<string>("slt");
            }
            else if (*(node->op) == "gt" || *(node->op) == "le") {
                op = make_unique<string>("sgt");
            }
            else {
                op = make_unique<string>(*(node->op));
            }
            std::unique_ptr<AssemblyRiscV> assemblyRiscV = 
                encapAssembly(((*(node->left) == 0 && node->isLeftVar == false)? 0: leftR), 
                        ((*(node->left) == 0 && node->isLeftVar == false)? R_X_REGISTER: R_T_REGISTER), 
                        (*(node->right) == 0 && node->isRightVar == false)? 0: rightR,
                        (*(node->right) == 0 && node->isRightVar == false)? R_X_REGISTER: R_T_REGISTER,
                        hasAssign ? (rm.currentFree() - 1) : rm.allocate_t(), R_T_REGISTER, *op);
            setRiscV->instructions->push_back(std::move(assemblyRiscV));
            if (*(node->op) == "eq" || *(node->op) == "le" || *(node->op) == "ge") {
                std::unique_ptr<AssemblyRiscV> assemblyRiscV2 = 
                    encapAssembly(rm.currentFree() - 1, R_T_REGISTER, 
                                rm.currentFree() - 1, R_T_REGISTER, 0, R_NOT_USED, "seqz");

                setRiscV->instructions->push_back(std::move(assemblyRiscV2));
            }
            else if (*(node->op) == "ne" || *(node->op) == "or") {
                std::unique_ptr<AssemblyRiscV> assemblyRiscV2 = 
                    encapAssembly(rm.currentFree() - 1, R_T_REGISTER, 
                                rm.currentFree() - 1, R_T_REGISTER, 0, R_NOT_USED, "snez");

                setRiscV->instructions->push_back(std::move(assemblyRiscV2));
            }
        }
    }

    cout << "In ValueIRVisitor before RETURN" << endl;
    return setRiscV;
};

unique_ptr<BaseRiscV> ValuesIRVisitor::visit(const BaseIR *baseNode) const {
    return nullptr;
};

unique_ptr<BaseRiscV> BasicBlockIRVisitor::visit(const BaseIR *baseNode) const {
    cout << "In BasicBlockIRVisitor" << endl;
    const auto* node = dynamic_cast<const BasicBlockIR*>(baseNode);
    unique_ptr<GroupRiscV> groupRiscV = make_unique<GroupRiscV>();
    IRVisitor* visitor = new ValueIRVisitor();
    std::unique_ptr<std::vector<unique_ptr<AssemblyRiscV>>> instructions = make_unique<std::vector<unique_ptr<AssemblyRiscV>>>();
    for (const std::unique_ptr<ValueIR> &val : *(node->vals->values)) {
        unique_ptr<BaseRiscV> res = val->accept(visitor);
        unique_ptr<AssemblySetRiscV> setRiscV = unique_ptr<AssemblySetRiscV>(dynamic_cast<AssemblySetRiscV*>(res.release()));
        std::unique_ptr<std::vector<unique_ptr<AssemblyRiscV>>> tmp = std::move(setRiscV->instructions);
        instructions->insert(instructions->end(), std::make_move_iterator(tmp->begin()), std::make_move_iterator(tmp->end()));
    }
    groupRiscV->instructions = std::move(instructions);
    cout << "In BasicBlockIRVisitor before RETURN" << endl;
    return groupRiscV;
};

unique_ptr<BaseRiscV> FunctionIRVisitor::visit(const BaseIR *baseNode) const {
    cout << "In FunctionIRVisitor" << endl;
    const auto* node = dynamic_cast<const FunctionIR*>(baseNode);
    unique_ptr<FunctionRiscV> functionRiscV = make_unique<FunctionRiscV>();
    IRVisitor* visitor = new BasicBlockIRVisitor();
    functionRiscV->name = make_unique<string>(*(node->name));
    unique_ptr<BaseRiscV> res = node->blocks->accept(visitor);
    functionRiscV->group = unique_ptr<GroupRiscV>(dynamic_cast<GroupRiscV*>(res.release()));
    cout << "In FunctionIRVisitor before RETURN" << endl;
    return functionRiscV;
};

unique_ptr<BaseRiscV> IRProgramVisitor::visit(const BaseIR *baseNode) const {
    cout << "In IRProgramVisitor" << endl;
    const auto* node = dynamic_cast<const IRProgram*>(baseNode);
    unique_ptr<ProgramRiscV> programRiscV = make_unique<ProgramRiscV>();
    IRVisitor* visitor = new FunctionIRVisitor();
    unique_ptr<BaseRiscV> res = node->funcs->accept(visitor);
    programRiscV->functionRiscV = unique_ptr<FunctionRiscV>(dynamic_cast<FunctionRiscV*>(res.release()));
    cout << "In IRProgramVisitor before RETURN" << endl;
    return programRiscV;
};