//
// Created by dkope on 15.12.2021.
//
#ifndef NET_SIMULATION_FACTORY_HPP
#define NET_SIMULATION_FACTORY_HPP


#include <types.hpp>
#include <nodes.hpp>
#include <sstream>
#include <iostream>
#include <regex>
template<typename Node>
class NodeCollection{
public:

    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;


    const_iterator cbegin() const { return container_.cbegin(); }
    const_iterator cend() const { return container_.cend(); }
    const_iterator begin() const { return container_.cbegin(); }
    const_iterator end() const { return container_.cend(); }
    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }

    void add(Node&& node) { container_.push_back(std::move(node)); };

    const_iterator find_by_id(ElementID id) const { return std::find_if(begin(), end(), [id](const auto& elem) { return (elem.get_id() == id);});}
    iterator find_by_id(ElementID id) { return std::find_if(begin(), end(), [id](auto& elem){ return (elem.get_id() == id);});}


    void remove_by_id(ElementID id);

private:
    container_t container_;
};

template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID id) {
    iterator it = find_by_id(id);
    if (it != end()){
        container_.erase(it);
    }
}

enum class NodeColor { UNVISITED, VISITED, VERIFIED };



class Factory{
public:

    void add_ramp(Ramp&& ramp) { ramp_collection_.add(std::move(ramp));}
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramp_collection_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramp_collection_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_collection_.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_collection_.cend();}





    void add_worker(Worker&& worker) { worker_collection_.add(std::move(worker));}
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_collection_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return worker_collection_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_collection_.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const { return worker_collection_.cend();}



    void add_storehouse(Storehouse&& storehouse) { storehouse_collection_.add(std::move(storehouse));}
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_collection_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_collection_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_collection_.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_collection_.cend();}


    bool is_consistent();

    void do_deliveries(Time t);

    void do_package_passing();

    void do_work(Time t);

private:
    NodeCollection<Worker> worker_collection_;
    NodeCollection<Ramp> ramp_collection_;
    NodeCollection<Storehouse> storehouse_collection_;

    template<typename Node>
    void remove_receiver(NodeCollection<Node>& collection , ElementID id );

};

template<typename Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id) {
    for(auto& node: collection){
        auto& map = node.receiver_preferences_.preferences_t_;
        auto ptr = std::find_if(std::begin(map), std::end(map),[id](auto& pair){ return pair.first->get_id() == id;} );
        auto ptr2 = ptr->first;
        node.receiver_preferences_.remove_receiver(ptr2);
    }
}

enum ElementType{
    LOADING_RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ElementType element_type_;
    std::map<std::string, std::string> parameters;
};



bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);
ParsedLineData parse_line(std::string line);
std::string enum_to_string_pqt(PackageQueueType type);
std::string enum_to_string_rt(ReceiverType type);
std::vector<IPackageReceiver*> find_receivers(const PackageSender& sender);
#endif //NET_SIMULATION_FACTORY_HPP
