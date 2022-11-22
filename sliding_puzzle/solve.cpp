#include <iostream>
#include <vector>
#include <string.h>
#include <utility>
#include <map>
#include <list>
#include <queue>
#define MAX_STR_SIZE 451
#define INF 999999
//#define vertex string

using namespace std;
using vertex = pair<string, char>;
using ii = pair<int, int>;

const static ii moves[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
const static char *blankMoves = "bcde";

int M, N;

int zeroPlace(string state)
{
  for (int i = 0; i < state.size(); i++)
  {
    if (state[i] == '0')
      return i;
  }

  return -1;
}

bool isSolved(string state)
{
  int str_size = state.size();
  for (int i = 0; i < str_size; i++)
  {
    char cell = state[i];
    int goalPosition = cell - '0';
    int currentPosition = i;

    if (currentPosition != goalPosition)
      return false;
  }

  return true;
}

class Graph
{
  map<vertex, list<vertex>> adj;

public:
  Graph(); // Constructor

  // function to add an edge to graph
  void addEdge(vertex v, vertex w);

  // prints BFS traversal from a given source s
  int BFS(vertex s, int M, int N);
};

Graph::Graph() {}

void Graph::addEdge(vertex v, vertex w)
{
  adj[v].push_back(w); // Add w to v’s list.
}

int manhatamDistance(string state, int rows, int stateSize)
{
  int result = 0;
  int str_size = stateSize;
  // cout << "CALCULATING MD OF: " << state << endl;
  // cout << "---------------------" << endl;
  for (int i = 0; i < str_size; i++)
  {
    int cell = state[i] - '0';
    int currentXpos = i / rows;
    int currentYpos = i % rows;
    int goalXpos = cell / rows;
    int goalYpos = cell % rows;

    int distance = abs(goalXpos - currentXpos) + abs(goalYpos - currentYpos);
    // cout << "CURRENT i: " << currentXpos << " GOAL i: " << goalXpos << " CURRENT j: " << currentYpos << " GOAL j: " << goalYpos << endl;

    result += distance;
  }

  // cout << "RESULTADO: " << result << endl;

  return result;
}

struct prioritize
{
  bool operator()(const vertex v, const vertex w) const
  {
    int m1, m2;
    m1 = manhatamDistance(v.first, M, M * N);
    m2 = manhatamDistance(w.first, M, M * N);

    // cout << v.first << " > " << w.first << " ? : " << (m1 > m2) << endl;
    return (m1 > m2);
  }
};

int Graph::BFS(vertex s, int N, int M)
{

  int steps = 0;

  if (isSolved(s.first))
    return steps;

  map<string, bool> visited;

  auto it = visited.begin();
  // Iterate over the map using Iterator till end.
  while (it != visited.end())
  {
    it->second = false;
    it++;
  }

  // Create a queue for BFS
  priority_queue<vertex, vector<vertex>, prioritize> queue;

  // Mark the current node as visited and enqueue it
  visited[s.first] = true;
  queue.push(s);
  while (!queue.empty())
  {
    // Dequeue a vertex from queue and print it
    s = queue.top();
    // cout << "CAUSA: " << s.first << endl;
    queue.pop();

    //cout << "ESCOLHIDO: " << s.first << endl;
    //cout << "MOVIMENTO GERADOR: " << s.second << endl;

    int zero = zeroPlace(s.first);

    // cout << "POSSIBILIDADES DE " << s.first << endl;
    // cout << "--------------------------------" << endl;

    visited[s.first] = true;

    for (int i = 0; i < 4; i++)
    {
      vertex aux;
      aux.first = s.first;
      aux.second = s.second;
      int x = moves[i].first;
      int y = moves[i].second;

      int currentXpos = zero / M;
      int currentYpos = zero % M;

      int newYpos = currentYpos + x;
      int newXpos = currentXpos + y;

      int newPos = zero + x + (y * M);

      // cout << "NEWX: " << newXzero << endl;
      // cout << "NEWY: " << newYzero << endl;
      // cout << "CURRENT 0: " << zero << endl;
      // cout << "NEW 0: " << newPos << endl;

      if ((newPos >= 0 && newPos < (M * N)) && (newYpos >= 0 && newYpos < (M)) && ((newXpos >= 0 && newXpos < (N))))
      {
        // cout << "CURRENTX: " << currentXpos << endl;
        // cout << "CURRENTY: " << currentYpos << endl;
        // cout << "NEWX: " << newXpos << endl;
        // cout << "NEWY: " << newYpos << endl;
        // cout << "NEW POSITION: " << newPos << endl;

        swap(aux.first[zero], aux.first[newPos]);
        aux.second = blankMoves[i];
        cout << "MOVE: " << aux.second << endl;
        cout << "PREV: " << s.first << endl;
        cout << "NEXT: " << aux.first << endl;
        // cout << "MOVIMENTO: " << aux.second << endl;
        //  cout << "NEWX: " << newXzero << " NEWY:" << newYzero << endl;
        // cout << "x: " << x << " | "
        //      << "y: " << y << endl;
        // cout << aux.first << endl;

        this->addEdge(s, aux);

        if (isSolved(aux.first))
        {
          // cout << "Encontrei!" << endl;
          // cout << "MOVIMENTO GERADOR: " << aux.second << endl;
          // cout << "FINAL: " << aux.first << endl;
          cout << "MOVIMENTO GERADOR: " << aux.second << endl;
          return ++steps;
        }
      }
    }

    if (isSolved(s.first))
    {
      cout << "MOVIMENTO GERADOR: " << s.second << endl;
      break;
    }

    for (auto adjecent : adj[s])
    {
      if (isSolved(adjecent.first))
      {
        cout << "MOVIMENTO: " << adjecent.second << endl;
        break;
      }
      if (!visited[adjecent.first])
      {
        queue.push(adjecent);
      }
    }

    steps++;
  }

  return -1;
}

int main()
{
  char s[MAX_STR_SIZE];
  int size;

  scanf("%d %d", &M, &N);

  size = N * M;

  cin.get();

  for (int i = 0; i < size; i++)
  {
    char c;
    scanf(" %c", &c);
    s[i] = c;
  }

  s[size] = '\0';

  string solved = isSolved(s) == 0 ? "Não" : "Sim";

  cout << "ESTADO INICIAL: " << s << endl;

  Graph g;

  vertex v = {s, '0'};

  int steps = g.BFS(v, M, N);

  if (steps >= 0)
  {
    cout << "Encontrou com : " << steps << " passos" << endl;
  }
  else
  {
    cout << "Não encontrou!" << endl;
  }
  return 0;
}