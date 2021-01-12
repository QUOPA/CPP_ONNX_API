#pragma once
#include<string>
#include<vector>
#include<map>
#include<memory>
#include<fstream>
#include <sstream>


#include "onnx/onnx_pb.h"
#include "onnx/proto_utils.h"






namespace ECMONNXAPI
{

//class ONNXModel;


class ONNXModel
{
public:
	//ONNXModel();
	//ONNXModel(std::string OnnxPath);
	//ONNXModel
	ONNXModel();
	ONNXModel(onnx::ModelProto*);

	ONNXModel(const ONNXModel& );
	ONNXModel(ONNXModel && ) noexcept;

	ONNXModel& operator=(const ONNXModel&) ;
	ONNXModel& operator=(ONNXModel &&) noexcept;

	virtual ~ONNXModel();

	//int OpenONNXFile(std::string OnnxPath);
	//int CloseONNXFile();
	
	//std::string GetONNXPath();

	// API 제작 이전 사용을 위해 제공
	onnx::ModelProto* GetModel() { return _model.get(); }

	
	int GetStatus() { return _status; }


private:
	// good if 0, bad O/W
	int _status;		
	std::unique_ptr<onnx::ModelProto> _model;

};

/*
class AttributeManager
{

};
*/


template<typename T>
std::string ShowListNames(const google::protobuf::RepeatedPtrField<T>& RepList);

template<typename T>
std::string ShowList(const std::vector<T>& RepList);

template<typename T>
std::string ShowList(const google::protobuf::RepeatedPtrField<T>& RepList);

//template<typename T>
//std::string ShowListTypeProto(const google::protobuf::RepeatedPtrField<T>& RepList);

std::string ShowTypeProtoListTypes(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList);

std::string ShowDimOfListVIP(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList);

std::string ShowMapPair(const onnx::StringStringEntryProto& SSPair);

std::vector<google::protobuf::int64> GetTensorDimAt(const google::protobuf::RepeatedPtrField<onnx::ValueInfoProto>& RepList, int n);

std::string AddrTypetoString(const int& typeNum);


ONNXModel ONNXRead(const std::string& OnnxPath);


ONNXModel ONNXRead(const std::string& Dir, const std::string& FileName);


template<typename T>
std::string ShowListNames(const google::protobuf::RepeatedPtrField<T>& RepList)
{
	
	std::string tmpRet = "[";
	if (RepList.size() > 0)
	{
		tmpRet.append(RepList[0].name());
		for (int i = 1; i < RepList.size(); i++)
		{
			tmpRet.append(", ");
			tmpRet.append(RepList[i].name());
		}
		
	}
	tmpRet.append("]");
	

	return tmpRet;
}

template<typename T>
std::string ShowList(const std::vector<T>& RepList)
{

	//std::stringstream ss;
	
	std::string tmpRet = "[";

	if (RepList.size() > 0)
	{
		tmpRet.append(std::to_string(RepList[0]));
		for (int i = 1; i < RepList.size(); i++)
		{
			tmpRet.append(", ");
			tmpRet.append(std::to_string(RepList[i]));
		}

	}
	tmpRet.append("]");

	return tmpRet;
}


template<typename T>
std::string ShowList(const google::protobuf::RepeatedPtrField<T>& RepList)
{

	//std::stringstream ss;

	std::string tmpRet = "[";

	if (RepList.size() > 0)
	{
		tmpRet.append(std::to_string(RepList[0]));
		for (int i = 1; i < RepList.size(); i++)
		{
			tmpRet.append(", ");
			tmpRet.append(std::to_string(RepList[i]));
		}

	}
	tmpRet.append("]");

	return tmpRet;
}





}
