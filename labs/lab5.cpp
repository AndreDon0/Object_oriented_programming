#include <iostream>
#include <string>
#include <typeindex>
#include <vector>
#include <map>

class Element {
public:
    virtual ~Element() = default;
    virtual bool isBasic() const = 0;
    virtual unsigned int getAmount() const = 0;
    virtual void addAmount(unsigned int amt) = 0;
    virtual void subtractAmount(unsigned int amt) = 0;
};

template<class T, const char* elementName, unsigned int InitialAmount>
class BasicElement : public Element {
public:
    BasicElement() { }
    bool isBasic() const override { return basic; }
    unsigned int getAmount() const override { return amount; }
    void addAmount(unsigned int amt) override { amount += amt; }
    void subtractAmount(unsigned int amt) override { 
        if (amt > amount) throw std::runtime_error("Not enough amount of " + std::string(elementName));
        amount -= amt;
    }
private:
    static unsigned int amount;
    static constexpr bool basic = true;
};
template<class T, const char* elementName, unsigned int InitialAmount>
unsigned int BasicElement<T, elementName, InitialAmount>::amount = InitialAmount;

template<class T, const char* elementName>
class CompoundElement : public Element {
public:
    CompoundElement() { }
    bool isBasic() const override { return basic; }
    unsigned int getAmount() const override { return amount; }
    void addAmount(unsigned int amt) override { amount += amt; }
    void subtractAmount(unsigned int amt) override { 
        if (amt > amount) throw std::runtime_error("Not enough amount of " + std::string(elementName));
        amount -= amt;
    }
private:
    static unsigned int amount;
    static constexpr bool basic = false;
};
template<class T, const char* elementName>
unsigned int CompoundElement<T, elementName>::amount = 0;


std::map<std::type_index, Element*> discoveredElements;
using RecipeInput = std::pair<std::type_index, std::type_index>;
using RecipeOutput = std::type_index;
std::vector<std::pair<RecipeInput, RecipeOutput>> recipes;

Element* operator+(Element& e1, Element& e2) {
    std::type_index a = typeid(e1);
    std::type_index b = typeid(e2);
    
    for (auto& r : recipes) {
        if ((r.first.first == a && r.first.second == b) || 
            (r.first.first == b && r.first.second == a)) {

            e1.subtractAmount(1);
            e2.subtractAmount(1);
            
            if (discoveredElements.count(r.second)) {
                discoveredElements[r.second]->addAmount(1);
                return discoveredElements[r.second];
            }
        }
    }
    return nullptr;
}

std::pair<Element*, Element*> operator-(Element& elem) {
    if (!elem.isBasic()) {
        std::type_index compound = typeid(elem);
            
        for (auto& r : recipes) {
            if (r.second == compound) {
                elem.subtractAmount(1);
                
                auto elem1 = discoveredElements[r.first.first];
                auto elem2 = discoveredElements[r.first.second];
                elem1->addAmount(1);
                elem2->addAmount(1);
                return {elem1, elem2};
            }
        }
        throw std::runtime_error("Decomposition recipe not found");
    }
    throw std::runtime_error("Cannot decompose basic element");
}

void RegisterRecipe(RecipeInput input, RecipeOutput output) {
    recipes.push_back({input, output});
}


constexpr char FireName[] = "Fire";
constexpr char WaterName[] = "Water";
constexpr char EarthName[] = "Earth";
constexpr char AirName[] = "Air";
constexpr char SteamName[] = "Steam";
constexpr char MudName[] = "Mud";
constexpr char LavaName[] = "Lava";
constexpr char EnergyName[] = "Energy";

class Fire final : public BasicElement<Fire, FireName, 50> {};
class Water final : public BasicElement<Water, WaterName, 50> {};
class Earth final : public BasicElement<Earth, EarthName, 50> {};
class Air final : public BasicElement<Air, AirName, 50> {};
class Steam final : public CompoundElement<Steam, SteamName> {};
class Mud final : public CompoundElement<Mud, MudName> {};
class Lava final : public CompoundElement<Lava, LavaName> {};
class Energy final : public CompoundElement<Energy, EnergyName> {};

int main() {
    discoveredElements[typeid(Fire)] = new Fire();
    discoveredElements[typeid(Water)] = new Water();
    discoveredElements[typeid(Steam)] = new Steam();
    discoveredElements[typeid(Earth)] = new Earth();
    discoveredElements[typeid(Air)] = new Air();
    discoveredElements[typeid(Mud)] = new Mud();
    discoveredElements[typeid(Lava)] = new Lava();
    discoveredElements[typeid(Energy)] = new Energy();
    
    RegisterRecipe({typeid(Fire), typeid(Water)}, typeid(Steam));
    RegisterRecipe({typeid(Earth), typeid(Water)}, typeid(Mud));
    RegisterRecipe({typeid(Fire), typeid(Earth)}, typeid(Lava));
    RegisterRecipe({typeid(Air), typeid(Fire)}, typeid(Energy));
    
    try {
        Fire& fire = *static_cast<Fire*>(discoveredElements[typeid(Fire)]);
        Water& water = *static_cast<Water*>(discoveredElements[typeid(Water)]);
        
        std::cout << "Before combination - Fire: " << fire.getAmount() << ", Water: " << water.getAmount() << std::endl;
        
        auto steam = fire + water;
        
        std::cout << "After combination - Fire: " << fire.getAmount() << ", Water: " << water.getAmount();
        if (steam) {
            std::cout << ", Steam: " << steam->getAmount() << std::endl;
        }

        if (steam && steam->getAmount() > 0) {
            auto [fire2, water2] = -*steam;
            std::cout << "After decomposition - Fire: " << fire2->getAmount() << ", Water: " << water2->getAmount() << ", Steam: " << steam->getAmount() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

