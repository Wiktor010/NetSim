//
// Created by dkope on 15.12.2021.
//


#ifndef NET_SIMULATION_REPORTS_HPP
#define NET_SIMULATION_REPORTS_HPP

#include <sstream>
#include "factory.hpp"





class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {};
    bool should_generate_report(Time t) { return turns_.find(t) != turns_.end(); };
private:
    std::set<Time> turns_;
};
class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to) : to_(to) {};
    bool should_generate_report(Time t) { return t%to_==1; };
private:
    TimeOffset to_;
};

void generate_structure_report(const Factory& f, std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);


void sort(std::map<int, std::string>& M);
#endif //NET_SIMULATION_REPORTS_HPP