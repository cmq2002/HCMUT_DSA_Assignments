#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE) {
    this->top = nullptr;
    this->root = nullptr;
}

StackFrame::~StackFrame() {
    clearStack();
    callclearAVL();
}

void StackFrame::opStackLoad (const string &cmd, const float &data, const int &line){
    if (cmd == "iconst"){
        if (isFull()){ //throw Stackfull
            throw StackFull(line);
            return;
        }
        push(data, 0);
    }

    if (cmd == "fconst"){
        if (isFull()){ //throw StackFull
            throw StackFull(line);
            return;
        }
        push(data, 1);
    }
}

void StackFrame::opStack (const string &cmd, const int &line){
    if (cmd == "iadd"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else {
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else {
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int sum = a+b;
        if (isFull()){
            throw StackFull(line);
            return;
        }//throw StackFull
        push((float)sum, 0);
    }

    if (cmd == "fadd"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        float sum = a+b;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push(sum, 1);
    }

    if (cmd == "isub"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int diff = b-a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push((float)diff, 0);
    }

    if (cmd == "fsub"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        float diff = b-a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push(diff, 1);
    }

    if (cmd == "imul"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int mul = a*b;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else
            push((float)mul, 0);
    }

    if (cmd == "fmul"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        float mul = a*b;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push(mul, 1);
    }

    if (cmd == "idiv"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (a == 0){
            throw DivideByZero(line);
            return;
        } //throw DivideByZero

        int div = b/a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push((float)div, 0);
    }

    if (cmd == "fdiv"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        if (a == 0){
            throw DivideByZero(line);
            return;
        } //throw DivideByZero
        pop();

        float b = getTopData();
        pop();

        float div = b/a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push(div, 1);
    }

    if (cmd == "irem"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (a == 0){
            throw DivideByZero(line);
            return;
        } //throw DivideByZero

        int rem = b-(b/a)*a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push((float)rem, 0);
    }

    if (cmd == "ineg"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b = -a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push((float)b, 0);
    }

    if (cmd == "fneg"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = -a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push(b, 1);
    }

    if (cmd == "iand"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = a&b;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        else push((float)res, 0);
    }

    if (cmd == "ior"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = a|b;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "ieq"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = 0;
        if (b == a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "feq"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        int res = 0;
        if (b == a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "ineq"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = 0;
        if (b != a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "fneq"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        int res = 0;
        if (b != a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "ilt"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = 0;
        if (b < a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "flt"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        int res = 0;
        if (b < a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "igt"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int b_code = getTopCode();
        int b = 0;
        if (b_code == 0) b = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = 0;
        if (b > a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "fgt"){
        if (this->count_stack < 2){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        float a = getTopData();
        pop();

        float b = getTopData();
        pop();

        int res = 0;
        if (b > a) res = 1;
        else res = 0;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "ibnot"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        int res = 0;
        if (a != 0) res = 0;
        else res = 1;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)res, 0);
    }

    if (cmd == "i2f"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMisMatch
        pop();

        float b = (float)a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push(b, 1);
    }


    if (cmd == "f2i"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty

        int a_code = getTopCode();
        float a = 0;
        if (a_code == 1) a = getTopData();
        else{
            throw TypeMisMatch(line);
            return;

        } //throw TypeMisMatch
        pop();

        int b = (int)a;
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push((float)b, 0);
    }

    if (cmd == "top"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //throw StackEmpty
        cout << getTopData() << '\n';
    }
}

void StackFrame::localAVL (const std::string &cmd, const std::string &var, const int &line){
    if (cmd == "istore"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //Stack Empty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (isFullAVL()){
            throw LocalSpaceFull(line);
            return;
        } // throw LocalSpaceFull

        callinsertAVL(var, (float)a, 0);
    }

    if (cmd == "fstore"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //Stack Empty

        int a_code = getTopCode();
        float a = 0;
        if (a_code == 1) a = getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (isFullAVL()){
            throw LocalSpaceFull(line);
            return;
        } // throw LocalSpaceFull

        callinsertAVL(var, a, 1);
    }

    if (cmd == "iload"){
        if (isEmptyAVL()){
            throw UndefinedVariable(line);
            return;
        } // Empty AVL tree

        string res1 = search_dataAVL(var);
        string res2 = search_codeAVL(var);
        if (res1 == ""){
            throw UndefinedVariable(line);
            return;
        } // throw UndefinedVariable

        // Retrieve data and code
        float data = stof(res1);
        int code = stoi(res2);

        if (code != 0){
            throw TypeMisMatch(line);
            return;
        } // throw TypeMisMatch

        if (isFull()){
            throw StackFull(line);
            return;
        }
        push(data,code);
    }

    if (cmd == "fload"){
        if (isEmptyAVL()){
            throw UndefinedVariable(line);
            return;
        } // Empty AVL tree

        string res1 = search_dataAVL(var);
        string res2 = search_codeAVL(var);
        if (res1 == ""){
            throw UndefinedVariable(line);
            return;
        } // throw UndefinedVariable

        // Retrieve data and code
        float data = stof(res1);
        int code = stoi(res2);

        if (code != 1){
            throw TypeMisMatch(line);
            return;
        } // throw TypeMisMatch

        if (isFull()){
            throw StackFull(line);
            return;
        }
        push(data,code);
    }

    if (cmd == "val"){
        if (isEmptyAVL()){
            throw UndefinedVariable(line);
            return;
        } // Empty AVL Tree

        string res = search_dataAVL(var);

        if (res == ""){
            throw UndefinedVariable(line);
            return;
        } // throw UndefinedVariable

        cout<<stof(res)<<'\n';
    }

    if (cmd=="par"){
        if (isEmptyAVL()){
            throw UndefinedVariable(line);
            return;
        } // Empty AVL Tree

        string res = search_parentAVL(var);
        if (res == ""){
            throw UndefinedVariable(line);
            return;
        } //Not found node with given key

        if (res == peak_keyAVL() && res == var)
            cout<<"null"<<'\n'; // Root has no parent
        else
            cout<<res<<'\n';  // The parent's name of given node
    }
}

void StackFrame::run(const string &filename) {
    string temp;
    ifstream input(filename);
    int line = 1;
    while(getline(input, temp)) {
        string cmd = {};
        string var = {};
        float val = 0;
        size_t l = temp.length();
        size_t space = temp.find(' ');
        if (space < l) {
            cmd = temp.substr(0, space);
            if (cmd != "istore" && cmd != "fstore" && cmd != "iload"
             && cmd != "fload" && cmd != "val" && cmd != "par") {
                val = stof(temp.substr(space + 1, l - space));
            }
            var = temp.substr(space + 1, l - space);
        }
        else cmd = temp; // for instructions that have only one part

        if (cmd == "iconst" || cmd == "fconst")
            opStackLoad(cmd, val, line);

        if (cmd == "iadd" || cmd == "fadd" || cmd == "isub" || cmd == "fsub"
            || cmd == "imul" || cmd == "fmul" || cmd == "idiv" || cmd == "fdiv"
            || cmd == "irem" || cmd == "ineg" || cmd == "fneg" || cmd == "iand"
            || cmd == "ior" || cmd == "ieq" || cmd == "feq" || cmd == "ineq"
            || cmd == "fneq" || cmd == "ilt" || cmd == "flt" || cmd == "igt"
            || cmd == "fgt" || cmd == "ibnot" || cmd == "i2f" || cmd == "f2i"
            || cmd == "top")
            opStack(cmd, line);

        if (cmd == "istore" || cmd == "fstore" || cmd == "iload" || cmd == "fload"
         || cmd == "val" || cmd == "par")
            localAVL(cmd, var, line);

        line++;
    }
    input.close();
}