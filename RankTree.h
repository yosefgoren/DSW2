#ifndef SEARCHTREE
#define SEARCHTREE

#include <cassert>
#include <functional>
#include <iostream>

template <class Data, class Key>
class SearchTree
{
public:
    struct node_t;
    typedef node_t* Node;

    Node head;
    Node biggest;
    Node smallest;
    void* parent_object;
    bool ittirate_in_revese;

    SearchTree(void* parent_object = nullptr)
        :head(nullptr), biggest(nullptr), smallest(nullptr), parent_object(parent_object), ittirate_in_revese(false){}
    /**
     * @brief copy ctor does not duplicate all of the data, 
     * it only makes another instance of the hub 'pointing' to the same data.
     */
    SearchTree(const SearchTree<Data, Key>& source) = default;
    ~SearchTree();

    bool insert(Key key, const Data& data);
    bool remove(Key key);

    Data& operator[](Key key);
    const Data& operator[](Key key) const;

    class iterator;
    iterator begin();
    iterator end();
    SearchTree<Data, Key>& bigToSmall(){ittirate_in_revese = true; return *this;}
    SearchTree<Data, Key>& smallToBig(){ittirate_in_revese = false; return *this;}

    static int height(Node node){return node == nullptr ? 0 : node->height;}
    int height(){return height(head);}

    Node findByOrder(int place);
    Node findNode(Key key){return findSub(head, key);}
    Node closestBiggerSon(Node current);
    Node closestSmallesrSon(Node current);

    enum Order{PREORDER, INORDER, POSORDER};
    /**
     * @brief executes fucntion over each data-node within the tree.
     * for example: to get 'inOrder', 'Ord = INORDER', 'revurse = false'.
     */
    void exec(std::function<void(Node)> fun, bool revurse = false, Order ord = INORDER);

    /**
     * @brief debug tool, checks if all nodes have the appropriate pointers,
     * will throw false assertion otherwise. deafult value becomes 'head'.
     */
    void assertStructure(Node initial = nullptr);

protected:
    static int BalanceFactor(Node node){return height(node->left)-height(node->right);}
    static int max(int a, int b){return a > b ? a : b;}
    static void updateHeight(Node node){node->height = 1+max(height(node->left),height(node->right));}

    void rollLL(Node A, Node B);
    void rollLR(Node A, Node B, Node C);
    void rollRL(Node A, Node B, Node C);
    void rollRR(Node A, Node B);

private:
    Node findSub(Node top, Key key) const;
    static void removeSub(Node top);
    static void execSub(Node top, std::function<void(Data&)> fun, Order ord, bool revurse);
};

template <class Data, class Key>
struct SearchTree<Data, Key>::node_t
{
    Key key;
    Data data;
    SearchTree<Data, Key>& tree;
    friend SearchTree<Data, Key>;
    void removeFromTree(){tree.remove(key);}
    bool lastInTree(){return father==nullptr && left==nullptr && right==nullptr;}
    int sub_tree_size;
private:
    node_t* father;
    node_t* left;
    node_t* right;
    int height;
    void updateData();
    node_t(Key key, const Data& data, SearchTree<Data, Key>& tree)
        :key(key), data(data), tree(tree), father(nullptr), left(nullptr), right(nullptr), height(1), sub_tree_size(1){}
};

template <class Data, class Key>
typename SearchTree<Data, Key>::Node SearchTree<Data, Key>::findByOrder(int place)
{
    if(head == nullptr)
        return nullptr;
    if(head->sub_tree_size < place)
        return nullptr;
    int skipped_counter = 0;
    Node current = head;
    while(!height(current) != 1)
    {
        int right_sub_size = current->right == nullptr ? 0 : current->right->sub_tree_size;
        //if skipping the right son will not skip too much, go inside it   
        if(skipped_counter + right_sub_size == place)//do we start counting at 0 or at 1? -assuming 0.
            return current;
        
        if(skipped_counter + right_sub_size > place)
            current = current->right;
        else//otherwise, skip it, and look in the left son
        {
            skipped_counter += right_sub_size + 1;
            current = current->left; 
        }
    }
    return current;
}

template <class Data, class Key>
void SearchTree<Data, Key>::node_t::updateData()
{
    updateHeight(this);
    sub_tree_size = 1;
    if(left != nullptr)
        sub_tree_size += left->sub_tree_size;
    if(right != nullptr)
        sub_tree_size += right->sub_tree_size;
}

template <class Data, class Key>
void SearchTree<Data, Key>::removeSub(Node top)
{
    if(top==nullptr)
        return;
    if(top->left != nullptr)
        removeSub(top->left);
    if(top->right != nullptr)
        removeSub(top->right);
    Node father = top->father;
    delete top;
    
    // if(father != nullptr)
    //     father->updateData();
}

template <class Data, class Key>
SearchTree<Data, Key>::~SearchTree()
{
    removeSub(head);
}

template <class Data, class Key>
void SearchTree<Data, Key>::assertStructure(Node initial)
{
    if(initial==nullptr)
    {
        if(head==nullptr)
            return;
        initial = head;
    }
    if(initial->left!=nullptr)
    {
        if(!(initial->left->father==initial))
            assert(false);
        assertStructure(initial->left);
    }
    if(initial->right!=nullptr)
    {
        if(!(initial->right->father==initial))
            assert(false);
        assertStructure(initial->right);
    } 
}

template <class Data, class Key>
void SearchTree<Data, Key>::execSub(Node top, std::function<void(Data&)> fun, Order ord, bool revurse)
{
    if(top == nullptr)
        return;

    if(ord == PREORDER)
        fun(top->data);
    revurse ? execSub(top->right, fun, ord, revurse) : execSub(top->left, fun, ord, revurse);
    if(ord == INORDER)
        fun(top->data);
    revurse ? execSub(top->left, fun, ord, revurse) : execSub(top->right, fun, ord, revurse);
    if(ord == POSORDER)
        fun(top->data);
}

template <class Data, class Key>
void SearchTree<Data, Key>::exec(std::function<void(SearchTree<Data, Key>::Node)> fun, bool revurse, Order ord)
{
    execSub(head, fun, ord, revurse);
}

template <class Data, class Key>
class SearchTree<Data, Key>::iterator
{
public:
    iterator(SearchTree<Data, Key>* tree, Node initial_node): tree(tree), current(initial_node), last(nullptr){}
    Node operator*(){return current;}
    iterator& operator++();
    bool operator==(const iterator& it) const{return tree==it.tree && current==it.current && last==it.last;}
    bool operator!=(const iterator& it) const {return !(*this == it);}
private:
    SearchTree<Data, Key>* tree;
    Node current;
    Node last;

    iterator& opAdvanceSmallToBig();
    iterator& opAdvanceBigToSmall();

    iterator& endRotation()
    {
        current = last = nullptr;
        return *this;
    }
    iterator& fatherDo()
    {
        last = current;
        current = current->father;
        return ++(*this);
    }
    iterator& doFather()
    {
        last = current;
        current = current->father;
        return *this;
    }
    iterator& leftDepends()
    {
        last = current;
        current = current->left;
        if(tree->ittirate_in_revese)
        {
            if(current->right==nullptr)
                return *this;
        }
        else
            if(current->left==nullptr)
                return *this;
        return ++(*this);
    }
    iterator& rightDepends()
    {
        last = current;
        current = current->right;
        if(tree->ittirate_in_revese)
        {
            if(current->right==nullptr)
                return *this;
        }    
        else
            if(current->left==nullptr)
                return *this;
        return ++(*this);
    }
};

template <class Data, class Key>
typename SearchTree<Data, Key>::iterator& SearchTree<Data, Key>::iterator::operator++()
{
    return tree->ittirate_in_revese ? opAdvanceBigToSmall() : opAdvanceSmallToBig();
}

template <class Data, class Key>
typename SearchTree<Data, Key>::iterator& SearchTree<Data, Key>::iterator::opAdvanceSmallToBig()
{
    //end state:
    if(current==nullptr && last==nullptr)
        return *this;
    
    //no father(1)
    if(current->father==nullptr)
    {
        //left son exists(1/1)
        if(current->left!=nullptr)
        {
            //right son exists(1/1/1)
            if(current->right!=nullptr)
            {
                //case 1 => r(1)
                if(last==current->left)
                    return rightDepends();
                //case 7 => e(2)
                if(last==current->right)
                    return endRotation();
            }
            //right son does not exist(1/1/2)
            if(current->right==nullptr)
            {
                //case 2 => e(1)
                if(last==current->left)
                    return endRotation();
            }
        }
        //left son does not exist(1/2)
        if(current->left==nullptr)
        {
            //right son exists(1/2/1)
            if(current->right!=nullptr)
            {
                //case 21 => r(0)
                if(last==nullptr)
                    return rightDepends();
                //case 8 => e(2)
                if(last==current->right)
                    return endRotation();
            }
            //right son does not exist(1/2/2)
            if(current->right==nullptr)
            {
                //case 22 => e(0)
                if(last==nullptr)
                    return endRotation();
            }
        }
    }
    //father to the left(2)
    if(current->father->right==current)
    {
        //left son exists(2/1)
        if(current->left!=nullptr)
        {
            //right son exists(2/1/1)
            if(current->right!=nullptr)
            {
                //case 3 => r(1)
                if(last==current->left)
                    return rightDepends();
                //case 9 => f(2)
                if(last==current->right)
                    return fatherDo();
                //case 13 => l(3)
                if(last==current->father)
                    return leftDepends();
            }
            //right son does not exist(2/1/2)
            if(current->right==nullptr)
            {
                //case 4 => f(1)
                if(last==current->left)
                    return fatherDo();
                //case 15 => l(3)
                if(last==current->father)
                  return leftDepends();  
            }
        }
        //left son does not exist(2/2)
        if(current->left==nullptr)
        {
            //right son exists(2/2/1)
            if(current->right!=nullptr)
            {
                //case 10 => f(2)
                if(last==current->right)
                    return fatherDo();
                //case 14 => r(3)
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(2/2/2)
            if(current->right==nullptr)
            {
                //case 16 => f(3)
                if(last==current->father)
                    return fatherDo();
            }
        }
    }
    //father to the right(3)
    if(current->father->left==current)
    {
        //left son exists(3/1)
        if(current->left!=nullptr)
        {
            //right son exists(3/1/1)
            if(current->right!=nullptr)
            {
                //case 5 => r(1)
                if(last==current->left)
                    return rightDepends();
                //case 11 => d(2)
                if(last==current->right)
                    return doFather();
                //case 17 => l(3)
                if(last==current->father)
                    return leftDepends();
            }
            //right son does not exist(3/1/2)
            if(current->right==nullptr)
            {
                //case 6 => d(1)
                if(last==current->left)
                    return doFather();
                //case 19 => l(3)
                if(last==current->father)
                    return leftDepends();
            }
        }
        //left son does not exist(3/2)
        if(current->left==nullptr)
        {
            //right son exists(3/2/1)
            if(current->right!=nullptr)
            {
                //case 23 => r(0)
                if(last==nullptr)
                    return rightDepends();
                //case 12 => d(2)
                if(last==current->right)
                    return doFather();
                //case 18 => r(3)
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(3/2/2)
            if(current->right==nullptr)
            {
                //case 24 => d(0)
                if(last==nullptr)
                    return doFather();
                //case 20 => d(3)
                if(last==current->father)
                    return doFather();
            }
        }
    }
    assert(false);
    return *this;
}

template <class Data, class Key>
typename SearchTree<Data, Key>::iterator& SearchTree<Data, Key>::iterator::opAdvanceBigToSmall()
{
    //end state:
    if(current==nullptr && last==nullptr)
        return *this;

    //no father(1)
    if(current->father==nullptr)
    {
        //left son exists(1)
        if(current->left!=nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 3 => e
                if(last==current->left)
                    return endRotation();
                //case 12 => l
                if(last==current->right)
                    return leftDepends();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                //case 24 => l
                if(last==nullptr)
                    return leftDepends();
                //case 6 => e
                if(last==current->left)
                    return endRotation();
            }
        }
        //left son does not exist(2)
        if(current->left==nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 9 => e
                if(last==current->right)
                    return endRotation();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                if(last==nullptr)
                    return endRotation();
            }
        }
    }
    //father to the right(2)
    if(current->father->left==current)
    {
        //left son exists(1)
        if(current->left!=nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 2 => f
                if(last==current->left)
                    return fatherDo();
                //case 11 => l
                if(last==current->right)
                    return leftDepends();
                //case 16 => r
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                //case 5 => f
                if(last==current->left)
                    return fatherDo();
                //case 20 => l
                if(last==current->father)
                    return leftDepends();
            }
        }
        //left son does not exist(2)
        if(current->left==nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 8 => f
                if(last==current->right)
                    return fatherDo();
                //case 14 => r
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                //case 18 => f
                if(last==current->father)
                    return fatherDo();
            }            
        }
    }
    //father to the left(3)
    if(current->father->right==current)
    {
        //left son exists(1)
        if(current->left!=nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 1 => d
                if(last==current->left)
                    return doFather();
                //case 10 => l
                if(last==current->right)
                    return leftDepends();
                //case 15 => r
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                //case 23 => l
                if(last==nullptr)
                    return leftDepends();
                //case 4 => d
                if(last==current->left)
                    return doFather();
                //case 19 => l
                if(last==current->father)
                    return leftDepends();
            }
        }
        //left son does not exist(2)
        if(current->left==nullptr)
        {
            //right son exists(1)
            if(current->right!=nullptr)
            {
                //case 7 => d
                if(last==current->right)
                    return doFather();
                //case 13 => r
                if(last==current->father)
                    return rightDepends();
            }
            //right son does not exist(2)
            if(current->right==nullptr)
            {
                //case 21 => d
                if(last==nullptr)
                    return doFather();
                //case 17 => d
                if(last==current->father)
                    return doFather();
            }            
        }
    }
    assert(false);
    return *this;
}

template <class Data, class Key>
typename SearchTree<Data, Key>::iterator SearchTree<Data, Key>::begin()
{
    return ittirate_in_revese ? iterator(this, biggest) : iterator(this, smallest);
}

template <class Data, class Key>
typename SearchTree<Data, Key>::iterator SearchTree<Data, Key>::end()
{
    return iterator(this, nullptr);
}

template <class Data, class Key>
bool SearchTree<Data, Key>::insert(Key key, const Data& data)
{
    Node new_node = new node_t(key, data, *this);

    if(head == nullptr)
    {
        head = biggest = smallest = new_node;
        return true;
    }

    Node current = head;
    while(true)
    {
        if(key == current->key)
        {
            delete new_node;
            return false;
        }
        if(key > current->key)
            if(current->right == nullptr)
            {
                current->right = new_node;
                if(biggest->right == new_node)
                    biggest = new_node;
                break;
            }
            else
                current = current->right;
        else
            if(current->left == nullptr)
            {
                current->left = new_node;
                if(smallest->left == new_node)
                    smallest = new_node;
                break;
            }
            else
                current = current->left;
    }
    new_node->father = current;


    //now fix the AVL condition: (the condition is: abs(BalanceFactor)) < 2)
    current = new_node;
    bool exit = false;
    while(current != head)
    {
        if(exit)
            break;
        Node next = current->father;
        switch (BalanceFactor(next))
        {
        case 2:
            if(BalanceFactor(next->left) == -1)
                rollLR(next->left->right, next->left, next);
            else rollLL(next->left, next);
            exit = true;
            break;
        case -2:
            if(BalanceFactor(next->right) == 1)
                rollRL(next->right->left, next->right, next);
            else rollRR(next->right, next);
            exit = true;
            break;
        default:
            next->updateData();
            current = next;
            break;
        }
    }
    while(current != nullptr)
    {
        current->updateData();
        current = current->father;
    }
    return true;
}

// template <class Data, class Key>
// static void SearchTree<Data, Key>::treePrint(Node current)
// {
//     if(current == nullptr)
//         return;
//     if(current->left != nullptr)
//         treePrint(current->left);
//     if(current->right != nullptr)
//         treePrint(current->right);
//     std::cout << current->key << std::endl;
// }

template <class Data, class Key>
typename SearchTree<Data, Key>::Node SearchTree<Data, Key>::closestBiggerSon(Node current)
{
    if(current->right==nullptr)
        return nullptr;
    
    current = current->right;
    while(current->left!=nullptr)
        current = current->left;
    return current;
}

template <class Data, class Key>
typename SearchTree<Data, Key>::Node SearchTree<Data, Key>::closestSmallesrSon(Node current)
{
    if(current->left==nullptr)
        return nullptr;

    current = current->left;
    while(current->right!=nullptr)
        current = current->right;
    return current;
}

template <class Data, class Key>
bool SearchTree<Data, Key>::remove(Key key)
{
    //find Node:
    Node to_remove = findSub(head, key);
    if(to_remove==nullptr)
        return false;

    //correct 'biggest' and 'smallest'
    if(to_remove==biggest)
    {
        biggest = closestSmallesrSon(to_remove);
        if(biggest==nullptr)//in case closestSmallerSon returned null - there is no smaller son.
            biggest = to_remove->father;
    }
    if(to_remove==smallest)
    {
        smallest = closestBiggerSon(to_remove);
        if(smallest==nullptr)
            smallest = to_remove->father;
    }

    //remove Node:
    Node first_to_fix = nullptr;//the first Node to check and fix the AVL condition on.
    //if it has two sons
    if(to_remove->left!=nullptr && to_remove->right!=nullptr)
    {
        //link the top of it's left son, with bottom (bottom left) of the right son.
        first_to_fix = closestBiggerSon(to_remove);
        to_remove->left->father = first_to_fix;//??
        first_to_fix->left = to_remove->left;
        
        if(to_remove==head)
        {
            head = to_remove->right;
            to_remove->right->father = nullptr;
        }
        else
        {
            (to_remove->father->left==to_remove ? to_remove->father->left : to_remove->father->right)
                = to_remove->right;
            to_remove->right->father = to_remove->father;
        }
    }
    //if it has no sons
    if(to_remove->left==nullptr && to_remove->right==nullptr)
    {
        if(to_remove->father==nullptr)
            head = nullptr;
        else//remove the pointer to 'to_remove' from it's father:
            (to_remove->father->left==to_remove ? to_remove->father->left : to_remove->father->right)
                = nullptr;

        first_to_fix = to_remove->father;
    }
    //if it has exactly one son
    if((to_remove->left!=nullptr && to_remove->right==nullptr)
        || (to_remove->left==nullptr && to_remove->right!=nullptr))
    {
        Node& son = to_remove->left!=nullptr ? to_remove->left : to_remove->right; 
        if(to_remove!=head)
        {
            //link son to father
            (to_remove->father->left==to_remove ? to_remove->father->left : to_remove->father->right)
                = son;
            son->father = to_remove->father;

            first_to_fix = to_remove->father;  
        }
        else
        {
            head = son;
            son->father = nullptr;
            //'first_to_fix' will stay null.
        }
    }
    delete to_remove;

    //fix AVL condition:
    Node current = first_to_fix;
    while(current!=nullptr)
    {
        Node new_top = current;
        switch (BalanceFactor(current))
        {
        case 2:
            if(BalanceFactor(current->left) == -1)
            {
                new_top = current->left->right;
                rollLR(new_top, current->left, current);
            }
            else
            {
                new_top = current->left;
                rollLL(new_top, current);
            }
            break;
        case -2:
            if(BalanceFactor(current->right) == 1)
            {
                new_top = current->right->left;
                rollRL(new_top, current->right, current);
            }
            else
            {
                new_top = current->right;
                rollRR(new_top, current);
            }
            break;
        default:
            current->updateData();
            break;
        }
        current = new_top->father;
    }
    
    return true;
}

template <class Data, class Key>
typename SearchTree<Data, Key>::Node SearchTree<Data, Key>::findSub(Node top, Key key) const
{
    if(top == nullptr || key == top->key)
        return top;
    if(key > top->key)
        return findSub(top->right, key);
    else
        return findSub(top->left, key);
}

template <class Data, class Key>
Data& SearchTree<Data, Key>::operator[](Key key)
{
    return findSub(head, key)->data;
}

template <class Data, class Key>
const Data& SearchTree<Data, Key>::operator[](Key key) const
{
    return findSub(head, key)->data;
}

template <class Data, class Key>
void SearchTree<Data, Key>::rollLL(Node A, Node B)
{
    Node oldRightA = A->right;
    A->right = B;
    if(B!=head)
    {
        if(B->father->left==B)
            B->father->left = A;
        else
            B->father->right = A;
    }
    else
        head = A;
    A->father = B->father;
    B->father = A;

    B->left = oldRightA;
    if(oldRightA!=nullptr)
        oldRightA->father = B;

    B->updateData();
    A->updateData();
}

template <class Data, class Key>
void SearchTree<Data, Key>::rollLR(Node A, Node B, Node C)
{
    Node oldLeftA = A->left;
    Node oldRightA = A->right;
    A->left = B;
    B->father = A;

    if(C!=head)
    {
        if(C->father->left==C)
            C->father->left = A;
        else
            C->father->right = A;
    }
    else
        head = A;
    A->father = C->father;

    A->right = C;
    C->father = A;

    B->right = oldLeftA;
    if(oldLeftA!=nullptr)
        oldLeftA->father = B;

    C->left = oldRightA;
    if(oldRightA!=nullptr)
        oldRightA->father = C;

    C->updateData();
    B->updateData();
    A->updateData();
}

template <class Data, class Key>
void SearchTree<Data, Key>::rollRL(Node A, Node B, Node C)
{
    Node oldLeftA = A->left;
    Node oldRightA = A->right;
    A->right = B;
    B->father = A;
    
    if(C!=head)
    {
        if(C->father->left==C)
            C->father->left = A;
        else
            C->father->right = A;
    }
    else
        head = A;
    A->father = C->father;

    A->left = C;
    C->father = A;

    B->left = oldRightA;
    if(oldRightA!=nullptr)
        oldRightA->father = B;

    C->right = oldLeftA;
    if(oldLeftA!=nullptr)
        oldLeftA->father = C;

    C->updateData();
    B->updateData();
    A->updateData();
}

template <class Data, class Key>
void SearchTree<Data, Key>::rollRR(Node A, Node B)
{
    Node oldLeftA = A->left;
    A->left = B;
    if(B!=head)
    {
        if(B->father->left==B)
            B->father->left = A;
        else
            B->father->right = A;
    }
    else
        head = A;
    A->father = B->father;
    B->father = A;

    B->right = oldLeftA;
    if(oldLeftA!=nullptr)
        oldLeftA->father = B;

    B->updateData();
    A->updateData();
}

#endif