// Datastructures.cc
//
// Student name: Maral Nourimand
// Student email: maral.nourimand@tuni.fi
// Student number: 151749113

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <QDebug>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)


Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations.size();
}

void Datastructures::clear_all()
{
    affiliations.clear();
    aff_coord_vector.clear();
    aff_name_vector.clear();
    coord_map.clear();
    publications.clear();
    references.clear();
    connections.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> aff_vector;
    // to reserve memory and speed up mempry allocation
    aff_vector.reserve(affiliations.size());
    for (const auto& a: affiliations){
        aff_vector.push_back(a.id);
    }
    return aff_vector;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    // check if id is unique
    // Find the item in the set
    auto it = search_for_affiliation(id);
    if (it == affiliations.end()) {
        affiliations.insert({id, name, xy});
        coord_map[xy] = {id, name, xy};
        aff_coord_vector.push_back({id,xy});
        aff_name_vector.push_back({id, name});
        return true;
    }

    return false;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    // check if the id exists
    auto it = search_for_affiliation(id);
    if (it != affiliations.end()) {
        return it->name;
    } else {
        return NO_NAME;
    }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    // check if the id exists
    auto it = search_for_affiliation(id);
    if (it != affiliations.end()) {
        return it->coord;
    } else {
        return NO_COORD;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    std::vector<AffiliationID> result;

    std::sort(aff_name_vector.begin(), aff_name_vector.end(),
              [](const auto& a, const auto& b) {
                  return a.second < b.second;});

    for(auto it=aff_name_vector.begin(); it != aff_name_vector.end(); ++it){
        result.push_back(it->first);
    }

    return result;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    std::vector<AffiliationID> result;

    std::sort(aff_coord_vector.begin(),aff_coord_vector.end(),
               [this](const auto &a, const auto &b) {
                      return compare_distance(a.second, b.second);
               });
    for(auto it=aff_coord_vector.begin(); it != aff_coord_vector.end(); ++it){
        result.push_back(it->first);
    }
    return result;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    std::map<Coord, Affiliation>::iterator it = coord_map.find(xy);
    if (it == coord_map.end()){
        return NO_AFFILIATION;
    }
    return coord_map[xy].id;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = search_for_affiliation(id);
    Coord old_coord;
    Name name;
    if (it != affiliations.end()){
        old_coord = it->coord;
        name = it->name;
        affiliations.erase(it);
        add_affiliation(id, name, newcoord);
        coord_map.erase(old_coord);

        std::pair<AffiliationID, Coord> search = {id, old_coord};
        auto ite = find(aff_coord_vector.begin(), aff_coord_vector.end(), search);
        aff_coord_vector.erase(ite);
        return true;
    }
    return false;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    // check if the publication id exists
    auto it = search_for_publication(id);
    if (it == publications.end()) {
        publications.insert({id, name, year, affiliations});

        // Update the connections set and connection_weights map with the new publication
        update_connections(affiliations);

        return true;
    } else {
        return false;
    }
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector< PublicationID > v;

    std::for_each(publications.begin(), publications.end(),
                  [&v](const Publication&p){v.push_back(p.id);});

    //for(const Publication &p:publications){
    //    v.push_back(p.id);
    //}
    return v;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    // check if publication exists
    auto it = search_for_publication(id);
    if (it == publications.end()) {
        return NO_NAME;
    } else {
        return it->name;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    // check if publication exists
    auto it = search_for_publication(id);
    if (it == publications.end()) {
        return NO_YEAR;
    }

    return it->year;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    // check if publication exists
    auto it = search_for_publication(id);
    if (it == publications.end()) {
        return {NO_AFFILIATION};
    }

    return it->affiliations;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    // check if publications exist
    auto it1 = search_for_publication(id);
    auto it2 = search_for_publication(parentid);

    if (it1 == publications.end() or it2 == publications.end()){
        return false;
    }
    references[parentid].push_back(id);  // unordered_map[prentid]= vector of ids
    return true;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    // check if publications exist
    auto it = search_for_publication(id);
    if (it == publications.end()){
        return {NO_PUBLICATION};
    }
    return references[id];
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    // check if affiliation or publication exist
    auto it_aff = search_for_affiliation(affiliationid);
    auto it = search_for_publication(publicationid); // constant iterator

    // no such a affiliation or publication exists
    if (it == publications.end() or it_aff == affiliations.end()){
        return false;
    }
    // affiliations is a vector inside Publication
    auto& publication = const_cast<Publication&>(*it);  // Remove const-ness.O(1)
    publication.affiliations.push_back(affiliationid);

    // update connections
    // update_connections(publication.affiliations);    
    auto before_end = publication.affiliations.end() ;
    --before_end ;
    Distance dist;
    for (auto it1 = publication.affiliations.begin(); it1 != before_end; ++it1){
        // Use std::pair to represent the connection
        auto connection1 = std::make_pair(*it1, affiliationid);
        auto connection2 = std::make_pair(affiliationid, *it1);

        int x1 = get_affiliation_coord(*it1).x;
        int x2 = get_affiliation_coord(affiliationid).x;
        int y1 = get_affiliation_coord(*it1).y;
        int y2 = get_affiliation_coord(affiliationid).y;

        dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) ;

        // Find the corresponding connection's weight in the map
        auto weight_it = connections.find(connection1);
        if (weight_it != connections.end()) {
            // Increase the weight for the common publication
            ++weight_it->second.weight;
        } else {
            // If the connection doesn't exist, add a new connection with weight 1
            connections.insert({connection1, {*it1, affiliationid, 1, dist}});
        }
        weight_it = connections.find(connection2);
        if (weight_it != connections.end()) {
            // Increase the weight for the common publication
            ++weight_it->second.weight;
        } else {
            // If the connection doesn't exist, add a new connection with weight 1
            connections.insert({connection2, {affiliationid, *it1, 1, dist}});
        }
    }

    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    // check if affiliation exists
    auto it_aff = search_for_affiliation(id);
    if (it_aff == affiliations.end()){
        return {NO_PUBLICATION};
    }

    std::vector<PublicationID> v;
    for(auto const &p:publications){
        auto it = find(p.affiliations.begin(), p.affiliations.end(), id);
        if (it != p.affiliations.end()){
            v.push_back(p.id);
        }
    }
    return v;
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    for (auto const &r:references){
        if(std::find(r.second.begin(), r.second.end(), id)
                != r.second.end()){
            return r.first;
        }
    }
    return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    auto it_aff = search_for_affiliation(affiliationid);
    if (it_aff == affiliations.end()){
        return {{NO_YEAR, NO_PUBLICATION}};
    }

    std::vector<std::pair<Year, PublicationID>> v;
    for(const auto &p:publications){
        if(p.year >= year and
           std::find(p.affiliations.begin(), p.affiliations.end(),
                     affiliationid) != p.affiliations.end()){
            v.push_back({p.year, p.id});
        }
    }

    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        // Compare by Year first
        if (a.first < b.first) {
            return true;
        } else if (a.first > b.first) {
            return false;
        }
        // If Years are equal, compare by PublicationID
        return a.second < b.second;
    });

    return v;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    auto it_pub = search_for_publication(id);
    if (it_pub == publications.end()){
        return {NO_PUBLICATION};
    }

    std::vector<PublicationID> v;
    PublicationID chain = get_parent(id);
    while (chain != NO_PUBLICATION) {
        v.push_back(chain);
        chain = get_parent(chain);  // find its parent
    }
    return v;
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    auto it_pub = search_for_publication(id);
    if (it_pub == publications.end()) {
        return {NO_PUBLICATION};
    }

    // Initialize the result vector
    std::vector<PublicationID> result;

    // Start traversing the reference chain
    traverse_references(id, result);

    return result;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    // a copy of aff_coord vector = all coords x-x0, y-y0
    std::vector<std::pair<AffiliationID, Coord>> dist_vector;
    for(const auto &v : aff_coord_vector){
        Coord distance = {v.second.x-xy.x , v.second.y-xy.y};
        dist_vector.push_back({v.first, distance});
    }

    // sort the distance vector
    std::sort(dist_vector.begin(), dist_vector.end(), [this]
              (const auto &a, const auto &b) {
                return compare_distance(a.second, b.second);});

    std::vector<AffiliationID> result;

    for (const auto &pair : dist_vector)
    {
        result.push_back(pair.first);
        // Break the loop if you have three affiliations
        if (result.size() == 3)
            break;
    }

    return result;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    // if such an AffiliationId does not exist
    auto ita = search_for_affiliation(id);
    if (ita == affiliations.end()){
        return false;
    }

    // get all publications of an affiliation
    std::vector<PublicationID> pubs = get_publications(id);

    for (auto &p:pubs){
        auto it = search_for_publication(p);
        Name name = it->name ;
        Year year = it->year;
        std::vector<AffiliationID> copy_v(it->affiliations.begin(), it->affiliations.end());

        // find the position of this affiliation in the vector
        auto ite = std::find(copy_v.begin(), copy_v.end(), id);

        // remove the affiliation from the vector
        copy_v.erase(ite);

        // remove this publication and then insert the newer version into the set
        publications.erase(it);
        add_publication(p, name, year, copy_v);
    }

    Coord coord = ita->coord;
    affiliations.erase(ita); // remove from affiliatons set
    coord_map.erase(coord); // remove from coord_map

    // Remove from aff_coord_vector
    aff_coord_vector.erase(
        std::remove_if(
            aff_coord_vector.begin(),
            aff_coord_vector.end(),
            [id](const std::pair<AffiliationID, Coord>& element) {
                return element.first == id;
            }
        ),
        aff_coord_vector.end()
    );

    // Remove from aff_name_vector
    aff_name_vector.erase(
        std::remove_if(
            aff_name_vector.begin(),
            aff_name_vector.end(),
            [id](const std::pair<AffiliationID, Name>& element) {
                return element.first == id;
            }
        ),
        aff_name_vector.end()
    );

    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    // Check if either ID doesn't match any publication
    auto iter1 = search_for_publication(id1);
    auto iter2 = search_for_publication(id2);

    if (iter1 == publications.end() or iter2 == publications.end()){
        return NO_PUBLICATION ;
    }

    // Get the ancestors of each publication
    std::vector<PublicationID> ancestors1 = get_referenced_by_chain(id1);
    std::vector<PublicationID> ancestors2 = get_referenced_by_chain(id2);

    auto it1 = ancestors1.begin();
    auto it2 = ancestors2.begin();

    while (it1 != ancestors1.end() and it2 != ancestors2.end()) {
        if (*it1 == *it2){
            return *it1;
        }
        ++it1;
        ++it2;
    }


    // Iterate through ancestors1 and check for common ancestor
    it1 = ancestors1.begin();
    while (it1 != ancestors1.end()) {
        auto commonAncestor = std::find(ancestors2.begin(), ancestors2.end(), *it1);
        if (commonAncestor != ancestors2.end()) {
            return *it1;
        }
        ++it1;
    }

    // Iterate through ancestors2 and check for common ancestor
    it2 = ancestors2.begin();
    while (it2 != ancestors2.end()) {
        auto commonAncestor = std::find(ancestors1.begin(), ancestors1.end(), *it2);
        if (commonAncestor != ancestors1.end()) {
            return *it2;
        }
        ++it2;
    }

    return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    // check if publication does not exist
    Publication search_publication2;
    search_publication2.id = publicationid;
    auto iter = publications.find(search_publication2);

    if (iter == publications.end()){
        return false ;
    }

    // remove from publications
    publications.erase(iter);

    // remove from reference as a key
    if (references.find(publicationid) != references.end()){
        references.erase(publicationid);
    }

    // travers reference, find publication in value and remove it
    for (auto &r : references){
        auto it = find(r.second.begin(), r.second.end(), publicationid);
        if (it != r.second.end()){
            r.second.erase(it);
        }
    }

    return true;
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    // Replace the line below with your implementation
    //throw NotImplemented("get_connected_affiliations()");
    std::vector<Connection> result;

    // Check if the source affiliation exists
    if (search_for_affiliation(id) == affiliations.end()) {
        // Affiliation with the given ID does not exist
        return result;
    }

    // Iterate through the connections and find those where the given ID is the source
    for (const auto& connection : connections) {
        if (connection.first.first == id) {
            // Found a connection where the given ID is the source
            result.push_back(connection.second);
        }
    }

    return result;
}

std::vector<Connection> Datastructures::get_all_connections()
{
    std::vector<Connection> result;
    result.reserve(connections.size()/2);

    // loop through all connections, save only if aff1 < aff2
    for(const auto &c:connections){
        if (c.second.aff1 < c.second.aff2){
            result.push_back(c.second);
        }
    }

    return result;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    std::vector<Connection> result;

    // if the source or target affiliations do not exist
    if (search_for_affiliation(source) == affiliations.end() or
        search_for_affiliation(target) == affiliations.end()) {
        //qDebug() << "Source or target affiliation does not exist.";
        return result;  // Source or target affiliation does not exist
    }

    return recursive_path_finder(source, target, result);
}

Path Datastructures::get_path_with_least_affiliations(AffiliationID source, AffiliationID target)
{
    // Check if the source or target affiliations do not exist
    if (search_for_affiliation(source) == affiliations.end() ||
        search_for_affiliation(target) == affiliations.end()) {
        return {}; // Source or target affiliation does not exist
    }

    // Initialize the distance vector to track the number of affiliations to reach each node
    std::unordered_map<AffiliationID, int> distance;
    for (const auto& aff : affiliations) {
        distance[aff.id] = std::numeric_limits<int>::max();
    }
    distance[source] = 0;

    // Priority queue to store {distance, node} pairs
    std::priority_queue<std::pair<int, AffiliationID>, std::vector<std::pair<int, AffiliationID>>, std::greater<>> pq;
    pq.push({0, source});

    // Parent map to reconstruct the path
    std::unordered_map<AffiliationID, Connection> parent;

    while (!pq.empty()) {
        auto [dist, current] = pq.top();
        pq.pop();

        // Break the loop when reaching the target
        if (current == target) {
            break;
        }

        for (const auto& connection : get_connected_affiliations(current)) {
            int new_distance = distance[current] + 1;
            if (new_distance < distance[connection.aff2]) {
                distance[connection.aff2] = new_distance;
                pq.push({new_distance, connection.aff2});
                parent[connection.aff2] = connection;
            }
        }
    }

    // Reconstruct the path
    std::vector<Connection> result;
    AffiliationID current = target;
    while (parent.find(current) != parent.end()) {
        result.push_back(parent[current]);
        current = parent[current].aff1;
    }

    // Reverse the result to have connections from source to target
    std::reverse(result.begin(), result.end());

    return result;
}

Path Datastructures::get_path_of_least_friction(AffiliationID source, AffiliationID target)
{
    // Check if the source or target affiliations do not exist
    if (search_for_affiliation(source) == affiliations.end() ||
        search_for_affiliation(target) == affiliations.end()) {
        return {}; // Source or target affiliation does not exist
    }

    // very simple and dummy way
    std::vector<Connection> anc;
    std::vector<Path> all_paths = recursive_all_paths_finder(source, target, anc);

    std::vector<Connection> best_path;
    std::vector<Connection> temp;

    for (auto it1 = all_paths.begin(); it1 != all_paths.end(); it1++){
        for (auto it2 = all_paths.begin(); it2 != all_paths.end(); it2++){
            temp = compare_path_by_friction(*it1, *it2);
            best_path = compare_path_by_friction(temp, best_path);
        }
    }

    return best_path;

/*
    std::priority_queue<std::pair<double, std::vector<Connection>>, std::vector<std::pair<double, std::vector<Connection>>>, std::greater<>> pq;

    // Initialize distances and paths
    std::unordered_map<AffiliationID, double> distances;
    std::unordered_map<AffiliationID, std::vector<Connection>> paths;

    distances[source] = 0;
    pq.push({0, {}});

    // Dijkstra's algorithm
    while (!pq.empty()) {
        auto [friction, path] = pq.top();
        pq.pop();

        AffiliationID current = path.empty() ? source : path.back().aff2;

        // Check if the current path reaches the target
        if (current == target) {
            // Compare the current path with the existing paths based on friction
            //for (const auto& existingPath : paths[target]) {
            //    path = compare_path_by_friction(path, existingPath);
            //}
            path = compare_path_by_friction(path, paths[target]);
            paths[target] = path; // Update the best path to the target
        }

        // Explore neighbors
        for (const auto &neighbor : get_connected_affiliations(current)) {
            double new_friction = 1.0 / neighbor.weight;
            double total_friction = friction + new_friction;

            // Update distance and path if a shorter path is found
            if (!distances.count(neighbor.aff2) || total_friction < distances[neighbor.aff2]) {
                distances[neighbor.aff2] = total_friction;
                auto new_path = path;
                new_path.push_back(neighbor);
                pq.push({total_friction, new_path});
            }
        }
    }

    return paths[target]; // Return the path with the lowest friction to the target
*/

}

PathWithDist Datastructures::get_shortest_path(AffiliationID source, AffiliationID target)
{
    // Replace the line below with your implementation
    //throw NotImplemented("get_shortest_path()");
    if (search_for_affiliation(source) == affiliations.end() ||
        search_for_affiliation(target) == affiliations.end()) {
        return {}; // Source or target affiliation does not exist
    }

/*
    // very simple and dummy way
    //std::vector<Connection> best_path;

    std::vector<Connection> anc;
    std::vector<Path> all_paths = recursive_all_paths_finder(source, target, anc);

    std::map<Distance, Path> dist_path_map;
    Distance total_dist;

    for( auto &path:all_paths){
        total_dist = 0;
        for(auto &connection:path){
            total_dist += connection.distance;
        }
        dist_path_map[total_dist] = path;
    }
    Distance least_dist = (*dist_path_map.begin()).first;

    std::vector<std::pair<Connection, Distance>> result;
    result.reserve(dist_path_map.size());
    for (auto &c:dist_path_map[least_dist]){
        result.push_back({c, c.distance});
    }

    return result;
*/

    // with A* algorithm
    std::priority_queue<std::pair<Distance, PathWithDist>,
            std::vector<std::pair<Distance, PathWithDist>>, std::greater<>> open_list;

    //std::vector<Connection> from_source = get_connected_affiliations(source);

    Connection source_c;
    source_c.aff2 = source;

    open_list.push({0, {{source_c, 0}}});

    std::set<AffiliationID> closed_set;

    std::vector<std::pair<Connection, Distance>> result;

    Distance current_dist;
    PathWithDist current;
    while(!open_list.empty()){
        current_dist = open_list.top().first;
        current = open_list.top().second;
        open_list.pop();
        closed_set.insert(current.at(current.size()-1).first.aff2);

        if (current.at(current.size()-1).first.aff2 == target){
            break;
        }

        std::vector<Connection> children = get_connected_affiliations
                (current.at(current.size()-1).first.aff2);

        for (auto &child:children){
            if(closed_set.find(child.aff2) != closed_set.end()){
                continue;
            }
            //current_dist += child.distance;
            PathWithDist temp = current;
            temp.push_back({child, child.distance});
            open_list.push({current_dist+child.distance, temp});
        }

    }

    if (current.at(current.size()-1).first.aff2 == target){
        for(uint i = 1 ; i < current.size() ; i++){
            result.push_back(current.at(i));
        }
    }

    return result;
}

bool Datastructures::compare_distance(const Coord &a, const Coord &b)
{
    int dist_a_squared = a.x * a.x + a.y * a.y;
    int dist_b_squared = b.x * b.x + b.y * b.y;

    if (dist_a_squared == dist_b_squared) {
        return a.y < b.y;
    }

    return dist_a_squared < dist_b_squared;
}

void Datastructures::traverse_references(PublicationID current_id, std::vector<PublicationID> &result)
{
    // Get direct references
    const auto& direct_references = references[current_id];

    // Add direct references to the result
    result.insert(result.end(), direct_references.begin(), direct_references.end());

    // Recursively traverse the reference chain for each direct reference
    for (const PublicationID ref_id : direct_references) {
        traverse_references(ref_id, result);
    }
}

std::set<Affiliation>::iterator Datastructures::search_for_affiliation(AffiliationID id)
{
    Affiliation search_affiliation;
    // Set the search_affiliation's id to the desired value to find
    search_affiliation.id = id;
    auto it = affiliations.find(search_affiliation);  //O(logN)

    return it;
}

std::set<Publication>::iterator Datastructures::search_for_publication(PublicationID id)
{
    Publication search_publication;
    // Set the search_affiliation's id to the desired value to find
    search_publication.id = id;
    auto it = publications.find(search_publication);  //O(logN)

    return it;
}

void Datastructures::update_connections(const std::vector<AffiliationID> &affiliations)
{
    // Iterate through pairs of affiliations
    for (auto it1 = affiliations.begin(); it1 != affiliations.end(); ++it1) {
        for (auto it2 = it1 + 1; it2 != affiliations.end(); ++it2) {
            // Use std::pair to represent the connection
            auto connection1 = std::make_pair(*it1, *it2);
            auto connection2 = std::make_pair(*it2, *it1);

            int x1 = get_affiliation_coord(*it1).x;
            int x2 = get_affiliation_coord(*it2).x;
            int y1 = get_affiliation_coord(*it1).y;
            int y2 = get_affiliation_coord(*it2).y;

            Distance dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) ;

            // Find the corresponding connection's weight in the map
            auto weight_it = connections.find(connection1);
            if (weight_it != connections.end()) {
                // Increase the weight for the common publication
                //++weight_it->second.weight;
                (*weight_it).second.weight += 1;
            } else {
                // If the connection doesn't exist, add a new connection with weight 1
                connections.insert({connection1, {*it1, *it2, 1, dist}});
            }

            weight_it = connections.find(connection2);
            if (weight_it != connections.end()) {
                // Increase the weight for the common publication
                //++weight_it->second.weight;
                (*weight_it).second.weight += 1;
            } else {
                // If the connection doesn't exist, add a new connection with weight 1
                connections.insert({connection2, {*it2, *it1, 1, dist}});
            }

        }
    }
}
/*
std::vector<Connection> Datastructures::recursive_path_finder(AffiliationID source, AffiliationID target, std::vector<Connection> ancestors)
{
    if (source == target){
        return ancestors;
    }
    std::vector<Connection> children =  get_connected_affiliations(source);


    for (const auto &child:children){
        //bool found = false;
        auto it = find_if(ancestors.begin(), ancestors.end(), [&child](auto &a)
                    {return a.aff1 == child.aff2;});
        if (it != ancestors.end()){
            continue;
        }
//        for(const auto &a:ancestors){
//            if(a.aff1 == child.aff2){
//                found = true;
//                break;
//            }
//        }
//        if(found){
//            continue;
//        }
        ancestors.push_back(child);
        return recursive_path_finder(child.aff2, target, ancestors);
    }
    return {}; // if we never reach the target
}
*/
std::vector<Connection> Datastructures::compare_path_by_friction(std::vector<Connection> &a, std::vector<Connection> &b)
{
    std::vector<Weight> v_weight_a;
    std::vector<Weight> v_weight_b;

    if (a.empty()){
        return b;
    }
    if (b.empty()){
        return a;
    }

    for (uint i = 0; i < a.size() ; i++){
        v_weight_a.push_back(a.at(i).weight);
    }

    for (uint i = 0; i < b.size() ; i++){
        v_weight_b.push_back(b.at(i).weight);
    }

    auto min_a = std::min_element(v_weight_a.begin(), v_weight_a.end());
    auto min_b = std::min_element(v_weight_b.begin(), v_weight_b.end());
    if (*min_a > *min_b){
        return a;
    }
    if (*min_a == *min_b and a.size() < b.size()){
        return a;
    }
    return b;
}

std::vector<Path> Datastructures::recursive_all_paths_finder(AffiliationID source, AffiliationID target, std::vector<Connection> ancestors)
{
    std::set<AffiliationID> ancestors_set;
    for (const auto& ancestor : ancestors) {
        ancestors_set.insert(ancestor.aff1);
    }

    std::vector<std::vector<Connection>> all_paths;

    if (source == target) {
        all_paths.push_back(ancestors);
        return all_paths;
    }

    std::vector<Connection> children = get_connected_affiliations(source);

    for (const auto& child : children) {
        auto it = ancestors_set.find(child.aff2);
        if (it != ancestors_set.end()) {
            //qDebug() << "found in the set";
            continue;
        }

        //qDebug() << "Not found in the set";
        std::vector<Connection> new_ancestors = ancestors;
        new_ancestors.push_back(child);

        auto recursive_paths = recursive_all_paths_finder(child.aff2, target, new_ancestors);
        all_paths.insert(all_paths.end(), recursive_paths.begin(), recursive_paths.end());
    }

    return all_paths;
}


std::vector<Connection> Datastructures::recursive_path_finder(AffiliationID source, AffiliationID target, std::vector<Connection> ancestors)
{
    if (source == target){
        return ancestors;
    }

    std::vector<Connection> children =  get_connected_affiliations(source);


    for (const auto &child:children){
        bool found = false;
        for (auto &a:ancestors){
            if(a.aff1 == child.aff2){
                found = true;
                break;
            }
        }

        if (found){
            continue;
        }
        ancestors.push_back(child);
        std::vector<Connection> temp = recursive_path_finder(child.aff2, target, ancestors);
        if (!temp.empty()){
            return temp;
        }
        ancestors.pop_back(); //remove this ancestor, got to check next child
        //return recursive_path_finder(child.aff2, target, ancestors);
    }
    return {}; // if we never reach the target
}


