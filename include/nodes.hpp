//
// Created by dkope on 15.12.2021.
//
#ifndef NET_SIMULATION_NODES_HPP
#define NET_SIMULATION_NODES_HPP

#include <storage_types.hpp>
#include <memory>
#include <map>
#include <helpers.hpp>

enum class ReceiverType{
    Worker, Storehouse
};

enum class NodeType{
    Ramp, Worker, Storehouse
};

class IPackageReceiver{

public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const= 0;

    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;

    virtual ReceiverType get_receiver_type() const = 0;
};




class ReceiverPreferences{

public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    ReceiverPreferences( ProbabilityGenerator pg = probability_generator) :  pg_(pg)  {}
    preferences_t preferences_t_;
    const preferences_t& get_preferences() const { return preferences_t_;}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver *r);
    IPackageReceiver* choose_receiver();

    const_iterator cbegin() const { return preferences_t_.cbegin(); }
    const_iterator cend() const { return preferences_t_.cend(); }
    const_iterator begin() const { return preferences_t_.cbegin(); }
    const_iterator end() const { return preferences_t_.cend(); }

private:
    ProbabilityGenerator pg_;
};

class PackageSender{
public:
    using Buffer = std::optional<Package>;


    ReceiverPreferences receiver_preferences_;
    PackageSender(PackageSender&&) = default;
    PackageSender() {}
    void send_package();
    const Buffer& get_sending_buffer() const {return s_buffer_; }

protected:
    virtual
    void push_package(Package&& r) {s_buffer_.emplace(std::move(r));};
    Buffer s_buffer_;
private:
    IPackageReceiver* receiver_ptr_ = nullptr;

};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }

private:
    ElementID id_;
    TimeOffset di_;
};

class Worker : public PackageSender, public IPackageReceiver{

public:

    Worker(ElementID ElementID, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : ElementID_(ElementID), pd_(pd), q_(std::move(q)) {};
    void do_work(Time t);
    TimeOffset get_processing_duration() const;
    Time get_package_processing_start_time() const;

    ElementID get_id() const override { return ElementID_;}
    void receive_package(Package&& p) override;
    ReceiverType get_receiver_type() const override{ return ReceiverType::Worker;}

    IPackageStockpile::const_iterator begin() const override {return q_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return q_->end();}
    IPackageStockpile::const_iterator cend() const override {return q_->cend();}

    IPackageQueue* get_queue() const { return q_.get();}
    const Buffer& get_processing_buffer() const { return p_buffer_;}

private:
    Buffer p_buffer_;
    Time start_t_;
    ElementID ElementID_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;

};

class Storehouse : public IPackageReceiver {
public:

    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)): id_(id), stock_ptr_(std::move(d)) {};
    ElementID get_id() const override { return id_;}
    void receive_package(Package&& p) override {stock_ptr_->push(std::move(p));}

    IPackageStockpile::const_iterator begin() const override {return stock_ptr_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return stock_ptr_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return stock_ptr_->end();}
    IPackageStockpile::const_iterator cend() const override {return stock_ptr_->cend();}

    ReceiverType get_receiver_type() const override{ return ReceiverType::Storehouse;}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> stock_ptr_;
};
#endif //NET_SIMULATION_NODES_HPP
