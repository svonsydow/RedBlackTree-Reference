
#include <bits/stdc++.h>
using namespace std;

struct Node {
  int data;
  Node *parent;
  Node *leftChild;
  Node *rightChild;
  int color;
};

typedef Node *NodePtr;

class RedBlackTree {
   private:
  NodePtr root;
  NodePtr TNULL;

public:
    RedBlackTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->leftChild = nullptr;
        TNULL->rightChild = nullptr;
        root = TNULL;
    }


//Traversal
  // Preorder
  void preOrderTraversal(NodePtr node) {
    if (node != TNULL) {
      cout << node->data << " ";
      preOrderTraversal(node->leftChild);
      preOrderTraversal(node->rightChild);
    }
  }

  // Inorder
  void inOrderTraversal(NodePtr node) {
    if (node != TNULL) {
      inOrderTraversal(node->leftChild);
      cout << node->data << " ";
      inOrderTraversal(node->rightChild);
    }
  }

  // Post order
  void postOrderTraversal(NodePtr node) {
    if (node != TNULL) {
      postOrderTraversal(node->leftChild);
      postOrderTraversal(node->rightChild);
      cout << node->data << " ";
    }
  }

  NodePtr searchTree(NodePtr node, int key) {
    if (node == TNULL || key == node->data) {
      return node;
    }

    if (key < node->data) {
      return searchTree(node->leftChild, key);
    }
    return searchTree(node->rightChild, key);
  }

  // For balancing the tree after deletion
  void deleteRebalance(NodePtr x) {
    NodePtr temp;
    while (x != root && x->color == 0) {
      if (x == x->parent->leftChild) {
          temp = x->parent->rightChild;
        if (temp->color == 1) {
            temp->color = 0;
          x->parent->color = 1;
          leftChildRotate(x->parent);
            temp = x->parent->rightChild;
        }

        if (temp->leftChild->color == 0 && temp->rightChild->color == 0) {
            temp->color = 1;
          x = x->parent;
        } else {
          if (temp->rightChild->color == 0) {
              temp->leftChild->color = 0;
              temp->color = 1;
            rightChildRotate(temp);
              temp = x->parent->rightChild;
          }

            temp->color = x->parent->color;
          x->parent->color = 0;
            temp->rightChild->color = 0;
          leftChildRotate(x->parent);
          x = root;
        }
      } else {
          temp = x->parent->leftChild;
        if (temp->color == 1) {
            temp->color = 0;
          x->parent->color = 1;
          rightChildRotate(x->parent);
            temp = x->parent->leftChild;
        }

        if (temp->rightChild->color == 0) {
            temp->color = 1;
          x = x->parent;
        } else {
          if (temp->leftChild->color == 0) {
              temp->rightChild->color = 0;
              temp->color = 1;
            leftChildRotate(temp);
              temp = x->parent->leftChild;
          }

            temp->color = x->parent->color;
          x->parent->color = 0;
            temp->leftChild->color = 0;
          rightChildRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = 0;
  }

  void rbTransplant(NodePtr u, NodePtr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->leftChild) {
      u->parent->leftChild = v;
    } else {
      u->parent->rightChild = v;
    }
    v->parent = u->parent;
  }

  void deleteNode(NodePtr node, int key) {
    NodePtr z = TNULL;
    NodePtr x, y;
    while (node != TNULL) {
      if (node->data == key) {
        z = node;
      }

      if (node->data <= key) {
        node = node->rightChild;
      } else {
        node = node->leftChild;
      }
    }

    if (z == TNULL) {
      cout << "Key not found in the tree" << endl;
      return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->leftChild == TNULL) {
      x = z->rightChild;
      rbTransplant(z, z->rightChild);
    } else if (z->rightChild == TNULL) {
      x = z->leftChild;
      rbTransplant(z, z->leftChild);
    } else {
      y = minimum(z->rightChild);
      y_original_color = y->color;
      x = y->rightChild;
      if (y->parent == z) {
        x->parent = y;
      } else {
        rbTransplant(y, y->rightChild);
        y->rightChild = z->rightChild;
        y->rightChild->parent = y;
      }

      rbTransplant(z, y);
      y->leftChild = z->leftChild;
      y->leftChild->parent = y;
      y->color = z->color;
    }
    delete z;
    if (y_original_color == 0) {
      deleteRebalance(x);
    }
  }

  // For balancing the tree after insertion
  void insertRebalance(NodePtr k) {
    NodePtr u;
    while (k->parent->color == 1) {
      if (k->parent == k->parent->parent->rightChild) {
        u = k->parent->parent->leftChild;
        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->leftChild) {
            k = k->parent;
            rightChildRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          leftChildRotate(k->parent->parent);
        }
      } else {
        u = k->parent->parent->rightChild;

        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->rightChild) {
            k = k->parent;
            leftChildRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          rightChildRotate(k->parent->parent);
        }
      }
      if (k == root) {
        break;
      }
    }
    root->color = 0;
  }

  void printTree(NodePtr root, string indent, bool last) {
    if (root != TNULL) {
      cout << indent;
      if (last) {
        cout << "R----";
        indent += "   ";
      } else {
        cout << "L----";
        indent += "|  ";
      }

      string sColor = root->color ? "RED" : "BLACK";
      cout << root->data << "(" << sColor << ")" << endl;
      printTree(root->leftChild, indent, false);
      printTree(root->rightChild, indent, true);
    }
  }

  void preorder() {
    preOrderTraversal(this->root);
  }

  void inorder() {
    inOrderTraversal(this->root);
  }

  void postorder() {
    postOrderTraversal(this->root);
  }

  NodePtr searchTree(int k) {
    return searchTree(this->root, k);
  }

  NodePtr minimum(NodePtr node) {
    while (node->leftChild != TNULL) {
      node = node->leftChild;
    }
    return node;
  }

  NodePtr maximum(NodePtr node) {
    while (node->rightChild != TNULL) {
      node = node->rightChild;
    }
    return node;
  }

  NodePtr successor(NodePtr x) {
    if (x->rightChild != TNULL) {
      return minimum(x->rightChild);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->rightChild) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  NodePtr predecessor(NodePtr x) {
    if (x->leftChild != TNULL) {
      return maximum(x->leftChild);
    }

    NodePtr y = x->parent;
    while (y != TNULL && x == y->leftChild) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftChildRotate(NodePtr x) {
    NodePtr y = x->rightChild;
    x->rightChild = y->leftChild;
    if (y->leftChild != TNULL) {
      y->leftChild->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->leftChild) {
      x->parent->leftChild = y;
    } else {
      x->parent->rightChild = y;
    }
    y->leftChild = x;
    x->parent = y;
  }

  void rightChildRotate(NodePtr x) {
    NodePtr y = x->leftChild;
    x->leftChild = y->rightChild;
    if (y->rightChild != TNULL) {
      y->rightChild->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->rightChild) {
      x->parent->rightChild = y;
    } else {
      x->parent->leftChild = y;
    }
    y->rightChild = x;
    x->parent = y;
  }

  // Inserting a node
  void insert(int key) {
    NodePtr node = new Node;
    node->parent = nullptr;
    node->data = key;
    node->leftChild = TNULL;
    node->rightChild = TNULL;
    node->color = 1;

    NodePtr y = nullptr;
    NodePtr x = this->root;

    while (x != TNULL) {
      y = x;
      if (node->data < x->data) {
        x = x->leftChild;
      } else {
        x = x->rightChild;
      }
    }

    node->parent = y;
    if (y == nullptr) {
      root = node;
    } else if (node->data < y->data) {
      y->leftChild = node;
    } else {
      y->rightChild = node;
    }

    if (node->parent == nullptr) {
      node->color = 0;
      return;
    }

    if (node->parent->parent == nullptr) {
      return;
    }

    insertRebalance(node);
  }

  NodePtr getRoot() {
    return this->root;
  }

  void deleteNode(int data) {
    deleteNode(this->root, data);
  }

  void printTree() {
    if (root) {
      printTree(this->root, "", true);
    }
  }
};

int main() {
  RedBlackTree Tree;
    Tree.insert(30);
    Tree.insert(28);
    Tree.insert(21);
    Tree.insert(11);
    Tree.insert(17);
    Tree.insert(4);

    Tree.printTree();

}