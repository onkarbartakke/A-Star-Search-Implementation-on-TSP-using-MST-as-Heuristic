#include <bits/stdc++.h>

using namespace std;

int N, E, start;

class State
{
public:
    int cost_till_now;
    int TotalCost;
    unordered_set<int> vis_st;
    int node;
    bool isGoalNode;
    State *parent;

    State(int u = 0, int c = 0)
    {
        node = u;
        cost_till_now = c;
        TotalCost = 0;
        vis_st.insert(node);
        isGoalNode = false;
        parent = NULL;
    }
};

unordered_map<int, vector<int>> mp;

int cost[1000][1000];

struct comp
{
    bool operator()(State *a, State *b)
    {
        if (a->TotalCost < b->TotalCost)
            return false;
        else
            return true;
    }
};

void Initialize()
{
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
            cost[i][j] = -1;
    }
}

priority_queue<State *, vector<State *>, comp> pq;

void PrintPath(State *s)
{
    cout << start << " -> ";

    while (s != NULL)
    {
        if (s->parent == NULL)
            cout << s->node;
        else
            cout << s->node << " -> ";

        s = s->parent;
    }
}

int MST_Cost(unordered_set<int> &st)
{
    if (st.size() == N || st.size() == N - 1)
        return 0;

    int n = N - st.size();

    vector<bool> mst(N + 1, false);
    vector<int> dist(N + 1, INT_MAX);

    set<pair<int, int>> stp;

    int src = -1;

    for (int i = 1; i <= N; i++)
    {
        if (st.find(i) != st.end())
            continue;

        if (src == -1)
            src = i;

        stp.insert({INT_MAX, i});
    }

    if (src == -1)
        return 0;

    stp.erase({INT_MAX, src});
    stp.insert({0, src});

    vector<int> parent(N + 1, -1);

    parent[src] = -1;
    while (!stp.empty())
    {
        auto p = (*stp.begin());
        stp.erase(p);

        dist[p.second] = p.first;
        int u = p.second;
        mst[u] = true;

        for (int v = 1; v <= N; v++)
        {
            if (st.find(v) != st.end())
                continue;

            if (cost[u][v] != -1 && dist[v] > cost[u][v] && mst[v] == false)
            {
                parent[v] = u;
                stp.erase({dist[v], v});
                dist[v] = cost[u][v];
                stp.insert({dist[v], v});
            }
        }
    }

    int MSTcost = 0;

    for (int i = 1; i <= N; i++)
    {
        if (st.find(i) != st.end())
            continue;

        MSTcost += cost[i][parent[i]];
    }

    return MSTcost;
}

void A_Star_Search(State *u)
{
    pq.push(u);

    while (!pq.empty())
    {
        State *s = pq.top();
        pq.pop();

        if (s->isGoalNode)
        {
            cout << "Total Cost of TSP Tour is :  " << s->cost_till_now << "\n";

            cout << "\nTSP path is : \n";
            PrintPath(s);
            return;
        }

        int HeuristCost;

        for (auto v : mp[s->node])
        {
            if (s->vis_st.find(v) != s->vis_st.end())
                continue;

            s->vis_st.insert(v);
            HeuristCost = MST_Cost(s->vis_st);
            s->vis_st.erase(v);

            if (s->vis_st.size() == N - 1)
            {
                State *p = new State(v);

                if (cost[v][start] == -1)
                {
                    p->cost_till_now = INT_MAX;
                    p->TotalCost = INT_MAX;
                    p->isGoalNode = false;
                    p->parent = s;
                }
                else
                {
                    p->cost_till_now = s->cost_till_now + cost[s->node][v] + cost[v][start];
                    p->TotalCost = p->cost_till_now + HeuristCost;
                    p->isGoalNode = true;
                    p->parent = s;
                }

                pq.push(p);
            }

            State *p = new State(v);
            p->cost_till_now = s->cost_till_now + cost[s->node][v];
            p->TotalCost = p->cost_till_now + HeuristCost;
            p->isGoalNode = false;
            p->vis_st = s->vis_st;
            p->vis_st.insert(v);
            p->parent = s;

            pq.push(p);
        }
    }
}

int main()
{

    cout << "Enter the number of nodes (1-N):  ";

    cin >> N;

    cout << "\nEnter the number of Edges : ";

    cin >> E;

    cout << "\n\nEnter the Edges with Edge cost (u,v,c => Depicts Edge from u to v with cost c) :  \n";

    start = -1;
    Initialize();
    for (int i = 0; i < E; i++)
    {
        int u, v, c;

        cin >> u >> v >> c;

        if (start == -1)
            start = u;

        mp[u].push_back(v);
        mp[v].push_back(u);

        cost[u][v] = c;
        cost[v][u] = c;
    }

    State *s = new State(start);
    cout << "\nProcessing...\n";
    A_Star_Search(s);
}