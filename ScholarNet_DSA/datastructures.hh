// Datastructures.hh
//
// Student name: Maral Nourimand
// Student email: maral.nourimand@tuni.fi
// AI tool used and the version: ChatGPT 3.5
// The purpose of the use: sometimes for debugging or improvement of minor errors.
// The part(s) where it was utilized: I don't recall exactly, but whenever I
//   faced an error that I couldn't figure out, I used ChatGPT to help debug.

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Type for Affiliation
struct Affiliation
{
    AffiliationID id;
    Name name;
    Coord coord;

    // Overload < operator based on id
    bool operator<(const Affiliation &other) const {
        return id < other.id;
    }
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}


// Type for Publication
struct Publication
{
    PublicationID id;
    Name name;
    Year year;
    std::vector<AffiliationID> affiliations;

    // Overload < operator based on id
    bool operator<(const Publication &other) const {
        return id < other.id;
    }
};

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: for function set.size()
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: from cppreference clear() complexity
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop goes through all the set elements.
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: major function call is find() which is
    // logarithmic. insert() is constant time on average.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate: because of sort function which is dominant
    // here with complexity nlogn.
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance:O(n*log(n))
    // Short rationale for estimate: because of sort function which is dominant
    // here with complexity nlogn.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: because of call function find() and
    // add_affiliaion() which are logarithmic.
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: because of call function find() and
    // insert() for set which are logarithmic but there is a vector./O(log(n))
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop goes through all the set elements.
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic but in return we have a vecture./O(log(n))
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: becuase of unordered_map::find and insertion
    // operation which are constant and n in worst case. Push_back in vector is
    // constant, too.
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: becuase of unordered_map::find and access
    // operation which are constant on average and n in worst case.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic.
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic and is called in for loop for all elements of the set./O(n * log(n))
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic and is called in for loop for all elements of the set./O(n * log(n))
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: beacuase of call function find() which is
    // logarithmic and is called in for loop for all elements of the set.
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: beacuase of call function get_parent()
    // which is n*logarithmic and is called in for loop.
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: because of for loop and also return vector.
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: because of vector implementation.
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: because of a for loop and call of erase and
    // find.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: because of vector implementation.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: because of a for loop and call of erase and
    // find.
    bool remove_publication(PublicationID publicationid);


private:
    std::set < Affiliation > affiliations ;
    std::vector <std::pair<AffiliationID, Coord>> aff_coord_vector;
    std::vector <std::pair<AffiliationID, Name>> aff_name_vector;
    std::map < Coord, Affiliation> coord_map ;
    std::set < Publication > publications ;
    std::unordered_map<PublicationID, std::vector<PublicationID>> references;

    bool compare_distance(Coord const &a, Coord const &b);
    void traverse_references(PublicationID current_id, std::vector<PublicationID> &result);
    std::set<Affiliation>::iterator search_for_affiliation(AffiliationID id);
    std::set<Publication>::iterator search_for_publication(PublicationID id);

};

#endif // DATASTRUCTURES_HH
