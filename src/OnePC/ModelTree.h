#ifndef MODELTREE_H
#define MODELTREE_H

#include <QWidget>
#include <QTreeWidget>

#include "Model.h"

class ModelTree : public QWidget
{
public:
    ModelTree(QWidget *parent = nullptr);
    std::map<std::string, uint> init(Model* model);
};

#endif // MODELTREE_H
