//
// Created by dkope on 15.12.2021.
//

#include <reports.hpp>



void sort(std::map<int, std::string>& M)
{

    std::vector<std::pair<int, std::string>> A;

    for (auto& it : M) {
        A.push_back(it);
    }

    std::sort(A.begin(), A.end(), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){return a.first < b.first;});

}

void generate_structure_report(const Factory& f, std::ostream& os) {


    os << "\n== LOADING RAMPS ==\n\n";

    std::string ramp_str;
    std::map<ElementID, std::string> ramp_map;

    for(auto ramp_it = f.ramp_cbegin(); ramp_it!= f.ramp_cend(); ramp_it++){
//        std::map<ElementID, std::string> ramp_receivers_map;
        std::vector<std::pair<ElementID, std::string>> ramp_receivers_vector;
        std::string ramp_receivers_string;
        ramp_str.clear();
        ElementID ramp_id = ramp_it->get_id();
        ramp_str += "LOADING RAMP #" + std::to_string(ramp_id) + "\n";
        ramp_str += "  Delivery interval: " + std::to_string(ramp_it->get_delivery_interval()) + "\n";
        ramp_str += "  Receivers:\n";
        for(std::pair<IPackageReceiver* const, double> preference_pair : ramp_it->receiver_preferences_.preferences_t_){
            ramp_receivers_string.clear();
            if(preference_pair.first->get_receiver_type() == ReceiverType::Worker){
                ramp_receivers_string = "    worker #" + std::to_string(preference_pair.first->get_id()) + "\n";
            }
            else if(preference_pair.first->get_receiver_type() == ReceiverType::Storehouse){
                ramp_receivers_string = "    storehouse #" + std::to_string(preference_pair.first->get_id()) + "\n";
            }

            std::pair<ElementID, std::string> worker_receiver_pair = {preference_pair.first->get_id(), ramp_receivers_string};
            ramp_receivers_vector.push_back(worker_receiver_pair);

        }

        std::sort(ramp_receivers_vector.begin(), ramp_receivers_vector.end(), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){return a.second[4] < b.second[4] ;});
        std::sort(ramp_receivers_vector.begin(), ramp_receivers_vector.end(), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){
            if (a.second[4] == b.second[4]){
                return a.first < b.first;
            }else{
                return false;
            }
        });
        for(auto& pair : ramp_receivers_vector){
            ramp_str += pair.second;
        }
        ramp_map.emplace(ramp_id, ramp_str);
    }

    sort(ramp_map);

    for(auto ramp_pair_ptr = ramp_map.begin(); ramp_pair_ptr != ramp_map.end(); ramp_pair_ptr++){
        os << ramp_pair_ptr->second << "\n";
    }

    os << "\n== WORKERS ==\n\n";

    std::string worker_str;
    std::map<ElementID, std::string> worker_map;

    for(auto worker_it = f.worker_cbegin(); worker_it!= f.worker_cend(); worker_it++){


        std::vector<std::pair<ElementID, std::string>> worker_receivers_vector;

        std::string worker_receivers_string;
        worker_str.clear();
        ElementID worker_id = worker_it->get_id();
        worker_str += "WORKER #" + std::to_string(worker_id) + "\n";
        worker_str += "  Processing time: " + std::to_string(worker_it->get_processing_duration()) + "\n";
        worker_str += "  Queue type: " + enum_to_string_pqt(worker_it->get_queue()->get_queue_type())+ "\n";
        worker_str += "  Receivers:\n";
        for(std::pair<IPackageReceiver* const, double> preference_pair : worker_it->receiver_preferences_.preferences_t_){
            worker_receivers_string.clear();
            if(preference_pair.first->get_receiver_type() == ReceiverType::Worker){
                worker_receivers_string = "    worker #" + std::to_string(preference_pair.first->get_id()) + "\n";
            } else{
                worker_receivers_string = "    storehouse #" + std::to_string(preference_pair.first->get_id()) + "\n";
            }
            std::pair<ElementID, std::string> worker_receiver_pair = {preference_pair.first->get_id(), worker_receivers_string};

            worker_receivers_vector.push_back(worker_receiver_pair);
        }

        std::sort(worker_receivers_vector.begin(), worker_receivers_vector.end(), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){return a.second[4] < b.second[4] ;});
        std::sort(worker_receivers_vector.begin(), worker_receivers_vector.end(), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){
            if (a.second[4] == b.second[4]){
                return a.first < b.first;
            }else{
                return false;
            }
        });
        for(auto& pair : worker_receivers_vector){
            worker_str += pair.second;
        }
        worker_map.emplace(worker_id, worker_str);
    }

    sort(worker_map);

    for(auto worker_pair_ptr = worker_map.begin(); worker_pair_ptr != worker_map.end(); worker_pair_ptr++){
        os << worker_pair_ptr->second << "\n";
    }
    worker_str.clear();

    os << "\n== STOREHOUSES ==\n\n";
    std::string storehouse_str;
    std::map<ElementID, std::string> storehouse_map;
    for(auto storehouse_it = f.storehouse_cbegin(); storehouse_it!= f.storehouse_cend(); storehouse_it++){

        storehouse_str.clear();
        ElementID storehouse_id = storehouse_it->get_id();
        storehouse_str += "STOREHOUSE #" + std::to_string(storehouse_id) + "\n";
        storehouse_map.emplace(storehouse_id, storehouse_str);
    }

    sort(storehouse_map);
    for(auto storehouse_pair_ptr = storehouse_map.begin(); storehouse_pair_ptr != storehouse_map.end(); storehouse_pair_ptr++){
        os << storehouse_pair_ptr->second << "\n";
    }
    os.flush();
}


void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {
    os << "=== [ Turn: "<< std::to_string(t) << " ] ===\n";

    os << "\n== WORKERS ==\n\n";

    std::string worker_str;
    std::map<ElementID, std::string> worker_map;

    for(auto worker_it = f.worker_cbegin(); worker_it!= f.worker_cend(); worker_it++){


        std::vector<std::pair<ElementID, std::string>> worker_receivers_vector;

        std::string worker_receivers_string;
        worker_str.clear();
        ElementID worker_id = worker_it->get_id();
        worker_str += "WORKER #" + std::to_string(worker_it->get_id()) + "\n";
        if (worker_it->get_processing_buffer().has_value()){
            worker_str += "  PBuffer: #" + std::to_string(worker_it->get_processing_buffer()->get_id()) + " (pt = " + std::to_string(t - worker_it->get_package_processing_start_time() + 1) + ")\n";
        }else{
            worker_str += "  PBuffer: (empty)\n";
        }

        worker_str += "  Queue: ";
        std::vector<Package> pckg;
        for (auto it = worker_it->get_queue()->begin(); it != worker_it->get_queue()->end(); ++it){
            pckg.push_back(it->get_id());
        }
        if (pckg.begin() != pckg.end()){
            for (auto iter = pckg.begin(); iter != pckg.end(); ++iter){
                if (iter + 1 == pckg.end()){
                    worker_str += "#" + std::to_string(iter->get_id()) + "\n";    // tutaj jest ostatni element kolejki ??czemu musze robic iter->get_id(), a nie iter
                }else{
                    worker_str += "#" + std::to_string(iter->get_id()) + ",";
                }
            }
        }else{
            worker_str += "(empty)\n";
        }
        worker_str += "  SBuffer: ";
        if (worker_it->get_sending_buffer().has_value()){
            worker_str += "#" + std::to_string(worker_it->get_sending_buffer()->get_id()) + "\n";
        }else{
            worker_str += "(empty)\n";
        }
        worker_map.emplace(worker_id, worker_str);
    }

    sort(worker_map);

    for(auto worker_pair_ptr = worker_map.begin(); worker_pair_ptr != worker_map.end(); worker_pair_ptr++){
        os << worker_pair_ptr->second << "\n";
    }

    os << "\n== STOREHOUSES ==\n\n";
    std::string storehouse_str;
    std::map<ElementID, std::string> storehouse_map;
    for(auto storehouse_it = f.storehouse_cbegin(); storehouse_it!= f.storehouse_cend(); storehouse_it++){

        storehouse_str.clear();
        ElementID storehouse_id = storehouse_it->get_id();
        storehouse_str += "STOREHOUSE #" + std::to_string(storehouse_id);


        std::string stock_str;
        std::map<ElementID, std::string> stock_map;
        if(storehouse_it->cbegin() != storehouse_it->cend()){
            storehouse_str += "\n  Stock:";
            for(auto stock_it = storehouse_it->cbegin(); stock_it != storehouse_it->cend(); stock_it++){
                ElementID stock_id = stock_it->get_id();
                stock_str.clear();
                stock_str += " #" + std::to_string(stock_id) + ',';
                std::pair<ElementID, std::string> stock_pair = {stock_id, stock_str};
                stock_map.emplace(stock_pair);
            }
            sort(stock_map);
            for(auto& pair : stock_map){
                storehouse_str += pair.second;
            }
            storehouse_str.pop_back();
            storehouse_map.emplace(storehouse_id, storehouse_str);
        }
        else{
            storehouse_str += "\n  Stock: (empty)";
            os << storehouse_str;
        }

    }
    sort(storehouse_map);

    for(auto storehouse_pair_ptr = storehouse_map.begin(); storehouse_pair_ptr != storehouse_map.end(); storehouse_pair_ptr++){
        os << storehouse_pair_ptr->second;
    }
    os << "\n\n";
    os.flush();
}
