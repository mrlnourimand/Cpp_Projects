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

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after
(AffiliationID affiliationid, Year year)
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


