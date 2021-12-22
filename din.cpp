#include <iostream>
#include <vector>
#include <algorithm>

//// Delete later
//#include <string>

#define NULLNODE -1

class Node {
public:
    Node() = default;
    Node(char letter);

    bool isInverted();
    bool isRoot();

    int getLongest();
    int getLeftChild();
    int getRightChild();
    int getParent();
    int getCount();

    int getLeft(int letter);
    int getRight(int letter);
    int getLongest(int letter);
    int getValue(int letter);

    // DEBUG
    char getChar();

    void invert();
    void setLeftChild(int id);
    void setRightChild(int id);
    void setParent(int id);
    void setCount(int value);

    void setLeft(int letter, int value);
    void setRight(int letter, int value);
    void setLongest(int letter, int value);
private:
    static int letterToInt(char letter);

    int left[4] = {0, 0, 0, 0};
    int right[4] = {0, 0, 0, 0};
    int longest[4] = {0, 0, 0, 0};
    int value[4] = {0, 0, 0, 0};

    bool inverted = false;
    int count = 0;

    int parent_id = NULLNODE;
    int left_child_id = NULLNODE, right_child_id = NULLNODE;
};

Node::Node(char letter) : Node() {
    value[letterToInt(letter)] = 1;
}

int Node::getLongest() {
    int max1, max2;
    max1 = std::max(longest[0], longest[1]);
    max2 = std::max(longest[2], longest[3]);

    return std::max(max1, max2);
}

int Node::getLeftChild() {
    return left_child_id;
}

int Node::getRightChild() {
    return right_child_id;
}

int Node::getParent() {
    return parent_id;
}

int Node::getCount() {
    return count;
}

int Node::getLeft(int letter) {
    return left[letter];
}

int Node::getRight(int letter) {
    return right[letter];
}

int Node::getLongest(int letter) {
    return longest[letter];
}

int Node::getValue(int letter) {
    return value[letter];
}

bool Node::isInverted() {
    return inverted;
}

bool Node::isRoot() {
    return parent_id == NULLNODE;
}

char Node::getChar() {
    if (value[0] != 0) {
        return 'A';
    }

    if (value[1] != 0) {
        return 'G';
    }

    if (value[2] != 0) {
        return 'C';
    }

    return 'T';
}

void Node::invert() {
    inverted = !inverted;
}

void Node::setLeftChild(int id) {
    left_child_id = id;
}

void Node::setRightChild(int id) {
    right_child_id = id;
}

void Node::setParent(int id) {
    parent_id = id;
}

void Node::setCount(int value) {
    count = value;
}

void Node::setLeft(int letter, int value) {
    left[letter] = value;
}

void Node::setRight(int letter, int value) {
    right[letter] = value;
}

void Node::setLongest(int letter, int value) {
    longest[letter] = value;
}

int Node::letterToInt(char letter) {
    switch (letter) {
        case 'A':
            return 0;
        case 'G':
            return 1;
        case 'C':
            return 2;
        case 'T':
            return 3;
        default:
            exit(1);
    }
}

class SplayTree {
public:
    SplayTree() = default;
    SplayTree(const std::vector<char> &code);

    void invert(int j, int k);
    int getLongest(int j, int k);
    
    // DEBUG
    void dbgPrintSeq();
    void dbgPrintSeq(int id, bool invert);

private:
    int root_id = NULLNODE;

    bool isEmpty();
    bool isRightChild(int id);

    void setRightChild(int parent, int child);
    void setLeftChild(int parent, int child);

    void insert(int id);
    void discharge(int id);

    void update(int id);
    void updateCount(int id);
    void updateValues(int id);
    void updateValues(int letter, int id);
    

    void zig(int id, bool is_right);
    void zigzig(int id, bool is_right);
    void zigzag(int id, bool is_right);

    void splayUp(int id);
    void splay(int n, int root_id, int &id);
    //void splay(int id);


    void split(int root_id, int id, int &root1_id, int &root2_id);
    void join(int root1_id, int root2_id, int &root_id);

    std::vector<Node> nodes;
};

void SplayTree::dbgPrintSeq() {
    dbgPrintSeq(root_id, false);
    std::cout << '\n';
}

void SplayTree::dbgPrintSeq(int id, bool invert) {
    if (id == NULLNODE) {
        return;
    }
    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();
    if (nodes[id].isInverted()) {
        invert = !invert;
    }

    if (invert) {
        int temp = l_child;
        l_child = r_child;
        r_child = temp;
    }

    dbgPrintSeq(l_child, invert);
    std::cout << nodes[id].getChar();
    dbgPrintSeq(r_child, invert);
}

SplayTree::SplayTree(const std::vector<char> &code) {
    nodes = std::vector<Node>(code.size());
    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i] = Node(code[i]);
        insert(i);
    }
}

// Insertion takes place only during initialization
// therefore we take advantage of the fact that
// every inserted node will be greater than the 
// previous one
void SplayTree::insert(int id) {
    if (isEmpty()) {
        root_id = id;
        update(id);
    }
    else {
        setLeftChild(id, root_id);
        root_id = id;
        update(id);
    }
}

void SplayTree::discharge(int id) {
    if (!nodes[id].isInverted()) {
        return;
    }

    nodes[id].invert();

    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    nodes[id].setLeftChild(r_child);
    nodes[id].setRightChild(l_child);

    if (l_child != NULLNODE) {
        nodes[l_child].invert();
    }

    if (r_child != NULLNODE) {
        nodes[r_child].invert();
    }
}

void SplayTree::update(int id) {
    updateCount(id);
    updateValues(id);
}

void SplayTree::updateCount(int id) {
    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    int count = 1;
    if (l_child != NULLNODE) {
        count += nodes[l_child].getCount();
    }

    if (r_child != NULLNODE) {
        count += nodes[r_child].getCount();
    }

    nodes[id].setCount(count);
}

void SplayTree::updateValues(int id) {
    int p_id = nodes[id].getParent();

    bool is_root = nodes[id].isRoot();
    bool is_right = !is_root && isRightChild(id);

    for (int i = 0; i < 4; ++i) {
        updateValues(i, id);
    }
}

void SplayTree::updateValues(int letter, int id) {
    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    bool l_null = l_child == NULLNODE;
    bool r_null = r_child == NULLNODE;

    int val = nodes[id].getValue(letter);

    int l_m = 0;
    int l_l = 0;
    int l_r = 0;
    int l_c = 0;
    if (!l_null) {
        l_m = nodes[l_child].getLongest(letter);
        l_l = nodes[l_child].getLeft(letter);
        l_r = nodes[l_child].getRight(letter);
        l_c = nodes[l_child].getCount();

        if (nodes[l_child].isInverted()) {
            int temp = l_l;
            l_l = l_r;
            l_r = temp;
        }
    }

    int r_m = 0;
    int r_r = 0;
    int r_l = 0;
    int r_c = 0;
    if (!r_null) {
        r_m = nodes[r_child].getLongest(letter);
        r_r = nodes[r_child].getRight(letter);
        r_l = nodes[r_child].getLeft(letter);
        r_c = nodes[r_child].getCount();

        if (nodes[r_child].isInverted()) {
            int temp = r_l;
            r_l = r_r;
            r_r = temp;
        }
    }

    int max1 = std::max(l_m, r_m);
    if (val == 0) {
        nodes[id].setLeft(letter, l_l);
        nodes[id].setRight(letter, r_r);
        
        nodes[id].setLongest(letter, max1);
        return;
    }

    // val > 0
    int s1 = 0, s2 = 0;
    if (l_r > 0) {
        if (r_l > 0) {
            s1 = l_r + 1 + r_l;
        }
        else {
            s1 = l_r + 1;
        }
    }
    else {
        if (r_l > 0) {
            s1 = r_l + 1;
        }
        else {
            s1 = 1;
        }
    }
    
    // Set Max
    int max2 = std::max(s1, s2);
    nodes[id].setLongest(letter, std::max(max1, max2));

    // Set Left and Right

    int l_v = 0;
    if (l_r == l_c) {
        l_v = l_r + 1 + r_l;
    }
    nodes[id].setLeft(letter, l_v);

    int r_v = 0;
    if (r_l == r_c) {
        r_v = r_l + 1 + l_r;
    }
    nodes[id].setRight(letter, r_v);
}

// parent is root
void SplayTree::zig(int id, bool is_right) {
    int p_id = nodes[id].getParent();

    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    if (is_right) {
        setLeftChild(id, p_id);
        setRightChild(p_id, l_child);
    }
    else {
        setRightChild(id, p_id);
        setLeftChild(p_id, r_child);
    }
    // Making x the root
    nodes[id].setParent(NULLNODE);

    update(p_id);
    update(id);
}

void SplayTree::zigzig(int id, bool is_right) {
    int p_id = nodes[id].getParent();
    int g_id = nodes[p_id].getParent();
    int gp_id = nodes[g_id].getParent();
    bool is_g_root = gp_id == NULLNODE;
    bool is_g_right = !is_g_root && isRightChild(g_id);
    
    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    int p_l_child = nodes[p_id].getLeftChild();
    int p_r_child = nodes[p_id].getRightChild();

    if (is_right) {
        setRightChild(g_id, p_l_child);
        setLeftChild(p_id, g_id);
        setRightChild(p_id, l_child);
        setLeftChild(id, p_id);
    }
    else {
        setLeftChild(g_id, p_r_child);
        setRightChild(p_id, g_id);
        setLeftChild(p_id, r_child);
        setRightChild(id, p_id);
    }
    if (is_g_root) {
        nodes[id].setParent(NULLNODE);
    }
    else {
        if (is_g_right) {
            setRightChild(gp_id, id);
        }
        else {
            setLeftChild(gp_id, id);
        }
    }

    update(g_id);
    update(p_id);
    update(id);
}

void SplayTree::zigzag(int id, bool is_right) {
    int p_id = nodes[id].getParent();
    int g_id = nodes[p_id].getParent();
    int gp_id = nodes[g_id].getParent();

    bool is_g_root = gp_id == NULLNODE;
    bool is_g_right = !is_g_root && isRightChild(g_id);

    int l_child = nodes[id].getLeftChild();
    int r_child = nodes[id].getRightChild();

    if (is_right) {
        setRightChild(p_id, l_child);
        setLeftChild(g_id, r_child);
        setLeftChild(id, p_id);
        setRightChild(id, g_id);
    }
    else {
        setLeftChild(p_id, r_child);
        setRightChild(g_id, l_child);
        setRightChild(id, p_id);
        setLeftChild(id, g_id);
    }
    if (is_g_root) {
        nodes[id].setParent(NULLNODE);
    }
    else {
        if (is_g_right) {
            setRightChild(gp_id, id);
        }
        else {
            setLeftChild(gp_id, id);
        }
    }

    update(g_id);
    update(p_id);
    update(id);
}

void SplayTree::splayUp(int id) {
    int p_id = nodes[id].getParent();

    bool is_right = isRightChild(id);
    bool is_p_root = nodes[p_id].isRoot();
    bool is_p_right = !is_p_root && isRightChild(p_id);

    if (is_p_root) {
        zig(id, is_right);
    }
    else if (is_right == is_p_right) {
        zigzig(id, is_right);
    }
    else {
        zigzag(id, is_right);
    }
}

void SplayTree::splay(int n, int root_id, int &id) {
    // Binsearch the tree to find the nth element
    // during which we discharge all the inversion 
    // bits on the path
    bool found = false;
    while (!found) {
        discharge(root_id);

        int left_child = nodes[root_id].getLeftChild();
        int right_child = nodes[root_id].getRightChild();

        int l_count = 0;
        if (left_child != NULLNODE) {
            l_count = nodes[left_child].getCount();
        }

        if (l_count >= n) {
            root_id = left_child;
        }
        else if (l_count == n - 1) {
            found = true;
            id = root_id;
        }
        else {
            root_id = right_child;
            n -= l_count + 1;
        }
    }

    // splayUp the tree until the found element 
    // is the root
    while (!nodes[id].isRoot()) {
        splayUp(id);
    }
}

void SplayTree::invert(int j, int k) {
    // inverting an interval of size 1 does not 
    // change the structure of the splay tree
    if (j == k) {
        return;
    }

    int left_root, mid_root, right_root;

    if (j == 0 && k == (nodes.size() - 1)) {
        nodes[root_id].invert();
        return;
    }

    if (j == 0) {
        split(root_id, k + 1, mid_root, right_root);
        nodes[mid_root].invert();

        join(mid_root, right_root, root_id);
        return;
    }

    if (k == (nodes.size() - 1)) {
        split(root_id, j, left_root, mid_root);
        nodes[mid_root].invert();

        join(left_root, mid_root, root_id);
        return;
    }

    split(root_id, j, left_root, mid_root);
    split(mid_root, k - j + 1, mid_root, right_root);

    nodes[mid_root].invert();

    join(left_root, mid_root, root_id);
    join(root_id, right_root, root_id);
}

int SplayTree::getLongest(int j, int k) {
    int left_root, mid_root, right_root;

    if (j == 0 && k == (nodes.size() - 1)) {
        return nodes[root_id].getLongest();
    }

    int longest;
    if (j == 0) {
        split(root_id, k + 1, mid_root, right_root);
        longest = nodes[mid_root].getLongest();

        join(mid_root, right_root, root_id);
        return longest;
    }

    if (k == (nodes.size() - 1)) {
        split(root_id, j, left_root, mid_root);
        longest = nodes[mid_root].getLongest();

        join(left_root, mid_root, root_id);
        return longest;
    }

    split(root_id, j, left_root, mid_root);
    split(mid_root, k - j + 1, mid_root, right_root);

    longest = nodes[mid_root].getLongest();

    join(left_root, mid_root, root_id);
    join(root_id, right_root, root_id);

    return longest;
}

bool SplayTree::isEmpty() {
    return root_id == NULLNODE;
}

bool SplayTree::isRightChild(int id) {
    int p_id = nodes[id].getParent();
    return nodes[p_id].getRightChild() == id;
}

void SplayTree::setRightChild(int parent, int child) {
    nodes[parent].setRightChild(child);
    if (child != NULLNODE) {
        nodes[child].setParent(parent);
    }
}

void SplayTree::setLeftChild(int parent, int child) {
    nodes[parent].setLeftChild(child);
    if (child != NULLNODE) {
        nodes[child].setParent(parent);
    }
}

void SplayTree::split(int root_id, int n, int &root1_id, int &root2_id) {
    // nth element will be the new root
    splay(n, root_id, root1_id);

    // Detach first tree from the second one
    if (nodes[root1_id].isInverted()) {
        root2_id = nodes[root1_id].getLeftChild();
        nodes[root1_id].setLeftChild(NULLNODE);
    }
    else {
        root2_id = nodes[root1_id].getRightChild();
        nodes[root1_id].setRightChild(NULLNODE);
    }

    nodes[root2_id].setParent(NULLNODE);

    // Update roots
    update(root1_id);
    update(root2_id);
}

void SplayTree::join(int root1_id, int root2_id, int &root_id) {
    // Splay the largest node
    int n = nodes[root1_id].getCount();
    int id;
    splay(n, root1_id, id);

    if (nodes[id].isInverted()) {
        setLeftChild(id, root2_id);
    }
    else {
        setRightChild(id, root2_id);
    }
    root_id = id;

    // Update the root
    update(root_id);
}

class Din {
public:
    void start();

    Din() = default;
private:
    int code_size = 0, op_count = 0;
    int size = 0;
    SplayTree tree;

    int getDinedSize();

    void loadParam();
    void loadCode();

    void calculate();

    void invert(int j, int k);
    void move(int j, int k, int l);
    void getLongest(int j, int k);
};

int Din::getDinedSize() {
    return size;
}

void Din::loadParam() {
    std::cin >> code_size >> op_count;
}

void Din::loadCode() {
    std::vector<char> temp_code(code_size);
    for (int i = 0; i < code_size; ++i) {
        std::cin >> temp_code[i];
    }

    this->tree = SplayTree(temp_code);
}

void Din::calculate() {
    char op;
    std::cin >> op;
    int j, k;
    std::cin >> j >> k;
    switch (op) {
        case 'O':
            invert(j - 1, k - 1);
            break;
        case 'P':
            int l;
            std::cin >> l;
            move(j - 1, k - 1, l - 1);
            break;
        case 'N':
            getLongest(j - 1, k - 1);
            break;
        default:
            break;
    }
}

void Din::invert(int j, int k) {
    tree.invert(j, k);
}

void Din::move(int j, int k, int l) {
    if (l < j) {
        int middle_index = l + k - j;
        invert(l, k);
        invert(l, middle_index);
        invert(middle_index + 1, k);
    }
    else {
        int mapped_l = l + (k - j + 1);
        int middle_index = j + mapped_l - k - 1;

        if (mapped_l - 1 == k) {
            return;
        }
        else {
            invert(j, mapped_l - 1);
            invert(j, middle_index - 1);
            invert(middle_index, mapped_l - 1);
        }
    }
}

void Din::getLongest(int j, int k) {
    int longest = tree.getLongest(j, k);
    std::cout << longest << "\n";
}

void Din::start() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    loadParam();
    loadCode();
    tree.dbgPrintSeq();

    for (int i = 0; i < this->op_count; i++) {
        calculate();
        tree.dbgPrintSeq();
    }
}

int main() {
    Din p;
    p.start();
}