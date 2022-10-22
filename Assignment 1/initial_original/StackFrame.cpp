#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE) {
    this->top = nullptr;
    this->localArr = new float [localVarArrSize];
    this->markArr = new int [localVarArrSize];
}

StackFrame::~StackFrame() {
    delete[] localArr;
    delete[] markArr;
    localArr = nullptr;
    markArr = nullptr;
    clearStack();
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
        push (data, 1);
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
        else push((float)mul, 0);
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

void StackFrame::localArray (const string &cmd, const int &ind,const int &line){
    if (cmd == "istore"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //StackEmpty

        int a_code = getTopCode();
        int a = 0;
        if (a_code == 0) a = (int)getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (ind > localVarArrSize){
            throw ArrayOutOfRange(line);
            return;
        } //throw ArrayOutofRange
        *(localArr+ind) = 0;
        *(localArr+ind+1) = (float)a;
        *(markArr+ind) = 1;
        *(markArr+ind+1) = -1;
    }

    if (cmd == "fstore"){
        if (isEmpty()){
            throw StackEmpty(line);
            return;
        } //StackEmpty

        int a_code = getTopCode();
        float a = 0;
        if (a_code == 1) a = getTopData();
        else{
            throw TypeMisMatch(line);
            return;
        } //throw TypeMismatch
        pop();

        if (ind > localVarArrSize){
            throw ArrayOutOfRange(line);
            return;
        } //throw ArrayOutofRange
        *(localArr+ind) = 1;
        *(localArr+ind+1) = a;
        *(markArr+ind) = 1;
        *(markArr+ind+1) = -1;
    }

    if (cmd == "iload"){
        if (ind > localVarArrSize){
            throw ArrayOutOfRange(line);
            return;
        } //throw ArrayOutofRange
        if (*(markArr+ind) != 1){
            throw UndefinedVariable(line);
            return;
        } //throw UndefinedVariable
        if (*(localArr+ind) != 0){
            throw TypeMisMatch(line);
            return;
        } //throw TypeMisMatch
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push(*(localArr+ind+1), 0);
    }

    if (cmd == "fload"){
        if (ind > localVarArrSize){
            throw ArrayOutOfRange(line);
            return;
        } //throw ArrayOutofRange
        if (*(markArr+ind) != 1){
            throw UndefinedVariable(line);
            return;
        } //throw UndefinedVariable
        if (*(localArr+ind) != 1){
            throw TypeMisMatch(line);
            return;
        } //throw TypeMisMatch
        if (isFull()){
            throw StackFull(line);
            return;
        } //throw StackFull
        push(*(localArr+ind+1), 1);
    }

    if (cmd == "val"){
        if (ind > localVarArrSize){
            throw ArrayOutOfRange(line);
            return;
        } //throw ArrayOutofRange
        if (*(markArr+ind) != 1){
            throw UndefinedVariable(line);
            return;
        } //throw UndefinedVariable
        cout<<*(localArr+ind+1)<<'\n';
    }
}

void StackFrame::run(const string &filename) {
    string temp;
    ifstream input(filename);
    int line = 1;
    float val = 0;
    while(getline(input, temp)){
        string cmd[1] = {};
        size_t l = temp.length();
        size_t space = temp.find(' ');
        if (space < l){
            cmd[0] = temp.substr(0,space);
            val = stof(temp.substr(space+1,l-space));
        }
        else cmd[0] = temp;

        if (cmd[0] == "iconst" || cmd[0] == "fconst")
            opStackLoad(cmd[0], val, line);

        if (cmd[0] == "iadd" || cmd[0] == "fadd" || cmd[0] == "isub" || cmd[0] == "fsub"
            || cmd[0] == "imul" || cmd[0] == "fmul" || cmd[0] == "idiv" || cmd[0] == "fdiv"
            || cmd[0] == "irem" || cmd[0] == "ineg" || cmd[0] == "fneg" || cmd[0] == "iand"
            || cmd[0] == "ior" || cmd[0] == "ieq" || cmd[0] == "feq" || cmd[0] == "ineq"
            || cmd[0] == "fneq" || cmd[0] == "ilt" || cmd[0] == "flt" || cmd[0] == "igt"
            || cmd[0] == "fgt" || cmd[0] == "ibnot" || cmd[0] == "i2f" || cmd[0] == "f2i"
            || cmd[0] == "top")
            opStack(cmd[0], line);

        if (cmd[0] == "val" || cmd[0] == "istore" || cmd[0] == "fstore" || cmd[0] == "iload" || cmd[0] == "fload")
            localArray(cmd[0], (int)val, line);
        line++;
    }
    input.close();
}