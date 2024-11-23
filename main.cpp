#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include <map>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

// Flight class to represent a flight between cities
class Flight
{
public:
    int destination;
    int cost;     // Cost in Rs
    float distance; // Distance in km
    float duration; // Duration in hours
    Flight(int dest, int c, float dist, float dur)
        : destination(dest), cost(c), distance(dist), duration(dur) {}
};

// Graph class for managing flights
class FlightGraph
{
private:
    int numCities;
    vector<vector<Flight>> adjList;
    map<string, int> cityIndex;  // Map to store city names to indices
    vector<string> cityNames;    // List to store indices to city names
    const double fuelRate = 5.0; // Fuel rate per km (assumed Rs 5 per km)

public:
    FlightGraph(int cities) : numCities(cities), adjList(cities)
    {
        cityNames.resize(cities);
    }

    // Method to add a city to the map
    void addCity(const string &name, int index)
    {
        cityIndex[name] = index;
        cityNames[index] = name;
    }

    // Method to add a flight with specified cost in Rs, distance in km, and duration in hours
    void addFlight(const string &u, const string &v, int cost, float distance, float duration)
    {
        if (cityIndex.find(u) == cityIndex.end() || cityIndex.find(v) == cityIndex.end())
        {
            cout << "Error: One or both cities not found.\n";
            return;
        }
        int uIndex = cityIndex[u];
        int vIndex = cityIndex[v];
        adjList[uIndex].emplace_back(vIndex, cost, distance, duration);
        adjList[vIndex].emplace_back(uIndex, cost, distance, duration); // For undirected graph
    }

    // Dijkstra’s algorithm to find the minimum cost path
    vector<int> dijkstraCost(const string &source)
    {
        if (cityIndex.find(source) == cityIndex.end())
        {
            cout << "Error: Source city not found.\n";
            return {};
        }
        int sourceIndex = cityIndex[source];
        vector<int> minCost(numCities, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        minCost[sourceIndex] = 0;
        pq.push({0, sourceIndex});

        while (!pq.empty())
        {
            int cost = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (cost > minCost[u])
                continue;

            for (const Flight &flight : adjList[u])
            {
                int v = flight.destination;
                int flightCost = flight.cost;
                if (minCost[u] + flightCost < minCost[v])
                {
                    minCost[v] = minCost[u] + flightCost;
                    pq.push({minCost[v], v});
                }
            }
        }
        return minCost;
    }

    // Dijkstra's algorithm to find the shortest path based on distance
    vector<float> dijkstraDistance(const string &source)
    {
        if (cityIndex.find(source) == cityIndex.end())
        {
            cout << "Error: Source city not found.\n";
            return {};
        }
        int sourceIndex = cityIndex[source];
        vector<float> minDistance(numCities, numeric_limits<float>::max());
        priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
        minDistance[sourceIndex] = 0;
        pq.push({0, sourceIndex});

        while (!pq.empty())
        {
            float distance = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (distance > minDistance[u])
                continue;

            for (const Flight &flight : adjList[u])
            {
                int v = flight.destination;
                float flightDistance = flight.distance;
                if (minDistance[u] + flightDistance < minDistance[v])
                {
                    minDistance[v] = minDistance[u] + flightDistance;
                    pq.push({minDistance[v], v});
                }
            }
        }
        return minDistance;
    }

    // Method to display flight details
    void displayFlightDetails(const vector<int> &costs, const vector<float> &distances, const string &source)
    {
        if (costs.empty() || distances.empty())
        {
            cout << "Error: Unable to display details as the source city is invalid or data is incomplete.\n";
            return;
        }

        cout << "\nFlight details from " << source << ":\n";
        cout << left << setw(15) << "City" << setw(15) << "Cost (Rs)"
             << setw(20) << "Distance (km)" << "Fuel Cost (Rs)" << endl;

        for (int i = 0; i < numCities; i++)
        {
            if (costs[i] == INT_MAX || distances[i] == numeric_limits<float>::max())
            {
                cout << setw(15) << cityNames[i] << setw(15) << "Unreachable"
                     << setw(20) << "Unreachable"
                     << "Unreachable" << endl;
            }
            else
            {
                double fuelCost = distances[i] * fuelRate;
                cout << setw(15) << cityNames[i]
                     << setw(15) << costs[i]
                     << setw(20) << fixed << setprecision(2) << distances[i]
                     << fixed << setprecision(2) << fuelCost << endl;
            }
        }
    }
};

// Helper function to get valid integer input
int getValidInt(const string &prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value <= 0)
        {
            cout << "Invalid input. Please enter a positive integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            return value;
        }
    }
}

int main()
{
    int numCities = getValidInt("Enter the number of cities: ");
    FlightGraph flightGraph(numCities);

    cout << "Enter the names of the cities:\n";
    for (int i = 0; i < numCities; i++)
    {
        string cityName;
        cin >> cityName;
        flightGraph.addCity(cityName, i);
    }

    int numFlights = getValidInt("Enter the number of flights: ");
    for (int i = 0; i < numFlights; i++)
    {
        string u, v;
        int cost;
        float distance, duration;

        while (true)
        {
            cout << "Flight " << i + 1 << ":\n";
            cout << "  Enter source, destination, cost (Rs), distance (km), and duration (hours): ";
            cin >> u >> v >> cost >> distance >> duration;

            if (cin.fail() || cost <= 0 || distance <= 0 || duration <= 0)
            {
                cout << "Invalid input. Please enter positive values for cost, distance, and duration.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else
            {
                flightGraph.addFlight(u, v, cost, distance, duration);
                break;
            }
        }
    }

    string source;
    cout << "Enter the source city: ";
    cin >> source;

    vector<int> minCosts = flightGraph.dijkstraCost(source);
    vector<float> minDistances = flightGraph.dijkstraDistance(source);

    flightGraph.displayFlightDetails(minCosts, minDistances, source);

    return 0;
}


give me the references slide data for this project..
