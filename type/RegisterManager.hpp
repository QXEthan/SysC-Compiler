#ifndef REGISTER_MANAGER_H
#define REGISTER_MANAGER_H

class BaseAST;

using namespace std;
class RegisterManager {
    int t_reg[7];
    int t_status_reg[7]; //0:free, 1: used
    int a_reg[7];
    int a_status_reg[7];


 public:
    RegisterManager() {
        for (int i = 0; i < 7; ++i) {
            t_reg[i] = 0;            
            t_status_reg[i] = 0;
            a_reg[i] = 0;            
            a_status_reg[i] = 0;
        }
    }

    int allocate_t() {
        for (int i = 0; i < 7; ++i) {
            if (t_status_reg[i] == 0) {
                t_status_reg[i] = 1;
                return i;
            }
        }
        throw std::runtime_error("No available t registers!");
    }
    int allocate_a() {
        return 0;
    }

    void release_t(int reg) {
        if (reg < 0 || reg >= 7) {
            throw std::invalid_argument("Invalid t register number!");
        }
        if (t_status_reg[reg] == 0) {
            throw std::runtime_error("t register already free!");
        }
        t_status_reg[reg] = 0; // release
    }

    void release_a(int reg) {
        if (reg != 0) {
            throw std::invalid_argument("Invalid a register number!");
        }
        if (a_status_reg[reg] == 0) {
            throw std::runtime_error("a register already free!");
        }
        a_status_reg[reg] = 0;
    }

    int getval_t(int reg) {
        return t_reg[reg];
    }

    int getval_a(int reg) {
        return a_reg[reg];
    }

    int currentFree() {
        for (int i = 0; i < 7; ++i) {
            if (t_status_reg[i] == 0) {
                return i;
            }
        }
        return -1;
    }
};

inline RegisterManager rm;

#endif