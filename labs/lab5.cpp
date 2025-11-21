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
protected:
    std::string name;
};

template<const char* elementName, unsigned int InitialAmount>
class BasicElement : public Element {
public:
    BasicElement() { name = elementName; }
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
template<const char* elementName, unsigned int InitialAmount>
unsigned int BasicElement<elementName, InitialAmount>::amount = InitialAmount;

template<const char* elementName>
class CompoundElement : public Element {
public:
    CompoundElement() { name = elementName; }
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
template<const char* elementName>
unsigned int CompoundElement<elementName>::amount = 0;


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
            discoveredElements[r.second]->addAmount(1);
            return discoveredElements[r.second];
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

void registerRecipe(RecipeInput input, RecipeOutput output) {
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

class Fire final : public BasicElement<FireName, 50> {};
class Water final : public BasicElement<WaterName, 50> {};
class Earth final : public BasicElement<EarthName, 50> {};
class Air final : public BasicElement<AirName, 50> {};
class Steam final : public CompoundElement<SteamName> {};
class Mud final : public CompoundElement<MudName> {};
class Lava final : public CompoundElement<LavaName> {};
class Energy final : public CompoundElement<EnergyName> {};

int main() {
    discoveredElements[typeid(Fire)] = new Fire();
    discoveredElements[typeid(Water)] = new Water();
    discoveredElements[typeid(Steam)] = new Steam();
    discoveredElements[typeid(Earth)] = new Earth();
    discoveredElements[typeid(Air)] = new Air();
    discoveredElements[typeid(Mud)] = new Mud();
    discoveredElements[typeid(Lava)] = new Lava();
    discoveredElements[typeid(Energy)] = new Energy();
    
    registerRecipe({typeid(Fire), typeid(Water)}, typeid(Steam));
    registerRecipe({typeid(Earth), typeid(Water)}, typeid(Mud));
    registerRecipe({typeid(Fire), typeid(Earth)}, typeid(Lava));
    registerRecipe({typeid(Air), typeid(Fire)}, typeid(Energy));
    
    try {
        Fire& fire = *static_cast<Fire*>(discoveredElements[typeid(Fire)]);
        Water& water = *static_cast<Water*>(discoveredElements[typeid(Water)]);
        
        std::cout << "Before combination - Fire: " << fire.getAmount() << ", Water: " << water.getAmount() << std::endl;
        
        auto steam = fire + water;
        
        std::cout << "After combination - Fire: " << fire.getAmount() << ", Water: " << water.getAmount();
        if (steam) {
            std::cout << ", Steam: " << steam->getAmount() << std::endl;
        }

        if (steam && (steam->getAmount() > 0)) {
            auto result = -(*steam);
            std::cout << "After decomposition - Fire: " << result.first->getAmount() << ", Water: " << result.second->getAmount() << ", Steam: " << steam->getAmount() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

