#include "ModelTree.h"

ModelTree::ModelTree(QWidget *parent) {

}

std::map<std::string, uint> ModelTree::init(Model* model){
    std::map<std::string, uint> model_infs;
    std::map<std::string, Handle(AIS_Shape)> model_AIS = model->getOnePC_AIS();
    for(auto model : model_AIS){
        model_infs[model.first] = 0;
    }
    return model_infs;
}
