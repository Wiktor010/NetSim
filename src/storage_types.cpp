//
// Created by dkope on 18.12.2021.
//
#include <storage_types.hpp>

Package PackageQueue::pop() {
    switch (get_queue_type()) {
        case PackageQueueType::FIFO: {
            Package apackage = Package(std::move(list_of_packages_.front()));
            list_of_packages_.pop_front();
            return apackage;
        }


        case PackageQueueType::LIFO: {
            Package apackage = (Package(std::move(list_of_packages_.back())));
            list_of_packages_.pop_back();
            return apackage;
        }
        default : {
            Package apackage = Package(std::move(list_of_packages_.front()));
            list_of_packages_.pop_front();
            return apackage;
        }
    }
}

PackageQueue::PackageQueue(PackageQueueType type) {

    type_of_Queue_ = type;

}

