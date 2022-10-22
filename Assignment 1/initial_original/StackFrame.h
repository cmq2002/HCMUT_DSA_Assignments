#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
public:
    class Node;
private:
    Node* top;              //Top of the operand stack
    int count_stack = 0;    //Count numbers of nodes in operand stack
    float* localArr;        //Local variable Array
    int*  markArr;          //Use to keep track of which blocks in localArr have been used
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();

    ~StackFrame();

    void opStackLoad (const std::string &cmd, const float &data,const int &line);

    void opStack (const std::string &cmd,const int &line);

    void localArray (const std::string &cmd, const int &ind,const int &line);

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(const std::string &filename);

    ////////////////////////////////////////////////////////
    //Support function for Operand Stack///////////////////
    //////////////////////////////////////////////////////
    void push (const float &data, const int &code){
        Node* pNew = new Node(data, code, nullptr);
        pNew->next = this->top;
        this->top = pNew;
        this->count_stack++;
    }
    int pop() {
        if (this->count_stack == 0) return 0;
        Node* dltptr = this->top;
        this->top = dltptr->next;
        this->count_stack--;
        delete dltptr;
        return 1;
    }
    float getTopData() {return this->top->data;}
    int getTopCode() {return this->top->code;}
    bool isEmpty(){
        if (this->count_stack == 0) return true;
        return false;
    }
    bool isFull(){
        if (this->count_stack == opStackMaxSize/2) return true;
        return false;
    }

    void clearStack(){
        Node* dltptr = this->top;
        while (dltptr != nullptr){
            dltptr = dltptr->next;
            delete this->top;
            this->top = dltptr;
        }
        this->count_stack = 0;
        this->top = nullptr;
    }

public:
    class Node{
    private:
        Node* next;
        float data;
        int code;
        friend class StackFrame;
    public:
        Node(float data, int code, Node* next){
            this->data = data;
            this->code = code;
            this->next = next;
        }
    };
};
#endif // !__STACK_FRAME_H__
