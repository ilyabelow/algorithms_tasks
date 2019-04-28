#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <stdio.h>
#include "Huffman.h"

class Buffer {
private:
    std::vector<byte> buff;

    unsigned long pos = 1;
    byte shift = 0;


public:
    Buffer() : pos(0), shift(0), buff() { buff.push_back(0); }

    Buffer(const Buffer &_b) {
        buff.clear();
        for (unsigned long i = 0; i < _b.buff.size(); ++i) {
            buff.push_back(_b.buff[i]);
        }
        pos = _b.pos;
        shift = _b.shift;
    }

    ~Buffer() = default;

    Buffer &operator=(const Buffer &app);


    void WriteBit(const bool app);

    void WriteByte(const byte app);

    void WriteBuffer(const Buffer &app);


    Buffer &operator+=(const bool app);

    Buffer &operator+=(const byte app);

    Buffer &operator+=(const Buffer &app);


    const Buffer operator+(const bool app) const;

    const Buffer operator+(const byte app) const;

    const Buffer operator+(const Buffer &app) const;


    bool ReadBit(const unsigned long index) const;

    byte ReadByte(const unsigned long index) const;

    byte ReadWholeByte(const unsigned long index) const;

    std::string Bits() const;

    std::vector<byte> GetBuffer() const;

    unsigned long GetBufferSize() const;

    unsigned long GetBitSize() const;

    byte GetShift() const;

    void SetShift(const byte _shift);

    void LastByteIsALie();

    void LastByteIsNotALie();

};


Buffer &Buffer::operator=(const Buffer &b) {
    if (&b == this) {
        return *this;
    }
    buff.clear();
    for (unsigned long i = 0; i < b.buff.size(); ++i) {
        buff.push_back(b.buff[i]);
    }
    pos = b.pos;
    shift = b.shift;
    return *this;
}


void Buffer::WriteBit(const bool app) {
    byte byte_app;
    app ? byte_app = 128 : byte_app = 0;
    buff[pos] |= byte_app >> shift;
    ++shift;
    if (shift == 8) {
        shift = 0;
        pos++;
        buff.push_back(0);
    }
}

void Buffer::WriteByte(const byte app) {
    buff[pos] |= app >> shift;
    pos++;
    buff.push_back(app << (8 - shift));
}

void Buffer::WriteBuffer(const Buffer &app) {
    for (unsigned long i = 0; i < app.GetBufferSize() - 1; ++i) {
        WriteByte(app.ReadWholeByte(i));
    }
    buff[pos] |= (app.ReadWholeByte(app.pos) >> shift);
    if (shift + app.shift < 8) {
        shift += app.shift;
    } else {
        buff.push_back(app.ReadWholeByte(app.pos) << (8 - shift));
        shift = (shift + app.shift) % 8;
        pos++;

    }
}


Buffer &Buffer::operator+=(const bool app) {
    WriteBit(app);
    return *this;
}

Buffer &Buffer::operator+=(const byte app) {
    WriteByte(app);
    return *this;
}

Buffer &Buffer::operator+=(const Buffer &app) {
    WriteBuffer(app);
    return *this;
}


const Buffer Buffer::operator+(const bool app) const {
    Buffer b(*this);
    b += app;
    return b;
}

const Buffer Buffer::operator+(const byte app) const {
    Buffer b(*this);
    b += app;
    return b;
}

const Buffer Buffer::operator+(const Buffer &app) const {
    Buffer b(*this);
    b += app;
    return b;
}


bool Buffer::ReadBit(const unsigned long index) const {
    return (buff[index / 8] & (128 >> (index % 8))) >> (7 - (index % 8)) == 1;
}

byte Buffer::ReadByte(const unsigned long index) const {
    if (index % 8 == 0) {
        return buff[index / 8];
    }
    byte result = 0;
    for (int i = 7; i >= 0; i--) {
        result |= (ReadBit(index + 7 - i) ? 1 : 0) << i;
    }
    return result;
}

byte Buffer::ReadWholeByte(const unsigned long index) const {
    return buff[index];
}

std::vector<byte> Buffer::GetBuffer() const {
    return buff;
}

unsigned long Buffer::GetBufferSize() const {
    return buff.size();
}

unsigned long Buffer::GetBitSize() const {
    return pos * 8 + shift;
}

std::string Buffer::Bits() const {
    std::string result;
    for (unsigned long i = 0; i < GetBitSize(); ++i) {
        result += (i % 8 == 0 ? " " : "");
        result += (ReadBit(i) ? '1' : '0');

    }
    return result;
}

byte Buffer::GetShift() const {
    return shift;
}

void Buffer::SetShift(const byte _shift) {
    shift = _shift;
}

void Buffer::LastByteIsALie() {
    --pos;
    buff.pop_back();
}

void Buffer::LastByteIsNotALie() {
    pos++;
    shift = 0;
    buff.push_back(0);
}

class HuffmanCompressor {
private:

    struct Node {
        struct Compare {
            bool operator()(const Node *first, const Node *second) {
                return first->frequency > second->frequency;
            }
        };

        byte key;
        unsigned long frequency;
        const Node *left;
        const Node *right;

        //Leaf
        Node(byte _key, unsigned long _frequency) : key(_key), frequency(_frequency), left(nullptr), right(nullptr) {};

        //Branch
        Node(Node *_left, Node *_right) : key(0), frequency(_left->frequency + _right->frequency), left(_left),
                                          right(_right) {};

        Node(const Node *n) : key(n->key), frequency(n->frequency), left(n->left), right(n->right) {}

        ~Node() = default;

        Node() : left(nullptr), right(nullptr), frequency(0), key(0) {}

        Node &operator=(const Node *&n);

        bool isLeaf() const;
    };


    Node *buildTree(Buffer &buffer) const;

    Buffer packTree(const Node *tree);

    Node *extractTree(Buffer &buffer, unsigned long &shift) const;

    void deleteTree(const Node *tree);

    void findByte(const Node *node, std::vector<Buffer> &table, const Buffer &way);


    Buffer encode(const Buffer &buffer, const Node *tree);

    Buffer decode(const Buffer &buffer, const Node *tree, unsigned long shift);

public:

    HuffmanCompressor() = default;

    ~HuffmanCompressor() = default;

    Buffer Encode(Buffer &buffer);

    Buffer Decode(Buffer &buffer);


};


bool HuffmanCompressor::Node::isLeaf() const {
    return left == nullptr && right == nullptr;
}

HuffmanCompressor::Node &HuffmanCompressor::Node::operator=(const Node *&n) {
    if (n == this) {
        return *this;
    }
    key = n->key;
    frequency = n->frequency;
    left = n->left;
    right = n->right;
    return *this;
}


HuffmanCompressor::Node *HuffmanCompressor::buildTree(Buffer &buffer) const {
    unsigned long freq[256];
    for (unsigned long &i : freq) {
        i = 0;
    }
    for (unsigned long i = 0; i < buffer.GetBufferSize() -
                                  1; ++i) { //-1 т.к. входной буфер точно быдуте полным и будет содержать в себе последний мусорный байт
        ++freq[buffer.ReadWholeByte(i)];
    }
    std::priority_queue<Node *, std::vector<Node *>, Node::Compare> heap;

    for (int i = 0; i < 256; ++i) {
        if (freq[i] != 0) {
            heap.push(new Node((byte) i, freq[i]));
        }
    }
    while (heap.size() > 1) { ;
        Node *node1 = heap.top();
        heap.pop();
        Node *node2 = heap.top();
        heap.pop();
        heap.push(new Node(node2, node1));
    }
    Node *tree = heap.top();
    heap.pop();
    return tree;
}

Buffer HuffmanCompressor::packTree(const HuffmanCompressor::Node *tree) {
    Buffer result;
    if (tree->isLeaf()) {
        result += true;
        result += tree->key;
    } else {
        Buffer leftTree = packTree(tree->left);
        Buffer rightTree = packTree(tree->right);
        result += false;
        result += leftTree;
        result += rightTree;
    }
    return result;
}

HuffmanCompressor::Node *HuffmanCompressor::extractTree(Buffer &buffer, unsigned long &shift) const {
    Node *node = nullptr;
    if (!buffer.ReadBit(shift)) {
        Node *leftTree = extractTree(buffer, ++shift);
        Node *rightTree = extractTree(buffer, ++shift);
        node = new Node(leftTree, rightTree);
    } else if (buffer.ReadBit(shift)) {
        node = new Node(buffer.ReadByte(++shift), 0);
        shift += 7; //7 а не 8 потому что на один бит уже съехали
    } else {
        throw 1;
    }
    return node;
}

void HuffmanCompressor::deleteTree(const Node *tree) {
    if (tree == nullptr) {
        return;
    }
    deleteTree(tree->left);
    deleteTree(tree->right);
    delete tree->left;
    delete tree->right;
}

void HuffmanCompressor::findByte(const Node *node, std::vector<Buffer> &table, const Buffer &way) {
    if (node == nullptr) {
        return;
    }
    if (node->isLeaf()) {
        table[node->key] = way;
    }
    findByte(node->left, table, way + false);
    findByte(node->right, table, way + true);
}


Buffer HuffmanCompressor::encode(const Buffer &buffer, const Node *tree) {
    Buffer result;
    std::vector<Buffer> table(256);
    Buffer str;
    findByte(tree, table, str);
    for (unsigned long i = 0; i < buffer.GetBufferSize() - 1; ++i) {//для -1 та же причина
        result += table[buffer.ReadWholeByte(i)];
    }
    return result;
}

Buffer HuffmanCompressor::decode(const Buffer &buffer, const Node *tree, unsigned long shift) {
    Buffer result;
    const Node *current = tree;
    for (unsigned long i = shift; i < buffer.GetBitSize(); ++i) {
        if (!buffer.ReadBit(i)) {
            current = current->left;
        } else if (buffer.ReadBit(i)) {
            current = current->right;
        } else {
            throw 1;
        }
        if (current->isLeaf()) {
            result += current->key;
            current = tree;
        }
    }
    return result;
}


Buffer HuffmanCompressor::Encode(Buffer &buffer) {
    Node *tree = buildTree(buffer);
    Buffer btree = packTree(tree);
    Buffer encoded = encode(buffer, tree);
    Buffer resultR;
    resultR = btree;
    resultR += encoded;
    Buffer result;
    byte shift = ((resultR.GetShift() + 3) % 8) << 5;
    result += shift;
    result.SetShift(3);
    result.LastByteIsALie();
    result += resultR;
    deleteTree(tree);
    delete tree;
    return result;
}

Buffer HuffmanCompressor::Decode(Buffer &buffer) {
    byte buffer_shift = buffer.ReadBit(0) * 4 + buffer.ReadBit(1) * 2 + buffer.ReadBit(2);
    unsigned long shift = 3;
    Node *tree = extractTree(buffer, shift);
    ++shift;
    buffer.SetShift(buffer_shift);
    Buffer result = decode(buffer, tree, shift);
    deleteTree(tree);
    delete tree;
    return result;
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    byte value;
    Buffer input;
    while (original.Read(value)) {
        input += value;
    }
    HuffmanCompressor hc;
    Buffer preoutput = hc.Encode(input);
    preoutput.LastByteIsNotALie();
    Buffer output = hc.Encode(preoutput);
    for (unsigned long i = 0; i < output.GetBufferSize(); ++i) {
        compressed.Write(output.ReadWholeByte(i));
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    byte value;
    Buffer input;
    while (compressed.Read(value)) {
        input += value;
    }
    input.LastByteIsALie();

    HuffmanCompressor hc;
    Buffer preoutput = hc.Decode(input);
    preoutput.LastByteIsALie();

    Buffer output = hc.Decode(preoutput);

    for (unsigned long i = 0; i < output.GetBufferSize() - 1; ++i) {
        original.Write(output.ReadWholeByte(i));
    }
}
