#include<iostream>
using namespace std;
struct Node{
    int data;
    Node* left;
    Node*right;
};
Node*root = NULL;
Node* insert(int val, Node* curr){
    if(curr == NULL){
        Node*tmp = (Node*)malloc(sizeof(Node));
        tmp ->data = val;
        tmp ->left = NULL;
        tmp->right = NULL;
        return tmp;
    }
    if(val > curr->data) {
        curr->right = insert (val,curr->right);
    }
    else 
    { 
        curr->left = insert(val, curr->left);
}
return curr;
}
void PreOrder(Node* curr){
    if (curr!= NULL){
        cout<< curr->data << " ";
        PreOrder(curr->left);
        PreOrder(curr->right);
    }
}
void PostOrder(Node* curr){
    if (curr!= NULL){
        PostOrder(curr->left);
        PostOrder(curr->right);
        cout<< curr->data << " ";
    }
}
void InOrder(Node*curr){
    if(curr!= NULL){
      InOrder(curr->left);
      cout<<curr->data << " ";
      InOrder(curr->right) ;
    }
}
Node* Search (int val, Node* curr){
    if (curr == NULL){
        cout<< "\n Not intialiazed";
        return NULL;
    }
    if(val == curr->data){
        cout<< "\n Value found";
        return curr;
    }
    if (val > curr->data){
        cout<< "\n Value found at right";
    
        curr-> right = Search(val, curr->right);
    }
    else {
        cout<<"\n Value found at left";
    curr->left = Search(val, curr->left);
    
}
return curr;
}
Node* DeleteNode(int val, Node* curr){
    if (curr== NULL){
        cout<< "\n Value not found";
        return NULL;
    }
    if(curr->data == val){
        cout<<"\n Value found";
        //When the node has zero child:
        if(curr->left == NULL && curr->right == NULL){
            free(curr);
            cout<< "\n Value deleted (No child node)";
            return NULL;
        }
        //When the node has 1 child (The left one):
        if(curr->left != NULL && curr->right == NULL){
          Node* tmp = curr->left;
          free(curr);
          cout<<"\n Value deleted (left one child Node) ";
          return tmp;
        }
        //When the left Node is empty and right has 1 child:
        if(curr->left== NULL && curr->right != NULL){
            Node* tmp = curr->right;
            free(curr);
            cout<<"\n Value deleted (right one child node)";
            return tmp;
        }
        if(curr->right != NULL && curr->left != NULL){
            Node* tmp = curr->right; // From the right side
            while(tmp->left != NULL){
                tmp = tmp ->left;
                //tmp = MinNode(curr->right);
                curr->data = tmp ->data;
                curr->right = DeleteNode(tmp->data, curr->right);
                cout<< "\n Value deleted (node has both child)";
            }
        }


    }
    if (val> curr->data){
        curr->right = DeleteNode(val, curr->right);
    }
    else {
        curr->left = DeleteNode (val, curr->left);
    }
    return curr;
}
int main() {
    int choice;
    int data;
     while(true) {
        cout<<"\n1.Insert";
        cout<<"\n2.Search";
        cout<<"\n3. Pre Order Traversal";
        cout<<"\n4.Post Order Traversal";
        cout<<"\n5.In Order Traversal";
        cout<<"\n6.Delete Node";
        cout<<"\n7.Exit";
        cout<<"\n Enter your choice: ";
        cin >> choice;
        switch(choice){
        
            case 1:
            cout<<"\nEnter the value to insert: ";
            cin>> data;
            root = insert(data, root);
            break;

            case 2:
            cout<<"\n Enter the value to search: ";
            cin>> data;
            root = Search(data, root) ;
            break; 

            case 3:
            cout<<"\n PreOrder Traversal: ";
            PreOrder(root);
            break;

            case 4:
            cout<< "\n PostOrder Traversal: ";
            PostOrder(root);
            break;

            case 5: 
            cout<< "\n InOrder Traversal: ";
            InOrder(root);
            break;
            
            case 6:
            cout<<"\n Deleting node: ";
            cin>>data;
            DeleteNode(data, root);
            break;
        
            case 7: 
            exit(0);
            cout<<"\n program Exited";
            break;

            default:
            cout<<"\nInvalid Choice";
            break;
        }
    }
    return 0;
}