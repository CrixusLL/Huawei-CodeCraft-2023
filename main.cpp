#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <cstdio>
#include <cstring>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>
#define INFINITY 65535 /* ��ʾȨֵ������*/
int channelAmount;     // ͨ������
int maxDistance;       // ���˥������
// const int n = 5000;  //��󶥵���
// const int m = 10000;  //������
const int inf = 2e9;

class Vertex
{ // ����
public:
    explicit Vertex(int vexNum)
    {
        this->vexNum = vexNum;
    }
    int vexNum;
    // vector<int> signalAmplifier;//�źŷŴ�������Ӧ��ֵΪ�Ŵ����Ŵ��ҵ����
};
class Edge
{ // ��
public:
    Edge(int edgeNum, int vex1, int vex2, int edgeWeight)
    {
        this->edgeNum = edgeNum;
        this->vex1 = vex1;
        this->vex2 = vex2;
        this->edgeWeight = edgeWeight;
        this->channel.assign(channelAmount, true);
        this->usableChannelAmounnt = channelAmount;
    }
    bool operator==(const Edge &e) const
    {
        if (this->edgeNum == e.edgeNum)
            return true;
        else
            return false;
    }
    int vex1; // ��¼�ߵ���������
    int vex2;
    int edgeNum;              // �ߵı��
    int edgeWeight;           // �ߵ�Ȩֵ
    vector<bool> channel;     // ͨ����ֵΪtrue��ʾͨ�����ã�Ϊfalse��ʾͨ��������
    int usableChannelAmounnt; // ����ͨ������
};
class Edges
{
public:
    Edges()
    {
        this->min = maxDistance + 1;
    }
    int min;            // ��Сֵ�����������ӱߵ�ʱ��ֱ�Ӹ�ֵΪ��Сֵ
    vector<Edge> edges; // ���Դ洢��������Ķ�����
};
class EdgePath
{ // ��·����������һ���࣬����ӱ�
public:
    EdgePath()
    {
        // this->pathLength=0;
    }
    // int pathLength;//·������
    vector<int> edgeNum;                                         // ��¼������·����Ӧ�ı��
    map<int, pair<int, vector<int>>> needAddQuantityAndLocation; // ��ȡ��Ҫ�ӱߵ������Լ���Ҫ�ӱߵĵط��Լ���Ӧͨ���ı��
    vector<int> vexPath;                                         // ÿ����·�����ж�Ӧ�ĵ�·��
};
class Service
{ // ҵ��
public:
    Service(int serviceNum, int start, int end)
    {
        this->serviceNum = serviceNum;
        this->start = start;
        this->end = end;
        this->passEdgeAmount = 0;
        this->passAmplifierAmount = 0;
        this->channelNum = -1;
    }
    int serviceNum;                 // ҵ����
    int start;                      // ҵ�������
    int end;                        // ҵ���յ���
    int channelNum;                 // ��ʹ�õ�ͨ�����
    int passEdgeAmount;             // �����ıߵ�����
    int passAmplifierAmount;        // �����ķŴ���������
    EdgePath passEdgeNum;           // �����ıߵı��
    vector<int> passAmplifierNum;   // �����ķŴ������ڵ�ı��
    vector<EdgePath> allPathEdge;   // ��ҵ����㵽�յ�����б�·��
    vector<vector<int>> allPathVex; // ��ҵ����㵽�յ�����е�·��
};
class AStar
{
public:
    AStar(int N, int M)
    {
        n = N;
        m = 0;
        cur = -1;
        h.resize(n, -1);
        // nxt.resize(m, -1);
        // p.resize(m, -1);
        // w.resize(m, 0);
        //tf.resize(n, false);
        //vis.resize(m, false);
        //path_vex.resize(n, -1);
        //path_edge.resize(n, -1);
    }

    struct node
    {
        int x, v;

        bool operator<(node a) const { return v > a.v; }
    };
    void add_edge(int x, int y, double z)
    {
        cur++;
        nxt.push_back(h[x]);
        h[x] = cur;
        p.push_back(y);
        w.push_back(z);
        m++;
    }
    void overallAddEdge(int u, int v, double ww)
    {
        add_edge(u, v, ww);
        add_edge(v, u, ww); // ����ͼ��������
    }
    void findAllPaths(int s, int t, vector<EdgePath> &allPathEdge)
    {
        priority_queue<node> Q;
        node x;
        vector<int> path_vex(this->n, -1);
        vector<int> path_edge(this->n, -1);
        vector<int> H(this->n, inf);
        vector<bool> tf(this->n, false);
        vector<bool> vis(this->m, false);
        Q.push({t, 0});
        while (!Q.empty())
        {
            x = Q.top();
            Q.pop();
            if (tf[x.x])
                continue;
            tf[x.x] = true;
            H[x.x] = x.v;
            for (int j = h[x.x]; j >= 0; j = nxt[j])
            {
                if (!vis[j / 2]) // �ж�ĳ�������ĳ�����Ƿ��Ѿ������ʹ�
                {
                    vis[j / 2] = true;
                    Q.push({p[j], x.v + w[j]});
                    path_vex[p[j]] = x.x;
                    path_edge[p[j]] = j / 2;
                }
            }
        }
        EdgePath temp;
        for (int i = s; i != t; i = path_vex[i])
        {
            temp.edgeNum.push_back(path_edge[i]);
        }
        allPathEdge.push_back(temp);
    }
    void removeEdge(int edgeNum)
    {
        int u = p[edgeNum * 2];
        int v = p[edgeNum * 2 + 1];
        if (h[u] == edgeNum * 2 || h[u] == edgeNum * 2 + 1)
        {
            h[u] = nxt[h[u]];
        }
        else
        {
            int pre = h[u];
            while (nxt[pre] != edgeNum * 2 && nxt[pre] != edgeNum * 2 + 1)
            {
                pre = nxt[pre];
            }
            nxt[pre] = nxt[nxt[pre]];
        }
        // ����ͼɾ������
        if (h[v] == edgeNum * 2 || h[v] == edgeNum * 2 + 1)
        {
            h[v] = nxt[h[v]];
        }
        else
        {
            int pre = h[v];
            while (nxt[pre] != edgeNum * 2 && nxt[pre] != edgeNum * 2 + 1)
            {
                pre = nxt[pre];
            }
            nxt[pre] = nxt[nxt[pre]];
        }
        w[edgeNum * 2] = 0;
        w[edgeNum * 2 + 1] = 0;
        p[edgeNum * 2] = -1;
        p[edgeNum * 2 + 1] = -1;
        nxt[edgeNum * 2] = -1;
        nxt[edgeNum * 2 + 1] = -1;
        m--;
    }
    int n, m; // u,v�Ǳߵ������˵㣬w�Ǳߵ�Ȩֵ��s����㣬t���յ�
    // static vector<int> H;// ÿ�����㵽�յ�t�Ĺ��ۺ���ֵ
    // h���ڽӱ���ͷ��ŵ�һ���ߵı�ţ�nxt���ڽӱ���ÿ���ߵ���һ���߱�ţ�p���ڽӱ���ÿ���ߵ��յ㣬w�Ǳߵ�Ȩֵ
    int cur;
    vector<int> h;
    vector<int> nxt;
    vector<int> p;
    vector<int> w;
    //vector<bool> tf;  // ���ÿ�������Ƿ��Ѿ�����չ��
    //vector<bool> vis; // dij�ж�ĳ�������ĳ�����Ƿ��Ѿ������ʹ�
};
class OpticalNetWork
{ // ������
public:
    OpticalNetWork(int vexAmount, int P, int D)
    {
        channelAmount = P;
        maxDistance = D;
        this->vexAmount = vexAmount;
        this->edgeAmount = 0;
        this->serviceAmount = 0;
        Edges vEdge;
        this->edgeTable = vector<vector<Edges>>(vexAmount, vector<Edges>(vexAmount, vEdge));
        for (int i = 0; i < vexAmount; i++)
        {
            Vertex v(i);
            this->vexTable.push_back(v);
        }
    }
    void addEdge(int u, int v, int edgeWeight)
    { // ���ӱ�
        if (edgeWeight < this->edgeTable[u][v].min)
        {
            this->edgeTable[u][v].min = edgeWeight;
            this->edgeTable[v][u].min = edgeWeight;
        }
        this->edgeTable[u][v].edges.emplace_back(this->edgeAmount, u, v, edgeWeight); // ����ͼ��Ҫ�ھ��������ط�������
        this->edgeTable[v][u].edges.emplace_back(this->edgeAmount, u, v, edgeWeight);
        this->edgeArr.emplace_back(this->edgeAmount, u, v, edgeWeight);
        this->edgeAmount++;
    }
    void addService(int start, int end)
    { // ����ҵ��
        this->serviceTable.emplace_back(this->serviceAmount, start, end);
        this->serviceAmount++;
    }
    vector<EdgePath> calculateCombination(vector<vector<int>> &inputList, vector<int> vexPath)
    { // ��������Ԫ���������
        vector<int> combination;
        vector<EdgePath> newAllPath;
        int n = inputList.size();
        for (int i = 0; i < n; i++)
        {
            combination.push_back(0);
        }
        int i = 0;
        bool isContinue = false;
        do
        {
            EdgePath v;
            v.vexPath = vexPath;
            for (int j = 0; j < n; j++)
            {
                // cout << inputList[j][combination[j]] << " ";
                v.edgeNum.push_back(inputList[j][combination[j]]);
            }
            // cout << endl;
            newAllPath.push_back(v);
            i++;
            combination[n - 1] = i;
            for (int j = n - 1; j >= 0; j--)
            {
                if (combination[j] >= inputList[j].size())
                {
                    combination[j] = 0;
                    i = 0;
                    if (j - 1 >= 0)
                    {
                        combination[j - 1] = combination[j - 1] + 1;
                    }
                }
            }
            isContinue = false;
            for (int integer : combination)
            {
                if (integer != 0)
                {
                    isContinue = true;
                }
            }
        } while (isContinue);
        return newAllPath;
    }
    // �������������㵽�յ������·��(�ݹ鷽ʽ��,���Ÿ�Ϊ����ͬʱ�ҵ���·��
    void DFS(int start, int end, vector<int> &path, vector<bool> &visited, vector<vector<int>> &allPath)
    {
        visited[start] = true;
        path.push_back(start);
        if (start == end)
        {
            allPath.push_back(path);
        }
        else
        {
            for (int i = 0; i < this->vexAmount; i++)
            {
                if (this->edgeTable[start][i].edges.size() != 0 && !visited[i])
                {
                    DFS(i, end, path, visited, allPath);
                }
            }
        }
        path.pop_back();
        visited[start] = false;
    }
    void DFSNonRecursion(int start, int end, vector<vector<int>> &allPath)
    { // �������������㵽�յ������·��(�ǵݹ鷽ʽ��
        vector<bool> visited(this->vexAmount, false);
        stack<int> major;
        stack<vector<int>> assist;
        major.push(start);
        visited[major.top()] = true;
        vector<int> nearVex;
        for (int i = 0; i < this->vexAmount; i++)
        {
            if (this->edgeTable[start][i].edges.size() != 0 && !visited[i])
            {
                nearVex.push_back(i);
            }
        }
        assist.push(nearVex);
        nearVex.resize(0);
        while (!major.empty())
        {
            if (!assist.top().empty())
            {
                major.push(assist.top().front());
                auto iter = assist.top().begin();
                assist.top().erase(iter);
                visited[major.top()] = true;
                for (int i = 0; i < this->vexAmount; i++)
                {
                    if (this->edgeTable[major.top()][i].edges.size() != 0 && !visited[i])
                    {
                        nearVex.push_back(i);
                    }
                }
                assist.push(nearVex);
                nearVex.resize(0);
            }
            else
            {
                if (major.top() == end)
                {
                    stack<int> s;
                    vector<int> path;
                    while (!major.empty())
                    {
                        s.push(major.top());
                        major.pop();
                    }
                    while (!s.empty())
                    {
                        path.push_back(s.top());
                        major.push(s.top());
                        s.pop();
                    }
                    allPath.push_back(path);
                }
                visited[major.top()] = false;
                major.pop();
                assist.pop();
            }
        }
    }
    void findAllPath(Service &lightSer)
    { // �ҵ���ҵ����㵽�յ������е�·��
        lightSer.allPathEdge.resize(0);
        /*lightSer.allPathVex.resize(0);
        vector<int> path;
        vector<bool> visited(this->vexAmount, false);
        vector<vector<int>> allPath;
        DFS(lightSer.start, lightSer.end, path, visited, lightSer.allPathVex);*/
        DFSNonRecursion(lightSer.start, lightSer.end, lightSer.allPathVex);
    }
    void vexPathToEdgePath(Service &lightSer)
    { // ��ת��
        // this->findAllPath(lightSer);
        lightSer.allPathEdge.resize(0);
        vector<EdgePath> newAllPath;
        for (int i = 0; i < lightSer.allPathVex.size(); ++i)
        {
            vector<vector<int>> onePathEdge;
            for (int j = 0; j < lightSer.allPathVex[i].size() - 1; j++)
            {
                vector<int> v;
                v.resize(0);
                for (int k = 0; k < this->edgeTable[lightSer.allPathVex[i][j]][lightSer.allPathVex[i][j + 1]].edges.size(); k++)
                {
                    v.push_back(this->edgeTable[lightSer.allPathVex[i][j]][lightSer.allPathVex[i][j + 1]].edges[k].edgeNum);
                }
                onePathEdge.push_back(v);
            }
            vector<EdgePath> agent = calculateCombination(onePathEdge, lightSer.allPathVex[i]);
            newAllPath.insert(newAllPath.end(), agent.begin(), agent.end());
        }
        lightSer.allPathEdge = newAllPath;
    }
    void vexPathToEdgePathSimplify(Service &lightSer)
    { // �򻯰��ת��
        vector<EdgePath> newAllPath;
        for (int i = 0; i < lightSer.allPathVex.size(); i++)
        {
            EdgePath path;
            for (int j = 0; j < lightSer.allPathVex[i].size() - 1; j++)
            {
                path.edgeNum.push_back(this->edgeTable[lightSer.allPathVex[i][j]][lightSer.allPathVex[i][j + 1]].edges[0].edgeNum);
            }
            path.vexPath = lightSer.allPathVex[i];
            newAllPath.push_back(path);
        }
        lightSer.allPathEdge = newAllPath;
    }
    int checkPathIsFeasible(EdgePath &path)
    { // ������·�����Ƿ��п��õ�ͨ�������еĻ�����ͨ����ţ�û�еĻ�����-1
        for(int i=0;i<channelAmount;i++){
            for(int j=0;j<path.edgeNum.size();j++){
                if(!this->edgeArr[path.edgeNum[j]].channel[i]){
                    break;
                }
                if(j==path.edgeNum.size()-1)
                    return i;
            }
        }
        return -1;
        /*auto iter = path.needAddQuantityAndLocation.begin();
        if (iter->first == 0)
        {
            return iter->second.first;
        }
        return -1;*/
    }
    int calculatePathDistance(vector<int> path)
    { // ����һ����·���ľ���֮��
        int distance = 0;
        for (int i = 0; i < path.size(); i++)
        {
            distance += this->edgeArr[path[i]].edgeWeight;
        }
        return distance;
    }
    void sortAllPathVex(vector<vector<int>> &allPathVex)
    { // ����ҵ����㵽�յ�ĵ�·������������
        for (int i = 0; i < allPathVex.size(); ++i)
        {
            for (int j = 0; j < allPathVex.size() - i - 1; ++j)
            {
                int sum1 = allPathVex[j].size();
                int sum2 = allPathVex[j + 1].size();
                if (sum1 > sum2)
                {
                    swap(allPathVex[j], allPathVex[j + 1]);
                }
            }
        }
    }
    void sortAllPathEdge(vector<vector<int>> &allPathEdge)
    { // ����ҵ����㵽�յ�ı�·������������(���������ܸ��ã�
        for (int i = 0; i < allPathEdge.size(); ++i)
        {
            for (int j = 0; j < allPathEdge.size() - i - 1; ++j)
            {
                /*int sum1 = this->calculatePathDistance(allPathEdge[j]);
                int sum2 = this->calculatePathDistance(allPathEdge[j+1]);*/
                int sum1 = allPathEdge[j].size();
                int sum2 = allPathEdge[j + 1].size();
                if (sum1 > sum2)
                {
                    swap(allPathEdge[j], allPathEdge[j + 1]);
                }
            }
        }
    }
    vector<int> deploySequenceEdge(const vector<Service> &services)
    { // ���ձ�·����ȷ������ҵ���˳��
        vector<int> sequence;
        vector<pair<int, int>> result;
        for (int i = 0; i < services.size(); i++)
        {
            // result.emplace_back(i,this->calculatePathDistance(services[i].allPathEdge[0]));
            result.emplace_back(i, services[i].allPathEdge[0].edgeNum.size());
        }
        sort(result.begin(), result.end(), [](const pair<int, int> &p1, const pair<int, int> &p2)
        {
            return p1.second > p2.second; // ���ڶ���Ӵ�С����
        });
        for (int i = 0; i < result.size(); i++)
        {
            sequence.push_back(result[i].first);
        }
        return sequence;
    }
    vector<int> deploySequenceVex(const vector<Service> &services)
    { // ���ձ�·����ȷ������ҵ���˳��
        vector<int> sequence;
        vector<pair<int, int>> result;
        for (int i = 0; i < services.size(); i++)
        {
            // result.emplace_back(i,this->calculatePathDistance(services[i].allPathEdge[0]));
            result.emplace_back(i, services[i].allPathVex[0].size());
        }
        sort(result.begin(), result.end(), [](const pair<int, int> &p1, const pair<int, int> &p2)
        {
            return p1.second > p2.second; // ���ڶ���Ӵ�С����
        });
        for (int i = 0; i < result.size(); i++)
        {
            sequence.push_back(result[i].first);
        }
        return sequence;
    }
    int findEdgeStart(Service &lightSer, vector<int> path, int edgeNum)
    { // ��ĳ��·����ĳ���߶�Ӧ�����(�ò����ˣ�
        vector<int> recordVex;
        if (lightSer.start == this->edgeArr[path[0]].vex1)
        {
            recordVex.push_back(this->edgeArr[path[0]].vex1);
            recordVex.push_back(this->edgeArr[path[0]].vex2);
        }
        else
        {
            recordVex.push_back(this->edgeArr[path[0]].vex2);
            recordVex.push_back(this->edgeArr[path[0]].vex1);
        }
        for (int i = 1; i < path.size(); i++)
        {
            if (edgeNum == path[i])
                return recordVex[i];
            else
            {
                if (recordVex[i] == this->edgeArr[path[i]].vex1)
                {
                    recordVex.push_back(this->edgeArr[path[i]].vex2);
                }
                else
                {
                    recordVex.push_back(this->edgeArr[path[i]].vex1);
                }
            }
        }
        return -1;
    }
    int edgeWeightMin(int vex1, int vex2)
    { // ��������֮��ߵ���СȨֵ
        Edges vEdge = this->edgeTable[vex1][vex2];
        auto it = min_element(vEdge.edges.begin(), vEdge.edges.end(),
                              [](const Edge &a, const Edge &b)
                              { return a.edgeWeight < b.edgeWeight; });
        int minWeight = it->edgeWeight;
        return minWeight;
    }
    void initialMap(EdgePath &path)
    {
        for (int i = 0; i < channelAmount; i++)
        {
            vector<int> location;
            int quantity = 0;
            for (int j = 0; j < path.edgeNum.size(); j++)
            {
                if (this->edgeArr[path.edgeNum[j]].channel[i] == false)
                {
                    quantity++;
                    location.push_back(path.edgeNum[j]);
                }
            }
            path.needAddQuantityAndLocation.insert(make_pair(quantity, make_pair(i, location)));
        }
    }
    void serviceDeploy(Service &lightSer, AStar &aStar)
    { // �����ҵ��
        /*for (int i = 0; i < lightSer.allPathEdge.size(); i++)
        { // ��ʼ����·��������
            this->initialMap(lightSer.allPathEdge[i]);
        }*/
        int channelNum = -1;
        for (int i = 0; i < lightSer.allPathEdge.size(); i++)
        { // �ҵ�����ͨ����·��
            channelNum = checkPathIsFeasible(lightSer.allPathEdge[i]);
            if (channelNum != -1)
            {                                                                     // ·�����п���ͨ��
                lightSer.channelNum = channelNum;                                 // ��ҵ��ʹ�õ�ͨ�����ȷ��
                lightSer.passEdgeAmount = lightSer.allPathEdge[i].edgeNum.size(); // ��ҵ�񾭹��ı�����
                lightSer.passEdgeNum = lightSer.allPathEdge[i];                   // ��ҵ�񾭹��ı߱��
                break;
            }
        }
        if (channelNum == -1)
        { // ·�����޿���ͨ�����üӱ�
            int amount=this->edgeAmount;
            for(int i=0;i<lightSer.allPathEdge[0].edgeNum.size();i++){
                addEdge(this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex1,
                        this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex2,
                        this->edgeTable[this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex1]
                        [this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex2].min);
                aStar.overallAddEdge(this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex1,
                                     this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex2,
                                     this->edgeTable[this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex1]
                                     [this->edgeArr[lightSer.allPathEdge[0].edgeNum[i]].vex2].min);
            }//��һ��·���ı߸���һ��
            lightSer.channelNum=0;
            lightSer.passEdgeAmount=this->edgeAmount-amount;
            for(int j=0;j<lightSer.passEdgeAmount;j++){
                lightSer.passEdgeNum.edgeNum.push_back(this->edgeArr[amount].edgeNum);
                amount++;
            }
            /*int min = INFINITY;          // ҵ�����ӱߵ�������
            int num = channelAmount + 1; // ҵ���ͨ����
            vector<int> path;            // ���ӱ߶�Ӧ��·��
            vector<int> path1;           // ��ȱ��ͨ������ҵ���޷�ͨ����ԭ��·��
            for (int i = 0; i < lightSer.allPathEdge.size(); i++)
            {
                // this->initialMap(lightSer.allPathEdge[i]);
                auto firstKey = lightSer.allPathEdge[i].needAddQuantityAndLocation.begin();
                if (min > firstKey->first)
                {
                    path1 = lightSer.allPathEdge[i].edgeNum;
                    min = firstKey->first;
                    num = firstKey->second.first;
                    path = firstKey->second.second;
                    lightSer.passEdgeNum.vexPath = lightSer.allPathEdge[i].vexPath;
                }
            }
            int amount = this->edgeAmount;
            for (int j = 0; j < min; j++)
            {
                int vex1 = this->edgeArr[path[j]].vex1;
                int vex2 = this->edgeArr[path[j]].vex2;
                addEdge(vex1, vex2, this->edgeTable[vex1][vex2].min);
                aStar.overallAddEdge(vex1, vex2, this->edgeTable[vex1][vex2].min);
            }
            lightSer.channelNum = num;
            lightSer.passEdgeAmount = path1.size();
            for (int i = 0, j = 0; i < path1.size(); i++)
            {
                if (path1[i] == path[j])
                {
                    lightSer.passEdgeNum.edgeNum.push_back(this->edgeArr[amount].edgeNum);
                    i++;
                    j++;
                    amount++;
                }
                else
                {
                    lightSer.passEdgeNum.edgeNum.push_back(path1[i]);
                    i++;
                }
            }*/
            // this->vexPathToEdgePath(lightSer);
            /*for(int i=lightSer.serviceNum;i<this->serviceTable.size();i++){//����ҵ���Լ�����ҵ���ȫ����·����·�����ȴ�С��������
                this->sortAllPathEdge(this->serviceTable[i].allPathEdge);
            }*/
            // this->findAllPath(lightSer);
            // serviceDeploy(lightSer);
            // return;
        }
        for (int j = 0; j < lightSer.passEdgeNum.edgeNum.size(); j++)
        { // �ѱ����Ѿ�ʹ���˵�ͨ����Ϊfalse
            this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].channel[lightSer.channelNum] = false;
            this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].usableChannelAmounnt--;
            auto it = find(this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2].edges.begin(),
                           this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2].edges.end(), this->edgeArr[lightSer.passEdgeNum.edgeNum[j]]);
            it->channel[lightSer.channelNum] = false;
            it->usableChannelAmounnt--;
            auto it1 = find(this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1].edges.begin(),
                            this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1].edges.end(), this->edgeArr[lightSer.passEdgeNum.edgeNum[j]]);
            it1->channel[lightSer.channelNum] = false;
            it1->usableChannelAmounnt--;
            // lightSer.allPathEdge[i].everyChannelLack[channelNum].push_back(lightSer.passEdgeNum[j]);
            if (it->usableChannelAmounnt == 0)
            {
                aStar.removeEdge(it->edgeNum);
                this->edgeTable[it->vex2][it->vex1].edges.erase(it1);
                this->edgeTable[it->vex1][it->vex2].edges.erase(it);
                if (this->edgeTable[it->vex1][it->vex2].edges.size() == 0)
                {
                    addEdge(this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1, this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2,
                            this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2].min);
                    aStar.overallAddEdge(this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1, this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2,
                                         this->edgeTable[this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex1][this->edgeArr[lightSer.passEdgeNum.edgeNum[j]].vex2].min);
                }
            }
        }
        for (int k = 0; k < lightSer.passEdgeNum.edgeNum.size(); k++)
        { // ���뽫Ҫ�������˥������ĵ�Ҫ�ŷŴ���
            int distance = 0;
            while (distance <= maxDistance && k < lightSer.passEdgeNum.edgeNum.size())
            {
                distance += this->edgeArr[lightSer.passEdgeNum.edgeNum[k]].edgeWeight;
                k++;
            }
            k--;
            if (distance > maxDistance)
            {
                int vex = this->findEdgeStart(lightSer, lightSer.passEdgeNum.edgeNum, lightSer.passEdgeNum.edgeNum[k]);
                // int vex=lightSer.passEdgeNum.vexPath[k];
                lightSer.passAmplifierNum.push_back(vex);
                lightSer.passAmplifierAmount++;
                k--;
            }
        }
        // break;
    }
    int vexAmount;                   // ��������
    vector<Vertex> vexTable;         // �����
    int serviceAmount;               // ҵ������
    vector<Service> serviceTable;    // ҵ���
    int edgeAmount;                  // ������
    vector<vector<Edges>> edgeTable; // �߱�
    vector<Edge> edgeArr;            // ����ͨ���ߵı���ҵ��ñ�
};
int main()
{
    int N, M, T, P, D;
    cin >> N >> M >> T >> P >> D; // �����һ��
    OpticalNetWork netWork(N, P, D);
    AStar aStar(N, M);

    for (int i = 0; i < M; i++)
    {
        int s, t, d;
        cin >> s >> t >> d; // ����M��
        netWork.addEdge(s, t, d);
        aStar.overallAddEdge(s, t, d); // ����A*�㷨���ҵ�·��
    }
    for (int i = 0; i < T; i++)
    {
        int s, t;
        cin >> s >> t; // ����T��
        netWork.addService(s, t);
    }
    cout<<endl;
    /*for(int i=0;i<netWork.serviceTable.size();i++){//���ҵ�������յ���ͬ�������ظ���һ��·��
        int j=0;
        for(;j<i;j++){
            if(netWork.serviceTable[i].start==netWork.serviceTable[j].start&&netWork.serviceTable[i].end==netWork.serviceTable[j].end){
                netWork.serviceTable[i].allPathVex=netWork.serviceTable[j].allPathVex;
                break;
            }
        }
        if(j==i){
            netWork.findAllPath(netWork.serviceTable[i]);
            netWork.sortAllPathVex(netWork.serviceTable[i].allPathVex);
        }
    }*/
    // for(int i=0;i<netWork.serviceTable.size();i++){
    //     aStar.findAllPaths(netWork.serviceTable[i].start,netWork.serviceTable[i].end,netWork.serviceTable[i].allPathEdge);
    // }
    for (int i = 0; i < netWork.serviceTable.size(); i++)
    { // ����ҵ��
        // netWork.findAllPath(netWork.serviceTable[i]);
        // netWork.vexPathToEdgePath(netWork.serviceTable[i]);
        aStar.findAllPaths(netWork.serviceTable[i].start, netWork.serviceTable[i].end, netWork.serviceTable[i].allPathEdge);
        netWork.serviceDeploy(netWork.serviceTable[i], aStar);
    }
    /*for(int i=0;i<netWork.serviceTable.size();i++){//������ҵ���ȫ����·����·�����ȴ�С��������
        netWork.sortAllPathVex(netWork.serviceTable[i].allPathVex);
    }
    vector<int> sequence=netWork.deploySequenceVex(netWork.serviceTable);
    for(int i=0;i<sequence.size();i++){//����ҵ��
        netWork.vexPathToEdgePath(netWork.serviceTable[sequence[i]]);
        netWork.serviceDeploy(netWork.serviceTable[sequence[i]]);
    }*/
    /*for(int i=0;i<netWork.serviceTable.size();i++){//����ҵ��
        netWork.vexPathToEdgePath(netWork.serviceTable[i]);
        netWork.serviceDeploy(netWork.serviceTable[i]);
    }*/
    cout << netWork.edgeArr.size() - M << endl; // ����¼��˶�������
    for (int i = M; i < netWork.edgeArr.size(); i++)
    { // ����¼ӵıߵ������յ�
        cout << netWork.edgeArr[i].vex1 << " " << netWork.edgeArr[i].vex2;
        cout << endl;
    }
    for (int i = 0; i < netWork.serviceTable.size(); i++)
    { // �������T��
        cout << netWork.serviceTable[i].channelNum << " " << netWork.serviceTable[i].passEdgeAmount << " "
             << netWork.serviceTable[i].passAmplifierAmount << " ";
        for (int j = 0; j < netWork.serviceTable[i].passEdgeNum.edgeNum.size(); j++)
        {
            cout << netWork.serviceTable[i].passEdgeNum.edgeNum[j] << " ";
        }
        for (int k = 0; k < netWork.serviceTable[i].passAmplifierNum.size(); k++)
        {
            cout << netWork.serviceTable[i].passAmplifierNum[k] << " ";
        }
        cout << endl;
    }
    return 0;
}
