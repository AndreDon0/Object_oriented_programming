#include <iostream>
#include <string>
#include <typeindex>
#include <vector>
#include <map>
#include <functional>

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
    BasicElement() { name = elementName; amount--; }
    bool isBasic() const override { return basic; }
    unsigned int getAmount() const override { return amount; }
    void addAmount(unsigned int amt) override { amount += amt; }
    void subtractAmount(unsigned int amt) override { 
        if (amt > amount) throw std::runtime_error("Not enough amount of " + std::string(elementName));
        amount -= amt;
    }
    ~BasicElement() { amount++; }
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
    unsigned int getAmount() const override { return 0; }
    void addAmount(unsigned int amt) override { }
    void subtractAmount(unsigned int amt) override { }
private:
    static constexpr bool basic = false;
};


std::unordered_map<std::type_index, std::function<Element*()>> factory;
using RecipeInput = std::pair<std::type_index, std::type_index>;
using RecipeOutput = std::type_index;
std::vector<std::pair<RecipeInput, RecipeOutput>> recipes;

Element* operator+(Element& e1, Element& e2) {
    std::type_index a = typeid(e1);
    std::type_index b = typeid(e2);
    
    for (auto& r : recipes) {
        if ((r.first.first == a && r.first.second == b) || 
            (r.first.first == b && r.first.second == a)) {

            auto combinedElement = factory[r.second]();
            return combinedElement;
        }
    }
    return nullptr;
}

std::pair<Element*, Element*> operator-(Element& elem) {
    if (!elem.isBasic()) {
        std::type_index compound = typeid(elem);
            
        for (auto& r : recipes) {
            if (r.second == compound) {
                delete &elem;
                
                auto elem1 = factory[r.first.first]();
                auto elem2 = factory[r.first.second]();
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

void factorySetup() {
    factory[typeid(Fire)] = []() { return new Fire(); };
    factory[typeid(Water)] = []() { return new Water(); };
    factory[typeid(Earth)] = []() { return new Earth(); };
    factory[typeid(Air)] = []() { return new Air(); };
    factory[typeid(Steam)] = []() { return new Steam(); };
    factory[typeid(Mud)] = []() { return new Mud(); };
    factory[typeid(Lava)] = []() { return new Lava(); };
    factory[typeid(Energy)] = []() { return new Energy(); };
}

void recipeSetup() {
    registerRecipe({typeid(Fire), typeid(Water)}, typeid(Steam));
    registerRecipe({typeid(Earth), typeid(Water)}, typeid(Mud));
    registerRecipe({typeid(Fire), typeid(Earth)}, typeid(Lava));
    registerRecipe({typeid(Air), typeid(Fire)}, typeid(Energy));
}

int main() {    
    factorySetup();
    recipeSetup();

    std::cout << "=== Element Combination System Demo ===\n\n";

    // Create basic elements
    Fire fire;
    Water water;
    Earth earth;
    Air air;

    std::cout << "Initial amounts:\n";
    std::cout << "Fire: " << fire.getAmount() << "\n";
    std::cout << "Water: " << water.getAmount() << "\n";
    std::cout << "Earth: " << earth.getAmount() << "\n";
    std::cout << "Air: " << air.getAmount() << "\n\n";

    // Test combination: Fire + Water = Steam
    std::cout << "=== Combining Fire + Water ===\n";
    Element* steam = fire + water;
    if (steam) {
        std::cout << "Created Steam! Amount: " << steam->getAmount() << "\n";
        std::cout << "Fire remaining: " << fire.getAmount() << "\n";
        std::cout << "Water remaining: " << water.getAmount() << "\n";
        std::cout << "Is Steam basic? " << (steam->isBasic() ? "Yes" : "No") << "\n\n";
    }

    // Test combination: Earth + Water = Mud
    std::cout << "=== Combining Earth + Water ===\n";
    Element* mud = earth + water;
    if (mud) {
        std::cout << "Created Mud! Amount: " << mud->getAmount() << "\n";
        std::cout << "Earth remaining: " << earth.getAmount() << "\n";
        std::cout << "Water remaining: " << water.getAmount() << "\n\n";
    }

    // Test combination: Fire + Earth = Lava
    std::cout << "=== Combining Fire + Earth ===\n";
    Element* lava = fire + earth;
    if (lava) {
        std::cout << "Created Lava! Amount: " << lava->getAmount() << "\n";
        std::cout << "Fire remaining: " << fire.getAmount() << "\n";
        std::cout << "Earth remaining: " << earth.getAmount() << "\n\n";
    }

    // Test combination: Air + Fire = Energy
    std::cout << "=== Combining Air + Fire ===\n";
    Element* energy = air + fire;
    if (energy) {
        std::cout << "Created Energy! Amount: " << energy->getAmount() << "\n";
        std::cout << "Air remaining: " << air.getAmount() << "\n";
        std::cout << "Fire remaining: " << fire.getAmount() << "\n\n";
    }

    // Test multiple combinations of same type
    std::cout << "=== Creating more Steam ===\n";
    Element* steam2 = fire + water;
    Element* steam3 = fire + water;
    std::cout << "Total Steam amount: " << steam->getAmount() << "\n";
    std::cout << "Fire remaining: " << fire.getAmount() << "\n";
    std::cout << "Water remaining: " << water.getAmount() << "\n\n";

    // Test decomposition
    std::cout << "=== Decomposing Steam ===\n";
    std::cout << "Steam amount before: " << steam->getAmount() << "\n";
    auto [elem1, elem2] = -(*steam);
    std::cout << "Steam amount after: " << steam->getAmount() << "\n";
    std::cout << "Decomposed into two elements:\n";
    std::cout << "Element 1 type: " << typeid(*elem1).name() << ", amount: " << elem1->getAmount() << "\n";
    std::cout << "Element 2 type: " << typeid(*elem2).name() << ", amount: " << elem2->getAmount() << "\n\n";

    // Test invalid combination
    std::cout << "=== Testing invalid combination (Fire + Fire) ===\n";
    Fire fire2;
    Element* invalid = fire + fire2;
    if (!invalid) {
        std::cout << "No recipe found - correctly returned nullptr\n\n";
    }

    // Test error handling: try to decompose basic element
    std::cout << "=== Testing decomposition of basic element ===\n";
    try {
        auto result = -fire;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected error: " << e.what() << "\n\n";
    }

    // Test error handling: not enough resources
    std::cout << "=== Testing insufficient resources ===\n";
    try {
        // Try to subtract more than available
        Fire testFire;
        testFire.subtractAmount(51); // We only have 50
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected error: " << e.what() << "\n\n";
    }

    // Show final state
    std::cout << "=== Final State ===\n";
    std::cout << "Fire: " << fire.getAmount() << "\n";
    std::cout << "Water: " << water.getAmount() << "\n";
    std::cout << "Earth: " << earth.getAmount() << "\n";
    std::cout << "Air: " << air.getAmount() << "\n";

    // Cleanup
    delete steam2;
    delete steam3;
    delete mud;
    delete lava;
    delete energy;

    return 0;
}

