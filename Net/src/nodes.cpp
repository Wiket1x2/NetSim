//
// Created by krzys on 10.12.2019.
//

#include "nodes.hpp"
#include "helpers.hpp"

std::set<ElementID> Ramp::Ramp_IDs;
std::set<ElementID> Worker::Worker_IDs;
std::set<ElementID> Storehouse::Storehouse_IDs;


//ReceiverPreferences
void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double probability = 1.0;
    if (probability_map.empty()){
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else{
        probability = 1.0/(probability_map.size()+1);  //prawdopodobienstwo wyboru jest identyczne dla każdego odbiorcy
        double coeff = 1.0-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*(it->second);
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r, double probability) {
    if (probability_map.empty()){
        probability = 1.0;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else if (probability>=0 && probability<=1){
        double coeff = 1.0-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*(it->second);
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
    else throw std::invalid_argument("Wrong probability value."); //FIXME czy rzucać błąd jeżeli wartość prawdopodobieństwa jest spoza zakresu [0,1] (R) Ta funkcja jest w sumie nie potrzebna, ale jeżeli chcesz z niej kożystać to tak lepiej zeby wywalała ci błąd.
} //metoda z inna lista parametrow-dodany parametr probability

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if (!probability_map.empty()){
        double coeff = 1.0-probability_map[r];;
        probability_map.erase(r);
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = (1/coeff)*(it->second);
    }
    else throw std::runtime_error("You don't have any receivers"); //FIXME czy rzucać błąd kiedy pusty? (R) nie trzeba funkcja erase nie wywala błędu jeżeli nie ma klucza r w mapie (sprawdzone)
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double generated_number = pg_(); //FIXME czy aby napewno ta liczba wygenerowana bedzie z przedialu 0-1?  (R) nie my podajemy funkcje jako argument (trudne do zabezpiecznenia)
    double sum = 0.0;
    IPackageReceiver* ptr = nullptr;

    if (!probability_map.empty()){
        for (auto it=probability_map.begin(); sum < generated_number; ++it){
            sum += it->second;
            ptr = it->first;
        }
    }
    else throw std::runtime_error("You don't have any receivers"); //FIXME czy rzucać błąd kiedy pusty? (R) mysle ze jak najbardziej - nie wiemy do kogo udał się produkt - produkt nam w najlepszym wypadku zniknie.

    return ptr;
}


//PackageSender
void PackageSender::send_package() {
    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    if(buffor_package_) {
        receiver->receive_package(std::move(buffor_package_.value()));
        buffor_package_.reset();
    }
    //FIXME pytanie czy tu powinien byc wyjatek - wydaje mi sie ze nie...? (R) nie, uzycie funkcji gdy bufor jest pusty nie wykona po prostu zadnej akcji.
}

void PackageSender::push_package(Package&& package) {
    if(!buffor_package_.has_value()) { buffor_package_= std::move(package);}
    else throw std::runtime_error("buffor is already full!");
} //FIXME rzucac wyjatek jak zajety? (R) można rzucić, wykona to sie tylko przy gdy ktoś z jakiegoś powodu uzyje 2 razy pod rzad push_package. Takie zabezpieczenie przed nieumiejetna praca.


//Ramp
Ramp::Ramp(ElementID id, TimeOffset di,ReceiverPreferences receiver_preferences): PackageSender(receiver_preferences)  {
    if(Ramp_IDs.find(id)==Ramp_IDs.end())
    {
        id_ = id;
        di_ = di;
        Ramp_IDs.insert(id_);
    } else throw std::runtime_error("Zajety identyfikator"); //FIXME Rzucac blad? (R) tak
}

void Ramp::deliver_goods(Time t) {
    if(t%di_==0) {
        push_package(Package()); //FIXME chyba tak to powinno wygladac (R) tak
    }
}

Ramp::~Ramp() {
    if (id_!=0) {
        Ramp_IDs.erase(id_);
    }
}


//Worker
Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q,ReceiverPreferences receiver_preferences): PackageSender(receiver_preferences)  {
    if(Worker_IDs.find(id)==Worker_IDs.end())
    {
        id_ = id;
        pd_ = pd;
        q_ = std::move(q);
        Worker_IDs.insert(id_);
        start_t = 1; //zobacz działanie do_work
    } else throw std::runtime_error("Zajety identyfikator"); //FIXME Rzucac blad?  (R) tak
}

void Worker::do_work(Time t) {
    if (t==0) {
        start_t=0;
    }
    if (!processing_buffor_.has_value() && (t-start_t==0 || t-start_t==pd_)) {
        processing_buffor_.emplace(q_->pop()); // przekazanie do bufora przetwarzania gdy jest pusty
        start_t=t;
    }
    if(t-start_t==pd_-1) {
        push_package(std::move(processing_buffor_.value()));
        processing_buffor_.reset();
    }

}

Worker::~Worker() {
    if (id_!=0) {
        Worker_IDs.erase(id_);
    }
}


//Storehouse
Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    if(Storehouse_IDs.find(id)==Storehouse_IDs.end())
    {
        id_ = id;
        d_=std::move(d);
        Storehouse_IDs.insert(id_);
    } else throw std::runtime_error("Zajety identyfikator"); //FIXME Rzucac blad? (R) tak
}

Storehouse::~Storehouse() {
    if (id_!=0) {
        Storehouse_IDs.erase(id_);
    }
}