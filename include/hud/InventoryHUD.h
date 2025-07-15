#ifndef INVENTORYHUD_H
#define INVENTORYHUD_H

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"

#define ITEM_CANO "Cano"
#define ITEM_LANTERNA "Lanterna"
#define ITEM_VELA "Vela"
#define ITEM_BALDE_TINTA "Balde de Tinta"

#define INVENTORY InventoryHUD::instance

class InventoryHUD : public Component
{
public:
    InventoryHUD(GameObject& associated);
    ~InventoryHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    // Inventory system
    class Item {
    public:
        Item(std::string n, const char* s, int a = 0) : name(n), sprite(Sprite(s, 1, 1, true)), amount(a) {}
        virtual void Render(float x, float y) {
            sprite.Render(x, y, sprite.GetWidth(), sprite.GetHeight());
        }
        std::string name;
        Sprite sprite;
        int amount;
    };
    class BaldeTinta : public Item {
    public:
        BaldeTinta(int a = 0) : Item(ITEM_BALDE_TINTA, "Recursos/img/items/balde.png"), tinta(Sprite("Recursos/img/items/tinta.png", 1, 1, true)) {}
        void Render(float x, float y) override {
            sprite.Render(x, y, sprite.GetWidth(), sprite.GetHeight());
            SDL_SetTextureColorMod(tinta.texture, 255, 82, 0);
            tinta.Render(x, y, sprite.GetWidth(), sprite.GetHeight());
        }
        Sprite tinta;
    };

    int Collect(std::string itemName, int amount = 1);
    int Remove(std::string itemName, int amount = 1);
    bool HasItemInHand(std::string itemName);

    Item* inventory[4] = {nullptr};
    std::unordered_map<std::string, Item*> ITEMS;

    static InventoryHUD* instance;
private:
    Sprite toolbar, toolbarSelect;
    TextHUD* amountText;
    TextHUD* itemNameText;
    int selToolbar;
};

#endif // INVENTORYHUD_H
