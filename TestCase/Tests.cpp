#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <map>

#include "onnx/onnx_pb.h"
#include "onnx/proto_utils.h"

#include "ONNXInterface.h"

namespace test1 {
int Test(const std::string& Net_Path)
{

	std::ifstream resnet50_stream(Net_Path, std::ios::binary);

	if (resnet50_stream.is_open())
	{
		resnet50_stream.seekg(0, std::ios::end);
		std::size_t FileSize = resnet50_stream.tellg();
		resnet50_stream.seekg(0, std::ios::beg);
		char* buffer = new char[FileSize];
		resnet50_stream.read(buffer, FileSize);

		std::unique_ptr<onnx::ModelProto> model(new onnx::ModelProto());
		onnx::ParseProtoFromBytes(model.get(), buffer, FileSize);

		std::cout << "model->graph().input().size():" << model->graph().input().size() << std::endl;

		//model->PrintDebugString();
		//model->copyFrom()

		auto irVer = model->ir_version();

		std::cout << "irVer: " << irVer << std::endl;

		auto opsetimport = model->opset_import();
		//std::cout << opsetimport << std::endl;

		auto opsetNum = opsetimport.size();

		//auto it = opsetimport.begin();
		//auto itEnd = opsetimport.end();

		std::vector<onnx::OperatorSetIdProto> VecSetId(opsetimport.begin(), opsetimport.end());



		auto numVec = VecSetId.size();
		auto OptId = VecSetId[0];

		OptId.version();
		//OptId.magic
		
		std::cout << "opsetNum: " << opsetNum << std::endl;

		std::cout << "OptId.version(): " << OptId.version() << std::endl;
		//auto aaa = opsetimport->magic();

		auto ProdName = model->producer_name();
		std::cout << "ProdName: " << ProdName << std::endl;

		auto ProdVer = model->producer_version();
		std::cout << "ProdVer: " << ProdVer << std::endl;

		auto Domi = model->domain();
		std::cout << "Domi: " << Domi << std::endl;

		auto MoVer = model->model_version();
		std::cout << "MoVer: " << MoVer << std::endl;

		auto docstr = model->doc_string();
		std::cout << "docstr: " << docstr << std::endl;

		auto Meta = model->metadata_props();

		auto TrnInfo = model->training_info();

		auto GM = model->graph();

		auto AAA = GM.name();

		auto Nodes = GM.node();

		std::vector VecNodes(Nodes.begin(), Nodes.end());

		auto InL = GM.input();
		std::cout << "GM.ByteSize(): " << GM.ByteSize() << std::endl;




	}



	return 0;
}
}

namespace test2 {

int Test(const std::string& Net_Path)
{
	std::ifstream resnet50_stream(Net_Path, std::ios::binary);
	if (resnet50_stream.is_open())
	{
		resnet50_stream.seekg(0, std::ios::end);
		std::size_t FileSize = resnet50_stream.tellg();
		resnet50_stream.seekg(0, std::ios::beg);
		char* buffer = new char[FileSize];
		resnet50_stream.read(buffer, FileSize);

		std::unique_ptr<onnx::ModelProto> model(new onnx::ModelProto());
		onnx::ParseProtoFromBytes(model.get(), buffer, FileSize);

		delete[] buffer;

		auto NDs = model->graph().node();

		int nodeidx = 1;

		auto itEnd = NDs.end();

		for (auto it = NDs.begin();  it != itEnd; ++it)
		{
			std::cout  << "<=============== Node #" << nodeidx++ << " ===============> ";
			std::cout << std::endl << std::endl;
			
			// Show Layer Info
			std::cout << "Layer Type : " << it->op_type() ;
			std::cout << std::endl;

			
			// Show Input Info
			auto inputs = it->input();
			std::cout << inputs.size() << " Inputs : ";
			auto itinpEnd = inputs.end();
			for (auto itinp = inputs.begin(); itinp != itinpEnd; ++itinp)
			{
				std::cout << *itinp << ", ";
			}			
			std::cout << std::endl;

			// Show Output Info
			auto outputs = it->output();
			std::cout << outputs.size() << " Outputs : " ;
			auto itoutEnd = outputs.end();
			for (auto itout = outputs.begin(); itout != itoutEnd; ++itout)
			{
				std::cout << *itout << ", ";
			}
			std::cout << std::endl;
			
			// Show Attributes
			auto Attr = it->attribute();
			std::cout << Attr.size() << " Attributes : ";
			auto AttrEnd = Attr.end();

			if (Attr.size() == 0)
				std::cout << std::endl;

			for (auto Attrit = Attr.begin(); Attrit != AttrEnd; ++Attrit)
			{
				int nType = Attrit->type();
				std::cout << "(" << ECMONNXAPI::AddrTypetoString(nType) << ", " << Attrit->name() << "), ";

				switch (nType)
				{
				case 0:
					std::cout << "no contentds" << std::endl;
					break;
				case 1:
					std::cout << Attrit->f() << std::endl;
					break;
				case 2:
					std::cout << Attrit->i() << std::endl;
					break;
				case 3:
					std::cout << Attrit->s() << std::endl;
					break;
				case 4:
					//std::cout << Attrit->t() << std::endl;
					std::cout << "A tensor" << std::endl;
					break;
				case 5:
					//std::cout << Attrit->g() << std::endl;
					std::cout << "A graph" << std::endl;
					break;
				case 6:
					std::cout << "[";
					for (int i = 0; i < Attrit->floats_size(); i++)
					{
						std::cout << Attrit->floats()[i] << ", ";
					}
					std::cout << "]" <<std::endl;
					break;

				case 7:
					std::cout << "[";
					for (int i = 0; i < Attrit->ints_size(); i++)
					{
						std::cout << Attrit->ints()[i] << ", ";
					}
					std::cout << "]" << std::endl;
					break;

				case 8:
					std::cout << "[";
					for (int i = 0; i < Attrit->strings_size(); i++)
					{
						std::cout << Attrit->strings()[i] << ", ";
					}
					std::cout << "]" << std::endl;
					break;
				default:
					std::cout << std::endl;
				}

			}
			std::cout << std::endl;

		}

	}



	return 0;
}
}

namespace ModelInfo {

int Test(const std::string & Net_Path)
{

	//std::string Net_Name = "bvlcalexnet-9.onnx";
ECMONNXAPI::ONNXModel OModel = ECMONNXAPI::ONNXRead(Net_Path);
if (OModel.GetStatus())
{
	std::cout << "Cannot Generate ONNX Model Object" << std::endl;
	return -1;
}
else
{
	auto pModel = OModel.GetModel();
	
	auto OppSetList = pModel->opset_import();
	std::cout << "<=============== Model Info ===============" << std::endl;
	std::cout <<"OpSet Size: " << OppSetList.size() << std::endl;
	
	for (int i = 0; i < OppSetList.size(); i++)
	{
		std::cout << "OpSet " << i << ": " <<OppSetList[i].version() << std::endl ;
	}


	auto MetaProp = pModel->metadata_props();
	std::cout << "MetaProp: " << MetaProp.size() << std::endl;

	auto TrainingProp = pModel->training_info();
	std::cout << "TrainingProp: " << TrainingProp.size() << std::endl;

	auto TheGraph = pModel->graph();
	std::cout << "TheGraph.name: " << TheGraph.name() << std::endl;

	std::cout << "TheGraph.node().size(): " << TheGraph.node().size() << std::endl;
	std::cout << "Names are : " << ECMONNXAPI::ShowListNames(TheGraph.node()) << std::endl;
	

	std::cout << "TheGraph.initializer().size() " << TheGraph.initializer().size() << std::endl;
	std::cout << "Names are : " << ECMONNXAPI::ShowListNames(TheGraph.initializer()) << std::endl;
	

	std::cout << "TheGraph.sparse_initializer().size() " << TheGraph.sparse_initializer().size() << std::endl;
	//std::cout << "Names are : " << ONNXAPI::ShowListNames(TheGraph.sparse_initializer()) << std::endl;
	
	std::cout << "TheGraph.doc_string(): " << TheGraph.doc_string() << std::endl;
	std::cout << "TheGraph.input().size(): " << TheGraph.input().size() << std::endl;
	std::cout << "Names are : " << ECMONNXAPI::ShowListNames(TheGraph.input()) << std::endl;
	std::cout << "Types are : " << ECMONNXAPI::ShowTypeProtoListTypes(TheGraph.input()) << std::endl;
	std::cout << "Dim At 1 : " << ECMONNXAPI::ShowList(ECMONNXAPI::GetTensorDimAt(TheGraph.input(), 1)) << std::endl;




	std::cout << "TheGraph.output().size(): " << TheGraph.output().size() << std::endl;
	std::cout << "Names are : " << ECMONNXAPI::ShowListNames(TheGraph.output()) << std::endl;
	std::cout << "Types are : " << ECMONNXAPI::ShowTypeProtoListTypes(TheGraph.output()) << std::endl;

	

}


return 0;

}

}

namespace TypeTest
{
int Test(const std::string & ONNX_Path)
{

ECMONNXAPI::ONNXModel OModel = ECMONNXAPI::ONNXRead(ONNX_Path);
if (OModel.GetStatus())
{
	std::cout << "Cannot Generate ONNX Model Object" << std::endl;
	return -1;
}
else
{
	auto pModel = OModel.GetModel();
	
	 //ShowTypeProtoListTypes

	//pModel
}



}

}

namespace TrainingInfo {

int Test(const std::string & Net_Path)
{



return 0;
}

}

int main()
{
	//std::string Net_Dir = "D:\\Sources\\Git\\ONNX\\models2\\vision\\classification\\resnet\\model\\";
    //std::string Net_Dir = "D:\\Sources\\Git\\ONNX\\models2\\vision\\classification\\mnist\\model\\";
	//std::string Net_Name = "resnet50-v2-7.onnx";
	//std::string Net_Name = "mnist-8.onnx";

	std::string Net_Path = "D:\\Sources\\Git\\ONNX\\models2\\vision\\classification\\alexnet\\model\\bvlcalexnet-9.onnx";

	int ret = 0;
	//ret = test1::Test(Net_Path);
	//ret = test2::Test(Net_Path);
	//ret = ModelInfo::Test(Net_Path);
	//ret = TypeTest::Test(Net_Path);
	ret = TrainingInfo::Test(Net_Path);

	return ret;
}