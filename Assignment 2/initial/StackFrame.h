#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
/*
StackFrame declaration
*/
enum BalanceValue{
    LH = -1,
    EH = 0,
    RH = 1
};

class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
public:
    class Node;
    class NodeAVL;
private:
    Node* top;                  // Top of the operand stack
    int count_stack = 0;        // Count numbers of nodes in operand stack
    NodeAVL* root;                 // Root of the local AVL Tree
    int count_avl = 0;          // Count numbers of nodes in the local AVL tree
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();

    ~StackFrame();

    void opStackLoad (const std::string &cmd, const float &data, const int &line);

    void opStack (const std::string &cmd, const int &line);

    void localAVL (const std::string &cmd, const std::string &var, const int &line);
    
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
        float data;
        int code;
        Node* next;
        friend class StackFrame;
    public:
        Node(float data, int code, Node* next){
            this->data = data;
            this->code = code;
            this->next = next;
        }
    };

    ////////////////////////////////////////////////////////
    //Support function for Local AVL Tree///////////////////
    ///////////////////////////////////////////////////////
    int getHeightRec (NodeAVL* _root){
        if (_root == nullptr) return 0;
        int lh = this->getHeightRec(_root->pLeft);
        int rh = this->getHeightRec(_root->pRight);
        return (lh > rh ? lh : rh) + 1;
    }

    int balanceFactor (NodeAVL* _root){
        return getHeightRec(_root->pLeft) - getHeightRec(_root->pRight);
    }

    NodeAVL* rotateRight (NodeAVL* _root){
        NodeAVL* temp = _root->pLeft;
        _root->pLeft = temp->pRight;
        temp->pRight = _root;
        return temp;
    }

    NodeAVL* rotateLeft (NodeAVL* _root){
        NodeAVL* temp = _root->pRight;
        _root->pRight = temp->pLeft;
        temp->pLeft = _root;
        return temp;
    }

    NodeAVL* rotation (NodeAVL* _root){
        if (balanceFactor(_root) == 2){
            if (balanceFactor(_root->pLeft) == 1){
                _root = rotateRight(_root);
            }
            else{
                _root->pLeft = rotateLeft(_root->pLeft);
                _root = rotateRight(_root);
            }
        }
        if (balanceFactor(_root) == -2){
            if (balanceFactor(_root->pRight) == -1){
                _root = rotateLeft(_root);
            }
            else{
                _root->pRight = rotateRight(_root->pRight);
                _root = rotateLeft(_root);
            }
        }
        return _root;
    }

    int compareKey (const std::string &key1, const std::string &key2){
        size_t L1 = key1.length();
        size_t L2 = key2.length();
        int res = 0;
        if (L1 > L2 || L1 < L2) {
            size_t shorter = (L1>L2?L1:L2);
            for (size_t i = 0; i < shorter; i++) {
                if (key1[i] < key2[i]) {
                    res = 1;
                    break;
                }
                if (key1[i] > key2[i]) {
                    res = -1;
                    break;
                }
            }
            if (res == 0) {
                if (L1 < L2) res = 1;
                if (L1 > L2) res = -1;
            }
        }
        if (L1 == L2){
            for (size_t i=0; i<L1; i++){
                if (key1[i] < key2[i]) {
                    res = 1;
                    break;
                }
                if (key1[i] > key2[i]) {
                    res = -1;
                    break;
                }
            }

        }
        return res;
    }

    NodeAVL* insertAVL (NodeAVL* _root, const std::string &key, const float &data, const int &code){
        if (_root == nullptr){
            NodeAVL* newroot = new NodeAVL(key, data, code);
            this->count_avl++;
            return newroot;
        }
        else{
            if (compareKey(key, _root->key) == 1)
                _root->pLeft = insertAVL(_root->pLeft, key, data, code);
            else if (compareKey(key, _root->key) == -1)
                _root->pRight = insertAVL(_root->pRight, key, data, code);
            else {
                _root->data = data;
                _root->code = code;
                return _root;
            }
            while (balanceFactor(_root) != 1 && balanceFactor(_root) != 0 && balanceFactor(_root) != -1)
                _root = rotation(_root);
            if (balanceFactor(_root) == 1) _root->balance = LH;
            if (balanceFactor(_root) == 0) _root->balance = EH;
            if (balanceFactor(_root) == -1) _root->balance = RH;
            return _root;
        }
    }

    void callinsertAVL (const std::string &key, const float &data, const int &code){
        this->root = insertAVL(this->root, key, data, code);
    }

    std::string search_dataAVL (const std::string &key){
        std::string res = "";
        NodeAVL* pRun = this->root;
        while (pRun != nullptr) {
            if (compareKey(pRun->key, key) == 0) {
                res = std::to_string(pRun->data);
                break;
            }
            else if (compareKey(pRun->key, key) == 1)
                pRun = pRun->pRight;
            else
                pRun = pRun->pLeft;
        }
        return res;
    }

    std::string search_codeAVL (const std::string &key){
        NodeAVL* pRun = this->root;
        std::string res = "";
        while (pRun != nullptr) {
            if (compareKey(pRun->key, key) == 0) {
                res = std::to_string(pRun->code);
                break;
            }
            else if (compareKey(pRun->key, key) == 1)
                pRun = pRun->pRight;
            else
                pRun = pRun->pLeft;
        }
        return res;
    }

    std::string search_parentAVL (const std::string &key){
        std::string res = "";
        if (compareKey(this->root->key, key) == 0)
            res = this->root->key;
        else {
            NodeAVL* pRun = this->root;
            NodeAVL* parent = pRun;
            if (((compareKey(pRun->key,key)==1) && pRun->pRight == nullptr) || ((compareKey(pRun->key,key)==-1) && pRun->pLeft == nullptr))
                return res;
            while (pRun->pLeft != nullptr || pRun->pRight != nullptr) {
                parent = pRun;
                if (compareKey(pRun->key, key) == 1) {
                    pRun = pRun->pRight;
                    if (compareKey(pRun->key, key) == 0)
                        break;
                }
                else if (compareKey(pRun->key, key) == -1) {
                    pRun = pRun->pLeft;
                    if (compareKey(pRun->key, key) == 0)
                        break;
                }
            }
            if (pRun != nullptr && (compareKey(pRun->key, key) == 0)) {
                res = parent->key;
            }
        }
        return res;
    }

    bool isFullAVL(){
        if (this->count_avl == localVarSpaceSize/2) return true;
        return false;
    }

    bool isEmptyAVL(){
        if (this->count_avl == 0) return true;
        return false;
    }

    std::string peak_keyAVL(){
        return this->root->key;
    }

    void clearAVL(NodeAVL* _root){
        if (_root == nullptr) return;
        clearAVL(_root->pLeft);
        clearAVL(_root->pRight);
        delete _root;
    }

    void callclearAVL(){
        clearAVL(this->root);
    }
public:
    class NodeAVL{
    private:
        std::string key;
        float data;
        int code;
        BalanceValue balance;
        NodeAVL *pLeft, *pRight;
        friend class StackFrame;
    public:
        NodeAVL(const std::string &key, float data, int code){
            this->key = key;
            this->data = data;
            this->code = code;
            balance = EH;
            this->pLeft = nullptr;
            this->pRight = nullptr;
        }
    };
};

#endif // !__STACK_FRAME_H__