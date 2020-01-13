#include <iostream>
#include<stdint.h>
#include <unordered_map>
#include <vector>
using namespace std;

vector<unsigned>listLength;


struct Node{
	unsigned termid;
	Node*prev, *next;
};
int64_t CACHE_SIZE = 1024 * 1024;

class LRUCache{
public:
	LRUCache();
	~LRUCache();
	Node* Put(unsigned key);
	Node* Get(unsigned key);
	void cacheClear();
	void print();
	uint64_t hit_size;
	uint64_t miss_size;
	uint64_t hit_count;
	uint64_t miss_count;

	void attach(Node *node);
	void detach(Node *node);

	unordered_map<unsigned, Node*>hashmap_;
	Node*head_, *tail_;
	int64_t sumBytes;
};

LRUCache::LRUCache()
{
	miss_size = 0; hit_size = 0;
	miss_count = 0; hit_count = 0;
	head_ = new Node;
	tail_ = new Node;
	head_->prev = NULL;
	head_->next = tail_;
	tail_->prev = head_;
	tail_->next = NULL;
	sumBytes = 0;
}

LRUCache::~LRUCache()
{
	delete head_;
	delete tail_;
}

Node* LRUCache::Put(unsigned key)
{
	Node *node;
	unsigned keylistlength = listLength[key];
	if (keylistlength> CACHE_SIZE)
	{
		cout << "That block overflow!!" << endl;
		return NULL;
	}
	node = tail_->prev;
	while (sumBytes + keylistlength>CACHE_SIZE)
	{
		if (node == head_){  break; }
		detach(node);
		sumBytes -= listLength[node->termid];
		hashmap_.erase(node->termid);
		Node *tmp = node->prev;
		delete node;
		node = tmp;
	}
	node = new Node();
	node->termid = key;
	sumBytes += keylistlength;
	attach(node); 
	hashmap_[key] = node;
	return node;
}

Node* LRUCache::Get(unsigned key)
{
	Node *node;
	unordered_map<unsigned, Node* >::iterator it = hashmap_.find(key);
	if (it != hashmap_.end())
	{
		node = it->second;
		hit_count++;
		detach(node);
		attach(node);
	}
	else
	{
		miss_count++;
		node = Put(key);
		miss_size += listLength[key];
	}
	return node;
}

void LRUCache::attach(Node *node)
{
	node->next = head_->next;
	head_->next = node;
	node->next->prev = node;
	node->prev = head_;
}


void LRUCache::detach(Node *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}

void LRUCache::print()
{
	unordered_map<unsigned, Node* >::iterator iter;
	int64_t mysumsize = 0;
	for (iter = hashmap_.begin(); iter != hashmap_.end(); iter++)
	{
		mysumsize += listLength[iter->second->termid];
	}
	cout << "sumsize=" << mysumsize << endl;
}
void LRUCache::cacheClear()
{
	Node *node = tail_->prev;
	while (node != head_)
	{
		detach(node);
		sumBytes -= listLength[node->termid];
		hashmap_.erase(node->termid);
		Node *tmp = node->prev;
		delete node;
		node = tmp;
	}
}
