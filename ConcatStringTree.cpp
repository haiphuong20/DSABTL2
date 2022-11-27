#include "ConcatStringTree.h"

int ConcatStringTree::bid = 1;

tNode::tNode()
{
	data = nullptr;
	length1 = 0;
	leftL = 0;
	left = nullptr;
	right = nullptr;
	key = 0;
	lithash = nullptr;
	if (ConcatStringTree::bid > 1e7) throw overflow_error("Id is overflow!");
	id = ConcatStringTree::bid;
	ConcatStringTree::bid++;
	tree = new ParentsTree();
}

tNode::~tNode()
{
	length1 = 0;
	leftL = 0;
	left = nullptr;
	right = nullptr;
	key = 0;
	delete tree;
	if (lithash != nullptr)
	{
		lithash->deletehash(data);
	}
	else
	{
		delete data;
	}
	data = nullptr;
	tree = nullptr;
	lithash = nullptr;
}
void tNode::checkPar(tNode* par)
{
	tree->root = tree->insertNode(par, tree->root);
	if (this->left != nullptr) this->left->tree->root = this->left->tree->insertNode(par, this->left->tree->root);
	if (this->right != nullptr) this->right->tree->root = this->right->tree->insertNode(par, this->right->tree->root);
}

ConcatStringTree::~ConcatStringTree() {
	if(root != nullptr) root->des(root);
	this->root = nullptr;
	sz = 0;
}
void tNode::des(tNode* par) {
	if (par == nullptr) return;
	tree->root = tree->delNode(tree->root, par);
	if (tree->size() <= 0)
	{
		if (this->left != nullptr) 
		{
			this->left->des(this);
		}
		if (this->right != nullptr) 
		{
			this->right->des(this);
		}
		delete this;
	}
}

ConcatStringTree::ConcatStringTree(const char* s) {
	tNode* root = new tNode();
	string sn = "";
	int i = 0;
	while (s[i] != '\0')
	{
		sn += s[i];
		i++;
	}
	stNode* node = new stNode(sn);
	root->data = node;
	root->leftL = 0;
	root->left = nullptr;
	root->right = nullptr;
	root->length1 = sn.length();
	this->root = root;
	sz = sn.length();
	root->checkPar(root);
}

ConcatStringTree::ConcatStringTree(tNode* root) {
	this->root = root;
	sz = root->length1;
	root->checkPar(root);
}

int ConcatStringTree::length() const {
	return this->sz;
}
char ConcatStringTree::get(int index) const {
	if (index < 0 || index >=  length()) throw out_of_range("Index of string is invalid!");
	tNode* cur = root;
	while (cur->data == nullptr)
	{
		if (index < cur->leftL)
		{
			cur = cur->left;
		}
		else
		{
			index -= cur->leftL;
			cur = cur->right;
		}
	}
	return cur->data->s[index];
}

void dfs(tNode *root, string &sn)
{
	if (root == nullptr) return;
	else
	{
		if (root->left == nullptr && root->right == nullptr)
		{
			sn += root->data->s;
		}
		dfs(root->left, sn);
		dfs(root->right, sn);
	}
}

int ConcatStringTree::indexOf(char c) const {
	string sn = "";
	dfs(root, sn);
	for (int i = 0; i < sn.length(); i++)
	{
		if (sn[i] == c)
		{
			return i;
		}
	}
	return -1;
}
void dfs1(tNode *root, string& sn1)
{
	if (root == nullptr) return;
	else
	{
		if (root->left == nullptr && root->right == nullptr)
		{
			sn1 += "(LL=";
			sn1 += to_string(root->leftL);
			sn1 += ",L=";
			sn1 += to_string(root->length1);
			sn1 += ",";
			if (root->data != nullptr)
			{
				sn1 += "\"";
				sn1 += root->data->s;
				sn1 += "\"";
			}
			else
			{
				sn1 += "<NULL>";
			}
			sn1 += ")";
		}
		else
		{
			sn1 += "(LL=";
			sn1 += to_string(root->leftL);
			sn1 += ",L=";
			sn1 += to_string(root->length1);
			sn1 += ",";
			sn1 += "<NULL>";
			sn1 += ")";
			if (root->left != nullptr)
			{
				sn1 += ";";
				dfs1(root->left, sn1);
			}
			if (root->right != nullptr)
			{
				sn1 += ";";
				dfs1(root->right, sn1);
			}
		}
	}
}
string ConcatStringTree::toStringPreOrder() const {
	string sn1 = "ConcatStringTree[";
	dfs1(root, sn1);
	sn1 += "]";
	return sn1;
}
string ConcatStringTree::toString() const {
	string s2 = "";
	s2 += "ConcatStringTree[\"";
	dfs(root, s2);
	s2 += "\"]";
	return s2;
}
ConcatStringTree ConcatStringTree::concat(const ConcatStringTree& otherS) const {
	tNode* fN = new tNode();
	fN->left = this->root;	
	fN->right = otherS.root;
	fN->leftL = root->length1;
	fN->length1 = root->length1 + otherS.root->length1;
	return ConcatStringTree(fN);
}
tNode* substr1(tNode* root, int from, int to) {
	if (root == nullptr) return nullptr;
	tNode* cur = new tNode();
	if (root->left == nullptr && root->right == nullptr)
	{
		stNode* node = new stNode(root->data->s.substr(from, to - from));
		cur->data = node;
		cur->length1 = cur->data->s.length();
		cur->leftL = 0;
		cur->left = nullptr;
		cur->right = nullptr;
	}
	else if (from >= root->leftL)
	{
		cur->left = nullptr;
		cur->leftL = 0;
		cur->right = substr1(root->right, from - root->leftL, to - root->leftL);
		cur->length1 = cur->right->length1;
		cur->data = nullptr;
	}
	else if (to <= root->leftL)
	{
		cur->data = nullptr;
		cur->right = nullptr;
		cur->left = substr1(root->left, from, to);
		cur->leftL = cur->left->length1;
		cur->length1 = cur->leftL;
	}
	else {
		cur->data = nullptr;
		cur->left = substr1(root->left, from, root->leftL);
		cur->right = substr1(root->right, 0, to - root->leftL);
		cur->leftL = cur->left->length1;
		cur->length1 = cur->left->length1 + cur->right->length1;
	}
	return cur;
}

ConcatStringTree ConcatStringTree::subString(int from, int to) const {
	tNode* cur = substr1(root, from, to);
	return ConcatStringTree(cur);
}

tNode* newdfs(tNode* root) {
	if (root == nullptr) return nullptr;
	tNode* cur = new tNode();
	if (root->data != nullptr) 
	{
		stNode* node = new stNode(string(root->data->s.rbegin(), root->data->s.rend()));
		cur->data = node;
		cur->length1 = cur->data->s.length();
		return cur;
	}
	cur->left = newdfs(root->right);
	cur->right = newdfs(root->left);
	cur->leftL = cur->left->length1;
	cur->length1 = cur->left->length1 + cur->right->length1;
	return cur;
}
ConcatStringTree ConcatStringTree::reverse() const {
	tNode *cur = newdfs(root);
	return ConcatStringTree(cur);
}

int ParentsTree::size() const {
	return this->szNode;
}

string dfs2(ParentsTree::TreeAVL* root)
{
	if (root == nullptr) return "";
	string s1 = "(id=";
	s1 += to_string(root->data->id);
	s1 += ")";
	if (root->left != nullptr)
	{
		s1 += ";";
		s1 += dfs2(root->left);
	}
	if (root->right != nullptr)
	{
		s1 += ";";
		s1 += dfs2(root->right);
	}
	return s1;
}
string ParentsTree::toStringPreOrder() const {
	string s1 = "ParentsTree[";
	s1 += dfs2(root);
	s1 += "]";
	return s1;
}

int ConcatStringTree::getParTreeSize(const string& query) const {
	for (auto x : query)
	{
		if (x != 'l' && x != 'r')
		{
			throw runtime_error("Invalid character of query");
		}
	}
	tNode* cur = this->root;
	for (auto x : query)
	{
		if (x == 'l')
		{
			cur = cur->left;
			if(cur == nullptr) throw runtime_error("Invalid query: reaching NULL");
		}
		else
		{
			cur = cur->right;
			if (cur == nullptr) throw runtime_error("Invalid query: reaching NULL");
		}
	}
	return cur->tree->size();
}

string ConcatStringTree::getParTreeStringPreOrder(const string& query) const {
	for (auto x : query)
	{
		if (x != 'l' && x != 'r')
		{
			throw runtime_error("Invalid character of query");
		}
	}
	tNode* cur = this->root;
	for (auto x : query)
	{
		if (x == 'l')
		{
			cur = cur->left;
			if (cur == nullptr) throw runtime_error("Invalid query: reaching NULL");
		}
		else
		{
			cur = cur->right;
			if (cur == nullptr) throw runtime_error("Invalid query: reaching NULL");
		}
	}
	return cur->tree->toStringPreOrder();
}

		//============================// Hashing //============================//

LitStringHash::LitStringHash(const HashConfig& hashConfig){
	z = hashConfig;
	maxsz = hashConfig.initSize;
	initsz = hashConfig.initSize;
	this->tail = -1;
	this->sz1 = 0;
	hash = new newhash[maxsz];
}
long long power(long long a, long long b) {
	long long res = 1;
	if (b == 0) return 1;
	for (int i = 1; i <= b; i++) {
		res *= a;
	}
	return res;
}

long long LitStringHash::hashfunction(string s1)
{
	long long sum1 = 0;
	long long p = 1;
	for (int i = 0; i < s1.length(); i++)
	{
		sum1 += int(s1[i]) * p;
		p *= this->z.p;
	}
	return sum1%maxsz;
}

long long LitStringHash::hashtable(long long x, int i)
{
	long long sum2 = x;
	x += ((long long)(z.c1 * i + z.c2 * i * i)%maxsz);
	x %= maxsz;	
	return x;
}

int LitStringHash::getLastInsertedIndex() const {
	return this->tail;
}


string LitStringHash::toString() const {
	string sn3 = "LitStringHash[";
	for (int i = 0; i < maxsz; i++)
	{
		if (hash[i].hashing == nullptr)
		{
			sn3 += "()";
		}
		else
		{
			sn3 += "(litS=\"" + hash[i].hashing->s + "\")";
		}
		if (i != maxsz - 1)
		{
			sn3 += ";";
		}
	}
	sn3 += "]";
	return sn3;
}

void LitStringHash::rehashing() {
	double loadsz = double(sz1 / maxsz);
	if (loadsz > z.lambda)
	{
		long long oldsz = maxsz;
		maxsz = floor(z.alpha * maxsz);
		newhash *hash1 = new newhash[maxsz];
		for (int i = 0; i < oldsz; i++)
		{
			if (hash[i].hashing != nullptr) 
			{
				long long idx = hashfunction(hash[i].hashing->s);
				bool flag = false;
				for (int j = 0; j < maxsz; j++)
				{
					long long newindex = hashtable(idx, j);
					if (hash1[newindex].hashing == nullptr)
					{
						hash1[newindex].hashing = hash[i].hashing;
						hash1[newindex].cnt = hash[i].cnt;
						tail = newindex;
						flag = true;
						break;
					}
				}
				if (flag == false)
				{
					throw runtime_error("No possible slot");
				}
			}
		}
		delete []hash;
		hash = hash1;
	}
}

void LitStringHash::inserthash(stNode* cur, int idx)
{
	if (hash == nullptr)
	{
		hash = new newhash[initsz];
	}
	bool flag1 = false;
	for (int i = 0; i < maxsz; i++)
	{
		long long newidx = hashtable(idx, i);
		if (hash[newidx].hashing == nullptr)
		{
			hash[newidx].hashing = cur;
			hash[newidx].cnt = 1;
			tail = newidx;
			sz1 += 1;
			flag1 = true;
			break;
		}
	}
	if (flag1 == false)
	{
		throw runtime_error("No possible slot");
	}
	rehashing();
}


