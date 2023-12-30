# CampusMap

##### _This project is part of the UIC's [CS 251](https://www.cs.uic.edu/~i251/) (Data Structures) class, by [Professor Joseph Hummel](https://cs.uic.edu/profiles/joe-hummel/)._

CampusMap is a console app that helps the user navigate UIC's campus. The project uses Dijkstra’s algorithm to find the shortest weighted path from a given starting location to a given ending location. This project is based on [OpenStreetMap](https://www.openstreetmap.org/) to navigate through the UIC campus, and uses [TinyXML](http://www.grinninglizard.com/tinyxml/) to parse the `.xml` files.

## Background

We’re all familiar with navigation apps.  While we don’t have the ability to display the results graphically, we can at least perform the back-end operations of loading the map, building the graph, and computing the shortest weighted path between two points.  In our case we’re going to navigate between UIC buildings on the East campus, using the footpaths.  But the foundation is there to extend the program to do more general navigation between any two points.

We are working with open-source maps from https://www.openstreetmap.org/.  Browse to the site and type “UIC” into the search field, and then click on the first search result.  You’ll see the East campus highlighted.  Notice the “export” button — we used this button to download the map file (map.osm) we’ll be working with.

Zoom in.  We’re going to focus on two features of a map:  “Nodes” and “Ways”.  A node is a point on the map, consisting of 3 values:  id, latitude, and longitude.  These are shown as red dots (there are thousands more).  A way is a series of nodes that define something.  The two most important examples in our case are buildings and footways.  In the screenshot to the right, the buildings are labeled and the footways are the dashed lines.  For a building, the nodes define the building’s perimeter.  For a footway, the nodes define the endpoints of the footway, but might also include intermediate points along the way (especially if the footway is not a straight line).  More details of openstreetmap are available on Wikipedia.

The assignment is to write a console-based C++ program to input a campus map (e.g. UIC’s East campus) and navigate between buildings via footways.  The program should be general enough to work with any college map, though we don’t plan to extensively test this.  Given the time constraints, we’re going to provide helper functions to read the map for you, which are available in XML format.  Your job is to build the underlying graph, input two buildings from the user, and then use Dijkstra’s algorithm to find the shortest weighted path.  This is repeated until the user enters # for the start building.  Here are the main program steps:

1. Load map into xmldoc.  
2. Read nodes.  
3. Read footways.  
4. Read buildings.  
5. Add nodes as vertices.  
6. Add edges based on footways.  
7. Input start and destination buildings, locate on map.  
8. Search the footways and find the nearest nodes to the start and destination buildings; these become the “start” and “dest” nodes.  
9. Run Dijkstra’s algorithm from the start node.  
10. Output the distance and path from start node to destination node. If no path exists,  output “Sorry, destination unreachable”.  
11. Repeat with another pair of buildings.  

## Working with the project

### Clone the project

```
git clone https://github.com/ayaanqui/uic-campus-map.git
```

### Compile the project

#### Linux and Mac

```
make build
```

#### Windows

```
g++ -O2 -std=c++11 -Wall main.cpp dist.cpp osm.cpp tinyxml2.cpp -o program.exe
```

_Ignore warnings._ This will create a new file in your local project directory, named `program.exe`

### Run program

#### Linux and Mac

```
make run
```

#### Windows

```
.\program.exe
```

Once the program starts input `map.osm` as the file name
