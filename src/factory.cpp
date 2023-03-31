#include <factory.hpp>


#include <factory.hpp>



bool Factory::is_consistent() {

    std::map<const PackageSender*, NodeColor> node_colors;
    for(auto& node : ramp_collection_){
        const auto *node_ptr = &node;
        node_colors[node_ptr] = NodeColor::UNVISITED;
    }
    for(auto& node : worker_collection_){
        const auto *node_ptr = &node;
        node_colors[node_ptr] = NodeColor::UNVISITED;
    }
    try{
        for(const auto& ramp : ramp_collection_){
            has_reachable_storehouse(&ramp, node_colors);
        }
    }catch(const std::logic_error&){
        return false;
    }
    return true;
}


void Factory::remove_ramp(ElementID id) {
    remove_receiver(worker_collection_, id);
    remove_receiver(ramp_collection_, id);
    ramp_collection_.remove_by_id(id);
}


void Factory::remove_storehouse(ElementID id) {

    remove_receiver(worker_collection_, id);
    remove_receiver(ramp_collection_, id);
    storehouse_collection_.remove_by_id(id);
}
void Factory::remove_worker(ElementID id) {

    remove_receiver(worker_collection_, id);
    remove_receiver(ramp_collection_, id);
    worker_collection_.remove_by_id(id);
}


void Factory::do_deliveries(Time t) {
    for (auto& ramp: ramp_collection_){
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto& elem: ramp_collection_){
        elem.send_package();
    }
    for (auto& elem: worker_collection_){
        elem.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto& worker: worker_collection_){
        worker.do_work(t);
    }
}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if (node_colors.at(sender) == NodeColor::VERIFIED){
        return true;
    }
    node_colors.at(sender) = NodeColor::VISITED;

    if(sender->receiver_preferences_.preferences_t_.size() == 0){
        throw std::logic_error("Nadawca nie posiada żadnego połączenia wyjściowego.");
    }

    bool has_another_receiver_except_self = false;
    for(auto& pair : sender->receiver_preferences_.preferences_t_){
        if(pair.first->get_receiver_type() == ReceiverType::Storehouse){
            has_another_receiver_except_self = true;
        }else{
            IPackageReceiver* receiver_ptr = pair.first;
            const auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            const PackageSender* sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if(sendrecv_ptr == sender){
                continue;
            }
            has_another_receiver_except_self = true;
            if(node_colors.at(sendrecv_ptr) == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }

        }
    }
    node_colors.at(sender) = NodeColor::VERIFIED;
    if(has_another_receiver_except_self){
        return true;
    }else{
        throw std::logic_error("Nadawca nie posiada żadnego połączenia wyjściowego.");
    }
}


std::vector<IPackageReceiver*> find_receivers(const PackageSender& sender){
    std::vector<IPackageReceiver*> receivers;
    for (const auto& pair: sender.receiver_preferences_){
        receivers.push_back(pair.first);
    }
    return receivers;
}


std::string enum_to_string_pqt(PackageQueueType type){
    switch(type){
        case PackageQueueType::FIFO:
            return "FIFO";
        case PackageQueueType::LIFO:
            return "LIFO";
        default:
            return "";
    }
}
std::string enum_to_string_rt(ReceiverType type){
    switch(type){
        case ReceiverType::Worker:
            return "worker-";
        case ReceiverType::Storehouse:
            return "store-";
        default:
            return "";
    }
}

ParsedLineData parse_line(std::string line){

    const static std::map<std::string, ElementType> element_type_converter {
            {"LOADING_RAMP", ElementType::LOADING_RAMP},
            {"WORKER", ElementType::WORKER},
            {"STOREHOUSE", ElementType::STOREHOUSE},
            {"LINK", ElementType::LINK},
    };

    ParsedLineData parsed_line_data;

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(std::move(line));
    char delimiter = ' ';
    while(std::getline(token_stream, token, delimiter)){
        tokens.push_back(token);
    }
    auto type = tokens[0];
    if(type != "LINK" and type!= "STOREHOUSE" and type!= "WORKER" and type != "LOADING_RAMP"){
        throw std::logic_error("Linia zaczyna się od złego tagu");
    }
    tokens.erase(tokens.begin());
    parsed_line_data.element_type_ = element_type_converter.at(type);
    char delimiter2 = '=';
    std::vector<std::string> to_map_converter;
    for(const auto& token_str : tokens){
        to_map_converter.clear();
        std::istringstream token_stream2(token_str);
        std::string token2;
        while(std::getline(token_stream2, token2, delimiter2)){
            to_map_converter.push_back(token2);
        }
        parsed_line_data.parameters[to_map_converter[0]] = to_map_converter[1];
        token_stream2.clear();
    }
    return parsed_line_data;
}
