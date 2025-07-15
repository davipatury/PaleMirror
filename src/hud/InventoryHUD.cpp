#include "hud/InventoryHUD.h"

#define AMT_OFFSET_X 36
#define AMT_OFFSET_Y 36

InventoryHUD* InventoryHUD::instance;

InventoryHUD::InventoryHUD(GameObject& associated) : Component(associated),
    toolbar("Recursos/img/hud/toolbar.png", 1, 1, true),
    toolbarSelect("Recursos/img/hud/toolbar-selector.png", 1, 1, true)
{
    selToolbar = 0;

    amountText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 15, TextHUD::BLENDED, "1", {0, 0, 0, 255});

    ITEMS[ITEM_CANO] = new Item(ITEM_CANO, "Recursos/img/objetos/Porta.png");
    ITEMS[ITEM_LANTERNA] = new Item(ITEM_LANTERNA, "Recursos/img/objetos/CADEIRAM.png");
    ITEMS[ITEM_VELA] = new Item(ITEM_VELA, "Recursos/img/items/vela.png");
    ITEMS[ITEM_BALDE_TINTA] = new BaldeTinta();
}

InventoryHUD::~InventoryHUD() {}

void InventoryHUD::Update(float dt) {
    if (INPUT_MANAGER.KeyPress(SDLK_1))         selToolbar = 0;
    else if( INPUT_MANAGER.KeyPress(SDLK_2))    selToolbar = 1;
    else if(INPUT_MANAGER.KeyPress(SDLK_3))     selToolbar = 2;
    else if (INPUT_MANAGER.KeyPress(SDLK_4))    selToolbar = 3;
}

void InventoryHUD::Render() {
    toolbar.Render(460, 760, toolbar.GetWidth(), toolbar.GetHeight());
    toolbarSelect.Render(467 + selToolbar * (9 + toolbarSelect.GetWidth()), 769, toolbarSelect.GetWidth(), toolbarSelect.GetHeight());

    for (int i = 0; i < 4; i++) {
        if (inventory[i] != nullptr) {
            float x = (62 * i) + 469;
            inventory[i]->sprite.Render(x, 771, inventory[i]->sprite.GetWidth(), inventory[i]->sprite.GetHeight());
            if (inventory[i]->amount > 1) {
                // Render amount text
                amountText->SetPos({x + AMT_OFFSET_X, 771 + AMT_OFFSET_Y});
                amountText->SetText(std::to_string(inventory[i]->amount));
                amountText->Render();
            }
        }
    }
    // 9, 11 hotbar item coords
}

bool InventoryHUD::Is(std::string type) {
    return type == "InventoryHUD";
}

int InventoryHUD::Collect(std::string itemName, int amount) {
    // Unknown item
    if (ITEMS.find(itemName) == ITEMS.end()) return -1;

    // Item already in inventory
    for (int i = 0; i < 4; i++) {
        if (inventory[i] != nullptr && inventory[i]->name == itemName) {
            inventory[i]->amount += amount;
            return inventory[i]->amount;
        }
    }

    // New item
    // Can't pickup without empty hands
    if (inventory[selToolbar] != nullptr) return -1;

    inventory[selToolbar] = ITEMS[itemName];
    inventory[selToolbar]->amount = amount;
    return 1;
}

int InventoryHUD::Remove(std::string itemName, int amount) {
    // Unknown item
    if (ITEMS.find(itemName) == ITEMS.end()) return -1;

    for (int i = 0; i < 4; i++) {
        if (inventory[i] != nullptr && inventory[i]->name == itemName) {
            if (amount < inventory[i]->amount) {
                inventory[i]->amount -= amount;
                return inventory[i]->amount;
            }
            inventory[i]->amount = 0;
            inventory[i] = nullptr;
            return 0;
        }
    }

    return -1;
}

bool InventoryHUD::HasItemInHand(std::string itemName) {
    return inventory[selToolbar] != nullptr && inventory[selToolbar]->name == itemName;
}
