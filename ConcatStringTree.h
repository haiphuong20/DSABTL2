#ifndef __CONCAT_STRING_TREE_H__
#define __CONCAT_STRING_TREE_H__

#include "main.h"

class ConcatStringTree;
class ParentsTree;
class LitStringHash;

struct stNode {
    string s;
    stNode(string s){
        this->s = s;
    }
};

class tNode {
public:
    stNode *data;
    int length1;
    int leftL;
    tNode* left;
    tNode* right;
    int key;
    int id;
    LitStringHash* lithash;
    ParentsTree* tree;
    tNode();
    void checkPar(tNode* par);
    void des(tNode* par);
    ~tNode();
};

class ConcatStringTree {
protected:
    tNode* root;
    int sz;
public:
    static int bid;
    ConcatStringTree(const char* s);
    ConcatStringTree(tNode* root);
    ConcatStringTree()
    {
        root = nullptr;
        sz = 0;
    }
    
    int length() const;
    char get(int index) const;
    int indexOf(char c) const;
    string toStringPreOrder() const;
    string toString() const;
    ConcatStringTree concat(const ConcatStringTree & otherS) const;
    ConcatStringTree subString(int from, int to) const;
    ConcatStringTree reverse() const;

    int getParTreeSize(const string & query) const;
    string getParTreeStringPreOrder(const string & query) const;
    
    ~ConcatStringTree();


};
class ParentsTree {
public:
    struct TreeAVL {
        TreeAVL* left;
        TreeAVL* right;
        int h;
        tNode* data;
        TreeAVL()
        {
            left = nullptr;
            right = nullptr;
            h = 1;
            data = nullptr;
        }
    };
    TreeAVL* root; 
    int szNode;
    int size() const;
    string toStringPreOrder() const;
    ParentsTree()
    {
        root = nullptr;
        szNode = 0;
    }
    int height(TreeAVL* N)
    {
        if (N == nullptr)
        {
            return 0;
        }
        return N->h;
    }
    int getbalance(TreeAVL* node)
    {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }
    TreeAVL* balance(TreeAVL *node, tNode* cur){
        int bal = getbalance(node);
        // Left Left Case
        if (bal > 1 && cur->id < node->left->data->id)
            return rightRotate(node);

        // Right Right Case
        if (bal < -1 && cur->id > node->right->data->id)
            return leftRotate(node);

        // Left Right Case
        if (bal > 1 && cur->id > node->left->data->id)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (bal < -1 && cur->id < node->right->data->id)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        /* return the (unchanged) node pointer */
        return node;
    }
    TreeAVL *insertNode(tNode* cur,TreeAVL *root) {
        if (root == nullptr)
        {
            root = new TreeAVL();
            root->data = cur;
            szNode++;
        }

        if (cur->id < root->data->id)
            root->left = insertNode(cur, root->left);
        else if (cur->id > root->data->id)
            root->right = insertNode(cur, root->right);
        else // Equal keys are not allowed in BST
            return root;

        /* 2. Update height of this ancestor node */
        root->h = 1 + max(height(root->left),height(root->right));
        return balance(root, cur);
    }
    TreeAVL* maxValueNode(TreeAVL* node)
    {
        TreeAVL* current = node;

        /* loop down to find the leftmost leaf */
        while (current->right != nullptr)
        {
            current = current->right;
        }

        return current;
    }
    TreeAVL* delNode(TreeAVL* root, tNode* cur) {
        // STEP 1: PERFORM STANDARD BST DELETE 
        if (cur == nullptr)
            return nullptr;
        if (root == nullptr) return nullptr;
        // If the key to be deleted is smaller 
        // than the root's key, then it lies
        // in left subtree 
        if (cur->id < root->data->id)
            root->left = delNode(root->left, cur);

        // If the key to be deleted is greater 
        // than the root's key, then it lies 
        // in right subtree 
        else if (cur->id > root->data->id)
            root->right = delNode(root->right, cur);

        // if key is same as root's key, then 
        // This is the node to be deleted 
        else
        {
            // node with only one child or no child 
            if ((root->left == nullptr) || (root->right == nullptr))
            {
                TreeAVL* temp = root->left ?
                    root->left :
                    root->right;

                // No child case 
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else // One child case 
                    *root = *temp; // Copy the contents of 
                // the non-empty child 
                delete temp;
                szNode--;
            }
            else
            {
                // node with two children: Get the inorder 
                TreeAVL* temp = maxValueNode(root->left);

                // Copy the inorder successor's 
                // data to this node 
                root->data = temp->data;

                // Delete the inorder successor 
                root->left = delNode(root->left, temp->data);
         
            }
        }
        // If the tree had only one node
        // then return 
        if (root == nullptr) return root;

        // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
        root->h = 1 + max(height(root->left), height(root->right));
        
        int balance = getbalance(root);

        // If this node becomes unbalanced, 
        // then there are 4 cases 

        // Left Left Case 
        if (balance > 1 && getbalance(root->left) >= 0)
        {
            return rightRotate(root);
        }

        // Left Right Case 
        if (balance > 1 && getbalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case 
        if (balance < -1 && getbalance(root->right) <= 0)
        {
            return leftRotate(root);
        }

        // Right Left Case 
        if (balance < -1 && getbalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
        
    }
    TreeAVL* rightRotate(TreeAVL* y)
    {
        TreeAVL* x = y->left;
        TreeAVL* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->h = max(height(y->left), height(y->right)) + 1;
        x->h = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }
    TreeAVL* leftRotate(TreeAVL* x)
    {
        TreeAVL* y = x->right;
        TreeAVL* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->h = max(height(x->left), height(x->right)) + 1;
        y->h = max(height(y->left), height(y->right)) + 1;
        // Return new root
        return y;
    }
};

class ReducedConcatStringTree; // forward declaration
class LitStringHash; // forward declaration

class HashConfig {
public:
    long long p;
    double c1, c2;
    double lambda;
    double alpha;
    long long initSize;
    HashConfig(long long p, double c1, double c2, double lambda, double alpha, long long initSize) {
        this->p = p;
        this->c1 = c1;
        this->c2 = c2;
        this->lambda = lambda;
        this->alpha = alpha;
        this->initSize = initSize;
    }
    HashConfig()
    {
        this->p = 0;
        this->c1 = 0;
        this->c2 = 0;
        this->lambda = 0;
        this->alpha = 0;
        this->initSize = 0;
    }
    friend class ReducedConcatStringTree;
    friend class LitStringHash;
};
struct newhash {
    stNode* hashing;
    long long cnt;
    newhash() {
        hashing = nullptr;
        cnt = 0;
    }
};

class LitStringHash {
public:
    HashConfig z;
    long long sz1, tail;
    long long maxsz, initsz;
    newhash *hash;
public:
    LitStringHash(const HashConfig& hashConfig);
    long long hashfunction(string s);
    long long hashtable(long long x, int i);
    void rehashing();
    int getLastInsertedIndex() const;
    string toString() const;
    void inserthash(stNode* cur, int idx);
    stNode* findnode(string sn4, long long ind)
    {
        if (hash == nullptr) return nullptr;
        for (int i = 0; i < maxsz; i++)
        {
            long long newindex = hashtable(ind, i);
            if (hash[newindex].hashing == nullptr) continue;
            if (hash[newindex].hashing->s == sn4)
            {
                hash[newindex].cnt += 1;
                return hash[newindex].hashing;
            }
        }
        return nullptr;
    }

    void deletehash(stNode *cur)
    {
        if (hash == nullptr) return;
        long long idx3 = hashfunction(cur->s);
        for (int i = 0; i < maxsz; i++)
        {
            long long newindex = hashtable(idx3, i);
            if (hash[newindex].hashing == nullptr) continue;
            if (hash[newindex].hashing->s == cur->s)
            {
                hash[newindex].cnt -= 1;
                if (hash[newindex].cnt == 0)
                {
                    delete hash[newindex].hashing;
                    hash[newindex].hashing = nullptr;
                    sz1 -= 1;
                }
                break;
            }
        }
        if (sz1 == 0)
        {
            delete[] hash;
            hash = nullptr;
            tail = -1;
            maxsz = z.initSize;
        }
    }
};

class ReducedConcatStringTree : public ConcatStringTree {
public:
    LitStringHash* litStringHash;
    ReducedConcatStringTree(tNode* newroot, LitStringHash* litStringHash)
    {
        this->root = newroot;
        sz = root->length1;
        root->checkPar(root);
        this->litStringHash = litStringHash;
    }
    ReducedConcatStringTree()
    {
        root = nullptr;
        sz = 0;
        litStringHash = nullptr;
    }
    ReducedConcatStringTree(const char* s, LitStringHash* litStringHash)
    {
        tNode* root = new tNode();
        string sn = "";
        int i = 0;
        while (s[i] != '\0')
        {
            sn += s[i];
            i++;
        }
        long long newidx1 = litStringHash->hashfunction(sn);
        root->data = litStringHash->findnode(sn, newidx1);
        if (root->data == nullptr)
        {
            stNode* node = new stNode(sn);
            root->data = node;
            litStringHash->inserthash(node, newidx1);
        }
        root->leftL = 0;
        root->left = nullptr;
        root->right = nullptr;
        root->length1 = sn.length();
        root->lithash = litStringHash;
        this->root = root;
        sz = sn.length();
        root->checkPar(root);
        this->litStringHash = litStringHash;
    }
    ReducedConcatStringTree concat(const ReducedConcatStringTree& otherS) const
    {
        tNode* fN = new tNode();
        fN->left = this->root;
        fN->right = otherS.root;
        fN->leftL = root->length1;
        fN->length1 = root->length1 + otherS.root->length1;
        return ReducedConcatStringTree(fN, litStringHash);
    }
    ReducedConcatStringTree subString(int from, int to) const
    {
        return ReducedConcatStringTree();
    }
    ReducedConcatStringTree reverse() const
    {
        return ReducedConcatStringTree();
    }
    ~ReducedConcatStringTree()
    {
        if (root != nullptr) root->des(root);
        this->root = nullptr;
        litStringHash = nullptr;
        sz = 0;
    }
};

#endif // __CONCAT_STRING_TREE_H__