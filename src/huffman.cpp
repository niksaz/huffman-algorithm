#include <iostream>
#include <vector>
#include <queue>

#include "huffman.hpp"
#include "bitio.hpp"
#include "constants.hpp"

using namespace std;

typedef pair<size_t, uint8_t> CharInfo;

struct TreeNode {
    size_t cnt;
    size_t left;
    size_t right;
    bool operator<(const TreeNode& rhs) const {
        return cnt > rhs.cnt;
    }
};

void get_information_from_file(ByteReader& input, vector<CharInfo>& cnt) {
    cnt.clear();
    for (size_t code = 0; code < ALPHABET; code++) {
        cnt.push_back({0, code});
    }
    while (!input.no_more_characters()) {
        cnt[input.read_byte()].first++;
    }
}

vector<TreeNode> build_tree_from_info(vector<CharInfo>& cnt) {
    sort(cnt.begin(), cnt.end());
    vector<TreeNode> tree;
    priority_queue<TreeNode> node_set;
    for (auto& entry : cnt) {
        if (entry.first != 0) {
            node_set.push({entry.first, entry.second, entry.second});
        }
    }
    while (node_set.size() > 1) {
        auto node1 = node_set.top();
        node_set.pop();
        tree.push_back(node1);
        
        auto node2 = node_set.top();
        node_set.pop();
        tree.push_back(node2);
        
        node_set.push({node1.cnt + node2.cnt, tree.size() - 1, tree.size() - 2});
    }
    tree.push_back(node_set.top());
    return tree;
}

void traverse(vector<TreeNode>& tree, vector<string>& convert_to, ByteWriter& out, size_t node, string prefix) {
    if (tree[node].left == tree[node].right) {
        out.write_bit(0);
        out.write_byte(tree[node].left);
        convert_to[tree[node].left] = prefix;
        return;
    }
    out.write_bit(1);
    traverse(tree, convert_to, out, tree[node].left,  prefix + '0');
    traverse(tree, convert_to, out, tree[node].right, prefix + '1');
}

void encode(ByteReader& input, ByteWriter& output, vector<string>& convert_to) {
    while (!input.no_more_characters()) {
        output.write_binary_string(convert_to[input.read_byte()]);
    }
}

void HuffmanArchiver::archive(const string& input_file, const string& output_file) {
    ByteReader input(input_file);
    ByteWriter output(output_file);
    
    if (input.no_more_characters()) {
        cout << "0\n0\n0\n";
        return;
    } 
    
    vector<CharInfo> cnt;
    try {
        get_information_from_file(input, cnt);
    } catch (ios_base::failure exception) {
        cerr << exception.what() << "\n";
        return;
    }
    
    auto tree = build_tree_from_info(cnt);
    
    input.clear_flags();
    auto input_size = input.tell_complete_bytes();
    
    output.write_size_t(input_size, LOG_MAX_SIZE);
    
    vector<string> convert_to(ALPHABET);
    traverse(tree, convert_to, output, tree.size() - 1, "");
    auto extra_size = output.tell_complete_bytes();
    
    input.seekg(0);
    try {
        encode(input, output, convert_to);
    } catch (ios_base::failure exception) {
        cerr << exception.what() << "\n";
        return;
    }

    output.complete_byte();
    auto output_size = output.tell_complete_bytes() - extra_size;
    
    cout << input_size << "\n" << output_size << "\n" << extra_size << "\n";
}

size_t build_traverse(vector<TreeNode>& tree, ByteReader& reader) {
    auto current_pos = tree.size();
    auto going = reader.read_bit();
    if (going == 0) {
        auto code = reader.read_byte();
        tree.push_back({0, code, code});
    } else {
        tree.push_back({0, 0, 0});
        tree[current_pos].left = build_traverse(tree, reader);
        tree[current_pos].right = build_traverse(tree, reader);
    }
    return current_pos;
}

void decode(ByteReader& input, ByteWriter& output, vector<TreeNode>& tree, size_t output_size) {
    for (size_t unzipped_characters = 0; unzipped_characters < output_size; unzipped_characters++) {
        size_t node = 0;
        while (tree[node].left != tree[node].right) {
            auto next_bit = input.read_bit();
            node = (next_bit == 0 ? tree[node].left : tree[node].right);
        }
        output.write_byte(tree[node].left);
    }
}

void HuffmanArchiver::unzip(const std::string &input_file, const std::string &output_file) {
    ByteReader input(input_file);
    ByteWriter output(output_file);
    
    if (input.no_more_characters()) {
        cout << "0\n0\n0\n";
        return;
    }
    
    size_t output_size;
    try {
        output_size = input.read_size_t(LOG_MAX_SIZE);
    } catch (ios_base::failure exception) {
        cerr << exception.what() << "\n";
        return;
    }
    
    vector<TreeNode> tree;
    try {
        build_traverse(tree, input);
    } catch (ios_base::failure exception) {
        cerr << exception.what() << "\n";
        return;
    }
    
    auto extra_size = input.tell_complete_bytes();
    
    try {
        decode(input, output, tree, output_size);
    } catch (ios_base::failure exception) {
        cerr << exception.what() << "\n";
        return;
    }
    
    input.complete_byte();
    auto input_size = input.tell_complete_bytes() - extra_size;
    if (!input.no_more_characters()) {
        throw ios_base::failure("Incorrect format. Extra bits were found.");
    }
    
    cout << input_size << "\n" << output_size << "\n" << extra_size << "\n";
}
