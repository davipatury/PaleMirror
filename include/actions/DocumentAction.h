#ifndef DOCUMENTACTION_H
#define DOCUMENTACTION_H

#include "actions/Action.h"
#include "core/GameObject.h"
#include "core/Component.h"
#include <string>

class DocumentAction : public Action{
private:
    std::string documentPath;
    GameObject* imageObject = nullptr;
    bool imageVisible = false;

public:
    DocumentAction(const std::string& path);
    void Execute() override;
    std::unique_ptr<Action> Clone() const override;
};

#endif
