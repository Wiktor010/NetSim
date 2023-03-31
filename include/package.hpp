//
// Created by dkope on 15.12.2021.
//
#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP



#include <algorithm>
#include <iostream>
#include <set>
#include <types.hpp>


class Package{
public:

    Package();
    Package(ElementID ElementID): ElementID_(ElementID) {}
    Package(Package&& package);


    Package& operator=( Package&& package);
    const ElementID& get_id() const { return ElementID_; };
    ~Package();

private:
    inline static std::set<int> assigned_IDs;
    inline static std::set<int> freed_IDs;
    ElementID ElementID_;
};


#endif //NET_SIMULATION_PACKAGE_HPP

