#include <iostream>
#include <fstream>
#include "onnx.pb.h" 

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "Use this executable as ./exe_name <path_to_model>\n";
        return 1;
    }
    //verifying Protobuf runtime matches generated headers
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::ifstream input(argv[1], std::ios::in | std::ios::binary);
    if(!input){
        std::cerr << "Failed to open the ONNX file.\n";
        return 1;
    }

    //creating the model instance
    onnx::ModelProto model;
    if(!model.ParseFromIstream(&input)){
        std::cerr << "Failed to parse ONNX protobuf data.\n";
        return 1;
    }

    std::cout << "ONNX IR Version: " << model.ir_version() << "\n";
    std::cout << "Model Version: " << model.model_version() << "\n";
    std::cout << "Producer Name: " << model.producer_name() << "\n";
    std::cout << "Producer Version: "<< model.producer_version() << "\n";

    //reference to the Graph
    const onnx::GraphProto& graph = model.graph();

    //looping through the initializers using the _size() syntax
    std::cout << "\nTotal Initializers: " << graph.initializer_size() << "\n\n";
    
    for(int i = 0; i < graph.initializer_size(); ++i){
        // Access each individual tensor weight
        const onnx::TensorProto& tensor = graph.initializer(i);
        
        std::cout << "Initializer [" << i << "] Name: " << tensor.name() << "\n";
        std::cout << "  Dimensions: [";
        for (int j = 0; j < tensor.dims_size(); ++j) {
            std::cout << tensor.dims(j) << (j == tensor.dims_size() - 1 ? "" : ", ");
        }
        std::cout << "]\n\n";
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}