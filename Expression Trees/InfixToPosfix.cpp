#include<iostream>
#include<string>
using namespace std;
struct TreeNode { 
    int data;
    bool isOp;
    TreeNode*left; //Left child
    TreeNode*right; // right child
    };
TreeNode*newNode(int val, bool op){
TreeNode*temp = (TreeNode*)malloc(sizeof(TreeNode));
temp->data= val;
temp->isOp = op;
temp->left = NULL;
temp->right=NULL;
return temp;
}
struct OpNode{ // operator stack
    char data;
    OpNode*next;
};
OpNode*OpTop = NULL;
void OpPush(char c){
    OpNode*temp=(OpNode*)malloc(sizeof(OpNode));
    temp->data =c;
    temp->next = OpTop;
    OpTop = temp;
}
char OpPop(){
    char v = OpTop->data;
    OpNode*curr = OpTop;
    OpTop= OpTop->next;
    free(curr);
    return v;
}
char OpPeek(){
    return OpTop->data;
}
bool OpEmpty(){
    return OpTop == NULL;
}
struct stackNode{ // Node Stack
    TreeNode*Tnode;
    stackNode*next;
};
stackNode*NodeTop=NULL;
void NodePush(TreeNode*t){
    stackNode*temp = (stackNode*)malloc(sizeof(stackNode));
    temp->Tnode =t;
    temp->next = NodeTop;
    NodeTop = temp;
}
TreeNode*NodePop(){
    TreeNode* t = NodeTop->Tnode;
    stackNode*curr = NodeTop;
    NodeTop= NodeTop->next;
    free(curr);
    return t;
}
bool NodeEmpty(){
    return NodeTop == NULL;
}
// Helpers
int Precedence(char op){
    if(op== '+' || op =='-'){
        return 1;
    }
    if(op== '*' || op=='/'){
        return 2;
    }
    return 0;
}
bool isOperator(char c){
    return c =='+' || c == '-' || c == '*' || c =='/';
}
// Now build the tree by popping out one operator and two operands
//Pushing back into node after wiring them together
void BuildSubTree(){
    char op = OpPop();
    TreeNode*right = NodePop();
    TreeNode*left = NodePop();
// Wire them together:
    TreeNode* OpNode = newNode((int) op, true);
    OpNode->left = left;
    OpNode->right = right;
//Now push the result in stackNode push
    NodePush(OpNode);
}

// Build Expression Tree from infix input
TreeNode*BuildTree(string infix){
  for(int i =0; i < infix.length(); i++){
    char ch = infix[i];

    if(ch ==' '){ // if there's an empty space then skip it and continue
        continue;
    }
    if(isdigit(ch)){
        int val = ch - '0';  // Convert from char to int by -48
        NodePush(newNode(val, false));
    }
    else if(ch == '('){
        OpPush(ch); // push operator in the bracket in operator stack
    }
    else if(ch ==')'){
        while(!OpEmpty() && OpPeek()!='('){
            BuildSubTree();
        }
        OpPop();  // Discard '('
    }
    else if(isOperator(ch)){
        while(!OpEmpty() && Precedence(OpPeek() >= Precedence(ch))){
            BuildSubTree();
        }
        OpPush(ch); // otherwise push in stack
    }
  }
  while(!OpEmpty()){
    BuildSubTree();
  }
  return NodePop(); // root of the complete expression tree
}

// Now for post order Traversal or postfix output

void PrintPostfix(TreeNode*root){
    if(root == NULL){
        return;
    }
    PrintPostfix(root->left);
    PrintPostfix(root->right);
    if(root->isOp){
        cout<<(char)root->data<<" ";
    }
    else {
        cout<<root->data << " ";
    }
}

// Now to evaluate the Tree
int evaluate(TreeNode*root){
    // Base Case: If its a leaf node  then simply print it
    if(!root->isOp){
        return root->data;
    }
    //First evaluate both sides
    int leftVal = evaluate(root->left);
    int rightVal = evaluate(root->right);
    char op = (char)root->data;

    if(op == '+'){
        return leftVal + rightVal;
    }
    if(op == '-'){
        return leftVal - rightVal;
    }
    if(op == '*'){
        return leftVal * rightVal;
    }
    if(op == '/'){
        if(rightVal !=0){
        return leftVal/rightVal;
        }
        else {
            cout<<"Error: Cannot be divided by zero";
        }
    }
    return 0;
}
// main program
int main(){
    string infix;
    cout<< "Enter infix expression: ";
    getline(cin,infix);
    cout<<endl;
    TreeNode*root = BuildTree(infix);

    cout<<"PostFix expression : ";
    PrintPostfix(root);
    cout<< endl;

    int result = evaluate(root);
    cout<< "Evaluated Result: "<< result << endl;
    return 0;
}


