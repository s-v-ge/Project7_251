// application.cpp <Starter Code>
// Zach Savignano
//
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Fall 2022
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//
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

#include "dist.h"
#include "graph.h"
#include "osm.h"
#include "tinyxml2.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip> /*setprecision*/
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace tinyxml2;

void application(map<long long, Coordinates> &Nodes,
                 vector<FootwayInfo> &Footways, vector<BuildingInfo> &Buildings,
                 graph<long long, double> &G) {
  string person1Building, person2Building;
  Coordinates temp1;
  Coordinates temp2;
  Coordinates mid;
  BuildingInfo dest;
  double minLat = 100000;
  double minLon = 100000;
  bool found1 = false;
  bool found2 = false;
  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);
  if (person1Building != "#") {
    for (size_t i = 0; i < Buildings.size() && !found1; i++) {
      if (Buildings.at(i).Abbrev == person1Building) {
        temp1 = Buildings.at(i).Coords;
        found1 = true;
        person1Building = Buildings.at(i).Fullname;
      } else if (Buildings.at(i).Fullname.substr(0, person1Building.size()) ==
                 person1Building) {
        temp1 = Buildings.at(i).Coords;
        found1 = true;
        person1Building = Buildings.at(i).Fullname;
      }
    }
  }
  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    //
    // TO DO: lookup buildings, find nearest start and dest nodes, find center
    // run Dijkstra's alg from each start, output distances and paths to
    // destination:
    //
    for (size_t i = 0; i < Buildings.size() && !found2; i++) {
      if (Buildings.at(i).Abbrev == person2Building) {
        temp2 = Buildings.at(i).Coords;
        found2 = true;
        person2Building = Buildings.at(i).Fullname;
      } else if (Buildings.at(i).Fullname.substr(0, person2Building.size()) ==
                 person2Building) {
        temp2 = Buildings.at(i).Coords;
        found2 = true;
        person2Building = Buildings.at(i).Fullname;
      }
    }
    if (!found1)
      cout << "Person 1's building not found" << endl;
    if (!found2)
      cout << "Person 2's building not found" << endl;
    if (found1 && found2) {
      cout << "Person 1's point:" << endl;
      cout << " " << person1Building << endl;
      cout << " (" << temp1.Lat << ", " << temp1.Lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << person2Building << endl;
      cout << " (" << temp2.Lat << ", " << temp2.Lon << ")" << endl;
      mid = centerBetween2Points(temp1.Lat, temp1.Lon, temp2.Lat, temp2.Lon);
      for (size_t i = 0; i < Buildings.size(); i++) {
        double latDiff = abs(mid.Lat - Buildings.at(i).Coords.Lat);
        double lonDiff = abs(mid.Lon - Buildings.at(i).Coords.Lon);
        if (latDiff < minLat && lonDiff < minLon) {
          dest = Buildings.at(i);
          minLon = lonDiff;
          minLat = latDiff;
        }
      }
      cout << "Destination Building:" << endl;
      cout << dest.Fullname << endl;
      cout << " (" << dest.Coords.Lat << ", " << dest.Coords.Lon << ")" << endl;

      cout << "Person 1's distance to dest: ";
      cout << distBetween2Points(temp1.Lat, temp1.Lon, dest.Coords.Lat,
                                 dest.Coords.Lon);
      cout << " miles" << endl;
      cout << "Person 2's distance to dest: ";
      cout << distBetween2Points(temp2.Lat, temp2.Lon, dest.Coords.Lat,
                                 dest.Coords.Lon);
      cout << " miles" << endl;
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}

int main() {
  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates> Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo> Footways;
  // info about each building, in no particular order
  vector<BuildingInfo> Buildings;
  XMLDocument xmldoc;

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
  // add vertices
  for (auto e : Nodes)
    G.addVertex(e.first);
  cout << "# of vertices: " << G.NumVertices() << endl;
  // add edges
  for (size_t i = 0; i < Footways.size(); i++) {
    for (size_t j = 0; j < Footways[i].Nodes.size() - 1; j++) {
      double dist = distBetween2Points(Nodes.at(Footways[i].Nodes[j]).Lat,
                                       Nodes.at(Footways[i].Nodes[j]).Lon,
                                       Nodes.at(Footways[i].Nodes[j + 1]).Lat,
                                       Nodes.at(Footways[i].Nodes[j + 1]).Lon);
      G.addEdge(Footways[i].Nodes[j], Footways[i].Nodes[j + 1], dist);
      G.addEdge(Footways[i].Nodes[j + 1], Footways[i].Nodes[j], dist);
    }
  }

  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  //
  // Menu
  //
  string userInput;
  cout << "Enter \"a\" for the standard application or "
       << "\"c\" for the creative component application> ";
  getline(cin, userInput);
  if (userInput == "a") {
    application(Nodes, Footways, Buildings, G);
  } else if (userInput == "c") {
    // TO DO: add arguments
    // creative();
  }
  //
  // done:
  //
  cout << "** Done **" << endl;
  return 0;
}
