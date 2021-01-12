#include "ONNXInterface.h"
#include <sstream>
#include <string>

namespace ECMONNXAPI
{

	
std::string ShowTypeProtoListTypes(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList)
{
	std::stringstream  ss;
	
	/*
	kTensorType = 1,
		kSequenceType = 4,
		kMapType = 5,
		VALUE_NOT_SET = 0,
		*/

	ss << "[";
	for (int i = 0; i < RepList.size(); i++)
	{
		int ValCase = RepList[0].type().value_case();
		if (i != 0)
			ss << ", ";

		switch (ValCase)
		{
		case onnx::TypeProto::ValueCase::kTensorType:
			ss << "TensorType";
			break;

		case onnx::TypeProto::ValueCase::kSequenceType:
			ss << "SequenceType";
			break;

		case onnx::TypeProto::ValueCase::kMapType:
			ss << "MapType";
			break;

		case onnx::TypeProto::ValueCase::VALUE_NOT_SET:
			ss << "NotSet";
			break;
		default:
			ss << "NONE";
			break;
		}
	
	}
	ss << "]";

	return ss.str();
}

std::string ShowDimOfListVIP(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList)
{
	using namespace ECMONNXAPI;
	std::string tmpRet = "[";

	if (RepList.size() > 0)
	{
		tmpRet.append(ShowList(GetTensorDimAt(RepList,0)));
		for (int i = 1; i < RepList.size(); i++)
		{
			tmpRet.append(", ");
			tmpRet.append(ShowList(GetTensorDimAt(RepList, i)));
		}

	}
	tmpRet.append("]");

	return tmpRet;
	
}

std::string ShowMapPair(const onnx::StringStringEntryProto& SSPair)
	{	return "(" + SSPair.key() + ", " + SSPair.key() + ")"; }

std::vector<google::protobuf::int64> GetTensorDimAt(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList, int n )
{
	
	if (n >= RepList.size() || n < 0)
	{
		return std::vector<google::protobuf::int64>();
	}
	const auto & TPList = RepList[n].type();

	if (TPList.value_case() != onnx::TypeProto::ValueCase::kTensorType)
	{
		return std::vector<google::protobuf::int64>();
	}
	auto & dimLst = TPList.tensor_type().shape().dim();


	std::vector<google::protobuf::int64> tmpret(dimLst.size());
	
	for (int i = 0; i< dimLst.size(); i++)
	{
		tmpret [i]= dimLst[i].dim_value();
	}


	return tmpret;
	//return std::vector<google::protobuf::int64>();
}


	std::string AddrTypetoString(const int& typeNum)
{
	static std::map<int, std::string> AttrStrMap =
	{ {0 ,"UNDEFINED" }
	,{1 ,"FLOAT" 	}
	,{2 ,"INT"}
	,{3 ,"STRING"}
	,{4 ,"TENSOR"}
	,{5 ,"GRAPH" }
	,{6 ,"FLOATS" 	}
	,{7 ,"INTS"			}
	,{8 ,"STRINGS"		}
	,{9 ,"TENSORS"		}
	,{10,"GRAPHS"		}
	,{11,"SPARSE_TENSOR"}
	,{12,"SPARSE_TENSORS"} };

	std::string RetStr;
	if (0 <= typeNum && typeNum <= 12)
	{
		RetStr = AttrStrMap[typeNum];
	}
	else
	{
		RetStr = "";
	}
	return RetStr;
}

ONNXModel ONNXRead(const std::string& OnnxPath)
{
	std::ifstream ONNX_Fstream(OnnxPath, std::ios::binary);
	if (ONNX_Fstream.is_open())
	{
		ONNX_Fstream.seekg(0, std::ios::end);
		std::size_t FileSize = ONNX_Fstream.tellg();
		ONNX_Fstream.seekg(0, std::ios::beg);
		
		
		char* buffer = new char[FileSize];
		ONNX_Fstream.read(buffer, FileSize);
		
		onnx::ModelProto * model = new onnx::ModelProto();
		onnx::ParseProtoFromBytes(model, buffer, FileSize);

		ONNXModel RetModel(model);

		delete[] buffer;
		buffer = nullptr;
		return RetModel;
	}
	else
	{
		return ONNXModel();
	}

}

ONNXModel ONNXRead(const std::string& Dir, const std::string& FileName)
{
	std::string tmpPath = Dir;
	if (*Dir.rbegin() != '\\' && *Dir.rbegin() != '/')
		tmpPath.append("\\");
	tmpPath.append(FileName);
	return ONNXRead(tmpPath);
}


ONNXModel::ONNXModel()	
	: _model(nullptr)
	, _status(-1)
{
}

ONNXModel::ONNXModel(onnx::ModelProto* model)
	: _model(model)
	, _status(0)
{	
	if (model == nullptr)
		_status = -1;
}

ONNXModel::ONNXModel(const ONNXModel& Modelobj)
	: _model(new onnx::ModelProto(*Modelobj._model))
	, _status(0)
{
}

ONNXModel::ONNXModel(ONNXModel&& Modelobj) noexcept
{
	 *this = std::move(Modelobj);
}

ONNXModel& ONNXModel::operator=(const ONNXModel& Modelobj)
{
	_model.reset(new onnx::ModelProto(*Modelobj._model));
	return *this;
}

ONNXModel& ONNXModel::operator=(ONNXModel&& Modelobj) noexcept
{
	_model = std::move(Modelobj._model);
	_status = Modelobj._status;
	return *this;
}

ONNXModel::~ONNXModel()
{

}

}


