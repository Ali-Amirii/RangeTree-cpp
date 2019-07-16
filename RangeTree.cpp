#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

typedef pair < double, double > point;
#define X first
#define Y second

point a[10001], tmp[10001];

class Node
{
public:
	Node *left, *right, *yTree;
	int l, r;
	double val, maxChild, minChild;
	vector < point > p;
	Node(double v = 0., int ll = 0, int rr = 0)
	{
		val = v;
		l = ll;
		r = rr;
		maxChild = minChild = v;
		left = right = NULL;
	}
};

bool CompareByX(point x, point y) {
	return x.X < y.X;
}
bool CompareByY(point x, point y) {
	return x.Y < y.Y;
}

class RangeTree
{
	Node *root;
public:
	RangeTree()
	{
		root = NULL;
	}
	Node *build(point a[], int l, int r, bool X)
	{
		queue < Node* > q;
		for (int i = l; i < r; i++)
		{
			if (X)
				q.push(new Node(a[i].X, i, i + 1));
			else
			{
				Node *par = new Node(a[i].Y, i, i + 1);
				par->p.push_back(a[i]);
				q.push(par);
			}
		}
		while (q.size() > 1)
		{
			Node *x = q.front(); q.pop();
			if (x->val <= q.front()->val)
			{
				Node *y = q.front(); q.pop();
				Node *par = new Node(x->maxChild, x->l, y->r);
				par->left = x;
				par->right = y;
				par->maxChild = y->maxChild;
				par->minChild = x->minChild;
				if (X == false)
				{
					for (int i = 0; i < x->p.size(); i++)
						par->p.push_back(x->p[i]);
					for (int i = 0; i < y->p.size(); i++)
						par->p.push_back(y->p[i]);
				}
				q.push(par);
			}
			else
				q.push(x);
		}
		return q.front();
	}
	void buildX(point a[], int n)
	{
		sort(a, a + n, CompareByX);
		root = build(a, 0, n, true);
		traverseAndBuildY(root, a);
	}
	void traverseAndBuildY(Node *root, point a[])
	{
		if (root == NULL)
			return;
		int l = root->l, r = root->r;
		for (int i = l; i < r; i++)
			tmp[i] = a[i];
		sort(tmp + l, tmp + r, CompareByY);
		root->yTree = build(tmp, l, r, false);
		traverseAndBuildY(root->left, a);
		traverseAndBuildY(root->right, a);
	}
	
	vector < point > search(double x1, double y1, double x2, double y2)
	{
		return searchX(root, x1, x2, y1, y2);
	}
	vector < point > searchX(Node *root, double x1, double x2, double y1, double y2)
	{
		vector < point > result;
		if (root == NULL)
			return result;
		if (x1 <= root->minChild && root->maxChild <= x2)
			return searchY(root->yTree, y1, y2);
		vector < point > r1, r2;
		r1 = searchX(root->left, x1, x2, y1, y2);
		r2 = searchX(root->right, x1, x2, y1, y2);
		for (int i = 0; i < r1.size(); i++)
			result.push_back(r1[i]);
		for (int i = 0; i < r2.size(); i++)
			result.push_back(r2[i]);
		return result;
	}
	vector < point > searchY(Node *root, double y1, double y2)
	{
		vector < point > result;
		if (root == NULL)
			return result;
		if (y1 <= root->minChild && root->maxChild <= y2)
		{
			for (int i = 0; i < root->p.size(); i++)
				result.push_back(root->p[i]);
			return result;
		}
		vector < point > r1, r2;
		r1 = searchY(root->left, y1, y2);
		r2 = searchY(root->right, y1, y2);
		for (int i = 0; i < r1.size(); i++)
			result.push_back(r1[i]);
		for (int i = 0; i < r2.size(); i++)
			result.push_back(r2[i]);
		return result;
	}
};

int main()
{
	int n, m;
	double x1, x2, y1, y2;
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> a[i].X;
	for (int i = 0; i < n; i++)
		cin >> a[i].Y;
	RangeTree range;
	range.buildX(a, n);
	cin >> m;
	for (int i = 0; i < m; i++)
	{
		cin >> x1 >> y1 >> x2 >> y2;
		vector < point > result = range.search(x1, y1, x2, y2);
		if (result.size() == 0)
			cout << "None" << endl;
		else
		{
			for (int i = 0; i < result.size(); i++)
				cout << result[i].X << " ";
			cout << endl;
			for (int i = 0; i < result.size(); i++)
				cout << result[i].Y << " ";
			cout << endl;
		}
	}
	return 0;
}
