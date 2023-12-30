// application.cpp <Starter Code>
// <Khang Ho>
//
// University of Illinois at Chicago
// CS 251: Fall 2021
// Project #7 - Openstreet Maps
//
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <limits>
#include <queue>

#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

const double INF = numeric_limits<double>::max();


//
// Data structure: queueNode for dijkstra algorithm
// includes ID, distance, and predecessor
//
struct queueNode {
    long long ID;
    long long pred;
    double dist;

    queueNode() {}
    queueNode(long long ID, long long Pred, double Dist) {
        this->ID = ID;
        this->pred = Pred;
        this->dist = Dist;
    }
};

class prioritize {
 public:
    bool operator()(queueNode& v1, queueNode& v2) const {
        if (v1.dist == v2.dist) {
            return v1.ID > v2.ID;
        }
        else {
            return v1.dist > v2.dist;
        }
    }
};

//
// helper function: buildingReachable
// Check if a building is within unreachableBuilding,
// return false
// else return true
//
bool buildingReachable(BuildingInfo& building,
    vector<BuildingInfo>& unreachableBuilding) {
    if (unreachableBuilding.size() == 0) {
        return true;
    }
    else {
        for (auto& bd : unreachableBuilding) {
            if (building.Fullname == bd.Fullname) {
                return false;
            }
        }
    }
    return true;
}

// MS: 10
// Dijkstra Algorithm
map<long long, queueNode> DijkstraSearch(graph<long long, double>& G,
    long long startV, long long dest) {
    map<long long, queueNode> directionMap;
    if (startV == dest) {
        queueNode newNode(startV, -1, 0);
        directionMap[startV] = newNode;
        return directionMap;
    }
    set<long long> visited;
    vector<long long> Vertices = G.getVertices();
    priority_queue<queueNode, vector<queueNode>, prioritize> pq;

    for (auto v : Vertices) {
        directionMap[v] = queueNode(v, -1, INF);
        pq.push(queueNode(v, -1, INF));
    }

    directionMap[startV].dist = 0;
    pq.push(queueNode(startV, -1, 0));

    while (!pq.empty()) {
        queueNode currentV = pq.top();
        pq.pop();

        if (currentV.dist == INF) break;
        else if (visited.count(currentV.ID) == 1)
            continue;
        else visited.insert(currentV.ID);

        set<long long> neighbors = G.neighbors(currentV.ID);
        for (auto& v : neighbors) {
            double edgeWeight;
            G.getWeight(currentV.ID, v, edgeWeight);
            double alterDist = currentV.dist + edgeWeight;

            if (alterDist < directionMap.at(v).dist) {
                directionMap[v].dist = alterDist;
                directionMap[v].pred = currentV.ID;
                if (v == dest) {
                    return directionMap;
                }
                else pq.push(queueNode(v, currentV.ID, alterDist));
            }
        }
    }

    return directionMap;
}

//
// helper function: printPath
// Print the path to go to destination
//
void printPath(map<long long, queueNode>& map,
    long long start, long long dest) {
    if (start == dest) {
        cout << start;
        return;
    }
    printPath(map, start, map.at(dest).pred);
    cout << "->" << dest;
}

//
// helper function: printPath
// Show path from person 1 to destination building
// and from person 2 to destination building
//
void printPath(map<long long, queueNode>& map1, map<long long, queueNode>& map2,
    long long& start1, long long& start2, long long& dest) {
    cout << "Person 1's distance to dest: ";
    cout << map1.at(dest).dist << " miles" << endl;
    cout << "Path: ";
    printPath(map1, start1, dest);
    cout << endl << endl;
    cout << "Person 2's distance to dest: ";
    cout << map2.at(dest).dist << " miles" << endl;
    cout << "Path: ";
    printPath(map2, start2, dest);
    cout << endl;
}

//
// Implement your standard application here
// TO DO: add a parameter for the graph you make.
//
void application(
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings, graph<long long, double>& G) {

    string person1Building, person2Building;

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    while (person1Building != "#") {
        cout << "Enter person 2's building (partial name or abbreviation)> ";
        getline(cin, person2Building);
        // MS7: Find person 1 building and person 2 building
        BuildingInfo building1, building2;
        // Check if the building is found or not
        bool found1 = false;
        bool found2 = false;
        // search for abbreviation
        for (auto& pb : Buildings) {
            if (pb.Abbrev == person1Building) {
                building1 = pb;
                found1 = true;
            }
            if (pb.Abbrev == person2Building) {
                building2 = pb;
                found2 = true;
            }
        }
        // search for partial/ full name
        for (auto& pb : Buildings) {
            if (!found1 && pb.Fullname.find(person1Building) != string::npos) {
                building1 = pb;
                found1 = true;
            }
            if (!found2 && pb.Fullname.find(person2Building) != string::npos) {
                building2 = pb;
                found2 = true;
            }
        }
        // If user input not valid name of buildings
        if (!found1) {
            cout << "Person 1's building not found" << endl;
            cout << endl;
            cout << "Enter person 1's building "
                << "(partial name or abbreviation), or #> ";
            getline(cin, person1Building);
            continue;
        }
        else if (!found2) {
            cout << "Person 2's building not found" << endl;
            cout << endl;
            cout << "Enter person 1's building "
                << "(partial name or abbreviation), or #> ";
            getline(cin, person1Building);
            continue;
        }
        bool found = false;
        vector<BuildingInfo> unreachableBuilding;
        while (!found) {
            // MS8: Find the center of 2 building
            Coordinates C1 = building1.Coords;
            Coordinates C2 = building2.Coords;
            Coordinates centerCoords =
                centerBetween2Points(C1.Lat, C1.Lon, C2.Lat, C2.Lon);
            Coordinates centerBuilding;
            double minDist = INF;
            BuildingInfo desB;
            /* Handle case when b != b1 and b2*/
            if (C1.ID == C2.ID) {
                centerBuilding = C1;
                desB = building1;
            }
            else {
                for (auto& b : Buildings) {
                    double dist = distBetween2Points(centerCoords.Lat,
                        centerCoords.Lon, b.Coords.Lat, b.Coords.Lon);
                    if (minDist > dist
                        && buildingReachable(b, unreachableBuilding)) {
                        minDist = dist;
                        centerBuilding = b.Coords;
                        desB = b;
                    }
                }
            }

            // MS9: Find node closest to building 1, 2 and destination
            // C1, C2 : coordinates for building1 and building2
            // centerBuilding: location of center building
            // finalBuilding: BuildingInfo of center building
            long long start1, start2, destination;
            double min1 = INF;
            double min2 = INF;
            double minDes = INF;
            for (auto& path : Footways) {
                for (auto& node : path.Nodes) {
                    Coordinates nodeCoord = Nodes.at(node);
                    double dist1 = distBetween2Points(C1.Lat, C1.Lon,
                        nodeCoord.Lat, nodeCoord.Lon);
                    double dist2 = distBetween2Points(C2.Lat, C2.Lon,
                        nodeCoord.Lat, nodeCoord.Lon);
                    double distDes = distBetween2Points(centerBuilding.Lat,
                        centerBuilding.Lon, nodeCoord.Lat, nodeCoord.Lon);
                    if (min1 > dist1) {
                        min1 = dist1;
                        start1 = node;
                    }
                    if (min2 > dist2) {
                        min2 = dist2;
                        start2 = node;
                    }
                    if (minDes > distDes) {
                        minDes = distDes;
                        destination = node;
                    }
                }
            }
            // print info about start1, start2, and current destination
            if (unreachableBuilding.size() == 0) {
                cout << "Person 1's point:" << endl;
                cout << " " << building1.Fullname << endl;
                cout << " (" << building1.Coords.Lat << ", "
                    << building1.Coords.Lon << ")" << endl;

                cout << "Person 2's point:" << endl;
                cout << " " << building2.Fullname << endl;
                cout << " (" << building2.Coords.Lat << ", "
                    << building2.Coords.Lon << ")" << endl;

                cout << "Destination Building:" << endl;
                cout << " " << desB.Fullname << endl;
                cout << " (" << desB.Coords.Lat << ", "
                    << desB.Coords.Lon << ")" << endl;
                cout << endl;

                cout << "Nearest P1 node:" << endl;
                cout << " " << Nodes[start1].ID << endl;
                cout << " (" << Nodes[start1].Lat << ", "
                    << Nodes[start1].Lon << ")" << endl;

                cout << "Nearest P2 node:" << endl;
                cout << " " << Nodes[start2].ID << endl;
                cout << " (" << Nodes[start2].Lat << ", "
                    << Nodes[start2].Lon << ")" << endl;

                cout << "Nearest destination node:" << endl;
                cout << " " << Nodes[destination].ID << endl;
                cout << " (" << Nodes[destination].Lat << ", "
                    << Nodes[destination].Lon << ")" << endl;
                cout << endl;
            }
            else {
                cout << "New destination building:" << endl;
                cout << " " << desB.Fullname << endl;
                cout << " (" << desB.Coords.Lat << ", "
                    << desB.Coords.Lon << ")" << endl;

                cout << "Nearest destination node:" << endl;
                cout << " " << Nodes[destination].ID << endl;
                cout << " (" << Nodes[destination].Lat << ", "
                    << Nodes[destination].Lon << ")" << endl;
                cout << endl;
            }
            // MS10 + 11: Run Dijkstra to find path
            // between start 1, start 2 and destination
            // check badBuildings.size() to avoid redundant search 1 to 2
            // check from building1 to building2 to 
            // make sure there is a possible path
            // If there is no path from person 1's
            // building to person 2's building
            // then there is no building they can meet at.
            if (unreachableBuilding.size() == 0) {
                map<long long, queueNode> s1_to_s2
                    = DijkstraSearch(G, start1, start2);
                if (s1_to_s2.size() > 1 && s1_to_s2.at(start2).dist == INF) {
                    cout << "Sorry, destination unreachable." << endl;
                    break;
                }
            }
            map<long long, queueNode> map1
                = DijkstraSearch(G, start1, destination);
            // If the destination is unreachable
            // repeat steps 8-11 with the second closest building to the center
            if (map1.size() > 1 && map1.at(destination).dist == INF) {
                unreachableBuilding.push_back(desB);
                cout << "At least one person was unable to reach the "
                    << "destination building. "
                    << "Finding next closest building...\n" << endl;
                continue;
            }
            map<long long, queueNode> map2
                = DijkstraSearch(G, start2, destination);
            if (map2.size() > 1 && map2.at(destination).dist == INF) {
                unreachableBuilding.push_back(desB);
                cout << "At least one person was unable to reach the "
                    << "destination building. "
                    << "Finding next closest building...\n" << endl;
                continue;
            }
            // output the distance and path
            printPath(map1, map2, start1, start2, destination);
            found = true;
        }
        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter person 1's building "
            << "(partial name or abbreviation), or #> ";
        getline(cin, person1Building);
    }
}

//
// Creative Component:
//  check if there is a loop that goes through 3 different buildings
//
void creative(map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings, graph<long long, double>& G) {

    string person1Building, person2Building, person3Building;
    
    cout << endl << "Enter 3 buildings that you want to check " <<
        "if there is a loop that goes through them." << endl;

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    while (person1Building != "#") {
        cout << "Enter person 2's building (partial name or abbreviation)> ";
        getline(cin, person2Building);
        cout << "Enter person 3's building (partial name or abbreviation)> ";
        getline(cin, person3Building);
        // MS7: Find person 1 building and person 2 building
        BuildingInfo building1, building2, building3;
        // Check if the building is found or not
        bool found1 = false;
        bool found2 = false;
        bool found3 = false;
        // search for abbreviation
        for (auto& pb : Buildings) {
            if (pb.Abbrev == person1Building) {
                building1 = pb;
                found1 = true;
            }
            if (pb.Abbrev == person2Building) {
                building2 = pb;
                found2 = true;
            }
            if (pb.Abbrev == person3Building) {
                building3 = pb;
                found3 = true;
            }
        }
        // search for partial/ full name
        for (auto& pb : Buildings) {
            if (!found1 && pb.Fullname.find(person1Building) != string::npos) {
                building1 = pb;
                found1 = true;
            }
            if (!found2 && pb.Fullname.find(person2Building) != string::npos) {
                building2 = pb;
                found2 = true;
            }
            if (!found3 && pb.Fullname.find(person3Building) != string::npos) {
                building3 = pb;
                found3 = true;
            }
        }
        // If user input not valid name of buildings
        if (!found1) {
            cout << "Person 1's building not found" << endl;
            cout << endl;
            cout << "Enter person 1's building "
                << "(partial name or abbreviation), or #> ";
            getline(cin, person1Building);
            continue;
        }
        else if (!found2) {
            cout << "Person 2's building not found" << endl;
            cout << endl;
            cout << "Enter person 1's building "
                << "(partial name or abbreviation), or #> ";
            getline(cin, person1Building);
            continue;
        }
        else if (!found3) {
            cout << "Person 3's building not found" << endl;
            cout << endl;
            cout << "Enter person 1's building "
                << "(partial name or abbreviation), or #> ";
            getline(cin, person1Building);
            continue;
        }
        /////////////////////////////////
        // MS9: Find node closest to building 1, 2 and destination
        // C1, C2 : coordinates for building1 and building2
        // centerBuilding: location of center building
        // finalBuilding: BuildingInfo of center building
        long long start1, start2, start3;
        double min1 = INF;
        double min2 = INF;
        double min3 = INF;
        for (auto& path : Footways) {
            for (auto& node : path.Nodes) {
                Coordinates nodeCoord = Nodes.at(node);
                double dist1 = distBetween2Points(building1.Coords.Lat, building1.Coords.Lon,
                    nodeCoord.Lat, nodeCoord.Lon);
                double dist2 = distBetween2Points(building2.Coords.Lat, building2.Coords.Lon,
                    nodeCoord.Lat, nodeCoord.Lon);
                double dist3 = distBetween2Points(building3.Coords.Lat, building3.Coords.Lon,
                    nodeCoord.Lat, nodeCoord.Lon);
                if (min1 > dist1) {
                    min1 = dist1;
                    start1 = node;
                }
                if (min2 > dist2) {
                    min2 = dist2;
                    start2 = node;
                }
                if (min3 > dist3) {
                    min3 = dist3;
                    start3 = node;
                }
            }
        }
        // Print locations of 3 people
        cout << "Person 1's point:" << endl;
        cout << " " << building1.Fullname << endl;
        cout << " (" << building1.Coords.Lat << ", "
            << building1.Coords.Lon << ")" << endl;

        cout << "Person 2's point:" << endl;
        cout << " " << building2.Fullname << endl;
        cout << " (" << building2.Coords.Lat << ", "
            << building2.Coords.Lon << ")" << endl;

        cout << "Person 3's point:" << endl;
        cout << " " << building3.Fullname << endl;
        cout << " (" << building3.Coords.Lat << ", "
            << building3.Coords.Lon << ")" << endl;
        cout << endl;

        cout << "Nearest P1 node:" << endl;
        cout << " " << Nodes[start1].ID << endl;
        cout << " (" << Nodes[start1].Lat << ", "
            << Nodes[start1].Lon << ")" << endl;

        cout << "Nearest P2 node:" << endl;
        cout << " " << Nodes[start2].ID << endl;
        cout << " (" << Nodes[start2].Lat << ", "
            << Nodes[start2].Lon << ")" << endl;

        cout << "Nearest P3 node:" << endl;
        cout << " " << Nodes[start3].ID << endl;
        cout << " (" << Nodes[start3].Lat << ", "
            << Nodes[start3].Lon << ")" << endl;
        cout << endl;
        // Check weather there is a loop that cover 3 points
        map<long long, queueNode> map1
            = DijkstraSearch(G, start1, start2);
        map<long long, queueNode> map2
            = DijkstraSearch(G, start2, start3);
        map<long long, queueNode> map3
            = DijkstraSearch(G, start3, start1);

        if (map1.at(start2).dist == INF
            || map2.at(start3).dist == INF
            || map3.at(start1).dist == INF) {
            cout << "There is no loop that goes through " << building1.Fullname;
            cout << ", " << building2.Fullname;
            cout << ", and " << building3.Fullname << endl;
        } else {
            cout << "There is a loop that goes through " << building1.Fullname;
            cout << ", " << building2.Fullname;
            cout << ", and " << building3.Fullname << endl;
        }
        //
        // another navigation?
        //
        cout << endl;
        cout << "Enter person 1's building "
            << "(partial name or abbreviation), or #> ";
        getline(cin, person1Building);
    }
}

int main() {
    // maps a Node ID to it's coordinates (lat, lon)
    map<long long, Coordinates>  Nodes;
    // info about each footway, in no particular order
    vector<FootwayInfo>          Footways;
    // info about each building, in no particular order
    vector<BuildingInfo>         Buildings;
    XMLDocument                  xmldoc;

    cout << "** Navigating UIC open street map **" << endl;
    cout << endl;
    cout << std::setprecision(8);

    string def_filename = "map.osm";
    string filename;

    cout << "Enter map filename> ";
    getline(cin, filename);

    if (filename == "") {
        filename = def_filename;
    }

    //
    // Load XML-based map file
    //
    if (!LoadOpenStreetMap(filename, xmldoc)) {
        cout << "**Error: unable to load open street map." << endl;
        cout << endl;
        return 0;
    }

    //
    // Read the nodes, which are the various known positions on the map:
    //
    int nodeCount = ReadMapNodes(xmldoc, Nodes);

    //
    // Read the footways, which are the walking paths:
    //
    int footwayCount = ReadFootways(xmldoc, Footways);

    //
    // Read the university buildings:
    //
    int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

    //
    // Stats
    //
    assert(nodeCount == (int)Nodes.size());
    assert(footwayCount == (int)Footways.size());
    assert(buildingCount == (int)Buildings.size());

    cout << endl;
    cout << "# of nodes: " << Nodes.size() << endl;
    cout << "# of footways: " << Footways.size() << endl;
    cout << "# of buildings: " << Buildings.size() << endl;


    graph<long long, double> G;
    for (auto& node : Nodes) {
        G.addVertex(node.first);
    }
    // MS6: Add edges based on footways
    for (auto& vertex : Footways) {
        for (int i = 0; i < vertex.Nodes.size() - 1; i++) {
            Coordinates c1;
            Coordinates c2;
            c1 = Nodes.at(vertex.Nodes[i]);
            c2 = Nodes.at(vertex.Nodes[i + 1]);
            double distance;
            distance = distBetween2Points(c1.Lat, c1.Lon, c2.Lat, c2.Lon);
            G.addEdge(c1.ID, c2.ID, distance);
            G.addEdge(c2.ID, c1.ID, distance);
        }
    }
    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;

    string userInput;
    cout << "Enter \"a\" for the standard application or "
        << "\"c\" for the creative component application> ";
    getline(cin, userInput);
    if (userInput == "a") {
        // TO DO: add argument for the graph you make.
        application(Nodes, Footways, Buildings, G);
    }
    else if (userInput == "c") {
        creative(Nodes, Footways, Buildings, G);
    }
    //
    // done:
    //
    cout << "** Done **" << endl;
    return 0;
}
