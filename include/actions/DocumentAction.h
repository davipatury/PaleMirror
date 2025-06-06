#ifndef DOCUMENTACTION_H
#define DOCUMENTACTION_H

#include "actions/Action.h"
#include <string>

class DocumentAction : public Action {
private:
    std::string documentPath;

public:
    DocumentAction(const std::string& path)
        : documentPath(path)
    {}

    void Execute() override;
};

#endif
