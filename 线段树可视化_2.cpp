#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>
#include <cstdio>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS  // 禁用安全警告

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;
const int NODE_RADIUS = 30;
const int LEVEL_SPACING = 100;
const int NODE_SPACING = 70;
const int ANIMATION_SPEED = 500;  // 降低动画速度，便于观察

// 线段树节点结构
struct SegmentTreeNode {
	int id;
	int l, r;
	long long sum;
	long long tag;
	int x, y;
	bool visited;
	bool covered;
	
	SegmentTreeNode() : id(0), l(0), r(0), sum(0), tag(0), x(0), y(0),
	visited(false), covered(false) {}
	
	bool isValid() const {
		return id != 0;
	}
};

class SegmentTreeVisualizer {
private:
	int n;
	vector<long long> arr;
	vector<SegmentTreeNode> tree;
	vector<vector<int>> treeIndex;
	int maxDepth;
	
	// 获取整数输入
	int getIntegerInput(int x, int y, const char* prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
		cleardevice();
		
		// 显示提示信息
		settextcolor(BLACK);
		settextstyle(20, 0, _T("宋体"));
		char promptText[100];
		sprintf_s(promptText, "%s (范围: %d ~ %d):", prompt, minVal, maxVal);
		outtextxy(x, y, promptText);
		
		// 创建输入框
		rectangle(x + 400, y - 5, x + 600, y + 25);
		
		// 处理输入
		char input[20] = "";
		int pos = 0;
		
		while (true) {
			char ch = _getch();
			if (ch == '\r' || ch == '\n') {
				if (pos > 0) {
					int value = atoi(input);
					if (value >= minVal && value <= maxVal) {
						return value;
					}
					else {
						// 显示错误信息
						settextcolor(RED);
						outtextxy(x, y + 40, _T("输入值超出范围，请重新输入!"));
						Sleep(1000);
						setfillcolor(WHITE);
						fillrectangle(x, y + 40, x + 400, y + 80);
						// 清空输入框
						setfillcolor(WHITE);
						fillrectangle(x + 401, y - 4, x + 599, y + 24);
						pos = 0;
						input[0] = '\0';
						continue;
					}
				}
			}
			else if (ch == '\b' && pos > 0) {
				input[--pos] = '\0';
			}
			else if (ch >= '0' && ch <= '9' || (ch == '-' && pos == 0)) {
				if (pos < 10) {
					input[pos++] = ch;
					input[pos] = '\0';
				}
			}
			
			// 重新绘制输入框
			setfillcolor(WHITE);
			fillrectangle(x + 401, y - 4, x + 599, y + 24);
			settextcolor(BLACK);
			outtextxy(x + 405, y, input);
		}
	}
	
	// 获取长整数输入
	long long getLongLongInput(int x, int y, const char* prompt) {
		cleardevice();
		
		// 显示提示信息
		settextcolor(BLACK);
		settextstyle(20, 0, _T("宋体"));
		outtextxy(x, y, prompt);
		
		// 创建输入框
		rectangle(x + 400, y - 5, x + 600, y + 25);
		
		// 处理输入
		char input[30] = "";
		int pos = 0;
		
		while (true) {
			char ch = _getch();
			if (ch == '\r' || ch == '\n') {
				if (pos > 0) {
					return atoll(input);
				}
			}
			else if (ch == '\b' && pos > 0) {
				input[--pos] = '\0';
			}
			else if (ch >= '0' && ch <= '9' || (ch == '-' && pos == 0)) {
				if (pos < 20) {
					input[pos++] = ch;
					input[pos] = '\0';
				}
			}
			
			// 重新绘制输入框
			setfillcolor(WHITE);
			fillrectangle(x + 401, y - 4, x + 599, y + 24);
			settextcolor(BLACK);
			outtextxy(x + 405, y, input);
		}
	}
	
	// 懒标记下传
	void pushDown(int p, int l, int r) {
		if (tree[p].tag != 0) {
			int mid = (l + r) / 2;
			
			// 标记子节点为访问状态以显示动画
			if (p * 2 < (int)tree.size() && tree[p * 2].isValid()) {
				tree[p * 2].visited = true;
				tree[p * 2].sum += (mid - l + 1) * tree[p].tag;
				tree[p * 2].tag += tree[p].tag;
			}
			
			if (p * 2 + 1 < (int)tree.size() && tree[p * 2 + 1].isValid()) {
				tree[p * 2 + 1].visited = true;
				tree[p * 2 + 1].sum += (r - mid) * tree[p].tag;
				tree[p * 2 + 1].tag += tree[p].tag;
			}
			
			tree[p].tag = 0;
		}
	}
	
public:
	SegmentTreeVisualizer() : n(0), maxDepth(0) {
		initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
		setbkcolor(WHITE);
		cleardevice();
	}
	
	~SegmentTreeVisualizer() {
		closegraph();
	}
	
	// 初始化数组
	void initArray() {
		cleardevice();
		
		// 获取数组大小
		n = getIntegerInput(50, 50, "请输入数组元素个数 n", 1, 20);
		
		arr.resize(n + 1);
		
		// 输入数组元素
		cleardevice();
		settextcolor(BLACK);
		settextstyle(20, 0, _T("宋体"));
		outtextxy(50, 50, _T("请输入数组元素:"));
		
		for (int i = 1; i <= n; i++) {
			char prompt[50];
			sprintf_s(prompt, "a[%d] =", i);
			arr[i] = getLongLongInput(50, 100 + i * 40, prompt);
			
			// 显示当前已输入的元素
			cleardevice();
			outtextxy(50, 50, _T("请输入数组元素:"));
			for (int j = 1; j <= i; j++) {
				char element[50];
				sprintf_s(element, "a[%d] = %lld", j, arr[j]);
				outtextxy(50, 100 + j * 40, element);
			}
		}
	}
	
	// 计算节点位置
	void calculateNodePositions() {
		treeIndex.clear();
		
		// 计算树深度
		maxDepth = 0;
		int temp = 1;
		while (temp < n) {
			temp <<= 1;
			maxDepth++;
		}
		maxDepth++;
		
		// 为每一层分配空间
		treeIndex.resize(maxDepth);
		
		// 分配节点位置
		queue<pair<int, pair<int, int>>> q;
		q.push({ 1, {100, SCREEN_WIDTH - 100} });
		
		int depth = 0;
		while (!q.empty()) {
			int levelSize = (int)q.size();
			treeIndex[depth].resize(levelSize);
			
			for (int i = 0; i < levelSize; i++) {
				auto current = q.front();
				q.pop();
				
				int nodeId = current.first;
				int leftBound = current.second.first;
				int rightBound = current.second.second;
				
				tree[nodeId].x = (leftBound + rightBound) / 2;
				tree[nodeId].y = 100 + depth * LEVEL_SPACING;
				treeIndex[depth][i] = nodeId;
				
				if (nodeId * 2 < (int)tree.size() && tree[nodeId * 2].id != 0) {
					int mid = (leftBound + rightBound) / 2;
					q.push({ nodeId * 2, {leftBound, mid} });
					q.push({ nodeId * 2 + 1, {mid, rightBound} });
				}
			}
			depth++;
		}
	}
	
	// 建树
	void buildTree() {
		cleardevice();
		
		tree.resize(4 * n + 5);
		build(1, 1, n);
		
		calculateNodePositions();
		drawTree();
		
		settextcolor(RED);
		settextstyle(25, 0, _T("宋体"));
		outtextxy(SCREEN_WIDTH / 2 - 150, 30, _T("线段树构建完成!"));
		settextcolor(BLACK);
		settextstyle(20, 0, _T("宋体"));
		outtextxy(50, SCREEN_HEIGHT - 100, _T("按任意键继续..."));
		_getch();
	}
	
	// 递归建树
	void build(int p, int l, int r) {
		tree[p].id = p;
		tree[p].l = l;
		tree[p].r = r;
		tree[p].tag = 0;
		
		if (l == r) {
			tree[p].sum = arr[l];
		}
		else {
			int mid = (l + r) / 2;
			build(p * 2, l, mid);
			build(p * 2 + 1, mid + 1, r);
			tree[p].sum = tree[p * 2].sum + tree[p * 2 + 1].sum;
		}
	}
	
	// 绘制单个节点
	void drawNode(int nodeId, COLORREF fillColor, COLORREF textColor) {
		if (nodeId >= (int)tree.size() || !tree[nodeId].isValid()) return;
		
		const SegmentTreeNode& node = tree[nodeId];
		
		setfillcolor(fillColor);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 2);
		fillcircle(node.x, node.y, NODE_RADIUS);
		circle(node.x, node.y, NODE_RADIUS);
		
		settextcolor(textColor);
		settextstyle(14, 0, _T("宋体"));
		char idText[20];
		sprintf_s(idText, "%d", nodeId);
		outtextxy(node.x - 8, node.y - 35, idText);
		
		sprintf_s(idText, "[%d,%d]", node.l, node.r);
		outtextxy(node.x - 15, node.y - 15, idText);
		
		settextstyle(16, 0, _T("宋体"));
		char sumText[30];
		sprintf_s(sumText, "%lld", node.sum);
		int textWidth = (int)textwidth(sumText) / 2;
		outtextxy(node.x - textWidth, node.y + 5, sumText);
		
		if (node.tag != 0) {
			settextcolor(RED);
			settextstyle(12, 0, _T("宋体"));
			char tagText[30];
			sprintf_s(tagText, "tag=%lld", node.tag);
			outtextxy(node.x - 20, node.y + 35, tagText);
		}
		else {
			settextcolor(DARKGRAY);
			settextstyle(12, 0, _T("宋体"));
			outtextxy(node.x - 20, node.y + 35, "tag=0");
		}
	}
	
	// 绘制树结构
	void drawTree() {
		cleardevice();
		
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 2);
		
		for (int i = 1; i < (int)tree.size(); i++) {
			if (!tree[i].isValid()) continue;
			
			if (i * 2 < (int)tree.size() && tree[i * 2].isValid()) {
				line(tree[i].x, tree[i].y + NODE_RADIUS,
					tree[i * 2].x, tree[i * 2].y - NODE_RADIUS);
			}
			
			if (i * 2 + 1 < (int)tree.size() && tree[i * 2 + 1].isValid()) {
				line(tree[i].x, tree[i].y + NODE_RADIUS,
					tree[i * 2 + 1].x, tree[i * 2 + 1].y - NODE_RADIUS);
			}
		}
		
		for (int i = 1; i < (int)tree.size(); i++) {
			if (!tree[i].isValid()) continue;
			
			COLORREF fillColor = tree[i].visited ? LIGHTGREEN :
			(tree[i].covered ? LIGHTBLUE : WHITE);
			COLORREF textColor = tree[i].visited ? RED :
			(tree[i].covered ? BLUE : BLACK);
			
			drawNode(i, fillColor, textColor);
		}
		
		settextcolor(BLUE);
		settextstyle(25, 0, _T("宋体"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 20, _T("线段树可视化"));
		
		settextcolor(BLACK);
		settextstyle(16, 0, _T("宋体"));
		outtextxy(50, SCREEN_HEIGHT - 80, _T("图例:"));
		
		setfillcolor(LIGHTGREEN);
		fillrectangle(120, SCREEN_HEIGHT - 85, 140, SCREEN_HEIGHT - 65);
		outtextxy(150, SCREEN_HEIGHT - 85, _T("访问过的节点"));
		
		setfillcolor(LIGHTBLUE);
		fillrectangle(300, SCREEN_HEIGHT - 85, 320, SCREEN_HEIGHT - 65);
		outtextxy(330, SCREEN_HEIGHT - 85, _T("完全覆盖的节点"));
		
		setfillcolor(WHITE);
		fillrectangle(500, SCREEN_HEIGHT - 85, 520, SCREEN_HEIGHT - 65);
		setlinecolor(BLACK);
		rectangle(500, SCREEN_HEIGHT - 85, 520, SCREEN_HEIGHT - 65);
		outtextxy(530, SCREEN_HEIGHT - 85, _T("未访问的节点"));
	}
	
	// 重置所有节点的访问状态
	void resetVisited() {
		for (auto& node : tree) {
			node.visited = false;
			node.covered = false;
		}
	}
	
	// 延迟函数
	void delayAndDraw() {
		drawTree();
		Sleep(ANIMATION_SPEED);
	}
	
	// 区间修改动画
	void updateAnimation() {
		if (n == 0) return;
		
		resetVisited();
		
		// 获取用户输入
		int L = getIntegerInput(50, 50, "请输入修改区间左端点 L", 1, n);
		int R = getIntegerInput(50, 120, "请输入修改区间右端点 R", L, n);
		long long d = getLongLongInput(50, 190, "请输入要增加的值 d");
		
		// 显示操作信息
		settextcolor(RED);
		settextstyle(20, 0, _T("宋体"));
		char opText[100];
		sprintf_s(opText, "区间修改: [%d, %d] 每个元素增加 %lld", L, R, d);
		outtextxy(50, SCREEN_HEIGHT - 50, opText);
		
		// 执行修改动画
		update(L, R, 1, 1, n, d);
		
		// 完成动画
		delayAndDraw();
		
		// 清除操作信息
		setfillcolor(WHITE);
		fillrectangle(0, SCREEN_HEIGHT - 60, SCREEN_WIDTH, SCREEN_HEIGHT);
		
		// 显示完成信息
		settextcolor(GREEN);
		outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50, _T("修改完成!"));
		Sleep(1000);
		
		resetVisited();
		drawTree();
	}
	
	// 递归修改 - 修复版本
	void update(int L, int R, int p, int l, int r, long long d) {
		tree[p].visited = true;
		delayAndDraw();
		
		// 如果完全覆盖当前区间
		if (L <= l && R >= r) {
			tree[p].sum += (r - l + 1) * d;
			tree[p].tag += d;  // 累加懒标记，而不是覆盖
			tree[p].covered = true;
			delayAndDraw();
			return;
		}
		
		tree[p].covered = true;
		delayAndDraw();
		
		// 下传懒标记
		pushDown(p, l, r);
		delayAndDraw();
		
		int mid = (l + r) / 2;
		
		// 递归更新子区间
		if (L <= mid) {
			update(L, R, p * 2, l, mid, d);
		}
		if (R > mid) {
			update(L, R, p * 2 + 1, mid + 1, r, d);
		}
		
		// 更新当前节点的sum值
		if (p * 2 < (int)tree.size() && tree[p * 2].isValid() && 
			p * 2 + 1 < (int)tree.size() && tree[p * 2 + 1].isValid()) {
			tree[p].sum = tree[p * 2].sum + tree[p * 2 + 1].sum;
		}
		
		tree[p].visited = true;
		delayAndDraw();
	}
	
	// 区间查询动画
	void queryAnimation() {
		if (n == 0) return;
		
		resetVisited();
		
		// 获取用户输入
		int L = getIntegerInput(50, 50, "请输入查询区间左端点 L", 1, n);
		int R = getIntegerInput(50, 120, "请输入查询区间右端点 R", L, n);
		
		// 显示操作信息
		settextcolor(RED);
		settextstyle(20, 0, _T("宋体"));
		char opText[100];
		sprintf_s(opText, "区间查询: [%d, %d] 的元素和", L, R);
		outtextxy(50, SCREEN_HEIGHT - 50, opText);
		
		// 执行查询动画
		long long result = query(L, R, 1, 1, n);
		
		// 显示结果
		char resultText[100];
		sprintf_s(resultText, "查询结果: %lld", result);
		outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 80, resultText);
		
		delayAndDraw();
		
		// 显示完成信息
		settextcolor(GREEN);
		outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50, _T("查询完成!"));
		Sleep(1500);
		
		resetVisited();
		drawTree();
	}
	
	// 递归查询 - 修复版本
	long long query(int L, int R, int p, int l, int r) {
		tree[p].visited = true;
		delayAndDraw();
		
		if (L <= l && R >= r) {
			tree[p].covered = true;
			delayAndDraw();
			return tree[p].sum;
		}
		
		tree[p].covered = true;
		delayAndDraw();
		
		// 查询前也需要下传懒标记
		pushDown(p, l, r);
		delayAndDraw();
		
		int mid = (l + r) / 2;
		long long ans = 0;
		
		if (L <= mid) {
			ans += query(L, R, p * 2, l, mid);
		}
		if (R > mid) {
			ans += query(L, R, p * 2 + 1, mid + 1, r);
		}
		
		tree[p].visited = true;
		delayAndDraw();
		
		return ans;
	}
	
	// 显示主菜单
	void showMenu() {
		cleardevice();
		
		settextcolor(BLUE);
		settextstyle(30, 0, _T("宋体"));
		outtextxy(SCREEN_WIDTH / 2 - 150, 50, _T("线段树算法可视化"));
		
		settextcolor(BLACK);
		settextstyle(20, 0, _T("宋体"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 150, _T("1. 重新输入数组"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 200, _T("2. 区间修改"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 250, _T("3. 区间查询"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 300, _T("4. 显示当前树"));
		outtextxy(SCREEN_WIDTH / 2 - 100, 350, _T("5. 退出"));
		
		// 显示当前数组
		if (n > 0) {
			settextcolor(DARKGRAY);
			outtextxy(50, 450, _T("当前数组:"));
			
			char arrText[200] = "a[] = {";
			char temp[20];
			
			for (int i = 1; i <= min(n, 10); i++) {
				sprintf_s(temp, "%lld", arr[i]);
				strcat_s(arrText, temp);
				if (i < n && i < 10) strcat_s(arrText, ", ");
			}
			
			if (n > 10) strcat_s(arrText, ", ...}");
			else strcat_s(arrText, "}");
			
			outtextxy(50, 480, arrText);
		}
	}
	
	// 运行主循环
	void run() {
		bool firstTime = true;
		
		while (true) {
			if (firstTime) {
				initArray();
				buildTree();
				firstTime = false;
			}
			
			showMenu();
			
			char choice = _getch();
			
			switch (choice) {
				case '1': {
					initArray();
					buildTree();
					break;
				}
				
				case '2': {
					if (n == 0) {
						MessageBox(GetHWnd(), _T("请先输入数组!"), _T("提示"), MB_OK);
						break;
					}
					updateAnimation();
					break;
				}
				
				case '3': {
					if (n == 0) {
						MessageBox(GetHWnd(), _T("请先输入数组!"), _T("提示"), MB_OK);
						break;
					}
					queryAnimation();
					break;
				}
				
				case '4': {
					if (n == 0) {
						MessageBox(GetHWnd(), _T("请先输入数组!"), _T("提示"), MB_OK);
						break;
					}
					resetVisited();
					drawTree();
					outtextxy(50, SCREEN_HEIGHT - 100, _T("按任意键返回菜单..."));
					_getch();
					break;
				}
				
				case '5': {
					return;
				}
				
				default: {
					break;
				}
			}
		}
	}
};

// 主函数
int main() {
	system("chcp 65001 > nul");
	
	SegmentTreeVisualizer visualizer;
	visualizer.run();
	
	return 0;
}
