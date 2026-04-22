#include<iostream>
#include<string>
using namespace std;
//  TREE NODE
struct TreeNode {
    int       data;
    bool      isOp;
    TreeNode* left;   // left child
    TreeNode* right;  // right child
};

TreeNode* newNode(int val, bool op) {
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->data  = val;
    temp->isOp  = op;
    temp->left  = NULL;
    temp->right = NULL;
    return temp;
}

//  STACK 1 — operator stack (chars)
struct OpNode {
    char    data;
    OpNode* next;
};
OpNode* OpTop = NULL;

void OpPush(char c) {
    OpNode* temp = (OpNode*)malloc(sizeof(OpNode));
    temp->data   = c;
    temp->next   = OpTop;
    OpTop        = temp;
}

char OpPop() {
    char    v    = OpTop->data;
    OpNode* curr = OpTop;
    OpTop        = OpTop->next;
    free(curr);
    return v;
}

char OpPeek() {
    return OpTop->data;
}

bool OpEmpty() {
    return OpTop == NULL;
}

//  STACK 2 — node stack (TreeNode* pointers)
struct stackNode {
    TreeNode*  Tnode;
    stackNode* next;
};
stackNode* NodeTop = NULL;

void NodePush(TreeNode* t) {
    stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
    temp->Tnode     = t;
    temp->next      = NodeTop;
    NodeTop         = temp;
}

TreeNode* NodePop() {
    TreeNode*  t    = NodeTop->Tnode;
    stackNode* curr = NodeTop;
    NodeTop         = NodeTop->next;
    free(curr);
    return t;
}

bool NodeEmpty() {
    return NodeTop == NULL;
}

//  HELPERS
int Precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

//  BUILD SUBTREE
//  Pops one operator + two nodes, wires them, pushes result back
void BuildSubTree() {
    char      op    = OpPop();
    TreeNode* right = NodePop();   // right child — popped first
    TreeNode* left  = NodePop();   // left  child — popped second

    TreeNode* OpNode  = newNode((int)op, true);
    OpNode->left  = left;
    OpNode->right = right;

    NodePush(OpNode);
}

// BUILD EXPRESSION TREE FROM INFIX
TreeNode* BuildTree(string infix) {
    for (int i = 0; i < infix.length(); i++) {
        char ch = infix[i];

        // skip spaces
        if (ch == ' ') {
            continue;
        }

        // digit operand
        if (isdigit(ch)) {
            int val = ch - '0';          // '3' → 3
            NodePush(newNode(val, false));
        }

        // letter operand
        else if (isalpha(ch)) {
            NodePush(newNode((int)ch, false));   // store ASCII of letter
        }

        // opening bracket
        else if (ch == '(') {
            OpPush(ch);
        }

        // closing bracket → build subtrees until matching '('
        else if (ch == ')') {
            while (!OpEmpty() && OpPeek() != '(') {
                BuildSubTree();
            }
            OpPop();   // discard '('
        }

        // operator → flush higher/equal precedence, then push
        else if (isOperator(ch)) {
            while (!OpEmpty() && Precedence(OpPeek()) >= Precedence(ch)) {
                BuildSubTree();
            }
            OpPush(ch);
        }
    }

    // flush remaining operators
    while (!OpEmpty()) {
        BuildSubTree();
    }

    return NodePop();   // root of complete expression tree
}

// POSTORDER TRAVERSAL → postfix output
//  Order: Left → Right → Node
void PrintPostfix(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    PrintPostfix(root->left);
    PrintPostfix(root->right);

    if (root->isOp) {
        cout << (char)root->data << " ";   // cast back to char for operator
    } else {
        cout << root->data << " ";         // print digit as number
    }
}

//  PREORDER TRAVERSAL → prefix output
//  Order: Node → Left → Right
void PrintPrefix(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    // visit node FIRST
    if (root->isOp) {
        cout << (char)root->data << " ";
    } else {
        cout << root->data << " ";
    }
    // then left subtree
    PrintPrefix(root->left);
    // then right subtree
    PrintPrefix(root->right);
}

//INORDER TRAVERSAL → infix output (with brackets)
//  Order: Left → Node → Right
void PrintInfix(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    if (root->isOp) cout << "(";
    PrintInfix(root->left);

    if (root->isOp) {
        cout << (char)root->data;
    } else {
        cout << root->data;
    }

    PrintInfix(root->right);
    if (root->isOp) cout << ")";
}

// EVALUATE the tree (only works for digit operands)
bool canEvaluate(TreeNode* root) {
    if (root == NULL) return true;
    // letter operand found — cannot evaluate numerically
    if (!root->isOp && root->data > 9) return false;
    return canEvaluate(root->left) && canEvaluate(root->right);
}

int evaluate(TreeNode* root) {
    // base case: leaf node → return the number
    if (!root->isOp) {
        return root->data;
    }

    // recursive case: evaluate both sides then apply operator
    int leftVal  = evaluate(root->left);
    int rightVal = evaluate(root->right);
    char op      = (char)root->data;

    if (op == '+') return leftVal + rightVal;
    if (op == '-') return leftVal - rightVal;
    if (op == '*') return leftVal * rightVal;
    if (op == '/') {
        if (rightVal != 0) {
            return leftVal / rightVal;
        } else {
            cout << "Error: Cannot divide by zero" << endl;
            return 0;
        }
    }
    return 0;
}

//  FREE ALL TREE MEMORY
void deleteTree(TreeNode* root) {
    if (root == NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

//  MAIN
int main() {
    string infix;
    cout << "Enter infix expression: ";
    getline(cin, infix);
    cout << endl;

    // Stage 1: build the tree
    TreeNode* root = BuildTree(infix);

    // Stage 2: print all three notations
    cout << "Postfix  expression : ";
    PrintPostfix(root);
    cout << endl;

    cout << "Prefix   expression : ";
    PrintPrefix(root);
    cout << endl;

    cout << "Infix    expression : ";
    PrintInfix(root);
    cout << endl;

    // Stage 3: evaluate only if all operands are digits
    if (canEvaluate(root)) {
        int result = evaluate(root);
        cout << "Evaluated Result    : " << result << endl;
    } else {
        cout << "Evaluated Result    : cannot evaluate (has letter variables)" << endl;
    }

    // free all heap memory
    deleteTree(root);
    return 0;
}