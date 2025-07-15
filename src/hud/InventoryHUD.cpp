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
    itemNameText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 25, TextHUD::BLENDED, "1", {255, 255, 255, 255});

    ITEMS[ITEM_CANO] = new Item(ITEM_CANO, "Recursos/img/items/cano.png");
    ITEMS[ITEM_LANTERNA] = new Item(ITEM_LANTERNA, "Recursos/img/items/lanterna.png");
    ITEMS[ITEM_VELA] = new Item(ITEM_VELA, "Recursos/img/items/vela.png");
    ITEMS[ITEM_BALDE_TINTA] = new BaldeTinta();
}

InventoryHUD::~InventoryHUD() {}

void InventoryHUD::Update(float dt) {
    if (INPUT_MANAGER.KeyPress(SDLK_1))         selToolbar = 0;
    else if( INPUT_MANAGER.KeyPress(SDLK_2))    selToolbar = 1;
    else if(INPUT_MANAGER.KeyPress(SDLK_3))     selToolbar = 2;
    else if (INPUT_MANAGER.KeyPress(SDLK_4))    selToolbar = 3;

    if (INPUT_MANAGER.GetMouseWheel() < 0 || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
        selToolbar++;
        if (selToolbar > 3) selToolbar = 0;
    }
    if (INPUT_MANAGER.GetMouseWheel() > 0 || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) {
        selToolbar--;
        if (selToolbar < 0) selToolbar = 3;
    }
}

void InventoryHUD::Render() {
    // Render toolbar
    toolbar.Render(460, 760, toolbar.GetWidth(), toolbar.GetHeight());
    toolbarSelect.Render(467 + selToolbar * (9 + toolbarSelect.GetWidth()), 769, toolbarSelect.GetWidth(), toolbarSelect.GetHeight());

    // Render item icons
    for (int i = 0; i < 4; i++) {
        if (inventory[i] != nullptr) {
            float x = (62 * i) + 469;
            float y = 771;
            inventory[i]->Render(x, y);
            if (inventory[i]->amount > 1) {
                // Render amount text
                amountText->SetPos({x + AMT_OFFSET_X, 771 + AMT_OFFSET_Y});
                amountText->SetText(std::to_string(inventory[i]->amount));
                amountText->Render();
            }
        }
    }

    // Render selected item name
    if (inventory[selToolbar] != nullptr) {
        itemNameText->SetText(inventory[selToolbar]->name);
        itemNameText->SetPos({600 - itemNameText->GetWidth() / 2, 850});
        itemNameText->Render();
    }
}

bool InventoryHUD::Is(std::string type) {
    return type == "InventoryHUD";
}

int InventoryHUD::Collect(std::string itemName, int amount) {
    // Unknown item
    if (ITEMS.find(itemName) == ITEMS.end()) return -1;

    // Put item in first empty slot
    int firstEmpty = -1;
    for (int i = 0; i < 4; i++) {
        if (inventory[i] == nullptr) {
            if (firstEmpty == -1) firstEmpty = i;
        } else if (inventory[i]->name == itemName) {
            inventory[i]->amount += amount;
            return inventory[i]->amount;
        }
    }

    // Full inventory
    if (firstEmpty == -1) return -1;

    inventory[firstEmpty] = ITEMS[itemName];
    inventory[firstEmpty]->amount = amount;
    return 1;
}

int InventoryHUD::Remove(std::string itemName, int amount) {
    // Unknown item
    if (ITEMS.find(itemName) == ITEMS.end()) return 0;

    for (int i = 0; i < 4; i++) {
        if (inventory[i] != nullptr && inventory[i]->name == itemName) {
            if (amount < inventory[i]->amount) {
                inventory[i]->amount -= amount;
                return amount;
            }
            int removed = inventory[i]->amount;
            inventory[i]->amount = 0;
            inventory[i] = nullptr;
            return removed;
        }
    }

    return 0;
}

bool InventoryHUD::HasItemInHand(std::string itemName) {
    return inventory[selToolbar] != nullptr && inventory[selToolbar]->name == itemName;
}
