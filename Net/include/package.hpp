// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#ifndef NET_SIM_PACKAGE_HPP
#define NET_SIM_PACKAGE_HPP

#include "types.hpp"
#include <set>
#include <stdexcept>

class Package {

public:
    Package();
    explicit Package(ElementID id);
    Package(const Package&) = delete;
    Package(Package&& other) noexcept: id_(other.id_) { other.id_ = 0; }
    Package& operator=(const Package&) = delete;
    Package& operator=(Package&& other) noexcept;
    ElementID get_id() const { return id_; }
    ~Package();
private:
    ElementID id_=0;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //NET_SIM_PACKAGE_HPP

// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)